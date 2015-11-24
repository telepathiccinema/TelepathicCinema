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

#ifndef __BDF_h__
#define __BDF_h__

#include <cv.h>
#include "bdts.h"

namespace VisageSDK
{

class BDF
{
public:
	BDF(const char* path);
	~BDF();

	float* Evaluate(IplImage* image);
    float* y;

private:
	DecisionForest forest;
};

}

#endif // __BDF_h__