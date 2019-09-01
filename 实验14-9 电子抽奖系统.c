#include <AT89X52.h>
#include <intrins.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

bit  bT0Flg = FALSE;

//U2 595的驱动引脚定义
sbit sbSHCP2 = P2^0;
sbit sbDS2 = P2^1;
sbit sbSTCP2 = P2^2;
//U3 595的驱动引脚定义
sbit sbSHCP3 = P2^3;
sbit sbDS3 = P2^4;
sbit sbSTCP3 = P2^5;
//U4 595的驱动引脚定义
sbit sbSHCP4 = P2^6;
sbit sbDS4 = P2^7;
sbit sbSTCP4 = P3^0;
//U5 595的驱动引脚定义
sbit sbSHCP5 = P3^1;
sbit sbDS5 = P3^3;
sbit sbSTCP5 = P3^4;
//U6 595的驱动引脚定义
sbit sbSHCP6 = P3^5;
sbit sbDS6 = P3^6;
sbit sbSTCP6 = P3^7;

unsigned char temp2,temp3,temp4,temp5,temp6;
bdata unsigned char sw;    //位定义

sbit sw0 = sw ^ 0;
sbit sw1 = sw ^ 1;
sbit sw2 = sw ^ 2;
sbit sw3 = sw ^ 3;
sbit sw4 = sw ^ 4;
sbit sw5 = sw ^ 5;
sbit sw6 = sw ^ 6;
sbit sw7 = sw ^ 7;

unsigned char code SEGtable[]=
{
 	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,	
};
/*
void Delay(unsigned int x)
{
 	unsigned char i;
	while(x--)
	{
	 	for(i=0;i<120;i++);
	}
} */

//595输入函数2号
void Input5952()
{
 	unsigned char i;
	for(i=0;i<8;i++)
	{
	 	temp2 <<= 1;
		sbDS2   = CY;
		sbSHCP2 = 1;
		_nop_();
		_nop_();
		sbSHCP2 = 0;
	}
}
//595输出函数2号
void Output5952()
{
 	sbSTCP2 = 0;
	_nop_();
	sbSTCP2 = 1;
	_nop_();
	sbSTCP2 = 0;
}
//595输入函数3号
void Input5953()
{
 	unsigned char i;
	for(i=0;i<8;i++)
	{
	 	temp3 <<= 1;
		sbDS3   = CY;
		sbSHCP3 = 1;
		_nop_();
		_nop_();
		sbSHCP3 = 0;
	}
}
//595输出函数3号
void Output5953()
{
 	sbSTCP3 = 0;
	_nop_();
	sbSTCP3 = 1;
	_nop_();
	sbSTCP3 = 0;
}

//595输入函数4号
void Input5954()
{
 	unsigned char i;
	for(i=0;i<8;i++)
	{
	 	temp4 <<= 1;
		sbDS4   = CY;
		sbSHCP4 = 1;
		_nop_();
		_nop_();
		sbSHCP4 = 0;
	}
}
//595输出函数4号
void Output5954()
{
 	sbSTCP4 = 0;
	_nop_();
	sbSTCP4 = 1;
	_nop_();
	sbSTCP4 = 0;
}

//595输入函数5号
void Input5955()
{
 	unsigned char i;
	for(i=0;i<8;i++)
	{
	 	temp5 <<= 1;
		sbDS5   = CY;
		sbSHCP5 = 1;
		_nop_();
		_nop_();
		sbSHCP5 = 0;
	}
}
//595输出函数5号
void Output5955()
{
 	sbSTCP5 = 0;
	_nop_();
	sbSTCP5 = 1;
	_nop_();
	sbSTCP5 = 0;
}

//595输入函数6号
void Input5956()
{
 	unsigned char i;
	for(i=0;i<8;i++)
	{
	 	temp6 <<= 1;
		sbDS6   = CY;
		sbSHCP6 = 1;
		_nop_();
		_nop_();
		sbSHCP6 = 0;
	}
}
//595输出函数6号
void Output5956()
{
 	sbSTCP6 = 0;
	_nop_();
	sbSTCP6 = 1;
	_nop_();
	sbSTCP6 = 0;
}

void initrand(void)
{
   	unsigned char counter=0;
    P1 = 0xff;
    sw = P1;
    counter = 0;  //统计开关闭合的数码
    if(sw0 == 1)
    {
       counter++;
    }
    if(sw1 == 1)
    {
       counter++;
    }
    if(sw2 == 1)
    {
       counter++;
    }
    if(sw3 == 1)
    {
       counter++;
    }
    if(sw4 == 1)
    {
       counter++;
    }
    if(sw5 == 1)
    {
       counter++;
    }
    if(sw6 == 1)
    {
       counter++;
    }
    if(sw7 == 1)
    {
       counter++;
    }
    srand(counter);       //初始化种子
}


void Timer0Init(void)                           //定时器0初始化函数
{
  TMOD = 0x01;                                  //设置T1工作方式
	TH0 = 0x00;
	TL0 = 0x0C;					                          //100ms定时
  ET0 = 1;                                      //开启定时器0中断
//  TR0 = 1;                                      //启动定时器
}
void Timer0Deal(void) interrupt 1 using 1       //定时器0中断处理函数
{
	ET0 = 0;                       //首先关闭中断
	TH0 = 0x00;                    //然后重新装入预制值
	TL0 = 0x0C;				
  ET0 = 1;                       //打开T0中断
  bT0Flg = TRUE;                  //定时器中断标志位
}
EX_INT0() interrupt 0 using 1         //外部中断0服务函数
{
  if(TR0 == 1)                       //判断当前定时计数器的状态
  {
    TR0 = 0;
  }
  else
  {
    TR0 = 1;
    initrand();   //初始化种子
  }
}
void main()
{
  unsigned int randdata = 0;
  unsigned char wdata,qdata,baidata,sdata,gdata;
  Timer0Init();                     //初始化时钟
  IT0 = 1;                           //设置外部中断0触发方式为低脉冲
  EX0 = 1;                           //使能外部中断0
  EA = 1;                           //打开串口中断标志
	while(1)
	{
    while(bT0Flg==FALSE);            //等待延时标志位
    bT0Flg=FALSE;  

    randdata = 2 * rand();    //获得随机数
  
    wdata = randdata/10000;    //输出万位
    temp2 = SEGtable[wdata];
	  Input5952();
		Output5952();
    
    qdata = randdata%10000/1000;  //输出千位
    temp3 =  SEGtable[qdata];
	  Input5953();
		Output5953();

    baidata = randdata%1000/100;  //输出百位
    temp4 =  SEGtable[baidata];
	  Input5954();
		Output5954();

    sdata = randdata%100/10;    //输出10位
    temp5 =  SEGtable[sdata];
	  Input5955();
		Output5955();

    gdata = randdata%10;    //输出个位
    temp6 =  SEGtable[gdata];
	  Input5956();
		Output5956();
	}
}