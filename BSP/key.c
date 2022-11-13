/*******************************************************
*          ����ɨ�躯��						        *
********************************************************/
#include "key.h"
#include "delay.h"
#include "beep.h"
#include "sys.h"
#include "usart.h"
#include "OLED_I2C.h"

u8 temper=23;				//ȫ�ֱ���
s16 speed=1100;			//ȫ�ֱ���
u8 function=0;			//ȫ�ֱ���
u8 temp;
u8 flag_cls=0;
u16 func_counter=0;
u8 flag_func=0;
	
void keyscan()	//�����¶ȣ�20-30��
{
  if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4)==0)	   //�Ƿ�KEY3����
	{
		 delay_ms(40);
	   if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4) ==0)		//��һ���жϰ����Ƿ��ɿ�
	   {
		  beep(1);		//������һ��
		  temp++;			//���ܱ��
			func_counter=0;
			OLED_ON();
		  while(!GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_4))	//��ʱ�ȴ�KEY3
		  {
//		  	send(); //���ݴ���
//	 			display(b5,b4,b3,b2,b1,b0);	//��ʾ
		  } //�����Ƿ��ɿ�
		  if(temp==1)	 //�����¶���ֵ
		  {
			 	function=1;flag_cls=1;
				flag_func=1;	//������ֵʱ���Զ�����
		  }
		  if(temp==2)	 //�ֶ������ٶ�
		  {
			 	function=2;flag_cls=1;
		  }
		  if(temp==3)	 //�ָ��Զ�״̬���˳�����
		  {
		  	function=0;
				flag_cls=1;
			 	temp=0;
		  }
	   }
	}
	if(temp!=0)		//����ʱ
	{
	  if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_1) ==0)		//KEY2�Ӳ���
	  {
		 delay_ms(40);	 //����
		 if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_1) ==0)	  //��һ���жϰ����Ƿ��ɿ�
		 {
		  beep(1);		//������һ��
			func_counter=0;
			while(!GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_1))
			{	}  //�����Ƿ��ɿ�
			if(temp==1)	   // ��ֵ��
			{
			  temper++;
			  if(temper>30)
			   {
		  		temper=30;
		  		beep(1);		//������2��
			   }
			}
			if(temp==2)	    // �ٶȼ�
			{
			  speed=speed+100;
			  if(speed > 1999)	//PWM���ֵ
			  {
		  		speed = 1999;
		  		beep(1);		//������2��
			  }
			}
		}
  }
	  if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) ==0)		//KEY1������
	  {
		delay_ms(40);	 //����
		if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) ==0)	 //KEY1
		{
		   beep(1);		//������1��
			 func_counter=0;
		   while(!GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0))
		   {		   }	  //�����Ƿ��ɿ�
		   if(temp==1)		// ��ֵ��
		   {
			  temper--;
			  if(temper<20)
			  {
			     temper=20;
		   		 beep(1);		//������2��
			  }
		   }
		   if(temp==2)	 // �ٶȼ�
		   {
			   speed=speed-100;
			   if( speed <= 600 )
			   {
		  		speed=800;
		  		beep(1);		//������2��
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
