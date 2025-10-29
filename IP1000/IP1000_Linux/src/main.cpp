#include <iostream>
#include "nrcAPI.h"
#include "protocol.h"
#include <iomanip>
#include <string>
#include <ctime>

void PrintHex(const std::string &tag, const uint8_t *data, size_t len);

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

      SerialPort Serial_STM("/dev/ttyS1", 115200);
      SerialPort Serial_User("/dev/ttyS2", 115200);


      uint8_t TX_buf[128];
      uint8_t RX_buf[128];
      NRC_Delayms(1000);
      while(true) {
          uint8_t User_len = Serial_User.Read(TX_buf, sizeof(TX_buf));
          if (User_len > 0) {
              Serial_STM.Write(TX_buf, User_len);
          }

          uint8_t STM_len = Serial_STM.Read(RX_buf, sizeof(RX_buf));
          if (STM_len > 0) {
              Serial_User.Write(RX_buf, STM_len);
          }

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
