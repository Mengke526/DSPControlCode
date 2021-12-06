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

void ScibGpioSet(void)
{
    EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 2;
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 2;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO55 = 1;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO54 = 1;
    EDIS;
}

// SCIA  DLB, 8-bit word, baud rate 460800, default, 1 STOP bit, no parity
void SciaEchobackInit(void)
{
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.SCIA_RX_INT = &sciaRxFifoIsr;
    EDIS;   // This is needed to disable write to EALLOW protected registers

    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

    SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

    //
    // SCIA at 460800 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x00 and LBAUD = 0x0D.
    //
    //SciaRegs.SCIHBAUD.all    = 0x0002;
    //SciaRegs.SCILBAUD.all    = 0x008B;
    SciaRegs.SCIHBAUD.all = 0x0000;
    SciaRegs.SCILBAUD.all = 0x000D;

    //SciaRegs.SCIFFTX.all = 0xE040;
    //SciaRegs.SCIFFRX.all = 0x2044;
    SciaRegs.SCIFFTX.all=0xC022;
    SciaRegs.SCIFFRX.all=0x0022;
    SciaRegs.SCIFFCT.all = 0x0;

    SciaRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset
    SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, INT1
    IER |= M_INT9; // Enable CPU INT

    flag = 3;
    count = 3;
}

// SCIB  DLB, 8-bit word, baud rate 9600, default, 1 STOP bit, no parity
void ScibEchobackInit(void)
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

    ScibRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    ScibRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.all = 0x0003;
    //ScibRegs.SCICTL2.bit.TXINTENA = 1;
    //ScibRegs.SCICTL2.bit.RXBKINTENA = 1;

    //
    // SCIB at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    //
    //ScibRegs.SCIHBAUD.all    = 0x0002;
    //ScibRegs.SCILBAUD.all    = 0x008B;
    ScibRegs.SCIHBAUD.all    = 0x0000;
    ScibRegs.SCILBAUD.all    = 0x000D;

    ScibRegs.SCIFFTX.all = 0xE040;
    ScibRegs.SCIFFRX.all = 0x2044;
    SciaRegs.SCIFFCT.all = 0x0;

    ScibRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset
    //ScibRegs.SCIFFRX.bit.RXFIFORESET=1;

    //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
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

void ScibTx8(int a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0) {}//0h (R/W) = Transmit FIFO is empty
    ScibRegs.SCITXBUF.all =a;
}

void ScibTx16(int a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0) {}//0h (R/W) = Transmit FIFO is empty
    ScibRegs.SCITXBUF.all = a & 0x00FF;
    ScibRegs.SCITXBUF.all = a >> 8;
}

interrupt void sciaRxFifoIsr(void)
{
    rx = SciaRegs.SCIRXBUF.all;  //读取接收缓存中的一个字节数据
    if(flag != 0){
        if(flag == 3){
            if(rx == 0x00BD){
                flag--;
                Ref_Buff[count] = rx;
                count++;
            }
        }
        else if(flag == 2){
            if(rx == 0x00DB){
                flag--;
                Ref_Buff[count] = rx;
                count++;
            }
        }
        else if(flag == 1){
            if(rx == 0x000A){
                flag--;
                Ref_Buff[count] = rx;
                count++;
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
            flag = 3;
        }
    }
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}
