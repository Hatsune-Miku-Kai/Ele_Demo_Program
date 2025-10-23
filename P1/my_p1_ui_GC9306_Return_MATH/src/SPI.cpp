#include <SPI.hpp>
#include "Serial.hpp"
#include <vector>
#include "driver/spi_slave.h"

#define SPI_CS 4   // 片选引脚
#define SPI_MOSI 2 // 主设备数据输出引脚
#define SPI_SCK 3  // 时钟线引脚
#define SPI_MISO 1 // 主设备数据输入引脚

#define SPI_SEND_BUFFER_SIZE 128
#define SPI_RECV_BUFFER_SIZE 1024
#define HEADER 0xFE
#define MAX_CHUNK_SIZE 128

// uint8_t Send_Buffer[20] = {0xA5, 0X00, 0X00, 0X00, 0X00, 
//                            0X00, 0X00, 0X00, 0X00, 0X00, 
//                            0X00, 0X00, 0X00, 0X00, 0x00,
//                            0X00, 0X00, 0X00, 0X00, 0X00};//Get_Angles_Cmd   

static const char *TAG = "SPI_SLAVE";
uint8_t Recv_Buffer[SPI_RECV_BUFFER_SIZE];
uint8_t Send_Buffer[SPI_SEND_BUFFER_SIZE];

void SPI_Slave_Init()
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MOSI,
        .miso_io_num = SPI_MISO,
        .sclk_io_num = SPI_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .flags = SPICOMMON_BUSFLAG_SCLK | SPICOMMON_BUSFLAG_MISO | SPICOMMON_BUSFLAG_MOSI,
    };

    spi_slave_interface_config_t slvcfg = {
        .spics_io_num = SPI_CS,
        .flags = 0,
        .queue_size = 3,
        .mode = 1,
        .post_setup_cb = NULL,
        .post_trans_cb = NULL,
    };

    esp_err_t ret = spi_slave_initialize(SPI2_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    if(ret != ESP_OK)
    {
      Serial.println("SPI Slave Init Failed");
    }

    //memset(Send_Buffer, 0xFF, SPI_BUFFER_SIZE);  // 默认填充 0xFF
    // memset(Send_Buffer, 0xAA, sizeof(Send_Buffer));
    for (int i = 0; i < 128; i++) {
        Send_Buffer[i] = i + 1;  // 从 0x01 到 0x80
}
}


//设置从机的发送缓冲区
void SPI_Set_Send_Buffer(uint8_t cmd, const std::vector<uint8_t>& data)
{
  std::vector<uint8_t> frame;
  frame.push_back(HEADER);
  frame.push_back(HEADER);

  uint8_t len = 1 + data.size() + 2;// CMD + data.size() + CRC1 + CRC2
  frame.push_back(len);

  frame.push_back(cmd);

  for (std::vector<uint8_t>::const_iterator it = data.begin(); it != data.end(); ++it)//将数据写入发送缓冲区
  {
    frame.push_back(*it);
  }

  uint16_t crc16 = Check_CRC16(frame);

  Serial.print("crc16: 0x");
  Serial.println((crc16 >> 8 & 0xFF), HEX);

  Serial.print("crc16: 0x");
  Serial.println((crc16 & 0xFF), HEX);

  frame.push_back(crc16 & 0xFF);
  frame.push_back(crc16 >> 8 & 0xFF);

  memset(Send_Buffer, 0x00, SPI_SEND_BUFFER_SIZE);
  memcpy(Send_Buffer, frame.data(), frame.size());
}


