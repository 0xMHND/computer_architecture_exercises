/*
 * Endian-neutral 
 * Swap the value using runtime test.
 */
#include <stdio.h>

int endianess();
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

   if(!endianess())
      val = val>>8|val<<8;
   printf("Runrime test\n");
   printf("%x\n", val);
							
   return 0;
}

int endianess()
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

   if(val != 0x1234)
      return 0;
   else 
      return 1;

}
