#include "beep.h"
#include "delay.h"

//蜂鸣器PC13('0'有效)初始化函数

void beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//NO1

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;							//NO2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOC,GPIO_Pin_13);//置1，蜂鸣器关				//NO3
}

void beep(u8 beep_times)
{
	for(u8 i=0;i < beep_times;i++)
	{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//置1，蜂鸣器关				//ON
	delay_ms(60);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//置1，蜂鸣器关					//OFF
	delay_ms(60);
	}
}
