
#ifndef __VisageTracker_h__
#define __VisageTracker_h__

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
#include "FbaAction.h"
#include "StaticModel.h"
#include <FBFT/FBFT.h>

using namespace std;

#ifndef WIN32
#include <pthread.h>
#include <sys/types.h>
#define HANDLE pthread_t*
#endif

namespace VisageSDK
{

class VisageTrackerObserver;

/** VisageTracker is the alternative, AAM-based facial features tracker implementation (see VisageTracker2 for the main tracker implementation).
* 
* This class implements person specific, frontal face tracking functionality (i.e., tracking the facial features in video), providing both
* real-time on-the-fly animation capability and the off-line generation of animation files,
* as well as the possibility to augment animations dynamically using the VisageTrackerObserver mechanism.
*
* The tracking is based on Active Appearance Models (AAM). An AAM is necessary for each user.
*
* The function @ref LoadDynamicModel() loads a particular appearance model into memory, making the system ready to
* track the facial movements of that particular face. The model is comprised of two files: a .shape and a .app. 
*
* @ref trackFromAvi() and trackFromCam() track the facial movements from a video file
* or from a video camera connected to the computer, the animation can be played directly in the 
* instance of the class is passed to a visageFAPlayer object or it can be encoded into a .fba and/or .fap file.
* During tracking, the VisageTrackerObserver can be used to capture the MPEG-4 FAPs FDPs (in image coordinate).
* To attach one or more observers, use the attach() method.
* This is a powerful mechanism allowing full access to the timing and animation at the same time as it 
* is being generated, and dynamic modification of the animation. See this in action in 
* <a href="../../../Samples/OpenGL/build/msvc90/FaceTracker/doc/html/index.html">Example8</a>
*
*
* VisageTracker is an implementation of FbaAction, and therefore it can be played in the FAPlayer as any other
* FbaAction. This makes it easy to obtain on-the-fly motion - initialise the player, add a VisageTracker to it,
* and call the trackFromAvi() or trackFromCam() funtion.
*
* Functions are provided to stop, pause and resume the tracking as well as controlling the visual output: 
* one can choose to see the Candide model and/or the FDPs drawn on the face being tracker.
*
* Implemented in vsvision.lib
*
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/FaceTracker/doc/html/index.html">FaceTracker sample project</a>.
*
*/
class VISAGE_DECLSPEC VisageTracker : public FbaAction
{
public:

	/** Constructor.
	*	
	*/
	VisageTracker();


	/** Destructor.
	*/
	~VisageTracker();

	/** Load the specific Active Appearance %Model into memory. 
	*
	*	The model is comprised of two files: a txt file containing the shape information
		and a binary file containing the appearance information (images).
		Currently, the two files have extension .shape and .app respectively.
		The filename passed as parameter should have no extension. If the files are not in the working directory
		the full path must be given.
		For example, if we want to load the model comprised of the files model1.shape and model1.app in C:\Data\models\
		we should pass "C:\Data\models\model1" as argument to the function.

		@param aamFileName the name of appearance model files
		@return true if successful
	*/
	bool LoadDynamicModel(const char* aamFileName);

	/** Track the facial movements from a connected digital camera
	*
	*	The tracking is applied to the video stream coming from a digital camera.
		The result can be saved in a .fba file (MPEG-FBA ecoded), just leave the parameter NULL if not necessary.
	*	
	*	@param outFbaFileName the name of the output .fba file (can be full path)
		@return true if successful
	*
	*/
	bool trackFromCam(const char* outFbaFileName = NULL);


	/** Track the facial movements of a frontal face from an AVI file
	*
	*	The resulting animation can be saved on a .fba (MPEG-FBA ecoded), leave the parameter NULL if you do not want any file to be created.	
	*	
	*	@param inAviFileName the name of the input avi file (can be full path)
	*	@param outFbaFileName the name of the output .fba file (can be full path)
		@return true if successful
	*/
	bool trackFromAvi(const char* inAviFileName,
					  const char* outFbaFileName = NULL);



	/** Pause the tracking
	*
	*	Pause the face tracking. To resume, use resume(). 
	*	In case we are tracking from a video file, the tracking will be resumed from the frame following the one during which the tracking was paused. 
	*
	* 
	*/
	void pause();
	

	/** Resume tracking.
	* 
	*	Resume tracking after a call to pause(), nothing would happen if we are currently tracking.
	*	If tracking was applied to the video stream coming from a digital camera, 
	*	the next good frame coming from the stream will be used.
	*
	*/
	void resume();
	
									

	/** Attaches an observer implementation to the tracker.
	*
	* @param _obs pointer to a VisageTrackerObserver implementation.
	* @see VisageTrackerObserver
	*/ 
	void attach(VisageTrackerObserver * _obs) {obs[nObs++] = _obs;};


