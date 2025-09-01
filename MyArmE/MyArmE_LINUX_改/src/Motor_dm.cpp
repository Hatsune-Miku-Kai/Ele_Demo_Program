#include <Motor_dm.h>
#include <cstring>

// #include <serial/serial.h>

int readArray[8];
uint8_t Serial_Recv_Buffer_CAN[32];

uint8_t data_buffer[16];

uint8_t power_on_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC};
uint8_t power_off_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD};
uint8_t set_zero_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE};
uint8_t clear_err_array[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB};
uint8_t motor_control[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F};
uint8_t Change_Register_Parament[8] = {0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Get_Register_Parament[8] = {0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Set_Motors_Speeds[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Set_Motors_Current[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t Serial_Send_Buffer_CAN[30] = {0x55, 0xAA, 0x1E, 0x01, 0X01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xFF,
                                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x88}; // 第13位                                                //第21

uint8_t Serial_Send_Buffer_CAN_Power_On[30] = {0x55, 0xAA, 0x1E, 0x01, 0X01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xFF,
                                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x88};

float Speed_Mapping[8] = {J1_MAX_SPEED_MAPPING, J2_MAX_SPEED_MAPPING, J3_MAX_SPEED_MAPPING,
                          J4_MAX_SPEED_MAPPING, J5_MAX_SPEED_MAPPING, J6_MAX_SPEED_MAPPING,
                          J7_MAX_SPEED_MAPPING, J8_MAX_SPEED_MAPPING};

float flo, sp;
unsigned char charArray[4], spArray[4];

int JOINT_MAX_ARR_DM[8] = {J1_MAX, J2_MAX, J3_MAX, J4_MAX, J5_MAX, J6_MAX, J7_MAX, J8_MAX};
int JOINT_MIN_ARR_DM[8] = {J1_MIN, J2_MIN, J3_MIN, J4_MIN, J5_MIN, J6_MIN, J7_MIN, J8_MIN};

float liju;

#define CAN_RX 33 // 33 14
#define CAN_TX 32 // 32 13

static int g_sock = -1; // SocketCAN 文件描述符
static struct sockaddr_can g_addr;
static const char *ifname = "can0";

void Motor_dm::Init_Serial(const char *Port)
{

  int a = serial.openDevice(Port, 921600, SERIAL_DATABITS_8, SERIAL_PARITY_NONE, SERIAL_STOPBITS_1);
  if (a == -1)
  {
    std::cerr << "无法打开串口" << std::endl;
  }

  else
  {
    std::cerr << "打开串口成功" << std::endl;
  }
  sleep(2);
}

Motor_dm::Motor_dm(const char *Port)
{
  Init_Serial(Port);
}

Motor_dm::~Motor_dm()
{
  serial.closeDevice();
}

float Uint8_tIntoFloat(int *arr, uint8_t pos)
{
  int32_t temp = 0;
  temp = arr[pos] << 24 | arr[pos - 1] << 16 | arr[pos - 2] << 8 | arr[pos - 3];
  float result;
  std::memcpy(&result, &temp, sizeof(result));
  return result;
}

// 发送CAN消息
void Send_Msg(uint8_t *data, byte len, int id)
{
  if (g_sock < 0)
    return;

  struct can_frame frame;
  frame.can_id = id;
  frame.can_dlc = len;
  memcpy(frame.data, data, len);

  if (write(g_sock, &frame, sizeof(frame)) != sizeof(frame))
  {
    perror("CAN send failed");
  }
}

// 接收CAN消息（带100ms超时）
void Read_Msg(int readArray[8])
{
  if (g_sock < 0)
    return;

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000; // 100ms超时

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(g_sock, &readfds);

  // 等待数据到达
  if (select(g_sock + 1, &readfds, NULL, NULL, &tv) <= 0)
  {
    return; // 超时或无数据
  }

  struct can_frame frame;
  if (read(g_sock, &frame, sizeof(frame)) < 0)
  {
    perror("CAN read failed");
    return;
  }

  // 填充全局readArray
  for (int i = 0; i < frame.can_dlc; i++)
  {
    readArray[i] = frame.data[i];
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

void Motor_dm::Set_Rad(uint8_t id, float rad, float speed)
{
  uint32_t frame_id = (0x01 << 8) | id; //
  Serial_Send_Buffer_CAN[13] = frame_id;
  Serial_Send_Buffer_CAN[14] = frame_id >> 8;
  Serial_Send_Buffer_CAN[15] = frame_id >> 16;
  Serial_Send_Buffer_CAN[16] = frame_id >> 24;

  float2Bytes(charArray, rad);
  float2Bytes(spArray, speed);

  Serial_Send_Buffer_CAN[21] = charArray[0];
  Serial_Send_Buffer_CAN[22] = charArray[1];
  Serial_Send_Buffer_CAN[23] = charArray[2];
  Serial_Send_Buffer_CAN[24] = charArray[3];
  Serial_Send_Buffer_CAN[25] = spArray[0];
  Serial_Send_Buffer_CAN[26] = spArray[1];
  Serial_Send_Buffer_CAN[27] = spArray[2];
  Serial_Send_Buffer_CAN[28] = spArray[3];

  serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
  // for(int i = 0; i < 30; i++)
  // {
  //       std::cerr << "set_angle " << i << std::endl;
  //       std::cerr << "send_buffer: ";
  //       std::cerr << std::hex << std::setw(2) << std::setfill('0')
  //                 << (int)Serial_Send_Buffer_CAN[i] << std::endl;
  // }
}

float Motor_dm::Get_Rad(uint8_t id)
{
  uint32_t frame_id = (0x01 << 8) | id;
  Serial_Send_Buffer_CAN[13] = frame_id;
  Serial_Send_Buffer_CAN[14] = frame_id >> 8;
  Serial_Send_Buffer_CAN[15] = frame_id >> 16;
  Serial_Send_Buffer_CAN[16] = frame_id >> 24;

  for (int i = 0; i < 8; i++)
  {
    Serial_Send_Buffer_CAN[21 + i] = power_on_array[i];
  }

  // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
  // {
  //   std::cout << std::hex;
  //   std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
  // }

  serial.flushReceiver();
  serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN)); // 发送数据

  while (true)
  {
    int byte_num = serial.readBytes(Serial_Recv_Buffer_CAN, sizeof(Serial_Recv_Buffer_CAN)); // 读取数据
    if (byte_num >= 0)
    {
      for (int i = 0; i < sizeof(Serial_Recv_Buffer_CAN); i++)
      {
        if (Serial_Recv_Buffer_CAN[i] == 0xAA && Serial_Recv_Buffer_CAN[i + 1] == 0x11 && Serial_Recv_Buffer_CAN[i + 2] == 0x08)
        {
          std::memcpy(data_buffer, &Serial_Recv_Buffer_CAN[i], 16);

          // for (int i = 0; i < sizeof(data_buffer); i++)
          // {
          //   std::cout << std::hex;
          //   std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(data_buffer[i]) << " ";
          // }
          //   std::cout << std::endl;
          uint16_t combined = data_buffer[8] << 8 | data_buffer[9];
          float pos = uint_to_float(combined, -12.5, 12.5, 16);
          return pos;
        }
      }
    }
    else
    {
      std::cout << "error" << std::endl;
    }
  }
}

float Motor_dm::Get_Angle_Single(uint8_t id)
{
  float angle = (Get_Rad(id) / 3.14) * 180;
  std::cout << angle << "," << " ";
  return angle;
}

void Motor_dm::Power_On_Single(uint8_t id)
{

  uint32_t frame_id = (0x01 << 8) | id;

  Serial_Send_Buffer_CAN[13] = frame_id;
  Serial_Send_Buffer_CAN[14] = frame_id >> 8;
  Serial_Send_Buffer_CAN[15] = frame_id >> 16;
  Serial_Send_Buffer_CAN[16] = frame_id >> 24;

  for (int i = 0; i < 8; i++)
  {
    Serial_Send_Buffer_CAN[21 + i] = power_on_array[i];
  }

  int a = serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
  // std::cout << a << std::endl;
  usleep(1000);

  // for(int i = 0; i < 30; i++)
  // std::cerr << std::hex << std::setw(2) << std::setfill('0')
  // << (int)Serial_Send_Buffer_CAN[i] << " ";
  // std::cout<<std::endl;
  // serial.writeBytes(Serial_Send_Buffer_CAN,sizeof(Serial_Send_Buffer_CAN_1));
  // Serial_Send_Buffer_CAN_1[13] = 0x02;
  // serial.writeBytes(Serial_Send_Buffer_CAN,sizeof(Serial_Send_Buffer_CAN));
}

void Motor_dm::Power_On_All()
{
  for (int i = 1; i < 9; i++)
  {
    Power_On_Single(i);
  }
}

void Motor_dm::Power_Off_Single(uint8_t id)
{
  uint32_t frame_id = (0x01 << 8) | id;

  Serial_Send_Buffer_CAN[13] = frame_id;
  Serial_Send_Buffer_CAN[14] = frame_id >> 8;
  Serial_Send_Buffer_CAN[15] = frame_id >> 16;
  Serial_Send_Buffer_CAN[16] = frame_id >> 24;

  for (int i = 0; i < 8; i++)
  {
    Serial_Send_Buffer_CAN[21 + i] = power_off_array[i];
  }

  int a = serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
  // int frame_id = (0x01 << 8) | id[i];
  // Send_Msg(power_off_array, 8, frame_id);
  // Read_Msg(readArray);
}

void Motor_dm::Power_Off_All()
{
  for (int i = 1; i < 7; i++)
  {
    Power_Off_Single(i);
  }
}

/**
 * @brief 设置多个电机的零点校准
 * @param id 电机ID数组指针，包含需要校准的电机ID
 * @param len 电机数量，表示id数组的长度
 * @note 该方法会遍历所有指定电机，发送零点校准命令
 *       每个电机的校准命令通过CAN总线发送
 */
void Motor_dm::Set_Calibration(uint8_t id)
{
  uint32_t frame_id = (0x01 << 8) | id;
  Serial_Send_Buffer_CAN[13] = frame_id;
  Serial_Send_Buffer_CAN[14] = frame_id >> 8;
  Serial_Send_Buffer_CAN[15] = frame_id >> 16;
  Serial_Send_Buffer_CAN[16] = frame_id >> 24;

  for (int i = 0; i < 8; i++)
  {
    Serial_Send_Buffer_CAN[21 + i] = set_zero_array[i];
  }

  serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
  // for(int i = 0; i < 30; i++)
  //         std::cerr << std::hex << std::setw(2) << std::setfill('0')
  //               << (int)Serial_Send_Buffer_CAN[i] << std::endl;
}

void Motor_dm::Clear_Err(uint8_t id)
{

  uint32_t frame_id = (0x01 << 8) | id;
  Serial_Send_Buffer_CAN[13] = frame_id;
  Serial_Send_Buffer_CAN[14] = frame_id >> 8;
  Serial_Send_Buffer_CAN[15] = frame_id >> 16;
  Serial_Send_Buffer_CAN[16] = frame_id >> 24;

  for (int i = 0; i < 8; i++)
  {
    Serial_Send_Buffer_CAN[21 + i] = clear_err_array[i];
  }

  serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
}

/**
 * @brief 设置多个电机的目标角度
 * @param id 电机ID数组
 * @param len 电机数量
 * @param angles 各关节的最大最小角度范围 目标角度数组(单位：度)
 * @param speed 0~100 运动速度(单位：弧度度/秒)
 * @note 速度映射为1:各关节最大角度,随后又映射为弧度,最大可达到3.92rad/s    1:22.5°
 *       角度同样映射为弧度,在不限位的情况下可最大1s运动225°
 */
void Motor_dm::Set_Angles_Single(uint8_t id, float angles, float speed)
{
  uint8_t is_exceed_angles = Is_In_Position(id, angles);
  if (speed > 100)
    speed = 100;
  if (speed < 0)
    speed = 0;

  if (is_exceed_angles == 0)
  {
    float speed1 = speed * Speed_Mapping[id - 1]; // 映射对应的关节id的最大速度
    float rad = (angles / 180.00) * 3.14;
    float angles_speed = (speed1 / 180.00) * 3.14;
    Set_Rad(id, rad, angles_speed);
  }
  else
  {
    printf("Motor %d Angle Exceed Limit\n\r", is_exceed_angles);
    printf("Joint %d must in range %d to %d\n\r", is_exceed_angles, JOINT_MIN_ARR_DM[is_exceed_angles - 1], JOINT_MAX_ARR_DM[is_exceed_angles - 1]);
  }
  usleep(200);
}

void Motor_dm::Set_Angles_All(const std::vector<float> &angles, float speed)
{
  for (int i = 0; i < 6; i++)
  {
    Set_Angles_Single(i + 1, angles[i], speed);
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
 *       D:通讯丢失 E:过载 F:编译器自动填充值,也为一种异常状态
 */
uint8_t Motor_dm::Get_Status(uint8_t id)
{
    uint cnt = 0;
    uint32_t frame_id = (0x01 << 8) | id;

    Serial_Send_Buffer_CAN[13] = frame_id;
    Serial_Send_Buffer_CAN[14] = frame_id >> 8;
    Serial_Send_Buffer_CAN[15] = frame_id >> 16;
    Serial_Send_Buffer_CAN[16] = frame_id >> 24;

    for (int i = 0; i < 8; i++)
    {
      Serial_Send_Buffer_CAN[21 + i] = power_on_array[i];
    }

    int a = serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
    // std::cout << a << std::endl;
    usleep(1000);

    serial.flushReceiver();
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN)); // 发送数据

    while (true)
    {
      int byte_num = serial.readBytes(Serial_Recv_Buffer_CAN, sizeof(Serial_Recv_Buffer_CAN)); // 读取数据
      if (byte_num >= 0)
      {
        for (int i = 0; i < sizeof(Serial_Recv_Buffer_CAN); i++)
        {
          if (Serial_Recv_Buffer_CAN[i] == 0xAA && Serial_Recv_Buffer_CAN[i + 1] == 0x11 && Serial_Recv_Buffer_CAN[i + 2] == 0x08)
          {
            std::memcpy(data_buffer, &Serial_Recv_Buffer_CAN[i], 16);

            // for (int i = 0; i < sizeof(data_buffer); i++)
            // {
            //   std::cout << std::hex;
            //   std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(data_buffer[i]) << " ";
            // }
            //   std::cout << std::endl;
            unsigned int temp = data_buffer[7] & ~frame_id;
            // std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(data_buffer[7]) << " ";
            unsigned int motor_status = temp >> 4;
            std::cout <<  std::hex << motor_status << std::endl;
            std::cout <<  std::dec;
            std::memset(data_buffer, 0, 16);
            return motor_status;
          }
        }
      }
      else
      {
        std::cout << "error" << std::endl;
      }
    }
    usleep(200);
}

/**
 * @brief 获取多个电机的当前角度值
 * @param id 电机ID数组指针，包含需要查询的电机ID
 * @param len 电机数量，表示id数组的长度
 * @return Angles 返回包含所有电机角度值的数组
 * @note 该方法会遍历所有指定电机，通过Get_Angle()方法获取每个电机的角度
 *       角度单位为度，由弧度值转换而来(弧度转角度公式: 角度 = 弧度 * 180/π)
 */
Angles Motor_dm::Get_Angles_All()
{
  Angles angles;
  for (int i = 1; i < 7; i++)
  {
    Get_Angle_Single(i);
    // Serial.print(angles[i]);
    // Serial.print(" ");
  }
  // // Serial.println();
  // return angles;
}

/**
 * @brief 检查电机角度是否在允许范围内
 * @param ID 需要判断的电机ID,传入需要控制的ID序号数组(1~6)
 * @param id_len 电机ID长度
 * @param angles 要检查的角度数组
 * @return uint8_t 0表示所有角度在范围内，否则返回超出范围的电机ID
 */
uint8_t Is_In_Position(uint8_t id, float angles)
{
  switch (id)
  {
  case 1:
  {
    if (angles > J1_MIN && angles < J1_MAX)
      break;
    else
      return 1;
  }
  case 2:
  {
    if (angles > J2_MIN && angles < J2_MAX)
      break;
    else
      return 2;
  }
  case 3:
  {
    if (angles > J3_MIN && angles < J3_MAX)
      break;
    else
      return 3;
  }
  case 4:
  {
    if (angles > J4_MIN && angles < J4_MAX)
      break;
    else
      return 4;
  }
  case 5:
  {
    if (angles > J5_MIN && angles < J5_MAX)
      break;
    else
      return 5;
  }
  case 6:
  {
    if (angles > J6_MIN && angles < J6_MAX)
      break;
    else
      return 6;
  }
  }
  return 0;
}

/**
 * @brief 控制夹爪的开启和关闭
 * @param mode 0:关闭 1:开启
 */
void Motor_dm::Gripper_Control(uint8_t mode)
{
  uint8_t gripper_open_1[] = {0x07, 0xb5, 0x02, 0x0e, 0x06, 0x00, 0x0b, 0x00};
  uint8_t gripper_open_2[] = {0x05, 0xb5, 0x01, 0x64, 0xf9, 0x1c};

  uint8_t gripper_close_1[] = {0x07, 0xb5, 0x02, 0x0e, 0x06, 0x00, 0x0b, 0x00};
  uint8_t gripper_close_2[] = {0x05, 0xb5, 0x01, 0x00, 0Xf8, 0xf7};
  uint16_t frame_id = 0x0A;
  Serial_Send_Buffer_CAN[13] = 0x0A;
  Serial_Send_Buffer_CAN[14] = 0X0A >> 8;

  if (mode == 0)
  {
    Serial_Send_Buffer_CAN[18] = 0x08;

    for (int i = 0; i < 8; i++)
      Serial_Send_Buffer_CAN[i + 21] = gripper_close_1[i];
    //   for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //   std::cout << std::hex;
    //   std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
    // std::cout << std::endl;
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    Serial_Send_Buffer_CAN[18] = 0x06;
    for (int i = 0; i < 6; i++)
      Serial_Send_Buffer_CAN[i + 21] = gripper_close_2[i];

    Serial_Send_Buffer_CAN[27] = 0x00;
    Serial_Send_Buffer_CAN[28] = 0x00;

    //     for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //   std::cout << std::hex;
    //   std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
    // std::cout << std::endl;
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
  }

  else if (mode == 1)
  {
    Serial_Send_Buffer_CAN[18] = 0x08;
    for (int i = 0; i < 8; i++)
    {
      Serial_Send_Buffer_CAN[i + 21] = gripper_open_1[i];
    }

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //   std::cout << std::hex;
    //   std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
    //   std::cout << std::endl;
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    Serial_Send_Buffer_CAN[18] = 0x06;
    for (int i = 0; i < 6; i++)
    {
      Serial_Send_Buffer_CAN[i + 21] = gripper_open_2[i];
    }

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //   std::cout << std::hex;
    //   std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
    //   std::cout << std::endl;
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
  }

  Serial_Send_Buffer_CAN[18] = 0x08;
}
