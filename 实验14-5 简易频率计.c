#include <AT89X52.H> 
unsigned char code dispbit[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};     //P2��ɨ��λ
unsigned char code dispcode[]={0x3f,0x06,0x5b,0x4f,0x66, 
                               0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x40};    //����ܵ����α���
unsigned char dispbuf[8]={0,0,0,0,0,0,10,10}; //��ʼ����ʾֵ
unsigned char temp[8];     //�����ʾ������
unsigned char dispcount;   //��ʾ������ֵ
unsigned char T0count;     //T0�ļ�����ֵ
unsigned char timecount;   //��ʱ������ֵ
bit flag;                  //��־λ
unsigned long x;           //Ƶ��ֵ
//Ƶ�ʼ��㺯��
void HzCal(void)
{
  unsigned char i; 
  x=T0count*65536+TH0*256+TL0; //�õ�T0��16λ������ֵ
  for(i=0;i<8;i++) 
  { 
    temp[i]=0; 
  } 
         i=0; 
         while(x/10)         //���
           { 
             temp[i]=x%10; 
             x=x/10; 
             i++; 
           } 
         temp[i]=x; 
         for(i=0;i<6;i++)    //����Ϊ��ʾ����
           { 
             dispbuf[i]=temp[i]; 
           } 
         timecount=0; 
         T0count=0; 
}
  
void main(void) 
{
  
  TMOD=0x15;              //���ö�ʱ��������ʽ
  TH0=0; 
  TL0=0; 
  TH1=(65536-5000)/256; 
  TL1=(65536-5000)%256;       //��ʼ��T1
  TR1=1; 
  TR0=1; 
  ET0=1; 
  ET1=1; 
  EA=1;                     //���ж�
  
  while(1) 
   { 
     if(flag==1) 
       { 
         flag=0; 
         HzCal();   //Ƶ�ʼ��㺯��
         TH0=0; 
         TL0=0; 
         TR0=1; 
       } 
   } 
} 
//��ʱ��T0�жϷ����Ӻ���
void t0(void) interrupt 1 using 0 
{ 
  T0count++; 
} 
//��ʱ��T1�жϷ����Ӻ���  
void t1(void) interrupt 3 using 0 
{ 
  TH1=(65536-5000)/256; 
  TL1=(65536-5000)%256;          //��ʼ��T1Ԥװֵ��1ms��ʱ
  timecount++;                   //ɨ��
  if(timecount==200)             //�붨ʱ
    { 
      TR0=0;                     //����T0
      timecount=0; 
      flag=1; 
    } 
  P2=0xff;                         //��ʼ��ѡ������
  P0=dispcode[dispbuf[dispcount]]; //�������ʾ����
  P2=dispbit[dispcount]; 
  dispcount++;                    //�л�����һ��ѡ������
  if(dispcount==8)                //����Ѿ�ɨ������л�
    { 
      dispcount=0; 
    } 
} 

