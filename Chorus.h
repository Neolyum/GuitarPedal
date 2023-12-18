#pragma once

#include <Arduino.h>
#include "Audio.h"
#define ENSEMBLE_BUFFER_SIZE 1024
// to put a channel 90 degrees out of LFO phase for stereo spread
#define PHASE_90 367
#define LFO_SIZE 1470
#define COUNTS_PER_LFO 200

// LFO wavetable parameters
#define LFO_SAMPLES 1470
#define LFO_RANGE 100

class Chorus {
public:
  Chorus(void);
	audio_block_t* tick(audio_block_t* block);
	void setStrength(float f);

private:
    // buffers
    int16_t delayBuffer[ENSEMBLE_BUFFER_SIZE];
    
    // LFO wavetable until I resolve progmem issues
    int16_t lfoTable[LFO_SAMPLES];
    
    // input index
    int16_t inIndex;
    // output indexes
    // default to csenter of buffer
    int16_t outIndex1;
    int16_t outIndex2;
    int16_t outIndex3;
    // lfo index
    // seprated by thirds to approximate 120 degree phase relationship
    int16_t lfoIndex1;
    int16_t lfoIndex2;
    int16_t lfoIndex3;

    // lfo rate counter
    int16_t lfoCount;
    // output index offset
    int16_t offset1;
    int16_t offset2;
    int16_t offset3;
    int16_t offsetIndex1;
    int16_t offsetIndex2;
    int16_t offsetIndex3;
    int16_t offsetIndex1B;
    int16_t offsetIndex2B;
    int16_t offsetIndex3B;
    
    int16_t iC;
    
};