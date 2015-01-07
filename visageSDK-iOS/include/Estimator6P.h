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