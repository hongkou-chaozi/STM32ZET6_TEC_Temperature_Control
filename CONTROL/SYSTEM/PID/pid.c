#include "pid.h"
#include "bsp_ds18b20.h"
#include "bsp_GeneralTim.h" 

extern float temperature;

PID_struct PID;

void PID_Init()
{
	PID.setTemp = 25;
	PID.Kp = 1500;
//	PID.tSample = 400;
	PID.Ti = 2;
	PID.Td = 0;
}

void PID_Calculate()
{

		double Dk1;
		double Dk2;
		
//		if(PID.tFlag < PID.tSample / 10 ) return;
		
		PID.tempFromSensor = DS18B20_GetTemp_SkipRom ();
		PID.ePresent = PID.setTemp - PID.tempFromSensor;
	
		Dk1 = PID.ePresent - PID.eLast;
		
		Dk2 = PID.ePresent - 2 * PID.eLast + PID.eBeforeLast;
		
		PID.t1 = PID.Kp * Dk1;
		
		PID.t2 = PID.Ti * PID.ePresent;
	
		
		PID.t3 = PID.Td * Dk2;
		
		PID.Delta_OUT = PID.t1 + PID.t2 + PID.t3;
		
		
		PID.OUT += PID.Delta_OUT;

		if(PID.OUT > 500)
			PID.OUT = 500;
		if(PID.OUT < 0)
			PID.OUT = 0;
		
		PID.eBeforeLast = PID.eLast;
		PID.eLast = PID.ePresent;
}
		
		


