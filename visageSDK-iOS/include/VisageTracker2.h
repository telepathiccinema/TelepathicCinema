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


#ifndef __VisageTracker2_h__
#define __VisageTracker2_h__

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
#include "FAPs.h"
#include "FBFT/FBFT.h"

#include "TrackerOpenGLInterface.h"
#include "TrackerGUIInterface.h"
#include "TrackerInternalInterface.h"
#include "SmoothingFilter.h"
#include "VisageTrackerObserver.h"
//#include "VisageFeaturesDetector.h"
#include "VisageTrackerFrameGrabber.h"
#include "FaceData.h"
#include "VisageDetector.h"
#include "VisageTrackerCore.h"
#include "SimpleTrackerCore.h"
#include "PoseEstimator.h"
#include "Definitions.h"


#ifdef WIN32
class videoInput;
#endif
#ifdef IOS
namespace VisageSDK
{
class iOSCapture;
}
#endif

#ifdef MAC_OS_X
namespace VisageSDK
{
	class OSXCapture;
}
#endif

#ifdef ANDROID
extern "C" {
	VISAGE_DECLSPEC void _loadVisageVision();
}
#endif

#ifndef WIN32
#include <pthread.h>
#include <sys/types.h>
#define HANDLE pthread_t*
#endif


namespace VisageSDK
{

#define TRACK_STAT_OFF 0
#define TRACK_STAT_OK 1
#define TRACK_STAT_RECOVERING 2
#define TRACK_STAT_INIT 3

#define VISAGE_CAMERA_UP 0
#define VISAGE_CAMERA_DOWN 1
#define VISAGE_CAMERA_LEFT 2
#define VISAGE_CAMERA_RIGHT 3

class ModelFitter;
class PoseEstimator;
class FrameGrabberInternal;

/** VisageTracker2 is a face tracker capable of tracking the head pose, facial features and gaze in video coming from a
* video file, camera or other sources.
*
* The tracker is fully configurable through comprehensive tracker configuration files. visage|SDK contains optimal configurations 
* for common uses such as head tracking and facial features tracking.
* Please refer to the <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for the list of available configurations.
* \if IOS_DOXY
* Please read more details about configuration selection in the section <a href="../../doc/creatingxc.html#config_selection">Device-specific configuration selection</a>
* \endif
*
* The <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> provides full detail
* on all available configuration options, allowing to customize the tracker in terms of performance, quality, tracked features, range
* of tracked facial actions and other options and produce in effect a variety of different trackers suited for specific applications.
*
* \if ANDROID_DOXY
* trackFromCam() performs face tracking from a video camera.
* Face tracking from raw image input is enabled by track() and allows tracking
* from any source. Due to instability of low-level camera input on some devices, the raw image input is currenty the recommended way to implement
* tracking from camera. The sample project <a href="../tracker.html">VisageTrackerDemo</a> shows how to do this, with full source code.
* \else
* trackFromVideo() and trackFromCam() perform face tracking from a video file
* or from a video camera.
* Tracking from raw image input is enabled by track() and allows tracking
* from any source.
* \endif
*
* The tracker offers the following outputs, available through method getTrackingData():
* - 3D head pose,
* - facial expression,
* - gaze direction,
* - eye closure,
* - facial feature points,
* - full 3D face model, textured.
* 
*
* The tracker can apply a smoothing filter to tracking results to reduce the inevitable tracking noise. Smoothing factors 
* are adjusted separately for global face rotation, translation and different parts of the face. The smoothing settings 
* in the supplied tracker configurations are adjusted conservatively to avoid delay in tracking response, yet provide 
* reasonable smoothing. For further details please see the smoothing_factors parameter array in the 
* <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>.
*
* By default, the functions trackFromVideo() and trackFromCam() start tracking in a separate thread and immediately return control. 
* This means that the VisageTracker2::getTrackingData() function can
* be called at any time to interrogate the tracker and get results. This is the simplest
* way to obtain the results when tracking from camera or video file.  
*
* To track from other video sources (e.g. custom frame grabbers, series of images), video frames (images) are passed sequentially to the track() method,
* which immediately returns results for the given frame.
* 
* \if IOS_DOXY
*
* Furthermore, the VisageTrackerObserver can be used to obtain the tracking results. This is particularly useful for synchronisation.

* To attach one or more observers, use the attach() method. The VisageTrackerObserver::Notify() function
* shall then be called by the tracker after each processed video frame (even if the face was not found in the current frame).
*
* \elseif ANDROID_DOXY
*
* Furthermore, the VisageTrackerObserver can be used to obtain the tracking results. This is particularly useful for synchronisation.

* To attach one or more observers, use the attach() method. The VisageTrackerObserver::Notify() function
* shall then be called by the tracker after each processed video frame (even if the face was not found in the current frame).
*
* \else Additional mechanisms for obtaining outputs are provided for
* convenience:
* - VisageTrackerObserver: During tracking, the VisageTrackerObserver can be used to obtain the tracking results.
* To attach one or more observers, use the attach() method. The VisageTrackerObserver::Notify() function
* shall then be called by the tracker after each processed video frame (even if the face was not found in the current frame).
* This is particularly useful for synchronisation.
* In the FaceTracker2 sample this mechanism is used to render the facial animation on a face model based on tracking
* results.
* - Writing into an FBA file: The tracking results in form of MPEG-4 Face and Body Animation Parameters can be saved in a .fba file (MPEG-FBA ecoded).
* See @ref trackFromVideo() and @ref trackFromCam() for details.
* 
* \endif
*
* The tracker requires a set of data and configuration files, available in Samples/data
*
* Please either copy the complete contents of this folder into your application's working folder, or consult <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for detailed settings.
*
* The tracker also provides an auxiliary method DetectStrip() that can be used to detect a standard magnetic card stripe if visible in the image and return its size to be used as reference for the size of other
* objects in the image.
*
*/
class VISAGE_DECLSPEC VisageTracker2 : public FbaAction
{
public:
	/** Constructor.
	*
	* @param trackerConfigFile the name of the tracker configuration file (.cfg; default configuration files are provided in Samples/data folder.
	* Constructor must be called with the full path to the configuration file. The configuration file is in the same directory as other data files needed by the tracker. Because it is in the same directory its path can be parsed to set the path for the other data files.
	* For further details see <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>\if IOS_DOXY and section on <a href="../../doc/creatingxc.html#config_selection">device-specific configuration selection</a>.\endif).
	*/
	VisageTracker2(const char* trackerConfigFile);

