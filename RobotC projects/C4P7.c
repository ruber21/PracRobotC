#pragma config(Sensor, S2, kCompass, sensorI2CHiTechnicCompass)
#pragma config(Motor, motorB, rightMotor,tmotorNXT,PIDControl)
#pragma config(Motor, motorC, leftMotor,tmotorNXT,PIDControl)
#pragma config(Sensor, S4, sonarSensor, sensorSONAR)//NO estaba en el libro pero supongo que se necesita >:V
//Libreria necesaria para el funcionamiendo del Sensor
//#include "NXT Compass Sensor Driver.c"//Sigue sin funcionar :V




//Revisada: Sirve, Calibrar antes
//SENSORES: Compass 2 Sonar 4 Motores B,C
//ROBOT CALCULA TRAYECTORIA+OBSTÁCULO
int Angulo(int Svalue)
{
	int result;
	if(Svalue<0)//es Svalue no svalue
	{
		result=(128+Svalue)+128;//es Svalue no svalue
	}
	else
	{
		result=Svalue;//es Svalue no svalue
	}
	return result;
}


int theta(float valor_x, float valor_y)
{
	float tangente; //se crea la variable tangente
	float result; //se crea la variable para guarar el resultado
	int thetaa; //variable donde se almacena el angulo
	tangente=valor_y/valor_x;
	result = atan(tangente)*180.0 / PI;//arcotangente en grados
	thetaa=floor(result)+3;//redondeo de grados
	return thetaa;
}


void girar(int g)
{
	motor[rightMotor]=7*g;
	motor[leftMotor]=-7*g;
}

int x; //variables globales para almacenar la coordenada destino
int y;


