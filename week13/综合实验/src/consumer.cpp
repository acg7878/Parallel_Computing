#include "common.hpp"

int main() {
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::endpoint endpoint(tcp::v4(), CONSUMER_PORT);

    socket.connect(endpoint);

    char buffer[BUFFER_SIZE];
    while (true) {
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(buffer), ec);

        if (ec == boost::asio::error::eof) break;
        if (ec) throw boost::system::system_error(ec);

        std::string message_data(buffer, length);
        Message msg = Message::deserialize(message_data);

        int64_t number = std::stoll(msg.body);
        bool prime = isPrime(number);

        std::cout << "Message ID: " << msg.id
                  << ", Timestamp: " << msg.timestamp
                  << ", Tag: " << msg.tag
                  << ", Body: " << msg.body
                  << " --> " << (prime ? "Prime" : "Not Prime") << std::endl;
    }

    return 0;
}