	/** Destructor.
	*/
	virtual ~VisageTracker2();

	/** Perform face tracking on video input from a digital camera \if ANDROID_DOXY (note: due to instability of low-level camera access on some devices it is currently recommended to use track()) \endif.
	*
	* The function opens a camera video stream and performs face tracking (assuming that there is a face in the video).
	* If no camera is available or if it fails opening the camera video stream, the function
	* returns false. Choice between multiple cameras is made using the camera_device parametar in the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration file</a> or the pickCamDevice argument.
	*
	* If parameter startThread is true, the tracking is started in a separate thread so this function returns immediately and the
	* function getTrackingData() can be used
	* to obtain the tracking results at any time. For more details about the available
	* mechanisms for obtaining the tracking results see the VisageTracker2 main documentation text.
	*
	* The resulting animation can be saved in a .fba file (MPEG-FBA encoded); If outFbaFileName is NULL, no file will be created.
	*
	* \if IOS_DOXY
	* It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
	* functions in order to get a practical understanding of the functioning of the tracker.
	* \elseif ANDROID_DOXY
	* It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
	* functions in order to get a practical understanding of the functioning of the tracker.
	* \else
	* It is  recommended to run the FaceTracker2 sample and get familiar with all its
	* functions by reading the tutorial in order
	* to get a practical understanding of the functioning of the tracker.
	* \endif
	*
	* @param outFbaFileName the name of the output .fba file (can be full path). If NULL, the .fba file is not produced.
	* @param orientation camera orientation, VISAGE_CAMERA_UP (default), VISAGE_CAMERA_DOWN (camera turned upside-down), VISAGE_CAMERA_LEFT (camera rotated left), VISAGE_CAMERA_RIGHT (camera rotated right)
	* @param frameGrabberImageFormat image format of camera frame, used only on Android. Format can be one of the following:
	* - VISAGE_FRAMEGRABBER_FMT_RGB: each pixel of the image is represented by three bytes representing red, green and blue channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_BGR: each pixel of the image is represented by three bytes representing blue, green and red channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_RGBA: each pixel of the image is represented by four bytes representing red, green, blue and alpha (ignored) channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_BGRA: each pixel of the image is represented by four bytes representing blue, green, red and alpha (ignored) channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_LUMINANCE: each pixel of the image is represented by one byte representing the luminance (gray level) of the image.
	* @param startThread flag that indicates whether the tracking should be started in a separate thread. 
	* @param pickCamDevice camera device number. If set to -1, camera_device number from the tracker configuration file is used. If pickCamDevice is set to 0 or higher, this number overrides
	* the camera_device number from the configuration file. The mapping of physical cameras to camera device numbers is hardware-dependent and should be determined experimentaly for a specific computer or device.
	* @param camWidth requested camera frame width. If set to -1, camera_width value from the tracker configuration file is used. If camWidth is set to 0 or higher, this value overrides
	* the camera_width value from the configuration file. Actual value used will depend on camera driver support.
	* @param camHeight requested camera frame height. If set to -1, camera_height value from the tracker configuration file is used. If camHeight is set to 0 or higher, this value overrides
	* the camera_height value from the configuration file. Actual value used will depend on camera driver support.
	*
	* @return true if successful, false if tracking could not start
	*
	* @see getTrackingData(), FaceData, trackFromVideo(), track()
	*/
	bool trackFromCam(const char* outFbaFileName = NULL, int orientation = VISAGE_CAMERA_UP, int frameGrabberImageFormat = VISAGE_FRAMEGRABBER_FMT_RGB, bool startThread = true, int pickCamDevice = -1, int camWidth = -1, int camHeight = -1);
	