void imprimir_LCD()
{
	x=0;
	y=0;
	int b=0; //variable para indicar que valor se modifica x=0 y=1
	nxtDisplayTextLine(1, "Coordenadas punto");
	//Desplegar mensaje
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
	    		x++;
	    	}
	      if(b==1)//incrementar y
	      {
	    		y++;
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
	//ROTAR ROBOT
	imprimir_LCD();//Ingresar coordenadas del punto destino
	int alfa; //variable para almacenar el ángulo de apertura
	int sensor=0; //Variable para almacenar angulo posicion original
	int angulo_final=0;
	int valor; //variable de ayuda
	int esc=20; //Variable de escalamiento
	//const tSensors kCompass = S2; //Lectura del sensor//estaba en el anterior programa

	x=x*esc;//escalando el plano
	y=y*esc;

	sensor=SensorValue[kCompass]; //Leer ángulo inicio
	valor = Angulo(sensor); //Convertir angulo
	alfa = theta(x,y); //Calcular angulo de aprtura
	angulo_final=valor-alfa; //Ángulo_final = ángulo de posición inicial-angulo de apertura
	if(angulo_final<0)//Ajustar águlo
	{
		angulo_final=360+angulo_final;
	}
	eraseDisplay(); //Borrar LCD
	//Imprimir ángulo de apertura
	nxtDisplayTextLine(3, "Angulo des: %d", alfa);
	//IMprimir águlo de pos inicial
	nxtDisplayTextLine(5, "Angulo real: %d",valor);

	while(true) //Ciclo infinito
	{
		sensor= SensorValue[kCompass];
		valor = Angulo(sensor);//Leer ángulo inicio
		nxtDisplayTextLine(5, "Angulo real: %d",valor);//IMprimir angulo de pos inicial
		girar(1);//Girar robot
		if(valor>=angulo_final-7 && valor<=angulo_final+7)
		{
			break;
		}
	}

	//Una vez que se orienta el robot hacia el punto A, entonces la siguiente parte del codigo lo desplaza
	float xe=0, ye=0; //Variables de ayuda//nuevo
	int B=0;
	int C=0;
	int final;
	int obs=0;//nuevo
	int ValorUltrasonico=0;//nuevo
  int temp=0;//nuevo
	nMotorEncoder[motorC]=0;//Resetear encoders
	nMotorEncoder[motorB]=0;
	motor[rightMotor]=0;//Velocidad motores 0
	motor[leftMotor]=0;
	while(true)
	{
		final=sqrt((x*x)+(y*y));//Calcular distancia euclidiana
		eraseDisplay();//Borrar LCD
		motor[rightMotor]=30;
		//Encender motores (desplazar robot)
		motor[leftMotor]=30;
		B=(nMotorEncoder[motorB])/20;//Ajuste de pasos
		C=(nMotorEncoder[motorC])/20;
		ValorUltrasonico=SensorValue(sonarSensor);//NO SE CONOCE LA VARIABLE sonarSensor
		nxtDisplayTextLine(2, "Distancia %d", B);//Imp distancia recorrida
		nxtDisplayTextLine(4, "Meta %d", final);//Imprimir distancia total
		nxtDisplayTextLine(6, "obstaculo %d", ValorUltrasonico);//Imprimir valor del ultrasonico

		if(ValorUltrasonico<25)//Se detectó un objeto??
		{
			obs=1; //Activar bandera de detección de objeto
			motor[rightMotor]=0;
			motor[leftMotor]=0;
			//Calcular distancia en x=cos(theta)*(distancia_total-distancia_recorrida)
			xe=(cos((alfa)*(PI/180))*(final-B))+(esc/2);
			//Calcular distancia en y=sin(theta)*(distancia_total-distancia_recorrida)
			ye=(sin((alfa)*(PI/180))*(final-B))*1;//multiplica por 1 -.-
			sensor=SensorValue[kCompass];//Leer ángulo inicio
			valor=Angulo(sensor);//Convretir ángulo
			angulo_final=valor+alfa;//Angulo_final=ángulo de posicion inicial+angulo de apertura
			if(angulo_final>359)//Ajustar angulo
			{
				angulo_final=angulo_final-360;
			}
			while(true)
			{
				temp=SensorValue[kCompass]; //Leer angulo inicio
				valor = Angulo(temp);//Convertir angulo
				girar(-1);//Girar robot
				if(valor==angulo_final)//Robot pos eje x??
				{
					motor[rightMotor]=0;//Frenar robot
					motor[leftMotor]=0;
					nMotorEncoder[motorC]=0;//Resetear encoders
					nMotorEncoder[motorB]=0;
					break;
				}
			}
			while(true)//Ciclo desplazar eje x
			{
		  	motor[rightMotor]=26;//Desplazar robot eje x
				motor[leftMotor]=26;
				C=(nMotorEncoder[motorC])/20;//Ajustes de pasos
				B=(nMotorEncoder[motorB])/20;
				eraseDisplay();
				nxtDisplayTextLine(2,"Distancia %d",C);
				nxtDisplayTextLine(6,"Nueva meta %d", xe);
				if(C==floor(xe))
				{
					motor[rightMotor]=0;//Frenar robot
					motor[leftMotor]=0;
					nMotorEncoder[motorC]=0;//Resetear encoders
					nMotorEncoder[motorB]=0;
					break;
				}
			}
			temp=SensorValue[kCompass];
			valor=Angulo(temp);
			angulo_final=valor-90;
			if(angulo_final<0)
			{
				angulo_final=360+angulo_final;
			}
			while(true)//Ciclo girar eje y
			{
				temp=SensorValue[kCompass];
				valor=Angulo(temp);
				girar(1);
				if(valor==angulo_final)
				{
					motor[rightMotor]=0;//Frenar robot
					motor[leftMotor]=0;
					nMotorEncoder[motorC]=0;//Resetear encoders
					nMotorEncoder[motorB]=0;
					break;
				}
			}
			while(true)//Desplazar eje y
			{
				motor[rightMotor]=26;//Desplazar robot eje x
				motor[leftMotor]=26;
				C=(nMotorEncoder[motorC])/20;
				eraseDisplay();
				nxtDisplayTextLine(2,"Distancia %d",C);
				nxtDisplayTextLine(6,"Nueva meta %d", ye);
				if(C>=floor(ye))
				{
					motor[rightMotor]=0;//Frenar robot
					motor[leftMotor]=0;
					obs=0;
					break;
				}
			}
			break;
	  }
	  if(obs==0 && (floor(C)==floor(final)))
	  {
	  	break;
	  }
	}
	return;
}
