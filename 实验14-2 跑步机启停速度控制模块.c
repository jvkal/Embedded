#include <AT89X52.h>			

unsigned char code SEGtable[ ]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};     //�ַ�����

sbit SEKey =  P1 ^ 0;			//��������ͣ��ֹͣ����
sbit INCKey = P1 ^ 4;     //�ٶ����Ӽ�
sbit DECKey = P1 ^ 7;     //�ٶȼ��ټ�

sbit pauseLED = P3 ^ 0;		//��ָͣʾ��
sbit stopLED = P3 ^ 7;			    //ָֹͣʾ��

bit keyFlg;					//�����������̰���־λ 0Ϊ�̰� 1Ϊ����

unsigned char TimeCounter;					//����ר��
unsigned char KeyDownFlg,set;			//����ר��

unsigned char yansi,key,send,unitsdData,tensdData,num,keynum;
bit	keybz;

//��ʱ����
void delay(unsigned char time)
{
	unsigned char x,y;
	for(x=time;x>0;x--)
  {
			for(y=110;y>0;y--);
  }
}
//ɨ��������ֹͣ����ͣ����
void StartAndStopKeyScan()
{	
  if(SEKey == 0)					//��������
	{
	  KeyDownFlg = 1;					//�ü����±�־λ
	  TR0 = 1;							//������ʱ��
	}
	
	if((SEKey == 1) && (KeyDownFlg == 1))		//�ж����ü��Ƿ��ɿ�
	{
    KeyDownFlg = 0;						//�������־λ
		if(keyFlg == 0)				//����Ƕ̰�
		{
		  stopLED = 1;
		  pauseLED =~pauseLED;
	  }
	  TR0 = 0;							//�رն�ʱ��
  	TimeCounter = 0;						  	//����������
	  keyFlg = 0;						//����̰���������־λ
	}	
}

void SegDisplay(unsigned char tensdData,unsigned char unitsdData) //��ʱ����
{
  P0 = SEGtable[tensdData];
  	delay(10);	
	P2 = SEGtable[unitsdData];	
  	delay(10);
}

void keyscan1()			//�������ӹ��ܵĵİ���ɨ�����
{
	if(INCKey==0)							//�ж�INCKey���Ƿ���?
		{
			SegDisplay(tensdData,unitsdData);					//��ʱȥ����
			if(INCKey==0)					//�������¾�ȥִ�м��̳���
			{
				if(keybz==0)				//�ж��Ƿ��ǵ�һ�ΰ���?
						{
							num++;		//ֵ��һ
							if(num==100)	//�ж��Ƿ�ӵ�100	
								{num=0;}//�ӵ�100��0
							keybz=1;	//��һ�ν����ñ�־λ
							key--;		//��������������
							keynum=5;	//��Ӱ�������������
							tensdData=num/10;	//BCD��תΪʮ����ֵ
							unitsdData=num%10;
							return;		//���ü������ֱ�ӷ���
									
						}
					else				//����ǵڶ��ΰ�����ִ����������
					{
						if(key==0)		//�жϰ���������100������?		
							{
							if(keynum==0)	//��ⰴ��ʱ���Ƿ񳬹���5�ε�ʱ��
								{
									key=10;		//�����������ʱ�䳬����5�ε�ʱ�����Ժ�ÿ10��ִ�мӹ�������
									num++;
									if(num==99)
										{
											num=0;
										}
									tensdData=num/10;
									unitsdData=num%10;
									return;
								}
							  else		//û�е�5��ʱ����ִ����������
								  keynum--;	//��Ӽ�������һ
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
						else			//û�е�100���´������ж�.
							key--;
							return;
					
					}

										
			}

			if(INCKey!=0)							//���ֺ����еļ������岢��Ĭ��ֵ
				{
					keynum=5;
				        key=30;
					keybz=0;
					return;						//����
				}		    
		}
/*************************�����Ǽ�һ���������,ִ��Ч��ͬ��һ����*******************************************************************/	
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
	if(TimeCounter==250)			//��ʱʱ�䵽
	{
		keyFlg = 1;				      //�ó�����־λ
		pauseLED = 1;				
		stopLED = ~stopLED;			//ȡ��
		TR0=0;					        //�رն�ʱ��
	}
  TH0 = 0xd8;
  TL0 = 0xf0;					//��ʱ�����¸���ֵ
}
//������
void main(void)
{
	EA = 1;
	TMOD = 0x01;
  TH0 = 0xd8;  			//10ms
  TL0 = 0xf0;
	ET0 = 1;				//���ö�ʱ��1

	unitsdData=0;
	tensdData=0;
	P1=0xff;
	P2=0;
	key=100;
  SegDisplay(0,9);
	while(1)
	{
	  StartAndStopKeyScan();				//���ð���ɨ���ӳ���
    keyscan1();
  	SegDisplay(tensdData,unitsdData);
	}
}


