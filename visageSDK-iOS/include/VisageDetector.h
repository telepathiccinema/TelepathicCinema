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


#ifndef __VisageDetector_h__
#define __VisageDetector_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include <cv.h>
#include "FaceData.h"
#include "LBFShape.h"
#include "CascadeManager.h"

#ifndef WIN32
#include <pthread.h>
#include <sys/types.h>
#define HANDLE pthread_t*
#endif

#ifdef WIN32
#include <windows.h>
#endif

#define VS_EYES		(1)
#define VS_NOSE		(2)
#define VS_MOUTH	(4)
#define VS_FACE		(VS_EYES + VS_NOSE + VS_MOUTH)

#define LBF_THRESHOLD 5.0f

namespace VisageSDK
{

class BDF;
class FDP;
class FBFT;
struct Cascade;
struct Ensemble;
struct odet;

/*
*
* Implemented in vsvision.lib
*
* Demonstrated in .
*
*/
class VisageDetector {

friend class VisageFeaturesDetector;
friend class VisageFaceAnalyser;
friend class VisageTrackerCore;
friend class SimpleTrackerCore;
friend class VisageTracker2;
friend class LBFShape;

public:

	/** Constructor.
	*	
	*/

	VisageDetector();

	VisageDetector(const char *dataPath);


	/** Destructor.
	*/
	~VisageDetector();

	/**
	* Performs detection of facial bounding boxes from a still image.
	*
	* @param grayImage the input grayscale image.
	* @param boundingBoxes pointer to the CvRect object in which the resulting bounding boxes will be returned.
	* @param minFaceScale defines scale of smallest face to be searched for, defined as percentage[0-1] of input image size (min(width, height))
	* @return represents the result of the detection as number of faces found.
	*/
	int detectFaces(IplImage* grayImage, CvRect* boundingBoxes, float minFaceScale = 0.2f);

	/**
	* Performs detection of facial features points on a still image with given face bounding box.
	*
	* The result are the coordinates of facial feature points. Coordinates are normalised, so that
	* the upper left corner of the image has coordinates 0,0 and the lower right corner has coordinates 1,1. 
	*
	* The feature points are identified
	* according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
	* belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
	* illustrated in Figure 2 on Page 8 of the <a href="MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
	*
	* The resulting feature point coordinates are returned in form of an FDP object. This is a container class used for storage of MPEG-4 feature points.
	* It provides functions to access each feature point by its group and index and to read its coordinates. Note that FDP can store 3D points but
	* here only the x and y coordinates of each point are used.
	*
	* @param grayImage the input grayscale image.
	* @param boundingBox square bounding box containing face in given grayImage.
	* @param featurePoints pointer to the FDP object in which the results will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @return flag indicating success of detection. 
	*
	* @see FDP
	*/	
	bool detectFeaturePoints(const IplImage &grayImage, const CvRect &boundingBox, FDP* featurePoints);

	/**
	* Performs detection of multiple faces and matching facial features points on a still image.
	*
	* The result are the coordinates of facial feature points for every detected face. Coordinates are normalised, so that
	* the upper left corner of the image has coordinates 0,0 and the lower right corner has coordinates 1,1. 
	*
	* The feature points are identified
	* according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
	* belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
	* illustrated in Figure 2 on Page 8 of the <a href="MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
	*
	* The resulting feature point coordinates are returned in form of an FDP object. This is a container class used for storage of MPEG-4 feature points.
	* It provides functions to access each feature point by its group and index and to read its coordinates. Note that FDP can store 3D points but
	* here only the x and y coordinates of each point are used.
	*
	* @param image the input image.
	* @param featurePoints pointer to the FDP object in which the results will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @param maxFaceCount defines maximum number of detected faces that will be processed.
	* @param minFaceScale defines scale of smallest face to be searched for, defined as percentage[0-1] of input image size (min(width, height))
	* @param yawEstimates pointer to the float array in wich yaw estimations will be written; must be allocated to maxFaceCount size. 
	* @return  represents the result of the detection as number of faces found.
	*
	* @see FDP
	*/
	int detectFacesAndFeatures(const IplImage &image, FDP* featurePoints, int maxFaceCount = 1, float minFaceScale = 0.1f, float* yawEstimates = NULL);

	//yaw estimation
	void initYawKF();
	bool detectYaw(IplImage* frame, const CvRect &rect, float& detectedYaw);
	bool detectYaw(IplImage* frame, FDP* featurePoints, float& detectedYaw);

	void setLBFPerturbations(int nperturb = 5);

	bool isInitialized() { return m_cascadeManager->isLBFDataLoaded(); }

	LPLocRuntime* m_lpLoc;

	CascadeManager *m_cascadeManager;

	float m_detectedYaw;

#ifdef D_DISPLAY_DIAGNOSTIC_RECTS
	std::vector<CvRect> m_diagnosticRects;
	std::vector<CvScalar> m_diagnosticRectColors;
#endif

	void displayRect(CvRect rect, CvScalar color);
	void displayPoint(CvPoint point, CvScalar color);

	static void setFaceFinderQC(float qc);

	static void calculateBoundingBox(int width, int height, FDP* fdp, CvRect* boundingBox, float* d = NULL, bool flipY = false);

private:
	bool detectTrackingPoints(const IplImage &grayImage, const CvRect &boundingBox, FDP* featurePoints);

	bool detectLBFPoints_prevDet(IplImage* image, LBFShape* shape, FDP* fdp, float r, float c, float s, int nperturb, float d, bool lprofile = false);

	static void transformData(FDP *fp, int* data, bool order);

	bool detectLBFFace(IplImage* image, FDP* fdp, float r, float c, float s, int nperturb, float d, FaceOrientation orientation);
	bool detectLBFFace_mean(IplImage* image, FDP* fdp, float r, float c, float s, int nperturb, float d, float yaw);
	bool detectLBFPoints(IplImage* image, LBFShape* shape, FDP* fdp, float r, float c, float s, int nperturb, float d, bool lprofile = false);

#ifndef EMSCRIPTEN
	cv::KalmanFilter* m_yawKF;
#endif

	int m_fliph;

	int m_nperturb;
};

}
#endif // __VisageDetector_h__

