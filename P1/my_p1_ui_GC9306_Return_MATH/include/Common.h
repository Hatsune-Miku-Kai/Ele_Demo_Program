#ifndef __COMMON__H__
#define __COMMON__H__
#include "Common.h"
#include "Serial.hpp"
#include "Screen_Base.h"

#define DATA1 0x01
#define DATA2 0x02
#define DATA3 0x03
#define DATA4 0x04

//对主控接口
#ifdef MyCobot_Pro_450
void Set_VR_Mode_450(bool mode);
void Program_Pause_450(uint8_t mode = 1);
void Program_Resume_450();
void Start_Record_450();
void Run_Record_450();
void Stop_Record_450();
void Clear_Record_450();

void Program_Stop_450(uint8_t mode = 1);
void Jog_Move_Angle_450(uint8_t Joint_id, uint8_t dir, uint8_t Speed);
void Set_Jog_Increment_Angle_450(uint8_t Joint_id, float angle, uint8_t Speed);

void Set_Jog_Increment_Coord_450(uint8_t Joint_id, float coord, uint8_t Speed);
void Jog_Move_Coord_450(uint8_t Joint_id, uint8_t dir, uint8_t Speed);

void Set_Calibration_450(uint8_t Joint_id);


//对后端接口
void Init_Cmd_Table();
void DefaultHandler(const std::vector<uint8_t>&);

void Get_Software_Major_Version_450(const std::vector<uint8_t>&);
void Get_Software_Modify_Version_450(const std::vector<uint8_t>&);



void Program_Pause_450_BackEnd(uint8_t offset);

void Program_Resume_450_BackEnd(uint8_t offset);

void Get_Blockly_Runner_File_Time_450_BackEnd(uint8_t offset);

void Save_Record_450_BackEnd(uint8_t mode);

void Run_Saved_Record_450_BackEnd(uint8_t mode, uint8_t offset, uint8_t num);

void Get_Blockly_Runner_File_Remaining_Run_Num_450_BackEnd(uint8_t offset);

void Get_Blockly_Runner_File_Status_450_BackEnd(uint8_t offset);

void Delete_Blockly_Runner_File_450_BackEnd(uint8_t mode, uint8_t offset);

void Get_Blockly_Runner_File_Num_450_BackEnd();



#endif

#endif
