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
#include "Estimator.h"
#include "TrackerGazeCalibrator.h"

//using namespace std;

namespace VisageSDK
{

//typedef vnl_vector<double> vnlVector; // VXL
//typedef vnl_matrix<double> vnlMatrix; // VXL

class EstimatorGauss : public Estimator
{
public:
	EstimatorGauss(bool isRealTime, ScreenSpaceGazeRepository* calibrator);
	~EstimatorGauss(void);

	//// USING IMAGES
	int usedLeft;
	int usedRight;
	
	void Calibrate(int pass);
	bool flag;
	ScreenSpaceGazeData* Estimate(InputGazeData* data);
	
	void AddCData(int index, FaceData* faceData, bool isDefined, long timetsamp);
	void AddEData(int index, FaceData* faceData, bool isDefined, long timestamp);

	IplImage* FillParameters(int i);

private:

	//// USING IMAGES
	double imagedistance(const IplImage *im1, const IplImage *im2);
	//vnlMatrix EstimatorGauss::getcovariancematrix(vector<IplImage*> in1, vector<IplImage*> in2); // VXL
	CvMat* GetCovarianceMatrix(std::vector<IplImage*> in1, vector<IplImage*> in2);
	double CovarianceFunction(const IplImage* im1, const IplImage* im2);
	//vnlVector EstimatorGauss::GaussianProcess(vector<IplImage*> inputs, vnlVector targets, double noise); // VXL
	CvMat* GaussianProcess(std::vector<IplImage*> inputs, CvMat* targets, double noise);
	//double EstimatorGauss::getmeans(IplImage* test, vector<IplImage*> inputs, vnlVector alpha);
	double GetMeans(IplImage* test, std::vector<IplImage*> inputs, CvMat* alpha);

	IplImage* MultiplyIplImage(IplImage* src, double scalar);
	void AddIplImage(IplImage* src1, IplImage* src2, IplImage* dest);
	
	std::vector<IplImage*> images;
	std::vector<double> targets_x;
	std::vector<double> targets_y;
	int scale;

	double sigma;
	double lscale;
	double noise;
	int kernel;

	CvMat* alpha_x;
	CvMat* alpha_y;

	std::vector<IplImage*> inputs;

	CvMat* lAlpha_x;
	CvMat* lAlpha_y;
	CvMat* rAlpha_x;
	CvMat* rAlpha_y;
	
	std::vector<IplImage*> lImages;
	std::vector<IplImage*> rImages;

	std::vector<IplImage*> images_x;
	std::vector<IplImage*> images_y;

	std::vector<double> lTargets_x;
	std::vector<double> lTargets_y;
	std::vector<double> rTargets_x;
	std::vector<double> rTargets_y;

	IplImage* FillParametersX(InputGazeData *data);
	IplImage* FillParametersY(InputGazeData *data);

	std::vector<int> lCounter;
	std::vector<int> rCounter;

	void printMat(string filename, CvMat* mat);

	float prev_x;
	float prev_y;
	float scaleFactor;

	int lwidth;
	int lheight;
	int lnChannels;
	int ldepth;

	int rwidth;
	int rheight;
	int rnChannels;
	int rdepth;

	int leftEyeCount;
	int rightEyeCount;

};

class CalibrationRegularization
{

public:
	CalibrationRegularization(void);
	~CalibrationRegularization(void);

	int AddWeights(ScreenSpaceGazeRepository* calibrationData);
};

}