	/** \ifnot ANDROID_DOXY */
	/** Perform face tracking on a video file.
	*
	* The function opens the video file and performs face tracking (assuming that there is a face in the video).
	*
	* The tracking is started in a separate thread so this function returns immediately and
	* function getTrackingData() can be used
	* to obtain the tracking results at any time. For more details about the available
	* mechanisms for obtaining the tracking results see the VisageTracker2 main documentation text.
	*
	* The resulting animation can be saved in a .fba file (MPEG-FBA encoded); if outFbaFileName is NULL, no file will be created.
	*
	* \if IOS_DOXY
	* It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
	* functions in order to get a practical understanding of the functioning of the tracker.
	* \elseif ANDROID_DOXY
	* It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
	* functions in order to get a practical understanding of the functioning of the tracker.
	* \else
	* It is  recommended to run the FaceTracker2 sample and get familiar with all its
	* functions by reading the tutorial in order
	* to get a practical understanding of the functioning of the tracker.
	* \endif
	*
	* @param inVideoFileName the name of the input video file (can be full path). It is recommended to use a DIVX encoded AVI file on Windows and MP4 or Quicktime MOV file on iOS since these are the formats this method has been tested on.
	* @param outFbaFileName the name of the output .fba file (can be full path). If NULL, the .fba file is not produced.
	* @return true if successful
	*
	* @see getTrackingData(), FaceData, trackFromCam(), trackFromRawImages()
	*/
	bool trackFromVideo(const char* inVideoFileName,
					  const char* outFbaFileName = NULL);

	/** \endif */

	/**
	* Performs face tracking in the given image and returns tracking results and status.
	* This function should be called repeatedly on a series of images in order to perform continuous tracking.  
	*   
	* If the tracker needs to be initialized, this will be done automatically before tracking is performed on the given image. 
	* Initialization means loading the tracker configuration file, required data files and allocating various data buffers to the given image size. 
	* This operation may take several seconds.
	* This happens in the following cases:
	*   - In the first frame (first call to track() function).
	*   - When frameWidth or frameHeight are changed, i.e. when they are different from the ones used in the last call to track() function.
	*   - If setTrackerConfigurationFile() function was called after the last call to track() function.
	*  
	*  The tracker results are returned in faceData.
	*  
	* @param frameWidth Width of the frame
	* @param frameHeight Height of the frame
	* @param p_imageData Pointer to image pixel data; size of the array must correspond to frameWidth and frameHeight
	* @param facedata FaceData instance that will receive the tracking results. No tracking results will be returned if NULL pointer is passed. 
	* On first call of this function, the memory for the required member variables of the passed FaceData object will be allocated and initialized automatically.
	* @param format Format of input images passed in p_imageData. It can not change during tracking. Format can be one of the following:
	* - VISAGE_FRAMEGRABBER_FMT_RGB: each pixel of the image is represented by three bytes representing red, green and blue channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_BGR: each pixel of the image is represented by three bytes representing blue, green and red channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_RGBA: each pixel of the image is represented by four bytes representing red, green, blue and alpha (ignored) channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_BGRA: each pixel of the image is represented by four bytes representing blue, green, red and alpha (ignored) channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_LUMINANCE: each pixel of the image is represented by one byte representing the luminance (gray level) of the image.
	* @param origin Origin of input images that will be passed in p_imageData. It can not change during tracking. Format can be one of the following:
	* - VISAGE_FRAMEGRABBER_ORIGIN_TL: Origin is the top left pixel of the image. Pixels are ordered row-by-row starting from top left.
	* - VISAGE_FRAMEGRABBER_ORIGIN_BL: Origin is the bottom left pixel of the image. Pixels are ordered row-by-row starting from bottom left.
	* @param widthStep Width of the image data buffer, in bytes.
	* @param timeStamp The timestamp of the the input frame. The passed value will be returned with the tracking data for that frame (FaceData::timeStamp). Alternatively, the value of -1 can be passed, in which case the tracker will return time, in milliseconds, measured from the moment when tracking started. If a VisageTrackerObserver is used, the timeStamp is also passed through VisageTrackerObserver::Notify() method.
	* @returns tracking status (TRACK_STAT_OFF, TRACK_STAT_OK, TRACK_STAT_RECOVERING and TRACK_STAT_INIT, see @ref FaceData for more details)
	*
	* @see FaceData, trackFromCam(), trackFromVideo()
	*/
	virtual int track(int frameWidth, int frameHeight, const char* p_imageData, FaceData* facedata, int format = VISAGE_FRAMEGRABBER_FMT_RGB, int origin = VISAGE_FRAMEGRABBER_ORIGIN_TL, int widthStep = 0, long timeStamp = -1);

