#pragma config(Sensor,S2,kCompass,sensorI2CHiTechnicCompass)
#pragma config(Motor, motorB, rightMotor, tmotorNXT, PIDControl)
#pragma config(Motor, motorC, leftMotor, tmotorNXT, PIDControl)
//#include "NXT Compass Sensor Driver.c"//por alguna razón funciona sin esto
//#include "drivers/hitechnic-compass.h"

//Revisada: Sirve, pero correr el calibrador antes.
//SENSORES: Compass 2  Motores B,C
//ROBOT CALCULA TRAYECTORIA
int Angulo (int svalue)
{
	int result;
	if (svalue<0)
	{
		result=(128+svalue)+128;
	}
	else
	{
		result=svalue;
	}
	return result;
}

int theta(float valor_x, float valor_y)
{
	float tangente;
	float result;
	int thetaa;
	if(valor_x<=0)
	{
		tangente=valor_y;
	}
	else
	{
		tangente=valor_y/valor_x;//creo que aqui es donde divido entre 0...
	}
	result = atan(tangente)*(180.0 / PI);//divido entre 0?
	thetaa=floor(result);
	//thetaa=floor(result)+3;//para que +3?
	return thetaa;
}

void girar()
{
	motor[rightMotor]=7;
	motor[leftMotor]=-7;
}

int x; //variables globales para almacenar la coordenada destino
int y;


void imprimir_LCD()
{
	x=0;
	y=0;
	int b=0; //variable para indicar que valor se modifica x=0 y=1
	nxtDisplayTextLine(1, "Coordenadas punto");//Desplegar mensaje
	nxtDisplayTextLine(2, "destino:");
	nxtDisplayClearTextLine(4);//Borrar linea 3
	nxtDisplayTextLine(4, "X: %d",x); //Actualiza el valor de X
	nxtDisplayClearTextLine(5);//Borrar linea 4
	nxtDisplayTextLine(5, "Y: %d",y); //Actualiza el valor de Y

	while(b<2)
	{
		switch(nNxtButtonPressed)
		{
			case kRightButton: //Boton incrementar presionado??
	    	if(b==0)//incrementar x
	    	{
	    		x++;//escalado es *20
	    	}
	      if(b==1)//incrementar y
	      {
	    		y++;//escalado es *20
	    	}
	    	wait1Msec(300);//Delay
	    	nxtDisplayClearTextLine(4);//Borrar linea 3
				nxtDisplayTextLine(4, "X: %d",x); //Actualiza el valor de X
				nxtDisplayClearTextLine(5);//Borrar linea 4
				nxtDisplayTextLine(5, "Y: %d",y); //Actualiza el valor de Y
		  break;
	    case kLeftButton:  //Boton decrementar presionada??
	    	if(b==0)//decrementar x
	    	{
	    		if(x>0)
	    		x--;
	    	}
	      if(b==1)//decrementar y
	    	{
	    		if(y>0)
	    		y--;
	    	}
	    	wait1Msec(300);//Delay
	    	nxtDisplayClearTextLine(4);//Borrar linea 3
				nxtDisplayTextLine(4, "X: %d",x); //Actualiza el valor de X
				nxtDisplayClearTextLine(5);//Borrar linea 4
				nxtDisplayTextLine(5, "Y: %d",y); //Actualiza el valor de Y
		  break;
		  case kEnterButton: //Boton enter presionada
		  	b++; //Incrementar selector
		  	nxtDisplayClearTextLine(4);//Borrar linea 3
				nxtDisplayTextLine(4, "X: %d",x); //Actualiza el valor de X
				nxtDisplayClearTextLine(5);//Borrar linea 4
				nxtDisplayTextLine(5, "Y: %d",y); //Actualiza el valor de Y
				wait1Msec(300);//Delay
			break;
		}
	}
}

task main()
{
	imprimir_LCD();//Ingresar coordenadas del punto destino
	int alfa; //variable para almacenar el ángulo de apertura
	int sensor=0; //Variable para almacenar angulo posicion original
	int esc=20; //Variable de escalamiento
	int angulo_final=0;
	int valor; //variable de ayuda
	const tSensors kCompass = S2; //Lectura del sensor

	x=x*esc;//escalando el plano
	y=y*esc;

	sensor=SensorValue[kCompass]; //Leer ángulo inicio
	valor = Angulo(sensor); //Convertir angulo
	alfa = theta(x,y); //Calcular angulo de apertura
	angulo_final=valor-alfa; //Ángulo_final = ángulo de posición inicial-angulo de apertura



	if(angulo_final<0)//Ajustar águlo
	{
		angulo_final=360+angulo_final;
	}

	eraseDisplay(); //Borrar LCD
	//Imprimir ángulo de apertura
	nxtDisplayTextLine(1, "Angulo des: %d", alfa);
	//IMprimir águlo de pos inicial
	nxtDisplayTextLine(3, "Angulo final: %d",angulo_final);
	nxtDisplayTextLine(5, "Angulo real: %d",valor);



	while(true) //Ciclo infinito
	{
		sensor= SensorValue[kCompass];
		valor = Angulo(sensor);//Leer ángulo inicio
		nxtDisplayTextLine(5, "Angulo real: %d",valor);//IMprimir angulo de pos inicial
		girar();//Girar robot
		if(valor>=angulo_final-3 && valor<=angulo_final+3)
		{
			nxtDisplayTextLine(7, "Encontrado");
			break;//se sale de todo el programa.
		}
	}
	//Una vez orientado el robot, se procede a desplazarlo para llegar a la meta
	int B=0;
	int C=0;
	int final;
	nMotorEncoder[motorC]=0;//Resetear encoders
	nMotorEncoder[motorB]=0;
	motor[rightMotor]=0;//Velocidad motores 0
	motor[leftMotor]=0;

	while(true)
	{
		//Calcular distancia euclidiana
		final=sqrt((x*x)+(y*y));
		eraseDisplay();//Borrar LCD
		motor[rightMotor]=30;
		//Encender motores (desplazar robot)
		motor[leftMotor]=30;
		B=(nMotorEncoder[motorB])/20;
		//Ajuste de pasos
		C=(nMotorEncoder[motorC])/20;
		//Imprimir distancia final
		nxtDisplayTextLine(3, "Distancia %d", B);//Por alguna razón no me imprime la C
		nxtDisplayTextLine(6, "Meta %d", final);
		//Distancia recorrida es igual a la distancia total??
		if(floor(B)>=floor(final))//La cambié a B por que la C me daba 0 siempre
		{
			motor[rightMotor]=0; //Frenar robot
			motor[leftMotor]=0;
			break;
		}
	//return;
	}
	return;
}
