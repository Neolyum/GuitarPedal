/* Audio Library for Teensy 3.X
 * Copyright (c) 2019, Alexander Davis info@matrixwide.com
 *
 * Permission is hereby granted for non-commercial use only.
 * You may not use this code in any product offered for sale without the
 * expresss written approval of the author.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// My take on a string machine ensemble chorus sound like the Roland RS-202
// or Lowrey Symphonic Strings.
// It takes the approach of 6.0 Hz and 0.6 Hz sinewaves making up an LFO,
// which then modulates three delay lines 120 degrees apart in the LFO waveform.

#include <Arduino.h>
#include "Chorus.h"
#include "utility/dspinst.h"
#include "arm_math.h"

#define MULT_16 32767
#define DIV_16 0.0000305185


Chorus::Chorus() {
    memset(delayBuffer, 0, sizeof(delayBuffer));
    memset(lfoTable, 0, sizeof(lfoTable));

    // input index
    inIndex = 0;
    // output indexes
    // default to center of buffer
    outIndex1 = 512;
    outIndex2 = 512;
    outIndex3 = 512;
    // lfo index
    // seprated by thirds to approximate 120 degree phase relationship
    lfoIndex1 = 0;
    lfoIndex2 = 245;
    lfoIndex3 = 490;

    // lfo rate counter
    lfoCount = 0;
    // input index offset
    offset1 = 0;
    offset2 = 0;
    offset3 = 0;
    offsetIndex1 = 0;
    offsetIndex2 = 0;
    offsetIndex3 = 0;
    offsetIndex1B = 0;
    offsetIndex2B = 0;
    offsetIndex3B = 0;

    // generate the LFO wavetable
    for (iC = 0; iC < LFO_SAMPLES; iC++)
    {
        lfoTable[iC] = round(((sin(((2.0 * M_PI)/LFO_SAMPLES) * iC) * LFO_RANGE) / 2.0) + (((sin(((20.0 * M_PI)/LFO_SAMPLES) * iC)) * LFO_RANGE) / 3.0));
    }
}

audio_block_t* Chorus::tick(audio_block_t* block) {
    uint16_t i;

    if (!block)
        return block;

    // buffer the incoming block
    for (i=0; i < AUDIO_BLOCK_SAMPLES; i++)
    {

        // wrap the input index
        inIndex++;
        if (inIndex > (ENSEMBLE_BUFFER_SIZE - 1))
            inIndex = 0;

        delayBuffer[inIndex] = block->data[i] * 0.5;

    }
    // re-load the block with the delayed data
    for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
        // advance the wavetable indexes every COUNTS_PER_LFO
        // so the LFO modulates at the correct rate
        lfoCount++;
        if (lfoCount > COUNTS_PER_LFO) {
            // wrap the lfo index
            lfoIndex1++;
            if (lfoIndex1 > (LFO_SIZE - 1))
                lfoIndex1 = 0;
            lfoIndex2++;
            if (lfoIndex2 > (LFO_SIZE - 1))
                lfoIndex2 = 0;
            lfoIndex3++;
            if (lfoIndex3 > (LFO_SIZE - 1))
                lfoIndex3 = 0;

            // reset the counter
            lfoCount = 0;
        }

        // wrap the output index
        outIndex1++;
        if (outIndex1 > (ENSEMBLE_BUFFER_SIZE - 1))
            outIndex1 = 0;

        outIndex2++;
        if (outIndex2 > (ENSEMBLE_BUFFER_SIZE - 1))
            outIndex2 = 0;

        outIndex3++;
        if (outIndex3 > (ENSEMBLE_BUFFER_SIZE - 1))
            outIndex3 = 0;

        // get the delay from the wavetable
        offset1 = lfoTable[lfoIndex1];
        offset2 = lfoTable[lfoIndex2];
        offset3 = lfoTable[lfoIndex3];

        // add the delay to the buffer index to get the delay index
        offsetIndex1 = outIndex1 + offset1;
        offsetIndex2 = outIndex2 + offset2;
        offsetIndex3 = outIndex3 + offset3;
        /*offsetIndex1B = outIndex1 + offset1 + PHASE_90;
        offsetIndex2B = outIndex2 + offset2 + PHASE_90;
        offsetIndex3B = outIndex3 + offset3 + PHASE_90;*/


        // wrap the index if it goes past the end of the buffer
        if (offsetIndex1 > (ENSEMBLE_BUFFER_SIZE - 1))
            offsetIndex1 = offsetIndex1 - ENSEMBLE_BUFFER_SIZE;
        if (offsetIndex2 > (ENSEMBLE_BUFFER_SIZE - 1))
            offsetIndex2 = offsetIndex2 - ENSEMBLE_BUFFER_SIZE;
        if (offsetIndex3 > (ENSEMBLE_BUFFER_SIZE - 1))
            offsetIndex3 = offsetIndex3 - ENSEMBLE_BUFFER_SIZE;

        // wrap the index if it goes past the buffer the other way
        if (offsetIndex1 < 0)
            offsetIndex1 = ENSEMBLE_BUFFER_SIZE + offsetIndex1;
        if (offsetIndex2 < 0)
            offsetIndex2 = ENSEMBLE_BUFFER_SIZE + offsetIndex2;
        if (offsetIndex3 < 0)
            offsetIndex3 = ENSEMBLE_BUFFER_SIZE + offsetIndex3;
        
        /*// wrap the index if it goes past the end of the buffer
        if (offsetIndex1B > (ENSEMBLE_BUFFER_SIZE - 1))
            offsetIndex1B = offsetIndex1B - ENSEMBLE_BUFFER_SIZE;
        if (offsetIndex2B > (ENSEMBLE_BUFFER_SIZE - 1))
            offsetIndex2B = offsetIndex2B - ENSEMBLE_BUFFER_SIZE;
        if (offsetIndex3B > (ENSEMBLE_BUFFER_SIZE - 1))
            offsetIndex3B = offsetIndex3B - ENSEMBLE_BUFFER_SIZE;
        
        // wrap the index if it goes past the buffer the other way
        if (offsetIndex1B < 0)
            offsetIndex1B = ENSEMBLE_BUFFER_SIZE + offsetIndex1B;
        if (offsetIndex2B < 0)
            offsetIndex2B = ENSEMBLE_BUFFER_SIZE + offsetIndex2B;
        if (offsetIndex3B < 0)
            offsetIndex3B = ENSEMBLE_BUFFER_SIZE + offsetIndex3B;
*/
        // combine delayed samples into output
        // add the delayed and scaled samples
        block->data[i] = int(round((delayBuffer[offsetIndex1] + delayBuffer[offsetIndex2] + delayBuffer[offsetIndex3]) / 3.0));
        //outblockB->data[i] = int(round((delayBuffer[offsetIndex1B] + delayBuffer[offsetIndex2B] + delayBuffer[offsetIndex3B]) / 3.0));

    }

    return block;
}


