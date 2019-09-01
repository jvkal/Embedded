#include <AT89X52.h>
#define ON 0
#define OFF 1

sbit Relay = P2^7;      //继电器控制引脚
//初始化串口
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
//发送一个字节的数据
void Send(unsigned char x)
{
  SBUF = x;
  while(TI==0);
  TI = 0;
}

void Serial(void) interrupt 4 using 0
{
  unsigned char temp;
  if(RI == 1)      //接收数据
  {
    RI = 0;
    temp = SBUF;
    if(temp == 0x01)  //如果是打开继电器
    {
      Relay = ON; //打开继电器
      Send(0x01); //反馈继电器状态信息
    }
    else if(temp == 0x02)  //如果是关闭继电器
    {
      Relay = OFF;
      Send(0x02); //反馈继电器状态
    }
    else
    {
      Send(0x03);//无动作
    }
  }
}

main()
{
  InitUART();
  Relay = OFF;    //继电器断开
	while(1)
	{
	}
}


