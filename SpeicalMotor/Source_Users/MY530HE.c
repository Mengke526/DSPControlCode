/*
 * MY530HE.c
 */
#include "MY530HE.h"
#include "Sci_Set.h"
#include "F28x_Project.h"
#include "math.h"

#define Kp 10.0
#define Ki 0.005
#define halfT 0.005

MY530HE_DATA imu;
MY530HE_DATA offset;

float q0 = 1.0;
float q1 = 0.0;
float q2 = 0.0;
float q3 = 0.0;
float exInt = 0.0;
float eyInt = 0.0;
float ezInt = 0.0;
float angle[3];

void my530he_read(void)
{
    imu.GryX = Ref_Buff[6]<<24 | Ref_Buff[5]<<16 | Ref_Buff[4]<<8 | Ref_Buff[3];
    imu.GryY = Ref_Buff[10]<<24 | Ref_Buff[9]<<16 | Ref_Buff[8]<<8 | Ref_Buff[7];
    imu.GryZ = Ref_Buff[14]<<24 | Ref_Buff[13]<<16 | Ref_Buff[12]<<8 | Ref_Buff[11];
    imu.AccX = Ref_Buff[18]<<24 | Ref_Buff[17]<<16 | Ref_Buff[16]<<8 | Ref_Buff[15];
    imu.AccY = Ref_Buff[22]<<24 | Ref_Buff[21]<<16 | Ref_Buff[20]<<8 | Ref_Buff[19];
    imu.AccZ = Ref_Buff[26]<<24 | Ref_Buff[25]<<16 | Ref_Buff[24]<<8 | Ref_Buff[23];
    imu.gryx = 0.017453 * bytes2float(imu.GryX);
    imu.gryy = 0.017453 * bytes2float(imu.GryY);
    imu.gryz = 0.017453 * bytes2float(imu.GryZ);
    imu.accx = bytes2float(imu.AccX);
    imu.accy = bytes2float(imu.AccY);
    imu.accz = bytes2float(imu.AccZ);
}

float bytes2float(Uint32 data){
    Uint32 m0,m1,m2,m3;
    m0 = (data >> 24) & 0x00FF;
    m1 = (data >> 16) & 0x00FF;
    m2 = (data >> 8) & 0x00FF;
    m3 = data & 0x00FF;
    float sig = 1;
    if (m0 >=128)
        sig = -1;
    float jie = 0.;
    if (m0 >=128)
    {
        jie = m0-128;
    }
    else
    {
        jie = m0;
    }
    jie = jie * 2;
    if (m1 >= 128)
        jie += 1;
    jie -= 127;
    float tail = 0;
    if (m1 >= 128)
        m1 -= 128;
    tail =  m3 + (m2 + m1 * 256) * 256;
    tail  = (tail)/8388608;   //   8388608 = 2^23
    float f = sig * pow(2, jie) * (1+tail);
    return f;

}

void my530he_offset(void){
    static Uint16 offindex = 0;
    if(offset.status == 0){
        offset.gryx += imu.gryx;
        offset.gryy += imu.gryy;
        offset.gryz += imu.gryz;
        offset.accx += imu.accx;
        offset.accy += imu.accy;
        offindex++;
        if(offindex == 100){
            offset.status = 1;
            offset.gryx /= 100;
            offset.gryy /= 100;
            offset.gryz /= 100;
            offset.accx /= 100;
            offset.accy /= 100;
        }
    }
    else{
        imu.gryx -= offset.gryx;
        imu.gryy -= offset.gryy;
        imu.gryz -= offset.gryz;
        //imu.accx -= offset.accx;
        //imu.accy -= offset.accy;
    }
}

void quat_update(void)
{
    float ax = imu.accx,ay = imu.accy,az = imu.accz;
    float gx = imu.gryx,gy = imu.gryy,gz = imu.gryz;
    float vx, vy, vz;
    float ex, ey, ez;
    float norm;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;

    if(ax*ay*az==0){
        return;
    }

    norm = invSqrt(ax*ax + ay*ay + az*az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;
    ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);

    exInt += ex * Ki;
    eyInt += ey * Ki;
    ezInt += ez * Ki;

    gx += Kp*ex + exInt;
    gy += Kp*ey + eyInt;
    gz += Kp*ez + ezInt;

    q0 += (-q1*gx - q2*gy - q3*gz) * halfT;
    q1 += (q0*gx + q2*gz - q3*gy) * halfT;
    q2 += (q0*gy - q1*gz + q3*gx) * halfT;
    q3 += (q0*gz + q1*gy - q2*gx) * halfT;

    norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;

    angle[0] = -atan2f(2 * q2q3 + 2 * q0q1, q0q0 - q1q1 - q2q2 + q3q3)* 57.3 ;
    angle[1] = asinf(2 * (q1q3 - q0q2))* 57.3;
    //angle[2] = atan2f(2.f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3)* 57.3;
}

float invSqrt(float x)
{
    float halfx = 0.5 * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5F3759DF - (i>>1);
    y = *(float*)&i;
    y = y * (1.5 - (halfx * y * y));
    return y;
}
