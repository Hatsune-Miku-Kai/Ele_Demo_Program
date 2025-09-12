#ifndef __SPI_HPP__
#define __SPI_HPP__
#include <Arduino.h>
#include <SPI.h>

void SendGcode(const char* cmd);
void SPI_Init();
void SendByte(uint8_t data);
void SendArray(uint8_t* txData, uint8_t* rxData);
float bytes_to_float(uint8_t *bytes);

#endif
