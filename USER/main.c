//节能型自适应风扇: 自动方式、温度、PWM、阈值
//手动方式、温度、PWM、阈值
//
//
//
#include<stdio.h>
#include<assert.h>
#include "stm32f10x.h"
#include "led.h"
#include "beep.h"
#include "adc.h"
#include "NTC1.h"
#include "delay.h"
#include "usart.h"	 
#include "pwm.h"	
#include "OLED_I2C.h"
#include "key.h"
#include "Timer.h"


//	extern u16 NTCTAB[];
//	u8 look_up_table(u16 *a,u8 ArrayLong,u16 data);
//	float num_to_temperature(u8 num);

extern u8 temper;		//开关所设置的阈值下限
extern u16 speed;		//开关所设置的手动PWM
extern u8 function;		//开关所设置的手动方式function=2
extern u8 flag_cls;
extern u8 flag_off;
extern u8 flag_sec;

int main(void)
{
	u16 adcx;
	u8 num ,flag_sf=0;
	u8 threshold=34;		//阈值的上下限
	float temp,temp1;		//临时温度数据
	u32 arr=1999;		//PWM最大值
	u16 pwm_dt = 900;		//PWM的当前值
	unsigned char buff[4];

	RCC_Configuration();
	GPIO_Configuration();
	TIM2_Configuration(arr, 0);		//pwm的init
	delay_init();	    	 //延时函数初始化
	uart_init(115200);	 	//串口初始化为115200
 	Adc_Init();		  		//ADC1初始化
	LED_Init();
	beep_Init();//蜂鸣器初始化
	I2C_Configuration();
	OLED_Init();
	key_Init();
	keyscan();
	OLED_CLS();//清屏
	Timer_Init();
	TIM3_NVIC_Configuration();
	
	void itoa(int i,unsigned char* string,u8 num);

	
	for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//‘节能型自适应风扇’ 
		}
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(200);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(400);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(200);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//蜂鸣器开,'0'有效
	delay_ms(200);
		
	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_5,10);		//ADC值
		temp=(float)adcx*(3.3/4096);							//电压值
    num=look_up_table(NTCTAB,241,adcx);				//查表得到下标
		temp=num_to_temperature(num);							//计算得到的温度值

