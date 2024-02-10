//
//  SimpleFilter.hpp
//  SimpleFilter - VST3
//
//  Created by Nicholas Burczyk 1 on 2/7/24.
//  Copyright © 2024 melvinczyk. All rights reserved.
//

#pragma once

#include <vector>
#include "JuceHeader.h"

#define PI 3.14159265358979323846

class SimpleFilter
{
public:
    void setHighPass(bool highPass);
    void setCuttoff(float cuttoffFreq);
    void setSampleRate(float sampleRate);
    
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);
    
private:
    bool highPass;
    float cuttoffFreq;
    float sampleRate;
    
    std::vector<float> dnBuffer;
};
