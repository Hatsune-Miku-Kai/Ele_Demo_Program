#include <iostream>
#include "nrcAPI.h"
#include "protocol.h"
#include <iomanip>
#include <string>
#include <ctime>
#include <string.h>
#include <thread>

void SystemStartup()
{

  Config_STM32F407_GPIO(16);
  // 输出Nexmotion版本库信息
  std::cout << "库版本：" << NRC_GetNexMotionLibVersion() << std::endl;
  // 启动控制系统
  NRC_StartController();

  std::cout << "Setup_Successful" << std::endl;

  // 检测控制系统是否初始化完成
  while (NRC_GetControlInitComplete() != 1)
  {
    NRC_Delayms(100); // 延时100ms
  }
  // 清除所有错误
  NRC_ClearAllError();

  std::cout << "----" << NRC_GetControlInitComplete() << std::endl;
  std::cout << "StartController Success" << std::endl;
  std::cout << "获取同步版本号" << NRC_GetSyncVersion() << std::endl;
  NRC_Delayms(200);
}

int main()
{
#if defined(__arm__) || defined(__aarch64__)
  // if (daemon(1, 1) < 0)
  // {
  //   perror("error daemon\n");
  //   exit(1);
  // }
#endif

  // 系统启动
  SystemStartup();
  std::cout << "Hello, Here is Edwin Kevin" << std::endl;

  for(int i = 1 ; i < 17; i++)
  {
    Set_Virtual_IO_Status(i,0);
  }
  try
  {
    std::thread t1(Handle_the_host_computer);
    std::thread t2(Run);
    t1.join();
    t2.join();
    // Check_Power_On();
    // while (true)
    // {
    // }
  }

  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  // 保持程序继续运行
}
