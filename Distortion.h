#pragma once
#include "Arduino.h"

class Distortion{
public:
  Distortion();
  
  void setDrive(float drive);
  void setOffset(float offset);
  void setGain(float gain);
  float tick(float input);

private:
  float algorithm(float x);
  float drive, offset, gain;

};