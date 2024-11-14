#!/bin/bash

# 检查是否需要重新编译
echo -n "Do you want to recompile all files? (y/n): "
read recompile_choice

# 检查是否已经编译过可执行文件，如果没有则进行编译
check_and_compile() {
    for dir in consumerClient producerClient queueServer; do
        # 获取每个子目录的可执行文件名
        mkdir ${dir}/build
        exec_file="${dir}/build/${dir}"

        # 如果选择重新编译，或没有可执行文件，则进行编译
        if [ "$recompile_choice" = "y" ] || [ ! -f "$exec_file" ]; then
            echo "Compiling $dir..."
            g++ -o "$exec_file" "$dir"/*.cpp
            if [ $? -ne 0 ]; then
                echo "Compilation of $dir failed."
                exit 1
            fi
            echo "$dir compiled successfully."
        else
            echo "$exec_file already exists."
        fi
    done
}

# 启动队列服务器
start_queue_server() {
    echo "Starting queue server..."
    ./queueServer/build/squeueServer &
    # 给服务器一些时间启动
    sleep 1
}

# 启动生产者
start_producer() {
    local num_producers=$1
    echo "Starting $num_producers producers..."
    for ((i=0; i<num_producers; i++)); do
        ./producerClient/build/producerClient &
    done
}

# 启动消费者
start_consumer() {
    local num_consumers=$1
    echo "Starting $num_consumers consumers..."
    for ((i=0; i<num_consumers; i++)); do
        ./consumerClient/build/consumerClient &
    done
}

# 运行脚本
check_and_compile

# 获取用户输入的生产者和消费者数量
echo -n "Enter the number of producers: "
read num_producers

echo -n "Enter the number of consumers: "
read num_consumers

# 启动服务端
start_queue_server

# 启动指定数量的生产者和消费者
start_producer $num_producers
start_consumer $num_consumers
