
#ifndef __VisageFeaturesDetector_h__
#define __VisageFeaturesDetector_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif


#include <cstdlib>
#include "VisageDetector.h"
#include "FBFT/FBFT.h"
#include "FaceData.h"

using namespace std;

namespace VisageSDK
{

/** Faces and facial features detector implementation.
* 
* This class detects one or more faces and their facial features in an image. The input is an image bitmap or an image file in one of the supported file formats: JPEG, PNG, BMP or PPM.
* The results are, for each detected face, the 3D head pose, the coordinates of facial feature points, e.g. chin tip, nose tip, lip corners etc. and 3D face model fitted to the face.
* The results are returned in one or more FaceData objects, one for each detected face. Please refer to the FaceData documentation for detailed description of returned data.
*
*
* To use the detector, first initialise it by calling the function @ref Initialize(), then call the function @ref detectFacialFeatures()
* to perform facial features detection on the image. Several data files are needed for facial features detection and the path to the folder containing these data files must be passed to the initialization function, for example: 
*	
* \code
* std::string dataPath("."); //Assuming the current working folder contains data files.	
*
* m_Detector->Initialize(dataPath.c_str());
* \endcode
*
* The data files are provided in the folder Samples/[PLATFORM]/data/FaceDetector and its subfolders. The whole FaceDetector folder must be distributed with an application using VisageFeaturesDetector, and its path passed to the initialization function.
*
* Implemented in vsvision.lib
*
* Demonstrated in  <a href="../../../Samples/OpenGL/build/msvc90/FaceDetector/doc/html/index.html">FaceDetector</a> sample project.
*
*/
class VISAGE_DECLSPEC VisageFeaturesDetector{

public:

	/** Constructor.
	*	
	*/
	VisageFeaturesDetector();


	/** Destructor.
	*/
	~VisageFeaturesDetector();

	/* Initialise the feature detector. 
	*
	* In order to initialise the feature detector, this function loads the generic Active Appearance %Model into memory. This model is needed for facial features detection. 
	* The default model named "genericModel" is provided in the folder Samples/OpenGL/data/FaceDetector. Therefore, to initialise the feature detector
	* you may do something like:
	*
	*	std::string aamFileName("../../data/FaceDetector/genericModel");
	*	
	*	m_Detector->LoadStaticModel(&aamFileName);
	*
	* It is best to see how it works in the sample project <a href="../../../Samples/OpenGL/build/msvc90/FaceTracker/doc/html/index.html">VisionExample</a>.
	*
	* The model is actually comprised of two files: one containing the shape information
	* and another containing the appearance information.
	* The two files have extension .shape and .app respectively.
	* The filename passed as parameter should have no extension. If the files are not in the working directory
	* the full path must be given.
	* 
	* 
	* @param aamSFileName the name of appearance model files
	* @return true if successful
	*/
	//bool LoadStaticModel(const char* aamSFileName, bool show = false);
	
	/** Initialise the feature detector. 
	*
	* Several data files are needed for facial features detection and the path to the folder containing these data files must be passed to the initialization function, for example:
	*	
	* \code
	* std::string dataPath("."); //Assuming the current working folder contains data files.	
	*
	* m_Detector->Initialize(dataPath.c_str());
	* \endcode
	*
	* The data files are provided in the folder Samples/[PLATFORM]/data/FaceDetector and its subfolders. The whole FaceDetector folder must be distributed with an application using VisageFeaturesDetector, and its path passed to the initialization function.
	*
	* 
	* @param path the path to the detector data files
	* @return true if successful
	*/
	bool Initialize(const char* path);

	/*
	* Performs facial features detection from a still image containing a face.
	*  
	* The face in the picture should have neutral expression (i.e., facial muscles relaxed and lips closed)
	* and be facing the camera (not rotated). To achieve the best detection please provide a good quality picture, with diffuse
	* lighting conditions and where the head is in the middle of the image, taking not too much of the image area (i.e., less than 50%).
	* 
	* The algorithm detects the face and its features. The result are the coordinates of facial feature points, e.g. chin tip, nose tip, lip corners etc. Coordinates are normalised, so that
	* the upper left corner of the image has coordinates 0,0 and the lower right corner has coordinates 1,1. 
	*
	* The feature points are identified
	* according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
	* belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
	* illustrated in Figure 2 on Page 8 of the <a href="MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
	*
	* Certain feature points, like the ones on the tongue and teeth, can not be detected so they are not returned
	* and their coordinates are always set to zero. These points are:
	* 3.5, 3.6, 6.1, 6.2, 6.3, 6.4, 7.1, 9.8, 9.9, 9.10, 9.11, 10.5, 10.6, 10.9, 10.10, 11.5, 11.6
	* Several other points are estimated, rather than accurately detected, due to their specific locations. Examples of such points
	* are 11.4 (usually hidden in the hair), ears (10.x) as well as cheeck points 5.1 - 5.4 which can not be precisely defined due to lack of
	* features on the cheek. Four additional points from FDP group 12 are also estimated: left and right temple (12.44, 12.45) and the points
	* on the hairline above the temple, left and right (12.46, 12.47).
	*
	* The resulting feature point coordinates are returned in form of an FDP object. This is a container clas used for storage of MPEG-4 feature points.
	* It provides functions to access each feature point by its group and index and to read its coordinates. Note that FDP can store 3D points but
	* here only the x and y coordinates of each point are used.
	* 
	* If a face was found in the image the return value of the function will be 0. If the detection failed, e.g. if the image does not 
	* contain a face, the function returns the value of -1, and the FDP object is not filled with any feature point coordinates.
	* 
	* @param imageFileName pointer to the source image file name. The supported file formats are JPEG, PNG, BMP and PPM.
	* @param output pointer to the FDP object in which the results will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @return code representing the result of the detection: 
	* -1 face not found
	* 0 detection done
	*
	*/
	int detectFacialFeatures(const char* imageFileName, FDP* output,  bool show = false);//, double accuracy = 0.8);

