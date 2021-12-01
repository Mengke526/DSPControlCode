/*
 * Timer_Set.h
 */

#ifndef INCLUDE_USER_TIMER_SET_H_
#define INCLUDE_USER_TIMER_SET_H_

extern float t;

extern void TimerGpioSet(void);
extern void TimerInit(void);
extern __interrupt void cpu_timer0_isr(void);
extern __interrupt void cpu_timer1_isr(void);
extern __interrupt void cpu_timer2_isr(void);

#endif /* INCLUDE_USER_TIMER_SET_H_ */
