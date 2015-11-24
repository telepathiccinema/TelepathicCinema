#ifndef __LBFSHAPE_H__
#define __LBFSHAPE_H__

#include <cv.h>
#include <string>
#include <memory>
#include <vector>
#include "FDP.h"
#include "FaceData.h"

namespace VisageSDK
{
	class LPLocRuntime;
	class LBFShape
	{
	public:
		LBFShape(LPLocRuntime* lpLocRuntime);
		~LBFShape();

		bool loadPacked(const std::string& filePath);
		bool load(const std::string& shapeDir);
		bool loadYaw(const std::string& shapeDir);
		
		bool loadFAnShape(const std::string& shapeDir);

		void getLBFOutput(float* r, float* c, float s, unsigned char* pixels, int R, int C, int ldim);
		bool estimateYaw(IplImage* gray, CvRect rectangle, float *yaw, float *sigma);

		int estimateGender(FaceData *data, CvRect *rect);
		IplImage *estimateEmotion(FaceData *data, CvRect *rect, float &emotionProbability,bool prepareImage = true);

		IplImage *prepareImage(const IplImage &inputImg);
		IplImage *getBinFeaturesForFAn(int binaryFeatures[], FaceData *data, CvRect *rect, bool prepareImage = true);

		void findLandmarkPoints(IplImage* gray, float* r, float* c, float s, float* sigma, float* r_vars, float* c_vars, int nPerturbs, int disable_macro = 0, bool lprofile = false);
		void findLandmarkPoints_prevDet(IplImage* gray, float* r, float* c, float s, int disable_macro = 0, bool lprofile = false);
		int getLandmarkCount() const;

		std::vector<int> getLandmarkGroupIndexes() const;
		std::vector<int> getLandmarkPointIndexes() const;

	private:
		class LBFShapeImpl;
		std::auto_ptr<LBFShapeImpl> m_impl;
	};
}

#endif  //__LBFSHAPE_H__