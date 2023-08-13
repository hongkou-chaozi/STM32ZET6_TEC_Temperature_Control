#include "timer.h"
#include "stm32f10x.h"
#include "pid.h"

extern uint16_t Kms10;
extern PID_struct PID;
 
void Timer4_init()	//T4 10ms时钟
{	 //72000000/7200=100us
//
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4,ENABLE);
TIM_TimeBaseStructure.TIM_Period = 10000-1; //计数个数     //100us*10=1000us=10ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//分频值   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//分割时钟			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
 TIM_DeInit(TIM4);
TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM4, ENABLE); 	 //使能定时器2

 /*以下定时器4中断初始化*/
TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //向上计数溢出产生中断
}
 
 void TIM4_IRQHandler() //10ms 1次
{					  
//  
	static u8 st;
	st=TIM_GetFlagStatus(TIM4, TIM_IT_Update);
	if(st!=0)
	{  TIM_ClearFlag(TIM4, TIM_IT_Update); 
		Kms10++;
//    PID.tFlag++;
	}
}
