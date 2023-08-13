//              ----------------------------------------------------------------
//              屏幕是七脚0.96寸屏幕，采用SPI通信
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PG12（SCL）
//              D1   接PD5（SDA）
//              RES  接PD4
//              DC   接PD15
//              CS   接PD1    
//              通过定时器3，输出比较通道1输出可控PWM信号
//              ----------------------------------------------------------------
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "led.h"
#include "bsp_SysTick.h"
#include "bsp_ds18b20.h"
#include "bsp_key.h"
#include "bsp_GeneralTim.h" 
#include "pid.h"
#include "timer.h"
#include "bsp_usart.h"


extern uint16_t CCR1_Val;
uint16_t Kms10;
uint16_t Kms500;
extern float temperature;
extern float iSetVal;
//extern float Temp[0],Temp[1],Temp[2];


void Isr_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;// TIM4_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init (&NVIC_InitStructure); 


  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;// TIM3_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init (&NVIC_InitStructure);
}


int main(void)
{
	delay_init();
	OLED_Init();
	Key_GPIO_Config();
	USART_Config();
	Timer4_init();
	TIM4_IRQHandler();
	DS18B20_Init();
	PID_Init();
	Isr_Init();
	
		while(1)
		{	
			
			if(Key_Scan(GPIOA,GPIO_Pin_0))
				PID.setTemp += 0.5;
			
			if(Key_Scan(GPIOC,GPIO_Pin_13))
				PID.setTemp -= 0.5;
			
			temperature = DS18B20_GetTemp_SkipRom ();
			OLED_ShowString(0,0,"Temperatrue:",16);
			OLED_ShowFloatNumber(0,16,temperature,16);
			OLED_ShowString(44,16,"degrees",16);				
//			OLED_Refresh();
			OLED_ShowString(0,32,"Target temp:",16);
			OLED_ShowFloatNumber(0,48,PID.setTemp,16);
			OLED_ShowString(33,48,"degrees",16);	
			OLED_Refresh();
		 
		  PID_Calculate();
		  CCR1_Val=500 - PID.OUT;

	    GENERAL_TIM_Init();
			printf ( "%.2f,%.2f,%.2f,%.2f,%.2f,%.d\n",PID.setTemp,DS18B20_GetTemp_SkipRom (),PID.t1,PID.t2,PID.t3,CCR1_Val);
			delay_ms(50);
	
    }
		 
}

