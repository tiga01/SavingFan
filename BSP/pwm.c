//filename: pwm.c
#include "pwm.h"

void RCC_Configuration(void)
{
    SystemInit();

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//proteus仿真，需要开启TIM1		//###
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//TIM2_CH4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//端口复用
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_3;//TIM2_CH4,pa3
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void TIM2_Configuration(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStructure;

 //   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//###//

    TIM_TimeBaseStruct.TIM_Period=arr;
    TIM_TimeBaseStruct.TIM_Prescaler=psc;
    TIM_TimeBaseStruct.TIM_ClockDivision=0;
    TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruct);//TIM2_CH4

    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;

    TIM_OC4Init(TIM2,&TIM_OCInitStructure);//TIM2_CH4
    TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);//TIM2_CH4
    TIM_Cmd(TIM2,ENABLE);//TIM2_CH4
}
