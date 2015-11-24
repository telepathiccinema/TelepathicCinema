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
#include "FBFT/FBFT.h"

namespace VisageSDK {

class PoseEstimator
{
	// Methods
public:
	PoseEstimator(FBFT* fbft, const FDP *input);
	~PoseEstimator(void);

	void InitInformFilter(const FDP* input);
	void ExtractInformFilterData(float r[3], float t[3], CvMat* su, CvMat* x);
	void ResetInformFilterData(bool au_only);
	void GetImageFittFPs(const FDP* input, const int width, const int height);
	void SetEifR();
	bool UpdateEIF(CvMat* y, float dt);
	void OverrideSensitivityParams(bool force);

	void CalcInitTranslations(const FDP *input, const float aspect);

	bool FitModelToFace(const FDP* input, const int width, const int height);

	bool CheckBadStates();

private:
	void GetModelFittFPs(const FDP* input);


	// Attributes
public:
	CvMat* imageFittFPs; // Points in image used for fitting

private:
	ExtendedInformationFilter* extendedIF;
	FBFT* fbft;

	int nFPs; // number of tracked feature points

	SystemModel* ifSysModel;
	FaceModel* ifFaceModel;
		
	int* FPIndexes;
	CvMat* modelFittFPs; // Vertices on model used for fitting
	std::map<std::pair<int, int>, int> indicesFPs;
};

}
