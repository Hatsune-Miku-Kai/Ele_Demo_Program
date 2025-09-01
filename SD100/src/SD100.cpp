#include "SD100.h"
#include "serialib.h"

uint8_t Recv_Buffer[32] = {0};

uint8_t Data_Buffer[16] = {0};

uint8_t Set_Pos_Mode_Array[8] = {0x2f, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00};

uint8_t Power_On_Array[8] = {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};

uint8_t Power_Off_Array[8] = {0x2B, 0x40, 0x60, 0x00, 0x07, 0x00, 0x00, 0x00};

uint8_t Set_Pos_Array[8] = {0x23, 0x7a, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t Run_Array[8] = {0x2B, 0x40, 0x60, 0x00, 0x1f, 0x00, 0x00, 0x00};

uint8_t Read_Pos_Array[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t Set_Speed_Array[8] = {0x23, 0x81, 0x60, 0x00, 0x50, 0xC3, 0x00, 0x00};

uint8_t Set_Acc_Array[8] = {0x23, 0x83, 0x60, 0x00, 0x50, 0xC3, 0x00, 0x00};

uint8_t Set_Dec_Array[8] = {0x23, 0x84, 0x60, 0x00, 0x50, 0xC3, 0x00, 0x00};

uint8_t Stop_Array[8] = {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00};

uint8_t Clear_Err_Array[8] = {0x2B, 0x40, 0x60, 0x00, 0x80, 0x00, 0x00, 0x00};

// uint8_t Set_Calibration_Array[8] = {0x2b, 0x40, 0x60, 0x00, 0x04, 0x00, 0x00, 0x00};

// uint8_t Read_Offset_Array[8] = {0x40, 0x7c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t Clear_Odd_Pos_Array[8] = {0x23, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t Serial_Send_Buffer_CAN[30] = {0x55, 0xAA, 0x1E, 0x01, 0X01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xFF,
                                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x88}; // 13                                            //21

serialib serial;

void Init_Serial()
{
    int a = serial.openDevice("/dev/ttyACM0", 921600, SERIAL_DATABITS_8, SERIAL_PARITY_NONE, SERIAL_STOPBITS_1);
    if (a == -1)
    {
        std::cerr << "无法打开串口" << std::endl;
    }

    else
    {
        std::cerr << "成功打开串口" << std::endl;
    }

    sleep(2);
}

SD100::SD100()
{
    Init_Serial();
}

SD100::~SD100()
{
}

// 总行程1m,螺纹杆为10mm一圈,因此总共行进100圈,可将10mm作为最小的行进单位,映射为0~100,同时,电机转一圈的脉冲数为2500,因此可得映射关系为10mm:2500
void SD100::Set_Pos(uint8_t ID, int32_t len, uint32_t speed, uint32_t acc, uint32_t dec)
{
    std::cout << "sss" << std::endl;

    Power_On(ID);
    usleep(800);
    std::cout << std::endl;

    uint16_t frame_id = (6 << 8) | ID;
    Serial_Send_Buffer_CAN[13] = frame_id;
    Serial_Send_Buffer_CAN[14] = frame_id >> 8;

    // for (int i = 0; i < 8; i++)
    // {
    //     Serial_Send_Buffer_CAN[i + 21] = Clear_Odd_Pos_Array[i];
    // }
    // serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    // usleep(500);

    Set_Mode(ID, 0x01); // 默认设置为0x01,轮廓位置模式,需要传入位置,速度,加/减速度
    usleep(800);
    std::cout << std::endl;

    Set_Acc(ID, acc);
    usleep(800);
    std::cout << std::endl;

    Set_Dec(ID, dec);
    usleep(800);
    std::cout << std::endl;

    Set_Speed(ID, speed);
    usleep(800);
    std::cout << std::endl;

    int32_t paulse = len * 2500;
    if (paulse > 150 * 2500)
        paulse = 150 * 2500;

    if (paulse < -150 * 2500)
        paulse = -150 * 2500;

    Set_Pos_Array[4] = paulse & 0x000000FF;
    Set_Pos_Array[5] = (paulse & 0x0000FF00) >> 8;
    Set_Pos_Array[6] = (paulse & 0x00FF0000) >> 16;
    Set_Pos_Array[7] = (paulse & 0xFF000000) >> 24;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Set_Pos_Array[i];
    }

    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
    // std::cout << std::endl;

    Run(ID);
    usleep(800);
    std::cout << std::endl;
}

