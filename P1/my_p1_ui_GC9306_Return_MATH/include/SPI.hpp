#ifndef __SPI_HPP__
#define __SPI_HPP__
#include <Arduino.h>
#include <vector>

// void SendGcode(const char* cmd);
void SPI_Slave_Init();
// void SendByte(uint8_t data);
// void SendArray(uint8_t* txData, uint8_t* rxData);
float bytes_to_float(uint8_t *bytes);
uint16_t Check_CRC16(const std::vector<uint8_t>& data);
void SPI_Set_Send_Buffer(uint8_t cmd, const std::vector<uint8_t>& data);
void SPI_Slave_Recv(uint8_t *rx_data, int total_len);

#endif
