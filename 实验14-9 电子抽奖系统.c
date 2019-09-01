#include <AT89X52.h>
#include <intrins.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

bit  bT0Flg = FALSE;

//U2 595���������Ŷ���
sbit sbSHCP2 = P2^0;
sbit sbDS2 = P2^1;
sbit sbSTCP2 = P2^2;
//U3 595���������Ŷ���
sbit sbSHCP3 = P2^3;
sbit sbDS3 = P2^4;
sbit sbSTCP3 = P2^5;
//U4 595���������Ŷ���
sbit sbSHCP4 = P2^6;
sbit sbDS4 = P2^7;
sbit sbSTCP4 = P3^0;
//U5 595���������Ŷ���
sbit sbSHCP5 = P3^1;
sbit sbDS5 = P3^3;
sbit sbSTCP5 = P3^4;
//U6 595���������Ŷ���
sbit sbSHCP6 = P3^5;
sbit sbDS6 = P3^6;
sbit sbSTCP6 = P3^7;

unsigned char temp2,temp3,temp4,temp5,temp6;
bdata unsigned char sw;    //λ����

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

//595���뺯��2��
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
//595�������2��
void Output5952()
{
 	sbSTCP2 = 0;
	_nop_();
	sbSTCP2 = 1;
	_nop_();
	sbSTCP2 = 0;
}
//595���뺯��3��
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
//595�������3��
void Output5953()
{
 	sbSTCP3 = 0;
	_nop_();
	sbSTCP3 = 1;
	_nop_();
	sbSTCP3 = 0;
}

//595���뺯��4��
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
//595�������4��
void Output5954()
{
 	sbSTCP4 = 0;
	_nop_();
	sbSTCP4 = 1;
	_nop_();
	sbSTCP4 = 0;
}

//595���뺯��5��
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
//595�������5��
void Output5955()
{
 	sbSTCP5 = 0;
	_nop_();
	sbSTCP5 = 1;
	_nop_();
	sbSTCP5 = 0;
}

//595���뺯��6��
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
//595�������6��
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
    counter = 0;  //ͳ�ƿ��رպϵ�����
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
    srand(counter);       //��ʼ������
}


void Timer0Init(void)                           //��ʱ��0��ʼ������
{
  TMOD = 0x01;                                  //����T1������ʽ
	TH0 = 0x00;
	TL0 = 0x0C;					                          //100ms��ʱ
  ET0 = 1;                                      //������ʱ��0�ж�
//  TR0 = 1;                                      //������ʱ��
}
void Timer0Deal(void) interrupt 1 using 1       //��ʱ��0�жϴ�����
{
	ET0 = 0;                       //���ȹر��ж�
	TH0 = 0x00;                    //Ȼ������װ��Ԥ��ֵ
	TL0 = 0x0C;				
  ET0 = 1;                       //��T0�ж�
  bT0Flg = TRUE;                  //��ʱ���жϱ�־λ
}
EX_INT0() interrupt 0 using 1         //�ⲿ�ж�0������
{
  if(TR0 == 1)                       //�жϵ�ǰ��ʱ��������״̬
  {
    TR0 = 0;
  }
  else
  {
    TR0 = 1;
    initrand();   //��ʼ������
  }
}
void main()
{
  unsigned int randdata = 0;
  unsigned char wdata,qdata,baidata,sdata,gdata;
  Timer0Init();                     //��ʼ��ʱ��
  IT0 = 1;                           //�����ⲿ�ж�0������ʽΪ������
  EX0 = 1;                           //ʹ���ⲿ�ж�0
  EA = 1;                           //�򿪴����жϱ�־
	while(1)
	{
    while(bT0Flg==FALSE);            //�ȴ���ʱ��־λ
    bT0Flg=FALSE;  

    randdata = 2 * rand();    //��������
  
    wdata = randdata/10000;    //�����λ
    temp2 = SEGtable[wdata];
	  Input5952();
		Output5952();
    
    qdata = randdata%10000/1000;  //���ǧλ
    temp3 =  SEGtable[qdata];
	  Input5953();
		Output5953();

    baidata = randdata%1000/100;  //�����λ
    temp4 =  SEGtable[baidata];
	  Input5954();
		Output5954();

    sdata = randdata%100/10;    //���10λ
    temp5 =  SEGtable[sdata];
	  Input5955();
		Output5955();

    gdata = randdata%10;    //�����λ
    temp6 =  SEGtable[gdata];
	  Input5956();
		Output5956();
	}
}