void SPI_Slave_Recv(uint8_t *rx_data, int total_len)
{
    int offset = 0;

    while (offset < total_len) {
        int chunk_size = min(MAX_CHUNK_SIZE, total_len - offset);

        // memset(Send_Buffer, 0, chunk_size);
        memset(Recv_Buffer, 0, MAX_CHUNK_SIZE); // 清空，避免残留

        spi_slave_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.length = chunk_size * 8;        // 单位为 bit
        t.tx_buffer = Send_Buffer;
        t.rx_buffer = Recv_Buffer;

        // 把事务放入队列（非阻塞传输模式的标准流程）
        esp_err_t rc = spi_slave_queue_trans(SPI2_HOST, &t, portMAX_DELAY);
        if (rc != ESP_OK) {
            ESP_LOGE("SPI_SLAVE", "queue_trans failed: %d", rc);
        }

        // 等待传输完成
        spi_slave_transaction_t *ret_trans = NULL;
        rc = spi_slave_get_trans_result(SPI2_HOST, &ret_trans, portMAX_DELAY);
        if (rc != ESP_OK || ret_trans == NULL) {
            ESP_LOGW("SPI_SLAVE", "get_trans_result timeout/fail: %d", rc);
        }

        //把本次接收到的数据放到用户缓冲区里
        memcpy(rx_data + offset, Recv_Buffer, chunk_size);

        // 调试打印（可选）
        // Serial.printf("Chunk received @offset %d (%d bytes): ", offset, chunk_size);
        // for (int i = 0; i < 32; ++i) Serial.printf("%02X ", Recv_Buffer[i]);
        // Serial.println();

        offset += chunk_size;
}
}

double parsed[6];

double J1;
double J2;
double J3; 

// void SPI_Init()
// {
//   pinMode(SPI_CS, OUTPUT);
//   digitalWrite(SPI_CS, HIGH); // 空闲状态 CS 高

//   // 初始化 SPI 接口
//   MySpi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
//   // 手动控制 CS
//   MySpi.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

//   Serial.println("HSPI 初始化完成！");
// }

// void SendGcode(const char *cmd)
// {
//   digitalWrite(SPI_CS, LOW); // 开始通信
//   // Serial.print("Sending G-code: ");
//   // Serial.println(cmd);

//   for (size_t i = 0; i < strlen(cmd); i++)
//   {
//     uint8_t received = MySpi.transfer(cmd[i]);
//     // Serial.print("Sent: 0x");
//     // Serial.print(cmd[i], HEX);
//     // Serial.print(", Received: 0x");
//     // Serial.println(received, HEX);
//     Recv_Buffer[i] = received;
//     delay(1);
//   }
//     // MySpi.transfer(0X0D);
//     // MySpi.transfer(0X0A);
//     digitalWrite(SPI_CS, HIGH); // 结束通信

// }

// //目前先返回一个符号位,0x2D表示负号,0x34表示正号,返回的数据前两个字节不要
// void SendArray(uint8_t *txData, uint8_t *rxData)
// {
//   digitalWrite(SPI_CS, LOW); // 开始通信

//   for (size_t i = 0; i < 20; i++)
//   {
//     uint8_t received = MySpi.transfer(txData[i]);
//     if (rxData != nullptr)
//     {
//       rxData[i] = received; // 存到接收数组
//     }

//     // Serial.print("Sent: 0x");
//     // Serial.print(txData[i], HEX);
//     // Serial.print(", Received: 0x");
//     // Serial.println(received, HEX);
//     delay(1);
//   }

//   digitalWrite(SPI_CS, HIGH); // 结束通信
//   parseSPIData(rxData);
//   // Serial.println(parsed[0]);
//   // Serial.println(parsed[1]);
//   // Serial.println(parsed[2]);
//   J1 = parsed[0];
//   J2 = parsed[1];
//   J3 = parsed[2];
// }


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

void parseSPIData(uint8_t* buf) 
{
    for (int i = 0; i < 3; i++) {
        char str[6 + 1]; // 每个数据加 '\0'
        for (int j = 0; j < 6; j++) {
            str[j] = (char)buf[1 + i * 6 + j];
        }
        str[6] = '\0';
        parsed[i] = atof(str);  // 转浮点数
    }
}


bool Handle_Recv(std::vector<uint8_t>& Rx_Data)
{
  std::vector<uint8_t> temp; 
  uint16_t Origin_CRC = Rx_Data.back() | (Rx_Data[Rx_Data.size() - 2] << 8);

  for(uint8_t i = 0; i < Rx_Data.size() - 2; i++)
  {
    temp.push_back(Rx_Data[i]);
  }
  uint16_t crc = Check_CRC16(temp);
  if(crc == Origin_CRC)
    return true;
  return false;
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
uint16_t Check_CRC16(const std::vector<uint8_t>& data)
{
    uint16_t crc = 0xFFFF;

    // 用传统下标方式遍历 data
    for (uint8_t i = 0; i < data.size(); ++i)
    {
        uint8_t byte = data[i];  // 取出当前字节

        crc ^= byte;
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}
