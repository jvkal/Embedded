#include<reg52.h>//lcd1602 数字钟 可调整时间和日期
#include<intrins.h>
//定义一个蜂鸣器变量
sbit beep = P3^5;
//定义类型
#define uint unsigned int
#define uchar unsigned char
//lcd_data=P0
//位定义,显示器显示
sbit lcdrs=P2^2;
sbit lcdrw=P2^1;
sbit lcden=P2^0;
//定义按键功能
sbit k1=P1^0;//修改时间
sbit k2=P1^3;//+
sbit k3=P1^4;//-
sbit k4=P1^1;//定闹钟
sbit k5=P1^2;//秒表
//函数声明
uchar pz = 0;
uchar ppap =0 , kk =0;
uchar k4act = 0, k5act = 0;
uchar temp_1 = 0, temp_2 = 0,temp_3 = 0;
uchar k4_h = 0,k4_m = 0,k4_s = 0;//设定秒表模式下的时钟，分钟，秒钟
uchar k5_h = 0,k5_m = 0,k5_s = 0,k5_month=9,k5_day=3;//设定闹钟模式下的时钟，分钟，秒钟,月,日
uchar i,t=0,k1num=0,t_miaobiao=0;//t为记录的中断次数计数，k1num为k1按下的次数（实现修改时间功能）
uint year=2019;//year年
char h=0,m=0,s=0,w=7,month=9,day=3;//h时，m分，s秒，w星期，year年，month月，day日
//设定一开始的字符
uchar code table1[]={" 2019-09-02 SUN "};//日期
uchar code table2[]={"    00 00 00    "};//时间
//蜂鸣器模块
//蜂鸣器发声模块
void delay_spk(unsigned int i)
{
	while(i--);
}	   
//初始化声音
void spk_deep_init()
{
	unsigned char i=100;
	while(i--)
	{
		beep = 0;
		delay_spk(200);
		beep = 1;
		delay_spk(100);	
		beep = 0;
		delay_spk(50);
		beep = 1;
		delay_spk(80);	
	}
}
//延时函数 传入z既可以延时zms
void delay(uchar z)
{
	uchar x,y;
	for(x=z;x>0;x--){
	    for(y=110;y>0;y--);{
	    
	    }
	}
		
}
 //写位置，当输入com显示在不同的位置出现字符
void write_com(uchar com)
{
	lcdrs=0;//不显示的意思
	lcdrw=0;
	P0=com;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
}
//对lcd进行写数据处理，当后面+上int类型屏幕上显示对应的int信息
void write_dat(uchar dat)
{
	lcdrs=1;
	lcdrw=0;
	P0=dat;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
}
//显示日期的函数
void Monday(void)
{
	write_com(0x80+12);
	write_dat('M');
	write_com(0x80+13);
	write_dat('O');
	write_com(0x80+14);
	write_dat('N');
}
void Tuesday(void)
{
	write_com(0x80+12);
	write_dat('T');
	write_com(0x80+13);
	write_dat('U');
	write_com(0x80+14);
	write_dat('E');
}
void Wednesday(void)
{
	write_com(0x80+12);
	write_dat('W');
	write_com(0x80+13);
	write_dat('E');
	write_com(0x80+14);
	write_dat('D');
}
void Thursday(void)
{
	write_com(0x80+12);
	write_dat('T');
	write_com(0x80+13);
	write_dat('H');
	write_com(0x80+14);
	write_dat('U');
}
void Friday(void)
{
	write_com(0x80+12);
	write_dat('F');
	write_com(0x80+13);
	write_dat('R');
	write_com(0x80+14);
	write_dat('I');
}
void Saturday(void)
{
	write_com(0x80+12);
	write_dat('S');
	write_com(0x80+13);
	write_dat('A');
	write_com(0x80+14);
	write_dat('T');
}
void Sunday(void)
{
	write_com(0x80+12);
	write_dat('S');
	write_com(0x80+13);
	write_dat('U');
	write_com(0x80+14);
	write_dat('N');
}
void Print(uchar *str)	 //指定坐标输出字符串
{
	while(*str)  
		write_dat(*str++);
}
//lcd初始设置
void init(void)
{
	lcden=0;
	write_com(0x38); //8位数据端口,2行显示,5*7点阵
	write_com(0x0c); //显示模式
	write_com(0x06);//输入模式
	write_com(0x01);//清屏
	write_com(0x80);//切换当前位置到第一行
	TMOD=0x01;
	ET0=1;
	EA=1;
	TH0=0x3c;TL0=0xb0;//50ms
	TR0=1;
	spk_deep_init();	
}

