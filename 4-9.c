#pragma config(Sensor, S1,     touchSensor,    sensorTouch)
#pragma config(Sensor, S2,     compassSensor,  sensorVirtualCompass)
#pragma config(Sensor, S3,     lightSensor,    sensorLightActive)
#pragma config(Sensor, S4,     sonarSensor,    sensorSONAR)
#pragma config(Motor,  motorA,          grippeMotor,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          rightMotor,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          leftMotor,     tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "hitechnic-compass.h"


float fuzzy(signed int e, signed int r)
{
  float L = 4 * 180, s;
  if( e < L && r < L)
  {
    if(abs(e) > abs(6) && (e != abs(r)))
    {
      s = (L/(2 * (2 * L - abs(e)))) * (e + r);
    }
    else if(abs(r) > abs(e) && (e != abs(r)))
    {
      s = (L / (2 * (2 * L - abs(r)))) * (e + r);
    }
  }
  else
  {
    if((e > L) && ((-L < r) && (r < L)) && (r != 0))
      s = (L + r) / 2;
    else if((r > L) && ((-L < e) && (e < L)) && e != 0)
      s = (L +e) / 2;
    else if((e < -L) && ((-L < r) && (r < L)) &&  r != 0)
      s = (-L + r) / 2;
    else if ((r < -L) && ((-L < e) && (e < L)) && (e != 0))
      s = (-L + e) / 2;
    else if(e > L && r > L)
      s = L;
    else if(e < -L && r < -L)
      s = -L;
    else if(((e < -L) && (r > L)) || ((e > L) && (r < -L)))
      s = 0;
  }
  return s;
}

task main()
{
  signed int ge = 5, gr = 2, e, r, eant;
  int ac;
  int sFinal = 0, ref = 0;
  float s, gu = 0.5;
  const tSensors kCompass = S2;
  SensorType[kCompass] = sensorI2CCustomStd;
  SensorSubType[kCompass] = subTypeHiTechnicCompass;
  StartTask[taskCompassDeviceDriver];

  while(true)
  {
    wait1Msec(100);
    ref = 90;
    ac = SensorValue[kCompass];
    if(ac < 0) ac = 256 + ac;
    ac = 360 - ac;
    ac %= 360;
    ref %= 360;
    e = ref - ac;
    if (e > 180) e = e - 360;
    if (e < -180) e = e + 360;
    r = e - eant;
    eant = e;
    e = e * ge;
    r = r * gr;
    s = fuzzy(e,r);
    sFinal = (int)abs(s);
    if(sFinal > 50) sFinal = 50;

    if(e > 0)
    {
      motor[motorB] = sFinal;
      motor[motorC] = - sFinal;
    }
    else if( e < 0)
    {
      motor[motorB] = - sFinal;
      motor[motorC] = sFinal;
    }

    nxtDisplayTextLine(1, "angulo: %i", ac);
    nxtDisplayTextLine(4, "error: %i", e);
    nxtDisplayTextLine(5, "referencia: %i", ref);
    nxtDisplayTextLine(7, "salida: %i", sFinal);
  }
}
