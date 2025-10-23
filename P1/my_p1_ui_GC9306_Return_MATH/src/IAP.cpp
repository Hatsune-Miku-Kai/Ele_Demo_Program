#include "IAP.h"    

#include <Arduino.h>
#include <Update.h>

#define IAP_Rx 40
#define IAP_Tx 26
#define CMD_START 0xAA
#define CMD_END 0x55

void IAP_Serial_Init()
{
    Serial.begin(115200, IAP_Rx, IAP_Tx);
}


void IAP_Update()
{
    while(1)
    {
    if (Serial.available() > 0) {
      uint8_t cmd = Serial.read();

      if (cmd == CMD_START) {
        uint32_t fw_size = 0;//获取固件大小
        uint8_t size_buf[4];//大小通过四个字节进行传输
        Serial.readBytes(size_buf, 4);
        fw_size = size_buf[0] | (size_buf[1] << 8) | (size_buf[2] << 16) | (size_buf[3] << 24);

        char md5_buf[33] = {0};
        Serial.readBytes(md5_buf, 32);
        String expected_md5 = String(md5_buf);

        Serial.printf("[IAP] 固件大小: %u bytes, MD5: %s\n", fw_size, expected_md5.c_str());

        if (!Update.begin(fw_size)) {
          Serial.println("[IAP] OTA 初始化失败");
          continue;
        }

        uint8_t buf[1024];
        uint32_t received = 0;

        while (received < fw_size) {
          int chunk = Serial.readBytes(buf, sizeof(buf));
          if (chunk > 0) {
            Update.write(buf, chunk);
            received += chunk;
          }
        }

        uint8_t endFlag = Serial.read();
        if (endFlag == CMD_END && Update.end()) {
          String actual_md5 = Update.md5String();
          Serial.printf("[IAP] 实际MD5: %s\n", actual_md5.c_str());

          if (actual_md5 == expected_md5) {
            Serial.println("[IAP] 校验成功，准备重启");
            delay(1000);
            ESP.restart();
          } else {
            Serial.println("[IAP] 校验失败，升级中止");
            Update.abort();
          }
        } else {
          Serial.println("[IAP] OTA 写入失败");
          Update.abort();
        }
      }
    }
    }

}
