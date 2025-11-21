#ifndef __CMD_H__
#define __CMD_H__

#define HEADER 0xFE
#define PRIVATE_HEADER 0xFF

#define MAJOR_VERSION 20
#define MODIFY_VERSION 0

#define LOCAL 0x00 //本地轨迹
#define BLOCKLY_RUNNER 0x01  //云端轨迹

//对主控固件

#define Pause_Cmd_450 0x26
#define Pause_Fast_Len_450 3
#define Pause_Slow_Len_450 4

#define Resume_Cmd_450 0x28
#define Resume_Len_450 3

#define Stop_Cmd_450 0x29
#define Stop_Fast_Len_450 3
#define Stop_Slow_Len_450 4

#define Jog_Move_Angle_Cmd_450 0x30
#define Jog_Move_Angle_Len_450 6

#define Jog_Move_Coord_Cmd_450 0x32
#define Jog_Move_Coord_Len_450 6

#define Jog_Increment_Angle_Cmd_450 0x33
#define Jog_Increment_Angle_Len_450 7

#define Jog_Increment_Coord_Cmd_450 0x34
#define Jog_Increment_Coord_Len_450 7

#define Set_Servo_Calibration_Cmd_450 0x54
#define Set_Servo_Calibration_Len_450 4

#define Set_Tool_IO_Status 0x61
#define Get_Tool_IO_Status 0x62

#define Start_Record_Cmd_450 0x70
#define Start_Record_Len_450 3

#define Run_Record_Cmd_450 0x71
#define Run_Record_Len_450 3

#define Stop_Record_Cmd_450 0x72
#define Stop_Record_Len_450 3

#define Clear_Record_Cmd_450 0x73
#define Clear_Record_Len_450 3

#define Set_VR_Mode_Cmd_450 0x7A
#define Set_VR_Mode_Len_450 4


//与后端通信的命令
#define Get_Major_Version_Cmd_450 0x01
#define Get_Major_Version_Len_450 4

#define Get_Modify_Version_Cmd_450 0x02
#define Get_Modify_Version_Len_450 4

#define Save_Record_Cmd_450 0x03
#define Save_Record_Len_450 4

#define Run_Saved_Record_Cmd_450 0x04
#define Run_Saved_Record_Len_450 6

#define Program_Pause_Cmd_450 0x05
#define Program_Pause_Len_450 4

#define Program_Resume_Cmd_450 0x06
#define Program_Resume_Len_450 4

#define Get_Blockly_Runner_File_Time_Cmd_450 0x07
#define Get_Blockly_Runner_File_Time_Len_450 4

#define Get_Blockly_Runner_File_Num_Cmd_450 0x08
#define Get_Blockly_Runner_File_Num_Len_450 3

#define Get_Blockly_Runner_File_Status_Cmd_450 0x09
#define Get_Blockly_Runner_File_Status_Len_450 4

#define Delete_Blockly_Runner_File_Cmd_450 0x0A
#define Delete_Blockly_Runner_File_Len_450 4

#define Get_Blockly_Runner_File_Remaining_Run_Num_Cmd_450 0x0B
#define Get_Blockly_Runner_File_Remaining_Run_Num_Len_450 4

#endif
