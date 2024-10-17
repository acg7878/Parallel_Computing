#include "my_thread.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "buffer.h"
#include "consumer.h"
#include "producer.h"

void ThreadManager::run(int numProducers, int numConsumers, int numToGenerate) {
    Buffer buffer;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    Producer producer;
    Consumer consumer;

    auto start = std::chrono::high_resolution_clock::now();

    // 启动生产者线程
    for (int thread_id = 0; thread_id < numProducers; thread_id++) {
        producers.emplace_back(&Producer::random_product, &producer,
                               std::ref(buffer), numProducers, thread_id);
    }
    // 启动消费者线程
    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back(&Consumer::isPrimeNumber, &consumer,
                               std::ref(buffer));
    }

    // 等待所有生产者完成
    for (auto& p : producers) {
        p.join();
    }
    // 等待所有消费者完成
    for (auto& c : consumers) {
        c.join();
    }

    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "耗时：" << elapsed.count() << " seconds." << std::endl;
}
