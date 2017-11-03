#pragma config(Sensor, S1,     touchSensor,    sensorTouch)

float B;
float C;

//Revisada, presionar el botón continuamente para terminar el programa
//SENSORES: Touch 1  Motores B,C
//Medidor de rotaciones
task main()
{
	B=0;
	C=0;
	nMotorEncoder[motorB]=0;
	nMotorEncoder[motorC]=0;

	while(SensorValue(touchSensor)==0)
	{
		motor[motorB]=20;
		motor[motorC]=20;
		B=nMotorEncoder[motorC];
		C=nMotorEncoder[motorB];

		nxtDisplayBigStringAt(12,50,"%3.1f",B);
		nxtDisplayBigStringAt(12,20,"%3.1f",C);
	}
	motor[motorB]=0;
	motor[motorC]=0;
	wait1Msec(5000);

}
