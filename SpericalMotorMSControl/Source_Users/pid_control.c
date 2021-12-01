/*
 * pid_control.c
 */

#include "F2837xD_device.h"
#include "F2837xD_Examples.h"
#include "pid_control.h"

PID_ValueStr pid0;
PID_ValueStr pid1;
PID_ValueStr pid2;
PID_ValueStr pid3;
PID_ValueStr pid4;
PID_ValueStr pid5;
PID_ValueStr pid6;
PID_ValueStr pid7;
PID_ValueStr pid8;
PID_ValueStr pid9;
PID_ValueStr pid10;
PID_ValueStr pid11;

float dCur[12];
float rCur[12];
float rCur_adc[12];
Uint16 pidflag;

void PID_AllInit(void)
{
    PID_init(&pid0);
    PID_init(&pid1);
    PID_init(&pid2);
    PID_init(&pid3);
    PID_init(&pid4);
    PID_init(&pid5);
    PID_init(&pid6);
    PID_init(&pid7);
    PID_init(&pid8);
    PID_init(&pid9);
    PID_init(&pid10);
    PID_init(&pid11);
}


void PID_init(PID_ValueStr* pid)
{
    pid->SetSpeed = 0.0;
    pid->ActualSpeed = 0.0;
    pid->OutSpeed = 1000.0;
    pid->err = 0.0;
    pid->err_next = 0.0;
    pid->err_last = 0.0;
    pid->Kp = 2;
    pid->Ki = 2;
    pid->Kd = 0.0;
    pid->increamentSpeed = 0;
}

float PID_realize(PID_ValueStr* pid,float SetSpeed,float ActualSpeed)
{
    pid->SetSpeed = SetSpeed;

    pid->ActualSpeed = ActualSpeed;

    pid->err = pid->SetSpeed-pid->ActualSpeed;

    pid->increamentSpeed = pid->Kp*(pid->err-pid->err_next)+pid->Ki*pid->err+pid->Kd*(pid->err-2*pid->err_next+pid->err_last);

    pid->OutSpeed += pid->increamentSpeed;

    pid->OutSpeed = (pid->OutSpeed > 2000) ? 2000 : pid->OutSpeed;
    pid->OutSpeed = (pid->OutSpeed < 0) ? 0 : pid->OutSpeed;

    pid->err_last = pid->err_next;
    pid->err_next = pid->err;

    return pid->OutSpeed;
}

void CurPid(void)
{
    EPwm2Regs.CMPA.bit.CMPA = PID_realize(&pid0,dCur[0],rCur[0]);
    EPwm2Regs.CMPB.bit.CMPB = PID_realize(&pid1,dCur[1],rCur[1]);

    EPwm3Regs.CMPA.bit.CMPA = PID_realize(&pid2,dCur[2],rCur[2]);
    EPwm3Regs.CMPB.bit.CMPB = PID_realize(&pid3,dCur[3],rCur[3]);

    EPwm4Regs.CMPA.bit.CMPA = PID_realize(&pid4,dCur[4],rCur[4]);
    EPwm4Regs.CMPB.bit.CMPB = PID_realize(&pid5,dCur[5],rCur[5]);

    EPwm5Regs.CMPA.bit.CMPA = PID_realize(&pid6,dCur[6],rCur[6]);
    EPwm5Regs.CMPB.bit.CMPB = PID_realize(&pid7,dCur[7],rCur[7]);

    EPwm6Regs.CMPA.bit.CMPA = PID_realize(&pid8,dCur[8],rCur[8]);
    EPwm6Regs.CMPB.bit.CMPB = PID_realize(&pid9,dCur[9],rCur[9]);

    EPwm7Regs.CMPA.bit.CMPA = PID_realize(&pid10,dCur[10],rCur[10]);
    EPwm7Regs.CMPB.bit.CMPB = PID_realize(&pid11,dCur[11],rCur[11]);
}

void CurAdc(void)
{
    int i;
    rCur_adc[0] = AdcaResultRegs.ADCRESULT0;
    rCur_adc[1] = AdcaResultRegs.ADCRESULT1;
    rCur_adc[2] = AdcaResultRegs.ADCRESULT2;
    rCur_adc[3] = AdcbResultRegs.ADCRESULT3;
    rCur_adc[4] = AdcbResultRegs.ADCRESULT4;
    rCur_adc[5] = AdcbResultRegs.ADCRESULT5;
    rCur_adc[6] = AdccResultRegs.ADCRESULT6;
    rCur_adc[7] = AdccResultRegs.ADCRESULT7;
    rCur_adc[8] = AdccResultRegs.ADCRESULT8;
    rCur_adc[9] = AdcdResultRegs.ADCRESULT9;
    rCur_adc[10] = AdcdResultRegs.ADCRESULT10;
    rCur_adc[11] = AdcdResultRegs.ADCRESULT11;
    for(i = 0; i < 12;i++)
    {
        rCur[i] = rCur_adc[i] * 0.00366211 - 7.5;
    }
}
