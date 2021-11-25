/*
 * Epwm_Adc_Set.c
 */

#include "F2837xD_device.h"
#include "F2837xD_Examples.h"
#include "Epwm_Adc_Set.h"

// Defines
//
#define EPWM_TIMER_TBPRD   2000  // Period register
#define EPWM_MIN_CMPA      1000
#define EPWM_MIN_CMPB      1000

void ConfigureEPWM(void)
{
    EALLOW;
    // Assumes ePWM clock is already enabled
    EPwm1Regs.ETSEL.bit.SOCAEN  = 0;            // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = 4;            // Select SOC on up-count
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;             // Generate pulse on 1st event
    EPwm1Regs.CMPA.bit.CMPA = 200;              // Set compare A value to 200 counts
    EPwm1Regs.TBPRD = 400;                      // Set period to 400 counts
    EPwm1Regs.TBCTL.bit.CTRMODE = 3;            // freeze counter
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;   // Clock ratio to SYSCLKOUT /1
    EPwm1Regs.TBCTL.bit.CLKDIV = 0; // High Speed Time Base Clock Pre-Scale Bits /1

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;  //enable SOCA
    EPwm1Regs.TBCTL.bit.CTRMODE = 0; //unfreeze, and enter up count mode

    EDIS;
}

void InitEPwm2Example()
{
   //
   // Setup TBCLK
   //
   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm2Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0x0;   // Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = 0x0;

   //
   // Setup shadow register load on ZERO
   //
   EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm2Regs.CMPA.bit.CMPA = EPWM_MIN_CMPA;     // Set compare A value
   EPwm2Regs.CMPB.bit.CMPB = EPWM_MIN_CMPB;     // Set Compare B value

   //
   // Set actions
   //
   EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
                                                 // up count

   EPwm2Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
                                                 // up count
}

void InitEPwm3Example()
{
   //
   // Setup TBCLK
   //
   EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm3Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period
   EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0x0;   // Clock ratio to SYSCLKOUT
   EPwm3Regs.TBCTL.bit.CLKDIV = 0x0;

   //
   // Setup shadow register load on ZERO
   //
   EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm3Regs.CMPA.bit.CMPA = EPWM_MIN_CMPA;     // Set compare A value
   EPwm3Regs.CMPB.bit.CMPB = EPWM_MIN_CMPB;     // Set Compare B value

   //
   // Set actions
   //
   EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
                                                 // up count

   EPwm3Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
                                                 // up count
}

void InitEPwm4Example()
{
   //
   // Setup TBCLK
   //
   EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm4Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period
   EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm4Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm4Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0x0;   // Clock ratio to SYSCLKOUT
   EPwm4Regs.TBCTL.bit.CLKDIV = 0x0;

   //
   // Setup shadow register load on ZERO
   //
   EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm4Regs.CMPA.bit.CMPA = EPWM_MIN_CMPA;     // Set compare A value
   EPwm4Regs.CMPB.bit.CMPB = EPWM_MIN_CMPB;     // Set Compare B value

   //
   // Set actions
   //
   EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
                                                 // up count

   EPwm4Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
                                                 // up count
}

void InitEPwm5Example()
{
   //
   // Setup TBCLK
   //
   EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm5Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period
   EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm5Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm5Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm5Regs.TBCTL.bit.HSPCLKDIV = 0x0;   // Clock ratio to SYSCLKOUT
   EPwm5Regs.TBCTL.bit.CLKDIV = 0x0;
   //
   // Setup shadow register load on ZERO
   //
   EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm5Regs.CMPA.bit.CMPA = EPWM_MIN_CMPA;     // Set compare A value
   EPwm5Regs.CMPB.bit.CMPB = EPWM_MIN_CMPB;     // Set Compare B value

   //
   // Set actions
   //
   EPwm5Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
                                                 // up count

   EPwm5Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
                                                 // up count
}

void InitEPwm6Example()
{
   //
   // Setup TBCLK
   //
   EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm6Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period
   EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm6Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0x0;   // Clock ratio to SYSCLKOUT
   EPwm6Regs.TBCTL.bit.CLKDIV = 0x0;

   //
   // Setup shadow register load on ZERO
   //
   EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm6Regs.CMPA.bit.CMPA = EPWM_MIN_CMPA;     // Set compare A value
   EPwm6Regs.CMPB.bit.CMPB = EPWM_MIN_CMPB;     // Set Compare B value

   //
   // Set actions
   //
   EPwm6Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
                                                 // up count

   EPwm6Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
                                                 // up count
}

void InitEPwm7Example()
{
   //
   // Setup TBCLK
   //
   EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm7Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period
   EPwm7Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm7Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
   EPwm7Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm7Regs.TBCTL.bit.HSPCLKDIV = 0x0;   // Clock ratio to SYSCLKOUT
   EPwm7Regs.TBCTL.bit.CLKDIV = 0x0;

   //
   // Setup shadow register load on ZERO
   //
   EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   //
   // Set Compare values
   //
   EPwm7Regs.CMPA.bit.CMPA = EPWM_MIN_CMPA;     // Set compare A value
   EPwm7Regs.CMPB.bit.CMPB = EPWM_MIN_CMPB;     // Set Compare B value

   //
   // Set actions
   //
   EPwm7Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero
   EPwm7Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on event A,
                                                 // up count

   EPwm7Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm7Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B,
                                                 // up count
}

void ConfigureADCA(void)
{
    EALLOW;

    //write configurations
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADC
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    EDIS;
}

void ConfigureADCB(void)
{
    EALLOW;

    //write configurations
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //Set pulse positions to late
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADC
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    EDIS;
}

void ConfigureADCC(void)
{
    EALLOW;

    //write configurations
    AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //Set pulse positions to late
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADC
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    EDIS;
}

void ConfigureADCD(void)
{
    EALLOW;

    //write configurations
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //Set pulse positions to late
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADC
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    EDIS;
}

void SetupADCEpwm(void)
{
    Uint16 acqps;

    //determine minimum acquisition window (in SYSCLKS) based on resolution
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION){
        acqps = 14; //75ns
    }
    else { //resolution is 16-bit
        acqps = 63; //320ns
    }

    //Select the channels to convert and end of conversion flag
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;  //SOC0 will convert pin A1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;  //SOC0 will convert pin A2
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 0;  //SOC0 will convert pin B0
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcbRegs.ADCSOC4CTL.bit.CHSEL = 1;  //SOC0 will convert pin B1
    AdcbRegs.ADCSOC4CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcbRegs.ADCSOC5CTL.bit.CHSEL = 2;  //SOC0 will convert pin B2
    AdcbRegs.ADCSOC5CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC5CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC6CTL.bit.CHSEL = 2;  //SOC0 will convert pin C2
    AdccRegs.ADCSOC6CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdccRegs.ADCSOC6CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC7CTL.bit.CHSEL = 3;  //SOC0 will convert pin C3
    AdccRegs.ADCSOC7CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdccRegs.ADCSOC7CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC8CTL.bit.CHSEL = 4;  //SOC0 will convert pin C4
    AdccRegs.ADCSOC8CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdccRegs.ADCSOC8CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcdRegs.ADCSOC9CTL.bit.CHSEL = 0;  //SOC0 will convert pin D0
    AdcdRegs.ADCSOC9CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC9CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcdRegs.ADCSOC10CTL.bit.CHSEL = 1;  //SOC0 will convert pin D1
    AdcdRegs.ADCSOC10CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC10CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C

    AdcdRegs.ADCSOC11CTL.bit.CHSEL = 2;  //SOC0 will convert pin D2
    AdcdRegs.ADCSOC11CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC11CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C
}
