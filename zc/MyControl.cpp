#include "MyControl.h"


void LEDStart(int LEDPin)
{
  pinMode(LEDPin,OUTPUT);
}


void LEDControl(int LEDPin)
{
  static int IncreaseValueLED = 1;
  static int LEDValue = 0;
  

  if(lightIntensity < targetLight) IncreaseValueLED = 10;
  else IncreaseValueLED = -10;
  LEDValue = LEDValue + IncreaseValueLED;

  if(LEDValue > 255) LEDValue = 255;
  if(LEDValue < 0) LEDValue = 0;


  analogWrite(LEDPin,LEDValue);
}


void FanStart(int FanPin1,int FanPin2)
{
  pinMode(FanPin1,OUTPUT);
  pinMode(FanPin2,OUTPUT);
}

void FanControl(int FanPin1,int FanPin2)
{
  if(temperature > targetTemp || co2>targetCO2 || tvoc>targetTVOC || airHumidity>targetAir)
  {
    digitalWrite(FanPin1,1);
    digitalWrite(FanPin2,1);
  }
  else{
    
    digitalWrite(FanPin1,0);
    digitalWrite(FanPin2,0);
  }
}

