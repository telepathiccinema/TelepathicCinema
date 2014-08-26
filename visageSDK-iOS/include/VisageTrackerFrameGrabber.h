
#ifndef __VisageTrackerFrameGrabber_h__
#define __VisageTrackerFrameGrabber_h__

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

using namespace std;

namespace VisageSDK
{

#define VISAGE_FRAMEGRABBER_FMT_RGB 0
#define VISAGE_FRAMEGRABBER_FMT_BGR 1 
#define VISAGE_FRAMEGRABBER_FMT_LUMINANCE 2
#define VISAGE_FRAMEGRABBER_FMT_RGBA 3
#define VISAGE_FRAMEGRABBER_FMT_BGRA 4

#define VISAGE_FRAMEGRABBER_ORIGIN_TL 0
#define VISAGE_FRAMEGRABBER_ORIGIN_BL 1

/** VisageTrackerFrameGrabber interface.

* This virtual class provides the raw image memory interface to VisageTracker2, allowing the application to pass images directly to the tracker.
* The application that needs to pass raw images to the tracker must
* implement an instance of the VisageTrackerFrameGrabber class. Specifically, the
* GrabFrame() function is called for every frame of the video processed by the tracker.
* To associate the input frames with the output tracking data the derived class can also implement a timestamping mechanism, or simply set the timeStamp to -1 if there is no need for such mechanism.
*
* \if IOS_DOXY
* \elseif ANDROID_DOXY
* \else
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/FaceTracker2/doc/html/index.html">FaceTracker2 sample project</a>.
* \endif
*/
class VISAGE_DECLSPEC VisageTrackerFrameGrabber {
	public:

		/** Frame grabbing function (virtual - to be implemented by the application).
		* When tracking is started using VisageTracker2::trackFromRawImages(), the GrabFrame() function is called by VisageTracker2 during tracking 
		* whenever a new video frame is required. 
		* The application must implement this function, and the function must return the pointer to the latest video frame data.
		* The size and format of the video frame data is defined in the call to VisageTracker2::trackFromRawImages() that starts the tracking.
		*
		* The mechanism works as follows:
		* - The application calls VisageTracker2::trackFromRawImages() and passes it an instance of VisageTrackerFrameGrabber.
		* - VisageTracker2::trackFromRawImages() starts tracking in a new thread and returns immediately.
		* - During tracking, the tracker repeatedly calls VisageTrackerFrameGrabber::GrabFrame() to obtain video frames (images) and perform tracking on those images. If NULL is returned, tracking stops.
		* - Tracking results can be obtained using all mechanisms explained in the documentation of VisageTracker2 class.
		*
		* @param timeStamp The timestamp of the the input frame. The passed value will be returned with the tracking data for that frame (FaceData::timeStamp). Alternatively, the value of -1 can be passed, in which case the tracker will return time, in milliseconds, measured from the moment when tracking started. If a VisageTrackerObserver is used, the timeStamp is also passed through VisageTrackerObserver::Notify() method.
		* @return pointer to image pixel data; if NULL is returned, the tracker stops.
		*
		* @see VisageTracker2::trackFromRawImages(), VisageTracker2::getTrackingData(), FaceData
		*/
	virtual unsigned char *GrabFrame(long &timeStamp) = 0;

};

}
#endif // __VisageTrackerFrameGrabber_h__

