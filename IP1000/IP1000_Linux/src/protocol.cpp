#include "protocol.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <string.h>
#include <mutex>

std::mutex serial_mutex;
static int STM32_Din_IO_Buffer[16] = {0};
static int STM32_Dout_IO_Buffer[16] = {0};

#define VERSION 0x0A

SerialPort Serial_User("/dev/ttyS2", 115200);
SerialPort Serial_STM("/dev/ttyS1", 115200);
uint8_t return_data_Ok[7] = {0xfe, 0xfe, 0x04, 0x03, 0x01, 0xCD, 0XBC};
uint8_t return_data_Error[7] = {0xfe, 0xfe, 0x04, 0x03, 0x00, 0x0D, 0X7D};
uint8_t TX_buf[128] = {0};
uint8_t RX_buf[128] = {0};
bool Ispower_On = false;
uint8_t version_array[7] = {0xfe, 0xfe, 0x04, 0x04, VERSION, 0x3A, 0XFF};

struct GPIO_Status
{
    uint8_t IO_Statis[16];
};

struct GPIO_Status gpio_status = {0};

void Run()
{
    while (true)
    {
        std::lock_guard<std::mutex> lock(serial_mutex); // 加锁
        uint8_t User_len = Serial_User.Read(TX_buf, sizeof(TX_buf));

        if (User_len > 0)
        {

            for (int k = 0; k < User_len; ++k)
            {
                std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                          << (int)TX_buf[k] << " ";
            }
            std::cout << std::dec << std::endl;

            for (int i = 0; i < User_len; i++)
            {
                if (TX_buf[i] == 0xFE && TX_buf[i + 1] == 0xFE)
                {

                    uint8_t cmd_len = TX_buf[i + 2];
                    uint8_t cmd = TX_buf[i + 3];
                    uint8_t status = TX_buf[i + 5];

                    uint8_t CRC1 = TX_buf[cmd_len + 1];
                    uint8_t CRC2 = TX_buf[cmd_len + 2];

                    uint8_t temp[cmd_len + 1];
                    memcpy(temp, TX_buf + i, cmd_len + 1); // 将命令进行校验

                    uint16_t crc = Check_CRC_Serial(temp, cmd_len + 1, 1);

                    if (CRC1 == ((crc >> 8) & 0xFF) && CRC2 == (crc & 0xFF))
                    {
                        switch (TX_buf[i + 3])
                        {
                            case 1:
                            {
                                uint8_t IO = TX_buf[i + 4];
                                NRC_DigOutByBoard(2, IO, status);
                                gpio_status.IO_Statis[IO - 1] = status;

                                Serial_STM.Write(TX_buf, User_len);
                                break;
                            }

                            case 2: 
                            {
                                Serial_STM.Write(TX_buf, User_len);
                                break;
                            }
                        }
                    }
                }
                break;
            }

            uint8_t STM_len = Serial_STM.Read(RX_buf, sizeof(RX_buf));
            if (STM_len > 0)
            {
                for (int i = 0; i < sizeof(RX_buf); i++)
                {
                    if (RX_buf[i] == 0xFE && RX_buf[i + 1] == 0xFE && RX_buf[i + 3] == 0x02)
                    {
                        uint8_t IO = RX_buf[i + 4];
                        uint8_t status = RX_buf[i + 5];
                        NRC_DigIn_Set(IO, status);
                    }
                }

                Serial_User.Write(RX_buf, STM_len);
            }
            memset(TX_buf, 0, sizeof(TX_buf));
            memset(RX_buf, 0, sizeof(RX_buf));
        }

        // 避免空转高占用
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
}

int gpio_read_value(int gpio)
{
    char path[64];
    char value;
    int fd;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open failed");
        return -1; // 打开失败
    }

    if (read(fd, &value, 1) != 1)
    {
        perror("read failed");
        close(fd);
        return -1; // 读取失败
    }

    close(fd);

    // 返回状态：0 或 1
    return (value == '0') ? 0 : 1;
}

uint16_t Check_CRC_Serial(uint8_t *data, uint16_t length, uint8_t is_high_first)
{
    uint16_t temp = 0xFFFF;
    uint16_t i, k = 0;

    while (k < length) // 要计算总字节数
    {
        temp ^= data[k]; // 输入要检验字节，异或运算结果
        i = 0;
        while (i < 8) // 单字节位移数
        {
            if ((temp & 0x01) == 0) // 检查运算结果最后位是否为零
            {
                temp >>= 1; // 向右移一位
            }
            else
            {
                temp >>= 1;     // 向右    // NRC_DigOutByBoard(2, pin, status);移一位
                temp ^= 0xA001; // 异或运算结果
            }
            i++;
        }
        k++; // 下一个校验字节
    }

    if (!is_high_first)
        temp = (temp >> 8) | (temp << 8);

    return temp;
}

