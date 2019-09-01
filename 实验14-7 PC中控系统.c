#include <AT89X52.h>
#define ON 0
#define OFF 1

sbit Relay = P2^7;      //�̵�����������
//��ʼ������
void InitUART(void)
{
    TMOD = 0x20;     //9600bps
    SCON = 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}
//����һ���ֽڵ�����
void Send(unsigned char x)
{
  SBUF = x;
  while(TI==0);
  TI = 0;
}

void Serial(void) interrupt 4 using 0
{
  unsigned char temp;
  if(RI == 1)      //��������
  {
    RI = 0;
    temp = SBUF;
    if(temp == 0x01)  //����Ǵ򿪼̵���
    {
      Relay = ON; //�򿪼̵���
      Send(0x01); //�����̵���״̬��Ϣ
    }
    else if(temp == 0x02)  //����ǹرռ̵���
    {
      Relay = OFF;
      Send(0x02); //�����̵���״̬
    }
    else
    {
      Send(0x03);//�޶���
    }
  }
}

main()
{
  InitUART();
  Relay = OFF;    //�̵����Ͽ�
	while(1)
	{
	}
}