// void SD100::Power_Off(uint8_t ID)
// {
//     uint16_t id = (0x06 << 8) | ID;
//     Serial_Send_Buffer_CAN[13] = id;
//     Serial_Send_Buffer_CAN[14] = id >> 8;

//     for (int i = 0; i < 8; i++)
//     {
//         Serial_Send_Buffer_CAN[i + 21] = Power_Off_Array[i];
//     }

//     serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

//     // Serial_Send_Buffer_CAN[25] = 0x07;
//     // serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
//     // usleep(500);

//     // Serial_Send_Buffer_CAN[25] = 0x0F;
//     // serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
//     // usleep(500);

//     // Serial_Send_Buffer_CAN[25] = 0x1F;
//     // serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
//     // usleep(500);
// }

void SD100::Power_On(uint8_t ID)
{
    uint16_t id = (0x06 << 8) | ID;
    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Power_On_Array[i];
    }

    Serial_Send_Buffer_CAN[25] = 0x06;
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
    usleep(800);

    Serial_Send_Buffer_CAN[25] = 0x07;
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
    usleep(800);
    Serial_Send_Buffer_CAN[25] = 0x0F;
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
    usleep(800);

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
}

int32_t SD100::Read_Pos(uint8_t ID)
{
    uint16_t id = (0x06 << 8) | ID;
    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Read_Pos_Array[i];
    }

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }

    serial.flushReceiver();
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
    while(true)
    {
        int byte_num = serial.readBytes(Recv_Buffer, sizeof(Recv_Buffer));
        // uint16_t temp = Recv_Buffer[4] << 8 | Recv_Buffer[3];
        if (byte_num >= 8)
        {
            for (int i = 0; i < sizeof(Recv_Buffer); i++)
            {
                if (Recv_Buffer[i] == 0xAA && Recv_Buffer[i + 1] == 0x11) //&& Recv_Buffer[i + 8] == 0x64)// && Recv_Buffer[i + 9] == 0x60 && Recv_Buffer[i + 15] == 0x55)
                {
                    std::memcpy(Data_Buffer, &Recv_Buffer[i], 16);
    
                    // std::cout << "581" << std::endl;
                    // for (int i = 0; i < sizeof(Data_Buffer); i++)
                    // {
                    //     std::cout << std::hex;
                    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Data_Buffer[i]) << " ";
                    // }
                    // std::cout << std::endl;
                    int32_t pos = 0;
                    pos = Data_Buffer[11] | (Data_Buffer[12] << 8) | (Data_Buffer[13] << 16) | (Data_Buffer[14] << 24);
                    std::cout << pos << std ::endl;
                    return pos;
                }
            }
        }
    
        else
            std::cout << "error" << std::endl;
    }

}

void SD100::Set_Speed(uint8_t ID, uint32_t speed)
{
    uint16_t id = (0x06 << 8) | ID;
    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    Set_Speed_Array[4] = speed & 0x000000FF;
    Set_Speed_Array[5] = (speed & 0x0000FF00) >> 8;
    Set_Speed_Array[6] = (speed & 0x00FF0000) >> 16;
    Set_Speed_Array[7] = (speed & 0xFF000000) >> 24;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Set_Speed_Array[i];
    }
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
}

void SD100::Set_Acc(uint8_t ID, uint32_t acc)
{
    uint16_t id = (0x06 << 8) | ID;
    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    Set_Acc_Array[4] = acc & 0x000000FF;
    Set_Acc_Array[5] = (acc & 0x0000FF00) >> 8;
    Set_Acc_Array[6] = (acc & 0x00FF0000) >> 16;
    Set_Acc_Array[7] = (acc & 0xFF000000) >> 24;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Set_Acc_Array[i];
    }
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
}

void SD100::Set_Dec(uint8_t ID, uint32_t dec)
{
    uint16_t id = (0x06 << 8) | ID;
    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    Set_Dec_Array[4] = dec & 0x000000FF;
    Set_Dec_Array[5] = (dec & 0x0000FF00) >> 8;
    Set_Dec_Array[6] = (dec & 0x00FF0000) >> 16;
    Set_Dec_Array[7] = (dec & 0xFF000000) >> 24;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Set_Dec_Array[i];
    }
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));


    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
}

