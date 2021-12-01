/*
 * SphericalMotor.h
 */

#ifndef INCLUDE_USER_SPHERICALMOTOR_H_
#define INCLUDE_USER_SPHERICALMOTOR_H_

extern void CurCauculate(float* angle,float* Td,float* u);
extern float dfLambdaFit(float sigma);
extern void CurBias(float* u);

#endif /* INCLUDE_USER_SPHERICALMOTOR_H_ */
