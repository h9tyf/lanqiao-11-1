#include "var.h"
u8 button_flag[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 button_state[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
u8 button_count[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 button_check[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void check_button()
{
	u8 i;
	u8 loc;
	P3 = 0x0f;
	if(P3 != 0x0f){
		if((P3 & 0x08) == 0)loc = 0;
		if((P3 & 0x04) == 0)loc = 1;
		if((P3 & 0x02) == 0)loc = 2;
		if((P3 & 0x01) == 0)loc = 3;
		
		P3 = 0xf0;
		if((P3 & 0x80) == 0)loc += 0;
		if((P3 & 0x40) == 0)loc += 4;
		if((P3 & 0x20) == 0)loc += 8;
		if((P3 & 0x10) == 0)loc += 12;
		
		for(i = 0; i < 16; i++){
			if(i == loc){
				button_check[i] = 0;
			} else {
				button_check[i] = 1;
			}
		}
	} else {
		for(i = 0; i < 16; i++){
			button_check[i] = 1;
		}
	}
	
	for(i = 0; i < 16; i++){
		if(button_check[i] == button_state[i]){
			button_count[i] = 0;
		} else {
			button_count[i] += 1;
		}
	}
	
	for(i = 0; i < 16; i++){
		if(button_check[i] != button_state[i] && button_count[i] >= 5){
			button_state[i] = 1 - button_state[i];
			//button_count[i] = 0;
			if(button_state[i] == 0){
				button_flag[i] = 1;
			}
		}
	}
}

void respond_to_button()
{
	u8 i;
	for(i = 0; i < 16; i++){
		if(button_flag[i] == 1){
			button_flag[i] = 0;
			if(i == S12){
				invalid_operation_count = 0;
				if(show_state == PARA){
					save_vp();
				}
				show_state = show_state%3 + 1;
			} else if(i == S13){
				if(show_state == COUNT){
					invalid_operation_count = 0;
					count = 0;
				} else {
					invalid_operation_count += 1;
				}
			} else if(i == S16){
				if(show_state == PARA){
					invalid_operation_count = 0;
					vp = (vp + 5) % 55;
					save_vp();
				} else {
					invalid_operation_count += 1;
				}
			} else if(i == S17){
				if(show_state == PARA){
					invalid_operation_count = 0;
					vp = (vp  + 55 - 5) % 55;
					save_vp();
				} else {
					invalid_operation_count += 1;
				}
			} else {
				invalid_operation_count += 1;
			}
		}
	}
}

