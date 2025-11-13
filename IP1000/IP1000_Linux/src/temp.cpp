// else if (TX_buf[i] == 0xFE && TX_buf[i + 1] == 0xFE && TX_buf[i + 3] == 0x04)
// {
//     Serial_User.Write(version_array, sizeof(version_array));
// }

// switch (status)
// {
// case 0:
// {
//     std::cout << "Into Switch 0" << std::endl;
//     system("echo out > /sys/class/gpio/gpio114/direction");
//     system("echo 0 > /sys/class/gpio/gpio114/value");
//     Serial_User.Write(return_data_Error, sizeof(return_data_Error));
//     Ispower_On = false;
//     Check_Power_On();
//     break;
// }

// case 1:
// {
//     Serial_User.Write(return_data_Ok, sizeof(return_data_Ok));
//     break;
// }
// }

// void Check_Power_On()
// {
//     while (!Ispower_On)
//     {
//         uint8_t User_len = Serial_User.Read(RX_buf, sizeof(RX_buf));
//         if (User_len <= 0)
//             continue;
//         std::cout << "User_len=" << std::hex << std::uppercase << (int)User_len << std::dec << std::endl;
//         std::cout << "RAW:";
//         for (int k = 0; k < User_len; ++k)
//         {
//             std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
//                       << (int)RX_buf[k] << " ";
//         }
//         std::cout << std::dec << std::endl;

//         for (int i = 0; i < User_len; i++)
//         {
//             if (RX_buf[i] == 0xfe && RX_buf[i + 1] == 0xfe && RX_buf[i + 3] == 0x03)
//             {
//                 std::cout << "Check CRC" << std::endl;
//                 uint8_t cmd_len = RX_buf[i + 2];
//                 uint8_t cmd = RX_buf[i + 3];
//                 volatile uint8_t status = RX_buf[i + 4];
//                 std::cout << "status=" << std::hex << std::uppercase << (int)status << std::dec << std::endl;

//                 uint8_t CRC1 = RX_buf[i + cmd_len + 1];
//                 uint8_t CRC2 = RX_buf[i + cmd_len + 2];

//                 uint8_t temp[cmd_len + 1];
//                 memcpy(temp, RX_buf + i, cmd_len + 1); // 将命令进行校验

//                 volatile uint16_t crc = Check_CRC_Serial(temp, cmd_len + 1, 1);
//                 std::cout << "Compare: CRC1=" << std::hex << (int)CRC1
//                           << " expect=" << ((crc >> 8) & 0xFF)
//                           << " | CRC2=" << (int)CRC2
//                           << " expect=" << (crc & 0xFF)
//                           << std::dec << std::endl;

//                 if ((uint8_t)CRC1 == (uint8_t)((crc >> 8) & 0xFF) && (uint8_t)CRC2 == (uint8_t)(crc & 0xFF))
//                 {
//                     std::cout << "Into Switch" << std::endl;

//                     switch ((uint8_t)status)
//                     {
//                     case 1:
//                     {
//                         if (Ispower_On == true)
//                         {
//                             Serial_User.Write(return_data_Ok, sizeof(return_data_Ok));
//                             return;
//                         }
//                         std::cout << "CRC OK -> Into Switch (status=1)" << std::endl;
//                         std::cout << "Into Switch 1" << std::endl;
//                         int ret = system("echo out > /sys/class/gpio/gpio114/direction");
//                         std::cout << "direction ret=" << ret << std::endl;
//                         ret = system("echo 1 > /sys/class/gpio/gpio114/value");
//                         std::cout << "value ret=" << ret << std::endl;

//                         int res = gpio_read_value(100);
//                         std::cout << "res=" << std::hex << std::uppercase << (int)res << std::dec << std::endl;

//                         if (res == 0 || res == -1)
//                         {
//                             std::cout << "false " << std::endl;
//                             Ispower_On = false;
//                             Serial_User.Write(return_data_Error, sizeof(return_data_Error));
//                         }
//                         else if (res == 1)
//                         {
//                             std::cout << "true " << std::endl;
//                             Ispower_On = true;
//                             Serial_User.Write(return_data_Ok, sizeof(return_data_Ok));
//                         }
//                         break;
//                     }

//                     case 0:
//                     {
//                         std::cout << "CRC OK -> Into Switch default (status=" << (int)status << ")" << std::endl;
//                         system("echo out > /sys/class/gpio/gpio114/direction");
//                         system("echo 0 > /sys/class/gpio/gpio114/value");
//                         Serial_User.Write(return_data_Error, sizeof(return_data_Error));
//                         Ispower_On = false;
//                         break;
//                     }
//                     }
//                 }
//             }
//         }
//     }
// }