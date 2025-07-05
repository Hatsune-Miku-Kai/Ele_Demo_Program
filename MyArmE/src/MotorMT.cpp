#include "MotorMT.h"
#include <array>
#include "CanCommu.h"
#include "MotorMTCmd.h"

int JOINT_MAX_ARR[6] = {J1_MAX, J2_MAX, J3_MAX, J4_MAX, J5_MAX, J6_MAX};
int JOINT_MIN_ARR[6] = {J1_MIN, J2_MIN, J3_MIN, J4_MIN, J5_MIN, J6_MIN};

CanCommu CAN;

/**
 * @brief 检查电机角度是否在允许范围内
 * @param ID 需要判断的电机ID,传入需要控制的ID序号数组(1~6)
 * @param id_len 电机ID长度
 * @param angles 要检查的角度数组
 * @return uint8_t 0表示所有角度在范围内，否则返回超出范围的电机ID
 */
uint8_t MotorMT::Is_In_Position(uint8_t *ID, uint8_t id_len, float *angles)
{
    for(int i = 0; i < id_len; i++)
    {
        switch(ID[i])
        {
            case 1:
            {
                if(angles[0] > J1_MIN && angles[0] < J1_MAX)
                    break;
                else 
                    return 1;
            }
            case 2:
            {
                if(angles[1] > J2_MIN && angles[1] < J2_MAX)
                    break;
                else 
                    return 2;
            }
            case 3:
            {
                if(angles[2] > J3_MIN && angles[2] < J3_MAX)
                    break;
                else 
                    return 3;
            }
            case 4:
            {
                if(angles[3] > J4_MIN && angles[3] < J4_MAX)
                    break;
                else 
                    return 4;
            }
            case 5:
            {
                if(angles[4] > J5_MIN && angles[4] < J5_MAX)
                    break;
                else 
                    return 5;
            }
            case 6:
            {
                if(angles[5] > J6_MIN && angles[5] < J6_MAX)
                    break;
                else 
                    return 6;
            }
        }
    }
    return 0;

}

/**
 * @brief MotorMT类的构造函数
 * @details 初始化CAN通信，设置CAN接口引脚和波特率
 * @note 波特率设置为1Mbps(1000000)
 */
MotorMT::MotorMT()
{
    CAN.InitCan(CAN_RX, CAN_TX, 1000000);
}

MotorMT::~MotorMT()
{

}
/**
 * @brief 将4个连续的uint8_t数据转换为int32_t
 * @param arr 包含uint8_t数组的pair对象，first为长度，second为数据数组
 * @param pos 起始位置索引(指向最高字节)
 * @return int32_t 转换后的32位有符号整数
 * @note 数据排列方式为大端序(Big-Endian):
 *       arr.second[pos]    - 最高字节(MSB)
 *       arr.second[pos-1]  - 次高字节
 *       arr.second[pos-2]  - 次低字节
 *       arr.second[pos-3]  - 最低字节(LSB)
 */
int32_t Uint8_tIntoInt32_t(std::pair<int, vector<uint8_t>>  &arr, uint8_t pos)
{
    int32_t temp = 0; 
    temp = arr.second[pos] << 24 | arr.second[pos -1] << 16 | arr.second[pos - 2] << 8 | arr.second[pos - 3];
    return temp;
}


/**
 * @brief 角度映射函数，将角度值转换为CAN通信所需的字节数组
 * @param angles 角度数组指针，每个元素代表一个关节的角度值(度)
 * @param len 角度数组长度(电机数量)
 * @return std::vector<uint8_t> 转换后的字节数组，每个角度占4个字节
 * @note 映射规则: 1度 = 100个内部单位 (0x0064)
 *       数据格式: 小端序(Little-Endian)
 *       负数处理: 使用0xFFFF表示负值
 */
