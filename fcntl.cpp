#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <thread>

class My_error final : public std::exception
{
public:
    explicit My_error(const std::string &&message) noexcept : message{std::move(message)} {}
    const char *what() const noexcept override { return message.c_str(); } // message
private:
    std::string message; // message
};

void writeFile(int fd, char *buf, size_t data_write, int buffer_size)
{
    long count = 10;
    int temp = data_write;
    while (count != 0)
    {
        data_write = temp;
        for (; data_write != 0; data_write - buffer_size)
        {
            size_t bytes_to_write = buffer_size;
            for (int bytes_written = 0; bytes_written < bytes_to_write;)
            {

                int currently_written = write(fd, buf + bytes_written, bytes_to_write - bytes_written); // write
                if (currently_written == -1)
                {
                    throw My_error("[ERROR] Cannot write file!");
                }
                bytes_written += currently_written;
            }
            data_write -= buffer_size;
            sleep(3);
        }
        count--;
    }
}

void printF(int i)
{
    std::cout << "HIII"<<std::endl;
}

int main()
{
    size_t buffer_size = 4096;
    size_t data_write = 100;
    std::unique_ptr<char[]> buf(new char[buffer_size]);
    std::unique_ptr<char[]> buf2(new char[buffer_size]);

    std::string str = "[WARN] HACK ATEMPT: 123.123.123.123 port 80 (connection refused)\n";
    std::string str2 = "[ACESS] INPUT: 12.113.13.100 port 80 (connection OK)\n";

    for (int i = 0; i < str.length(); i++)
    {
        buf[i] = str[i];
        std::cout << buf[i] << "\t";
    }

    for (int i = 0; i < str2.length(); i++)
    {
        buf2[i] = str2[i];
        std::cout << buf2[i] << "\t";
    }

    std::cout << "\n";

    try
    {
        int fd1 = open("data.txt", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0777);
        std::cout << "fd: " << fd1 << std::endl;
        std::cout << "lseek: " << lseek(fd1, 0, SEEK_END) << std::endl;

        // int numb = 10;
        int size = str.length();
        std::thread t1(writeFile, std::ref(fd1), buf.get(), std::ref(size),std::ref(size));
        t1.detach();
        // writeFile(fd1, buf.get(), str.length(), str.length());
        // std::cout << "lseek: " << lseek(fd1, 0, SEEK_END) << std::endl;

        int fd2 = open("data.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
        writeFile(fd2, buf2.get(), str2.length(), str2.length());
        std::cout << "fd: " << fd1 << std::endl;
        std::cout << "lseek: " << lseek(fd2, 0, SEEK_END) << std::endl;

        system("cat ./data.txt");
        std::cout << "\n";

        int close_fd1 = close(fd1);
        if (close_fd1 == -1)
        {
            std::cout << "[ERROR] Cannot close file1!" << std::endl;
        }
        int close_fd2 = close(fd2);
        if (close_fd2 == -1)
        {
            std::cout << "[ERROR] Cannot close file2!" << std::endl;
        }
        // t1.join();

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // int fd = open("data", O_WRONLY | O_CREAT | O_SYNC | O_APPEND, 0755);
    // int flags = fcntl(fd, F_GETFL);
    // flags |= O_APPEND;
    // fcntl(fd, F_SETFL, flags);
    // std::cout << flags << std::endl;

    // if (flags & O_APPEND)
    // {
    //     std::cout << "добавление данных" << std::endl;
    // }

    // if (flags & O_SYNC)
    // {
    //     std::cout << "Записи синхронизированы" << std::endl;
    // }

    // int access_mode = flags & O_ACCMODE;

    // if (access_mode = O_WRONLY)
    // {
    //     std::cout << "Только запись" << std::endl;
    // }
}