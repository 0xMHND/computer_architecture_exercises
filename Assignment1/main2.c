//
//  main.c
//  cpuid_v1.0
//
//  Created by Muhannad Alarifi on 10/4/15.
//  Copyright (c) 2015 Muhannad Alarifi. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//static inline void eax0x02(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);
static inline void cpu_model(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);//eax0x01
static inline void L1_cache(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx); // eax: 0x02
static inline void cpu_features(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx); //eax=0x01,0x06,0x07
static inline void TLB(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);
static inline void Logical_CPUs(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx); //eax = 0x0B
static inline void CPU_freq(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx); //eax = 0x16
static inline void addr_width(unsigned int *eax);
static inline void memory_hierarchy(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);
static inline void pBrand(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);

const char *four_bytes_to_binary(int x)
{
    static char b[33];
    b[0] = '\0';
    
    unsigned int z;
    for (z = 2147483648; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    
    return b;
}

int main(int argc, char **argv)
{
    
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    eax = 0x0;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(eax)
                         );
    unsigned int invalue = eax;
    printf("Max input value for cpuid: %#x\n",invalue);
    
    printf("\n**********************************************************\n");    
    cpu_model(&eax, &ebx, &ecx, &edx);
    cpu_features(&eax,&ebx,&ecx,&edx);
    TLB(&eax, &ebx, &ecx, &edx); //TLB Information
    memory_hierarchy(&eax,&ebx,&ecx,&edx);
    L1_cache(&eax,&ebx,&ecx,&edx);
    addr_width(&eax);
    //eax0x02(&eax, &ebx, &ecx, &edx); //TLB Information
    if(invalue>0x0A)
    {	
       Logical_CPUs(&eax, &ebx, &ecx, &edx);
       if(invalue>0x15)
	  CPU_freq(&eax,&ebx,&ecx,&edx);
       else
       {
	  printf("\n\t\t ** CPU's frequency **\n");
	  printf("CPU's frequency is not supportd\n");
       }
    }
    else{
       printf("\n\t\t ** Logical CPUs **\n");
       printf("Number of logical CPUs is not supportd\n");
       printf("\n\t\t ** CPU's frequency **\n");
       printf("CPU's frequency is not supportd\n");
    }
    printf("**********************************************************\n\n");    
    
    return 0;
}
#define BSIZE 500
static inline void cpu_model(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    *eax = 0x01;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );

    printf("\t\t** CPU model ** \n");
    //EAX
    if ( ((*eax>>8)&0xf) == 6 || ((*eax>>8)&0xf) == 15) {
        printf("Family ID: %#x\nModel ID: %#x\n", (*eax>>8)&0xf,( ((*eax>>12)&0xf0)+((*eax>>4)&0xf) ) );
    }
    pBrand(eax,ebx,ecx,edx);
}

