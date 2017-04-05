/*
 * Endian-neutral 
 * Swap the value using compiler directives.
 */
#include <stdio.h>

#define SWAP_UINT16(x) (((x) >> 8) | ((x) << 8))

int main(int argc, char **argv)
{
   short val;
   char *p_val;
   p_val = (char *) &val;
   /*
     The following two lines assume big-endian
     Architecture to initialize the variable Val
     to 0x1234.
   */
   p_val[0] = 0x12;
   p_val[1] = 0x34;

#ifndef BIG_ENDIAN
   val = (val>>8) | (val<<8); 
#else
#endif
   printf("%x\n", val);
							
   return 0;
}
