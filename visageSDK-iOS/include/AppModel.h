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

#ifndef __AppModel_h__
#define __AppModel_h__

#include "ShapeModel.h"
#include <cv.h>
#include "FAPs.h"

namespace VisageSDK
{

typedef std::vector<IplImage*> ImageVec;

/**
	Appearance %Model class.

	The class inherits from the ShapeModel class and builds on it the functionality more related to the appearance
	part of the model.
	The appearance is defined as the pixel intensities within a defined shape (the average shape), it should be seen as
	uni-dimensional vector of floating point values bounded between 0 and 1.
	Changes in the appearance are obtained as a linear combination of the average appearance and the so called
	"eigenfaces", representing the most important components of the appearance variation seen during training.


*/
class VISAGE_DECLSPEC AppModel : public ShapeModel
{
public:

	/*
	*	Constructor
	*/
	AppModel(void);

	/*
	*	Destructor
	*/
	~AppModel(void);

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
		@return 1 if successful, -1 if the shape model could not be read, -2 if the issue concerns the appearance model,
		-3 if the error arised while trying to load the xml cascade file
	*/
	int ReadFromFile(const char* fName);

	/*
	*	Initialise the tracking buy running the face detector routine provided by OpenCV.
		The result is a rectangle sorrounding the face and that is used to initialize the position and
		scale of the appearance model.

		@param input the image where to find the face
		@scaleFactor a value that adjusts the first guess of the scale parameter based on the size of the
			rectangle surrounding the detected face, set it smaller than 1 for a smaller initial value and
			larger otherwise
		@Returns true is a face was found in the image and the model place onto it.
	*/
	bool InitTracking(IplImage* input, float scaleFactor, CvRect* roi = NULL);

	/*
	*	Adapt the appearance model to an image. Proper initialization must be provided, i.e., the model should
		be placed roughly on the face.
		The algorithm is the Project Out Inverse Compositional Algorithm described by Baker and al. in "Active Appearance Models Revisited"

		@param input the image
		@param maxIter max number of allowed iterations
		@param epsilon criterion to stop the iterations
		@param show if true a set of debug information (mostly images) are shown

		@returns the measure of the fit, i.e., the image difference between the input image warped onto the mean shape
		and the current instance of the appearance model
	*/
	double AdaptToFrame(IplImage* input, int maxIter = 20, double epsilon = 0.01, bool show = false);

	/** Gets a subset of the MPEG-4 Facial Definition Points' coordinates, normalised.
		@param f pointer to the @ref FDP object that will store the retrieved values
		@param w width of input image
		@param h height of input image
	*/
	void GetFDP(FDP* f, int w, int h);

	/** Computes a particual FAP's value, comparing the current shaoe with the mean one.
		@param fap the index of the desired FAP
		@return the desired FAP's value
	*/
	int GetFAP( int fap ) const;


	/** Retrieves the number of appearance parameters.
		@return the number of appearance modes contained in the appearance model
	*/
	uint GetNumAppearanceParams(){return mNumAppearanceParameters;}

	/** Sets the number of used appearance parameters. Less parameters will give faster tracking/detection.
		No check is perfomed here!
		@param mNumAppearanceParameters the desired number of appearance parameters.
	*/
	void SetNumAppearanceParams(uint n){mNumAppearanceParameters = n;}

	/** Fills up the appearance part of the model with the provided mean, eigenfaces and parameters' boundaries
	*/
	void FillAppearance(int nAppVecs, IplImage* mean, IplImage** eigenFaces, float* eigenValues);

	void CreateTriangleMap();

	/** Set the global shape
	*/
	inline void SetGlobShape(const Shape2D& shape){
		mGlobalShape = shape;
	};

	IplImage* GetMask(){ return mMask; };

	CvSize GetMeanImageSize() const {return mMeanSize;};
	void SetMeanImageSize(const CvSize& size) {mMeanSize = size;};

	/** Do piecewise affine warping
	*/
	void Warp(IplImage* inputImage, IplImage* outputImage, bool show = false);

	void ComputeGradientsFilled(bool show = false, bool detector = false);

	void ComputeSteepestDescentImages(bool show);

	/** Save the appearance model to a .app file (the extension is added to the string given as argument)
	*/
	bool WriteAppearanceFile(const std::string& fname, bool detector = false);

	/** Initialize the appearance model for training purposes
	*/
	void InitAppearanceForTraining(const CvSize& size);

	void PrecomputeJacobianForTraining(bool show = false);

	//resets the appearance model (to be called before doing a new search)
	void ClearAppearanceParameters();

	void ShowAppModel();

private:


	void ValidFAPs(bool* faps) const;
	float GetFAPU( int fapu ) const;
	bool ValidFAP(int fap) const;


protected:


	void InitAppearance(uint w, uint h, uint numAppParameters);
	bool ReadAppFileCompact(const std::string& fName, bool show = false);
	bool LoadFaceCascade(std::string& fname);


	void GetAppearanceParameters(float* vals) const;
	void SetAppearanceParameters(const float* vals);

	void SetAppearanceParam(uint, float);
	float GetAppearanceParam(uint) const;
	void UpdateAppearance(bool show = false);

	void GetAllParameters(float* values) const;
	void SetAllParameters(const float* values);

	void ComputeInverseUpdates(bool show = false);
	void ComputeHessian();
	void ComputeAppParameters();
	void InvertWarp();

	bool IsInTriangle(int nface, float x, float y);

	//attributes
	CvSize mMeanSize;
	IplImage* mMeanImage;
	IplImage* mCurrImage;
	IplImage* mMask;
	IplImage* mTrianglesMap;
	IplImage* mImageDiff;
    IplImage* mImageDiffSquare;
    IplImage* mWarpedImage;
	IplImage* mGradX;
	IplImage* mGradY;
	IplImage** mEigenFaces;
	IplImage** mSDImages;
	uint mNumAppearanceParameters;
	CvMat* mAppearanceParameters;
	CvMat* mAppearanceBoundaries;
	CvMat* mHessian;
	CvMemStorage* mStorage;
    ///container for the xml file trained to detect frontal faces
	CvHaarClassifierCascade* mFaceCascade;
//	CvHaarClassifierCascade* mFaceCascadeAlt;
//	CvHaarClassifierCascade* mFaceCascadeAltTree;
//	CvHaarClassifierCascade* mFaceCascadeAlt2;
	CvHaarClassifierCascade* mFaceCascadeProfile;
	CvHaarClassifierCascade* mLeftEyeCascade;
	CvHaarClassifierCascade* mRightEyeCascade;
	CvHaarClassifierCascade* mMouthCascade;
	CvHaarClassifierCascade* mNoseCascade;

	///array of images that store the x jacobian of the piece-wise affine warp (shape).
    IplImage** mDeW_deP_x;
     ///array of images that store the y jacobian of the piece-wise affine warp (shape).
    IplImage** mDeW_deP_y;

	IplImage** mJacoImagesXY;

	//These are used only to construct the model for facial features detection
	ImageVec mEigenFacesGradX;
	ImageVec mEigenFacesGradY;
	IplImage* mTempX, *mTempY, *mGradTempX, *mGradTempY;



};

}

#endif // __AppModel_h__
