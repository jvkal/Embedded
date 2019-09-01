#include <AT89X52.h>

unsigned char code SEGtable[]=
{
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0x00
};

sbit sw7 = P0 ^ 7;
sbit sw6 = P0 ^ 6;
sbit sw5 = P0 ^ 5;
sbit sw4 = P0 ^ 4;
sbit sw3 = P0 ^ 3;
sbit sw2 = P0 ^ 2;
sbit sw1 = P0 ^ 1;
sbit sw0 = P0 ^ 0;

sbit FMQ = P1 ^ 7;
bit alarmflg = 0;
unsigned RxByte = 0x00;
unsigned char counter = 0;
unsigned char Scounter,Lcounter;  //分别存放自己的水位状态和远方的水位状态
unsigned char FMcounter = 0x02;   //报警状态参数

void DelayFM(unsigned int x)  
{
 	unsigned char t;
	while(x--)
	{
	 	for(t=0;t<120;t++);
	}
}
//蜂鸣器驱动函数，参数为发声声调
void FM(unsigned char x)
{
 	unsigned char i;
	for(i=0;i<100;i++)
	{
	 	FMQ = ~FMQ;
		DelayFM(x);
	}
	FMQ = 0;
}

void Delayms(unsigned int MS)                     //延时ms函数             
{
	unsigned int i,j;
	for( i=0;i<MS;i++)
		for(j=0;j<1141;j++);	                        
}
void InitUART(void)
{
    TMOD = 0x20;     //9600bps
    SCON = 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}
void Send(unsigned char x)
{
  SBUF = x;
  while(TI == 0);
  TI = 0;
}
void Serial(void) interrupt 4 using 1
{
	unsigned char RxByte;
	if(RI == 1)
	{
		RxByte = SBUF;
		RI = 0;
    P2 = RxByte;
    switch(RxByte)
    {
      case 0x00: Lcounter = 0;break;
      case 0xf9: Lcounter = 1;break;
      case 0xa4: Lcounter = 2;break;
      case 0xb0: Lcounter = 3;break;
      case 0x99: Lcounter = 4;break;
      case 0x92: Lcounter = 5;break;
      case 0x82: Lcounter = 6;break;
      case 0xF8: Lcounter = 7;break;
      case 0x80: Lcounter = 8;break;
      default:{};
    }
    if(Scounter > Lcounter)
    {
      if((Scounter - Lcounter) > 2)       //如果超过2%
      {
        FMcounter = Scounter - Lcounter;
        alarmflg = 1;              //报警标志位
      }
      else
      {
        alarmflg = 0;
      }
    }
    else
    {
      alarmflg = 0;     //清除
    }
	}	
}
main(void)
{
   InitUART();
   while(1)
   {
     Delayms(100);  
     P0 = 0xFF;
     counter = 0;
     counter = counter + (unsigned char)sw7;
     counter = counter + (unsigned char)sw6;
     counter = counter + (unsigned char)sw5;
     counter = counter + (unsigned char)sw4;
     counter = counter + (unsigned char)sw3;
     counter = counter + (unsigned char)sw2;
     counter = counter + (unsigned char)sw1;
     counter = counter + (unsigned char)sw0;  //计算当前的负载
     Scounter = counter;
     Send(SEGtable[counter]);
     if(alarmflg == 1)
     {
      FM(FMcounter);
     }
   }
}
