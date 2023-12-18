#pragma once
#include "Arduino.h"
#include "Effect.h"

class MyDistortion : Effect {
public:
  MyDistortion();
  
  void setDrive(float drive);
  void setOffset(float offset);
  void setGain(float gain);
  float tick(float input) override;

  void setStrength(float f) override {
    setDrive(f);
  }

private:
  float algorithm(float x);
  float drive, offset, gain;

};