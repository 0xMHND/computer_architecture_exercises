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

static inline void eax0x02(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);
static inline void cpu_model(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);//eax0x01
static inline void TLB(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);
static inline void Logical_CPUs(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);
static inline void addr_width(unsigned int *eax);
static inline void cache_info(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx);
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
    printf("\n**********************************************************\n");    
    cpu_model(&eax, &ebx, &ecx, &edx);
    TLB(&eax, &ebx, &ecx, &edx); //TLB Information
    eax0x02(&eax, &ebx, &ecx, &edx); //TLB Information
    Logical_CPUs(&eax, &ebx, &ecx, &edx);
    cache_info(&eax,&ebx,&ecx,&edx);
    addr_width(&eax);
    printf("**********************************************************\n");    
    
    return 0;
}
#define BSIZE 128
static inline void cpu_model(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    *eax = 0x0;
    __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
    printf("Max input value for cpuid: %#x\n",*eax);
    printf("\t\t** CPU model ** \n");
    //EAX
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
    /*
    char code_num[7];
    sprintf(code_num, "0%x_%xH",(*eax>>8)&0xf,( ((*eax>>12)&0xf0)+((*eax>>4)&0xf) ));
    code_num[6]=0;
    // make sure every letter is uppercase
    for (int i=0; i<7; i++) {
        if(code_num[i]>96 && code_num[i]<123)
            code_num[i] -= 32;
    }
    
    FILE *f;
    f = fopen("cpu_CN.txt","r");
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
        if(strstr(buf,code_num) != NULL)
        {
            printf("%s", buf);
            fclose(f);
            break;
        }
    }
    fclose(f);
    //EBX
    printf("Number of Logical CPUs: %d\n", (*ebx>>16) & 0xff);
    
    //printf("hex ecx: %x\n", ecx);
    //printf("hex edx: %x\n", edx);
    */
}
/*
 *eax = 0x01;
 __asm__ __volatile__(
 "cpuid;"
 : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
 : "a"(*eax)
 );
 printf("** eax 0x01 ** \n");
 printf("binary eax: %s\n", four_bytes_to_binary(eax) );
 //eax [13:12]: Processor Type [Original OEM Processor]
 //eax [07:04]: Model          [ 00111101 = 3d]
 //eax [11:08]: Family ID      [0110]
 
 printf("hex ebx: %x\n", ebx);
 //ebx 00H: Null
 //ebx F0H: 64-Byte prefetching
 //ebx B5H: Instruction TLB: 4KByte pages, 8-way set associative, 64 entries
 //ebx FFH: CPUID leaf 2 does not report cache descriptor information, use CPUID leaf 4 to query cache parameters
 
 printf("hex ecx: %x\n", ecx);
 //ecx 00H: Null
 //ecx 00H: Null
 //ecx 00H: Null
 //ecx 00H: Null
 
 printf("hex edx: %x\n", edx);
 //edx 00H: Null
 //edx C3H: Shared 2nd-Level TLB: 4 KByte /2 MByte pages, 6-way associative, 1536 entries. Also 1GBbyte pages, 4-way, 16 entries.
 //edx 00H: Null
 //edx 00H: Null
 */
static inline void TLB(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx) // eax: 0x01, 0x02
{
   *eax = 0x02;
   __asm__ __volatile__(
                         "cpuid;"
                         : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                         : "a"(*eax)
                         );
   printf("\n\t\t** TLB ** \n");
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
static inline void cache_info(unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx)
{
    int cnt =0;
    int s =1;
    printf("\n\t\t** Cache info **\n");
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
	    printf("Cache Size: %d\n", (((*ebx>>22)&0x3ff) + 1) * (((*ebx>>12)&0x3ff) + 1) * ((*ebx&0xfff) + 1) * (*ecx + 1));
        }
        else{
            printf("Finished enumerating Leaf 0x04\n");
            s = -1;
        }
    }
    
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
            printf("** eax 0x0B,  ecx 0x0%d ** \n", cnt++);
            printf("Level Type: ");
            switch ( (*ecx>>8) & 0xff) {           //compare bits 15-08
                case 0:
                    printf("Invalid\n");
                    break;
                case 1:
                    printf("SMT\n");
                    break;
                case 2:
                    printf("Core\n");
                    break;
                default:
                    printf("N/A\n");
                    break;
            }
            //printf("L%d\n",(*eax&0x30)>>4 );           //prints bits 07-05
	    printf("Number of Logical CPUs: %d\n", (*ebx) & 0xff);
            
            //printf("binary ebx: %s\n", four_bytes_to_binary(*ebx) );
            //printf("binary ecx: %s\n", four_bytes_to_binary(*ecx) );
            //printf("binary edx: %s\n", four_bytes_to_binary(*edx) );
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
/*k
 *
 eax = 0x04;
 ecx = 0x0;
 __asm__ __volatile__(
 "cpuid;"
 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
 : "a"(eax), "c"(ecx)
 );
 printf("** eax 0x04 ecx 0x00** \n");
 printf("binary eax: %s\n", four_bytes_to_binary(eax) );
 //eax[04:00]: Data cache
 //eax[07:05]: Level 2
 //eax[08]: Data cache
 //eax[09]: Data cache
 //eax[4:0]: Data cache
 //eax[4:0]: Data cache
 printf("binary ebx: %s\n", four_bytes_to_binary(ebx) );
 //ebx 00H: Null
 //ebx F0H: 64-Byte prefetching
 //ebx B5H: Instruction TLB: 4KByte pages, 8-way set associative, 64 entries
 //ebx FFH: CPUID leaf 2 does not report cache descriptor information, use CPUID leaf 4 to query cache parameters
 
 printf("binary ecx: %s\n", four_bytes_to_binary(ecx) );
 //ecx 00H: Null
 //ecx 00H: Null
 //ecx 00H: Null
 //ecx 00H: Null
 
 printf("binary edx: %s\n", four_bytes_to_binary(edx) );
 //edx 00H: Null
 //edx C3H: Shared 2nd-Level TLB: 4 KByte /2 MByte pages, 6-way associative, 1536 entries. Also 1GBbyte pages, 4-way, 16 entries.
 //edx 00H: Null
 //edx 00H: Null
*/
