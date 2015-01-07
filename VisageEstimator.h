#include "Estimator.h"
#include "RANSACUtil.h"

namespace VisageSDK
{
class VisageEstimator : public Estimator
{
public:
	VisageEstimator(bool isRealTime, ScreenSpaceGazeRepository* calibrator);

	ScreenSpaceGazeData* Estimate(InputGazeData* data);
	void Calibrate(int pass = 0);
	~VisageEstimator();


private:
	int offset;
	int CountSamples();
	void ReadConfig();
		
	float* xGazeParams;
	float* yGazeParams;

	float initEyex;
	float initEyey;
	float dxCenterEye;
	float dyCenterEye;

	float avgtx;
	float avgty;
	float avgtz;

	//physical params
	float scaleX;
	float scaleY;
	float camOffsetX;
	float camOffsetY;
	int resolutionX;
	int resolutionY;
	float fx;
	float fy;

};
}