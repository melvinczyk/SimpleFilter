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

class SimpleFilter
{
public:
    void setHighPass(bool highPass);
    void setCuttoff(float cuttoffFreq);
    void setSampleRate(float sampleRate);
private:
    bool highPass;
    float cuttoffFreq;
    float sampleRate;
};
