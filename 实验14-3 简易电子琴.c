#include<AT89X52.h>

#define KeyPort P1


unsigned char High,Low; //��ʱ��Ԥװֵ�ĸ�8λ�͵�8λ

sbit SPK=P3^7;          //����������ӿ�
sbit playSongKey=P0^0;          //���ܼ�

sbit RS=P2^0;//Һ����������ѡ��
sbit RW=P2^1;//Һ����дѡ��
sbit EN=P2^2;//Һ��ʹ�ܶ�

unsigned char code freq[][2]={ 
  0xD8,0xF7,//00440HZ 1   
  0xBD,0xF8,//00494HZ 2  
  0x87,0xF9,//00554HZ 3  
  0xE4,0xF9,//00587HZ 4    
  0x90,0xFA,//00659HZ 5     
  0x29,0xFB,//00740HZ 6     
  0xB1,0xFB,//00831HZ 7      
  0xEF,0xFB,//00880HZ `1
};

unsigned char Time; 
unsigned char code YINFU[9][1]={{' '},{'1'},{'2'},{'3'},{'4'},{'5'},{'6'},{'7'},{'8'}};
                          //����ֻ����������ݱ�
unsigned char code MUSIC[]={ 6,2,3,      5,2,1,      3,2,2,    5,2,2,    1,3,2,    6,2,1,    5,2,1,
                     6,2,4,      3,2,2,      5,2,1,    6,2,1, 	  5,2,2, 	3,2,2, 	  1,2,1,
                     6,1,1,      5,2,1,      3,2,1, 	2,2,4, 	  2,2,3, 	3,2,1,    5,2,2,
                     5,2,1,      6,2,1,      3,2,2, 	2,2,2,    1,2,4, 	5,2,3, 	  3,2,1,
                     2,2,1,      1,2,1,      6,1,1, 	1,2,1, 	  5,1,6, 	0,0,0 
                                      };
                         // ����Ƶ�ʱ� �߰�λ
unsigned char code FREQH[]={
                         0xF2,0xF3,0xF5,0xF5,0xF6,0xF7,0xF8, 
                         0xF9,0xF9,0xFA,0xFA,0xFB,0xFB,0xFC,0xFC, //1,2,3,4,5,6,7,8,i
                         0xFC,0xFD,0xFD,0xFD,0xFD,0xFE,
                         0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,
                               } ;
                         // ����Ƶ�ʱ� �Ͱ�λ
unsigned char code FREQL[]={
                         0x42,0xC1,0x17,0xB6,0xD0,0xD1,0xB6,
                         0x21,0xE1,0x8C,0xD8,0x68,0xE9,0x5B,0x8F, //1,2,3,4,5,6,7,8,i
                         0xEE,0x44, 0x6B,0xB4,0xF4,0x2D, 
                         0x47,0x77,0xA2,0xB6,0xDA,0xFA,0x16,
                                };

void Init_Timer0(void);//��ʱ����ʼ��

//��ʱ������ԼԼ2*z+5us
void delay2xus(unsigned char z)
{
   while(z--);
}
// ��ʱ������ԼԼ1ms
void delayms(unsigned char x)
{
   while(x--)
   {
     delay2xus(245);
     delay2xus(245);
   }
}


/*------------------------------------------------
                ������ʱ����
 ����1/4����ʱ�䣺
 ��4/4  125ms
 ��2/4  250ms
 ��3/4  187ms
------------------------------------------------*/
void delayTips(unsigned char t)
{
    unsigned char i;
	  for(i=0;i<t;i++)
    {
	    delayms(250);
    }
    TR0=0;
 }
//�������ֵĺ���
void PlaySong()
{
   TH0=High;//��ֵ��ʱ��ʱ�䣬����Ƶ��
   TL0=Low;
   TR0=1;      //�򿪶�ʱ��
   delayTips(Time); //��ʱ����Ҫ�Ľ���                      
}

//��ʱ��T0��ʼ���ӳ���
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
 EA=1;            //���жϴ�
 ET0=1;           //��ʱ���жϴ�
}
//��ʱ��T0�ж��ӳ���
void Timer0_isr(void) interrupt 1 
{
 TH0=High;
 TL0=Low;
 SPK=!SPK;
}
//������
void main (void)
{
  unsigned char num,k,i;                 
  Init_Timer0();    //��ʼ����ʱ��0����Ҫ��������ܶ�̬ɨ��
  SPK=0;			//��δ����ʱ�����ȵ͵�ƽ����ֹ���ڸߵ�ƽ������         
  while (1)
  {
   switch(KeyPort)          //�԰������д���
		{
		 case 0xfe:num= 1;break;
		 case 0xfd:num= 2;break;
		 case 0xfb:num= 3;break;
		 case 0xf7:num= 4;break;
		 case 0xef:num= 5;break;
		 case 0xdf:num= 6;break;
		 case 0xbf:num= 7;break;
		 case 0x7f:num= 8;break;      //�ֱ��Ӧ���õ�����
		 default:num= 0;break;
		}
    P2 = KeyPort;
    if(num==0)
    {
       TR0=0;
       SPK=0;   //��δ����ʱ�����ȵ͵�ƽ����ֹ���ڸߵ�ƽ������
	  }
    else 
    {
	    High=freq[num-1][1];
   		Low =freq[num-1][0];
		  TR0=1;
	  }
   if(playSongKey==0)        //����������ְ���������
   {
     delayms(10);
     if(playSongKey==0)
     {
       i=0;  
       while(i<100)
	    {         
            k=MUSIC[i]+7*MUSIC[i+1]-1;//ȥ������Ƶ����������
            High=FREQH[k];
            Low=FREQL[k];
            Time=MUSIC[i+2];          //����ʱ��
            i=i+3;
			if(P1!=0xff)//��������8�����˳�����
			{
               delayms(10);
               if(P1!=0xff)
			         i=101;
			}
            PlaySong();  
	    }
		   TR0=0;
     }
   }

  }
}