	/* DEPRECATED, This function is deprecated and replaced with track()
	*
	*
	* 
	* Track the face and facial features in images passed from the application using the raw image interface (VisageTrackerFrameGrabber)
	* 
	* The tracking is started in a separate thread so this function returns immediately and
	* function getTrackingData() can be used
	* to obtain the tracking results at any time. For more details about the available
	* mechanisms for obtaining the tracking results see the VisageTracker2 and FaceData main documentation text.
	*
	* The application must pass the video frames (raw video images) to the tracker using the VisageTrackerFrameGrabber
	* interface. The VisageTrackerFrameGrabber object is passed as an argument to this function. The tracker then calls
	* VisageTrackerFrameGrabber::GrabFrame() periodically to obtain new video frames in which it will perform tracking.
	*
	* The resulting animation can be saved in a .fba file (MPEG-FBA encoded); leave the parameter outFbaFileName NULL if you do not want any file to be created.
	* 
	* 
	* \if IOS_DOXY
	* It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
	* functions in order to get a practical understanding of the functioning of the tracker.
	* \elseif ANDROID_DOXY
	* It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
	* functions in order to get a practical understanding of the functioning of the tracker.
	* \else
	* It is  recommended to run the FaceTracker2 sample and get familiar with all its
	* functions in order to get a practical understanding of the functioning of the tracker.
	* \endif
	*
	* @param frameGrabber The frame grabber interface object - an implementation of the VisageTrackerFrameGrabber class.
	* @param width Width of input images that will be passed to the tracker by VisageTrackerFrameGrabber::GrabFrame(). It can not change during tracking.
	* @param height Height of input images that will be passed to the tracker by VisageTrackerFrameGrabber::GrabFrame(). It can not change during tracking.
	* @param format Format of input images that will be passed to the tracker by VisageTrackerFrameGrabber::GrabFrame(). It can not change during tracking. Format can be one of the following:
	* - VISAGE_FRAMEGRABBER_FMT_RGB: each pixel of the image is represented by three bytes representing red, green and blue channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_BGR: each pixel of the image is represented by three bytes representing blue, green and red channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_RGBA: each pixel of the image is represented by four bytes representing red, green, blue and alpha (ignored) channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_BGRA: each pixel of the image is represented by four bytes representing blue, green, red and alpha (ignored) channels, respectively.
	* - VISAGE_FRAMEGRABBER_FMT_LUMINANCE: each pixel of the image is represented by one byte representing the luminance (gray level) of the image.
	*	@param origin Origin of input images that will be passed to the tracker by VisageTrackerFrameGrabber::GrabFrame(). It can not change during tracking. Format can be one of the following:
	* - VISAGE_FRAMEGRABBER_ORIGIN_TL: Origin is the top left pixel of the image. Pixels are ordered row-by-row starting from top left.
	* - VISAGE_FRAMEGRABBER_ORIGIN_BL: Origin is the bottom left pixel of the image. Pixels are ordered row-by-row starting from bottom left.
	*	@param outFbaFileName the name of the output .fba file (can be full path). If NULL, the .fba file is not produced.
	*	@return true if successful
	*
	* @see VisageTrackerFrameGrabber, getTrackingData(), FaceData, trackFromCam(), trackFromVideo()
	*/
	bool trackFromRawImages(VisageTrackerFrameGrabber *frameGrabber,
							int width, int height, int format, int origin,
							const char* outFbaFileName = NULL);

	/** Get face data and status.
	* 
	* This method fills the given FaceData structure and returns the tracking status.
	*
	* On first call of this function, the memory for the required member variables of the passed FaceData object will be allocated and initialized automatically.
	*
	* @param data FaceData structure that will be filled with current face data
	* @return tracking status (TRACK_STAT_OFF, TRACK_STAT_OK, TRACK_STAT_RECOVERING and TRACK_STAT_INIT, see @ref FaceData for more details)
	*
	* @see FaceData
	*/
	int getTrackingData(FaceData *data);

	/** Set configuration file name.
	*
	* The tracker configuration file name is set and this configuration file will be used for next tracking session (i.e. when trackFromVideo(), trackFromCam() or track() is called). Default configuration files (.cfg) are provided in Samples/data folder.
	* Please refer to the  <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for further details
	* on using the configuration files and all configurable options. \if IOS_DOXY Also, please read the section on automatic device-specific configuration selection.\endif
	* @param trackerConfigFile the name of the tracker configuration file.
	*/
	void setTrackerConfigurationFile(char* trackerConfigFile);

	/* Is tracker active?
	* This function is deprecated; please use @ref getTrackingStatus().
	*
	* Checks whether the tracker is currently active.
	* @return true if tracker is currently active, false otherwise.
	*/
	bool isActive() {return this->active;};

	/** Attaches an observer implementation to the tracker.
	*
	* @param _obs pointer to a VisageTrackerObserver implementation.
	* @see VisageTrackerObserver, detach()
	*/
	void attach(VisageTrackerObserver * _obs) {obs[nObs++] = _obs;};


	/** Detaches all attached observers from the tracker.
	* This function deactivates the observer mechanism.
	*
	* NOTE: the observer objects are only detached, not destroyed.
	* @see VisageTrackerObserver, attach()
	*/
	void detach() {nObs = 0;};


	/*
	* Set the upper and lower limit for each of Facial Animation Parameters, i.e., the maximum and minimum allowed values for
	* each of the 68 FAPs. The tracker will cut off any values outside these limits.
	*
	* Initially it is set to a very large range so the limits will not be reached in practice.
	*
	* @param upperLimit pointer to the FAPs object containing the upper FAP limits
	* @param lowerLimit pointer to the FAPs object containing the lower FAP limits
	* @see getUpperFAPLimits(), getLowerFAPLimits()
	*/
	void setFAPLimits(FAPs *upperLimit, FAPs *lowerLimit);

	/*
	* Returns the pointer to the FAPs object containing the upper FAP limits
	* @return the upper FAP limits.
	*/
	FAPs *getUpperFAPLimits();

	/*
	* Returns the pointer to the FAPs object containing the lower FAP limits.
	* See setFAPLimits() for details regarding the FAP limitation mechanism.
	* @return the lower FAP limits.
	*/
	FAPs *getLowerFAPLimits();
	
	/** Sets the inter pupillary distance
	* 
	* Inter pupillary distance (IPD) is used by the tracker to estimate the distance of the face from the camera. 
	* By default, IPD is set to 0.065 (65 millimeters) which is considered average. If the actual IPD of the tracked person is known, this function can be used to set this IPD. As a result, the calculated distance from the camera will be accurate (as long as the camera focal lenght is also set correctly).
	* This is important for applications that require accurate distance. For example, in Augmented Reality applications objects such as virtual eyeglasses can be rendered at appropriate distance and will thus appear in the image with real-life scale.
	* 
	* @param value the inter pupillary distance (IPD) in meters.
	* @see getIPD()
	*/
	void setIPD(float value);

