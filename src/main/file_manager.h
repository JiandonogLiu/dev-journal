/**
 * @file fvfile_type.h
 * @brief FileManager类用于管理文件的读写操作。
 * @author jiandong
 * @version 0.1
 * @date 2024-09-12
 *
 * @copyright Copyright (c) 2024 forvision
 *
 * @par Change Log:
 * <table>
 * <tr><th>Date       <th>Version  <th>Author       <th>Description
 * <tr><td>2024-09-12 <td>1.0      <td>jiandong.liu     <td>first version
 * </table>
 */

#include <fcntl.h>     // open()
#include <unistd.h>    // read(), write(), close()
#include <string>      // std::string
#include <cstring>     // strerror()
#include <sys/types.h> // ssize_t

/**
 * @brief FileManager类用于处理文件的读写操作
 * @note 读写分别维护一个文件描述符,文件会在析构时被删除
 */
class FileManager
{
  public:
    /**
     * @brief 构造函数，使用指定的文件名初始化FileManager
     * @param filename 文件名
     */
    explicit FileManager(const std::string &filename) : filename_(filename)
    {
        write_fd_ = open(filename_.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        read_fd_  = open(filename_.c_str(), O_RDONLY);
    }

    // 禁用拷贝构造和赋值操作
    FileManager(const FileManager &)            = delete;
    FileManager &operator=(const FileManager &) = delete;

    /**
     * @brief 移动构造函数
     * @param other 被移动的FileManager对象
     */
    FileManager(FileManager &&other) noexcept
        : filename_(std::move(other.filename_)),
          write_fd_(other.write_fd_),
          read_fd_(other.read_fd_)
    {
        other.write_fd_ = -1;
        other.read_fd_  = -1;
    }

    FileManager &operator=(FileManager &&other) noexcept
    {
        if (this != &other)
        {
            if (write_fd_ != -1)
            {
                close(write_fd_);
            }
            if (read_fd_ != -1)
            {
                close(read_fd_);
            }
            filename_       = std::move(other.filename_);
            write_fd_       = other.write_fd_;
            read_fd_        = other.read_fd_;
            other.write_fd_ = -1;
            other.read_fd_  = -1;
        }
        return *this;
    }

    ~FileManager()
    {
        if (write_fd_ != -1)
        {
            close(write_fd_);
        }
        if (read_fd_ != -1)
        {
            close(read_fd_);
        }
        remove(filename_.c_str());
    }

    /**
     * @brief 将数据保存到文件
     * @param data 指向要保存的数据的指针
     * @param size 数据大小
     * @return 如果保存成功则返回true，否则返回false
     */
    bool save_file(const void *data, size_t size) const
    {
        if (write_fd_ == -1)
        {
            return false;
        }
        ssize_t bytes_written = write(write_fd_, data, size);
        return bytes_written == static_cast<ssize_t>(size);
    }

    /**
     * @brief 从文件中读取数据
     * @param data 指向用于接收数据的指针
     * @param size 读取数据的大小
     * @return 如果读取成功则返回true，否则返回false
     */
    bool read_file(void *data, size_t size) const
    {
        if (read_fd_ == -1)
        {
            return false;
        }
        ssize_t bytes_read = read(read_fd_, data, size);
        return bytes_read == static_cast<ssize_t>(size);
    }

    /**
     * @brief 跳过文件中的特定大小的数据
     * @param size 要跳过的数据大小
     * @return 如果操作成功则返回true，否则返回false
     */
    bool read_file_skip_bytes(size_t size) const
    {
        if (read_fd_ == -1)
        {
            return false;
        }
        off_t result = lseek(read_fd_, size, SEEK_CUR);
        return result != -1;
    }

    /**
     * @brief 重新设置读取文件的指针指向文件头
     * @return 如果操作成功则返回true，否则返回false
     */
    bool reset_read_file()
    {
        if (read_fd_ != -1)
        {
            close(read_fd_);
        }
        read_fd_ = open(filename_.c_str(), O_RDONLY);
        return read_fd_ != -1;
    }

  private:
    std::string filename_;      ///< 文件名
    mutable int write_fd_ = -1; ///< 用于写操作的文件描述符
    mutable int read_fd_  = -1; ///< 用于读操作的文件描述符
};