#include "led.h"
//LED PB0/PB1/PA7('0'��Ч)��ʼ������
//FAN PA3('1'��Ч)��ʼ������
void LED_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		// | GPIO_Pin_3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_7);//��1��B_LED��
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);//��1��R_G_LED��
//	GPIO_ResetBits(GPIOA,GPIO_Pin_3);//��0��FAN��

}