	/** Detaches all attached observers from the tracker.
	* This function deactivates the observer mechanism.
	* NOTE: the observer objects are only detatched, not destroyed.
	* @see VisageTrackerObserver
	*/
	void detach() {nObs = 0;};

	/*
	*	Compute FAPs by comparing the current shape with the average (more or less neutral) face.		
		@param fbaps pointer to the resulting FBAPs object
	*/
	void GetFAPs( FBAPs* fbaps ) const;

	/*
	* Get FBAPs (FbaAction implementation).
	*
	* This function implements the FbaAction interface. Do not call it directly.
	*
	* @param globalTime the time for which the FAPs are requested, in milliseconds
	* @param lastFBAPs the final FBAPs from the previous frame of animation; in this implementation we ignore this parameter
	* @param afm the Animatable Face and Body %Model currently used in the player; in this implementation we ignore this parameter
	*/
	FBAPs *getFBAPs(long globalTime, FBAPs *lastFBAPs, VisageCharModel* model);


	/*
	* Start (FbaAction implementation).
	*
	* This function implements the FbaAction interface. Do not call it directly.
	*
	* @param globalTime the reference time when the action is started, in milliseconds
	*/
	void start(long globalTime);

	/*
	* Stop playing (FbaAction implementation).
	*
	* This function implements the FbaAction interface. Do not call it directly.
	*
	*/
	void stop();

	/*
	* Return the name of the action type (VisageTracker) (FbaAction implementation).
	*
	* This function implements the FbaAction interface. Do not call it directly.
	*
	* @return the name of the action type.
	*/
	char* actionTypeName() {return "VisageTracker";};

	/**
	* Open a window where the source video is shown.
	*
	*/
	void show();

	/**
	* Hide the window displaying the source video.
	*/
	void hide();

	/**
	* Draw the Candide model (a simple facial mesh) on the face shown in the source video, call the function again to hide the model.
	*/
	void showCandideModel();

	/**
	* Draw the MPEG-4 Facial Definition Points (feature points) on the source video, call the function again to hide the points.
	*/
	void showFDPs();

	/**
	* Set the upper and lower limit for each of Facial Animation Parameters, i.e., the maximum and minimum allowed values for
	* each of the 68 FAPs. This is to avoid the animated face to look unnatural.
	*
	* @param upperLimit pointer to the FAPs object containing the upper FAP limits
	* @param lowerLimit pointer to the FAPs object containing the lower FAP limits
	*/
	void setFAPLimits(FAPs *upperLimit, FAPs *lowerLimit);


	/**
	* Returns the pointer to the FAPs object containing the upper FAP limits
	*/
	FAPs *getUpperFAPLimits();

	/**
	* Returns the pointer to the FAPs object containing the lower FAP limits
	*/
	FAPs *getLowerFAPLimits();

	/*
	*	Compute FAPs by comparing the current shape with the average (more or less neutral) face.		
		@param fbaps pointer to the resulting FBAPs object
	void GetFAPs( FBAPs* fbaps ) const;
	*/

	
	/** Draws the (previously detected) FDPs on top of the provided image
		@param img the picture where the FDPs are to be drawn. The image should be stored in	
		a IplImage structure, provided by the Intel's OpenCV libraries.
	*/
	void DrawFDPs(IplImage* img);

private:


	bool track(const char* inAviFileName,
							  const char* outFbaFileName);

	static void  trackInThread(void* vt);
	void writeBits(FILE *streamHandle, unsigned char *bits, int size); //void writeBits(int streamHandle, unsigned char *bits, int size);
	
	//source video filename
	char source[500];

	long globalStartTime;

	//interesting FAP groups are 2,3,4,5,8 (actually 1,2,3,4,7 as the counting starts from 0)
	FBAPs *fbaps; /*!Pointer to a Face and Body Animation Parameters object*/
	FDP* fdps;/*!Pointer to a Facial Definition Point object*/

	FAPs* upperLimits;
	FAPs* lowerLimits;

	VisageTrackerObserver *obs[100];/*!<observers list*/
	int nObs; /*!number of attached observers*/
	/*!The actual appearance model*/
	AppModel* aam;

	//various flags
	bool initialised;
	bool active;
	bool display;
	bool showCandide;
	bool showfdps;
	bool paused;
	bool toFile;

	HANDLE trackingThreadHandle;/*!Handle to the thread used for tracking*/
	CvCapture* capture;/*!<Structure for the capture of video stream, from OpenCV*/

	CFBAEncoder* fbaEncoder;/*!encoder object*/
	FILE *fbaFileHandle;//int fbaFileHandle;/*!handle for the FBA file*/

	char fbaFileName[500];/*!fba file name*/
	
	unsigned char byte;
	int bitcnt;	

};

}
#endif // __VisageTracker_h__

