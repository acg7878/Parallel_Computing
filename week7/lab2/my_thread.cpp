#include "my_thread.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "buffer.h"
#include "consumer.h"
#include "producer.h"

void run(int numProducers, int numConsumers, int numToGenerate) {
    Buffer buffer;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    auto start = std::chrono::high_resolution_clock::now();

    // 启动生产者线程
    for (int i = 0; i < numProducers; ++i) {
    }

    // 启动消费者线程
    for (int i = 0; i < numConsumers; ++i) {
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

    std::cout << "Experiment with " << numProducers << " producer(s) and "
              << numConsumers << " consumer(s) took " << elapsed.count()
              << " seconds." << std::endl;
}
