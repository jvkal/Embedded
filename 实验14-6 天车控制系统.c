#include "AT89X52.h"
#include "intrins.h"

// Definition for output port and input pins
#define out_port  P3

unsigned char const sequence[8] = {0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x03};

sbit key_for = P1 ^ 2;
sbit key_rev = P1 ^ 3; 

sbit Inc = P1 ^ 0;
sbit Dec = P1 ^ 1;
sbit Dir = P2 ^ 0;
sbit PWM = P2 ^ 1;

int speed;

void delay(unsigned int j)
{ 
  for(; j>0; j--);
}
void delayms(unsigned int j)
{ 
  unsigned char i;
  for(; j>0; j--)
  { 
    i = 120;
    while (i--);  
  }
 }

void DCMotoDeal(void)
{
      if(!Inc)
      {
         speed = speed > 0 ? speed - 1 : 0;
      }
      if(!Dec)
      {
         speed = speed < 500 ? speed + 1 : 500;
       } 
      PWM=1;
      delay(speed);
      PWM=0;
      delay(500-speed);
}

void StepMotoDeal(void)
{
    unsigned char i;
    if (!key_for)
    { 
       i = i<8 ? i+1 : 0;
         out_port = sequence[i];
         delayms(50);
    }
    else if (!key_rev)
    { 
        i = i>0 ? i-1 : 7;	
        out_port = sequence[i];
        delayms(50);
    }
}

void main(void)
{ 
   // 选择方向和时间
   Dir = 1;
   if (Dir)
   {
      speed = 400;
    }
   else
   {
      speed = 100;
   }
   out_port = 0x03;
   while(1)
    { 
      DCMotoDeal();
      StepMotoDeal();
    }
 }