std::vector<uint8_t> Angular_Mapping(float * angles, uint8_t len)
{
    std::vector<uint8_t> temp_angles;
    for(int i = 0; i < len; i++)
    {
        
        int32_t temp = angles[i] * 100;
        temp_angles.push_back(temp & 0xFF);       
        temp_angles.push_back((temp >> 8) & 0xFF); 
        if(temp < 0)
        {
            temp_angles.push_back(0xFF); 
            temp_angles.push_back(0xFF); 
        }

        else
        {
            temp_angles.push_back((temp >> 16) & 0xFF); 
            temp_angles.push_back((temp >> 24) & 0xFF); 
        }

    }
    return temp_angles;
}

/**
 * @brief 速度映射函数，将速度百分比转换为CAN通信所需的字节数组
 * @param speed 速度百分比值(0-100)
 * @return std::vector<uint8_t> 转换后的字节数组，包含2个字节
 * @note 映射规则: 
 *       1. 速度限制在0-100范围内
 *       2. 实际速度值 = 速度百分比 × 1.8
 *       3. 数据格式: 小端序(Little-Endian)
 *       4. 最大速度值: 65535 (0xFFFF)考虑到速度过快,先使用小速度进行测试
 */
std::vector<uint8_t> Speed_Mapping(int speed)
{
    if(speed > 100)
        speed = 100;
    std::vector<uint8_t> speeds(2);
    uint16_t temp = speed * 1.8;
    speeds[0] = temp & 0xFF;

    speeds[1] = (temp >> 8) & 0XFF;

    //printf("speed[0] = %x\n\r",speeds[0]);
    //printf("speed[1] = %x\n\r",speeds[1]);
    return speeds;
}


/**
 * @brief 设置多个电机的目标角度
 * @param ID 电机ID数组指针，每个元素代表一个电机的ID,传入需要控制的ID序号数组(1~6)
 * @param id_len 电机ID长度
 * @param angles 目标角度数组(单位：度)
 * @param speed 运动速度(0-100百分比)
 * @note 该函数会：
 *       1. 检查角度是否在允许范围内
 *       2. 将角度和速度转换为CAN通信格式
 *       3. 通过CAN总线发送控制命令
 *       4. 若角度超限会打印错误信息
 */
void MotorMT ::Set_Motor_Angles(uint8_t *ID, uint8_t id_len, float *angles, int speed)
{
    uint8_t is_exceed_angles = Is_In_Position(ID, id_len, angles);
    if(is_exceed_angles == 0)
    {
        uint8_t i = 0;
        uint8_t send_buffer[8] = {0};
        send_buffer[0] = SET_ANGLE;

        std::vector<uint8_t> speed_temp = Speed_Mapping(speed);
        send_buffer[2] = speed_temp[0];
        send_buffer[3] = speed_temp[1];

        std::vector<uint8_t> angles_temp = Angular_Mapping(angles, id_len);
        for (; i < id_len; i++)
        {
            send_buffer[4] = angles_temp[i * 4];
            send_buffer[5] = angles_temp[i * 4 + 1];
            send_buffer[6] = angles_temp[i * 4 + 2];
            send_buffer[7] = angles_temp[i * 4 + 3];
            CAN.SendMsg(send_buffer, 8, ID[i] + SINGLE_MOTOR_CMD);
        }  
    }
    else
    {
        printf("Motor %d Angle Exceed Limit\n\r", is_exceed_angles);
        printf("Joint %d must in range %d to %d\n\r", is_exceed_angles, JOINT_MIN_ARR[is_exceed_angles - 1], JOINT_MAX_ARR[is_exceed_angles - 1]);
    }
    
}

/**
 * @brief 获取多个电机的当前角度
 * @param ID 电机ID数组指针，每个元素代表一个电机的ID(1~6)
 * @param id_len 电机数量
 * @return Angles 包含所有电机当前角度的数组(单位：度)
 * @note 该函数会：
 *       1. 通过CAN总线读取每个电机的角度数据
 *       2. 将原始数据转换为实际角度值(除以100)
 *       3. 将角度值存入对应ID的位置
 */
