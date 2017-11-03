#pragma config(Motor, motorB, rightMotor, tmotorNormal,PIDControl)
#pragma config(Motor, motorC, leftMotor, tmotorNormal,PIDControl)
#pragma config(Sensor, S2, kCompass, sensorI2CHiTechnicCompass)
//#include "NXT Compass Sensor Driver.c"//creo que no va a servir
#include "Includes/hitechnic-irseeker-v2.h"//BUENO, NOPE?



//Revisada: Hice muchos cambios pero ya sirve.
//SENSORES: Compass 2  Motores B,C
//ROBOT BRÚJULA
int Angulo(int svalue)
{
	int result;
	if(svalue<0)
	{ result=(128+svalue)+128;}
	else
	{ result=svalue;}
	return result;
}

void girar()
{
	motor[rightMotor]=5;
	motor[leftMotor]=-5;
}



task main()
{
	//int nCompassHeading=0;///se usa???
	int valor,temp;
	//const tSensors kCompass=S1;
	//SensorSubType[kCompass]=subTypeHiTechnicCompass;//no sirve
	//startTask(taskCompassDeviceDriver); //no sirve usando StartTask, se cambió a minúsculas
	while(true)
	{
		temp=SensorValue[kCompass];
		valor=Angulo(temp);
		nxtDisplayTextLine(4,"Angulo: %d", valor);
		girar();
	}
	return;
}