void SD100::Set_Mode(uint8_t ID, uint8_t mode = 0x01)

{
    uint16_t id = (0x06 << 8) | ID;
    Set_Pos_Mode_Array[4] = mode;

    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;
    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Set_Pos_Mode_Array[i];
    }
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
}

void SD100::Run(uint8_t ID)
{
    uint16_t id = (0x06 << 8) | ID;

    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Run_Array[i];
    }
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

    usleep(800);
    // for (int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
    // {
    //     std::cout << std::hex;
    //     std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
    // }
}

void SD100::Stop(uint8_t ID)
{
    uint16_t id = (0x06 << 8) | ID;

    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Stop_Array[i];
    }
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
    usleep(800);
}

void SD100::Clear_Err(uint8_t ID)
{
    uint16_t id = (0x06 << 8) | ID;

    Serial_Send_Buffer_CAN[13] = id;
    Serial_Send_Buffer_CAN[14] = id >> 8;

    for (int i = 0; i < 8; i++)
    {
        Serial_Send_Buffer_CAN[i + 21] = Stop_Array[i];
    }
    serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
    usleep(800);
}

// void SD100::Set_Calibration(uint8_t ID)
// {
//     uint16_t id = (0x06 << 8) | ID;
//     Serial_Send_Buffer_CAN[13] = id;
//     Serial_Send_Buffer_CAN[14] = id >> 8;

//     int32_t pos = 0 - Read_Pos(ID);
//     Set_Calibration_Array[4] = pos & 0xFF;
//     Set_Calibration_Array[5] = (pos >> 8) & 0xFF;
//     Set_Calibration_Array[6] = (pos >> 16) & 0xFF;
//     Set_Calibration_Array[7] = (pos >> 24) & 0xFF;

//     for (int i = 0; i < 8; i++)
//     {
//         Serial_Send_Buffer_CAN[i + 21] = Set_Calibration_Array[i];
//     }
//     serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

//     Power_On(1);
// }

// int32_t SD100::Read_Offset(uint8_t ID)
// {
//     uint16_t id = (0x06 << 8) | ID;
//     Serial_Send_Buffer_CAN[13] = id;
//     Serial_Send_Buffer_CAN[14] = id >> 8;
//     for (int i = 0; i < 8; i++)
//     {
//         Serial_Send_Buffer_CAN[i + 21] = Read_Offset_Array[i];
//     }
//     serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));
//     usleep(800);
//     serial.readBytes(Recv_Buffer, sizeof(Recv_Buffer));
//         for (int i = 0; i < sizeof(Recv_Buffer); i++)
//         {
//             std::cout << std::hex;
//             std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Recv_Buffer[i]) << " ";
//         }
//     int32_t offset = (Recv_Buffer[14] << 24) | (Recv_Buffer[13] << 16) | (Recv_Buffer[12] << 8) | Recv_Buffer[11];
//     return offset;
// }

// void SD100::Set_Up()
// {
//     Serial_Send_Buffer_CAN[13] = 0x00;
//     Serial_Send_Buffer_CAN[14] = 0x00;
//     Serial_Send_Buffer_CAN[15] = 0x00;
//     Serial_Send_Buffer_CAN[16] = 0x00;

//     Serial_Send_Buffer_CAN[18] = 0x02;

//     Serial_Send_Buffer_CAN[21] = 0x01;
//     Serial_Send_Buffer_CAN[22] = 0x00;
//     Serial_Send_Buffer_CAN[23] = 0x00;
//     Serial_Send_Buffer_CAN[24] = 0x00;
//     Serial_Send_Buffer_CAN[25] = 0x00;
//     Serial_Send_Buffer_CAN[26] = 0x00;
//     Serial_Send_Buffer_CAN[27] = 0x00;
//     Serial_Send_Buffer_CAN[28] = 0x00;
//     serial.writeBytes(Serial_Send_Buffer_CAN, sizeof(Serial_Send_Buffer_CAN));

//     for(int i = 0; i < sizeof(Serial_Send_Buffer_CAN); i++)
//     {
//         std::cout << std::hex;
//         std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(Serial_Send_Buffer_CAN[i]) << " ";
//     }
//     Serial_Send_Buffer_CAN[18] = 0x08;
// }