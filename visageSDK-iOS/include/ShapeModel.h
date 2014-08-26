
#ifndef __ShapeModel_h__
#define __ShapeModel_h__

#include "Shape2D.h"
#include "Vertex.h"
#include <vector>
#include "FDP.h"
#include "FBAPs.h"
#include "CandideModel.h"

namespace VisageSDK
{

/**	
	The Shape %Model class represents the shape of the face as a 2D vector. 
	The shape model is created from a set of annotated training images using the Principal Components Analysis technique.
	Different facial configurations	are obtained by linear combination of an average shape and a set of shape vectors,
	each representing one of the principal components of the shape deformations observed during training.

	There are two kinds of shape vectors: the ones that control the local deformations and the ones
	that control the global motion, i.e., translation, in-plane rotation and scaling. 
	Two set of parameters make it possible to control respectively the local and global deformation od the shape.

	The internal representation of the shapes is done through objects of the @ref Shape2D class. 
*/
class VISAGE_DECLSPEC ShapeModel
{
public:
	/*	Constructor
	*/
	ShapeModel(void);
	/*	Destructor
	*/
	~ShapeModel(void);

	
	#pragma region Get/Set methods

	/** Returns the mean shape*/
	inline Shape2D& GetMeanShape() {return mMeanShape;} 

	/** Returns the current shape (after only the local deformations have been aplied to the mean shape,
		i.e., before any similarity transform)
	*/
	inline Shape2D& GetCurrentShape() {return mCurrentShape;} 


	/** Returns the global shape (i.e., after having applied the similarity transform to currentshape)
	*/
	inline Shape2D& GetGlobShape() {return mGlobalShape;} 


	/** Returns a reference shape: basically the mean shape translated in order to fit the standard image 
		patch containing the appearance
	*/
	inline Shape2D& GetReferenceShape() {return mReferenceShape;} 


	/** Returns the number of points in the shape*/
	inline uint GetNumPoints() const {return mMeanShape.GetNumPoints();}


	/** Returns the number of triangles (faces) in the shape*/
	inline uint GetNumTriangles() const { return static_cast<uint>(mTriangles.size()) ;}

	/*	Retrieves a specific shape vector 
	*	@param i index number of the desired shape vector 
		@returns a reference to the selected shape vector
	*/
	Shape2D& GetShapeVector(uint i);

	/**	Set a specific shape vector
	*	@param i index number of the desired shape vector
	*	@param vect reference to the new shape vector
	*/
	void SetShapeVector(uint i, const Shape2D& vect);

	/**	Returns the number of shape parameters (modes of shape variation)
	*	@returns the number of modes
	*/
	inline uint GetNumShapeParameters() const {return static_cast<uint>(mShapeVectors.size());} 

	/** Returns a specific shape parameter value
	*	@param i index number of the desired shape parameter 
	*	@returns The desired value
	*/
	float GetShapePar(uint i) const;

	/**	Sets a specific shape parameter value, constraints are applied
	*	@param i index of the desired shape parameter
	*	@param val the new value
	*/
	void SetShapePar(uint i, float val);

	/** Sets all the shape parameters (SetShapePar is called), NO CHECK IS PERFORMED ON THE PASSED ARRAY
	*	@param val array of desired values	
	*/
	void SetShapeParameters(const float* val);

	/** Get all the shape parameters
	*	@param val array stroing the resulting values	
	*/
	void GetShapeParameters(float* val) const;

	/** Sets to zero all the (local) shape parameters
	*	
	*/
	void ClearShapeParameters();

	/** Sets to zero all the (local) shape parameters
	*	@param i index
		@returns the (absolute) boundary value
	*/
	float GetShapeParBoundary(uint i) const;

	/** Sets a specific shape parameter's boundary (absolute value)
	*	@param i index
		@param val the desired boundary value (usually set to 3.0*(standard deviation))
	*/
	void SetShapeParBoundary(uint i, float val);

	/**	Gets the rotation parameter
	*	@return The shape's (in-plane) rotation
	*/
	inline float GetRotation() const {return mGlobalParameters[3];}

	/** Sets the (in plane) rotation parameter
		@param r the rotation value
	*/
	inline void SetRotation(float r) {
		
		mGlobalParameters[3] = r;
		mGlobParModified = true; 

	}