Angles MotorMT::Get_Motor_Angles(uint8_t *ID, uint8_t id_len)
{
    std::pair<int, vector<uint8_t>> Read_Buffer;
    Read_Buffer.second.resize(8);
    Angles angles;
    uint8_t i = 0;
    uint8_t send_buffer[8] = {0};
    send_buffer[0] = READ_ANGLE;
    for (; i < id_len; i++)
    {
        std::pair<int, vector<uint8_t>> Read_Buffer;
        Read_Buffer.second.resize(8);
        CAN.ReadMsg(send_buffer, 8, ID[i] + SINGLE_MOTOR_CMD, Read_Buffer, false);

        //printf("%x\n\r ",Read_Buffer.second[7]);

        //printf("%x\n\r ",Read_Buffer.second[6]);

        angles[ID[i] - 1] = (float)Uint8_tIntoInt32_t(Read_Buffer, 7) / 100;//让角度在对应的位置

        printf("%0.2f\n\r ",angles[i]);

        //printf("\n\r");
    }
    return angles;
}
/**
 * @brief 获取多个电机的错误状态
 * @param ID 电机ID数组指针，每个元素代表一个电机的ID(1~6)
 * @param id_len 电机数量
 * @return Error 包含所有电机错误状态的数组
 * @note 错误状态格式：
 *       1. 通过CAN总线读取错误码
 *       2. 错误码由两个字节组成(Read_Buffer.second[7]和[6])
 *       3. 返回的错误码为16位数值
 *       4. 多个错误出现时,错误码会进行叠加,如0x02错误 + 0x03错误 = 0x05
 *       5. 错误状态参考文件--伺服电机控制协议V4.3--中文版.pdf P28
 */
Error MotorMT::Get_Motor_Errors(uint8_t *ID, uint8_t id_len)
{
    std::pair<int, vector<uint8_t>> Read_Buffer;
    Read_Buffer.second.resize(8);
    Error error;
    uint8_t i = 0;
    uint8_t send_buffer[8] = {0};
    send_buffer[0] = READ_ERROR; // Read the error of motor
    for (; i < id_len; i++)
    {
        CAN.ReadMsg(send_buffer, 8, ID[i] + SINGLE_MOTOR_CMD, Read_Buffer, false);
        error[i] = Read_Buffer.second[7] << 8 | Read_Buffer.second[6];
        //printf("%x\n\r",error[i]);
    }
    
    return error;
}

void MotorMT::Power_on()
{
    uint8_t send_buffer[8] = {0};
    send_buffer[0] = POWER_ON;
    CAN.SendMsg(send_buffer, 8, ALL_MOTOR_CMD);
}


/**
 * @brief 关闭所有电机电源,此时电机会释放
 * @note 该函数会：
 *       1. 发送断电指令到所有电机
 *       2. 使用广播地址(ALL_MOTOR_CMD)同时控制所有电机
 *       3. 断电后电机将停止运行并进入待机状态
 */
void MotorMT::Power_off()
{
    uint8_t send_buffer[8] = {0};
    send_buffer[0] = POWER_OFF;
    CAN.SendMsg(send_buffer, 8, ALL_MOTOR_CMD);
}

/**
 * @brief 修改单个电机的ID号
 * @param New_ID 新的电机ID号(1-32)
 * @note 该函数会：
 *       1. 发送修改ID指令到当前连接的电机
 *       2. 使用特殊ID 0x300进行通信
 *       3. 新ID号会被拆分为高低字节分别存储
 *       4. 修改后需要重新上电才能生效
 * @warning 应单个电机修改,否则可能出现所有电机ID都被修改的情况
 */
void MotorMT::Change_Motor_ID(uint8_t New_ID)
{
    uint8_t send_buffer[8] = {0};

    send_buffer[0] = CHANGE_Motor_ID;
    send_buffer[6] = New_ID << 8;
    send_buffer[7] = New_ID;
    CAN.SendMsg(send_buffer, 8, 0x300);
}


