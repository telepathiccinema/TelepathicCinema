#ifndef SCREENSPACEGAZEMAPPING_H
#define SCREENSPACEGAZEMAPPING_H

#include "FaceData.h"
#include "InputGazeData.h"
#include "TrackerGazeCalibrator.h"

namespace VisageSDK
{
	struct EyeData
	{
	public:

		EyeData();
		EyeData(const FaceData& faceData);
		EyeData(const EyeData& data);
		~EyeData();

		EyeData& operator= (const EyeData& data); 

		void resize(int rw, int rh, int lw, int lh);

		IplImage* leftEye;
		IplImage* rightEye;	

	private:
		IplImage* crop(const CvRect& rectangle, IplImage* frame);
		void filter(IplImage* image);
	};

	struct TrainingGazeData : public ScreenSpaceGazeData
	{
	public:

		void addCalibData(float x, float y, int type, int frameIndex);
		void addEyeData(EyeData eyeData);
		void addInputData(InputGazeData data);


		EyeData eyeData;
		InputGazeData data;
		int trackerStatus;
	};


	struct GaussianProcessParams
	{
		float sigma;
		float lscale;
		float noise;
	};

	class GaussianProcessMapping
	{
		public:

			GaussianProcessMapping() {};

			GaussianProcessMapping(const GaussianProcessMapping& mapping);
			
			~GaussianProcessMapping();

			GaussianProcessMapping& operator=(const GaussianProcessMapping& mapping);

			GaussianProcessMapping(std::vector<CvMat*> samples, const GaussianProcessParams& params);

			CvMat* GetMatrix(std::vector<CvMat*> input);

			GaussianProcessParams GetParams();
		
		private:
			GaussianProcessParams params;
			std::vector<CvMat*> samples;
	};
}
#endif