	/** Gets the scale parameter 
	*	@returns the scaling parameter
	*/
	inline float GetScale() const {return mGlobalParameters[2];}

	/** Sets the scale parameter, within the boundaries
	*	@param sc the new scaling factor
	*/
	inline void SetScale(float sc) {
		mGlobalParameters[2] = MIN(mScaleUpperBound, MAX(mScaleLowerBound, sc)); 
		mGlobParModified = true;
	}

	/** Sets the scale parameter's boundaries, calculating them from the size of an image. 
		This will keep the model whitin reasonable size provided the input image.
	*	@param imgWidth width of the image
		@param imgHeight height of the image
	*/
	void SetScaleBoundaries(int imgWidth, int imgHeight);

	/** Gets the translation parameters
		@param x pointer to the x translation value
		@param y pointer to the y translation value
		
	*/
	inline void GetTranslation(float* x, float* y) const { *x = mGlobalParameters[0]; *y = mGlobalParameters[1];}

	/** Sets the translation parameters
	*	@param x the new x translation value
		@param y the new y translation value
	*/
	inline void SetTranslation(const float& x, const float& y){ 
		mGlobalParameters[0] = x; 
		mGlobalParameters[1] = y; 
		mGlobParModified = true; 
	}

	/** Sets the translation parameters, ints as arguments
	*	@param x the new x translation value
	*	@param y the new y translation value
	*/
	inline void SetTranslation(const int& x, const int& y){
		mGlobalParameters[0] = static_cast<float>(x);
		mGlobalParameters[1] = static_cast<float>(y);
		mGlobParModified = true;
	}

	/** Gets the global parameters, NO CHECK PERFORMED ON THE ARRAY 
	*	@param p the array where the global parameters' values will be stored
	*/
	void GetGlobalParameters(float* p) const;

	/** Sets the global parameters, NO CHECK PERFORMED ON THE ARRAY
		@param p the array containing the four global parameters to be set
	*/
	void SetGlobalParameters(const float* p);

	/** Gets a single global parameter value
	*	@param i the index value
		@returns the desired parameter value
	*/
	float GetGlobalPar(uint i) const;

	/** Sets a single global parameter value</summary>
		@param i the index value
		@param val the new value
	*/
	void SetGlobalPar(uint i, float val);

	/** Sets all the global parameters to zero (one for scaling)
	*/
	void ClearGlobalParameters();

	/** Sets all shape parameters to zero
	*/
	void SetAllShapeParameters(const float* vals);

	/** Gets all shape parameters 
		@param vals pointer to store the retrieved values
	*/
	void GetAllShapeParameters(float* vals) const;




#pragma endregion



	/**
		Draws the shape on a image using the selected color
		@param img the image 
		@param c1 r channel value
		@param c2 b channel value
		@param c3 g channel value
		@param mean if true the reference shape is drawn instead of the global one
	*/
	void Draw(IplImage* img, unsigned int c1 = 255, unsigned int c2 = 255, unsigned int c3 = 255, bool mean = false) const;

	/**
		Draws the shape on an image having different size as the original one using the selected color
		@param img the resized image 
		@param w the original image's width 
		@param h the original image's height 
		@param c1 r channel value
		@param c2 b channel value
		@param c3 g channel value
	*/
	void ReDraw(IplImage* img, int w, int h, unsigned int c1, unsigned int c2, unsigned int c3) const;


	/**
		Fills the shape object with data read from file or gathered by training
	*/
	void FillShape(int nPoints, int nVecs, const Shape2D& mean, int numFaces, const FaceVec& fv);


protected:


	/** Initialize the shape model by allocating all the needed memory.
		@param numPoints number of points
		@param numTriangles number of triangles
		@param numParameters number of shape parameters 
	*/
	void Init(uint numPoints, uint numTriangles, uint numParameters);

	/** Initialize the four global shape vectors. 

	*/
	void InitGlobalShapeVectors();

	
	

public:
	/**  
		Performs the update of the shape, applying the current shape parameters (local transformation)
		@returns false if the shape parameters have not been modified from the last update
	*/
	bool UpdateShape();

	/** Performs the update of the shape, applying the current global parameters (global transformation)
		@returns false if the global parameters have not been modified from the last update
	*/
	bool UpdateGlobal();

