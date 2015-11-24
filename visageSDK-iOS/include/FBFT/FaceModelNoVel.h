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

#include "FaceModel.h"

namespace VisageSDK
{

class FaceModelNoVel: public FaceModel
{
public:

	bool Create(unsigned int nAUs, unsigned int nSUs, unsigned int nVerts, unsigned int nFPs, float focal_len, float _scale);

	void SetInitialState(float* r, float* t, CvMat* a, CvMat* x);
	void ExtractData(float* r, float* t, CvMat* a, CvMat* x);

	// process
	bool PredictState(CvMat* x_pred, CvMat* x, float dt);
	bool ProcessJacobian(CvMat* F, CvMat* x, float dt);
	bool ProcessNoise(CvMat* Q);
	bool ProcessNoiseInv(CvMat* Qinv);

	// observation
	bool Observe(CvMat* y, CvMat* x);
	bool ObservationJacobian(CvMat* H, CvMat* x);
	bool ObservationNoise(CvMat* R);
	bool ObservationNoiseInv(CvMat* Rinv);

	//
	void Destroy();

	~FaceModelNoVel()
	{
		Destroy();
	}
};

}
