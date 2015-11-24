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

#ifndef _QUATERNION_H
#define _QUATERNION_H

#ifndef _VEC3F_H
#include "Vec3f.h"
#endif

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

/**
* Quaternion.
* \ingroup VRMLParser
*/
class VISAGE_DECLSPEC Quaternion {
public:
		Quaternion()
		{ x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f; }
		Quaternion(float ix, float iy, float iz, float iw)
		{ x = ix; y = iy; z = iz; w = iw; }
		Quaternion(const Vec3f &axis, float angle);

    float	norm() const;
    void	normalize();
    Quaternion	conj() const;
    Quaternion	operator *(float f) const;
    Quaternion	operator *(const Quaternion &q2) const;
    Vec3f	operator *(const Vec3f &v) const;

public:
    float	x, y, z, w;
};

#endif // _QUATERNION_H