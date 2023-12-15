#pragma once

class Effect {
public:
  virtual void setStrength(float f) = 0;
  virtual float tick(float input) = 0;  
};