/**
 * @brief 修改电机的PID参数
 * @param ID 电机ID数组指针(1~6)
 * @param id_len 电机数量
 * @param New_P 新的比例参数
 * @param New_I 新的积分参数
 * @param New_D 新的微分参数
 * @param mode PID模式：
 *             1 - 电流环(调整Kp和Ki)
 *             2 - 速度环(调整Kp和Ki) 
 *             3 - 位置环(调整Kp、Ki和Kd)
 * @note 该函数会：
 *       1. 根据模式选择不同的PID参数调整方式
 *       2. 将浮点数参数转换为4字节数据格式
 *       3. 通过CAN总线发送修改指令
 *       4. 断电后参数不会保存
 */
void MotorMT::Change_Motor_PID(uint8_t *ID, uint8_t id_len, float New_P, 
                                float New_I, float New_D, uint8_t mode)
{
 uint8_t send_buffer[8] = {0};
    send_buffer[0] = CHANGE_Motor_PID;
    uint32_t P_value = *reinterpret_cast<uint32_t*>(&New_P);
    uint32_t I_value = *reinterpret_cast<uint32_t*>(&New_I);
    uint32_t D_value = *reinterpret_cast<uint32_t*>(&New_D);
    while(id_len)
    {
        if(mode == 1 || mode == 2)
    {
        if (New_P != P)
        {
            send_buffer[1] = 0x01;
            for (uint8_t i = 0; i < 4; i++)
            {
                send_buffer[i + 4] = (P_value >> (i * 8)) & 0xFF; 
            }
            CAN.SendMsg(send_buffer, 8, ID[id_len - 1] + SINGLE_MOTOR_CMD);
                
        }

        if (New_I != I)
        {
            {
                send_buffer[1] = 0x02;
                for (uint8_t i = 0; i < 4; i++)
                {
                    send_buffer[i + 4] = (I_value >> (i * 8)) & 0xFF; 
                }
                CAN.SendMsg(send_buffer, 8, ID[id_len - 1] + SINGLE_MOTOR_CMD);
                
            }
        }

    }

    else if(mode == 3)
    {
        if (New_P != P)
        {
            send_buffer[1] = 0x01;
            for (uint8_t i = 0; i < 4; i++)
            {
                send_buffer[i + 4] = (P_value >> (i * 8)) & 0xFF; 
            }
            CAN.SendMsg(send_buffer, 8, ID[id_len - 1] + SINGLE_MOTOR_CMD);
                
        }

        if (New_I != I)
        {
            {
                send_buffer[1] = 0x02;
                for (uint8_t i = 0; i < 4; i++)
                {
                    send_buffer[i + 4] = (I_value >> (i * 8)) & 0xFF; 
                }
                CAN.SendMsg(send_buffer, 8, ID[id_len - 1] + SINGLE_MOTOR_CMD);
                
            }
        }

        if (New_D != D)
        {
            {
                send_buffer[1] = 0x02;
                for (uint8_t i = 0; i < 4; i++)
                {
                    send_buffer[i + 4] = (D_value >> (i * 8)) & 0xFF; 
                }
                CAN.SendMsg(send_buffer, 8, ID[id_len - 1] + SINGLE_MOTOR_CMD);
                
            }
        }

    }
        id_len --;
    }

}

/**
 * @brief 立即停止所有电机运行
 * @note 该函数会：
 *       1. 发送紧急停止指令到所有电机
 *       2. 使用广播地址(ALL_MOTOR_CMD)同时控制所有电机
 *       3. 电机将立即停止并保持当前位置
 *       4. 不同于断电(Power_off)，电机仍保持通电状态,电机会处于制动状态
 */
void MotorMT::Stop_Run()
{
    uint8_t send_buffer[8] = {0};

    send_buffer[0] = STOP_RUN;

    CAN.SendMsg(send_buffer, 8, ALL_MOTOR_CMD);

}


/**
 * @brief 设置电机的零位校准位置
 * @param ID 电机ID数组指针(1~6)
 * @param id_len 电机数量
 * @note 该函数会：
 *       1. 发送设置校准指令到指定电机
 *       2. 将当前电机位置设置为零位
 *       3. 最后发送复位指令使设置生效
 *       4. 校准后需要重新上电才能永久保存
 */
