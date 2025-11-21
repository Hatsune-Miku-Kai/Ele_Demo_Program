#include "Serial.hpp"
#include "SPI.hpp"
#include <TFT_eSPI.h>
#include "Cmd.h"
#include "Common.h"

// 全局数据变量
#include "Global_Data.h"
#define BUF_SIZE 1024 // 缓冲区大小

uint8_t User_Buffer[BUF_SIZE]; // 用于存储接收到的数据
std::vector<uint8_t> Data;
// size_t uartBufferLen = 0;     // 当前缓冲区长度

bool Serial_ReadLine(char *line, size_t lineSize)
{
    static char buf[256];  // 持续保存未完整的输入
    static size_t idx = 0; // 当前写入位置

    while (Serial.available() > 0)
    {
        char c = Serial.read();

        if (c == '\r') // 行结束
        {
            if (idx > 0) // 有内容才算一行
            {
                // 在结尾补 "\r\n"
                if (idx + 2 < sizeof(buf))
                {
                    buf[idx++] = '\r';
                    buf[idx++] = '\n';
                }
                buf[idx] = '\0'; // C 字符串结束符

                // 拷贝到外部缓冲
                strncpy(line, buf, lineSize);
                line[lineSize - 1] = '\0'; // 保证结尾

                idx = 0; // 清空准备下一行
                return true;
            }
            // 如果是连续的 \r\n，仅跳过
        }
        else
        {
            if (idx + 1 < sizeof(buf))
            {
                buf[idx++] = c; // 累积字符
            }
            else // 超出缓冲，丢弃
            {
                idx = 0;
            }
        }
    }

    return false; // 还没接收完整的一行
}

void Serial_Read()
{
    int bytesRead = 0;
    while (Serial.available() > 0)
    {
        uint8_t c = Serial.read();
        User_Buffer[bytesRead++] = c;
        // Serial.write(c);
    }

    if (bytesRead > 0)
    {
        for (int i = 0; i < bytesRead; i++)
        {
            if (User_Buffer[i] == HEADER && User_Buffer[i + 1] == HEADER)
            {
                uint8_t len = User_Buffer[i + 2];
                uint8_t cmd = User_Buffer[i + 3];
                // Serial.println(len,HEX);
                // for(int j = 0; j < len + 3; j++)
                // {
                //     Serial.print(" ");
                //     Serial.print(User_Buffer[i + j], HEX);
                // }
                // Serial.println();
                uint8_t Check_CRC1 = User_Buffer[i + len + 1];
                uint8_t Check_CRC2 = User_Buffer[i + len + 2];
                // Serial.print("USER_CRC1: ");
                // Serial.println(Check_CRC1, HEX);
                // Serial.print("USER_CRC2: ");
                // Serial.println(Check_CRC2, HEX);

                std::vector<uint8_t> data(User_Buffer + i, User_Buffer + i + len + 1); //将数据缓存
                // for(int j = 0; j < data.size(); j++)
                // {
                //     Serial.print(" ");
                //     Serial.print(data[j], HEX);
                // }
                // Serial.println();

                uint16_t crc = Check_CRC_Serial(data, true);// 计算该条指令的校验码

                // Serial.print("CacluCRC1: ");
                // Serial.println(crc >> 8 & 0XFF, HEX);
                // Serial.print("CacluCRC2: ");
                // Serial.println(crc & 0xFF, HEX);

                uint8_t CRC1 = crc >> 8 & 0XFF;
                uint8_t CRC2 = crc & 0xFF;
                if (CRC1 == Check_CRC1 && CRC2 == Check_CRC2)
                {
                    Global_Data::Data.assign(data.begin() + 4, data.end());             // 获取data的实用数据,会先清空再赋值
                    Global_Data::Recv_Data_Origin.assign(data.begin() + 2, data.end()); // 获取data包含命令和长度的数据,会先清空再赋值
                    // for(int i = 0 ; i < Global_Data::Data.size(); i++)
                    // {
                    //     Serial.print(" ");
                    //     Serial.print(Global_Data::Data[i], HEX);
                    // }
                    // Serial.println();

                    // for(int i = 0 ; i < Global_Data::Recv_Data_Origin.size(); i++)
                    // {
                    //     Serial.print(" ");
                    //     Serial.print(Global_Data::Recv_Data_Origin[i], HEX);
                    // }
                    // Serial.println();
                    // Serial.println(Global_Data::Data.size());
                }
            }
            
            if(User_Buffer[i] == PRIVATE_HEADER && User_Buffer[i + 1] == PRIVATE_HEADER)
            {
                uint8_t len = User_Buffer[i + 2];
                uint8_t cmd = User_Buffer[i + 3];

                uint8_t Check_CRC1 = User_Buffer[i + len + 1];
                uint8_t Check_CRC2 = User_Buffer[i + len + 2];

                std::vector<uint8_t> data(User_Buffer + i, User_Buffer + i + len + 1); // 计算该条指令的校验码
                uint16_t crc = Check_CRC_Serial(data, true);

                uint8_t CRC1 = crc >> 8 & 0XFF;
                uint8_t CRC2 = crc & 0xFF;
                if (CRC1 == Check_CRC1 && CRC2 == Check_CRC2)
                {
                    Global_Data::BackEnd_Data.assign(data.begin() + 2, data.end()); // 获取data包含命令和长度的数据,会先清空再赋值
                }
            }
        }
    }
    bytesRead = 0;
}

