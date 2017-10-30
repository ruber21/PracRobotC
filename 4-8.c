#pragma config(Sensor, S3,     lightSensor,    sensorLightActive)
#pragma config(Sensor, S4,     sonarSensor,    sensorSONAR)
#pragma config(Motor,  motorB,          leftMotor,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorNormal, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void giro()
{
	motor[motorC] = 50;
	motor[motorB] = 0;
	wait1Msec(1500);
}

void alto()
{
	motor[motorC] = 0;
	motor[motorB] = 0;
	wait1Msec(200);
}

void reversa()
{
	motor[motorC] = -30;
	motor[motorB] = -30;
	wait1Msec(600);
}

task main()
{
	while(true)
	{
		int v1,v2,v3,v4, bandera = 1;
		short distancia = 0;
		giro();
		v1 = SensorValue[lightSensor];
		alto();
		giro();
		v2 = SensorValue[lightSensor];
		alto();
		giro();
		v3 = SensorValue[lightSensor];
		alto();
		giro();
		v4 = SensorValue[lightSensor];
		alto();
		wait1Msec(400);

		if(v1 > v2 && v1 > v3 && v1 > v4)
		{
			/*motor[motorC] = 50;
			motor[motorB] = 0;
			wait1Msec(820);*/
			giro();
		}
		else
		{
			if(v2 > v3 && v2 > v4)
			{
				//motor[motorC] = 50;
				//motor[motorB] = 0;
				//wait1Msec(1640);
			giro();
			giro();
			}
			else
			{
				if(v3 > v2 && v3 > v4)
				{
					//motor[motorC] = 40;
					//motor[motorB] = 0;
					//wait1Msec(820);
				giro();
				giro();
				giro();
				}
			}
		}

		while(bandera)
		{
			distancia = SensorValue[sonarSensor];
			nxtDisplayTextLine(3, "Sensor Sonar: %d", distancia);
			if(SensorValue[sonarSensor] >  30)
			{
				motor[motorC] = 40;
				motor[motorB] = 40;
			}
			else
			{
				alto();
				reversa();
				PlayTone(784,15);
				wait1Msec(50);
				PlayTone(784,15);
				wait1Msec(50);
				bandera = 0;
			}
		}




	}
}
