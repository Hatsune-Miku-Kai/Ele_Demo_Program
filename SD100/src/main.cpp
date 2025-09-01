#include "SD100.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>



int main()
{
    SD100 SD;
    sleep(1);

    //SD.Set_Up();

    SD.Set_Pos(1,150,100,200,200);
    sleep(5);
    SD.Stop(1);
    while(1)
    {
        int32_t pos = SD.Read_Pos(1);
        std::cout << std::dec;
        std::cout << "pos: "<< pos << std::endl;
        sleep(2);
    }
}

