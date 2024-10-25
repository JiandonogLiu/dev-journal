

#include <iostream>
#include <cstring>
#include "file_manager.h"

int main()
{
    // 测试文件名
    const std::string filename = "testfile.bin";

    // 创建 FileManager 对象
    FileManager file_manager(filename);

    // 测试写入数据
    const char write_data[] = "Hello, POSIX!";
    if (file_manager.save_file(write_data, sizeof(write_data)))
    {
        std::cout << "数据写入成功: " << write_data << std::endl;
    }
    else
    {
        std::cerr << "数据写入失败！" << std::endl;
        return 1;
    }

    // 测试从文件读取数据
    char read_data[20];
    memset(read_data, 0, sizeof(read_data));
    if (file_manager.reset_read_file() &&
        file_manager.read_file(read_data, sizeof(write_data)))
    {
        std::cout << "数据读取成功: " << read_data << std::endl;
    }
    else
    {
        std::cerr << "数据读取失败！" << std::endl;
        return 1;
    }

    // 测试跳过字节功能
    const int skip_size = 7;
    if (file_manager.reset_read_file() && file_manager.read_file_skip_bytes(skip_size))
    {
        memset(read_data, 0, sizeof(read_data));
        if (file_manager.read_file(read_data, sizeof(write_data) - skip_size))
        {
            std::cout << "跳过 " << skip_size << " 字节后读取的数据: " << read_data
                      << std::endl;
        }
        else
        {
            std::cerr << "跳过字节后的数据读取失败！" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "跳过字节失败！" << std::endl;
        return 1;
    }

    return 0;
}