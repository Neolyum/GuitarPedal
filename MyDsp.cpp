#include "MyDsp.h"
#include "Distortion.h"
#include "BiQuad.h"

#define AUDIO_OUTPUTS 2
#define AUDIO_INPUTS 2

#define MULT_16 32767

MyDsp::MyDsp() : 
AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS]) {}

MyDsp::~MyDsp(){}



void MyDsp::update() {
  audio_block_t* inBlock[AUDIO_INPUTS];
  audio_block_t* outBlock[AUDIO_OUTPUTS];
  for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
    outBlock[channel] = allocate();
    inBlock[channel] = receiveReadOnly(channel);
    if (outBlock[channel]) {
      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        float currentSample = inBlock[channel]->data[i];
        currentSample = distortion.tick(currentSample);  //0;//echo.tick(sawtoothSynth() + firstSynth() + secondSynth() + vibratoSynth()) *0.4; // TODO: get mic input as current sample
        currentSample = max(-1,min(1,currentSample));
        int16_t val = currentSample*MULT_16;
        outBlock[channel]->data[i] = val;
      }
      transmit(outBlock[channel], channel);
      release(outBlock[channel]);
    }
  }
}