//	printf("%f\n",temp);
		
	if(function==0)		//自动模式
		{
		if(flag_cls) {OLED_Fill(0x00);flag_cls=0;}			//第一次进入清屏
		for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//‘节能型自适应风扇’ 
		}
		for(u8 i=8;i<12;i++)  //‘自动方式’
		{
			OLED_ShowCN(31+(i-8)*16,2,i);//‘自动方式’   温度占空比阈值
		}
		OLED_ShowCN(0,4,12);		//温
		OLED_ShowCN(16,4,13);		//度
		temp1 = temp*10;		//保护小数		
		itoa(temp1,buff,1);	//加小数点标志
		OLED_ShowStr(32,4,buff,2);
		
		keyscan();			//得到阈值temper和PWM风速speed

		OLED_ShowCN(66,4,17);		//阈
		OLED_ShowCN(82,4,18);		//值
		itoa(temper,buff,0);	//不加小数点标志
		OLED_ShowStr(98,4,buff,2);

		OLED_ShowCN(0 ,6,14);		//占
		OLED_ShowCN(16,6,15);		//空
		OLED_ShowCN(32,6,16);		//比
		itoa(speed,buff,0);	//不加小数点标志
		OLED_ShowStr(48,6,buff,2);
		}

	if(function==1)		//调节阈值
		{
		keyscan();
		if(flag_cls) {OLED_Fill(0x00);flag_cls=0;}			//第一次进入清屏
		for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//‘节能型自适应风扇’ 
		}
		OLED_ShowCN(31,2,12);//‘温
		OLED_ShowCN(47,2,13);//‘度
		OLED_ShowCN(63,2,17);//‘阈
		OLED_ShowCN(79,2,18);//‘值

		OLED_ShowCN(31,4,12);		//温
		OLED_ShowCN(47,4,13);		//度
		temp1 = temp*10;		//保护小数		
		itoa(temp1,buff,1);	//加小数点标志
		OLED_ShowStr(63,4,buff,2);

		OLED_ShowCN(31,6,17);		//阈
		OLED_ShowCN(47,6,18);		//值
		itoa(temper,buff,0);	//不加小数点标志
		OLED_ShowStr(63,6,buff,2);
		}

	if(function==2)		//手动方式
		{
		keyscan();			//得到阈值temper和PWM风速speed
		if(flag_cls) {OLED_Fill(0x00);flag_cls=0;}			//第一次进入清屏
		for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//‘节能型自适应风扇’ 
		}
		OLED_ShowCN(31,2, 6);//‘手
		OLED_ShowCN(47,2, 9);//‘动
		OLED_ShowCN(63,2,10);//‘阈
		OLED_ShowCN(79,2,11);//‘值

		OLED_ShowCN(31,4,12);		//温
		OLED_ShowCN(47,4,13);		//度
		temp1 = temp*10;		//保护小数		
		itoa(temp1,buff,1);	//加小数点标志
		OLED_ShowStr(63,4,buff,2);

		OLED_ShowCN(15 ,6,14);		//占
		OLED_ShowCN(31,6,15);		//空
		OLED_ShowCN(47,6,16);		//比
		itoa(speed,buff,0);	//不加小数点标志
		OLED_ShowStr(63,6,buff,2);
		}

		if(temp < temper) {pwm_dt = 0;flag_sf=0;}	//23.5=0
		else
		if(flag_sf==0){
			TIM_SetCompare4(TIM2,1999);//TIM2_CH4
			delay_ms(1000);
			flag_sf=1;
		}
		if(temp < threshold && temp >= temper) pwm_dt=100*(temp-14);	//公式？
		if(temp >= threshold) pwm_dt = 1999;	//
		
		if(function!=2)	TIM_SetCompare4(TIM2,pwm_dt);	//TIM2_CH4,自动的PWM值
			else	TIM_SetCompare4(TIM2,speed);					//TIM2_CH4,手动的PWM值

		itoa(pwm_dt,buff,0);	//不加小数点标志
		OLED_ShowStr(96,6,buff,2);
		
//		if(flag_off) {OLED_OFF();flag_off=0;}
		if(flag_sec){
			printf("%c\n",0xAA);
			adcx=(int)temp1;
			printf("%c\n",adcx/256);
			printf("%c\n",adcx%256);
			printf("%c\n",temper);
			printf("%c\n",pwm_dt/256);
			printf("%c\n",pwm_dt%256);
			printf("%c\n",speed/256);
			printf("%c\n",speed%256);
			printf("%c\n",(int)temp1+temper+pwm_dt+speed);
			printf("%c\n",0xFF);
			flag_sec=0;
		}
		delay_ms(40);
	}
}
/////////////////////////////////////////
void itoa(int i,unsigned char* string,u8 num)		//char* string 的类型是 unsigned，要前后一致
{
    int power, j;
    j=i;
    for(power=1;j>=10;j/=10)   power*=10;		//计算位数
    for(;power>0;power/=10)
    {
      if(power == 1 && num ) *string++='.' ;
			*string++='0'+i/power;		//用于保存各个位，百位时power=100
      i%=power;
    }
    *string='\0';
}
/////////////////////////////////////////
//		OLED_ShowStr(0,7, buff,1);
//		printf("测得的温度为%s\n\r\n",CH);	//%%%
//			USART_SendData(USART1,temper);	//%%%
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}		//%%%
//			USART_SendData(USART1,speed);	//%%%
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}		//%%%
//			USART_SendData(USART1,function);	//%%%
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}		//%%%
			

