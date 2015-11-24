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
