
task main()
{
	//Original
	nSyncedMotors = synchBC;
	nSyncedTurnRatio = +50;

	//setMotorSync(motorB,motorC, +50, 100);
	motor[motorB] = 100;
	wait1Msec(4750);


}
