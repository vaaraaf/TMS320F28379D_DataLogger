# TMS320F28379D_DataLogger
This code is for logging data from 8 sensors.<br />
** __IMPORTANT NOTE__**<br />
  1- Due to large number of data (8 sensors and __9600__ fro each of them), the memory allocation of the DSP has been modified.
     You'll need to modify the __2837xD_RAM_Ink_cpu1.cmd__ file as attached.<br />
  2- In this code all the data is gathers synchronously in one interrupt (__cpu_timer0_isr__). But int he bode of the code you have access to uncomment ohter interupts too.<br />
  3- In this code only __ADC_A__ and __ADC_B__ are used. But you have access to uncomment the lines for __ADC_C__ and __ADC_D__ based on your needs.<br />
  4- Based on the structure of your board, you may need to activate other GPIOs.<br />


  __How to save the data in PC__:<br /><br />
  1- Once you have collected data, right click on the adcAResults2 (or others as you need) and click __View Memory__<br />
  <img width="155" alt="View_Memory" src="https://github.com/user-attachments/assets/d9dc6325-e901-4496-a7f9-9e69e9076263" /><br /><br />
  2- On the __Memory Brower__ tab you'll see the variable and all the saved data. righ click on the variable and select __Save Memory__.<br />
  <img width="109" alt="View_Memory" src="https://github.com/user-attachments/assets/b2d55136-605f-4bed-878c-80df349f3171" /><br /><br />
  3- In opened window select the directory you want the data to be saved and click next:<br />
  <img width="284" alt="image" src="https://github.com/user-attachments/assets/e228cead-9f95-4dda-8a94-ae5af2123a66" /><br /><br />
  4- In the last step set the __Start Address__ as the start address you see in the __Memory Browser__ as shows below. Finally select the number of words you need in the __Specify the number of memory words to read__. In this case we have __9600__ data. Finally click Finish.<br />
  <img width="268" alt="register_number" src="https://github.com/user-attachments/assets/7bdb0924-38c0-42e0-8725-4b47fe989e02" />




  
