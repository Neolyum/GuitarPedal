#include <cmath>

#include "Distortion.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

Distortion::Distortion() : 
drive(0.0),
offset(0.0),
gain(1.0){}

void Distortion::setDrive(float drive) {
  this->drive = drive;
}

void Distortion::setOffset(float offset) {
  this->offset = offset;
}

void Distortion::setGain(float gain) {
  this->gain = gain;
}

float Distortion::algorithm(float x) {
  return sgn(x) * (1 - exp(-abs(x))); //   x / abs(x) * (1 - exp(-(x*x)/abs(x)));
}

float Distortion::tick(float input){
  float output = input * pow(10.0, 2 * drive) + offset;
  output = fmax(-1, fmin(1, output));
  output = algorithm(output);
  return output * gain;
}