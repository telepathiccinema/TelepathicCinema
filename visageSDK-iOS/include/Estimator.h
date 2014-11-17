#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include "FDP.h"
#include "FaceData.h"
#include <cv.h>
#include <cxcore.h>
#include "Weighter.h"
#include "TrackerGazeCalibrator.h"
#include "MAFilter.h"

using namespace std;

namespace VisageSDK
{

class Estimator
{
public:
	Estimator();
	Estimator(bool isRealTime, ScreenSpaceGazeRepository* calibrator);
	virtual void AddCData(int index, FaceData* faceData, bool isDefined, long timetsamp);
	virtual void AddEData(int index, FaceData* faceData, bool isDefined, long timestamp);

	void Reset();
	std::vector<int> FindGoodRecalibrationPoints(ScreenSpaceGazeData* estimation);
	virtual ~Estimator(void);

	virtual ScreenSpaceGazeData* Estimate(InputGazeData* data);
	virtual void Calibrate(int pass) = 0;

	bool inCalibration;
	bool Calibrated;
	bool isRealTime;
	float f;
	bool eyesOn;

	float gFactorX;
	float gFactorY;

	std::vector<InputGazeData*> trackerEData;
	std::vector<InputGazeData*> trackerCData;

	std::vector<InputGazeData*> trackerData;

	float gazeOffsetX;
	float gazeOffsetY;

	float quality;
	float calib_perc;
	float estim_perc;

	int width;
	int height;

protected:
	ScreenSpaceGazeRepository* calibrator;
	ScreenSpaceGazeRepository* estimatedCalibration;

	float gazeParameter[100];
	int gazeParameterCount;

	int undefinedEstimCount;
	int undefinedCalibCount;

	bool IsOutOfScreen(ScreenSpaceGazeData* data);
	int outOfScreenCount;

	Filter* filter;
	MWFilter* mwFilter;

	int calibOutliersCount;
	int calibDefinedCount;
	int calibFramesCount;
	int estimDefinedCount;
	int estimFramesCount;

	int fullIter;
	int allocSize;

	void ExportDebugInfo(std::string text);
};
}
#endif