	/** Initialize the reference model by translating and scaling thew
		reference shape in order to fit the face in a predefined image patch,
		such patch will be the size of the appearance model.
		@param w the width of the appearance patch, the height is automatically computed

	*/
	void InitializeReferenceModel(int w, int h, bool detector = false);

	/** Precompute the part of the warp jacobian related to the global warp,
	*/
	void PreComputeGlobalWarpJacobian();
	void PreComputeWarpJacobian();

	/** Computes the shape parameters needed to warp the part of the input image under 
		the global shape onto the reference shape
		@param train needs to be set to true in case we are training a model
	*/
	void ComputeWarpParameters(bool train = false);

	/** 
		Save the shape model to a text file, the .shape extension is added automatically to
		the given name, passed as argument
		@returns True if the complete model could be written, false otherwise 
	*/
	bool WriteToTxtFileShape(const std::string& fName) const;



	/** 
		Used for training, it creates the image patch that will be used to construct the appearance model.
		Given the desired width, the height is computed from the average shape.
		@returns the computed height of the image patch
	*/
	int CreateReferenceModel(int w, bool detector = false);

	/**The current global shape, after the application of the global 2D similarity transform to mCurrentShape*/
	Shape2D mGlobalShape;

protected:
	/** Performs the update of the refernce shape shape, applying the provided scaling and translation
		@param x the desired x translation
		@param y the desired y translation
		@param scale desired scaling factor
	*/
	void UpdateReferenceCoords(float x, float y, float scale = 1.0f);
	
	
	/** Computes the global parameters needed to warp the part of the input image under 
		the global shape onto the reference shape
	*/
	void ComputeGlobalWarpParameters();
	
	
	void ComputeGlobalWarpParametersForInversion(Shape2D& newShape);

	void WarpForInversion(const Shape2D& source, Shape2D& dest);

	/** Read a shape model from a txt file, with .shape extension.
		@param fName the desired filename (fullpath)
		@returns true in case all the file could be read
	*/
	bool ReadShapeFile(const char *fName);

	int mNumPoints;
	int mNumShapeParameters;
	int mNumTriangles;

	/**mean shape, never changes*/
	Shape2D mMeanShape;

	/**vector of triangles: each contains the vertexes indexes*/
	std::vector<v3i> mTriangles;

	/**An int for each triangle telling its orientation*/
	int* mTriangleOrientation;

	/**The current face shape, driven by the shape parameters*/
	Shape2D mCurrentShape;


	/**Stores the point coordinates of the shape model aligned on the reference image*/
	Shape2D mReferenceShape;

	/**Vector of shapes representing the "eigenshapes" or shape vectors*/
	std::vector<Shape2D> mShapeVectors;
	/**Vector of shapes, representing the global similarity transform*/
	std::vector<Shape2D> mGlobalTransformVectors;

	/**Array of shape parameters, controlling the local shape of the object*/
	CvMat* mShapeParameters;

	/**Absolute values of the boundaries inside which the shape parameters are contrained*/
	CvMat* mShapeParBoundaries;

	/**Global affine transform parameters*/
	float* mGlobalParameters;

	/**True if the shape parameters have been modified*/
	bool mShapeParModified;

	/**True if the global parameters have been modified*/
	bool mGlobParModified;
	
	/**Upper bound of scale parameter (related to the reference image size and the model size)*/
	float mScaleUpperBound;
	/**Lower bound of scale parameter (related to the reference image size and the model size)*/
	float mScaleLowerBound;

	float mScalem, mTraslYm, mTraslXm;

	CvMat* mShapeParamUpdates;

	float *mX0i,*mX0j,*mX0k,*mY0i,*mY0j,*mY0k;
	float *mDet;
	float *mTmp1, *mTmp2, *mTmp3;

	float mXi, mXj, mXk, mYi, mYj, mYk; 
	float mDetG;
    float mTemp1G, mTemp2G;

	float *mWP1,*mWP2,*mWP3,*mWP4,*mWP5,*mWP6;
	float *mWP1_2,*mWP2_2,*mWP3_2,*mWP4_2,*mWP5_2,*mWP6_2;

	float mGlobWP1,mGlobWP2,mGlobWP3,mGlobWP4,mGlobWP5,mGlobWP6;

	float* mGlobalNormalizingValues;

	float mMeanShapeWidth, mMeanShapeHeight;

};

}

#endif // __ShapeModel_h__
