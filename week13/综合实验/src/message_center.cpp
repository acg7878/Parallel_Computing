#include "common.hpp"
#include <boost/asio.hpp>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

// 消息队列
std::queue<Message> message_queue;
std::mutex queue_mutex;
std::condition_variable queue_cv;

// 注册的消费者列表
std::set<std::shared_ptr<tcp::socket>> consumers;
std::mutex consumers_mutex;

// 处理生产者的消息
void handle_producer(tcp::socket producer_socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        boost::system::error_code ec;
        size_t length = producer_socket.read_some(boost::asio::buffer(buffer), ec);

        if (ec == boost::asio::error::eof) {
            std::cout << "Producer disconnected." << std::endl;
            break;
        }
        if (ec) {
            std::cerr << "Error receiving message from producer: " << ec.message() << std::endl;
            break;
        }

        std::string message_data(buffer, length);
        Message msg = Message::deserialize(message_data);

        // 将消息存入队列
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            message_queue.push(msg);
            std::cout << "Received message from producer. ID: " << msg.id << std::endl;
        }
        queue_cv.notify_all(); // 唤醒等待的消费者线程
    }
}

// 处理单个消费者的连接
void handle_consumer_socket(std::shared_ptr<tcp::socket> consumer_socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        boost::system::error_code ec;
        size_t length = consumer_socket->read_some(boost::asio::buffer(buffer), ec);

        if (ec == boost::asio::error::eof) {
            std::cout << "Consumer disconnected." << std::endl;
            break;
        }
        if (ec) {
            std::cerr << "Error receiving message from consumer: " << ec.message() << std::endl;
            break;
        }

        std::string message_data(buffer, length);
        Message msg = Message::deserialize(message_data);
        std::cout << "Received ack from consumer: " << msg.body << std::endl;
    }
}

// 广播消息给所有消费者
void broadcast_message(const Message &msg) {
    std::lock_guard<std::mutex> lock(consumers_mutex);
    for (const auto &consumer_socket : consumers) {
        boost::system::error_code ec;
        boost::asio::write(*consumer_socket, boost::asio::buffer(msg.serialize()), ec);
        if (ec) {
            std::cerr << "Error sending message to consumer: " << ec.message() << std::endl;
        }
    }
}

// 处理消费者注册
void handle_consumer(tcp::acceptor &consumer_acceptor) {
    while (true) {
        auto consumer_socket = std::make_shared<tcp::socket>(consumer_acceptor.get_executor());
        consumer_acceptor.accept(*consumer_socket);

        {
            std::lock_guard<std::mutex> lock(consumers_mutex);
            consumers.insert(consumer_socket);
        }

        std::cout << "Consumer registered." << std::endl;

        // 启动线程单独处理该消费者
        std::thread(handle_consumer_socket, consumer_socket).detach();
    }
}

// 处理队列消息并广播
void process_messages() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // 等待消息或超时
        queue_cv.wait(lock, [] { return !message_queue.empty(); });

        if (message_queue.empty()) {
            continue;
        }

        Message msg = message_queue.front();
        message_queue.pop();
        lock.unlock();

        broadcast_message(msg); // 广播消息
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor producer_acceptor(io_context,
                                        tcp::endpoint(tcp::v4(), PRODUCER_PORT));
        tcp::acceptor consumer_acceptor(io_context,
                                        tcp::endpoint(tcp::v4(), CONSUMER_PORT));

        std::vector<std::thread> threads;

        // 启动消费者处理和消息处理线程
        threads.emplace_back(std::thread(handle_consumer, std::ref(consumer_acceptor)));
        threads.emplace_back(std::thread(process_messages));

        // 持续接受生产者连接
        while (true) {
            tcp::socket producer_socket(io_context);
            producer_acceptor.accept(producer_socket);

            // 为每个生产者启动一个新线程
            threads.emplace_back(std::thread(handle_producer, std::move(producer_socket)));
        }

        // 等待所有线程完成
        for (auto &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}