//时间显示模块
void display_week(uchar week)//星期显示
{
	switch(week)
	{
	case 1:Monday();break;
	case 2:Tuesday();break;
	case 3:Wednesday();break;
	case 4:Thursday();break;
	case 5:Friday();break;
	case 6:Saturday();break;
	case 7:Sunday();break;
	}
}
void display_time(uchar ad,uchar time)//显示时间
{
	uchar a,b;
	a=time/10;b=time%10; //取十位和百位
	write_com(0x80+0x40+ad);
	write_dat(0x30+a);
	write_com(0x80+0x40+1+ad);
	write_dat(0x30+b);
}
void display_date(uint nian,char yue,char ri)//日期显示
{
	uint y1,y2,y3,y4;
	char a,b,c,d;
	y1=nian/1000;         //千位
	y2=nian%1000/100;     //百位
	y3=nian%1000%100/10;  //十位
	y4=nian%1000%100%10;  //个位
	a=yue/10;
	b=yue%10;
	c=ri/10;
	d=ri%10;
	write_com(0x80+1);
	write_dat(0x30+y1);
	write_com(0x80+2);
	write_dat(0x30+y2);
	write_com(0x80+3);
	write_dat(0x30+y3);
	write_com(0x80+4);
	write_dat(0x30+y4);
	write_com(0x80+6);
	write_dat(0x30+a);
	write_com(0x80+7);
	write_dat(0x30+b);
	write_com(0x80+9);
	write_dat(0x30+c);
	write_com(0x80+10);
	write_dat(0x30+d);
}
//显示闹钟
void show_all_time(void)
{
	display_date(year,month,day);
	display_week(w);
	display_time(10,s);
	display_time(7,m);
	display_time(4,h);	
}
void show_miaobiao_1(void)
{
	display_time(10,temp_1);
	display_time(7,temp_2);
	display_time(4,temp_3);
}
void show_miaobiao_2(void)
{
	display_time(10,k4_s);
	display_time(7,k4_m);
	display_time(4,k4_h);	
}
void show_naozhong(void)
{
	display_date(year,k5_month,k5_day);
	display_time(10,k5_s);
	display_time(7,k5_m);
	display_time(4,k5_h);	
}
void keyscan(void)//按键扫描
{	
	uint a=0,b=0,c=0;
	if(k5==0)//闹钟模式
	{
		delay(10);//消除抖动
		if(k5==0)
		{
			while(!k5);
			k5act++; //用于计数
			write_com(0x01);//清屏函数
			if(k1num==1)
			{write_com(0x80+0x40+11);write_com(0x0f);}//s
			if(k1num==2)
			{write_com(0x80+0x40+8);}//m
			if(k1num==3)
			{write_com(0x80+0x40+5);}//h
			if(k1num==4)
			{write_com(0x80+10);}//day
			if(k1num==5)
			{write_com(0x80+7);}//month
			if(k5act==6)
			{
				write_com(0x01);//清屏函数
				Print("set ok");
				for(c=131;c>0;c--)		 //100000微妙，单片机小精灵测出来
			        for(b=156;b>0;b--)
			            for(a=3;a>0;a--);
				write_com(0x01);//清屏函数
				k5act=0;	 	   //退出秒表功能
				write_com(0x0c);   //显示模式
				write_com(0x01);   //清屏函数
				pz = 1;				
			}	
		}	
	}
	if(k5act!=0)
	{
		if(k2==0)
		{
			delay(5);
			if(k2==0)
			{
				while(!k2);
				if(k5act==1)
				{
					k5_s++;
					if(k5_s==60)
					k5_s=0;
					display_time(10,k5_s);
				}
				if(k5act==2)
				{
					k5_m++;
					if(k5_m==60)
					k5_m=0;
					display_time(7,k5_m);
				}
				if(k5act==3)
				{
					k5_h++;
					if(k5_h==24)
					k5_h=0;
					display_time(4,k5_h);
				}
				if(k5act==4)
				{
					k5_day++;
					if(k5_day==32)
					k5_day=1;
					display_date(year,k5_month,k5_day);
				}
				if(k5act==5)
				{
					k5_month++;
					if(k5_month==13)
					k5_month=1;
					display_date(year,k5_month,k5_day);
				}
			}
		}
		if(k3==0)
		{
			delay(10);
			if(k3==0)
			{
				while(!k3);
				if(k5act==1)
				{
					k5_s--;
					if(k5_s==-1)
					k5_s=59;
					display_time(10,k5_s);
				}
				if(k5act==2)
				{
					k5_m--;
					if(m==-1)
					k5_m=59;
					display_time(7,k5_m);
				}
				if(k5act==3)
				{
					k5_h--;
					if(h==-1)
					k5_h=23;
					display_time(4,k5_h);
				}
				if(k5act==4)
				{
					k5_day--;
					if(day==0)
					k5_day=31;
					display_date(year,k5_month,k5_day);
				}
				if(k5act==5)
				{
					k5_month--;
					if(month==0)
					k5_month=12;
					display_date(year,k5_month,k5_day);
				}
			}
		}
	}

	if(k4==0)//秒表模式
	{
		delay(10);//消除抖动
		if(k4==0)
		{
			while(!k4);
			k4act++; //用于计数
			if(k4act==1)//
			{	
				write_com(0x01);//清屏函数
				Print("welcome to MB");
				k4_s=0;
				k4_h=0;
				k4_m=0;
			}
			if(k4act==2)
			{
				k4act=0;	 	   //退出秒表功能
				write_com(0x0c);   //显示模式
				write_com(0x01);//清屏函数
				//清零计数
				k4_s=0;
				k4_m=0;
				k4_h=0;
				temp_1 = 0;
				temp_2 = 0;
				temp_3 = 0;
				ppap=0;				
			}	
		}			
	}
	if(k4act==1){
		if(k5==0){
			delay(10);
			if(k5==0){
				while(!k5);	
				{
					ppap ++;
					if(ppap%2!=0){
						kk=1;
						k4_s = temp_1;
						k4_m = temp_2;
						k4_h = temp_3;	
					}else{				
						kk=0;
					 	temp_1 = k4_s;
						temp_2 = k4_m;
						temp_3 = k4_h;
					}							
				}
			}
		}
	}
	if(k1==0) //修改时间模式
	{
		delay(10);
		if(k1==0)
		{
			k1num++;
			TR0=1;
			t=0; //让晶振次数又重新归零
			while(!k1);
			if(k1num==1)
			{write_com(0x80+0x40+11);write_com(0x0f);}//s
			if(k1num==2)
			{write_com(0x80+0x40+8);}//m
			if(k1num==3)
			{write_com(0x80+0x40+5);}//h
			if(k1num==4)
			{write_com(0x80+14);}//week
			if(k1num==5)
			{write_com(0x80+10);}//day
			if(k1num==6)
			{write_com(0x80+7);}//month
			if(k1num==7)
			{write_com(0x80+4);}//year
			if(k1num==8)
			{
				k1num=0;
				write_com(0x0c);   //显示模式
				TR0=1;
			}			
		}
	}
	if(k1num!=0)
	{
		if(k2==0)
		{
			delay(5);
			if(k2==0)
			{
				while(!k2);
				if(k1num==1)
				{
					s++;
					if(s==60)
					s=0;
					display_time(10,s);
				}
				if(k1num==2)
				{
					m++;
					if(m==60)
					m=0;
					display_time(7,m);
				}
				if(k1num==3)
				{
					h++;
					if(h==24)
					h=0;
					display_time(4,h);
				}
				if(k1num==4)
				{
					w++;
					if(w==8)
					w=1;
					display_week(w);
				}
				if(k1num==5)
				{
					day++;
					if(day==32)
					day=1;
					display_date(year,month,day);
				}
				if(k1num==6)
				{
					month++;
					if(month==13)
					month=1;
					display_date(year,month,day);
				}
				if(k1num==7)
				{
					year++;
					display_date(year,month,day);
				}
			}
		}
		if(k3==0)
		{
			delay(10);
			if(k3==0)
			{
				while(!k3);
				if(k1num==1)
				{
					s--;
					if(s==-1)
					s=59;
					display_time(10,s);
				}
				if(k1num==2)
				{
					m--;
					if(m==-1)
					m=59;
					display_time(7,m);
				}
				if(k1num==3)
				{
					h--;
					if(h==-1)
					h=23;
					display_time(4,h);
				}
				if(k1num==4)
				{
					w--;
					if(w==0)
					w=7;
					display_week(w);
				}
				if(k1num==5)
				{
					day--;
					if(day==0)
					day=31;
					display_date(year,month,day);
				}
				if(k1num==6)
				{
					month--;
					if(month==0)
					month=12;
					display_date(year,month,day);
				}
				if(k1num==7)
				{
					year--;
					display_date(year,month,day);
				}
			}
		}
	}
	if(k4act!=0){
	 if(ppap!=0){
		if(kk==0){
			show_miaobiao_1();
		}else{
			show_miaobiao_2();
		}
	 }
	}else if(k5act!=0){
		show_naozhong();
	}else{
	  show_all_time(); //显示时间函数
	}	
}


