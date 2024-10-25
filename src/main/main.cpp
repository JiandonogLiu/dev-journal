#include <stdio.h>  // printf()
#include <unistd.h> // write()
#include <iostream> // std::cout

int main(int argc, char *argv[])
{
    // 使用 printf 输出参数
    printf("argv[0]: %s\nargv[1]: %s\nargv[2]: %s\n", argv[0], argv[1], argv[2]);

    // 使用 std::cout 输出参数,不符合POSIX标准
    std::cout << "argv[0]: " << argv[0] << "\n"
              << "argv[1]: " << argv[1] << "\n"
              << "argv[2]: " << argv[2] << std::endl;

    // 使用 write 输出参数
    const char *message = "argv[0]: %s\nargv[1]: %s\nargv[2]: %s\n";
    dprintf(STDOUT_FILENO, message, argv[0], argv[1], argv[2]);

    return 0;
}
