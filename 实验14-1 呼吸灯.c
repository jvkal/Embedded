#include <AT89X52.h>

#define MAX 0x50   //定时上限定义
#define MIN 0x00   //定时下限定义
#define TIMELINE 11         //时间分频常数
#define TRUE  1
#define FALSE 0             //标志位常数

unsigned int TimeCounter;
bit ArrowFlg = 0;         //方向标志位
unsigned char upCounter,downCounter;    //增加计数器和减少计数器

sbit LED=P2^0;

//T0的中断服务子函数
void T0Deal() interrupt 1 using 0
{
	TH0=0xf1;
	TL0=0xf1;
	TR0=1;
	TimeCounter++;  //定时计数器增加
	if(TimeCounter == TIMELINE)
	{
		if((upCounter == MAX)&&(downCounter == MIN))  //计数方向标志位切换
    {
      ArrowFlg = FALSE;
    }
		if((upCounter == MIN)&&(downCounter == MAX))
    {
      ArrowFlg= TRUE;
    }
		if(ArrowFlg == 1)   //如果是增加计数
    {
      upCounter++;
      downCounter--;
    }
		else               //如果是减少计数
    {
      upCounter--;
      downCounter++;
    }
		TimeCounter=0;
	}
}
//延时函数
void Delay(unsigned int i)
{
	unsigned int j;
	while(i--)
	{
		for(j=0;j<32;j++);     //延时
	}
}

void main()
{
	upCounter = MIN;
	downCounter = MAX;     //计数器初始化
	TMOD = 0x01;          //设置定时器工作方式
	TH0 = 0xF0;
	TL0 = 0xF0 ;          //T0初始化值
	EA = 1;
	ET0 = 1;              //开中断
	TR0 = 1;              //启动T0
	while(1)
	{
		LED=0;            //输出变化的PWM波形
		Delay(downCounter);   
		LED=1;
		Delay(upCounter);
	}
}

