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
* The data files are provided in the folder Samples/data and its subfolders. The whole FaceDetector folder must be distributed with an application using VisageFeaturesDetector, and its path passed to the initialization function.
*
* Implemented in vsvision.lib
*
* \if IOS_DOXY
* Demonstrated in <a href="../facedetect.html">FaceDetector</a> sample project.
* \elseif MACOSX_DOXY
* Demonstrated in <a href="../facedetect.html">FaceDetector</a> sample project.
* \elseif LINUX_DOXY
* Demonstrated in <a href="../../../Samples/Linux/build/msvc100/VisageDetectorDemo/doc/index.html">FaceDetector</a> sample project.
* \elseif REDHAT_DOXY
* Demonstrated in <a href="../../../Samples/Linux/build/msvc100/VisageDetectorDemo/doc/index.html">FaceDetector</a> sample project.
* \else
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc100/FaceDetector/doc/index.html">FaceDetector</a> sample project.
* \endif
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

	/**
	* Performs faces and facial features detection in a still image.
	* 
	* The algorithm detects one or more faces and their features. The results are, for each detected face, the 3D head pose, gaze direction, eye closure, the coordinates of facial feature points, e.g. chin tip, nose tip, lip corners etc and 3D face model fitted to the face.
	*
	* The results are returned in form of FaceData objects. An array of FaceData objects passed to this method as output parameter should be allocated to maxFaces size. 
	* For example:
	* 
	* \code
	* FaceData* data = new FaceData[maxFaces];
	*
	* n = this->m_Detector->detectFacialFeatures(image, data, maxFaces, minFaceScale);
	* \endcode
	*
	* After this call, n contains the number of faces actually detected. The first n members of the data array are filled with resulting data for each detected face.
	* Please refer to the FaceData documentation for detailed description of returned parameters. If maxFaces is smaller than the number of faces actually present in the image, the function will return only first maxFaces detected faces, starting from top left corner of the image.
	*
	* @param frame the input image.
	* @param output pointer to an array of FaceData objects in which the results will be returned.
	* @param maxFaces maximum number of faces to be detected
	* @param minFaceScale scale of smallest face to be searched for, defined as percentage [0-1] of input image size (min(width, height))
	* @show 
	* @return number of detected faces (0 or more)
	*
	* @see FaceData
	*/
	int detectFacialFeatures(IplImage* frame, FaceData* output, int maxFaces = 1, float minFaceScale = 0.1f);

	/**
	* Performs faces and facial features detection in a still image.
	* 
	* The algorithm detects one or more faces and their features. The results are, for each detected face, the 3D head pose, gaze direction, eye closure, the coordinates of facial feature points, e.g. chin tip, nose tip, lip corners etc and 3D face model fitted to the face.
	*
	* The results are returned in form of FaceData objects. An array of FaceData objects passed to this method as output parameter should be allocated to maxFaces size. 
	* For example:
	* 
	* \code
	* FaceData* data = new FaceData[maxFaces];
	*
	* n = this->m_Detector->detectFacialFeatures(image, data, maxFaces, minFaceScale);
	* \endcode
	*
	* After this call, n contains the number of faces actually detected. The first n members of the data array are filled with resulting data for each detected face.
	* Please refer to the FaceData documentation for detailed description of returned parameters. If maxFaces is smaller than the number of faces actually present in the image, the function will return only first maxFaces detected faces, starting from top left corner of the image.
	* 
	* @param imageFileName pointer to the source image file name. The supported file formats are JPEG, PNG, BMP and PPM.
	* @param output pointer to an array of FaceData objects in which the results will be returned.
	* @param maxFaces maximum number of faces to be detected
	* @param minFaceScale scale of smallest face to be searched for, defined as percentage [0-1] of input image size (min(width, height))
	* @return number of detected faces (0 or more)
	*
	* @see FaceData
	*/
	int detectFacialFeatures(const char* imageFileName, FaceData* output, int maxFaces = 1, float minFaceScale = 0.1f);
	
	/**
	* Draws last detection results on top of the provided image.
	* The image format is IplImage, provided by the Intel's OpenCV libraries.
	* 
	* @param img the picture where the detection results are to be drawn
	* @param faceIndex index of the face to be drawn; if set to -1, all detected faces are drawn
	*/
	void drawResults(IplImage* img, int faceIndex = -1);

	/** Get normalized face image.
	*
	* This function returns a normalized face image with corresponding feature points.
	* Size of the normalized face in the image is such that interpupillary distance is approximately a quarter of the image width.
	* 
	* Face will be normalized to a varying degree depending on normalization type. For example, a rotated 
	* face with open mouth will only have its pose straightened with normalization type VS_NORM_POSE, while
	* with addition of VS_NORM_AU the normalized face will also have closed mouth.
	* 
	* Types of normalization are:
	*   - VS_NORM_POSE - face translation and rotation are set to zero thereby normalizing the pose
	*   - VS_NORM_SU - parameters describing the face shape (shape units) are set to zero thereby normalizing the face shape
	*   - VS_NORM_AU - parameters describing facial movements (action units) are set to zero, for example open mouth will be closed
	*
	* Different types of normalization can be combined with "|" operator, for example VS_NORM_POSE | VS_NORM_SU.
	*
	* @param frame grayscale image containing the face to be normalized (input)
	* @param normFace output grayscale image, to be filled with the normalized face image; it must be allocated before calling the function; face size will depend on this image size
	* @param face_data FaceData structure containing the information about the face that will be normalized (input)
	* @param normFDP output features points that correspond to the normalized face; coordinates are normalized to 0-1 range with the origin of the coordinate system (0,0) placed at the bottom left corner of the image
	* @param norm_type normalization type, a binary combination of VS_NORM_POSE - normalizes pose, VS_NORM_SU - normalizes shape units and VS_NORM_AU - normalizes action units
	*/
	void getNormalizedFaceImage (
		IplImage* frame,
		IplImage* normFace,
		FaceData* face_data,
		FDP* normFDP,
		int norm_type
	);

	void setOutputImage(IplImage *image);

	bool initialised;

private:

	friend class VisageTracker2;
	VisageFeaturesDetector(const char *dataPath);

	FDP* fdps;/*!Pointer to a Facial Definition Point object*/
	FDP* featurePoints2D;

	void calculateFDP(FDP* f, int w, int h, CvMat* vert, bool _3D);
	void setFDPIndices(FDP* f);

	bool fitModelToFace(FDP* input, FaceData* output, IplImage * frame);
	bool initFBFT(IplImage* frame);

	VisageDetector* detector;
	FBFT* fbft;
	char* cfg;

	FaceData* data;
	int faces;
};

}
#endif // __VisageFeaturesDetector_h__

