/*
 * Sci_Set.h
 */

#ifndef INCLUDE_USER_SCI_SET_H_
#define INCLUDE_USER_SCI_SET_H_

extern int count;
extern int flag;
extern int ctrlcount;
extern int ctrlflag;
extern unsigned long Ref_Buff[34];
extern unsigned long Control_Buff[13];
extern unsigned int rx;

extern void SciaEchobackInit(void);
extern void SciaGpioSet(void);
extern void SciaTx8(int a);
extern void SciaTx16(int a);
extern interrupt void sciaRxFifoIsr(void);
extern interrupt void scibRxFifoIsr(void);

extern void ScibEchobackInit(void);
extern void ScibGpioSet(void);
extern void ScibTx8(int a);
extern void ScibTx16(int a);

#endif /* INCLUDE_USER_SCI_SET_H_ */
