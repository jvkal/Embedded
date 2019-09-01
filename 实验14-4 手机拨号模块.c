#include<AT89X52.h>
#include <intrins.h>

#define Delaynop(){_nop_();_nop_();_nop_();_nop_();}


sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;

void Delayms(unsigned int ms);
bit LCD_Busy();
void LCD_Pos(unsigned char);
void LCD_Wcmd(unsigned char);
void LCD_Wdat(unsigned char);

char code Title_Text[]={"--Phone Code--  "};

unsigned char code Key_Table[]={'1','2','3','4','5','6','7','8','9','*','0','#'};
unsigned char Dial_Code_Str[]={"             "};
unsigned char KeyNo=0xff;
int tCount=0;
//���뼶��ʱ����
void Delayms(unsigned int x)
{
  unsigned char i;
  while(x--)
  { 
    for(i=0;i<120;i++);
  }
}
//��ʾ�ַ���
void Display_String(unsigned char *str,unsigned char LineNo)
{
  unsigned char k;
  LCD_Pos(LineNo);
  for(k=0;k<16;k++)
  {
    LCD_Wdat(str[k]);
  }
}
//���1602�Ƿ���æװ��
bit LCD_Busy_Check()
{
  bit Result;
  RS=0;
  RW=1;
  EN=1;
  Delaynop();
  Result=(bit)(P0 & 0x80);
  EN=0;
  return Result;
}
//��1602д��ָ��ĺ���
void LCD_Write_Command(unsigned char cmd)	
{
  while(LCD_Busy_Check());  //����Ƿ���æ״̬
  RS=0;                 
  RW=0;
  EN=0; 
  _nop_();
  _nop_();
  P0=cmd;   //д��ָ��
  Delaynop();
  EN=1;
  Delaynop();
  EN=0;
 }
 //��1602д����
void LCD_Wdat(unsigned char dat)
{
 while(LCD_Busy_Check());  //����Ƿ���æ״̬
 RS=1;
 RW=0;
 EN=0;
 P0=dat;                   //д������
 Delaynop();
 EN=1;
 Delaynop();
 EN=0;
}
//��ʼ��1602
void Init_LCD()
{
  LCD_Write_Command(0x38);Delayms(5);
  LCD_Write_Command(0x01);Delayms(5);
  LCD_Write_Command(0x06);Delayms(5);
  LCD_Write_Command(0x0c);Delayms(5);
}
//������ʾλ��
void LCD_Pos(unsigned char pos)
{
  LCD_Write_Command(pos|0x80);
}
/*
//T0���жϷ����Ӻ���
void T0Deal()interrupt 1
{
 TH0=-600/256;
 TL0=-600%256;
 if(++tCount==200)
 {
  tCount=0;
  TR0=0;
 }
}
*/
//����ɨ�躯��������ֵΪ��Ӧ�İ�������
unsigned char GetKey()
{
 unsigned char i,j,k=0;
 unsigned char KeyScanCode[]={0xef,0xdf,0xbf,0x7f};   //����ɨ���������
 unsigned char KeyCodeTable[]={
 0xee,0xed,0xeb,0xde,0xdd,0xdb,0xbe,0xbd,0xbb,0x7e,0x7d,0x7b};
 P3=0x0f;
 if(P3!=0x0f)       //����а���������
 {
  for(i=0;i<4;i++)    //���ν���ɨ��
  {
    P3=KeyScanCode[i];
    for(j=0;j<3;j++)
    {
      k=i*3+j;                //�����Ӧ�İ�������
      if(P3==KeyCodeTable[k])
      {
        return k;             //���ذ�������
      }
     }
  }
 }
 else 
 {
  return 0xff;                  //���߷���0xff
 }
}
//������
void main()
{
 unsigned char i=0,j;
 P0 = 0xFF;
 P2 = 0xFF;
 P1 = 0xFF;                   //��ʼ���˿�
// IE=0x82;                     //�����ж�
// TMOD=0x01;                   //����T0��ʱ��
 Init_LCD();                  //��ʼ��1602
 Display_String(Title_Text,0x00); //��ʾ --Phone Code--
 while(1)
 {
  KeyNo = GetKey();           //��ð���״̬
  if(KeyNo==0xff)
  {
    continue;                  //���û�а������������һ��ѭ��
  }
  if(++i==12)                 //����Ѿ�����11�����֣������ʾ��Ļ
  {
    for(j=0;j<16;j++)
    Dial_Code_Str[j]=' ';
    i=0;
  }
  Dial_Code_Str[i]=Key_Table[KeyNo];  //��ʾ��������
  Display_String(Dial_Code_Str,0x40);
//  TR0=1;
  while(GetKey()!=0xff);
 }
}
