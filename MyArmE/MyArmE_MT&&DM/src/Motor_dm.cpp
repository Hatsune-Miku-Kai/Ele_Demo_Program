#include <Motor_dm.h>
#include "MotorMT.h"
#include <cstring> 

int readArray[8];

uint8_t power_on_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC};
uint8_t power_off_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD};
uint8_t set_zero_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE};
uint8_t clear_err_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB};
uint8_t motor_control[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F};
uint8_t Change_PID[8] = {0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Get_PID[8] = {0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00};

float flo, sp;
unsigned char charArray[4], spArray[4];

int JOINT_MAX_ARR_DM[6] = {J1_MAX, J2_MAX, J3_MAX, J4_MAX, J5_MAX, J6_MAX};
int JOINT_MIN_ARR_DM[6] = {J1_MIN, J2_MIN, J3_MIN, J4_MIN, J5_MIN, J6_MIN};

float liju;

#define CAN_RX 33 // 33 14
#define CAN_TX 32 // 32 13

Motor_dm::Motor_dm()
{
  InitCan();
}

Motor_dm::~Motor_dm()
{
}

void Motor_dm::InitCan()
{
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, CAN_MODE_NORMAL);
  can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
  int err = can_driver_install(&g_config, &t_config, &f_config);
  can_start();
}

float Uint8_tIntoFloat( int *arr, uint8_t pos)
{
    int32_t temp = 0; 
    temp = arr[pos] << 24 | arr[pos -1] << 16 | arr[pos - 2] << 8 | arr[pos - 3];

    float result;
    std::memcpy(&result, &temp, sizeof(result));
    return result;
}

// 发送can数据
void Send_Msg(uint8_t *data, byte len, int id)
{
  can_message_t temp;
  while (can_receive(&temp, 0) == ESP_OK)
    ;

  can_message_t message;
  message.identifier = id;
  message.flags = CAN_MSG_FLAG_NONE;
  message.data_length_code = len;
  for (int i = 0; i < len; i++)
  {
    message.data[i] = data[i];
    // printf("%02x\n\r ",data[i]);
  }
  can_transmit(&message, pdMS_TO_TICKS(1000));
}

