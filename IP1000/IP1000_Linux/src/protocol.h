#ifndef __PROTOCOL__H__
#define __PROTOCOL__H__

#include "nrcAPI.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <iomanip>
#include <chrono>
#include <poll.h>
class SerialPort {
public:
    SerialPort(const std::string& port, unsigned int baud)
        : fd_(-1)
    {
        fd_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (fd_ < 0) {
            perror("[Error] open serial port");
            return;
        }

        termios tty;
        if (tcgetattr(fd_, &tty) != 0) {
            perror("[Error] tcgetattr");
            close(fd_);
            fd_ = -1;
            return;
        }

        // 设置波特率
        speed_t speed;
        switch (baud) {
            case 115200: speed = B115200; break;
            case 57600:  speed = B57600;  break;
            case 38400:  speed = B38400;  break;
            case 19200:  speed = B19200;  break;
            case 9600:   speed = B9600;   break;
            default:     speed = B115200; break;
        }
        cfsetispeed(&tty, speed);
        cfsetospeed(&tty, speed);

        // 8N1
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag |= CREAD | CLOCAL;

        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_oflag &= ~OPOST;

        tty.c_cc[VMIN]  = 0; // 非阻塞
        tty.c_cc[VTIME] = 0;

        if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
            perror("[Error] tcsetattr");
            close(fd_);
            fd_ = -1;
            return;
        }

        //int status = 0;
        //if (ioctl(fd_, TIOCMGET, &status) == 0) {
        //    status &= ~TIOCM_RTS;  // 关闭 RTS
        //    status &= TIOCM_DTR;
        //    ioctl(fd_, TIOCMSET, &status);
        //}

        //ioctl(fd_, TIOCMSET, &status);

        tcflush(fd_, TCIOFLUSH);

        std::cout << "[Init] " << port << " opened @" << baud << "bps\n";
    }

    ~SerialPort() {
        if (fd_ >= 0)
            close(fd_);
    }


    size_t Read(uint8_t* buf, size_t max_len, int timeout_ms = 10)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (fd_ < 0) return 0;

        struct pollfd fds;
        fds.fd = fd_;
        fds.events = POLLIN;
        size_t total_read = 0;

        int ret = poll(&fds, 1, timeout_ms);
        if (ret > 0 && (fds.revents & POLLIN)) {
            ssize_t n = read(fd_, buf, max_len);
            if (n > 0) total_read = n;
            else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
                perror("[ReadError]");
        }

        return total_read;
    }

    // 写数据
    void Write(const uint8_t* data, size_t len)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (fd_ < 0) return;

        ssize_t total_written = 0;
        while (total_written < (ssize_t)len)
        {
            ssize_t n = write(fd_, data + total_written, len - total_written);
            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
                perror("[WriteError]");
                break;
            }
            total_written += n;
        }

        // 不再 tcflush(fd_, TCIFLUSH);
        tcdrain(fd_);  // 确保数据发送完成
    }


// 专门的方法来清空接收缓冲区
void ClearInputBuffer() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (fd_ >= 0) {
        tcflush(fd_, TCIFLUSH);  // 只清空输入缓冲区
    }
}

// 清空输出缓冲区
void ClearOutputBuffer() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (fd_ >= 0) {
        tcflush(fd_, TCOFLUSH);  // 只清空输出缓冲区
    }
}

// 清空所有缓冲区
void ClearAllBuffers() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (fd_ >= 0) {
        tcflush(fd_, TCIOFLUSH);  // 清空输入和输出缓冲区
    }
}


private:
    int fd_;
    std::mutex mtx_;
};

#endif

