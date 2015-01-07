
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
#include "LPLocRuntime.h"

#define MAX_BDFS 256

#define VS_EYES		(1)
#define VS_NOSE		(2)
#define VS_MOUTH	(4)
#define VS_FACE		(VS_EYES + VS_NOSE + VS_MOUTH)

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

public:

	/** Constructor.
	*	
	*/

	VisageDetector();

	VisageDetector(const char *dataPath);


	/** Destructor.
	*/
	~VisageDetector();


	bool FindEyes(IplImage* faceImage, CvRect* faceRect, CvPoint2D32f* leye, CvPoint2D32f* reye);
	bool FindNose(IplImage* faceImage, CvRect* faceRect, CvPoint2D32f* nose);
	bool FindMouth(IplImage* faceImage, CvRect* faceRect, CvPoint2D32f* p1, CvPoint2D32f* p2);


	/**
	*	Performs fast detection of main facial features (eye centres, mouth corners and nose tip) from a still image containing a face.
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
	* The function tries to detect eye centres, mouth corners and the tip of the nose. The return value is a bit-wise indicator
	* of which features were succesfully detected. Starting from the least significant bit, the first bit indicates detection
	* of the face, second the eyes, third bit the mouth and fourth bit the nose. Thus return value 0 means that no face was detected, 1 means
	* the face was detectde but not any features, 2 means eyes were detected but not nose and mouth, 11 means nose and eyes were detected but not mouth, etc.
	* The value 15 means best possible detection when all features were detected.
	*
	* In case of nose and mouth, when the bit is set to 0 it means that these features are not detected and the corresponding feature points in the FDP object are not set. For the
	* eyes, the value of 0 means that the eyes positions were just estimated and not precisely detected. Eyes feature points in the FDP objects are always set.
	*
	* @param input the input image.
	* @param output pointer to the FDP object in which the results will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @param parts flag that determines which facial features will be detected: VS_EYES, VS_NOSE, VS_MOUTH, VS_FACE
	* @return code representing the result of the detection, see above 
	*
	* @see FDP
	*/	
	int detectMainFacialFeatures(IplImage* input, FDP* output, int parts = 7);
	
	int detectMainFacialFeaturesOld(IplImage* input, FDP* output, int parts = 7);
		
	/**
	* Performs fast detection of main facial features (eye centres, mouth corners and nose tip) from a still image containing a face.
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
	* The function tries to detect eye centres, mouth corners and the tip of the nose. The return value is number of faces detected.
	* Additionally function fills array of bitwise indicators of which features were succesfully detected on each detected face. Each element in filled array correspondes to one of detected faces. Starting from the least significant bit, the first bit in each element indicates detection
	* of the face, second the eyes, third bit the mouth and fourth bit the nose. Thus element value 1 means
	* the face was detected but not any features, 2 means eyes were detected but not nose and mouth, 11 means nose and eyes were detected but not mouth, etc. 
	* The value 15 means best possible detection when all features were detected. If no faces were detected null pointer is returned.
	*
	* In case of nose and mouth, when the bit is set to 0 it means that these features are not detected and the corresponding feature points in the FDP object are not set. For the
	* eyes, the value of 0 means that the eyes positions were just estimated and not precisely detected. Eyes feature points in the FDP objects are always set.
	*
	* @param input the input image.
	* @param output pointer to the FDP object in which the results will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @param success array of bitwise indicators of which features where succesfully detected on each detected face, see above. this object must be constructed. 
	* @param parts flag that determines which facial features will be detected: VS_EYES, VS_NOSE, VS_MOUTH, VS_FACE
	* @param maxFaces maximum number of faces that can be detected
	* @return represents the result of the detection as number of faces found
	*
	* @see FDP
	*/
	int detectMainFacialFeatures(IplImage* input, FDP* output, int* success, int maxFaces = 1, int parts = 7);

	/*
	*	Given an image of a face (i.e., output of a face detector), finds the two pupils. "Left eye" means left eye of the person in the image, not the one that is on the left side of the image.
	*
	* The function always returns positions for the eyes, but if they can not be detected precisely it estimates the positions. The return value indicates if the detection was precise or just estimation.

		@param faceImage the image
		@param leye left eye
		@param reye right eye
		@returns true if eyes were precisely detected, false if one or both eyes positions were just estimated because precise detection failed
	*/
	bool FindEyes(IplImage* faceImage, CvPoint2D32f* leye, CvPoint2D32f* reye);
	bool FindEyes2(IplImage* faceImage, CvRect* faceRect, CvPoint2D32f* leye, CvPoint2D32f* reye);
	bool FindEyesOld(IplImage* faceImage, CvPoint* leye, CvPoint* reye);

	/*
	*	Given an image of a face (i.e., output of a face detector), finds the tip of the nose.

		@param faceImage the image
		@param nose returns the coordinates of the nose tip
		@returns true on success, false if nose is not detected
	*/
	bool FindNose(IplImage* faceImage, CvPoint* nose);

	/*
	*	Given an image of a face (i.e., output of a face detector), finds the mouth corners, only works for BGR color images

		@param faceImage the image
		@param p1 left mouth corner
		@param p2 right mouth corner
		@returns 0 if mouth could not be detected nor estimated; 1 if mouth position was estimated; 2 if mouth was actually detected
	*/
	int FindMouth(IplImage* faceImage, CvPoint* p1, CvPoint* p2);

	void RefineEyePos(IplImage* frame, float &rep_x, float &rep_y, float &lep_x, float &lep_y);

	//
	int detectFDP(IplImage* frame, CvRect* faceRect, FDP* fdp, int group, int n, float& r_x, float& r_y); // for initialization
	int detectFDP(IplImage* frame, FDP* fdp, int group, int n, float& r_x, float& r_y, float& sigma);
	int detectFDPs(IplImage* frame, FDP* fdp, float *r_x, float *r_y, float *sigma);

	//
	float detectEyeOpenness(IplImage* frame, FDP* fdp, char eye, float *sigma = 0, float *out = 0);
	int detectGaze(IplImage* frame, FDP* fdp);

	//
	bool loadFaceCascade(const char *path);

	//
	bool initialised;

	int detectFacialFeatures(IplImage* frame, FDP* output, bool show = false);

	//return - broj lica, 
	int detectFacialFeatures(IplImage* frame, FaceData* output, bool show = false, int maxFaces = 1);

	void SetBBParams(float s0, float ds, float dp);

	void setMinFaceSize(int size);

	float m_s0;
	float m_ds;
	float m_dp;

	LPLocRuntime* LpLoc;
