#ifndef __PID 
#define __PID

#include "stm32f10x.h"

typedef struct
{
	float setTemp;
	float tempFromSensor;
	
  float Kp;
	float tSample;
	float Ti;
	float Td;
	
	float t1,t2,t3;
	
	float Pout, Iout, Dout;
	float eSum;
	
	float ePresent;
	float eLast;
	float eBeforeLast;

  float Delta_OUT;
	float OUT;
	
	uint16_t tFlag;
		
}PID_struct;
	
 void PID_Init(void);
	
	
extern PID_struct PID;

 void PID_Calculate(void);


#endif
