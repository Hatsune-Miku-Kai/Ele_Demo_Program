#include "Serial.hpp"
#include "SPI.hpp"
extern uint8_t Recv_Buffer[20];
extern uint8_t Send_Buffer[20];

extern float parsed[6];


String Serial_ReadLine()
{
    static String buffer;   // 持续保存未完整的输入
    while (Serial.available() > 0)
    {
        char c = Serial.read();
        if (c == '\r')    // 回车或换行视为结束
        {
            if (buffer.length() > 0)   // 有内容才返回
            {
                buffer += "\r\n";
                String line = buffer;
                buffer = "";           // 清空缓冲，准备下一行
                return line;           // 返回完整字符串
            }
            // 如果是连续的 \r\n，只跳过
        }
        else
        {
            buffer += c;               // 累积字符
        }
    }
    buffer = "";
    return ""; // 还没接收完整的一行
}




// void Send_Data()
// {
//   Serial.write(Recv_Buffer + 1, 12);
// }
