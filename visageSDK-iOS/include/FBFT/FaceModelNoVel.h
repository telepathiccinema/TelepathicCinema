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
