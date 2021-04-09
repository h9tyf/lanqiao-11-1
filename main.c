#include "var.h"
#include "iic.h"
long SysTick = 0;
long last = 0;

u8 digital_tube[8] = {OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF};
u8 tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xc1, 0x8c, 0xc8};
							//0			1			2			3			4			5			6			7			8			9		OFF		U		P			N

u8 vp;
long vain3;
u8 count = 0;
long last_vain3;
u8 invalid_operation_count;


void save_vp(){
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(0x00);
	IIC_WaitAck();
	IIC_SendByte(vp);
	IIC_WaitAck();
	IIC_Stop();
}

void get_vp(){
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(0x00);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	vp = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
}

void get_vain3()
{
	long temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	vain3 = temp ;
}

void LatchControl(num, value)
{
	P0 = value;
	_nop_();
	P2 = num << 5;
	_nop_();
	P2 = 0x0f;
	_nop_();
}

u8 index;
void func() interrupt 1
{
	u8 value;
	SysTick++;
	LatchControl(7, tab[OFF]);
	LatchControl(6, 1<<index);
	if(digital_tube[index] <= SHOW_N){
		value = tab[digital_tube[index]];
	} else {
		value = 0xfe;
	}
	if((show_state == DATA || show_state == PARA) && index == 5){
		value &= 0x7f;
	}
	LatchControl(7, value);
	index = (index + 1) % 8;
}

void Timer0Init(void)		//1??@12.000MHz
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TL0 = 0x20;		//??????
	TH0 = 0xD1;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
}


void change_state()
{
	check_button();
	respond_to_button();
}

void main()
{
	Timer0Init();
	EA = 1;
	ET0 = 1;
	LatchControl(4, 0xff);
	EA = 0;
	get_vp();
	EA = 1;
	while(1){
		long tickBkp = SysTick;
		
		if(tickBkp % 500 == 0){
			//cai yang
			last_vain3 = vain3;
			EA = 0;
			get_vain3();
			EA = 1;
			if(last_vain3 >= vp * 10 && vain3 < vp * 10){
				last = SysTick;
				count++;
			}
			if(last_vain3 < vp * 10 && vain3 >= vp * 10){
				last = SysTick;
			}
		}
		
		change_state();
		if(tickBkp % 10 == 0){
			EA = 0;
			change_show();
			EA = 1;
		}

		
		
		while(tickBkp == SysTick);
	}
}