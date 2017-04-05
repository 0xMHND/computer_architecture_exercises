#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>

union mem
{
   double df;
   long l;
   char str[8];
};

double myfrexp(double x, int *exp)
{
   double temp = x;
   *exp=0;
   while(fabs(temp)>=1)
   {
      temp = temp / 2;
      *exp += 1;
   }
   return temp;
}
int main(int argc,char **argv){

   union mem data;
   printf( "Memory size occupied by data : %d\n", sizeof(data));
   
   data.df= 0x40000000; 
   //printf("Mantissa = %d\n", (data.df&0xfffffffffffff) );
   //printf("exp = %d\n", ((data.df>>52)&0x7ff)-127 );
   //printf("sign = %d\n", data.df>>63);
   printf("float = %f\n", data.df);
   printf("long value= %ld\n", data.l);
   printf("long sign = %c\n",(data.l>>63)?'-':'+' );
   printf("8 char = %s\n", data.str);
 
/*
   double x, r;
   int exp;

   x = strtod(argv[1], NULL);
   r = myfrexp(x, &exp);

   printf("frexp(%g, &e) = %g: %g * %d^%d = %g\n",
	 x, r, r, 2, exp, x);
   exit(EXIT_SUCCESS);
*/   return 0;
}
/*
int op_speed(){

   int i=0;
   unsigned int eax;
   unsigned int ebx;
   unsigned int ecx;
   unsigned int edx;
  
   __asm__ __volatile__( 
	 		"cpuid;"
		       	:
	       		: 
			);


   __asm__ __volatile__(
			 "rdtsc;"
			 :"=a" eax, "=b" edx
			 : 
			 );


   for(i, i<ITER; ++i)
   {
      //8 op

   }
   for(i=0, i<ITER; ++i)
   {
      //4 op

   }


}
*/
