#include "Common.h"
#include "Serial.hpp"
#include <vector>
#include "Cmd.h"
#include <iostream>
#include "Global_Data.h"

#ifdef MyCobot_Pro_450
// 对主控接口
void Set_VR_Mode_450(bool mode)
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Set_VR_Mode_Len_450, Set_VR_Mode_Cmd_450, mode};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

// 1 -- 慢速暂停
// 0 -- 快速暂停
void Program_Pause_450(uint8_t mode)
{
    if (mode == 1)
    {
        std::vector<uint8_t> Send_Data = {HEADER, HEADER, Pause_Slow_Len_450, Pause_Cmd_450, mode};
        uint16_t crc = Check_CRC_Serial(Send_Data, true);
        Send_Data.push_back((crc >> 8) & 0xFF);
        Send_Data.push_back(crc & 0xFF);
        Serial.write(Send_Data.data(), Send_Data.size());
    }

    else if (mode == 0)
    {
        std::vector<uint8_t> Send_Data = {HEADER, HEADER, Pause_Fast_Len_450, Pause_Cmd_450};
        uint16_t crc = Check_CRC_Serial(Send_Data, true);
        Send_Data.push_back((crc >> 8) & 0xFF);
        Send_Data.push_back(crc & 0xFF);
        Serial.write(Send_Data.data(), Send_Data.size());
    }
}

void Program_Resume_450()
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Resume_Len_450, Resume_Cmd_450};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Start_Record_450()
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Start_Record_Len_450, Start_Record_Cmd_450};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Stop_Record_450()
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Stop_Record_Len_450, Stop_Record_Cmd_450};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Run_Record_450()
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Run_Record_Len_450, Run_Record_Cmd_450};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Clear_Record_450()
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Clear_Record_Len_450, Clear_Record_Cmd_450};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

// angle -- 想要的角度增量
void Set_Jog_Increment_Angle_450(uint8_t Joint_id, float angle, uint8_t Speed)
{
    uint8_t data[2];
    angle *= 100.0f;
    int16_t temp = (int16_t)angle;

    data[0] = (temp >> 8) & 0xFF;
    data[1] = temp & 0xFF;
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Jog_Increment_Angle_Len_450, Jog_Increment_Angle_Cmd_450, Joint_id, data[0], data[1], Speed};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

// 会一直移动至限位附近
void Jog_Move_Angle_450(uint8_t Joint_id, uint8_t dir, uint8_t Speed)
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Jog_Move_Angle_Len_450, Jog_Move_Angle_Cmd_450, Joint_id, dir, Speed};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

// 1 -- 慢速停止
// 0 -- 快速停止
void Program_Stop_450(uint8_t mode)
{
    if (mode == 1)
    {
        std::vector<uint8_t> Send_Data = {HEADER, HEADER, Stop_Slow_Len_450, Stop_Cmd_450, mode};
        uint16_t crc = Check_CRC_Serial(Send_Data, true);
        Send_Data.push_back((crc >> 8) & 0xFF);
        Send_Data.push_back(crc & 0xFF);
        Serial.write(Send_Data.data(), Send_Data.size());
    }

    else if (mode == 0)
    {
        std::vector<uint8_t> Send_Data = {HEADER, HEADER, Stop_Fast_Len_450, Stop_Cmd_450};
        uint16_t crc = Check_CRC_Serial(Send_Data, true);
        Send_Data.push_back((crc >> 8) & 0xFF);
        Serial.write(Send_Data.data(), Send_Data.size());
    }
}

void Set_Calibration_450(uint8_t Joint_id)
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Set_Servo_Calibration_Len_450, Set_Servo_Calibration_Cmd_450, Joint_id};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

// coord -- 想要的坐标增量
void Set_Jog_Increment_Coord_450(uint8_t Joint_id, float coord, uint8_t Speed)
{
    uint8_t data[2];
    if (Joint_id <= 3)
        coord *= 10.0f;

    else if (Joint_id > 3 && Joint_id <= 6)
        coord *= 100.0f;
    int16_t temp = (int16_t)coord;

    data[0] = (temp >> 8) & 0xFF;
    data[1] = temp & 0xFF;
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Jog_Increment_Coord_Len_450, Jog_Increment_Coord_Cmd_450, Joint_id, data[0], data[1], Speed};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