private:
	bool GetFacialLandmark(IplImage* gray, float* r, float* c, float s, float* sigma, IplImage* frame, int group, int index, bool flip);
	bool FindLandmarkPoint(IplImage* gray, float* r, float* c, float s, float* sigma, Cascade* cascade, int nperturbs, int stage = 0, int disable_macro = 0);

	bool FindLandmarkPoints(IplImage* gray, float* r, float* c, float s, float* sigma, int nperturbs, int disable_macro = 0);

	bool FindScalar(IplImage* gray, float* r, float* c, float s, float* o, float* sigma, Ensemble* ensemble, int nperturbs);

	int N3FindFaces(IplImage* gray, CvRect rects[], IplImage* frame);

	int minFaceSize;
	bool useMinFaceSize;

	bool Init(const char* dataPath);

	// bdfs for different stuff
	BDF* bdfs[MAX_BDFS];
	bool hFlipPatchAndResult[MAX_BDFS];

	Cascade *puploc;
	Cascade *puploc2;
	Cascade *cascade93;
	Cascade *cascade92;
	Cascade *cascade312;
	Cascade *cascade314;
	Cascade *cascade36;
	Cascade *cascade38;
	Cascade *cascade42;
	Cascade *cascade44;
	Cascade *cascade46;
	Cascade *cascade84;
	Cascade *cascade81;
	Cascade *cascade82;
	Cascade *cascade34;
	Cascade *cascade32;
	Cascade *cascade22;
	Cascade *cascade23;
	odet *faceOd;
	Ensemble *eye_closure;


	int noFaceDetectCount;
	IplImage *patch_eye;
	IplImage *patch_refine;

	CvMemStorage* mStorage;
    ///container for the xml file trained to detect frontal faces
	CvHaarClassifierCascade* mFaceCascade;
	CvHaarClassifierCascade* mMouthCascade;
	CvHaarClassifierCascade* mNoseCascade;
	CvHaarClassifierCascade* mRightEyeCascade;

	void calculateFDP(FDP* f, int w, int h, CvMat* vert, bool _3D);
	void setFDPIndices(FDP* f);
	bool fitModelToFace(FDP *input, FDP* output, IplImage *frame);
	
	//puni face data objekt
	bool fitModelToFace(FDP* input, FaceData* output, IplImage * frame);

	FBFT* fbft;

	int fliph;

	//Multiply with carry PRNG
	unsigned int m_w;
	unsigned int m_z;
	void smwcrand(unsigned int m_w_, unsigned int m_z_);
	unsigned int mwcrand();

};

}
#endif // __VisageDetector_h__

