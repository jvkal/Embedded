#include <AT89X52.h>

#define MAX 0x50   //��ʱ���޶���
#define MIN 0x00   //��ʱ���޶���
#define TIMELINE 11         //ʱ���Ƶ����
#define TRUE  1
#define FALSE 0             //��־λ����

unsigned int TimeCounter;
bit ArrowFlg = 0;         //�����־λ
unsigned char upCounter,downCounter;    //���Ӽ������ͼ��ټ�����

sbit LED=P2^0;

//T0���жϷ����Ӻ���
void T0Deal() interrupt 1 using 0
{
	TH0=0xf1;
	TL0=0xf1;
	TR0=1;
	TimeCounter++;  //��ʱ����������
	if(TimeCounter == TIMELINE)
	{
		if((upCounter == MAX)&&(downCounter == MIN))  //���������־λ�л�
    {
      ArrowFlg = FALSE;
    }
		if((upCounter == MIN)&&(downCounter == MAX))
    {
      ArrowFlg= TRUE;
    }
		if(ArrowFlg == 1)   //��������Ӽ���
    {
      upCounter++;
      downCounter--;
    }
		else               //����Ǽ��ټ���
    {
      upCounter--;
      downCounter++;
    }
		TimeCounter=0;
	}
}
//��ʱ����
void Delay(unsigned int i)
{
	unsigned int j;
	while(i--)
	{
		for(j=0;j<32;j++);     //��ʱ
	}
}

void main()
{
	upCounter = MIN;
	downCounter = MAX;     //��������ʼ��
	TMOD = 0x01;          //���ö�ʱ��������ʽ
	TH0 = 0xF0;
	TL0 = 0xF0 ;          //T0��ʼ��ֵ
	EA = 1;
	ET0 = 1;              //���ж�
	TR0 = 1;              //����T0
	while(1)
	{
		LED=0;            //����仯��PWM����
		Delay(downCounter);   
		LED=1;
		Delay(upCounter);
	}
}

