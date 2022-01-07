namespace Math
{
    #define PI 3.14159265358979323846f

    // Rounds the given value to the nearest int.
    int roundInt(double val);

    // Rounds down the given value.
    int floorInt(double val);

    // Rounds up the given value.
    int ceilInt(double val);

    // Returns the sqare power of the given value.
    double sqpow(double val);

    // Returns 1 if the given value is positive or null, and -1 if it is negative.
    int signOf(double val);

    // Converts the given angle from degrees to radians.
    double degToRad(double deg);

    // Converts the given angle from radians to degrees.
    double radToDeg(double rad);

    // Clamps the given value to be superior or equal to the minimum value and inferior or equal to the maximum value.
    double clamp(double val, double min, double max);

    // Clamps the given value to be inferior or equal to the maximum value.
    double clampUnder(double val, double max);

    // Clamps the given value to be superior or equal to the minimum value.
    double clampAbove(double val, double min);

    // Compute linear interpolation between start and end for the parameter val (if 0 <= val <= 1: start <= return <= end).
    double lerp(double val, double start, double end);

    // Remaps the given value from one range to another.
    double remap(double val, double inputStart, double inputEnd, double outputStart, double outputEnd);

    // Returns true if the given number is a power of 2.
    bool isPowerOfTwo(int val);

    // Returns the closest power of 2 that is inferior or equal to val.
    int getPowerOfTwoUnder(int val);

    // Returns the closest power of 2 that is superior or equal to val.
    int getPowerOfTwoAbove(int val);

    // Returns the number of digits of the given number.
    int getNumOfDigits(int val);
}