	/*
	* Performs facial features detection from a still image containing a face.
	* 
	* The face in the picture should have neutral expression (i.e., facial muscles relaxed and lips closed)
	* and be facing the camera (not rotated). To achieve the best detection please provide a good quality picture, with diffuse
	* lighting conditions and where the head is in the middle of the image, taking not too much of the image area (i.e., less than 50%).
	* 
	* The algorithm detects the face and its features. The result are the coordinates of facial feature points, e.g. chin tip, nose tip, lip corners etc. Coordinates are normalised, so that
	* the upper left corner of the image has coordinates 0,0 and the lower right corner has coordinates 1,1. 
	*
	* The feature points are identified
	* according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
	* belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
	* illustrated in Figure 2 on Page 8 of the <a href="MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
	*
	* Certain feature points, like the ones on the tongue and teeth, can not be detected so they are not returned
	* and their coordinates are always set to zero. These points are:
	* 3.5, 3.6, 6.1, 6.2, 6.3, 6.4, 7.1, 9.8, 9.9, 9.10, 9.11, 10.5, 10.6, 10.9, 10.10, 11.5, 11.6
	* Several other points are estimated, rather than accurately detected, due to their specific locations. Examples of such points
	* are 11.4 (usually hidden in the hair), ears (10.x) as well as cheeck points 5.1 - 5.4 which can not be precisely defined due to lack of
	* features on the cheek. Four additional points from FDP group 12 are also estimated: left and right temple (12.44, 12.45) and the points
	* on the hairline above the temple, left and right (12.46, 12.47).
	*
	* The resulting feature point coordinates are returned in form of an FDP object. This is a container clas used for storage of MPEG-4 feature points.
	* It provides functions to access each feature point by its group and index and to read its coordinates. Note that FDP can store 3D points but
	* here only the x and y coordinates of each point are used.
	* 
	* If a face was found in the image the return value of the function will be 0. If the detection failed, e.g. if the image does not 
	* contain a face, the function returns the value of -1, and the FDP object is not filled with any feature point coordinates.
	* 
	* @param frame the input image.
	* @param output pointer to the FDP object in which the results will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @return code representing the result of the detection: 
	* -1 face not found
	* 0 detection done
	*
	*/
	int detectFacialFeatures(IplImage* frame, FDP* output, bool show = false);//, double accuracy = 0.8);
	
	/**
	* Performs faces and facial features detection in a still image.
	* 
	* The algorithm detects one or more faces and their features. The results are, for each detected face, the 3D head pose, the coordinates of facial feature points, e.g. chin tip, nose tip, lip corners etc and 3D face model fitted to the face.
	*
	* The results are returned in form of FaceData objects. An array of FaceData objects passed to this method as output parameter should be allocated to maxFaces size. 
	* For example:
	* 
	* \code
	* FaceData* data = new FaceData[maxFaces];
	*
	* n = this->m_Detector->detectFacialFeatures(image, data, maxFaces);
	* \endcode
	*
	* After this call, n contains the number of faces actually detected. The first n members of the data array are filled with resulting data for each detected face.
	* Please refer to the FaceData documentation for detailed description of returned parameters. If maxFaces is smaller than the number of faces actually present in the image, the function will return only first maxFaces detected faces, starting from top left corner of the image.
	*
	* @param frame the input image.
	* @param output pointer to an array of FaceData objects in which the results will be returned.
	* @param maxFaces maximum number of faces to be detected
	* @show 
	* @return Number of detected faces (0 or more).
	*
	* @see FaceData
	*/
	int detectFacialFeatures(IplImage* frame, FaceData* output, int maxFaces = 1);

