
#ifndef __StaticModel_h__
#define __StaticModel_h__

#include "AppModel.h"
#include "VisageDetector.h"
#include <cv.h>

#define EYES (1)
#define NOSE (2)
#define MOUTH (4)
#define FACE (1+2+4)

namespace VisageSDK
{

/**
	Static Appearance %Model class. 

	The class inherits from the AppModel class and builds on it the functionality more related to the facial features detection. 
	

*/
class StaticModel : public AppModel
{
public:

	/*
	*	Constructor
	*/
	StaticModel(void);

	/*
	*	Destructor
	*/
	~StaticModel(void);

	/*
	*	Load a model from file(s).

		The model is comprised of two files: a txt file containing the shape information
		and a binary file containing the appearance information (images).
		Currently, the two files have extension .shape and .app respectively.
		The filename passed as parameter should have no extension. If the files are not in the working directory
		the full path must be given.
		For example, if we want to load the model comprised of the files model1.shape and model1.app in C:\Data\models\
		we should pass "C:\Data\models\model1" as argument to the function.

		@param aamFileName the name of appearance model files
		@return true if successful
	*/
	bool ReadFromFile(const char *fName, bool show = false);

	/*
	*	Adapt the appearance model to an image. Proper initialization must be provided, i.e., the model should
		be placed roughly on the face.
		The algorithm is the Simultaneous Inverse Compositional Algorithm described by Baker and al.
		the paper "Lucas-Kanade 20 Years On: A Unifying Framework: Part 3"

		@param input the image
		@param maxIter max number of allowed iterations 
		@param epsilon criterion to stop the iterations
		@param show if true a set of debug information (mostly images) are shown

		@returns the measure of the fit, i.e., the image difference between the input image warped onto the mean shape
		and the current instance of the appearance model
	*/
	double AdaptToFrame(IplImage* input, int maxIter = 40, double epsilon = 0.1, bool show = false);

	/*
	*	Initialize the features detection by first calling a face detector and then trying to locate the pupils and the mouth corners based on color.
		This only works for color images.

		@param input the image
		@returns true is a face and eyes were found in the image, false otherwise
	*/
//	bool FindFaceAndInitFeaturesDetection(IplImage* input, double scale_factor=1.1, int min_neighbors=3, int flags=CV_HAAR_DO_CANNY_PRUNING, float divx = 10.0f, float divy = 14.0f);
	bool FindFaceAndInitFeaturesDetection(IplImage* input, double scale_factor=1.2, int min_neighbors=2, int flags=CV_HAAR_DO_CANNY_PRUNING, float divx = 6.0f, float divy = 6.0f);

	/*
	*	Given an image of a face (i.e., output of a face detector), finds the two pupils. "Left eye" means left eye of the person in the image, not the one that is on the left side of the image.
	*
	* The function always returns positions for the eyes, but if they can not be detected precisely it estimates the positions. The return value indicates if the detection was precise or just estimation.

		@param faceImage the image
		@param leye left eye
		@param reye right eye
		@returns true if eyes were precisely detected, false if one or both eyes positions were just estimated because precise detection failed
	*/
	bool FindEyes(IplImage* faceImage, CvPoint* leye, CvPoint* reye);

	/*
	*	Given an image of a face (i.e., output of a face detector), finds the tip of the nose.

		@param faceImage the image
		@param nose returns the coordinates of the nose tip
		@returns true on success, false if nose is not detected
	*/
	bool FindNose(IplImage* faceImage, CvPoint* nose);

	/*
	*	Find iris in an image.
	* Coordinate system for return value is: x axis to the right, y axis downwards in the image.

		@param image the image
		@param center returns coordinates of iris centre within the image
		@param radius expected iris radius, in pixels
		@param eyeRegion If not NULL, defines the region within the input image in which iris detection is performed; otherwise the whole image is processed. Note that return coordinates are always given with respect to the whole input image, not the region.
		@returns nothing
	*/
	void FindIris(IplImage* image, CvPoint* center, int &radius,CvRect *eyeRegion = NULL);

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
	* The resulting feature point coordinates are returned in form of an FDP object. This is a container clas used for storage of MPEG-4 feature points.
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
	* @param parts flag that determines which facial features will be detected: EYES, NOSE, MOUTH, FACE
	* @return code representing the result of the detection, see above 
	*
	* @see FDP
	*/	
	int detectMainFacialFeatures(IplImage* input, FDP* output, int parts = FACE);
    
    int detectMainFacialFeatures(IplImage* input, FDP* output, CvRect faceRect);

	/*
	*	Given an image of a face (i.e., output of a face detector), finds the mouth corners, only works for BGR color images

		@param faceImage the image
		@param p1 left mouth corner
		@param p2 right mouth corner
		@returns 0 if mouth could not be detected nor estimated; 1 if mouth position was estimated; 2 if mouth was actually detected
	*/
	int FindMouth(IplImage* faceImage, CvPoint* p1, CvPoint* p2);

	void ComputeSteepestDescentImages(bool show = false);
	bool LoadFaceCascades(const char *path);


private:

	void UpdateAppearance(bool show);
	void InitAppearance(uint w, uint h, uint numAppParameters);
	bool ReadAppFileCompact(const char *fName, bool show = false);
	void ComputeInverseUpdates(bool show = false);
	void ComputeHessian();
	
	void PreComputeSteepestDescentImages(bool show = false);
	void InvertWarp();

	IplImage *inputSmall;
	CvMat* mParamsUpdates;
    
    int noFaceDetectCount;
	VisageDetector *m_detectorBDTS;

};

}

#endif // __StaticModel_h__
