
#ifndef __MathMacros_h__
#define __MathMacros_h__

// C++
#include <cmath>

namespace VisageSDK
{

#define V_PI 3.14159265f // PI
#define V_EQF( a, b ) ( ( fabs( ( a ) - ( b ) ) ) < ( 0.00001f ) ) // returns true if 2 float numbers are equal
#define V_EQFT( a, b, t ) ( ( fabs( ( a ) - ( b ) ) ) < ( t ) ) // returns true if 2 float numbers are equal with specified tolerance
#define V_DEG(x) ( (x) * 180.f / V_PI ) // converts radians to degrees
#define V_RAD(x) ( (x) * V_PI / 180.f ) // converts degrees to radians

}

#endif // __MathMacros_h__
