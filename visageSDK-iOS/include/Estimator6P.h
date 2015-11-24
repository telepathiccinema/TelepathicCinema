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

#ifndef ESTIMATOR6P_H
#define ESTIMATOR6P_H

#include "Estimator.h"


using namespace VisageSDK;
using namespace std;

class Filter;

class Estimator6P : public Estimator
{
public:
	//Estimator6P(FrameDataRepository*, FDPDataRepository*, GazeRepository*);
	Estimator6P(bool isRealTime, ScreenSpaceGazeRepository* calibrator) : Estimator(isRealTime, calibrator){}
	~Estimator6P(void);
	void AddCData(int index, FaceData* faceData, bool isDefined, long timestamp);
	void AddEData(int index, FaceData* faceData, bool isDefined, long timestamp);
	ScreenSpaceGazeData* Estimate(InputGazeData* data);
	void Calibrate(int pass);
	void Calibrate(std::vector<int> indices, int frameCount);
	float GetAvgCalibError();

	float amp_factor_x;
	float amp_factor_y;

protected:
	void CalculateQuality();

private:
	
	Weighter* weighter;
	static const int nParams2D;

	float avgXCalibError;
	float avgYCalibError;
	
	int totalSamples;

	void CalculateReconstructionError();
	float avg_recon_error_x;
	float avg_recon_error_y;

	void Calibrate1limited(CvMat*, CvMat*, CvMat*, int, float);
};

#endif