
#ifndef __TrackerGUIInterface_h__
#define __TrackerGUIInterface_h__

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
#include <cv.h>

using namespace std;

namespace VisageSDK
{

class VisageTracker2;
class FDP;

/** Optional GUI Interface for VisageTracker2.
*
* When VisageTracker2 is configured for semi-automatic operation it
* requires some interaction with the user through a Graphical User Interface (GUI).
* For this purpose, the application that uses the tracker in semi-automatic modes must
* implement an instance of the TrackerGUIInterface abstract class and provide
* the necessary functions to the tracker by implementing the functions
* in TrackerGUIInterface. 
*
* All functions in this class are virtual and must be implemented in the 
* inherited class.
*
* \if IOS_DOXY
* A sample implementation of TracerGUIInterface is currently not available on iOS.
* \elseif ANDROID_DOXY
* A sample implementation of TracerGUIInterface is currently not available on Android.
* \else
* The full implementation of an TrackerGUIInterface is
* provided, (with source code) in the FaceTracker2 sample, where the
* class COpenGLWnd is an implementation of the TrackerGUIInterface.
*
* It is highly recommneded to run the FaceTracker2 sample and get familiar with all its
* functions by reading the tutorial and watching the tutorial videos in order
* to get a practical understanding of what various functions of TrackerGUIInterface do.
* \endif
*/
class VISAGE_DECLSPEC TrackerGUIInterface
{
public:
	/** A virtual function for choosing the SU file (profile) name.
	*
	* VisageTracker2 calls this function when it starts tracking 
	* from a camera. The function prompts the user to choose an existing profile
	* name (a .su file) or to enter a new profile name.
	*
	* The file name is used by the tracker to retrieve the existing profile
	* and read the face shape parameters from it, thus avoiding the user 
	* having to adjust the face shape. Alternatively, if a new profile name is given,
	* the user will have to adjust the face shape parameters and they will be stored
	* in the profile for later use.
	*
	* @return su file name, including full path (if needed) and the extension ".su"
	*/
	virtual char* ChooseSuFileName() = 0;

	/** A virtual function for displaying a text message.
	*
	* VisageTracker2 calls this function when it needs to display a simple
	* text prompt to the user. The function displays the message and
	* lets the user click "OK" to continue. Essentiylly it is
	* the same as Windows MessageBox function.
	*
	* @param userMessage message to be displayed
	* @param caption the caption to be displayed on the message window
	* @param type type of message
	*/
	virtual void displayMessage(char *userMessage,char *caption,int type) = 0;


	/** A virtual function to let the user manually adjust the face shape mask.
	*
	* VisageTracker2 automatically detects the face in the first video frame and fits the face shape
	* mask to the image. However, the automatic fitting may not be absolutely precise, so
	* at this point it is may be necessary to manually adjust the face shape mask. For this
	* purpose the tracker calls the this function.
	*
	* The function allows the user to manually adjust the face translation, rotation, scale and shape.
	* The function arguments are pointers to the parameters that need to be adjusted.
	*
	* The function may allow saving the parameters to file and reading them from a file. The
	* existing implementation in the FaceTracker2 sample does this.
	*
	* @param face_translation pointer to face translation (array of three floats)
	* @param face_rotation pointer to face rotation (array of three floats)
	* @param face_scale pointer to face translation (a single float value)
	* @param face_shape pointer to face shape parameters (array of 34 floats \if IOS_DOXY \elseif ANDROID_DOXY \else , see implementation in FaceTracker2 sample for the meaning of parameters \endif )
	*/
	virtual void ManuallyAdjustShapeMask(float *face_translation, float *face_rotation, float *face_scale, float *face_shape) = 0;

	/** A virtual function to prompt the user to signal when (s)he is ready to start tracking.
	*
	* VisageTracker2 calls this function on two separate occassions while tracking from camera: first to prompt the user
	* to either make a neutral, front-facing face and second, to prompt the user to align his/her face with the
	* face shape mask displayed on the screen.
	*
	* A slight complication is that the tracker needs to run while waiting for the user to respond
	* to the prompt by clicking "OK. This is because the tracker must keep updating
	* the image, otherwise the user would not see what (s)he is doing. The solution is
	* that the function spawns a new thread, then returns control to the tracker. In the
	* newly created thread the user is prompted. The tracker gets the signal when the user
	* clicked "OK" through the readyFlag boolean value.
	*
	* @param userMessage the prompt message to be shown to the user
	* @param readyFlag the pointer to a flag that is set to false prior to calling the function; the function sets it to true when user clicks "OK"
	*/
	virtual void signalWhenUserReady(char *userMessage,bool *readyFlag) = 0;

	/**
	*	A virtual function to determine positions of main facial features (eye centres, mouth corners and nose tip) from a still image containing a face.
	*
	* This function is called by VisageTracker2 when the manual_face_detection tracker configuration parameter is set to 1. The function
	* replaces the tracker's automatic face and main features detection. It should present the input image to the user, let the user
	* pick eye centres, nose tip and mouth corners, and return the coordinates picked by the user in the output FDP object. Alternatively, 
	* the function can implement an algorithm to automatically detect the required features.
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
	* The function must determine positions of left eye centre (feature point 3.6), right eye centre (feature point 3.5), left mouth corner (feature point 8.4),
	* right mouth corner (feature point 8.3) and tip of the nose (feature point 9.3).
	*
	* The return value is a bit-wise indicator
	* of which features were succesfully determined. Starting from the least significant bit, the first bit indicates 
	* the face, second the eyes, third bit the mouth and fourth bit the nose. Thus return value 0 means that no face was determined, 1 means
	* the face was determined but not any features, 2 means eyes were determined but not nose and mouth, 11 means nose and eyes were determined but not mouth, etc.
	* The value 15 means that face and all features were determined.
	*
	* @param input the input image.
	* @param output pointer to the FDP object in which the results will be returned.
	* @return code representing the result of the detection, see above 
	*
	* @see FDP
	*/	
	virtual int selectMainFacialFeatures(IplImage* input, FDP* output) = 0;

	/** A pointer to the VisageTracker2 instance to which this TrackerGUIInterface is attached.
	*
	*/
	VisageTracker2 *vt2;
};

}
#endif // __TrackerGUIInterface_h__

