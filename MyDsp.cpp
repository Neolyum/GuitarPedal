#include "MyDsp.h"
#include "Distortion.h"
#include "BiQuad.h"


#define MULT_16 32767
#define DIV_16 0.0000305185

MyDsp::MyDsp() : 
AudioStream(AUDIO_CHANNELS, new audio_block_t*[AUDIO_CHANNELS]) {Serial.println("Instance MyDsp");}

MyDsp::~MyDsp(){}

void MyDsp::toggleEffect() {
  // sets the next effect in the List. The count is used to fall back to the first effect after the last one
  currentEffect = static_cast<Effect>((currentEffect + 1) % COUNT);
}

void MyDsp::update() {
  audio_block_t* inBlock[AUDIO_CHANNELS];
  audio_block_t* outBlock[AUDIO_CHANNELS];
  float currentSample = 0;
  for (int channel = 0; channel < AUDIO_CHANNELS; channel++) {
    inBlock[channel] = receiveReadOnly(channel);
    outBlock[channel] = allocate();
    if (outBlock[channel]) {
      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        if (inBlock[channel]) currentSample = inBlock[channel]->data[i] * DIV_16;

        switch (currentEffect) {
          case DistortionEffect:
            currentSample = distortion.tick(currentSample); 
            break;
          case ChorusEffect:
          default:
            break;
        }
        //currentSample = distortion.tick(currentSample);  //0;//echo.tick(sawtoothSynth() + firstSynth() + secondSynth() + vibratoSynth()) *0.4; // TODO: get mic input as current sample
        currentSample = max(-1,min(1,currentSample));
        int16_t val = currentSample*MULT_16;
        outBlock[channel]->data[i] = val;
      }
      if (inBlock[channel]) release(inBlock[channel]);
      transmit(outBlock[channel], channel);
      release(outBlock[channel]);
    }
  }
}
