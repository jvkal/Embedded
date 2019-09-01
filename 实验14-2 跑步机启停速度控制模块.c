#include <AT89X52.h>			

unsigned char code SEGtable[ ]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};     //字符编码

sbit SEKey =  P1 ^ 0;			//启动、暂停和停止按键
sbit INCKey = P1 ^ 4;     //速度增加键
sbit DECKey = P1 ^ 7;     //速度减少键

sbit pauseLED = P3 ^ 0;		//暂停指示灯
sbit stopLED = P3 ^ 7;			    //停止指示灯

bit keyFlg;					//按键长按、短按标志位 0为短按 1为长按

unsigned char TimeCounter;					//计数专用
unsigned char KeyDownFlg,set;			//按键专用

unsigned char yansi,key,send,unitsdData,tensdData,num,keynum;
bit	keybz;

//延时函数
void delay(unsigned char time)
{
	unsigned char x,y;
	for(x=time;x>0;x--)
  {
			for(y=110;y>0;y--);
  }
}
//扫描启动、停止、暂停按键
void StartAndStopKeyScan()
{	
  if(SEKey == 0)					//键被按下
	{
	  KeyDownFlg = 1;					//置键按下标志位
	  TR0 = 1;							//启动定时器
	}
	
	if((SEKey == 1) && (KeyDownFlg == 1))		//判断设置键是否松开
	{
    KeyDownFlg = 0;						//清除键标志位
		if(keyFlg == 0)				//如果是短按
		{
		  stopLED = 1;
		  pauseLED =~pauseLED;
	  }
	  TR0 = 0;							//关闭定时器
  	TimeCounter = 0;						  	//计数器清零
	  keyFlg = 0;						//清除短按、长按标志位
	}	
}

void SegDisplay(unsigned char tensdData,unsigned char unitsdData) //延时函数
{
  P0 = SEGtable[tensdData];
  	delay(10);	
	P2 = SEGtable[unitsdData];	
  	delay(10);
}

void keyscan1()			//具有连加功能的的按键扫描程序
{
	if(INCKey==0)							//判断INCKey键是否按下?
		{
			SegDisplay(tensdData,unitsdData);					//延时去抖动
			if(INCKey==0)					//如果真接下就去执行键盘程序
			{
				if(keybz==0)				//判断是否是第一次按下?
						{
							num++;		//值加一
							if(num==100)	//判断是否加到100	
								{num=0;}//加到100清0
							keybz=1;	//第一次进来置标志位
							key--;		//按键次数计数器
							keynum=5;	//快加按键次数计数器
							tensdData=num/10;	//BCD码转为十进制值
							unitsdData=num%10;
							return;		//不用检测松手直接返回
									
						}
					else				//如果是第二次按下则执行下面的语句
					{
						if(key==0)		//判断按键次数到100次了吗?		
							{
							if(keynum==0)	//检测按下时间是否超过加5次的时间
								{
									key=10;		//如果按键持续时间超过加5次的时间则以后每10次执行加工厂操作
									num++;
									if(num==99)
										{
											num=0;
										}
									tensdData=num/10;
									unitsdData=num%10;
									return;
								}
							  else		//没有到5次时间则执行下面的语句
								  keynum--;	//快加计数器减一
								  key=100;
								  num++;
							  if(num==99)
								  {
								  num=0;
								  }
							  tensdData=num/10;
							  unitsdData=num%10;
							  return;
								  
							}
						else			//没有到100次下次再来判断.
							key--;
							return;
					
					}

										
			}

			if(INCKey!=0)							//松手后所有的计数器清并置默认值
				{
					keynum=5;
				        key=30;
					keybz=0;
					return;						//返回
				}		    
		}
/*************************以下是减一键处理程序,执行效果同加一程序*******************************************************************/	
	if(DECKey==0)
		{
			SegDisplay(tensdData,unitsdData);
			if(DECKey==0)
			{
				if(keybz==0)
					{
						num--;
						if(num==-1)
							{num=99;}
						keybz=1;
						key--;
						keynum=5;
						tensdData=num/10;
						unitsdData=num%10;
						return;
								
					}
				else
					{
						if(key==0)
							{
							if(keynum==0)
								{
								key=10;
								num--;
								if(num==0)
									{
										num=99;
									}
								tensdData=num/10;
								unitsdData=num%10;
								return;
								}
							  else
							  keynum--;
							  key=100;
							  num--;
							  if(num==0)
								  {
								  num=99;
								  }
							  tensdData=num/10;
							  unitsdData=num%10;
							  return;
								  
							}
						else
							key--;
							return;					
					}										
			}			    
		}
	if(DECKey!=0)
		{
			keynum=5;
		        key=30;
			keybz=0;
			return;
		}
}
void Timer0Interrupt(void) interrupt 1
{
	TimeCounter++;
	if(TimeCounter==250)			//定时时间到
	{
		keyFlg = 1;				      //置长按标志位
		pauseLED = 1;				
		stopLED = ~stopLED;			//取反
		TR0=0;					        //关闭定时器
	}
  TH0 = 0xd8;
  TL0 = 0xf0;					//定时器重新赋初值
}
//主函数
void main(void)
{
	EA = 1;
	TMOD = 0x01;
  TH0 = 0xd8;  			//10ms
  TL0 = 0xf0;
	ET0 = 1;				//设置定时器1

	unitsdData=0;
	tensdData=0;
	P1=0xff;
	P2=0;
	key=100;
  SegDisplay(0,9);
	while(1)
	{
	  StartAndStopKeyScan();				//调用按键扫描子程序
    keyscan1();
  	SegDisplay(tensdData,unitsdData);
	}
}


