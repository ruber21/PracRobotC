#pragma config(Sensor, S3, touchSensor, sensorTouch)

float B;
float C;

task main()
{

	B = 0;
	C = 0;

	nMotorEncoder[motorC] = 0;
	nMotorEncoder[motorB] = 0;

	while(SensorValue(touchSensor) == 0)
	{
		motor[motorB] = 10;
		motor[motorC] = 10;
		B = nMotorEncoder [motorC];
		C = nMotorEncoder [motorB];


		nxtDisplayBigStringAt(12,50,"%3.1f", B);
		nxtDisplayBigStringAt(12,20,"%3.1f", C);
	}
	motor[motorC] = 0;
	motor[motorB] = 0;

	wait1Msec(5000);

}
