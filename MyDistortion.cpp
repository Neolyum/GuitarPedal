#include "MyDistortion.h"
#include <cmath>


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

MyDistortion::MyDistortion() : 
drive(0.3),
offset(0.0),
gain(1.0){}

void MyDistortion::setDrive(float drive) {
  this->drive = drive;
}

void MyDistortion::setOffset(float offset) {
  this->offset = offset;
}

void MyDistortion::setGain(float gain) {
  this->gain = gain;
}

float MyDistortion::algorithm(float x) {
  return sgn(x) * (1 - exp(-abs(x)));
}

float MyDistortion::tick(float input){
  float output = input * pow(10.0, 2 * drive) + offset;
  output = fmax(-1, fmin(1, output));
  output = algorithm(output);
  return output * gain;
}