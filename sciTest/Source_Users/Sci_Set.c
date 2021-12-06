/*
 * Sci_Set.c
 */

#include "Sci_Set.h"
#include "F28x_Project.h"
#include "Timer_Set.h"

int count;
int flag;
Uint32 Ref_Buff[34];
Uint16 rx;

void SciaGpioSet(void)
{
    EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 2;
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 2;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO64 = 1;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO65 = 1;
    EDIS;
}

// SCIA  DLB, 8-bit word, baud rate 460800, default, 1 STOP bit, no parity
void SciaInit(void)
{
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.SCIA_RX_INT = &sciaRxFifoIsr;
    EDIS;   // This is needed to disable write to EALLOW protected registers

    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    SciaRegs.SCICCR.bit.SCICHAR = 7;
    SciaRegs.SCICTL1.bit.RXENA = 1;
    SciaRegs.SCICTL1.bit.TXENA = 1;
    SciaRegs.SCIHBAUD.bit.BAUD = 0x0000;
    SciaRegs.SCILBAUD.bit.BAUD = 0x000D;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
    SciaRegs.SCICTL1.bit.SWRESET = 1;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, INT1
    IER |= M_INT9; // Enable CPU INT

    flag = 3;
    count = 3;
}

void SciaTx8(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}//0h (R/W) = Transmit FIFO is empty
    SciaRegs.SCITXBUF.all =a;
}

void SciaTx16(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}//0h (R/W) = Transmit FIFO is empty
    SciaRegs.SCITXBUF.all = a & 0x00FF;
    SciaRegs.SCITXBUF.all = a >> 8;
}
interrupt void sciaRxFifoIsr(void)
{
    rx = SciaRegs.SCIRXBUF.bit.SAR;  //读取接收缓存中的一个字节数据
    if(flag != 0){
        if(flag == 3){
            if(rx == 0x00BD){
                flag--;
            }
        }
        else if(flag == 2){
            if(rx == 0x00DB){
                flag--;
            }
        }
        else if(flag == 1){
            if(rx == 0x000A){
                flag--;
            }
        }
    }
    else{
        if(count < 34){
            Ref_Buff[count] = rx;
            count++;
        }
        if(count == 34){
            count = 0;
        }

    }
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}