static inline void cpu_features(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    *eax = 0x01;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
    printf("\n\t\t** CPU features ** \n");
    //ECX
    int pnt=0;
    //printf("%s\n",four_bytes_to_binary(*ecx));
    while(pnt!=32)
    {
	  char code_name[5];
	  sprintf(code_name,"%d_B",pnt);
	  code_name[4]=0;

	  FILE *f;
	  //f = fopen("cpu_features_eax0x01_ecx.txt","r");
	  f = fopen("encoded_cpu_features_eax0x01_ecx.txt","r");
	  char buf[BSIZE];
	  for(int i=0; i<BSIZE; i++)
	      buf[i] = '\0';
	  
	  if (f == NULL)
	  {
	      printf("File didnt OPEN\n");
	  }
	  while(!feof(f))
	  {
	      fgets(buf,BSIZE,f);
	      if(strstr(buf,code_name) != NULL && ((*ecx>>pnt)&0x01))
	      {
	      	  fgets(buf,BSIZE,f); //for encoded display, ignore when full display
		  buf[strlen(buf)-1]=0; //for encoded display, ignore for full display
		  if(!strstr(buf,"Reserved"))
		  printf(" %s ", buf);
		  fclose(f);
		  break;
	      }
	  }
	  pnt++;
    }
    //edx
    pnt = 0;
    while(pnt!=32)
    {
	  char code_name[5];
	  sprintf(code_name,"%d_B",pnt);
	  code_name[4]=0;

	  FILE *f;
	  //f = fopen("cpu_features_eax0x01_ecx.txt","r");
	  f = fopen("encoded_cpu_features_eax0x01_edx.txt","r");
	  char buf[BSIZE];
	  for(int i=0; i<BSIZE; i++)
	      buf[i] = '\0';
	  
	  if (f == NULL)
	  {
	      printf("File didnt OPEN\n");
	  }
	  while(!feof(f))
	  {
	      fgets(buf,BSIZE,f);
	      if(strstr(buf,code_name) != NULL && ((*ecx>>pnt)&0x01))
	      {
	      	  fgets(buf,BSIZE,f); //for encoded display, ignore when full display
		  buf[strlen(buf)-1]=0; //for encoded display, ignore for full display
		  if(!strstr(buf,"Reserved"))
		  	printf(" %s ", buf);
		  fclose(f);
		  break;
	      }
	  }
	  pnt++;
    }
    /* eax = 0x06
       Bit 00: Digital temperature sensor is supported if set
       Bit 01: Intel Turbo Boost Technology Available (see description of IA32_MISC_ENABLE[38]).
       Bit 02: ARAT. APIC-Timer-always-running feature is supported if set.
       Bit 03: Reserved
       Bit 04: PLN. Power limit notification controls are supported if set.
       Bit 05: ECMD. Clock modulation duty cycle extension is supported if set.
       Bit 06: PTM. Package thermal management is supported if set.
       Bit 07: HWP. HWP base registers (IA32_PM_ENABLE[bit 0], IA32_HWP_CAPABILITIES,
	     IA32_HWP_REQUEST, IA32_HWP_STATUS) are supported if set.
       Bit 08: HWP_Notification. IA32_HWP_INTERRUPT MSR is supported if set.
       Bit 09: HWP_Activity_Window. IA32_HWP_REQUEST[bits 41:32] is supported if set.
       Bit 10: HWP_Energy_Performance_Preference. IA32_HWP_REQUEST[bits 31:24] is supported if set.
       Bit 11: HWP_Package_Level_Request. IA32_HWP_REQUEST_PKG MSR is supported if set.
       Bit 12: Reserved.
       Bit 13: HDC. HDC base registers IA32_PKG_HDC_CTL, IA32_PM_CTL1, IA32_THREAD_STALL MSRs are
       supported if set.
    */
    *eax = 0x06;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
    pnt = 0;
    while(pnt!=14)
    {
       if( (*eax>>pnt) & 0x01 )
       {
	  switch(pnt)
	  {
	     case 0: 
		printf(" DigitalTemperatureSensor");
		break;
	     case 1: 
		printf(" TurboBoost");
		break;
	     case 2:
		printf(" AVAT");
		break;
	     case 3: 
		break;
	     case 4: 
		printf(" PLN");
		break;
	     case 5: 
		printf(" ECMD");
		break;
	     case 6: 
		printf(" PTM");
		break;
	     case 7: 
		printf(" HWP");
		break;
	     case 8: 
		printf(" HWP_Notification");
		break;
	     case 9: 
		printf(" HWP_Activity_Window");
		break;
	     case 10: 
		printf(" HWP_Energy_Performance");
		break;
	     case 11: 
		printf(" HWP_Package_Level_Request");
		break;
	     case 12: 
		break;
	     case 13: 
		printf(" HDC");
		break;
	     default:
		break;
	  }
	  pnt++;
       }
       else
	  pnt++;
    }
    /*eax = 0x07 ecx=0x0(0-eax)
     EBX 
     Bit 00: FSGSBASE. Supports RDFSBASE/RDGSBASE/WRFSBASE/WRGSBASE if 1.
     Bit 01: IA32_TSC_ADJUST MSR is supported if 1.
     Bit 02: Reserved
     Bit 03: BMI1
     Bit 04: HLE
     Bit 05: AVX2
     Bit 06: Reserved
     Bit 07: SMEP. Supports Supervisor-Mode Execution Prevention if 1.
     Bit 08: BMI2
     Bit 09: Supports Enhanced REP MOVSB/STOSB if 1.
     Bit 10: INVPCID. If 1, supports INVPCID instruction for system software that manages process-context
     identifiers.
     Bit 11: RTM
     Bit 12: Supports Platform Quality of Service Monitoring (PQM) capability if 1.
     Bit 13: Deprecates FPU CS and FPU DS values if 1.
     Bit 14: MPX. Supports Intel® Memory Protection Extensions if 1.
     Bit 15: Supports Platform Quality of Service Enforcement (PQE) capability if 1.
     Bits 17:16: Reserved
     Bit 18: RDSEED
     Bit 19: ADX
     Bit 20: SMAP. Supports Supervisor-Mode Access Prevention (and the CLAC/STAC instructions) if 1.
     Bits 24:21: Reserved
     Bit 25: Intel Processor Trace
     Bits 31:26: Reserved
     ECX 
     Bit 00: PREFETCHWT1
     Bits 02:01: Reserved
     Bit 03: PKU. Supports protection keys for user-mode pages if 1.
     Bit 04: OSPKE. If 1, OS has set CR4.PKE to enable protection keys (and the RDPKRU/WRPKRU instructions)
     Bits 31:05: Reserv
     */
    int cnt=0;
    int n=0;
    while(cnt!=-1){
    *ecx=0x00+cnt;
    *eax = 0x07;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax), "c"(*ecx)
                         );
    if(cnt==0)
       n=*eax;
    cnt++;
    if((cnt-1)>n)//Sub-leaf index n is invalid if n exceeds the value that sub-leaf 0 returns in EAX
    {
       cnt = -1;
    }
    pnt = 0;
    while(pnt!=14)
    {
       if( (*ebx>>pnt) & 0x01 )
       {
	  switch(pnt)
	  {
	     case 0: 
		printf(" FSGSBASE");
		break;
	     case 1: 
		printf(" IA32_TSC_ADJUST_MSR");
		break;
	     case 3: 
		printf(" BMI1");
		break;
	     case 4: 
		printf(" HLE");
		break;
	     case 5: 
		printf(" AVX2");
		break;
	     case 7: 
		printf(" SMEP");
		break;
	     case 8: 
		printf(" BMI2");
		break;
	     case 9: 
		printf(" Enhanced_REP_MOVSB/STOSB");
		break;
	     case 10: 
		printf(" INVPCID");
		break;
	     case 11: 
		printf(" RTM");
		break;
	     case 12: 
		printf(" PMQ");
		break;
	     case 13: 
		printf(" HDC");
		break;
	     case 14: 
		printf(" HDC");
		break;
	     case 15: 
		printf(" HDC");
		break;
	     case 18: 
		printf(" RDSEED");
		break;
	     case 19: 
		printf(" ADX");
		break;
	     case 20: 
		printf(" SMAP");
		break;
	     default:
		break;
	  }
	  pnt++;
       }
       else
	  pnt++;
    }
    /*
     ECX
     Bit 00: PREFETCHWT1
     Bits 02:01: Reserved
     Bit 03: PKU. Supports protection keys for user-mode pages if 1.
     Bit 04: OSPKE. If 1, OS has set CR4.PKE to enable protection keys (and the RDPKRU/WRPKRU instructions)
     */
    pnt =0;
    while(pnt!=4)
    {
       if( (*ecx>>pnt) & 0x01 )
       {
	  switch(pnt)
	  {
	     case 0: 
		printf(" PREFETCHWT1");
		break;
	     case 3: 
		printf(" PKU");
		break;
	     case 4: 
		printf(" OSPKE");
		break;
	     default:
		break;
	  }
	  pnt++;
       }
       else
	  pnt++;
    }
    }

}

