#pragma config(Sensor, S2,     kCompass,  sensorI2CHiTechnicCompass)
#pragma config(Motor,  motorB,          rightMotor,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          leftMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Sensor,S2,kCompass,sensorI2CHiTechnicCompass)
//#pragma config(Motor,  motorB,          rightMotor,    tmotorNXT, 	PIDControl)
//#pragma config(Motor,  motorC,          leftMotor,     tmotorNXT, PIDControl)
//#pragma config(Sensor, S2,     compassSensor,                sensorVirtualCompass)

//#pragma config(Sensor, S1,     HTMC,                sensorI2CCustom)
//#include "hitechnic-compass.h"
#include "Includes/hitechnic-irseeker-v2.h"

int Angulo(int svalue){
	int result;
	if(svalue < 0)
		result = 128 + svalue + 128;
	else
		result = svalue;

	return result;
}

int theta(float valorX, float valorY)
{
	float tangente;
	float result;
	int angle;

	if(valorX <= 0)
	{
		angle = 90;
	}
	else
	{
		tangente = valorY / valorX;
		result = atan(tangente) * 180 / PI;
		angle = floor(result);
	}


	return angle;
}

void girar()
{
	motor[leftMotor] = 10;
	motor[rightMotor] = -10;
}

int x,y; //Coordenadas destino

void imprimirLCD()
{
	x = 0;
	y = 0;
	int b = 0;
	nxtDisplayTextLine(1, "Coordenadas punto");
	nxtDisplayTextLine(2, "Destino");
	nxtDisplayTextLine(4, "X: %d",x);
	nxtDisplayClearTextLine(5);
	nxtDisplayTextLine(5, "Y: %d",y);

	while(b < 2)
	{
		switch(nNxtButtonPressed)
		{
		case kRightButton:
			if(b == 0)
				x++;
			if(b == 1)
				y++;
			wait1Msec(300);
			nxtDisplayClearTextLine(4);
			nxtDisplayTextLine(4, "X: %d", x);
			nxtDisplayClearTextLine(5);
			nxtDisplayTextLine(5, "Y: %d", y);
			break;
		case kLeftButton:
			if(b == 0 && x > 0)
				x--;
			if(b == 1 & y > 0)
				y--;
			wait1Msec(300);
			nxtDisplayClearTextLine(4);
			nxtDisplayTextLine(4, "X: %d", x);
			nxtDisplayClearTextLine(5);
			nxtDisplayTextLine(5, "Y: %d", y);
			break;

		case kEnterButton:
			b++;
			nxtDisplayClearTextLine(4);
			nxtDisplayTextLine(4, "X: %d", x);
			nxtDisplayClearTextLine(5);
			nxtDisplayTextLine(5, "Y: %d", y);
			wait1Msec(300);
			break;
		default: break;
		}
	}
}

task main()
{
	imprimirLCD();
	int alfa;
	int sensor = 0;
	int esc = 20;
	int angulo_final = 0;
	int valor;
	const tSensors kCompass = S2;
	//SensorSubType[kCompass] = subTypeHiTechnicCompass;
	//StartTask(taskCompassDeviceDriver);

	x = x * esc;
	y = y * esc;

	//_target = HTMCsetTarget(HTMC);
	sensor = SensorValue[kCompass];
	//temp = SensorValue[kCompass];
	valor = Angulo(sensor);
	alfa = theta(x,y);
	angulo_final = valor - alfa;
	if(angulo_final < 0)
		angulo_final = 360 + angulo_final;
	eraseDisplay();
	nxtDisplayTextLine(1, "Angulo des: %d", alfa);
	nxtDisplayTextLine(3, "Angulo real: %d", angulo_final);
	nxtDisplayTextLine(5, "Angulo real: %d", valor);
	while(valor != angulo_final)
	{
		sensor = SensorValue[kCompass];
		valor = Angulo(sensor);
		nxtDisplayTextLine(5, "Angulo real: %d", valor);
		girar();
	}

	/////////////Desplazamiento

	int B = 0;
	int C = 0;
	int final;
	nMotorEncoder[motorC] = 0;
	nMotorEncoder	[motorB] = 0;
	motor[rightMotor] = 0;
	motor[leftMotor] = 0;
	while(floor(C) <= floor(final))
	{
		final = sqrt((x * x) + (y * y));
		eraseDisplay();
		motor[rightMotor] = 30;
		motor[leftMotor] = 30;
		B = (nMotorEncoder[motorB]) / 20;
		C = (nMotorEncoder[motorC]) / 20;
		nxtDisplayTextLine(3, "Distancia %d", B);
		nxtDisplayTextLine(6, "Meta %d", final);
	}
	motor[rightMotor]=0; //Frenar robot
	motor[leftMotor]=0;

}
