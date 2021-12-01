/*
 * pid_control.h
 */

#ifndef INCLUDE_PID_CONTROL_H_
#define INCLUDE_PID_CONTROL_H_

typedef struct PID_Value
{
    float SetSpeed;//设定值
    float ActualSpeed;//实际值
    float OutSpeed;
    float err;
    float err_next; //上一个
    float err_last;//上上一个
    float Kp,Ki,Kd;
    float increamentSpeed;
}PID_ValueStr;

extern  PID_ValueStr pid0;
extern  PID_ValueStr pid1;
extern  PID_ValueStr pid2;
extern  PID_ValueStr pid3;
extern  PID_ValueStr pid4;
extern  PID_ValueStr pid5;
extern  PID_ValueStr pid6;
extern  PID_ValueStr pid7;
extern  PID_ValueStr pid8;
extern  PID_ValueStr pid9;
extern  PID_ValueStr pid10;
extern  PID_ValueStr pid11;

extern float dCur[12];
extern float rCur[12];
extern float rCur_adc[12];
extern Uint16 pidflag;

extern void PID_AllInit(void);
extern void PID_init(PID_ValueStr* pid);
extern float PID_realize(PID_ValueStr* pid,float SetSpeed,float ActualSpeed);
extern void CurPid(void);
extern void CurAdc(void);

#endif  //
