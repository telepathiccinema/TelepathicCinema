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

#ifndef FILTER_H
#define FILTER_H

#include "InputGazeData.h"
#include <vector>
#include <deque>

using namespace std;

namespace VisageSDK
{
class Filter
{
public:
	Filter(void);
	~Filter(void);

	void MovingAverageSmooth(std::vector<InputGazeData*> points, int group, int index);
	
	void MedianWindow(std::vector<InputGazeData*> points, int group, int index);

	void BlinkDetection(std::vector<InputGazeData*> trackHeadPosRep, int frameCounts);

	int WindowWidth;
private:
		std::deque<InputGazeData*> window;
		void AverageCurrentWindow(int group, int index);
		void MedianCurrentWindow(int group, int index);
};

class MWFilter
{
public:
	MWFilter(void);
	~MWFilter(void);

	void MedianWindow(std::vector<InputGazeData*> points, int group, int index);

	int WindowWidth;

private:
	std::deque<InputGazeData*> window;
	void MedianCurrentWindow(int group, int index);

};
}
#endif