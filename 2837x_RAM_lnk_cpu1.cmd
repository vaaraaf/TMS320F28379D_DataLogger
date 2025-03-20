
MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */

   BEGIN           	: origin = 0x000000, length = 0x000002
   RAMM0           	: origin = 0x000122, length = 0x0002DE
   RAMD0           	: origin = 0x01BEE0, length = 0x000010
   RAMLS0          	: origin = 0x008000, length = 0x000600
   RAMLS1          	: origin = 0x008600, length = 0x000010
   RAMLS2      		: origin = 0x008610, length = 0x000300
   RAMLS3      		: origin = 0x008910, length = 0x000400
   RAMLS4      		: origin = 0x008D10, length = 0x000300
   RESET           	: origin = 0x3FFFC0, length = 0x000002

PAGE 1 :

   BOOT_RSVD       : origin = 0x000002, length = 0x000120     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
   RAMD1           : origin = 0x01BEF0, length = 0x000010


   RAMLS5      : origin = 0x009010, length = 0x012ED0

   RAMGS0      : origin = 0x01BF00, length = 0x000010
   RAMGS1      : origin = 0x01BF10, length = 0x000010
   RAMGS2      : origin = 0x01BF20, length = 0x000010
   RAMGS3      : origin = 0x01BF30, length = 0x000010
   RAMGS4      : origin = 0x01BF40, length = 0x000010
   RAMGS5      : origin = 0x01BF50, length = 0x000010
   RAMGS6      : origin = 0x01BF60, length = 0x000010
   RAMGS7      : origin = 0x01BF70, length = 0x000010
   RAMGS8      : origin = 0x01BF80, length = 0x000010
   RAMGS9      : origin = 0x01BF90, length = 0x000010
   RAMGS10     : origin = 0x01BFA0, length = 0x000010
   RAMGS11     : origin = 0x01BFB0, length = 0x000010
   RAMGS12     : origin = 0x01BFC0, length = 0x000010
   RAMGS13     : origin = 0x01BFD0, length = 0x000010
   RAMGS14     : origin = 0x01BFE0, length = 0x000010
   RAMGS15     : origin = 0x01BFF0, length = 0x000010
   
   CPU2TOCPU1RAM   : origin = 0x03F800, length = 0x000400
   CPU1TOCPU2RAM   : origin = 0x03FC00, length = 0x000400
}


SECTIONS
{
   codestart        : > BEGIN,     PAGE = 0
   
#ifdef __TI_COMPILER_VERSION__
   #if __TI_COMPILER_VERSION__ >= 15009000
    .TI.ramfunc : {} > RAMM0,      PAGE = 0
   #else
   ramfuncs         : > RAMM0      PAGE = 0   
   #endif
#endif   
   
   .text            : >>RAMM0 | RAMD0 |  RAMLS0 | RAMLS1 | RAMLS2 | RAMLS3 | RAMLS4,   PAGE = 0
   .cinit           : > RAMM0,     PAGE = 0
   .pinit           : > RAMM0,     PAGE = 0
   .switch          : > RAMM0,     PAGE = 0
   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */

   .stack           : > RAMM1,     PAGE = 1
   .ebss            : > RAMLS5,    PAGE = 1
   .econst          : > RAMLS5,    PAGE = 1
   .esysmem         : > RAMLS5,    PAGE = 1
   Filter_RegsFile  : > RAMGS0,	   PAGE = 1

   ramgs0           : > RAMGS0,    PAGE = 1
   ramgs1           : > RAMGS1,    PAGE = 1
   
   /* The following section definitions are required when using the IPC API Drivers */ 
    GROUP : > CPU1TOCPU2RAM, PAGE = 1 
    {
        PUTBUFFER 
        PUTWRITEIDX 
        GETREADIDX 
    }
    
    GROUP : > CPU2TOCPU1RAM, PAGE = 1
    {
        GETBUFFER :    TYPE = DSECT
        GETWRITEIDX :  TYPE = DSECT
        PUTREADIDX :   TYPE = DSECT
    }  
	
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
