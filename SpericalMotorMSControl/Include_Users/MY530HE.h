/*
 * MY530HE.h
 */

#ifndef INCLUDE_USER_MY530HE_H_
#define INCLUDE_USER_MY530HE_H_

typedef struct MY530HE
{
    unsigned int status;
    unsigned long GryX;
    unsigned long GryY;
    unsigned long GryZ;
    unsigned long AccX;
    unsigned long AccY;
    unsigned long AccZ;
    float gryx;
    float gryy;
    float gryz;
    float accx;
    float accy;
    float accz;
}MY530HE_DATA;


extern MY530HE_DATA imu;
extern MY530HE_DATA offset;
extern float angle[3];

extern void my530he_read(void);
extern void my530he_offset(void);
extern float bytes2float(unsigned long data);
extern void quat_update(void);
extern float invSqrt(float x);

#endif /* INCLUDE_USER_MY530HE_H_ */
