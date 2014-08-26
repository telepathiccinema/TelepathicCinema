
#ifndef __AAMTrainer_h__
#define __AAMTrainer_h__

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
#include "StaticModel.h"

using namespace std;

namespace VisageSDK
{

/** AAMTrainer class, can be used to train a new appearance model from sample images
*
*	To successfully build an appearance model of a new subject, several (>20) image should be placed into a directory, together
*	with the corresponding .ann files containing the manually annotated landmarks, and the candide.wmf file.
*		
*	The function @ref TrainModel() accepts as argument the path to a folder containing the training data, the extensiosele
*	of the training images, the width of the desired appearance model (a larger model will give mroe precise results 
*	during tracking but also a slower tracking and training time, do not exceed 100 pixels), and the desired name for the 
*	resulting appearance (.app) model (the corresponding .shape model is automatically saved as well).
*
*	The training images should have a correspondent .ann file, containing 31 landmarks' coordinates in pixel values (integers).
*	The first row of the .ann file should contain the number of landmarks, in our case, 31.
*
*	A set of training images (with corresponding annotations) is provided as example in the Samples\\data\\FaceTracker media folder.
*	To manually select the landmarks' locations on new training images, the TrainingTool application is available together with
*	the Visage samples.
*
*	Implemented in vsvision.lib
*
*	Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/FaceTracker/doc/html/index.html">FaceTracker sample project</a>.
*
*/
class VISAGE_DECLSPEC AAMTrainer : public AppModel{

public:

	/** Constructor.
	*	
	*	@param detector if true, a model for the facial features detector will be built, otherwise (default), a model for real time, person-specific tracking will be built
	*/
	AAMTrainer(bool detector = false);

	/** Destructor.
	*	
	*/
	~AAMTrainer(void);

	/** Train a person specific model, or a person generic one for facial features detection purposes.
	*
	*	Can take some time.
	*
	*	Given a folder with annotated training images, the function builds a statistical model of the shape and appearance
	*	of the specific user shown in the images. The fuction takes as input the path to the folder containing the data (including
	*	the candide.wfm file), the extension of the images, the width of the desired appearance model, and the name (with full path)
	*	of the resulting .app file.
	*	For each image, the corresponding .ann file (containing 31 image coordinates) is read, and the Candide model is adapted to
	*	the 31 landmarks by means of the Levenberg-Marquardt algorithm.
	*	The resulting shapes and textures extracted from the images are statistically analysed and a model of the shape (.shape)
	*	and of the appearance (.app) of the training data is built and saved, ready to be used to track the face of the user.
	*
	*	For a successfull training, the images should depict the same subject (the user), with a nearly frontal face, displaying
	*	different expressions, like the kind of expressions that one will want to track later (i.e., eyes closed, smile, surprise,
	*	anger, frown, lip pressed together, mouth wide open, etc...). The training images should also include changes in the
	*	lighting conditions and in general changes of the appearance of the person which are to be expected.
	*	The more training images are provided and with the higher resolution, the better the resulting models will be.
	*	At least 20 pictures should be used, but more than 30 are recommended.
	*
	*	
	*
	*	@param pathToTrainData path to the folder containing the training data (images, annotations and candide model).
	*	@param img_ext extension of the training images, most of the image formats are supported.
	*	@param w width of the desired appearance patch, the height is computed automatically.
	*	@param modelName path to the appearance file (.app) that will store the resulting model, the .shape file name is identical.
	*	@return code representing:
	*		 0 all fine
	*		-1 candide model not found
	*		-2 problem while loading training data
	*		 1 problem while writing model
	*	
	*/
	int TrainModel(const char* pathToTrainData, std::string& img_ext, int w, const char* modelName, bool show = false);
	
private:

	void levenbergMarquardtOptimization(CvMat *X0, CvMat *observRes,CvMat *resultX,int maxIter,double epsilon);

	void lm(CvMat *X0, CvMat *observRes,CvMat *resultX,	int maxIter,double epsilon);

	void Jac_Func(CvMat *vectX, CvMat *Jac);
	void Res_Func(CvMat *vectX,CvMat *res);

	//scan the given folder looking for images with the given extension and .ann files containing the manually annotated landmarks
	bool LoadTrainingData(const char* path, std::string& image_ext, bool show = false);

	void estimateCandideGlobalParameters(const Shape2D& dest);

	void fitUsingLM(const Shape2D& dest);

	void PrepareShapesForPCA();

	/**	Computes the mean of the shapes contained in mTrainingShapes	*/
	void CalculateMeanShape(Shape2D& mean);

	//do shape PCA
	void ShapePCA(float percentageToRetain = 0.95f, bool show = false);

	//do texture pca
	void TexturePCA(float percentageToRetain = 0.95f, bool verbose = false);

	//write the trained model to a couple of .app and .shape files, the extensions are added to the argument fname
	bool WriteToFiles(const std::string& fname);	

	IplImage* currTrainImage;

	int _numOfTrainingShapeSamples;
	int _numOfRetainedVectors;
	
	std::vector<std::string> _trainingPicsNames;

	//training shapes (converted to shape2d for easier alignment)
	std::vector<Shape2D> _trainingShapes;
	std::vector<Shape2D> _imageCoordinates;

	//mean shape 
	Shape2D* _meanShape;

	//candide model 
	CandideModel* mCandideModel;

	//resulting AAM model
	AppModel* mAppModel;
	
	//size of the reference image (square)
	CvSize _imagePatchSize;

	//if true, then a model for facial features detection is trained
	bool mTrainDetector;

	bool mFlip, mBio;

	int mNumCorr;


};

}
#endif // __AAMTrainer_h__