static inline void TLB(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx) // eax: 0x01, 0x02
{
   *eax = 0x02;
   __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
   printf("\n\n\t\t** TLB ** \n");
   char descriptor[4];
   int cnt = 1;
    //EAX
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*eax>>(8*cnt+4))&0xf, (*eax>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"TLB"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //ebx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*ebx>>(8*cnt+4))&0xf, (*ebx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"TLB"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //ecx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*ecx>>(8*cnt+4))&0xf, (*ecx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"TLB"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //edx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*edx>>(8*cnt+4))&0xf, (*edx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"TLB"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
}

static inline void L1_cache(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx) // eax: 0x01, 0x02
{
   *eax = 0x02;
   __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
   printf("\n\n\t\t** L1 Cache ** \n");
   char descriptor[4];
   int cnt = 1;
    //EAX
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*eax>>(8*cnt+4))&0xf, (*eax>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"1st"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //ebx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*ebx>>(8*cnt+4))&0xf, (*ebx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"1st"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //ecx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*ecx>>(8*cnt+4))&0xf, (*ecx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"1st"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //edx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*edx>>(8*cnt+4))&0xf, (*edx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H") && strstr(buf,"1st"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
}
/*
static inline void eax0x02(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
   *eax = 0x02;
   __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
   printf("** eax 0x02 ** \n");
   char descriptor[4];
   int cnt = 1;
    //EAX
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*eax>>(8*cnt+4))&0xf, (*eax>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //ebx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*ebx>>(8*cnt+4))&0xf, (*ebx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //ecx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*ecx>>(8*cnt+4))&0xf, (*ecx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
   //edx
   cnt=0;
   while(cnt != 4){
    sprintf(descriptor, "%x%xH",(*edx>>(8*cnt+4))&0xf, (*edx>>(8*cnt))&0xf);
    descriptor[3]=0;
    cnt++;
    // make sure every letter is uppercase
    for (int i=0; i<4; i++) {
        if(descriptor[i]>96 && descriptor[i]<123)
            descriptor[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpuid_0x2_encoding.txt","r");
    char buf[BSIZE];
    for(int i=0; i<BSIZE; i++)
        buf[i] = '\0';
    
    if (f == NULL)
    {
        printf("File didnt OPEN\n");
    }
    while(!feof(f))
    {
        fgets(buf,BSIZE,f);
        if(strstr(buf,descriptor) != NULL && strcmp(descriptor,"00H"))
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
   }
}
*/
static inline void memory_hierarchy(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    int cnt =0;
    int s =1;
    printf("\n\t\t** Memory Hierarchy **\n");
    while (s!=-1) {
        *ecx = cnt;
        *eax = 0x04;
        __asm__ __volatile__(
                             "cpuid;"
                             : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                             : "a"(*eax), "c"(*ecx)
                             );
        if ((*eax&0xf) != 0 ){
            printf("** eax 0x04 ecx 0x0%d ** \n", cnt++);
          //  printf("binary eax: %s\n", four_bytes_to_binary(*eax) );
            switch (*eax & 0x1f) {           //compare bits 04-00
                case 0:
                    printf("Null\n");
                    break;
                case 1:
                    printf("Data Cache\n");
                    break;
                case 2:
                    printf("Instruction Cache\n");
                    break;
                case 3:
                    printf("Unified Cache\n");
                    break;
                default:
                    printf("FAIL\n");
                    break;
            }
            printf("L%d\n",(*eax>>5)&7 );           //prints bits 07-05
	    //This Cache Size in Bytes
	    //= (Ways + 1) * (Partitions + 1) * (Line_Size + 1) * (Sets + 1)
	    printf("Cache Size: %d", (((*ebx>>22)&0x3ff) + 1) * (((*ebx>>12)&0x3ff) + 1) * ((*ebx&0x7ff) + 1) * (*ecx + 1));
	    printf(", System Coherency Line Size: %d", ((*ebx)&0xfff) + 1);
	    printf(", Physical Line partitions: %d", ((*ebx>>12)&0x3ff) + 1);
	    printf(", %d-way set associative\n", ((*ebx>>22)&0x3ff) + 1);
        }
        else{
            s = -1;
        }
    }
    
}

