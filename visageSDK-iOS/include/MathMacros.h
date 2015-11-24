///////////////////////////////////////////////////////////////////////////////
// 
// (c) Visage Technologies AB 2002 - 2015  All rights reserved. 
// 
// This file is part of visage|SDK(tm). 
// Unauthorized copying of this file, via any medium is strictly prohibited. 
// 
// No warranty, explicit or implicit, provided. 
// 
// This is proprietary software. No part of this software may be used or 
// reproduced in any form or by any means otherwise than in accordance with
// any written license granted by Visage Technologies AB. 
// 
/////////////////////////////////////////////////////////////////////////////


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
