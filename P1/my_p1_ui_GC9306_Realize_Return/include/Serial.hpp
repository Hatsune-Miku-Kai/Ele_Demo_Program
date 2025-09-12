#ifndef __SERIAL_HPP__
#define __SERIAL_HPP__
#include <Arduino.h>
#include <SPI.h>

void Send_Data();
String Serial_ReadLine();
float bytes_to_float(uint8_t *bytes);

#endif