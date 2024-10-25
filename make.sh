#!/bin/bash

# 配置系统环境
project_name="dev-journal"
platform="linux" #[linux|qnx]
architecture="x86" #[x86|arm]
build_type="test" #[release|debug|test]
# 加载脚本
source ./script_tool/utils.sh

# 判断脚本执行的任务
if [ -z "$1" ]; then
    operation="build"
else
    case "$1" in
        "build")
            operation="build"
            ;;
        "run")
            operation="run"
            ;;
        "pack")
            operation="pack"
            ;;
        "clear")
            operation="clear"
            ;;
        "launch")
            operation="launch"
            ;;
        *)
            echo -e "${RED}Invalid parameter: $1${NC}. Use [build|run|pack|clear|launch|other:invalid]"
            exit 1
            ;;
    esac
fi

case "$operation" in
    "clear")
        # 清理工程
        rm -rf ./build*
        exit 1
    ;;
    "run")
        # 不编译，直接运行
        cd ./build_x86
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
        ./bin/main_run
        exit 1
    ;;
    "pack")
    ;;
esac


# 根据配置选项创建build文件夹,配置编译选项
case "$platform" in
    "linux")
        case "$architecture" in
            "x86")
                setup_build_dir "./build_x86"
                compiler_root_path="/usr"
                echo "cmake_find_root_path ${compiler_root_path}" >> ${local_file}
                echo "cmake_c_compiler ${compiler_root_path}/bin/gcc-9" >> ${local_file}
                echo "cmake_cxx_compiler ${compiler_root_path}/bin/g++-9" >> ${local_file}
                ;;
            "arm")
                setup_build_dir "./build_arm"
                compiler_root_path="/opt/aarch64--glibc--stable-2020.08-1"
                echo "cmake_find_root_path ${compiler_root_path}" >> ${local_file}
                echo "cmake_c_compiler ${compiler_root_path}/bin/aarch64-buildroot-linux-gnu-gcc" >> ${local_file}
                echo "cmake_cxx_compiler ${compiler_root_path}/bin/aarch64-buildroot-linux-gnu-g++" >> ${local_file}
                ;;
            *)
                echo -e "${RED}$1 unsupported architecture${NC}"
                exit 1
                ;;
        esac
        ;;
    "qnx")
        setup_build_dir "./build_qnx"
        compiler_root_path="/opt/QNX_8155/qnx700_121c1/host/linux/x86_64/usr"
        echo "cmake_find_root_path ${compiler_root_path}" >> ${local_file}
        echo "cmake_c_compiler ${compiler_root_path}/bin/aarch64-unknown-nto-qnx7.0.0-gcc" >> ${local_file}
        echo "cmake_cxx_compiler ${compiler_root_path}/bin/aarch64-unknown-nto-qnx7.0.0-g++" >> ${local_file}
        source /opt/QNX_8155/qnx700_121c1/qnxsdp-env.sh
        ;;
    *)
        echo -e "${RED}$1 unsupported platform${NC}"
        exit 1
        ;;
esac

# 配置本地编译配置文件
echo "project ${project_name}" >> ${local_file}
echo "platform  ${platform}" >> ${local_file}
echo "architecture ${architecture}" >> ${local_file}
echo "build_type ${build_type}" >> ${local_file}

# 配置install路径
cmake -DCMAKE_INSTALL_PREFIX=$PWD ..
getCpuNum
echo "Adjusted CPU number: $cpu_num"

# 编译
cmake .
ExitIfCheckFail "cmake"
make -j$cpu_num
ExitIfCheckFail "make"
make install
ExitIfCheckFail "install"
cp ./compile_commands.json ../cmake
cp ../script_tool/run.sh ./

# 运行 | 打包
case "$operation" in
    "launch")
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
        ./bin/main_run
    ;;
    "pack")
        cd ..
        rm ./*.zip
        output_dir="/mnt/d/desktop/"  # 目标文件夹
        mkdir -p "$output_dir"  # 创建目标文件夹（如果不存在）
        zip -r "${output_dir}/${project_name}_${platform}_${build_type}_$(date +'%Y%m%d').zip" "$build_dir"
    ;;

esac

echo "Done"
