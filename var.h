#ifndef _VAR_H
#define _VAR_H

#include <STC/STC15F2K60S2.H>
#include <intrins.h>


typedef unsigned char u8;

//main
extern u8 digital_tube[8];
extern long SysTick;
extern long last;
#define OFF 10
#define SHOW_U 11
#define SHOW_P 12
#define SHOW_N 13

extern u8 vp;
extern long vain3;
extern long last_vain3;
extern u8 count;
extern u8 invalid_operation_count;

void save_vp();
void get_vp();
void LatchControl(num, value);

//button
#define S12 8
#define S13 9
#define S16 12
#define S17 13
extern u8 show_num;
void check_button();
void respond_to_button();

//show
void change_show();
enum SHOW{
	DATA = 1, PARA, COUNT
};
extern enum SHOW show_state;

#endif