void PrintHex(const std::string &tag, const uint8_t *data, size_t len)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *ptm = std::localtime(&now);

    char time_buf[16];
    std::strftime(time_buf, sizeof(time_buf), "%H:%M:%S", ptm);

    std::cout << "[" << time_buf << "] " << tag << " ";

    for (size_t i = 0; i < len; i++)
        std::cout << std::uppercase << std::hex << std::setw(2)
                  << std::setfill('0') << (int)data[i] << " ";

    std::cout << std::dec << std::endl;
}

void Config_STM32F407_GPIO(uint8_t num)
{
    NRC_EnableCustomIOFunction();

    NRC_SetCustomIODoutConfig(num, STM32_Dout_IO_Buffer); // 16：指虚拟IO板的Dout端口初始化8个端口
    NRC_SetCustomIODinConfig(num, STM32_Din_IO_Buffer);   // 16：指虚拟IO板的Din端口初始化8个端口
}
void Set_Virtual_IO_Status(uint8_t pin, uint8_t status)
{
    uint8_t temp[6] = {0xFE, 0xFE, 0x05, 0x01, pin, status};
    uint8_t Send_Buffer[8] = {0xFE, 0xFE, 0x05, 0x01, pin, status, 0x00, 0x00};
    volatile uint16_t crc = Check_CRC_Serial(temp, sizeof(temp), 1);

    Send_Buffer[6] = (crc >> 8) & 0xFF;
    Send_Buffer[7] = crc & 0xFF;

    Serial_STM.Write(Send_Buffer, sizeof(Send_Buffer));
    for (int k = 0; k < sizeof(Send_Buffer); ++k)
    {
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << (int)Send_Buffer[k] << " ";
    }
    std::cout << std::dec << std::endl;
    Serial_STM.ClearAllBuffers();
}

void Get_Virtual_IO_Status(uint8_t pin)
{
    uint8_t Recv_Buffer[8] = {0};

    uint8_t temp[5] = {0xFE, 0xFE, 0x04, 0x02, pin};
    uint8_t Send_Buffer[7] = {0xFE, 0xFE, 0x04, 0x02, pin, 0x00, 0x00};
    volatile uint16_t crc = Check_CRC_Serial(temp, sizeof(temp), 1);
    Send_Buffer[5] = (crc >> 8) & 0xFF;
    Send_Buffer[6] = crc & 0xFF;
    Serial_STM.Write(Send_Buffer, sizeof(Send_Buffer));

    Serial_STM.Read(Recv_Buffer, sizeof(Recv_Buffer));

    Handle_Recv_Buffer(Recv_Buffer);
}

void Handle_Recv_Buffer(uint8_t *data)
{
    for (int i = 0; i < sizeof(data); i++)
    {
        if (data[i] == 0xfe && data[i + 1] == 0xfe && data[i + 3] == 0x02)
        {
            uint8_t cmd_len = data[i + 2];
            volatile uint8_t status = data[i + 4];

            uint8_t CRC1 = data[i + cmd_len + 1];
            uint8_t CRC2 = data[i + cmd_len + 2];

            uint8_t temp[cmd_len + 1];
            memcpy(temp, data + i, cmd_len + 1); // 将命令进行校验

            volatile uint16_t crc = Check_CRC_Serial(temp, cmd_len + 1, 1);

            if ((uint8_t)CRC1 == (uint8_t)((crc >> 8) & 0xFF) && (uint8_t)CRC2 == (uint8_t)(crc & 0xFF))
            {
                uint8_t pin = data[4];
                uint8_t status = data[5];
                NRC_DigIn_Set(pin, status);
            }
        }
    }
}

void Handle_the_host_computer()
{
    while (true)
    {
        for (uint8_t i = 1; i < 17; i++)
        {
            Get_Virtual_IO_Status(i);
            uint8_t res = NRC_ReadDigOutByBoard(2, i);
            // std::cout << std::hex << std::uppercase << std::setw(2)
            //       << std::setfill('0') << (int)res << " ";
            if (gpio_status.IO_Statis[i - 1] != res)
                Set_Virtual_IO_Status(i, res);
            gpio_status.IO_Statis[i - 1] = res;
        }
        Serial_STM.ClearAllBuffers();
        // 避免空转高占用
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
}