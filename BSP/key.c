/*******************************************************
*          键盘扫描函数						        *
********************************************************/
#include "key.h"
#include "delay.h"
#include "beep.h"
#include "sys.h"
#include "usart.h"
#include "OLED_I2C.h"

u8 temper=23;				//全局变量
s16 speed=1100;			//全局变量
u8 function=0;			//全局变量
u8 temp;
u8 flag_cls=0;
u16 func_counter=0;
u8 flag_func=0;
	
void keyscan()	//调节温度，20-30度
{
  if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4)==0)	   //是否KEY3设置
	{
		 delay_ms(40);
	   if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4) ==0)		//再一次判断按键是否松开
	   {
		  beep(1);		//蜂鸣器一声
		  temp++;			//功能编号
			func_counter=0;
			OLED_ON();
		  while(!GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4))	//耗时等待KEY3
		  {
//		  	send(); //数据处理
//	 			display(b5,b4,b3,b2,b1,b0);	//显示
		  } //按键是否松开
		  if(temp==1)	 //调节温度阈值
		  {
			 	function=1;flag_cls=1;
				flag_func=1;	//设置阈值时，自动返回
		  }
		  if(temp==2)	 //手动调节速度
		  {
			 	function=2;flag_cls=1;
		  }
		  if(temp==3)	 //恢复自动状态，退出设置
		  {
		  	function=0;
				flag_cls=1;
			 	temp=0;
		  }
	   }
	}
	if(temp!=0)		//设置时
	{
	  if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_1) ==0)		//KEY2加操作
	  {
		 delay_ms(40);	 //消抖
		 if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_1) ==0)	  //再一次判断按键是否松开
		 {
		  beep(1);		//蜂鸣器一声
			func_counter=0;
			while(!GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_1))
			{	}  //按键是否松开
			if(temp==1)	   // 阈值加
			{
			  temper++;
			  if(temper>30)
			   {
		  		temper=30;
		  		beep(1);		//蜂鸣器2声
			   }
			}
			if(temp==2)	    // 速度加
			{
			  speed=speed+100;
			  if(speed > 1999)	//PWM最大值
			  {
		  		speed = 1999;
		  		beep(1);		//蜂鸣器2声
			  }
			}
		}
  }
	  if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) ==0)		//KEY1减操作
	  {
		delay_ms(40);	 //消抖
		if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) ==0)	 //KEY1
		{
		   beep(1);		//蜂鸣器1声
			 func_counter=0;
		   while(!GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0))
		   {		   }	  //按键是否松开
		   if(temp==1)		// 阈值减
		   {
			  temper--;
			  if(temper<20)
			  {
			     temper=20;
		   		 beep(1);		//蜂鸣器2声
			  }
		   }
		   if(temp==2)	 // 速度减
		   {
			   speed=speed-100;
			   if( speed <= 600 )
			   {
		  		speed=800;
		  		beep(1);		//蜂鸣器2声
			   }
		   }
			}
	  }
  }
}

void key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//NO1

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4;		//KEY 0/1/4
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

}

///////////////////////////////////////////
