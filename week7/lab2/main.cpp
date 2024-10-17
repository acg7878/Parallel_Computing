#include <iostream>

#include "my_thread.h"

int main() {
    ThreadManager threadManager;

    // 用户选择实验
    int choice = 0;
    std::cout << "选择实验:\n";
    std::cout << "1. 使用1个生产者生成100万个数字，1个消费者判断素数。\n";
    std::cout << "2. 使用1个生产者生成100万个数字，2个消费者判断素数。\n";
    std::cout << "3. 使用2个生产者生成100万个数字，4个消费者判断素数。\n";
    std::cout << "输入实验编号(1, 2, 或 3): ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::cout << "运行实验1: 1个生产者，1个消费者...\n";
            threadManager.run(1, 1, 1000000);
            break;
        case 2:
            std::cout << "运行实验2: 1个生产者，2个消费者...\n";
            threadManager.run(1, 2, 1000000);
            break;
        case 3:
            std::cout << "运行实验3: 2个生产者，4个消费者...\n";
            threadManager.run(2, 4, 1000000);
            break;
        default:
            std::cout << "无效输入，请重新运行程序。\n";
            break;
    }

    return 0;
}