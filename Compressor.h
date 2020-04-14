/*
  ==============================================================================

    Compressor.h
    Created: 13 Apr 2020 10:58:41am
    Author:  Alex

  ==============================================================================
*/
#include "CircularBuffer.h"
#pragma once

class Compressor {
public:
    Compressor();
    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);
private:
    CircularBuffer buffer;
    float tav, rms, gain;
};