	/** Returns the inter pupillary distance.
	* Returns the current inter pupillary distance (IPD) setting.
	* IPD setting is used by the tracker to estimate the distance of the face from the camera. See setIPD() for further details.
	* @return current setting of inter pupillary distance (IPD) in meters.
	* @see setIPD()
	*/
	float getIPD();

	/** Detects a credit card magnetic stripe in the current frame.
	* 
	* This function attempts to detect a standard-size credit card stripe in the current frame. 
	* The search is performed within a search region defined with respect to the location of the face in the frame. 
	* The search is performed only while the tracker is tracking a face (tracking status returned by getTrackingData() is TRACK_STAT_OK). 
	* Various parameters of the search can be configured in the tracker configuration file - see the <a href="../VisageTracker Configuration Manual.pdf">Tracker Configuration Manual</a> for details.
	*
	* @param size detected stripe width in pixels.
	* @return true if the stripe was found, false otherwise.
	*/
	bool DetectStrip(double &size);
	
	/** \if IOS_DOXY\elseif ANDROID_DOXY\else Show the camera settings dialog.
	*
	* Opens the camera settings dialog box, allowing the user to set parameters such as brightness, contrast, gain etc.
	* The dialog opens only if the tracking from camera is currently active and if the camera_input parameter in the tracker configuration file is set to 0 (this is the default setting).
	*
	* @return true on success, false if dialog could not be shown
	* \endif
	*/
	bool showCameraSettingsDialog();


	/* Get Face Animation Parameters (FbaAction implementation).
	*
	* <b>NOTE:</b> Do not use this function directly, use getTrackingData() instead.
	*
	* This function implements the FbaAction interface. It is not intended to be called directly.
	* It returns a FBAPs class that contains the Face Animation Parameters (FAPs)
	* currently estimated by the tracker.
	* The global translation of the face is stored in the global translation Body Animation Parameters (BAPs).
	* The specification of the FAP and BAP parameters
	* is contained in the the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
	*
	* Certain parameters, like the ones on the tongue, teeth, nose and ears, can currently not be reliably estimated so they are not returned
	* and their values are always set to zero. These parameters are:
	* - FAPs 14 - 18 (jaw thrust and shift, lips forward push)
	* - FAPs 23 - 30 (eyeball rotation and thrust)
	* - FAPs 39 - 40 (puff cheeks)
	* - FAPs 43 - 47 (tongue motion)
	* - FAPs 61 - 68 (nose and ear motion)
	*
	* Furthermore, the parameters of the outer lip contour (51 - 60) and the corresponding parameters of the inner lip
	* contour (4 -13) are both set to the mean value of the outer and inner lip displacement. For example, parameters 4 (vertical displacement of top inner lip) and 51 (vertical displacement of top outer lip)
	* are both set to the same value, and this value is the mean displacement of the upper lip midpoint.
	*
	*
	* @param globalTime the time for which the FAPs are requested, in milliseconds; in this implementation we ignore this parameter so it can be 0. The function always returns the most recent estimated FBAPs.
	* @param lastFBAPs the final FBAPs from the previous frame of animation; in this implementation we ignore this parameter so it can be NULL
	* @param model the %Model currently used in the player; in this implementation we ignore this parameter so it can be NULL
	*/
	FBAPs *getFBAPs(long globalTime, FBAPs *lastFBAPs, VisageCharModel* model);

	/* Start the action (FbaAction implementation).
	*
	* Note: Do not call this function directly.
	*
	* This function implements the FbaAction interface.
	*/
	void start(long globalTime);

	/**Stops the tracking.
	*
	*/
	void stop();

	/** Action name (FbaAction implementation).
	*
	*/
	char* actionTypeName() {return "VisageTracker2";};

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
	void getNormalizedFaceImage(IplImage* frame, IplImage* normFace, FaceData* face_data, FDP* normFDP, int norm_type = VS_NORM_POSE);

	/** Reset tracking
	*
	* Resets the tracker. Tracker will reinitialise.
	*
	*/
	void reset();
	volatile bool doReset;

#ifdef IOS
	/** \if IOS_DOXY\ Set data bundle
	 * 
	 * Used to set bundle from which data files will be read. Default is main bundle.
	 *
	 * \endif
	 */
	void setDataBundle(NSBundle *bundle);
#endif
	
FBFT *fbft; /* Tracker engine implementation. */

//read scale and shape units from file
void read_profile (
	const string &filename
	);

	//write scale and shape units to file
void write_profile (
	const string &filename
	);

	//write scale and shape units to file, using the current file name
void write_profile (
	);

int startFrame; // start processing video from this frame
int endFrame; // end processing at this frame
int slowdownFrame; //insert pause to slow down for debugging from this frame
int slowdownTime;  // slow down each frame by this time, in milliseconds

IplImage* frame_input; // Current video frame, input; may be color or grayscale
IplImage* gl_image; // OpenGL window image, used for presenting the video image in the OpenGL window.

float VideoFrameRate;
float targetFps;

bool IsAutoStopped; // reason for stopping

int SimpleUpdate(void);
void Finish(void);


private:

