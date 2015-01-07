#ifndef FILTER_H
#define FILTER_H

#include "FDP.h"
#include "FaceData.h"
//#include "Estimator.h"
#include <vector>
#include <deque>

using namespace std;

namespace VisageSDK
{
struct InputGazeData
{
	FDP featurePoints2D;
	float faceRotation[3];
	float faceTranslation[3];
	float gazeDirection[2];
	float eyeClosure[2];
	int width, height, index;
	bool isDefined;
	bool isCalib;
	long timestamp;
	float frameRate;
	IplImage* lEyeImage;
	IplImage* rEyeImage;
	float lPupQuality;
	float rPupQuality;
	int usedEye;

	float regularizationWeight;
	float calib_x;
	float calib_y;


	InputGazeData();
	~InputGazeData();

	void setData(int index, bool isDefined, FaceData* data, bool isCalib, long timestamp);
	void cropEyeImage(IplImage* frame);
	IplImage* ExtractEyeRegion(IplImage* tmp, FeaturePoint pupil, float scale, bool eyeSwitch);
	IplImage* ExtractEyeRegionNew(IplImage* tmp, FeaturePoint eyeInCorner, FeaturePoint eyeOutCorner, FeaturePoint eyeTopLid, FeaturePoint eyeBottomLid, int scale, bool eyeSwitch);
};

class Filter
{
public:
	Filter(void);
	~Filter(void);

	void MovingAverageSmooth(std::vector<InputGazeData*> points, int group, int index);
	
	void MedianWindow(std::vector<InputGazeData*> points, int group, int index);

	void BlinkDetection(std::vector<InputGazeData*> trackHeadPosRep, int frameCounts);

	int WindowWidth;
private:
		std::deque<InputGazeData*> window;
		void AverageCurrentWindow(int group, int index);
		void MedianCurrentWindow(int group, int index);
};

class MWFilter
{
public:
	MWFilter(void);
	~MWFilter(void);

	void MedianWindow(std::vector<InputGazeData*> points, int group, int index);

	int WindowWidth;

private:
	std::deque<InputGazeData*> window;
	void MedianCurrentWindow(int group, int index);

};
}
#endif