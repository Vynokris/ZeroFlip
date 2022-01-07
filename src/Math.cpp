#include "Math.hpp"
#include <assert.h>
#include <cmath>

// Rounds the given value to the nearest int.
int Math::roundInt(double val)    { return (int)round(val); }

// Rounds down the given value.
int Math::floorInt(double val)    { return (int)floor(val); }

// Rounds up the given value.
int Math::ceilInt(double val)     { return (int)ceil(val); }

// Returns the sqare power of the given value.
double Math::sqpow(double val)    { return val * val; }

// Returns 1 if the given value is positive or null, and -1 if it is negative.
int Math::signOf(double val)      { if (val == 0) return 1; return val / abs((int)val); }

// Converts the given angle from degrees to radians.
double Math::degToRad(double deg) { return deg * (PI / 180.0f); }

// Converts the given angle from radians to degrees.
double Math::radToDeg(double rad) { return rad * (180.0f / PI); }

// Clamps the given value to be superior or equal to the minimum value and inferior or equal to the maximum value.
double Math::clamp(double val, double min, double max)
{
    assert (min <= max); 
    if (val < min) val = min;
    if (val > max) val = max;
    return val;
}

// Clamps the given value to be inferior or equal to the maximum value.
double Math::clampUnder(double val, double max) { if (val > max) val = max; return val; }

// Clamps the given value to be superior or equal to the minimum value.
double Math::clampAbove(double val, double min) { if (val < min) val = min; return val; }

// Compute linear interpolation between start and end for the parameter val (if 0 <= val <= 1: start <= return <= end).
double Math::lerp(double val, double start, double end)
{
    // Source: https://stackoverflow.com/a/4353537
    return start + val * (end - start);
}

// Remaps the given value from one range to another.
double Math::remap(double val, double inputStart, double inputEnd, double outputStart, double outputEnd)
{
    return outputStart + (val - inputStart) * (outputEnd - outputStart) / (inputEnd - inputStart);
}

// Returns true if the given number is a power of 2.
bool Math::isPowerOfTwo(int val)
{
    return val == (int)pow(2, (int)(log(val)/log(2)));
}

// Returns the closest power of 2 that is inferior or equal to val.
int Math::getPowerOfTwoUnder(int val)
{
    return (int)pow(2, (int)(log(val)/log(2)));
}

// Returns the closest power of 2 that is superior or equal to val.
int Math::getPowerOfTwoAbove(int val)
{
    if (isPowerOfTwo(val)) return (int)pow(2, (int)(log(val)/log(2)));
    else                   return (int)pow(2, (int)(log(val)/log(2)) + 1);
}

// Returns the number of digits of the given number.
int Math::getNumOfDigits(int val)
{
    double num_of_digits;
    for (num_of_digits = 0; val >= 1; num_of_digits++)
        val /= 10;
    return num_of_digits;
}
