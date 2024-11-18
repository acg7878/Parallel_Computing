#!/bin/bash

# 定义源文件和可执行文件
SRC_DIR="src"
BUILD_DIR="build"
MESSAGE_CENTER_SRC="$SRC_DIR/MessageCenter.cpp"
CONSUMER_SRC="$SRC_DIR/Consumer.cpp"
PRODUCER_SRC="$SRC_DIR/Producer.cpp"
MESSAGE_CENTER_EXEC="$BUILD_DIR/message_center"
CONSUMER_EXEC="$BUILD_DIR/consumer"
PRODUCER_EXEC="$BUILD_DIR/producer"

# 确保 build 文件夹存在
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    echo "Created build directory: $BUILD_DIR"
fi

# 询问是否需要编译
read -p "Do you want to compile the source files? (y/n): " COMPILE_CHOICE
if [[ "$COMPILE_CHOICE" =~ ^[Yy]$ ]]; then
    echo "Compiling source files..."

    # 编译消息中心
    g++ -o "$MESSAGE_CENTER_EXEC" "$MESSAGE_CENTER_SRC" -std=c++17 -pthread
    if [ $? -eq 0 ]; then
        echo "Compiled: message_center -> $MESSAGE_CENTER_EXEC"
    else
        echo "Error compiling message_center."
        exit 1
    fi

    # 编译消费者
    g++ -o "$CONSUMER_EXEC" "$CONSUMER_SRC" -std=c++17 -pthread
    if [ $? -eq 0 ]; then
        echo "Compiled: consumer -> $CONSUMER_EXEC"
    else
        echo "Error compiling consumer."
        exit 1
    fi

    # 编译生产者
    g++ -o "$PRODUCER_EXEC" "$PRODUCER_SRC" -std=c++17 -pthread
    if [ $? -eq 0 ]; then
        echo "Compiled: producer -> $PRODUCER_EXEC"
    else
        echo "Error compiling producer."
        exit 1
    fi
fi

# 按顺序运行程序
echo "Starting the system..."
echo "1. Launching Message Center..."
"$MESSAGE_CENTER_EXEC" &
MESSAGE_CENTER_PID=$!
sleep 1

echo "2. Launching Consumer..."
"$CONSUMER_EXEC" &
CONSUMER_PID=$!
sleep 1

echo "3. Launching Producer..."
"$PRODUCER_EXEC" &
PRODUCER_PID=$!

# 等待所有程序运行完成
wait $MESSAGE_CENTER_PID $CONSUMER_PID $PRODUCER_PID
echo "All processes completed."

# 脚本结束
exit 0
