#ifndef INPUTGAZEDATA_H
#define INPUTGAZEDATA_H

#include "FDP.h"
#include "FaceData.h"

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
	int usedEye;

	InputGazeData();
	~InputGazeData();

	void setData(int index, bool isDefined, FaceData* data, bool isCalib, long timestamp);
};
}
#endif