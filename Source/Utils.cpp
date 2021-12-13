#include "Utils.h"

float offsetFrequency(float frequency, float semitones)
{
    return std::pow(2, semitones / 12) * frequency;
}
