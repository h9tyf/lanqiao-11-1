#include "var.h"
enum SHOW show_state = DATA;

long low_time;

void change_led()
{
	u8 temp = 0xff;
	if(SysTick - last >= 5000 && vain3 < vp * 10){
		temp &= 0xfe;
	}
	if(count % 2 != 0){
		temp &= 0xfd;
	}
	if(invalid_operation_count >= 3){
		temp &= 0xfb;
	}
	LatchControl(4, temp);

}

void change_show()
{
	u8 i;
	u8 temp;
	long temp1;
	if(show_state == DATA){
		digital_tube[0] = SHOW_U;
		for(i = 1; i < 5; i++){
			digital_tube[i] = OFF;
		}
		
		temp1 = vain3;
		for(i = 7; i >= 5;i--){
			digital_tube[i] = temp1 % 10;
			temp1 /= 10;
		}
	} else if(show_state == PARA){
		digital_tube[0] = SHOW_P;
		for(i = 1; i < 5; i++){
			digital_tube[i] = OFF;
		}
		digital_tube[5] = vp / 10;
		digital_tube[6] = vp % 10;
		digital_tube[7] = 0;
	} else { //COUNT
		digital_tube[0] = SHOW_N;
		if(count == 0){
			for(i = 1; i <= 6; i++){
				digital_tube[i] = OFF;
			}
			digital_tube[7] = 0;
		} else {
			temp = count;
			for(i = 7; i >= 1; i--){
				if(temp == 0){
					digital_tube[i] = OFF;
				} else {
					digital_tube[i] = temp % 10;
					temp /= 10;
				}
			}
		}
	}
	
	change_led();
}