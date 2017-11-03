#pragma config(Sensor, S2, kCompass, sensorI2CHiTechnicCompass)//NO DICE QUE SE USA PERO SI SE USA
#pragma config(Motor, motorB, rightMotor, tmotorNormal, PIDControl, encoder)
#pragma config(Motor, motorC, leftMotor, tmotorNormal, PIDControl, encoder)
//#pragma config(Sensor, S1, touchSensor, sensorTouch)//NO SE USA
//#pragma config(Sensor, S3, lightSensor, sensorLightActive)//NO SE USA
//#pragma config(Sensor, S4, sonarSensor, sensorSONAR)//NO SE USA


//#pragma config(Motor, motorA, gripperMotor, tmotorNormal, PIDControl, encoder)//No se usa
//#pragma config(Sensor, S2, compassSensor, sensorVirtualCompass)//sepa por qué está comentado
//#include "NXT Compass Sensor Driver.C" //Se agrega la libreria del compass//Se supone, no puedo hacerlo
//#include "drivers/hitechnic-compass.h"


//Revisada: Sirve
//SENSORES: touch 1 Luz Compass 2 3 Sonar 4 Motores B,C
//SENSORES REALES: Compass 2
//ROBOT logica difuza PID

float fuzzy(signed int e, signed int r)
{
	float L=4*180,s;

	if((e<L) && (r<L))
	{
		if((abs(e)>abs(r))&&(e!=abs(r)))
		{
			s=(L/(2*(2*L-abs(e))))*(e+r);//IC 1,2,5,6
		}
		else if((abs(r)>abs(e))&&(e!=abs(r)))
		{
			s=(L/(2*(2*L-abs(r))))*(e+r);//IC 3,4,7,8
		}
	}
	else
	{
		if((e>L)&&((-L<r)&&(r<L)) && (r!=0))
		{
			s=(L+r)/2;//IC 9,10
		}
		else if((r>L)&&((-L<e)&&(e<L)) && (e!=0))
		{
			s=(L+e)/2;//IC 11,12
		}
		else if((e<-L)&&((-L<r)&&(r<L)) && (r!=0))
		{
			s=(-L+r)/2;//IC 13,14
		}
		else if((r<-L)&&((-L<e)&&(e<L)) && (e!=0))
		{
			s=(-L+e)/2;//IC 15,16
		}
		else if((e>L)&&(r>L))
		{
			s=L;//IC 17
		}
		else if((e>-L)&&(r>-L))
		{
			s=-L;//IC 19
		}
		else if(((e<-L)&&(r>L))||((e>L)&&(r<-L)))
		{
			s=0;//IC 18,20
		}
	}
return s;//regresar la salida difusa
}

task main()//Inicia el Programa
{
	signed int ge=5, gr=2, e, r, eant;//g=Ganancia e=error, r=deriada, eant=error anterior
	int ac;//angulo actual
	int sFinal=0, ref=90;//sFinal=Salida final, ref= angulo de referencia
	float s, gu=0.5;//Valor de la salida con punto flotante, gu=ganancia de la salida
	const tSensors kCompass=S2;//configuramos el sensor en el puerto S2

	//Establecemos comunicación con el sensor
	/*SensorType[kCompass]=sensorI2CCustomStd;
	SensorSubType[kCompass]=subTypeHiTechnicCompass;
	StartTask(taskCompassDeviceDriver);*/ //No sirve

	while(true)//Ciclo infinito
	{
		wait1Msec(100);
		ref=180;//Angulo deseado
		ac=SensorValue[kCompass];//Obteniendo el valor del compasFinal
		if(ac<0)//se ajusta el valor del angulo
		{
			ac=256+ac;//solo tiene esto el if de la pag 161.........
		}
		ac=360-ac;
	  	ac%=360;
		ref%=360;
		e=ref-ac;//se obtiene el error
		if(e>180)//se ajusta
		{
			e=e-360;
		}
		if(e<-180)
		{
			e=e+360;
		}
		r=e-eant;//obteniendo la derivada
		eant=e;//Se guarda el error anterior
		e=e*ge;//se multiplica el error por la ganancia
		r=r*gr;//se multiplica la deriva por la ganancia
		s=fuzzy(e,r);//funcion fuzzy, parametros(error,derivada)
		s=s*gu;//Se multiplicas la salida por la ganarcia de salida
		sFinal=(int)abs(s);//Se convierte en entero el valor absoluto de s.
		if(sFinal>50)//se acota la salida
		{
			sFinal=50;
		}
		//Determinar si es necesario girar en que sentido
		if((e>0))//dobles????
		{
			motor[motorB]=(sFinal);
			motor[motorC]=-(sFinal);
		}
		else if((e<0))
		{
			motor[motorB]=-(sFinal);
			motor[motorC]=(sFinal);
		}

	//mostramos información
	nxtDisplayTextLine(1, "angulo: %i",ac);
	nxtDisplayTextLine(4, "error: %i",e);
	nxtDisplayTextLine(5, "referencia: %i",ref);
	nxtDisplayTextLine(7, "salida: %i",sFinal);
	}
}
