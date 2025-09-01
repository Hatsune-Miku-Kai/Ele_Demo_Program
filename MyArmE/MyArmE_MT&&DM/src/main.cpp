#include "MotorMT.h"
#include "CanCommu.h"



extern float liju1;
MotorMT MT;

uint8_t motorIDs[] = {2,3,4,5,6} ;// 6个电机的ID
uint8_t direction = 0; // 方向
Angles temp;
// float angles[] = {0,0,0,0,0,0};
// float angles2[] = {-34.3,50.5,95.3,-50.8,-15.5,0};
// float angles3[] = {69.4,11.3,60.7,32.5,-9,0};
// float angles4[] = {74.9, 10.7, 131.8, 30.3, 14.3, 0};
// float angles5[] = {-28.5, 15.5, -69.5,53,-7, 0 };

uint8_t Calibration[] = {1,2,3,4,5,6};

float angles[] = {50,0,0,0,0,0};
float angles1[] = {-60,0,0,0,0,0};
void Get_Error();
void setup()
{
    //MT.Stop_Run();
    // MT.Set_Calibration(motorIDs,6);
    // delay(5000);

    // MT.Encous_Stop();
    // delay(1000);


    MT.Encous_Reset();
    delay(3000);
    MT.Encous_Set_Angle();
    delay(10000);
}

void loop()
{

    MT.Encous_Set_Angle();
    delay(2000);

    //MT.Encous_Stop();
    // MT.Encous_Set_Zero();
    // delay(1000);
    // MT.Encous();
    // delay(500);

    // MT.Encous_Reset();
    // delay(2000);


    // MT.Set_Motor_Angles(motorIDs,5,angles,20);
    // Get_Error();
    // delay(10000);
    // printf("first\n\r");
    // MT.Get_Motor_Angles(motorIDs,5);
    // delay(2000);


    // MT.Set_Motor_Angles(motorIDs,5,angles1,20);
    // Get_Error();
    // delay(10000);
    // printf("second\n\r");
    // MT.Get_Motor_Angles(motorIDs,5);
    // delay(2000);

}


void Get_Error()
{
    Error error;
    error = MT.Get_Motor_Errors(motorIDs,5);
    for(int i = 0; i < 5; i++)
    {
    if(error[i] != 0 )
    {
        while(1)
        {
            printf("Error: %x\n\r",error[i]);
            printf("%0.2f\n\r",liju1);
        }
    }

    }
    
}