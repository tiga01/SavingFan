#include "Timer.h"
#include "OLED_I2C.h"

extern u16 func_counter;
extern u8 flag_func;
extern u8 function;
u8 Oled_off_counter=0;
u8 flag_off=0,flag_sec;

void TIM3_NVIC_Configuration(void)			//需要设置2个：优先级的分组；中断控制的4个参数
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//4x4=16，

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	//TIM3_IRQHandler
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);		//中断控制的4个参数生效
}

void Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//设置TIM的周期；确定计数方向

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		//

    TIM_TimeBaseStructure.TIM_Period = (36000-1);	//重装值ARR=36000-1
    TIM_TimeBaseStructure.TIM_Prescaler = (2000-1);	//预分频值PSC=1999
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//72MHz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);		//4个参数生效

    TIM_ClearFlag(TIM3,TIM_FLAG_Update);	//更新(溢出)标志 = ‘0’
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);		//设置中断源的触发条件 = TIM_IT_Update =  ‘1’
    TIM_Cmd(TIM3,ENABLE);		//
}

void TIM3_IRQHandler(void)	//每1秒执行一次		//执行条件(触发条件) = TIM_IT_Update =  ‘1’
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)		//
    {		
      GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7))));
      TIM_ClearITPendingBit(TIM3,TIM_IT_Update);		//
			Oled_off_counter++;
			flag_sec=1;
			if(Oled_off_counter >= 15) { flag_off=1; Oled_off_counter=0; }
			//功能1时自动返回
			if(flag_func)
			{
				func_counter++;
				if(func_counter >= 10)
				{
					func_counter=0;
					function=0;
					flag_func=0;
				}
			}
			
    }
}

