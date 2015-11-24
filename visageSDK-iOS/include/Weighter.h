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

#ifndef WEIGHTER_H
#define WEIGHTER_H

#include <vector>
#include <string>
#include "TrackerGazeCalibrator.h"
#include <cmath>

namespace VisageSDK
{
class Weighter
{
public:
	Weighter(void);
	~Weighter(void);

	void AddWeight(ScreenSpaceGazeData* current, ScreenSpaceGazeData* next);
	int GetWeight(int index);
private:
	std::vector<int> weights;
};
}
#endif