/*
 * Timer_Set.c
 */

#include "Timer_Set.h"
#include "F28x_Project.h"
#include "pid_control.h"
#include "MY530HE.h"

float t;

__interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;

   if(t >= 500.0)
       t = 0.0;
   t += 0.0001;

   CurAdc();
   if(pidflag){
       CurPid();
   }

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

   GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
   GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;
   GpioDataRegs.GPCTOGGLE.bit.GPIO67 = 1;
   GpioDataRegs.GPCTOGGLE.bit.GPIO84 = 1;
}

__interrupt void cpu_timer1_isr(void)
{
   CpuTimer1.InterruptCount++;

   my530he_read();
   my530he_offset();
   if(offset.status != 0)
       quat_update();

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

   GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
   GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;
   GpioDataRegs.GPCTOGGLE.bit.GPIO67 = 1;
   GpioDataRegs.GPCTOGGLE.bit.GPIO84 = 1;
}

/*__interrupt void cpu_timer2_isr(void)
{
   CpuTimer2.InterruptCount++;

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

   GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
   GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;
   GpioDataRegs.GPCTOGGLE.bit.GPIO67 = 1;
   GpioDataRegs.GPCTOGGLE.bit.GPIO84 = 1;
}*/

void TimerGpioSet(void){
    GPIO_SetupPinMux(0, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(0, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(1, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(1, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(67, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(67, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(84, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(84, GPIO_OUTPUT, GPIO_PUSHPULL);
}

void TimerInit(void){
    t = 0.0;

    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.TIMER1_INT = &cpu_timer1_isr;
    //PieVectTable.TIMER2_INT = &cpu_timer2_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers

    // Initialize the Device Peripheral
    InitCpuTimers();   // For this example, only initialize the Cpu Timers

    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 200MHz CPU Freq, 0.5 second Period (in uSeconds)
    ConfigCpuTimer(&CpuTimer0, 200, 100);
    ConfigCpuTimer(&CpuTimer1, 200, 10000);
    //ConfigCpuTimer(&CpuTimer2, 200, 20);

    // To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
    // of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in F2837xS_cputimervars.h), the
    // below settings must also be updated.
    CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    //CpuTimer2Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0

    //  User specific code, enable interrupts:
    // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
    // which is connected to CPU-Timer 1, and CPU int 14, which is connected
    // to CPU-Timer 2:
    IER |= M_INT1;
    IER |= M_INT13;
    //IER |= M_INT14;

    // Enable TINT0 in the PIE: Group 1 interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}
