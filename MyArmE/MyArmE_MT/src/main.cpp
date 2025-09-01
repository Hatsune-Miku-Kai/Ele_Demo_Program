#include "MotorMT.h"
#include "CanCommu.h"




MotorMT MT;

uint8_t motorIDs[] = {1,2,3,4,5,6} ;// 6个电机的ID
uint8_t direction = 0; // 方向
Angles temp;
float angles[] = {0,0,0,0,0,0};
float angles2[] = {-34.3,50.5,95.3,-50.8,-15.5,0};
float angles3[] = {69.4,11.3,60.7,32.5,-9,0};
float angles4[] = {74.9, 10.7, 131.8, 30.3, 14.3, 0};
float angles5[] = {-28.5, 15.5, -69.5,53,-7, 0 };

uint8_t Calibration[] = {1,2,3,4,5,6};

void setup()
{
    
    //Serial.begin(115200);
    //MT.Power_on();

    //Serial.println("debug_state: " + String(debug::debug_state));
    //MT.Stop_Run();
    //MT.Power_on();
    // MT.Stop_Run();
    // delay(2000);
    // temp = MT.Get_Motor_Angles(motorIDs,6);
    // //MT.Set_Motor_Angles(motorIDs,6,angles, 50,0);
    // //MT.Set_Calibration(Calibration, 6);
    // //delay(5000);

    MT.Set_Motor_Angles(motorIDs,6,angles,20);
    delay(2000);

    MT.Set_Motor_Angles(motorIDs,6,angles2,20);
    delay(2000);

    MT.Set_Motor_Angles(motorIDs,6,angles3,20);
    delay(2000);

    MT.Set_Motor_Angles(motorIDs,6,angles4,20);
    delay(2000);

    MT.Set_Motor_Angles(motorIDs,6,angles5,20);
    delay(3000);

    MT.Set_Motor_Angles(motorIDs,6,angles,20);

    delay(3000);

    MT.Stop_Run();

}

void loop()
{
    //MT.Stop_Run();
    delay(2000);
    //Serial.println("debug_state: " + String(debug::debug_state));
    //MT.Set_Motor_Angles(motorIDs, 6, angles, speeds, direction);

    //for(int i = 0; i < 6; i++)
       // printf("%d ",temp[i]);
    //MT.Read_Motor_ID();
    //Serial.printf("%d", ID);
    //Serial.println("debug_state: " + String(debug::debug_state));
    //temp = MT.Get_Motor_Angles(motorIDs,6);

    // MT.Set_Motor_Angles(motorIDs,6,angles,10,0);
    // delay(4000);
    // printf("first\n\r");
    // temp = MT.Get_Motor_Angles(motorIDs,6);
    // delay(2000);

    // MT.Set_Motor_Angles(motorIDs,6,angles2,10,0);
    // delay(4000);
    // printf("second\n\r");
    // temp = MT.Get_Motor_Angles(motorIDs,6);
    // delay(2000);

    // MT.Set_Motor_Angles(motorIDs,6,angles3,10);
    // delay(4000);
    // printf("third\n\r");
    temp = MT.Get_Motor_Angles(motorIDs,6);
    //delay(2000);

    // MT.Set_Motor_Angles(motorIDs,6,angles4,10,0);
    // delay(4000);
    // printf("forth\n\r");
    // temp = MT.Get_Motor_Angles(motorIDs,6);
    // delay(2000);

    // MT.Set_Motor_Angles(motorIDs,6,angles5,10,0);
    // delay(4000);
    // printf("fifth\n\r");
    // temp = MT.Get_Motor_Angles(motorIDs,6);
    // delay(2000);

    // MT.Set_Motor_Angles(motorIDs,6,angles,10,0);

    //MT.Read_Calibration(motorIDs,6);
    //Error error =  MT.Get_Motor_Errors(motorIDs,6);
    //MT.Get_Motor_PID(motorIDs,6,1);
    //delay(5000);
}
