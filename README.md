# TMS320F28379D_DataLogger
This code is for logging data from 8 sensors.
** IMPORTANT NOTE:
  1- Due to large number of data (8 sensors and 9600 fro each of them), the memory allocation of the DSP has been modified.
     You'll need to modify the 2837xD_RAM_Ink_cpu1.cmd file as attached.
  2- In this code all the data is gathers synchronously in one interrupt (interrupt_0). But int he bode of the code you have access to uncomment ohter interupts too.
  3- In this code only ADC_A and ADC_B are used. But you have access to uncomment the lines for ADC_C and ADC_D based on your needs.
  4- Based on the structure of your board, you may need to activate other GPIOs.
  