static inline void CPU_freq(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx) //eax = 0x0B
{

    printf("\n\t\t ** CPU's frequency **\n");
    *eax = 0x16;
    __asm__ __volatile__(
		  "cpuid;"
		  : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
		  : "a"(*eax), "c"(*ecx)
		  );
    printf("CPU's base frequency: %d MHz\n", *eax&0xffff);
    printf("Maximun frequency: %d MHz\n", *ebx&0xffff );
    printf("Bus frequency: %d MHZ\n", *ecx&0xffff );
}


static inline void Logical_CPUs(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx) //eax = 0x0B
{

    int cnt =0;
    int s =1;
    printf("\n\t\t ** Logical CPUs **\n");
    while (s!=-1) {
        *ecx = cnt;
        *eax = 0x0B;
        __asm__ __volatile__(
                             "cpuid;"
                             : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                             : "a"(*eax), "c"(*ecx)
                             );

        if ( ((*ecx>>8)&0xff) != 0 ){
        //if ( cnt != 4 ){
            //printf("** eax 0x0B,  ecx 0x0%d ** \n", cnt++);
            //printf("Level Type: ");
	    cnt++;
            switch ( (*ecx>>8) & 0xff) {           //compare bits 15-08
                case 0:
                    printf("Invalid\n");
                    break;
                case 1:
                    break;
                case 2:
		    printf("Level Type: ");
                    printf("Core\n");
		    printf("Number of Logical CPUs: %d\n", (*ebx) & 0xff);
                    break;
                default:
                    printf("N/A\n");
                    break;
            }
        }
        else{
//            printf("Finished enumerating Leaf 0x0B\n");
            s = -1;
        }
    }
}


