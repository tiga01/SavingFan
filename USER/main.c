//����������Ӧ����: �Զ���ʽ���¶ȡ�PWM����ֵ
//�ֶ���ʽ���¶ȡ�PWM����ֵ
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

extern u8 temper;		//���������õ���ֵ����
extern u16 speed;		//���������õ��ֶ�PWM
extern u8 function;		//���������õ��ֶ���ʽfunction=2
extern u8 flag_cls;
extern u8 flag_off;
extern u8 flag_sec;

int main(void)
{
	u16 adcx;
	u8 num ,flag_sf=0;
	u8 threshold=34;		//��ֵ��������
	float temp,temp1;		//��ʱ�¶�����
	u32 arr=1999;		//PWM���ֵ
	u16 pwm_dt = 900;		//PWM�ĵ�ǰֵ
	unsigned char buff[4];

	RCC_Configuration();
	GPIO_Configuration();
	TIM2_Configuration(arr, 0);		//pwm��init
	delay_init();	    	 //��ʱ������ʼ��
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	Adc_Init();		  		//ADC1��ʼ��
	LED_Init();
	beep_Init();//��������ʼ��
	I2C_Configuration();
	OLED_Init();
	key_Init();
	keyscan();
	OLED_CLS();//����
	Timer_Init();
	TIM3_NVIC_Configuration();
	
	void itoa(int i,unsigned char* string,u8 num);

	
	for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//������������Ӧ���ȡ� 
		}
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(200);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(400);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(200);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(200);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//��������,'0'��Ч
	delay_ms(200);
		
	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_5,10);		//ADCֵ
		temp=(float)adcx*(3.3/4096);							//��ѹֵ
    num=look_up_table(NTCTAB,241,adcx);				//���õ��±�
		temp=num_to_temperature(num);							//����õ����¶�ֵ

//	printf("%f\n",temp);
		
	if(function==0)		//�Զ�ģʽ
		{
		if(flag_cls) {OLED_Fill(0x00);flag_cls=0;}			//��һ�ν�������
		for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//������������Ӧ���ȡ� 
		}
		for(u8 i=8;i<12;i++)  //���Զ���ʽ��
		{
			OLED_ShowCN(31+(i-8)*16,2,i);//���Զ���ʽ��   �¶�ռ�ձ���ֵ
		}
		OLED_ShowCN(0,4,12);		//��
		OLED_ShowCN(16,4,13);		//��
		temp1 = temp*10;		//����С��		
		itoa(temp1,buff,1);	//��С�����־
		OLED_ShowStr(32,4,buff,2);
		
		keyscan();			//�õ���ֵtemper��PWM����speed

		OLED_ShowCN(66,4,17);		//��
		OLED_ShowCN(82,4,18);		//ֵ
		itoa(temper,buff,0);	//����С�����־
		OLED_ShowStr(98,4,buff,2);

		OLED_ShowCN(0 ,6,14);		//ռ
		OLED_ShowCN(16,6,15);		//��
		OLED_ShowCN(32,6,16);		//��
		itoa(speed,buff,0);	//����С�����־
		OLED_ShowStr(48,6,buff,2);
		}

	if(function==1)		//������ֵ
		{
		keyscan();
		if(flag_cls) {OLED_Fill(0x00);flag_cls=0;}			//��һ�ν�������
		for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//������������Ӧ���ȡ� 
		}
		OLED_ShowCN(31,2,12);//����
		OLED_ShowCN(47,2,13);//����
		OLED_ShowCN(63,2,17);//����
		OLED_ShowCN(79,2,18);//��ֵ

		OLED_ShowCN(31,4,12);		//��
		OLED_ShowCN(47,4,13);		//��
		temp1 = temp*10;		//����С��		
		itoa(temp1,buff,1);	//��С�����־
		OLED_ShowStr(63,4,buff,2);

		OLED_ShowCN(31,6,17);		//��
		OLED_ShowCN(47,6,18);		//ֵ
		itoa(temper,buff,0);	//����С�����־
		OLED_ShowStr(63,6,buff,2);
		}

	if(function==2)		//�ֶ���ʽ
		{
		keyscan();			//�õ���ֵtemper��PWM����speed
		if(flag_cls) {OLED_Fill(0x00);flag_cls=0;}			//��һ�ν�������
		for(u8 i=0;i<8;i++)
		{
			OLED_ShowCN(1+i*16,0,i);//������������Ӧ���ȡ� 
		}
		OLED_ShowCN(31,2, 6);//����
		OLED_ShowCN(47,2, 9);//����
		OLED_ShowCN(63,2,10);//����
		OLED_ShowCN(79,2,11);//��ֵ

		OLED_ShowCN(31,4,12);		//��
		OLED_ShowCN(47,4,13);		//��
		temp1 = temp*10;		//����С��		
		itoa(temp1,buff,1);	//��С�����־
		OLED_ShowStr(63,4,buff,2);

		OLED_ShowCN(15 ,6,14);		//ռ
		OLED_ShowCN(31,6,15);		//��
		OLED_ShowCN(47,6,16);		//��
		itoa(speed,buff,0);	//����С�����־
		OLED_ShowStr(63,6,buff,2);
		}

		if(temp < temper) {pwm_dt = 0;flag_sf=0;}	//23.5=0
		else
		if(flag_sf==0){
			TIM_SetCompare4(TIM2,1999);//TIM2_CH4
			delay_ms(1000);
			flag_sf=1;
		}
		if(temp < threshold && temp >= temper) pwm_dt=100*(temp-14);	//��ʽ��
		if(temp >= threshold) pwm_dt = 1999;	//
		
		if(function!=2)	TIM_SetCompare4(TIM2,pwm_dt);	//TIM2_CH4,�Զ���PWMֵ
			else	TIM_SetCompare4(TIM2,speed);					//TIM2_CH4,�ֶ���PWMֵ

		itoa(pwm_dt,buff,0);	//����С�����־
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
void itoa(int i,unsigned char* string,u8 num)		//char* string �������� unsigned��Ҫǰ��һ��
{
    int power, j;
    j=i;
    for(power=1;j>=10;j/=10)   power*=10;		//����λ��
    for(;power>0;power/=10)
    {
      if(power == 1 && num ) *string++='.' ;
			*string++='0'+i/power;		//���ڱ������λ����λʱpower=100
      i%=power;
    }
    *string='\0';
}
/////////////////////////////////////////
//		OLED_ShowStr(0,7, buff,1);
//		printf("��õ��¶�Ϊ%s\n\r\n",CH);	//%%%
//			USART_SendData(USART1,temper);	//%%%
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}		//%%%
//			USART_SendData(USART1,speed);	//%%%
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}		//%%%
//			USART_SendData(USART1,function);	//%%%
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}		//%%%
			