void Serial_Read_Xiaowei()
{
    uint8_t data = 0;
    while (Serial.available() > 0)
    {
        data = Serial.read();
        // Serial.printf("read %d\n", data);
        /*if (!is_have_data)
            is_have_data = true;*/
    }
}

void Serial_Write()
{
    while (Serial.available() > 0)
    {
        Serial.write(Serial.read());
    }
}

/*
1．设置CRC寄存器，并给其赋初值FFFF(hex)。
2．将数据的第一个8bit字符与16位CRC寄存器的低8位进行异或，并把结果存入CRC寄存器。
3．CRC寄存器向右移一位，MSB补零，并检查移出的LSB。
4．如果LSB为0，重复第3步；若LSB为1，CRC寄存器与多项式码相异或。
5．重复第3与第4步直到8次移位全部完成。此时一个8bit数据处理完毕。
6．重复第2至第5步直到所有数据全部字节数据处理完成。
7. 将该通讯信息帧所有字节按上述步骤计算完成后，即可得到16位CRC寄存器的高、低
   字节。
8．最终CRC寄存器的内容即为CRC值，注意本程序CRC值高位在左，低位在右
*/
uint16_t Check_CRC_Serial(std::vector<uint8_t> &data, bool is_high_first)
{
    uint16_t temp = 0xffff, i, k = 0; // 初始值
#ifdef DEBUG_SERVO
    for (int i = 0; i < v_cmd.size(); ++i)
    {
        printf("%d--", v_cmd[i]);
    }
    printf("\nCalCheckCode %d\n", v_cmd.size());
#endif
    while (k < data.size()) // 要计算总字节数
    {
        temp = temp ^ data[k]; // 输入要检验字节,异或运算结果
        i = 0;
        while (i < 8) // 单字节位移数
        {
            if ((temp & 0x01) == 0) // 检查运算结果最后位是否为零
            {
                temp = temp >> 1; // 向右移一位
                i++;              // 计数器
            }
            else
            {
                temp = temp >> 1;     // 向右移一位
                temp = temp ^ 0xa001; // 异或运算结果
                i++;                  // 计数器
            }
        }
        k++; // 下一个校验字节
    }
    if (!is_high_first)
        temp = (temp >> 8) | (temp << 8);

    // Serial.printf("crc %d\n", temp);

    return temp;
}

void Handle_Cmd(std::vector<uint8_t> &data)
{
    if (data.empty())
        return;
    uint8_t cmd = data[1];

    std::unordered_map<uint8_t, Handle_Cmd_Func>::iterator it = Global_Data::Cmd_Table.find(cmd);
    if (it != Global_Data::Cmd_Table.end())
    {
        it->second(data);
        data.clear();
    }
    else
    {
        DefaultHandler({});
        data.clear();
    }
}