#ifndef __SERIAL_HPP__
#define __SERIAL_HPP__
#include <Arduino.h>
#include <SPI.hpp>
#include <vector>   

bool Serial_ReadLine(char *line, size_t lineSize);
float bytes_to_float(uint8_t *bytes);
void parseSPIData(uint8_t* buf);
void Serial_Read();
uint16_t Check_CRC_Serial(std::vector<uint8_t>& data, bool is_high_first);
#endif