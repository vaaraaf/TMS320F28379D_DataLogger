//###############################################################################################################
//
// FILE:            Data Logger for DSP TMS320F28379D (C2000 series from Texas Instruments)
// Written by:      Vahid Rafiei
// Date:            05/19/2023
//Description:      This is the code for logging data from 8 sensors. In this code you see sample
//                  data gathering process.
//                  Based on the number of data you want to log, you may need to change memory allocations
//                  in the file named: 2837xD_RAM_Ink_cpu1.cmd
//                  In this code 9600 sample data are gathered from each sensor.
//#################################################################################################################

//
// Included Files
//
#include "F28x_Project.h"
#include <stdio.h>
#include <stdlib.h>


//
// Defines
//
#define RESULTS_BUFFER_SIZE     9600

//
// Globals
//
uint16_t adcBResults6[RESULTS_BUFFER_SIZE];   // Buffer for results
uint16_t adcBResults7[RESULTS_BUFFER_SIZE];
uint16_t adcBResults8[RESULTS_BUFFER_SIZE];   // Buffer for results
uint16_t adcBResults9[RESULTS_BUFFER_SIZE];
uint16_t adcAResults2[RESULTS_BUFFER_SIZE];   // Buffer for results
uint16_t adcAResults3[RESULTS_BUFFER_SIZE];
uint16_t adcAResults4[RESULTS_BUFFER_SIZE];   // Buffer for results
uint16_t adcAResults5[RESULTS_BUFFER_SIZE];
float abA=4;
float abB=4;

uint16_t index          = 0;                  // Index into result buffer

int S=1;
float avg=0;
uint32_t sum,i;
volatile uint16_t bufferFull;                // Flag to indicate buffer is full
int collect_start       = 0;

//
// Function Prototypes
//
void GPIOConfiguration(void);
void initADC(void);
void ConfigureInterrupt(void);
void initADCSOC(void);
void Regionselect();
void Jselect();
void Hallcollect(int J);
void InitDatArray(void);
__interrupt void cpu_timer0_isr(void);
//__interrupt void cpu_timer1_isr(void);    //In case if you need it
//__interrupt void cpu_timer2_isr(void);    //In case if you need it


void main(void)
{
    InitSysCtrl();
    InitGpio();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    GPIOConfiguration();
    InitCpuTimers();
    ConfigureInterrupt();

    initADC();
    initADCSOC();
    InitDatArray();

    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

    //
    // Take conversions indefinitely in loop
    //
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;
    while(1)
    {

        GpioDataRegs.GPASET.bit.GPIO14 = 1;    // Based on the structure of your board, you may need to activate some Multiplexers.
        GpioDataRegs.GPASET.bit.GPIO15 = 1;    // Based on the structure of your board, you may need to activate some Multiplexers.

    }
}


void GPIOConfiguration(void)
{

    EALLOW;

    // Enable an GPIO output on GPIO10, set it high
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;   // Control selection of multiplexer on GPIO10
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;   // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;  // GPIO10 = GPIO10
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;   // GPIO10 = output

    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;   // Control selection of multiplexer on GPIO11
    GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;   // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;  // GPIO8 = GPIO11
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;   // GPIO8 = output

    //  SET ENABLES HIGH FOR Hall SENSORS
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;   // Enable pullup on GPIO14
    GpioDataRegs.GPASET.bit.GPIO14 = 1;   // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;  // GPIO9 = GPIO14
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;   // GPIO9 = output

    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;  // Enable pullup on GPIO15
    GpioDataRegs.GPASET.bit.GPIO15 = 1;  // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0; // GPIO10 = GPIO15
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;   // GPIO10 = output

    EDIS;
}

void initADC(void)
{
    EALLOW;

    AdcaRegs.ADCCTL2.bit.PRESCALE = 2;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 2;

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    AdcSetMode(ADC_ADCA, 0, 0); // single-ended mode
    AdcSetMode(ADC_ADCB, 0, 0); // single-ended mode
//    AdcSetMode(ADC_ADCC, 0, 0); // Based on the structure of your board, you may need ADC_C
//    AdcSetMode(ADC_ADCD, 0, 0); // Based on the structure of your board, you may need ADC_D
    EDIS;

    DELAY_US(100);
}


void initADCSOC(void)
{
    //
    // Select the channels to convert and the end of conversion flag
    //
    EALLOW;
    //select channel A1
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;      // SOC0 will convert pin A0 converts RTD1
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 50;     // Sample window is 50 SYSCLK cycles

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;      // A1 converts LHS1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 50;     // Sample window is 50 SYSCLK cycles


    // select channel B1
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 2;      // B2 converts RTD2
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = 50;     // Sample window is 50 SYSCLK cycles

    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 3;      //B3 converts LHS2
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = 50;     // Sample window is 50 SYSCLK cycles

    EDIS;
}