// 接收can数据
void Read_Msg(int readArray[8])
{
  can_message_t message;
  can_receive(&message, pdMS_TO_TICKS(100));
  if (!(message.flags & CAN_MSG_FLAG_RTR))
  {
    for (int i = 0; i < message.data_length_code; i++)
    {
      readArray[i] = message.data[i];
      Serial.print(message.data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void float2Bytes(byte bytes_temp[4], float float_variable)
{
  union
  {
    float a;
    byte bytes[4];
  } thing;
  thing.a = float_variable;
  memcpy(bytes_temp, thing.bytes, 4);
}

int float_to_uint(float x, float x_min, float x_max, int bits)
{
  float span = x_max - x_min;
  float offset = x_min;
  return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
  float span = x_max - x_min;
  float offset = x_min;
  return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}

void Set_Rad(uint8_t id, float rad, float speed)
{
  int frame_id = (0x01 << 8) | id;
  float2Bytes(charArray, rad);
  float2Bytes(spArray, speed);
  motor_control[0] = charArray[0];
  motor_control[1] = charArray[1];
  motor_control[2] = charArray[2];
  motor_control[3] = charArray[3];
  motor_control[4] = spArray[0];
  motor_control[5] = spArray[1];
  motor_control[6] = spArray[2];
  motor_control[7] = spArray[3];
  Send_Msg(motor_control, 8, frame_id);
  Read_Msg(readArray);
}

float Get_Rad(uint8_t id)
{
  int frame_id = (0x01 << 8) | id;
  can_clear_receive_queue();
  Send_Msg(power_on_array, 8, frame_id);
  Read_Msg(readArray);
  uint16_t combined = (readArray[1] << 8) | readArray[2];
  // Serial.printf("rad %d,%d\n", combined, id);
  float pos = uint_to_float(combined, -12.5, 12.5, 16);
  return pos;
}

float Get_Angle(uint8_t id)
{
  float angle = (Get_Rad(id) / 3.14) * 180;
  return angle;
}

void Motor_dm::Power_On(uint8_t *id, uint8_t len)
{
  for (int i = 0; i < len; i++)
  {
    int frame_id = (0x01 << 8) | id[i];
    Send_Msg(power_on_array, 8, frame_id);
    Read_Msg(readArray);
  }
}

void Motor_dm::Power_Off(uint8_t *id, uint8_t len)
{
  for (int i = 0; i < len; i++)
  {
    int frame_id = (0x01 << 8) | id[i];
    Send_Msg(power_off_array, 8, frame_id);
    Read_Msg(readArray);
  }
}

void Motor_dm::Set_Calibration(uint8_t *id, uint8_t len)
{
  for (int i = 0; i < len; i++)
  {
    int frame_id = (0x01 << 8) | id[i];
    Send_Msg(set_zero_array, 8, frame_id);
    Read_Msg(readArray);
  }
}

void Motor_dm::Clear_Err(uint8_t *id, uint8_t len)
{
  for (int i = 0; i < len; i++)
  {
    int frame_id = (0x01 << 8) | id[i];
    Send_Msg(clear_err_array, 8, frame_id);
    Read_Msg(readArray);
  }
}


/**
 * @brief 设置多个电机的目标角度
 * @param id 电机ID数组
 * @param len 电机数量
 * @param angles 目标角度数组(单位：度)
 * @param speed 运动速度(单位：度/秒)
 * @note 速度映射为1:1.5度,随后又映射为弧度,最大可达到3.14rad/s
 *       角度同样映射为弧度,在不限位的情况下最大可运动至180°
 */
void Motor_dm::Set_Angles(uint8_t *id, uint8_t len, float *angles, float speed)
{
  
  uint8_t is_exceed_angles = Is_In_Position(id, len, angles);
  if(speed > 100)
    speed = 100;
    float speed1 = speed * 1.8;

  if (is_exceed_angles == 0)
  {
    for (int i = 0; i < len; i++)
    {
      float rad = (angles[i] / 180.00) * 3.14;
      float angles_speed = (speed1 / 180.00) * 3.14;
      Set_Rad(id[i], rad, angles_speed);


      uint8_t start = readArray[4] & 0x0F;

      uint16_t temp = start << 8 | readArray[5];
      float temp_float = (float)(120 - ((temp / 4095.0) *240.0));
      Serial.printf("%0.2f\n\r",temp_float);

      liju = temp_float;
    }
  }
  else
  {
    printf("Motor %d Angle Exceed Limit\n\r", is_exceed_angles);
    printf("Joint %d must in range %d to %d\n\r", is_exceed_angles, JOINT_MIN_ARR_DM[is_exceed_angles - 1], JOINT_MAX_ARR_DM[is_exceed_angles - 1]);
  }
}

/**
 * @brief 设置多个电机的目标弧度位置和运动速度
 * @param id 电机ID数组
 * @param len 电机数量 
 * @param rad 目标弧度数组(单位：弧度)
 * @param speed 运动速度(单位：弧度/秒)
 * @note  发送为小端序,即低字节在前,高字节在后 
 */
void Motor_dm::Set_Rads(uint8_t *id, uint8_t len, float *rad, float speed)
{
  for (int i = 0; i < len; i++)
  {
    int frame_id = (0x01 << 8) | id[i];
    float2Bytes(charArray, rad[i]);
    float2Bytes(spArray, speed);
    motor_control[0] = charArray[0];
    motor_control[1] = charArray[1];
    motor_control[2] = charArray[2];
    motor_control[3] = charArray[3];
    motor_control[4] = spArray[0];
    motor_control[5] = spArray[1];
    motor_control[6] = spArray[2];
    motor_control[7] = spArray[3];
    Send_Msg(motor_control, 8, frame_id);
    Read_Msg(readArray);
  }
}


/**
 * @brief 获取多个电机的状态信息
 * @param id 电机ID数组
 * @param len 电机数量
 * @return Errors 返回电机状态数组
 * @note 状态编码:
 *       0:失能 1:使能 8:超压 9:欠压 
 *       A:过电流 B:MOS过温 C:电机线圈过温 
 *       D:通讯丢失 E:过载
 */
Errors Motor_dm::Get_Status(uint8_t *id, uint8_t len)
{
  Errors errors;
  for (int i = 0; i < len; i++)
  {
    int frame_id = (0x01 << 8) | id[i];
    Send_Msg(power_on_array, 8, frame_id);
    Read_Msg(readArray);
    unsigned int D0 = readArray[0];
    unsigned int ID = 0x0100 | id[i];
    unsigned int temp = D0 & ~ID;
    unsigned int motor_status = temp >> 4;
    errors[i] = motor_status;
    Serial.println(errors[i], HEX);
  }
  return errors;
}

Angles Motor_dm::Get_Angles(uint8_t *id, uint8_t len)
{
  Angles angles;
  for (int i = 0; i < len; i++)
  {
    angles[i] = Get_Angle(id[i]);
    Serial.print(angles[i]);
    Serial.print(" ");
  }
  Serial.println();
  return angles;
}

Rads Motor_dm::Get_Rads(uint8_t *id, uint8_t len)
{
  Rads rads;
  for (int i = 0; i < len; i++)
  {
    rads[i] = Get_Rad(id[i]);
    Serial.print(rads[i]);
    Serial.print(" ");
  }
  Serial.println();
  return rads;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 0:失能，1：使能，8：超压，9：欠压，A：过电流，B: MOS过温，C:电机线圈过温，D：通讯丢失，E：过载
u_char Motor_dm::get_status(uint8_t id)
{
  // id 0101 257
  int frame_id = (0x01 << 8) | id;
  Send_Msg(power_on_array, 8, frame_id);
  Read_Msg(readArray);
  unsigned int D0 = readArray[0];
  unsigned int ID = 0x0100 | id;
  unsigned int temp = D0 & ~ID;
  unsigned int motor_status = temp >> 4;
  Serial.println(motor_status, HEX);
  return motor_status;
}

// 读取电机弧度
float Motor_dm::get_rad(uint8_t id)
{
  int frame_id = (0x01 << 8) | id;
  can_clear_receive_queue();
  Send_Msg(power_on_array, 8, frame_id);
  Read_Msg(readArray);
  uint16_t combined = (readArray[1] << 8) | readArray[2];
  // Serial.printf("rad %d,%d\n", combined, id);
  float pos = uint_to_float(combined, -12.5, 12.5, 16);
  return pos;
}

// 读取电机角度
float Motor_dm::get_angle(uint8_t id)
{
  float angle = (get_rad(id) / 3.14) * 180;
  return angle;
}

// 读取电机速度
float Motor_dm::get_speed(uint8_t id)
{
  int frame_id = (0x01 << 8) | id;
  Send_Msg(power_on_array, 8, frame_id);
  Read_Msg(readArray);
  uint16_t combined = (readArray[3] << 4) | (readArray[4] >> 4);
  float sp = uint_to_float(combined, -45.0, 45.0, 12);
  return sp;
}

// 电机使能
void Motor_dm::power_on(uint8_t id)
{
  int frame_id = (0x01 << 8) | id;
  Send_Msg(power_on_array, 8, frame_id);
  Read_Msg(readArray);
}

// 电机失能
void Motor_dm::power_off(uint8_t id)
{
  int frame_id = (0x01 << 8) | id;
  Send_Msg(power_off_array, 8, frame_id);
  Read_Msg(readArray);
}

// 零位校准
void Motor_dm::set_zero(uint8_t id)
{
  int frame_id = (0x01 << 8) | id;
  power_off(id);
  Send_Msg(set_zero_array, 8, frame_id);
  Read_Msg(readArray);
  power_on(id);
}

// 清除异常
void Motor_dm::clear_err(uint8_t id)
{
  int frame_id = (0x01 << 8) | id;
  Send_Msg(clear_err_array, 8, frame_id);
  Read_Msg(readArray);
}

// 发送弧度rad、速度rad/s
void Motor_dm::send_rad(uint8_t id, float rad, float speed)
{
  int frame_id = (0x01 << 8) | id;
  float2Bytes(charArray, rad);
  float2Bytes(spArray, speed);
  motor_control[0] = charArray[0];
  motor_control[1] = charArray[1];
  motor_control[2] = charArray[2];
  motor_control[3] = charArray[3];
  motor_control[4] = spArray[0];
  motor_control[5] = spArray[1];
  motor_control[6] = spArray[2];
  motor_control[7] = spArray[3];
  Send_Msg(motor_control, 8, frame_id);
  Read_Msg(readArray);
}

// 发送角度angle、速度 度/s
void Motor_dm::send_angle(uint8_t id, float angle, float speed)
{
  float rad = (angle / 180.00) * 3.14;
  float angle_speed = (speed / 180.00) * 3.14;
  send_rad(id, rad, angle_speed);
}

/**
 * @brief 检查电机角度是否在允许范围内
 * @param ID 需要判断的电机ID,传入需要控制的ID序号数组(1~6)
 * @param id_len 电机ID长度
 * @param angles 要检查的角度数组
 * @return uint8_t 0表示所有角度在范围内，否则返回超出范围的电机ID
 */
uint8_t Is_In_Position(uint8_t *ID, uint8_t id_len, float *angles)
{
  for (int i = 0; i < id_len; i++)
  {
    switch (ID[i])
    {
    case 1:
    {
      if (angles[0] > J1_MIN && angles[0] < J1_MAX)
        break;
      else
        return 1;
    }
    case 2:
    {
      if (angles[1] > J2_MIN && angles[1] < J2_MAX)
        break;
      else
        return 2;
    }
    case 3:
    {
      if (angles[2] > J3_MIN && angles[2] < J3_MAX)
        break;
      else
        return 3;
    }
    case 4:
    {
      if (angles[3] > J4_MIN && angles[3] < J4_MAX)
        break;
      else
        return 4;
    }
    case 5:
    {
      if (angles[4] > J5_MIN && angles[4] < J5_MAX)
        break;
      else
        return 5;
    }
    case 6:
    {
      if (angles[5] > J6_MIN && angles[5] < J6_MAX)
        break;
      else
        return 6;
    }
    }
  }
  return 0;
}

// 1 -- POS
// 0 -- CURRENT

void Motor_dm::Change_P(uint8_t *id, float *KP, uint8_t len, uint8_t mode)

{
  if(mode)
  {
    for(int i = 0; i < len; i++)
    {
      int frame_id = PID_CMD;
      Change_PID[0] = id[i];
      Change_PID[1] = id[i] >> 4;
      Change_PID[3] = SPEED_KP_ARRD;
      float2Bytes(charArray, KP[i]);
      Change_PID[4] = charArray[0];
      Change_PID[5] = charArray[1];
      Change_PID[6] = charArray[2];
      Change_PID[7] = charArray[3];
      Send_Msg(Change_PID, 8, frame_id);
      Read_Msg(readArray);

      float KP = Uint8_tIntoFloat(readArray, 7);
      Serial.print(KP);
    }
  }

  else
  {
    for(int i = 0; i < len; i++)
    {
      int frame_id = PID_CMD;
      Change_PID[0] = id[i];
      Change_PID[1] = id[i] >> 4;
      Change_PID[3] = POS_KP_ARRD;
      float2Bytes(charArray, KP[i]);
      Change_PID[4] = charArray[0];
      Change_PID[5] = charArray[1];
      Change_PID[6] = charArray[2];
      Change_PID[7] = charArray[3];
      Send_Msg(Change_PID, 8, frame_id);
      Read_Msg(readArray);

      float KP = Uint8_tIntoFloat(readArray, 7);
      Serial.print(KP);
    }
  }

}

// mode : 1 -- SPEED
//        0 -- POS
void Motor_dm::Change_I(uint8_t *id, float *KI, uint8_t len, uint8_t mode)
{
  if(mode)
  {
    for(int i = 0; i < len; i++)
    {
      int frame_id = PID_CMD;
      Change_PID[0] = id[i];
      Change_PID[1] = id[i] >> 4;
      Change_PID[3] = SPEED_KI_ARRD;
      float2Bytes(charArray, KI[i]);
      Change_PID[4] = charArray[0];
      Change_PID[5] = charArray[1];
      Change_PID[6] = charArray[2];
      Change_PID[7] = charArray[3];
      Send_Msg(Change_PID, 8, frame_id);
      Read_Msg(readArray);

      float Ki = Uint8_tIntoFloat(readArray, 7);
      Serial.print(Ki);
    }

  }

  else
  {
    for(int i = 0; i < len; i++)
    {
      int frame_id = PID_CMD;
      Change_PID[0] = id[i];
      Change_PID[1] = id[i] >> 4;
      Change_PID[3] = POS_KI_ARRD;
      float2Bytes(charArray, KI[i]);
      Change_PID[4] = charArray[0];
      Change_PID[5] = charArray[1];
      Change_PID[6] = charArray[2];
      Change_PID[7] = charArray[3];
      Send_Msg(Change_PID, 8, frame_id);
      Read_Msg(readArray);
  
      float Ki = Uint8_tIntoFloat(readArray, 7);
      Serial.print(Ki);
    }
  }

}

// mode : 1 -- SPEED
//        0 -- POS
PID Motor_dm::Get_KP(uint8_t *id, uint8_t len, uint8_t mode)
{
  PID pid;
  if (mode)
  {
    for(int i = 0; i < len; i++ )
    {
      int frame_id = PID_CMD;
      Get_PID[0] = id[i];
      Get_PID[1] = id[i] >> 4;
      Get_PID[3] = SPEED_KP_ARRD;
      Send_Msg(Get_PID, 8, frame_id);
      Read_Msg(readArray);
      float KP = Uint8_tIntoFloat(readArray, 7);
      pid[id[i] - 1] = KP;

      Serial.print(KP,6);
      Serial.println();
    }
  }


  else
  {
    for(int i = 0; i < len; i++ )
    {
      int frame_id = PID_CMD;
      Get_PID[0] = id[i];
      Get_PID[1] = id[i] >> 4;
      Get_PID[3] = POS_KP_ARRD;
      Send_Msg(Get_PID, 8, frame_id);
      Read_Msg(readArray);
      float KP = Uint8_tIntoFloat(readArray, 7);
      pid[id[i] - 1] = KP;

      Serial.print(KP,6);
      Serial.println();
    }
  }
  return pid;
}

PID Motor_dm::Get_KI(uint8_t *id, uint8_t len, uint8_t mode)
{
  PID pid;
  if (mode)
  {
    for(int i = 0; i < len; i++ )
    {
      int frame_id = PID_CMD;
      Get_PID[0] = id[i];
      Get_PID[1] = id[i] >> 4;
      Get_PID[3] = SPEED_KI_ARRD;
      Send_Msg(Get_PID, 8, frame_id);
      Read_Msg(readArray);
      float KI = Uint8_tIntoFloat(readArray, 7);
      pid[id[i] - 1] = KI;

      Serial.print(KI,6);
      Serial.println();
    }

  }

  else
  {
    for(int i = 0; i < len; i++ )
    {
      int frame_id = PID_CMD;
      Get_PID[0] = id[i];
      Get_PID[1] = id[i] >> 4;
      Get_PID[3] = POS_KI_ARRD;
      Send_Msg(Get_PID, 8, frame_id);
      Read_Msg(readArray);
      float KI = Uint8_tIntoFloat(readArray, 7);
      pid[id[i] - 1] = KI;

      Serial.print(KI,6);
      Serial.println();
    }
  }
  return pid;
}


void Motor_dm::Save_PID(uint8_t *id, uint8_t len)
{
  int frame_id = PID_CMD;
  uint8_t Save_PID_Arr[8] = {0x00, 0x00, 0xAA, 0x01, 0x00, 0x00, 0x00, 0x00};
  for(int i = 0; i < len; i++)
  {
    Save_PID_Arr[0] = id[0];
    Save_PID_Arr[1] = id[0] >> 4;
  }
  Send_Msg(Get_PID, 8, frame_id);
  Read_Msg(readArray);
}