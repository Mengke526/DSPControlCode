#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "Sci_Set.h"
#include "Epwm_Adc_Set.h"
#include "pid_control.h"
#include "math.h"
#include "SphericalMotor.h"
#include "Timer_Set.h"
#include "MY530HE.h"
#include "stdlib.h"

// Globals
//
float T[3];
float stepx;
float stepy;
float stepz;
float Ax;
float Ay;
float Az;
float wx;
float wy;
float wz;
Uint16 gravityflag;
Uint16 stepflag;
Uint16 sinflag;

void main(void)
{
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

    InitSysCtrl();

    InitGpio();

    TimerGpioSet();

    SciaGpioSet();
    ScibGpioSet();

    CpuSysRegs.PCLKCR2.bit.EPWM2=1;
    CpuSysRegs.PCLKCR2.bit.EPWM3=1;
    CpuSysRegs.PCLKCR2.bit.EPWM4=1;
    CpuSysRegs.PCLKCR2.bit.EPWM5=1;
    CpuSysRegs.PCLKCR2.bit.EPWM6=1;
    CpuSysRegs.PCLKCR2.bit.EPWM7=1;

    InitEPwm2Gpio();
    InitEPwm3Gpio();
    InitEPwm4Gpio();
    InitEPwm5Gpio();
    InitEPwm6Gpio();
    InitEPwm7Gpio();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    TimerInit();

    SciaEchobackInit();  // Initialize SCIA for echoback
    ScibEchobackInit();  // Initialize SCIB for echoback

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    ConfigureEPWM();
    InitEPwm2Example();
    InitEPwm3Example();
    InitEPwm4Example();
    InitEPwm5Example();
    InitEPwm6Example();
    InitEPwm7Example();

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    PID_AllInit();

    ConfigureADCA();
    ConfigureADCB();
    ConfigureADCC();
    ConfigureADCD();

    SetupADCEpwm();

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    float m = 0.418;
    float h = 0.014317;

    stepx = 0;
    stepy = 0;
    stepz = 0;
    Ax = 0;
    Ay = 0;
    Az = 0;
    wx = 0;
    wy = 0;
    wz = 0;

    pidflag = 0;
    gravityflag = 0;
    stepflag = 0;
    sinflag = 0;

    T[0] = 0;
    T[1] = 0;
    T[2] = 0;

    while(1)
    {
        stepx = 0;
        stepy = 0;
        Ax = 0;
        Ay = 0;
        wx = 0;
        wy = 0;
        pidflag = 1;
        gravityflag = 1;
        stepflag = 0;
        sinflag = 0;

        if(stepflag)
        {
            T[0] = stepx;
            T[1] = stepy;
        }
        else if(sinflag)
        {
            T[0] = Ax * sin(wx * t);
            T[1] = Ay * sin(wy * t);
        }
        else
        {
            T[0] = 0;
            T[1] = 0;
        }

        if(gravityflag){
            T[0] += 9.8 * m * cosf(angle[1] * 0.01745) * sinf(angle[0] * 0.01745) * h;
            T[1] += 9.8 * m * sinf(angle[1] * 0.01745) * cosf(angle[0] * 0.01745) * h;
        }

        CurCauculate(angle,T,dCur);
        CurBias(dCur);
    }
}
