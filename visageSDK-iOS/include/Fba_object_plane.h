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


#ifndef __Fba_object_plane_h__
#define __Fba_object_plane_h__

// C++
#include <stdio.h>

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "Fba_in_buffer.h"
#include "Fba_object_plane.h"
#include "Face_object.h"
#include "Body_object.h"

namespace VisageSDK
{

class VISAGE_DECLSPEC Fba_object_plane  
{
public:
	Fba_object_plane();
	void init(FILE *inStr); //void init(int inStr);
	virtual ~Fba_object_plane();

	Fba_in_buffer stream;
	Face_object fo;
	Body_object bo;
};

}

#endif // __Fba_object_plane_h__
