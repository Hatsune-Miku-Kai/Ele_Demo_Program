#include "Motor_dm.h"
#include "CanCommu.h"

Motor_dm DM;


uint8_t motorIDs[] = {1,2,3,4,5,6,7} ;// 6个电机的ID

float Zero[7] = {0, 0, 0, 0, 0, 0, 0};

float Action_1[7] = {89, -87, 0, 1, 0, 0, 50};

float Action_2[7] = {83, 50, 8, -21, 2.8, 4.7, 0};

float Action_3[7] = {-128, 80, 2, 3, 7, 2.8, 50};

float Action_4[7] = {-76, -75, 159, 129, 50, -113, 0};

float Action_5[7] = {10, 8, -145, -5, -125, 117, 50};

void IoInit();
void setup()
{
    // delay(8000);
    IoInit();

    // delay(10000);
    // DM.Set_Calibration(motorIDs,7);


    Serial.begin(115200);

    delay(2000);
    DM.Power_On(motorIDs,7);

}

void loop()
{

    // delay(1000);
    // DM.Get_Angles(motorIDs,7);
    DM.Set_Angles(motorIDs, 7, Zero, 10);
    delay(10000);

    DM.Set_Angles(motorIDs, 7, Action_1, 10);
    delay(10000);

    DM.Set_Angles(motorIDs, 7, Action_2, 10);
    delay(10000);

    DM.Set_Angles(motorIDs, 7, Action_3, 10);
    delay(10000);

    DM.Set_Angles(motorIDs, 7, Action_4, 10);
    delay(10000);

    DM.Set_Angles(motorIDs, 7, Action_5, 10);
    delay(10000);
    


}

void IoInit()
{
    int inIO[7] = {34, 35, 39, 38, 37, 36, 21};
    int outIO[12] ={25, 26, 22, 4, 12, 19, 23, 18, 14, 27, 13};
    int set_out_io[6] = {27, 14, 13, 18, 23, 19};
    bool out_pin_state = true; //output io state

     for (int i = 0; i < 12; i++) {
        if (i < 7) {
            pinMode(inIO[i], INPUT_PULLUP);
            delay(20);
        }
        pinMode(outIO[i], OUTPUT);
        delay(20); 
    }
    //mode设置为NPN模式
    digitalWrite(25, 1);
    delay(200);
    digitalWrite(26, 0);
    delay(200);
    digitalWrite(22, 1);
    delay(200);
    digitalWrite(4, 1);//上电
    delay(200);

    //设置6个输出引脚为低电平 1是低电平
    for (int i = 0; i < 6; i++) {
        out_pin_state = false;
        digitalWrite(set_out_io[i], out_pin_state);
        delay(200);
    }
}