#include <iostream>
#include "nrcAPI.h"
#include "protocol.h"
#include <iomanip>
#include <string>
#include <ctime>
#include <string.h>

bool Ispower_On = false;

uint8_t TX_buf[128] = {0};
uint8_t RX_buf[128] = {0};
SerialPort Serial_User("/dev/ttyS2", 115200);
SerialPort Serial_STM("/dev/ttyS1", 115200);

void PrintHex(const std::string &tag, const uint8_t *data, size_t len);
int gpio_read_value(int gpio);

#define VERSION 0x0A
uint8_t version_array[7] = {0xfe, 0xfe, 0x04, 0x04, VERSION, 0x3A, 0XFF};

void SystemStartup() {
  //输出Nexmotion版本库信息
  std::cout << "库版本：" << NRC_GetNexMotionLibVersion() << std::endl;
  //启动控制系统
  NRC_StartController();

  std::cout << "Setup_Successful" << std::endl;

  //检测控制系统是否初始化完成
  while (NRC_GetControlInitComplete() != 1 ) {
    NRC_Delayms(100);   //延时100ms
  }
  //清除所有错误
  NRC_ClearAllError();

  std::cout << "Clear_Error_Successful" << std::endl;

  std::cout << "----" << NRC_GetControlInitComplete() << std::endl;
  std::cout << "StartController Success" << std::endl;
  std::cout << "获取同步版本号" << NRC_GetSyncVersion() << std::endl;
  NRC_Delayms(200);
}

uint8_t return_data_Ok[7] = {0xfe, 0xfe, 0x04, 0x03, 0x01, 0xCD, 0XBC};
uint8_t return_data_Error[7] = {0xfe, 0xfe, 0x04, 0x03, 0x00, 0x0D, 0X7D};



int main() {
#if defined(__arm__) || defined(__aarch64__)
if (daemon(1, 1) < 0)
{
perror("error daemon\n");
exit(1);
}
#endif


  //系统启动
  SystemStartup();
  std::cout << "Hello World" << std::endl;


  try
  {
      Check_Power_On();

      while(true)
      {

              uint8_t User_len = Serial_User.Read(TX_buf, sizeof(TX_buf));

              if(TX_buf > 0)
              {
                  for(int i = 0; i < User_len; i++)
                  {
                    if(TX_buf[i] == 0xFE && TX_buf[i + 1] == 0xFE && TX_buf[i + 3] == 0x03)
                    {
                        uint8_t cmd_len = TX_buf[i + 2];
                        uint8_t cmd = TX_buf[i + 3];
                        uint8_t status = TX_buf[i + 4];

                        uint8_t CRC1 = TX_buf[cmd_len + 1];
                        uint8_t CRC2 = TX_buf[cmd_len + 2];

//                        if(CRC1 == 0X0A )
//                           CRC1 = 0X0D;
//                        if(CRC2 == 0X0A)
//                            CRC2 = 0X0D;

                        uint8_t temp[cmd_len + 1];
                        memcpy(temp, TX_buf + i, cmd_len + 1);//将命令进行校验

                        uint16_t crc = Check_CRC_Serial(temp, cmd_len + 1 , 1);

                        if(CRC1 == ((crc >> 8) & 0xFF) && CRC2 == (crc & 0xFF) )
                        {
                            switch(status)
                            {
                              case 0:
                              {
                                  std::cout << "Into Switch 0"<<std::endl;
                                  system("echo out > /sys/class/gpio/gpio114/direction");
                                  system("echo 0 > /sys/class/gpio/gpio114/value");
                                  Serial_User.Write(return_data_Error, sizeof(return_data_Error));
                                  Ispower_On = false;
                                  Check_Power_On();
                                  break;
                              }

                              case 1:
                              {
                                    Serial_User.Write(return_data_Ok, sizeof(return_data_Ok));
                                    break;
                              }
                            }
                        }
                       }

                    else if(TX_buf[i] == 0xFE && TX_buf[i + 1] == 0xFE && TX_buf[i + 3] == 0x04)
                    {
                       Serial_User.Write(version_array,sizeof(version_array));
                    }

                    else
                    {
                       Serial_STM.Write(TX_buf, User_len);
                       break;
                    }

                  }

              }

              uint8_t STM_len = Serial_STM.Read(RX_buf, sizeof(RX_buf));
              if (STM_len > 0) {
                  Serial_User.Write(RX_buf, STM_len);
              }
              memset(TX_buf, 0, sizeof(TX_buf));
              memset(RX_buf, 0, sizeof(RX_buf));

              // 避免空转高占用
              std::this_thread::sleep_for(std::chrono::microseconds(100));

      }
  }

  catch(std::exception &e) {
      std::cerr << "Exception: " << e.what() << std::endl;
  }

  //保持程序继续运行

}

void PrintHex(const std::string &tag, const uint8_t *data, size_t len)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *ptm = std::localtime(&now);

    std::cout << "[" << std::put_time(ptm, "%H:%M:%S") << "] " << tag << " ";
    for (size_t i = 0; i < len; i++)
        std::cout << std::uppercase << std::hex << std::setw(2)
                  << std::setfill('0') << (int)data[i] << " ";
    std::cout << std::dec << std::endl;
}



