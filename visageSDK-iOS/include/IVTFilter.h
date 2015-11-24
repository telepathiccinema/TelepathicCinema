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

#pragma once
#include <vector>
#include <deque>
#include <math.h>
#include "TrackerGazeCalibrator.h"

using namespace std;

namespace VisageSDK
{

class IVTFilter
{
public:
	IVTFilter(void);
	~IVTFilter(void);

	//void VelocityThroughGazeDirection(FDPData* estimation, float x, float y, int index, float frameRate);

	void VelocityThroughGazeDirection(ScreenSpaceGazeData* estimation, float x, float y, int index, float frameRate);

	float vX,vY, deltaEstX, deltaEstY;
	
	//KalmanFilter KF(2, 2, 0);
	//Mat_<float> measurement(2,1);

	void KalmanInit(cv::KalmanFilter KF, cv::Mat_<float> measurement);
	void KalmanPredict(cv::KalmanFilter KF, cv::Mat_<float> measurement, ScreenSpaceGazeData* estimation);

	void IKFInit(cv::KalmanFilter KF, cv::Mat_<float> measurement, float frameRate);
	void IKFPredict(cv::KalmanFilter KF, cv::Mat_<float> measurement, ScreenSpaceGazeData* estimation);

	void CollapseFixationGroups(ScreenSpaceGazeData* estimation);

	std::vector<ScreenSpaceGazeData*> estimationIVT;

private:
	std::vector<ScreenSpaceGazeData*> fixationList;

};
}
