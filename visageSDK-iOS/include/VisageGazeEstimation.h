#ifndef VISAGEGAZEESTIMATION_H
#define VISAGEGAZEESTIMATION_H

#include "ScreenSpaceGazeMapping.h"

namespace VisageSDK
{

class GazeEstimationPredictor
{
	public:

		GazeEstimationPredictor(const GaussianProcessMapping& mapping, CvMat* coeficient);

		GazeEstimationPredictor(const GazeEstimationPredictor& predictor);

		GazeEstimationPredictor& operator=(const GazeEstimationPredictor& predictor);

		~GazeEstimationPredictor();

		float Predict(std::vector<CvMat*> sample);
	
	private:
		GaussianProcessMapping mapping;
		CvMat* coeficient;
};

class GazeEstimationCalibrator
{
	public:
		GazeEstimationPredictor* Calibrate(std::vector<CvMat*> samples, const CvMat* observations, GaussianProcessMapping& mapping);
};


}
#endif