// 会一直移动至限位附近
void Jog_Move_Coord_450(uint8_t Joint_id, uint8_t dir, uint8_t Speed)
{
    std::vector<uint8_t> Send_Data = {HEADER, HEADER, Jog_Move_Coord_Len_450, Jog_Move_Coord_Cmd_450, Joint_id, dir, Speed};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back((crc >> 8) & 0xFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}











// 对后端接口
void DefaultHandler(const std::vector<uint8_t> &)
{
    // std::cout << "[Unknown CMD]" << std::endl;
}

void Init_Cmd_Table()
{
    for (int i = 0; i < 256; i++)
        Global_Data::Cmd_Table[i] = DefaultHandler; // 默认处理

    Global_Data::Cmd_Table[Get_Major_Version_Cmd_450] = Get_Software_Major_Version_450;
    Global_Data::Cmd_Table[Get_Modify_Version_Cmd_450] = Get_Software_Modify_Version_450;
    // Global_Data::Cmd_Table[Save_Record_Cmd_450] = Save_Record_450;
    // Global_Data::Cmd_Table[Run_Saved_Record_Cmd_450] = Run_Saved_Record_450;
    // Global_Data::Cmd_Table[Program_Pause_Cmd_450] = static_cast<void(*)(const std::vector<uint8_t>&)>(Program_Pause_450);
    // Global_Data::Cmd_Table[Program_Resume_Cmd_450] = static_cast<void(*)(const std::vector<uint8_t>&)>(Program_Resume_450);
    // Global_Data::Cmd_Table[Get_Blockly_Runner_File_Time_Cmd_450] = Get_Blockly_Runner_File_Time_450;

}

void Get_Software_Major_Version_450(const std::vector<uint8_t> &)
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Get_Major_Version_Len_450, Get_Major_Version_Cmd_450, MAJOR_VERSION};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Get_Software_Modify_Version_450(const std::vector<uint8_t> &)
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Get_Modify_Version_Len_450, Get_Modify_Version_Cmd_450, MODIFY_VERSION};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

// 0 -- 保存在本地
// 1 -- 保存在Blockly
void Save_Record_450_BackEnd(uint8_t mode)
{
    if (mode == 0x00 || mode == 0x01)
    {
        std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Save_Record_Len_450, Save_Record_Cmd_450, mode};
        uint16_t crc = Check_CRC_Serial(Send_Data, true);
        Send_Data.push_back(crc >> 8 & 0XFF);
        Send_Data.push_back(crc & 0xFF);
        Serial.write(Send_Data.data(), Send_Data.size());
    }
}

// 0 -- 运行本地轨迹
// 1 -- 运行Blockly轨迹
void Run_Saved_Record_450_BackEnd(uint8_t mode, uint8_t offset, uint8_t num)
{
    if (mode == 0x00 || mode == 0x01)
    {
        std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Run_Saved_Record_Len_450, Run_Saved_Record_Cmd_450, mode, offset, num};
        uint16_t crc = Check_CRC_Serial(Send_Data, true);
        Send_Data.push_back(crc >> 8 & 0XFF);
        Send_Data.push_back(crc & 0xFF);
        Serial.write(Send_Data.data(), Send_Data.size());
    }
}

void Program_Pause_450_BackEnd(uint8_t offset)
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Program_Pause_Len_450, Program_Pause_Cmd_450, offset};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Program_Resume_450_BackEnd(uint8_t offset)
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Program_Resume_Len_450, Program_Resume_Cmd_450, offset};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Get_Blockly_Runner_File_Time_450_BackEnd(uint8_t offset)
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Get_Blockly_Runner_File_Time_Len_450, Get_Blockly_Runner_File_Time_Cmd_450, offset};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}


void Get_Blockly_Runner_File_Num_450_BackEnd()
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Get_Blockly_Runner_File_Num_Len_450, Get_Blockly_Runner_File_Num_Cmd_450};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Get_Blockly_Runner_File_Status_450_BackEnd(uint8_t offset)
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Get_Blockly_Runner_File_Status_Len_450, Get_Blockly_Runner_File_Status_Cmd_450, offset};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}

void Delete_Blockly_Runner_File_450_BackEnd(uint8_t mode, uint8_t offset)
{
    if (mode == 0x00 || mode == 0x01)
    {
        std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Delete_Blockly_Runner_File_Len_450, Delete_Blockly_Runner_File_Len_450, mode, offset};
        uint16_t crc = Check_CRC_Serial(Send_Data, true);
        Send_Data.push_back(crc >> 8 & 0XFF);
        Send_Data.push_back(crc & 0xFF);
        Serial.write(Send_Data.data(), Send_Data.size());
    }
}

void Get_Blockly_Runner_File_Remaining_Run_Num_450_BackEnd(uint8_t offset)
{
    std::vector<uint8_t> Send_Data = {PRIVATE_HEADER, PRIVATE_HEADER, Get_Blockly_Runner_File_Remaining_Run_Num_Len_450, Get_Blockly_Runner_File_Remaining_Run_Num_Cmd_450, offset};
    uint16_t crc = Check_CRC_Serial(Send_Data, true);
    Send_Data.push_back(crc >> 8 & 0XFF);
    Send_Data.push_back(crc & 0xFF);
    Serial.write(Send_Data.data(), Send_Data.size());
}


#endif