	static void processEyesClosure(IplImage* frame, FDP* fdp, float* t, float* r, float* out);
	static void GetPupilCoords(FDP* points, CvMat* eyes_coords, int w, int h);

	TrackerGUIInterface *guiIface;
	TrackerInternalInterface *internalIface;

	int auto_init; /* Control the automatic initialisation. 0 = semi-automatic initialisation of the tracker; 1 = fully automatic tracking with fast initialisation. Initially it is set through the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file.*/

	int manual_face_detection; /* When set to 1, the tracker disables the automatic detection of the face during the tracking initialization and instead it presents the image to the user and asks the user to click on the eyes,nose tip and mouth corners. This is used for videos in which automatic face detection does not work, so tracking can not start; manually picking the face allows the tracker to initialise and then it continues tracking. For example, this is the case with thermal videos and sometimes with videos where the face wears some painted markers; it has sometimes happened with videos of old people.
The function for manual feature picking is implemented through the TrackerGUIInterface abstract interface class; it is provided with full source code and developers can modify or replace it wih different picking methods, or even with their own automatic detection method. If this option is set to 0, automatic face detection is used. Initially it is set through the <a href="#config">tracker configuration file</a>.*/

	float init_pose_dev; /* Allowed deviation from ideal initialisation pose. This value is used only in automatic initialisation mode. It controls the level of quality of the head pose that triggers the initialisation of the tracker. The value of 0 means the tracker will require the best head pose before it starts; higher values relay the requirements.*/
	float init_yaw_threshold; /* This value is used during automatic initialisation or, in manual initialization mode while the tracker initially searches for the best frame on which to perform initialization. It controls the amount of yaw (left-right rotation of the head) allowed at initialisation; the tracker waits until the head pose is within this limit before it initialises and starts tracking. The value of 0 means the tracker will require perfectly frontal head pose before it starts (it is not recommended to set it to 0 because the tracker may then never start); higher values relax the requirements.*/
	float init_roll_threshold; /* This value is used during automatic initialisation or, in manual initialization mode while the tracker initially searches for the best frame on which to perform initialization. It controls the amount of roll (tilt of the head) allowed at initialisation; the tracker waits until the head pose is within this limit before it initialises and starts tracking. The value of 0 means the tracker will require perfectly frontal head pose before it starts (it is not recommended to set it to 0 because the tracker may then never start); higher values relax the requirements. */
	float init_velocity_threshold; /* This value is used only in automatic initialisation mode. It controls the speed of head movement allowed at initialisation; the tracker waits until the head stabilises below this speed limit before it initialises and starts tracking. It is expressed in meters per second. The value of 0 means the tracker will require perfectly still head before it starts (it is not recommended to set it to 0 because the tracker may then never start); higher values relax the requirements. */
	float init_timeout; /* This value is used only in automatic initialisation mode. It controls the time allowed at initialisation, in milliseconds. If the desired head pose was not found during this time, the tracker chooses the best available image seen during this time. The timing starts from the moment when face is detected.*/
	int init_timeout_enable; /* This value is used during automatic initialisation or, in manual initialization mode while the tracker initially searches for the best frame on which to perform initialization. It enables or disables the initialization timeout mechanism; when it is disabled, the parameter init_timeout (see above) is ignored and initialization continues until the desired head pose is reached. The setting is separate for camera, video file and raw image input modes and determined by the first, second and third bit of the value, respectively. Thus value 1 means that the timeout mechanism is enabled when tracking from camera; 2 means it is enabled when tracking from video file; 4 means it is enabled when using the raw image interface and 0 means it is always disabled; combinations are allowed, e.g. 6 enables timeout in video and raw image input modes.*/
	int init_display_status; /* This value is used during automatic initialisation or, in manual initialization mode while the tracker initially searches for the best frame on which to perform initialization. It enables or disables the initialization status display. When enabled, the initialization status is displayed interactively on the screen during initialization in order to help the user to position the head. The setting is separate for camera, video file and raw image input modes and determined by the first, second and third bit of the value, respectively. Thus value 1 means that the display is enabled when tracking from camera; 2 means it is enabled when tracking from video file; 4 means it is enabled when using the raw image interface and 0 means it is always disabled; combinations are allowed, e.g. 6 enables display in video and raw image input modes.*/
	float recovery_timeout; /* This value is used only in automatic initialisation mode. It is used when the tracker looses the face and can not detect any face in the frame. This value tells the tracker how long it should wait before considering that the current user is gone and initialising the full re-initialisation procedure.  If the face is detected before this time elapses, the tracker considers that it is the same person and it recovers, i.e. continues tracking it using the previous settings. The time is expressed in milliseconds. */

	bool display_video; /* Toggle video display on/off; initially it is set through the<a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file and usually it is on, so video will be visible while tracking. */
	bool display_model_texture; /* Toggle model texture display on/off; initially it is set through the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file. */
	bool display_tri_mask; /* Toggle triangle mask display on/off (triangle mask determines where track points are looked for); initially it is set through the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file. */
	bool display_model_wireframe; /* Toggle wireframe model display on/off; initially it is set through the <a href="#config">tracker configuration file</a>. */
	bool display_model_axes; /* Toggle model axes display on/off; initially it is set through the <a href="#config">tracker configuration file</a>. */
	int display_results; /* Control the display of tracking results. 0 = no results display; 1 = display tracked feature points; 2 = display head pose; 4 = display eye locations; values can be added together (e.g. 6 = display head pose and eye locations). Initially it is set through the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file. */
	bool display_track_points; /* Toggle the display of track points on/off (track points are automatically found points in the face that are suitable for tracking - they are not the same as feature points and their display is mainly useful for monitoring the functioning of the tracker); initially it is set through the <a href="#config">tracker configuration file</a>.. */