void main(void)//主函数
{
	uint a=0,b=0,c=0;
	init();	    
	write_com(0x80);//第一行
    Print("welcome to clock");
    for(c=131;c>0;c--)		 //100000微妙，单片机小精灵测出来
        for(b=156;b>0;b--)
            for(a=3;a>0;a--);
	/*		
	 for(c=143;c>0;c--)
        for(b=144;b>0;b--)
            for(a=88;a>0;a--);*/
	write_com(0x01);//清屏函数	 
    write_com(0x80);//第一行
	for(i=0;i<15;i++)
		{write_dat(table1[i]);delay(5);}
	write_com(0x80+0x40);//第二行
	for(i=0;i<11;i++)
		{write_dat(table2[i]);delay(5);}
	a=0,b=0,c=0;
	while(1)
	{
		keyscan();
		if(pz == 1)
		{
			if(k5_s==s)
			{
				if(k5_m==m)
				{
					if(k5_h==h)
					{
						if(k5_day==day)
						{
							if(k5_month=month)
							{
								write_com(0x01);//清屏函数
								write_com(0x80);//第一行
								Print("time up");
								for(c=143;c>0;c--)
							        for(b=144;b>0;b--)
							            for(a=88;a>0;a--);
								k5_h = 0,k5_m = 0,k5_s = 0,k5_month=9,k5_day=3;
								write_com(0x01);//清屏函数
							}
						}
					}
				}
			}
		}
	}
}

void timer0() interrupt 1//定时器T0中断函数
{
	t++;
	TH0=0x3c;TL0=0xb0;//50ms
	if(t==20)//t相当于软件控制时间的参数
	{s++;t=0;}
	if(s==60)
	{m++;s=0;}
	if(m==60)
	{h++;m=0;}
	if(h==24)
	{w++;day++;h=0;}
	if(w==8)
	{w=1;}
	if(day==32)
	{month++;day=1;}
	if(month==13)
	{year++;month=1;}	
	t_miaobiao++;
	TH0=0x3c;TL0=0xb0;//50ms
	if(t_miaobiao==20)
	{t_miaobiao=0;k4_s++;}
	if(k4_s==60)
	{k4_m++;k4_s=0;}
	if(k4_m==60)
	{k4_h++;k4_m=0;}
	if(k4_h==24){k4_h=0;k4_s=0;k4_m=0;}

}
    
