#!/bin/bash

# 配置运行选项
core_dump_storage=1  # [0:OFF|1:ON]
DUMP_DIR=~/dump

# 当前脚本目录
CURRENT_SCRIPT_DIR=$(dirname $(realpath $0))
echo "Current DIR: $CURRENT_SCRIPT_DIR"

# 获取 main_run 的 PID
PID=`top -b -n 1 | grep main_run | grep -v grep | awk '{print $1}'`
if [[ "$PID" != "" ]]; then
    echo "Current PID: $PID"
    echo "Stopping existing process with PID: $PID"
    sudo kill -9 $PID
fi

# 转储目录
if [[ "$core_dump_storage" != 1 ]]; then
    ulimit -c unlimited
    mkdir -p $DUMP_DIR
    sudo sysctl -w kernel.core_pattern=$DUMP_DIR/core-%e.%p.%h.%t
    echo "core dump dir : $DUMP_DIR"
fi


# 启动 main_run 并传入参数
echo "Starting main_run app with parameters: $@"
echo "____________________Run_____________________"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/
./bin/main_run "$@"