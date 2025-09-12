#include "Serial.hpp"
#include "SPI.hpp"
extern uint8_t Recv_Buffer[13];

void Serial_Init()
{
  Serial.begin(115200);
}

String Serial_ReadLine()
{
  static String input;
  static bool gotCR = false; // 记录是否已经收到 '\r'

  while (Serial.available() > 0)
  {
    char c = Serial.read();

    if (c == '\r')
    {
      gotCR = true;
    }
    else if (c == '\n')
    {
      if (gotCR && input.length() > 0)
      {
        input += "\r\n";
        String line = input;
        input = "";
        gotCR = false;
        SendGcode(line.c_str());
        return line;
      }
      gotCR = false;
    }
    else
    {
      input += c;
      gotCR = false;
    }
  }
  return ""; // 还未接收完整的 \r\n 行
}

void Send_Data()
{
  Serial.write(Recv_Buffer + 1, 12);
}
