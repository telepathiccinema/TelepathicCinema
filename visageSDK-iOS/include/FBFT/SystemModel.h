#pragma once

#include <cv.h>
#include <stdio.h>

namespace VisageSDK
{

class SystemModel
{
public:

	virtual bool IsCreated()=0;

	// process
	virtual unsigned int GetNumberOfStates()=0;

	virtual bool PredictState(CvMat* x_pred, CvMat* x, float dt)=0;
	virtual bool ProcessJacobian(CvMat* F, CvMat* x, float dt)=0;
	virtual bool ProcessNoise(CvMat* Q)=0;
	virtual bool ProcessNoiseInv(CvMat* Qinv)=0;

	// observation
	virtual unsigned int GetNumberOfObservations()=0;

	virtual bool Observe(CvMat* y, CvMat* x)=0;
	virtual bool ObservationJacobian(CvMat* H, CvMat* x)=0;
	virtual bool ObservationNoise(CvMat* R)=0;
	virtual bool ObservationNoiseInv(CvMat* Rinv)=0;
};

}
