#include <SPI.hpp>
#include "Serial.hpp"

#define SPI_CS 4   // 片选引脚
#define SPI_MOSI 2 // 主设备数据输出引脚
#define SPI_SCK 3  // 时钟线引脚
#define SPI_MISO 1 // 主设备数据输入引脚

SPIClass MySpi(HSPI); // 使用 HSPI

uint8_t Send_Buffer[20] = {0xA5, 0X00, 0X00, 0X00, 0X00, 
                           0X00, 0X00, 0X00, 0X00, 0X00, 
                           0X00, 0X00, 0X00, 0X00, 0x00,
                           0X00, 0X00, 0X00, 0X00, 0X00};//Get_Angles_Cmd   

uint8_t Recv_Buffer[20];

float parsed[6];

float J1;
float J2;
float J3; 

void SPI_Init()
{
  pinMode(SPI_CS, OUTPUT);
  digitalWrite(SPI_CS, HIGH); // 空闲状态 CS 高

  // 初始化 SPI 接口
  MySpi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
  // 手动控制 CS
  MySpi.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  Serial.println("HSPI 初始化完成！");
}

void SendGcode(const char *cmd)
{
  digitalWrite(SPI_CS, LOW); // 开始通信
  // Serial.print("Sending G-code: ");
  // Serial.println(cmd);

  for (size_t i = 0; i < strlen(cmd); i++)
  {
    uint8_t received = MySpi.transfer(cmd[i]);
    Serial.print("Sent: 0x");
    Serial.print(cmd[i], HEX);
    Serial.print(", Received: 0x");
    Serial.println(received, HEX);
    Recv_Buffer[i] = received;
    delay(1);

  }
    // MySpi.transfer(0X0D);
    // MySpi.transfer(0X0A);
    digitalWrite(SPI_CS, HIGH); // 结束通信

}

//目前先返回一个符号位,0x2D表示负号,0x34表示正号,返回的数据前两个字节不要
void SendArray(uint8_t *txData, uint8_t *rxData)
{
  digitalWrite(SPI_CS, LOW); // 开始通信

  for (size_t i = 0; i < 20; i++)
  {
    uint8_t received = MySpi.transfer(txData[i]);
    if (rxData != nullptr)
    {
      rxData[i] = received; // 存到接收数组
    }

    // Serial.print("Sent: 0x");
    // Serial.print(txData[i], HEX);
    // Serial.print(", Received: 0x");
    // Serial.println(received, HEX);
    delay(1);
  }

  digitalWrite(SPI_CS, HIGH); // 结束通信
  parseSPIData(rxData);
  // Serial.println(parsed[0]);
  // Serial.println(parsed[1]);
  // Serial.println(parsed[2]);
  J1 = parsed[0];
  J2 = parsed[1];
  J3 = parsed[2];
}

// test byte
void SendByte(uint8_t data)
{
  digitalWrite(SPI_CS, LOW); // 拉低片选
  uint8_t received = MySpi.transfer(data);
  digitalWrite(SPI_CS, HIGH); // 拉高片选

  // Serial.print("Sent Byte: 0x");
  // Serial.print(data, HEX);
  // Serial.print(", Received: 0x");
  // Serial.println(received, HEX);
}
// test end

float bytes_to_float(uint8_t *bytes)
{
  float f;
  memcpy(&f, bytes, sizeof(f));

  // 把整数的位模式原封不动地复制到 float（可移植且不触发别名问题）
  Serial.println(f);
  if(-180.0 > f || f > 360.0)
    return 0;
  return f;
}

void parseSPIData(uint8_t* buf) {
    for (int i = 0; i < 3; i++) {
        char str[6 + 1]; // 每个数据加 '\0'
        for (int j = 0; j < 6; j++) {
            str[j] = (char)buf[1 + i * 6 + j];
        }
        str[6] = '\0';
        parsed[i] = atof(str);  // 转浮点数
    }
}


