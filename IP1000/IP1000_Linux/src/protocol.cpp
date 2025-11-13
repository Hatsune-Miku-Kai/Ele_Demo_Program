#include "protocol.h"
#include <string.h>
extern SerialPort Serial_STM;
extern SerialPort Serial_User;
extern uint8_t return_data_Ok[7];
extern uint8_t return_data_Error[7];
extern uint8_t RX_buf[128];
extern bool Ispower_On;

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
                temp >>= 1;       // 向右移一位
                temp ^= 0xA001;   // 异或运算结果
            }
            i++;
        }
        k++; // 下一个校验字节
    }

    if (!is_high_first)
        temp = (temp >> 8) | (temp << 8);

    return temp;
}


void Check_Power_On()
{
    while(!Ispower_On)
    {
        uint8_t User_len = Serial_User.Read(RX_buf, sizeof(RX_buf));
        if(User_len <= 0) continue;
        std::cout << "User_len=" << std::hex << std::uppercase << (int)User_len<< std::dec << std::endl;
        std::cout << "RAW:";
        for (int k = 0; k < User_len; ++k) {
            std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                      << (int)RX_buf[k] << " ";
        }
        std::cout << std::dec << std::endl;

        for(int i = 0; i < User_len; i++)
        {
              if(RX_buf[i] == 0xfe && RX_buf[i + 1] == 0xfe && RX_buf[i + 3] == 0x03)
              {
                  std::cout << "Check CRC"<<std::endl;
                  uint8_t cmd_len = RX_buf[i + 2];
                  uint8_t cmd = RX_buf[i + 3];
                  volatile uint8_t status = RX_buf[i + 4];
                  std::cout << "status=" << std::hex << std::uppercase << (int)status<< std::dec << std::endl;

                  uint8_t CRC1 = RX_buf[ i + cmd_len + 1];
                  uint8_t CRC2 = RX_buf[ i + cmd_len + 2];
//                  if(CRC1 == 0X0A )
//                     CRC1 = 0X0D;
//                  if(CRC2 == 0X0A)
//                      CRC2 = 0X0D;

                  uint8_t temp[cmd_len + 1];
                  memcpy(temp, RX_buf + i, cmd_len + 1);//将命令进行校验

                  volatile uint16_t crc = Check_CRC_Serial(temp, cmd_len + 1 , 1);
                  std::cout << "Compare: CRC1=" << std::hex << (int)CRC1
                            << " expect=" << ((crc >> 8) & 0xFF)
                            << " | CRC2=" << (int)CRC2
                            << " expect=" << (crc & 0xFF)
                            << std::dec << std::endl;

                  if ((uint8_t)CRC1 == (uint8_t)((crc >> 8) & 0xFF) && (uint8_t)CRC2 == (uint8_t)(crc & 0xFF))
                   {
                      std::cout << "Into Switch"<<std::endl;

                      switch ((uint8_t)status)
                      {
                          case 1:
                          {
                            if(Ispower_On == true)
                            {
                                Serial_User.Write(return_data_Ok, sizeof(return_data_Ok));
                                return;
                            }
                              std::cout << "CRC OK -> Into Switch (status=1)" << std::endl;
                              std::cout << "Into Switch 1" << std::endl;
                              int ret = system("echo out > /sys/class/gpio/gpio114/direction");
                              std::cout << "direction ret=" << ret << std::endl;
                              ret = system("echo 1 > /sys/class/gpio/gpio114/value");
                              std::cout << "value ret=" << ret << std::endl;

                              int res = gpio_read_value(100);
                              std::cout << "res=" << std::hex << std::uppercase << (int)res << std::dec << std::endl;

                              if (res == 0 || res == -1)
                              {
                                  std::cout << "false " << std::endl;
                                  Ispower_On = false;
                                  Serial_User.Write(return_data_Error, sizeof(return_data_Error));
                              }
                              else if (res == 1)
                              {
                                  std::cout << "true " << std::endl;
                                  Ispower_On = true;
                                  Serial_User.Write(return_data_Ok, sizeof(return_data_Ok));
                              }
                               break;
                          }

                          case 0:
                          {
                              std::cout << "CRC OK -> Into Switch default (status=" << (int)status << ")" << std::endl;
                              system("echo out > /sys/class/gpio/gpio114/direction");
                              system("echo 0 > /sys/class/gpio/gpio114/value");
                              Serial_User.Write(return_data_Error, sizeof(return_data_Error));
                              Ispower_On = false;
                              break;
                          }

                      }


                  }
            }

        }
    }
}


int gpio_read_value(int gpio)
{
    char path[64];
    char value;
    int fd;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        return -1;  // 打开失败
    }

    if (read(fd, &value, 1) != 1) {
        perror("read failed");
        close(fd);
        return -1;  // 读取失败
    }

    close(fd);

    // 返回状态：0 或 1
    return (value == '0') ? 0 : 1;
}

void Control_Motor(uint8_t id)
{

}