static inline void addr_width(unsigned int *eax)
{

    *eax = 0x80000008;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax)
                         : "a"(*eax)
                         );
    printf("\n\t\t** Physical & Logical Address Width ** \n");
    if(*eax == 0 )
        printf("CPUID.80000008H:EAX[7:0] is NOT SUPPORTED\n");
    else{
       // printf("physical address width: %#x=%d bits\t size: %f\n", *eax&0xff,*eax&0xff,pow(2,(*eax&0xff)) );
        printf("physical address width: %d bits\n",*eax&0xff);
        printf("Logical address width: %d bits\n",(*eax&0xff00)>>8);
       // printf("Linear address width: %#x=%d bits\t size: %f\n", (*eax&0xff00)>>8,(*eax&0xff00)>>8,);
    }
    
}

static inline void pBrand(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    printf("Proccessor Brand: ");
    char PBrand[50];
    
    *eax = 0x80000002;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
    memcpy(PBrand, (char*)eax, 4);
    memcpy(PBrand+4, (char*)ebx, 4);
    memcpy(PBrand+8, (char*)ecx, 4);
    memcpy(PBrand+12, (char*)edx, 4);
    *eax = 0x80000003;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
    memcpy(PBrand+16, (char*)eax, 4);
    memcpy(PBrand+20, (char*)ebx, 4);
    memcpy(PBrand+24, (char*)ecx, 4);
    memcpy(PBrand+28, (char*)edx, 4);
    *eax = 0x80000004;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
    memcpy(PBrand+32, (char*)eax, 4);
    memcpy(PBrand+36, (char*)ebx, 4);
    memcpy(PBrand+40, (char*)ecx, 4);
    memcpy(PBrand+44, (char*)edx, 4);
    PBrand[49]=0;
    
    printf("%s\n", PBrand);
}
