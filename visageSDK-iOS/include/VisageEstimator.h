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

#include "Estimator.h"
#include "RANSACUtil.h"

namespace VisageSDK
{
class VisageEstimator : public Estimator
{
public:
	VisageEstimator(bool isRealTime, ScreenSpaceGazeRepository* calibrator);

	ScreenSpaceGazeData* Estimate(InputGazeData* data);
	void Calibrate(int pass = 0);
	~VisageEstimator();


private:
	int offset;
	int CountSamples();
	void ReadConfig();
		
	float* xGazeParams;
	float* yGazeParams;

	float initEyex;
	float initEyey;
	float dxCenterEye;
	float dyCenterEye;

	float avgtx;
	float avgty;
	float avgtz;

	//physical params
	float scaleX;
	float scaleY;
	float camOffsetX;
	float camOffsetY;
	int resolutionX;
	int resolutionY;
	float fx;
	float fy;

};
}