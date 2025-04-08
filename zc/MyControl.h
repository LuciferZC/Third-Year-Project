#ifndef _MyControl_H_
#define _MyControl_H_

#include "Arduino.h"
#include "MyParameters.h"

void LEDStart(int LEDPin);

void LEDControl(int LEDPin);


void FanStart(int FanPin1,int FanPin2);

void FanControl(int FanPin1,int FanPin2);

#endif
