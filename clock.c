#include<reg52.h>//lcd1602 数字钟 可调整时间和日期
#define uint unsigned int
#define uchar unsigned char
//lcd_data=P0
//位定义
sbit lcdrs=P2^2;
sbit lcdrw=P2^1;
sbit lcden=P2^0;
sbit k1=P3^3;//mod
sbit k2=P3^2;//+
sbit k3=P3^1;//-
//函数声明
uchar i,t=0,k1num=0;//t为中断次数计数，k1num为k1按下的次数
uint year=2013;//year年
char h=0,m=0,s=0,w=7,month=3,day=3;//h时，m分，s秒，w星期，year年，month月，day日

uchar code table1[]={" 2013-03-03 SUN "};//日期
uchar code table2[]={"    00:00:00    "};//时间
//延时z ms
void delay(uchar z)
{
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void write_com(uchar com)
{
	lcdrs=0;
	lcdrw=0;
	P0=com;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
}
//lcd写数据
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
//lcd初始设置
void init(void)
{
	lcden=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
	TMOD=0x01;
	ET0=1;
	EA=1;
	TH0=0x3c;TL0=0xb0;//50ms
	TR0=1;
	write_com(0x80);//第一行
	for(i=0;i<15;i++)
		{write_dat(table1[i]);delay(5);}
	write_com(0x80+0x40);//第二行
	for(i=0;i<11;i++)
		{write_dat(table2[i]);delay(5);}	
}


void display_time(uchar ad,uchar time)//时间显示
{
	uchar a,b;
	a=time/10;b=time%10;
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

void keyscan(void)//按键扫描
{
	if(k1==0)
	{
		delay(10);
		if(k1==0)
		{
			k1num++;TR0=0;t=0;
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
			{k1num=0;write_com(0x0c);TR0=1;}			
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
display_date(year,month,day);
display_week(w);
display_time(10,s);
display_time(7,m);
display_time(4,h);
}


void main(void)//主函数
{
	init();
	while(1)
	{
		keyscan();
	}
}

void timer0() interrupt 1//定时器T0中断函数
{
	t++;
	TH0=0x3c;TL0=0xb0;//50ms
	if(t==20)
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
}
//修改了两次
