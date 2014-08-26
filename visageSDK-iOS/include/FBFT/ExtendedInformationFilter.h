#pragma once

#include "SystemModel.h"

namespace VisageSDK
{

// without dynamics

class ExtendedInformationFilter ///: public DynamicFilter
{
public:
	ExtendedInformationFilter();

	//
	bool Create(SystemModel* systemModel);
	
	//
	bool Initialize(CvMat* x0, CvMat* I0);

	//
	bool Update(CvMat* y, float dt);

	//
	void Destroy();

	/*
		
	*/

	~ExtendedInformationFilter()
	{
		Destroy();
	}

	/// !!!!!!!!!!!!!!!!!
	SystemModel* mSystemModel;

///private:

	bool created;
	bool initialized;

	CvMat* x; //internal states
	CvMat* x_min;

	CvMat* I;
	CvMat* Iinv;
	CvMat* I_min;

	CvMat* Qinv; // inverted process noise covariance matrix
	CvMat* Rinv; // inverted measurement noise covariance matrix
	
	//the rest are temporary matrices/vectors
	
	CvMat* F;
	CvMat* QinvF;

	CvMat* y_min;
	CvMat* y_diff;

	CvMat* H;
	CvMat* HtRinv;

	CvMat* K;

	CvMat* T;
	CvMat* Tinv;

	CvMat* QinvFTinv;
};

}