void ConfigureInterrupt(void){

    EALLOW;
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
//    PieVectTable.TIMER1_INT = &cpu_timer1_isr;    //Un-comment if you need interrupt1
//    PieVectTable.TIMER2_INT = &cpu_timer2_isr;    //Un-comment if you need interrupt2
    EDIS;

    ConfigCpuTimer(&CpuTimer0, 200, 300);
//    ConfigCpuTimer(&CpuTimer1, 200, 1000);        //Un-comment if you need interrupt1
//    ConfigCpuTimer(&CpuTimer2, 200, 1000);        //Un-comment if you need interrupt2

    CpuTimer0Regs.TCR.all = 0x4000;
//    CpuTimer1Regs.TCR.all = 0x4000;               //Un-comment if you need interrupt1
//    CpuTimer2Regs.TCR.all = 0x4000;               //Un-comment if you need interrupt2

    IER |= M_INT1;  // Enable group 1 interrupts
    IER |= M_INT13;
    IER |= M_INT14;

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;      //Enable TIMER0

    EINT;           // Enable Global interrupt INTM
    ERTM;           // Enable Global real-time interrupt DBGM

}

//
// adcA1ISR - ADC A Interrupt 1 ISR
//
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    AdcaRegs.ADCSOCFRC1.bit.SOC0=1;     // Getting sample from the input signal
    AdcaRegs.ADCSOCFRC1.bit.SOC1=1;     // Getting sample from the input signal
    AdcbRegs.ADCSOCFRC1.bit.SOC0=1;     // Getting sample from the input signal
    AdcbRegs.ADCSOCFRC1.bit.SOC1=1;     // Getting sample from the input signal


    abA= AdcaResultRegs.ADCRESULT1*(3.3)/(4096);    //Converting to Decimal floating point
    abB= AdcbResultRegs.ADCRESULT1*(3.3)/(4096);    //Converting to Decimal floating point



    if (collect_start == 1){
            if (bufferFull==0 ){
                for (S=1; S<5; S++){
                    // Select the GPIO10, GPIO11, and GPIO8 to read the ADC related to S {1, 2, 3, 4}
                Regionselect(S);
                AdcaRegs.ADCSOCFRC1.bit.SOC0=1;
                AdcaRegs.ADCSOCFRC1.bit.SOC1=1;
                AdcbRegs.ADCSOCFRC1.bit.SOC0=1;
                AdcbRegs.ADCSOCFRC1.bit.SOC1=1;
                 Hallcollect(S);
            }



    index++;

//
// Set the bufferFull flag if the buffer is full
//
    if(RESULTS_BUFFER_SIZE < index)         // Setting Buffer Full if all the 9600 sample are taken
    {
        index = 0;
        bufferFull = 1;
    }

        }
    }


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


void Regionselect(int S)
{
    switch(S)
    {
    case 1:
    {
        GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
        DELAY_US(2);
        GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
        DELAY_US(2);
    }
    break;
    case 2:
    {
        GpioDataRegs.GPASET.bit.GPIO10 = 1;
        DELAY_US(2);
        GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
        DELAY_US(2);
    }
    break;
    case 3:
    {
        GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
        DELAY_US(2);
        GpioDataRegs.GPASET.bit.GPIO11 = 1;
        DELAY_US(2);
    }
    break;
    case 4:
    {
        GpioDataRegs.GPASET.bit.GPIO10 = 1;
        DELAY_US(2);
        GpioDataRegs.GPASET.bit.GPIO11 = 1;
        DELAY_US(2);
    }
    break;
    }
}


void Hallcollect(int J)
{
    switch(J)
       {
       case 1:
       {
           adcBResults9[index] = (AdcbResultRegs.ADCRESULT1);
           adcAResults5[index] = (AdcaResultRegs.ADCRESULT1);
           DELAY_US(2);
       }
       break;
       case 2:
       {
           adcBResults8[index] = (AdcbResultRegs.ADCRESULT1);
           adcAResults4[index] = (AdcaResultRegs.ADCRESULT1);
           DELAY_US(2);
       }
       break;
       case 3:
       {
           adcBResults7[index] = (AdcbResultRegs.ADCRESULT1);
           adcAResults3[index] = (AdcaResultRegs.ADCRESULT1);
           DELAY_US(2);
       }
       break;
       case 4:
       {
           adcBResults6[index] = (AdcbResultRegs.ADCRESULT1);
           adcAResults2[index] = (AdcaResultRegs.ADCRESULT1);
           DELAY_US(2);
       }
       break;
       }
}

//__interrupt void cpu_timer1_isr(void)
//{
//   CpuTimer1.InterruptCount++;
//}
//
//__interrupt void cpu_timer2_isr(void)
//{
//   CpuTimer2.InterruptCount++;
//}

void InitDatArray(void){

    for(index = 0; index < RESULTS_BUFFER_SIZE; index++)
    {
        adcAResults5[index] = 0;
        adcAResults4[index] = 0;
        adcAResults3[index] = 0;
        adcAResults2[index] = 0;
        adcBResults9[index] = 0;
        adcBResults8[index] = 0;
        adcBResults7[index] = 0;
        adcBResults6[index] = 0;
    }

    index = 0;
    bufferFull = 0;

}

void Delay(unsigned int n)
{
    unsigned i;
    for(i=0;i<n;i++){
    }
}

//
// End of File
//

