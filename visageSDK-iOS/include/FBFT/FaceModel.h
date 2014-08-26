#pragma once

#include "SystemModel.h"

namespace VisageSDK
{

class FaceModel: public SystemModel
{
public:

	FaceModel()
	{
		isCreated = false;

		M = 0;
		N = 0;

		b = 0;
		bVertexCount = 0;

		Wb = 0;
		WbVertexCount = 0;

		A = 0;
		WA = 0;
		a = 0;
		a_use = 0;
		nAU = 0;

		S = 0;
		WS = 0;
		s = 0;
		nSU = 0;

		f = 0;
		scale = 0;

		diagQ = 0;
		diagR = 0;

		Wbs = 0;
		Wbsa = 0;
		Wbsart = 0;
	}

	bool IsCreated()
	{
		return isCreated;
	}

	virtual bool Create(unsigned int nAUs, unsigned int nSUs, unsigned int nVerts, unsigned int nFPs, float focal_len, float _scale)=0;

	virtual void SetInitialState(float* r, float* t, CvMat* a, CvMat* x)=0;
	virtual void ExtractData(float* r, float* t, CvMat* a, CvMat* x)=0;

	// process
	unsigned int GetNumberOfStates()
	{
		return M;
	}

	// observation
	unsigned int GetNumberOfObservations()
	{
		return N;
	}

	//
	virtual void Destroy()=0;

	virtual ~FaceModel() {}

///private:

	bool isCreated;

	unsigned int M, N;

	CvMat* b;
	unsigned int bVertexCount;

	CvMat* Wb;
	unsigned int WbVertexCount;

	CvMat* A;
	CvMat* WA;
	CvMat* a;
	CvMat* a_use;
	unsigned int nAU;

	CvMat* S;
	CvMat* WS;
	CvMat* s;
	unsigned int nSU;

	float f; // camera focal length, used for model projection
	float scale;

	CvMat* diagQ;
	CvMat* diagR;

	// temporary matrices
	CvMat* Wbs;
	CvMat* Wbsa;
	CvMat* Wbsart;
};

}