	float translation_compensation_factor; /* Compensation factor for translation results. Default value is 1.0. Bigger values result in more compensation being applied. If it is set to 0, no compensation is applied. For details about translation compensation see getFaceTranslation()*/

	int smoothing_translation; /* Smoothing value for translation results. It must be set between 0 (no smoothing) and 10 (maximal smoothing). Initially it is set through the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file.. Smoothing reduces tracking noise and makes results smoother but it introduces delay so it should be used with caution.*/
	int smoothing_rotation; /* Smoothing value for rotation results. It must be set between 0 (no smoothing) and 10 (maximal smoothing). Initially it is set through the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file.  Smoothing reduces tracking noise and makes results smoother but it introduces delay so it should be used with caution.*/
	int smoothing_fp; /* Smoothing value for feature point results. It must be set between 0 (no smoothing) and 10 (maximal smoothing). Initially it is set through the <a href="../VisageTracker Configuration Manual.pdf">tracker configuration</a> file.  Smoothing reduces tracking noise and makes results smoother but it introduces delay so it should be used with caution.*/
	int video_file_sync; /* Synchronisation of video playback from file.
 If set to 0, all video frames are processed and displayed so the effective video playback speed
 depends on the available processing power - on a slow computer playback will be slower than real time, while on a fast
 computer it may be faster. If the flag is set to 1 playback is synchronised by
 skipping video frames or introducing delay as needed, so the video file is played at
 its normal speed. This may deteriorate tracking results on slower computers because video
 frames are skipped. */

// changed to public due to new user interface implementation
// TO DO: verify if all this really must be public

	friend class FBFT;
	friend class VisageFeaturesDetector;


	IplImage* frame_gray; // Current video frame converted to grayscale; all processing is done on grayscale images
	IplImage* best_frame; // Frame with best pose found during initialisation; grayscale; used only during initialisation
	IplImage* gl_image_gray; // OpenGL window image, grayscale, used for presenting grayscale video image in the OpenGL window.
	bool PoseTestTimedOut; //PoseTestTimedOut is set to true if pose test in WaitFrontalPose finished with a timeout or false if pose test found good pose.
	//Variables used for debugging, usually in batch processing mode from FaceTracker2; if set to -1 they are not used (that is the default)

protected:
	
	SmoothingFilter sf;
	CvMat *smoothing_factors;
	FaceData* trackingData;

	int frameCount; // frame count from beginning of tracking
	double frameTime; // duration of one frame in milliseconds
	long pts; // presentation time stamp, calculated by grabFrame()
	long pts_data; // time stamp
	int pts_frame;

	volatile bool active;
	volatile bool inThread;

private:
	int skippedCount;
	int foundCount;
	float videoFps;

	float detectStripAreaThreshold;
	float detectStripPerfectRatio;
	float detectStripRatioThreshold;
	float detectStripAngleThreshold;
	float detectStripRoiYOffset;
	float detectStripRoiWidth;
	float detectStripRoiHeight;

	float bdts_trees_factor;

	//VisageFeaturesDetector *m_Detector;
	VisageDetector *m_Detector;

	long getCurrentTimeMs(bool init);

	bool grabFrame(bool init);

	IplImage *frame_buffers[3];
	int a_frame;
	int p_frame;
	int v_frame;

	bool fitModelToFaceNew(FDP *latestFP, bool init_tracker, bool reinit = false);
	bool verifyPose(FDP *f);
	bool testPose(FDP *f, FDP *best);
	void writeBits(FILE *streamHandle, unsigned char *bits, int size);//void writeBits(int streamHandle, unsigned char *bits, int size);

	static void trackInThread2(void* vt);

	int Init(void);

	FDP *tmpLatestFP;
	FDP *tmpBestFP;
	bool presenceTest;
	int faceLostTime; //time when face was lost from view
	int faceFoundTime; //time when face was detected
	float bestPoseValue;
	int profile_loaded;
	int trackerState;

	bool debug_frame_pause;
	bool texInited;
	bool init_successful;
	//source video filename
	char source[500];

	// frame grabber interface
	VisageTrackerFrameGrabber *frameGrabber;
	int frameGrabberImageFormat; //image format for the frame grabber image (RGB, BGR or LUMINANCE)

	long globalStartTime;

	//interesting FAP groups are 2,3,4,5,8 (actually 1,2,3,4,7 as the counting starts from 0)
	FBAPs *fbaps; /*!Pointer to a Face and Body Animation Parameters object*/
	FBAPs *fbaps0;
	FDP* fdps;/*!Pointer to a Facial Definition Point object*/

	FAPs* upperLimits;
	FAPs* lowerLimits;

	VisageTrackerObserver *obs[100];/*!<observers list*/
	int nObs; /*!number of attached observers*/

	//initial face rotation and translation
	float init_face_translation[3];
	float init_face_rotation[3];

	//face rotation and translation
	float face_translation_rel[3];
	float face_translation_abs[3];
	float face_translation_comp[3];
	float face_rotation[3];
	float face_rotation_not_smoothed[3];

