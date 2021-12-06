#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "Sci_Set.h"
#include "Timer_Set.h"
#include "MY530HE.h"
int main(void)
{
    InitSysCtrl();
    InitGpio();
    TimerGpioSet();
    SciaGpioSet();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    TimerInit();
    SciaInit();
    EINT;
    ERTM;
    while(1){
    }
}