	/**
	* Performs faces and facial features detection in a still image.
	* 
	* The algorithm detects one or more faces and their features. The results are, for each detected face, the 3D head pose, the coordinates of facial feature points, e.g. chin tip, nose tip, lip corners etc and 3D face model fitted to the face.
	*
	* The results are returned in form of FaceData objects. An array of FaceData objects passed to this method as output parameter should be allocated to maxFaces size. 
	* For example:
	* 
	* \code
	* FaceData* data = new FaceData[maxFaces];
	*
	* n = this->m_Detector->detectFacialFeatures(image, data, maxFaces);
	* \endcode
	*
	* After this call, n contains the number of faces actually detected. The first n members of the data array are filled with resulting data for each detected face.
	* Please refer to the FaceData documentation for detailed description of returned parameters. If maxFaces is smaller than the number of faces actually present in the image, the function will return only first maxFaces detected faces, starting from top left corner of the image.
	* 
	* @param imageFileName pointer to the source image file name. The supported file formats are JPEG, PNG, BMP and PPM.
	* @param output pointer to an array of FaceData objects in which the results will be returned.
	* @param maxFaces maximum number of faces to be detected
	* @return Number of detected faces (0 or more).
	*
	* @see FaceData
	*/
	int detectFacialFeatures(const char* imageFileName, FaceData* output, int maxFaces = 1);

	/*
	* Performs fast detection of main facial features from a still image containing a face.
	*
	* This function is similar to detectFacialFeatures() but with these important differences:
	* - it works much faster
	* - it supports some head rotation and facial expression, not just front-faces neutral faces
	* - it detects only eye centres, mouth corners and nose tip
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
	* 
	* @param frame the input image.
	* @param output pointer to the FDP object in which the results will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @param parts flag that determines which facial features will be detected: EYES, NOSE, MOUTH, FACE
	* @return code representing the result of the detection, see above 
	*
	* @see FDP, detectFacialFeatures()
	*/
	int detectMainFacialFeatures(IplImage* frame, FDP* output, int parts = VS_FACE);

	/*
	* Performs fast detection of main facial features from a still image containing a face.
	*
	* This function is similar to detectFacialFeatures() but with these important differences:
	* - it works much faster
	* - it supports some head rotation and facial expression, not just front-faces neutral faces
	* - it detects only eye centres, mouth corners and nose tip
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
	* The function tries to detect eye centres, mouth corners and the tip of the nose. The return value is number of faces detected.
	* Additionaly function fills array of bitwise indicators of which features were succesfully detected on each detected face. Each element in filled array correspondes to one of detected faces. Starting from the least significant bit, the first bit in each element indicates detection
	* of the face, second the eyes, third bit the mouth and fourth bit the nose. Thus element value 1 means
	* the face was detected but not any features, 2 means eyes were detected but not nose and mouth, 11 means nose and eyes were detected but not mouth, etc. 
	* The value 15 means best possible detection when all features were detected. If no faces were detected null pointer is returned.
	*
	* In case of nose and mouth, when the bit is set to 0 it means that these features are not detected and the corresponding feature points in the FDP object are not set. For the
	* eyes, the value of 0 means that the eyes positions were just estimated and not precisely detected. Eyes feature points in the FDP objects are always set.
	*
	* @param input the input image.
	* @param output pointer to the FDP object or array of FDP objects in which the results for each detected face will be returned; this object must be constructed (e.g. FDP *f = new %FDP()).
	* @param success array of bitwise indicators of which features where succesfully detected on each detected face, see above.
	* @param maxFaces maximum number of faces that can be detected
	* @param parts flag that determines which facial features will be detected: VS_EYES, VS_NOSE, VS_MOUTH, VS_FACE
	* @return represents the result of the detection as number of faces found
	*
	*
	* @see FDP, detectFacialFeatures()
	*/
	int detectMainFacialFeatures(IplImage* frame, FDP* output, int* success, int maxFaces, int parts);
    
	/*
	* Draws the previously detected feature points on top of the provided image. 
	* The image format is IplImage, provided by the Intel's OpenCV libraries.
	* 
	* @param img the picture where the FDPs are to be drawn
	* @deprecated Will be removed in future release
	*/
	void DrawFDPs(IplImage* img);
	
	/**
	* Draws last detection results on top of the provided image.
	* The image format is IplImage, provided by the Intel's OpenCV libraries.
	* 
	* @param img the picture where the detection results are to be drawn
	*/
	void drawResults(IplImage* img);

	//appearance model "static" used for facial features detection in still images
	//StaticModel* aamS;

	bool initialised;

private:

	VisageFeaturesDetector(const char *dataPath);

	FDP* fdps;/*!Pointer to a Facial Definition Point object*/
	FDP* featurePoints2D;

	void calculateFDP(FDP* f, int w, int h, CvMat* vert, bool _3D);
	void setFDPIndices(FDP* f);

	bool fitModelToFace(FDP *input, FDP* output, IplImage *frame);
	bool fitModelToFace(FDP* input, FaceData* output, IplImage * frame);

	VisageDetector* detector;
	FBFT* fbft;
	char* cfg;

	FaceData* data;
	int faces;
	
	void drawLines(IplImage* img, int* points, int num, int r, int g, int b);
	void drawLineLoop(IplImage* img, int* points, int num, int r, int g, int b);
	void drawPoints2D(IplImage* img, int* points, int num, int r, int g, int b);

};

}
#endif // __VisageFeaturesDetector_h__