void MotorMT::Set_Calibration(uint8_t *ID, uint8_t id_len)
{
    uint8_t send_buffer[8] = {0};
    send_buffer[0] = SET_CALIBRATION;
    for( int i = 0; i < id_len; i++)
    {
        CAN.SendMsg(send_buffer, 8, ID[i] + SINGLE_MOTOR_CMD);
    }
    send_buffer[0] = RESET;
    CAN.SendMsg(send_buffer, 8, ALL_MOTOR_CMD);
}


/**
 * @brief 读取当前连接的电机ID号
 * @return uint8_t 返回读取到的电机ID号(1-32)
 * @note 该函数会：
 *       1. 发送读取ID指令到特殊地址0x300
 *       2. 等待并读取电机的响应数据
 *       3. 从响应数据中解析出电机ID号
 *       4. 返回解析得到的ID值
 */
uint8_t MotorMT::Read_Motor_ID()
{
    std::pair<int, vector<uint8_t>> Read_Buffer;
    Read_Buffer.second.resize(8);
    uint8_t send_buffer[8] = {0};
    send_buffer[0] = CHANGE_Motor_ID;
    send_buffer[2] = 0x01;
    CAN.SendMsg(send_buffer,8,300);
    CAN.ReadMsg(send_buffer, 8, 300, Read_Buffer, false);
    uint8_t ID = Read_Buffer.second[7] << 8 | Read_Buffer.second[6];
    return ID;
}

/**
 * @brief 读取电机的校准位置数据(实际上不需要使用,读取的是距离硬件零位的偏移值,与软件定义的零位无关)
 * @param ID 电机ID数组指针(1~6)
 * @param id_len 电机数量
 * @return Angles 包含所有电机校准位置的数组
 * @note 该函数会：
 *       1. 发送读取校准位置指令到每个电机
 *       2. 接收并解析电机的响应数据
 *       3. 将校准位置存入对应ID的位置
 *       4. 返回包含所有电机校准位置的数组
 * @warning 读取的校准位置是电机内部的原始数据，需要根据实际应用进行转换
 */
Angles MotorMT::Read_Calibration(uint8_t *ID, uint8_t id_len)
{
    Angles pos;
    uint8_t send_buffer[8] = {0};
    std::pair<int, vector<uint8_t>> Read_Buffer;
    Read_Buffer.second.resize(8);
    send_buffer[0] = READ_CALIBRATION;
    for( int i = 0; i < id_len; i++)
    {
        CAN.ReadMsg(send_buffer, 8, ID[i] + SINGLE_MOTOR_CMD, Read_Buffer, false);
        pos[ID[i] - 1] = Uint8_tIntoInt32_t(Read_Buffer, 7); 
        printf("\n\r"); 
    }
    return pos;
}

/**
 * @brief 获取电机的PID参数
 * @param ID 电机ID数组指针(1~6)
 * @param id_len 电机数量
 * @param mode PID模式：
 *             1 - 电流环PID参数
 *             2 - 速度环PID参数
 *             3 - 位置环PID参数
 * @return PID 包含所有电机PID参数的数组
 * @note 该函数会：
 *       1. 发送读取PID参数指令到每个电机
 *       2. 接收并解析电机的响应数据
 *       3. 将PID参数存入对应ID的位置
 *       4. 返回包含所有电机PID参数的数组
 */
PID MotorMT:: Get_Motor_PID(uint8_t *ID, uint8_t id_len, uint8_t mode)
{
    PID pid;
    uint8_t send_buffer[8] = {0};
    std::pair<int, vector<uint8_t>> Read_Buffer;
    Read_Buffer.second.resize(8);
    send_buffer[0] = READ_MOTOR_PID;
    send_buffer[1] = mode;
    for( int i = 0; i < id_len; i++)
    {
        CAN.ReadMsg(send_buffer, 8, ID[i] + SINGLE_MOTOR_CMD, Read_Buffer, false);
        pid[ID[i] - 1] = (float)Uint8_tIntoInt32_t(Read_Buffer, 7) / 1e9;
        printf("%0.2f\n\r",pid[i]);
    }
    return pid;
}