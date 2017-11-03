#pragma config(Motor, motorC, rightMotor, tmotorNormal, PIDControl, encoder)
#pragma config(Motor, motorB, leftMotor, tmotorNormal, PIDControl, encoder)
#pragma config(Sensor, S4, sonarSensor, sensorSONAR)
#pragma config(Sensor, S3, lightSensor, sensorLightActive)


//Revisada: Sirve
//SENSORES: Luz 3 Sonar 4 Motores B,C
//ROBOT Seguidor de luz

#define CUARTOVUELTA 800
#define MEDIAVUELTA 1500//valor original, no me gustó
#define GIRO CUARTOVUELTA

void giro()//funcion para girar el robot
{
	motor[motorC]=50;
	motor[motorB]=0;
	wait1Msec(GIRO);//lo cambié de 1500 por que este hace un cuearto de vuelta
}

void alto()//Definición de la función del detener al robot
{
		motor[motorC]=0;
		motor[motorB]=0;
		wait1Msec(200);
}

void reversa()//Definición de la función de reversa
{
	motor[motorC]=-30;
	motor[motorB]=-30;
	wait1Msec(600);
}

task main()
{
	while(true)
	{
		int luzaux,v1,v2,v3,v4,bandera=1;
		short distancia=0;
		giro(); //giro del lego sobre su propio eje

		v1=SensorValue[lightSensor];//la lectura de intensidad de luz
		alto();
		giro();
		v2=SensorValue[lightSensor];//2a lectura de intensidad de luz
		alto();
		giro();
		v3=SensorValue[lightSensor];//3a lectura de intensidad de luz
		alto();
		giro();
		v4=SensorValue[lightSensor];//4a lectura de intensidad de luz
		alto();
		wait1Msec(400);
		/*
		nxtDisplayTextLine(1,"V1: %d",v1);
		nxtDisplayTextLine(3,"V2: %d",v2);
		nxtDisplayTextLine(5,"V3: %d",v3);
		nxtDisplayTextLine(7,"V4: %d",v4);
		wait10Msec(100);
		*/

		nxtDisplayTextLine(1,"V4 MAYOR");
		if(v1>v2 && v1>v3 && v1>v4)
		{
			motor[motorC]=50;//Si v1 es mayor que los demás
			motor[motorB]=0;
			wait1Msec(GIRO);
			nxtDisplayTextLine(1,"V1 MAYOR");
		}
		else
		{
			if(v2>v3 && v2>v4)
			{
				motor[motorC]=50;
				motor[motorB]=0;//Si v2 es mayor que los demás
				wait1Msec(GIRO*2);
				nxtDisplayTextLine(1,"V2 MAYOR");
			}
			else
			{
				if(v3>v2 && v3>v4)
				{
					motor[motorC]=-50;
					motor[motorB]=0;//Si v3 es mayor que los demás
					wait1Msec(GIRO);
					nxtDisplayTextLine(1,"V3 MAYOR");
				}
			}
		}

		//Avanzar en la dirección con mayor intensidad luminosa hasta encontrar un obstaculo
		while(bandera)
		{
			distancia=SensorValue[sonarSensor];
			luzaux=SensorValue[lightSensor];
			nxtDisplayTextLine(3,"Sensor Sonar: %d",distancia);
			nxtDisplayTextLine(5,"Sensor Luz: %d",luzaux);
			//Mostrar el valor del sensor ultrasónico en el LCD del robot
			if(SensorValue[sonarSensor]>30)
			{
				motor[motorC]=40;
				motor[motorB]=40;
			}
			else
			{
				alto();
				reversa();
				playTone(784,15);//no es PlayTone...es en minúsculas
				//sonido de alto
				wait10Msec(50);
				bandera=0;
			}
		}
	}
}
