
#ifndef __VisageTrackerObserver_h__
#define __VisageTrackerObserver_h__

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
#include "VisageTracker2.h"

using namespace std;

namespace VisageSDK
{

/** VisageTrackerObserver interface.

* This virtual class allows to get notification from VisageTracker2.
*
* The notification happens through the callback function Notify().
* This function is implemented by the user in a specific class derived from
* the VisageTrackerObserver class. One or more instances of this class may be attached to the VisageTracker2
* using the VisageTracker2::attach() function, and the functions implemented
* by the user are called when events occur.
*
* Notify() function is called for every frame of the video processed by the tracker except the first one which is used for initialisation.
*
* \if IOS_DOXY
* \elseif ANDROID_DOXY
* \else
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/FaceTracker2/doc/html/index.html">FaceTracker2 sample project</a>.
* \endif
*/
class VISAGE_DECLSPEC VisageTrackerObserver {
	public:

	/** Notification function.
	* This function is called by VisageTracker2 during tracking 
	* whenever a new video frame is processed. It gets the frame time stamp and it also receives the pointer to the actual tracker object
	* that can be used to retreive all other data from the tracker.
	* The function will not be called for the first frame as that frame is used for initialisation purposes.
	*
	* This mechanism is commonly used to synchronise the application with the tracker.
	*
	* @param vt instance of the VisageTracker2 that called the function; 
	* this can be used to access further information from the tracker,
	* or for identification if more than one tracker object is used at the same time.
	* @param timeStamp when tracking from camera, timeStamp returns time, in milliseconds, measured from the moment when tracking started. When tracking from video, timeStamp returns the time of the current video frame, in milliseconds. When tracking from raw images, timeStamp returns the value it received from the VisageTrackerFrameGrabber::GrabFrame() function if it is different than -1, otherwise it returns time, in milliseconds, measured from the moment when tracking started.
	*
	* @see VisageTracker2::getTrackingData(), FaceData, VisageTracker2::attach(), VisageTracker2::detach()
	*/
	virtual void Notify(VisageTracker2 *vt, long timeStamp) = 0;


};

}
#endif // __VisageTrackerObserver_h__