	// values holding the initialization status
	bool init_face_detected;
	float init_yaw;
	float init_roll;
	float init_velocity;

	//various flags
	bool toFile;
	bool editing; //used by display function; when true, display function ignores other display flags and displays only the wireframe model for editing
	bool fitting; //used by display function to display the wireframe model in different color while face fitting process is on, mainly for debugging purposes
	HANDLE trackingThreadHandle;/*!Handle to the thread used for tracking*/
	HANDLE detectorThreadHandle;/*!Handle to the thread used for detecting face*/
#ifndef EMSCRIPTEN
	CvCapture* capture;/*!<Structure for the capture of video stream, from OpenCV*/
#endif
#if defined (IOS)
	NSAutoreleasePool *pool;
#endif
#ifdef WIN32
	videoInput *VI; /*!<Structure for the capture of video from camera, from videoInput library*/
#endif
#ifdef IOS
	iOSCapture *IC;
#endif
#ifdef MAC_OS_X
	OSXCapture *OSXInput;
#endif
	 
	bool tex_too_small; // video frame dimensions are bigger than max supported tex size

	CFBAEncoder* fbaEncoder;/*!encoder object*/
	FILE *fbaFileHandle;//int fbaFileHandle;/*!handle for the FBA file*/

	char fbaFileName[500];/*!fba file name*/


	unsigned char byte;
	int bitcnt;

	string profile_filename;
	string configuration_filename; //tracker configuration file
	string configuration_filestring; //tracker configuration file contents
	string texture_filename; // file name for texture (read from profile)
	string detector_data_path; // path to the folder containing Haar cascade files

	float gaze[2]; //gaze direction
#ifdef IOS
	NSBundle *dataBundle;
#endif

	//stuff copied from main.cpp in Nils' original project
	volatile bool tracking_ok; //tracking status OK
	f32 trackingFrameRate; // measured tracking frame rate
	iu32 cam_input; //0: videoinput library; 1: OpenCV
	iu32 cam_device; //camera device number
	int m_cam_device; //if -1, read cam_device number from configuration file, else take this number
	iu32 cam_width; //camera width
	iu32 cam_height; //camera height
	iu32 gl_width; //width of opengl display window
	iu32 gl_height; //height of opengl display window
	iu32 cam_fps; //camera frame rate to set
	iu32 cam_mirror; // if 1, flip camera image horizontally to achieve mirror effect
	iu32 cam_flip; // if 1, flip camera image vertically
	iu32 cam_settings; //if 1, show camera settings dialog
	iu32 cam_auto; //if 1, do automatic camera settings when tracking starts
	IplImage* new_frame; //any new frames
	void app_setup();  // read application settings from configuration file
	int load_profile(); // try to load profile, return 1 on success, 0 if profile not found, -1 if profile found but failed to load
	int cam_orientation;

	//from func_gl.h
	//clear buffer

	//stuff for calculating FAPs and FDPs
	float calculateFAPU( int fapu );
	void calculateFAPUs();
	float fapuValue[6];
	int calculateFAP( int fap ) ;
	void calculateFAPs( FBAPs* fbaps);
	void calculateFDP(FDP* f, int w, int h, CvMat* vert, bool _3D, bool use_detected_points = false);
	void setFDPIndices(FDP* f);

	FDP *featurePoints2D;
	FDP *featurePoints3D;
	FDP *featurePoints3DR;
	FDP *origFeaturePoints2D;
	FDP *origFeaturePoints3D;
	FDP *origFeaturePoints3DR;
	FDP *m_bestFP;

	ModelFitter* modelFitter;
	int nFPs; // number of feature points used for fitting

	void initTrackingData(void);
	void swapTrackingData(void);
	void smoothTrackingData(void);

	volatile bool inSwap;
	volatile bool inRead;
	volatile int turn;

#if defined(IOS) || defined(ANDROID) || defined(MAC_OS_X) || defined(LINUX)
	pthread_mutex_t mutex;
#endif

#ifdef WIN32
	CRITICAL_SECTION trackingData_cs;
#endif

	string bdts_data_path; // path to the folder containing boosted decision trees
	VisageDetector* m_DetectorBDFS;
	VisageTrackerCore* m_trackerCore;
	//SimpleTrackerCore* m_trackerCore;

	volatile bool inDetectorThread;
	volatile bool stopDetectorThread;

	IplImage* detector_frame;
	FDP* detector_fp;
	volatile int facialFeat;
	volatile int detector_parts;
	volatile int detect;
	volatile int new_detector_frame;
	int windowScaler;
	
	CvMat* projection;
	CvMat* neutral;

	PoseEstimator* poseEstimator;
	bool estimatorInited;
	bool fitted;

	bool inited;
	int trackerFrameWidth;
	int trackerFrameHeight;
	int trackerFrameFormat;
	FrameGrabberInternal* internalGrabber;
	bool configChanged;
};

class FrameGrabberInternal : public VisageTrackerFrameGrabber
{

public:
	FrameGrabberInternal();
	~FrameGrabberInternal();

	unsigned char *GrabFrame(long &timeStamp);
	void UpdateFrame(int width, int height, const char* imageData, int format, int origin, int widthStep, long timeStamp);

public:
	IplImage* frame;
	char* data;
	long timestamp;

};

}

#endif // __VisageTracker2_h__

