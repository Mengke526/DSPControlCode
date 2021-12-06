/*
 * Sci_Set.h
 */

#ifndef INCLUDE_USER_SCI_SET_H_
#define INCLUDE_USER_SCI_SET_H_

extern int count;
extern int flag;
extern unsigned long Ref_Buff[34];
extern unsigned int rx;

extern void SciaInit(void);
extern void SciaGpioSet(void);
extern void SciaTx8(int a);
extern void SciaTx16(int a);
extern interrupt void sciaRxFifoIsr(void);

#endif /* INCLUDE_USER_SCI_SET_H_ */
