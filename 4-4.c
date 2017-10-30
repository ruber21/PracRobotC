#pragma config(Sensor, S4, sonarSensor, sensorSONAR)

void izquierda(){
	motor[motorC] = -20;
	motor[motorB] = 20;
	eraseDisplay();
	nxtDisplayCenteredTextLine(2, "Izquierda");
	wait1Msec(500);
}

void derecha(){
	motor[motorC] = 20;
	motor[motorB] = -20;
	eraseDisplay();
	nxtDisplayCenteredTextLine(2, "Derecha");
	wait1Msec(500);
}


task main()
{
	int velocidad = 0;
	int valorUltrasonico = 0;
	int distancia = 35;
	int ran = 0;

	while(true){
		valorUltrasonico = SensorValue(sonarSensor);
		nxtDisplayCenteredTextLine(0,"Valor Ultrasonico");
		nxtDisplayCenteredBigTextLine(2,"%d",valorUltrasonico);
		nxtDisplayCenteredTextLine(5, "%d", velocidad);
		nxtDisplayCenteredTextLine(7, "Velocidad del motor");
		wait1Msec(100);
		if(SensorValue(sonarSensor) > 50)
		{
			velocidad = SensorValue(sonarSensor) - distancia;
			motor[motorC] = velocidad;
			motor[motorB] = velocidad;
		}
		else if(SensorValue(sonarSensor) < 35)
		{
			ran = random(1);
			if(ran == 1)
				derecha();
			else
				izquierda();
		}
	  if(velocidad > 100)
	  	velocidad = 100;
	}
}
