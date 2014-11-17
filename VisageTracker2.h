
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
#include "TrackingData.h"
#include "VisageDetector.h"
#include "TrackerGazeCalibrator.h"
//#include "Estimator6P.h"
#include "VisageEstimator.h"
#include "Estimator.h"
#include "EstimatorGauss.h"
//#include "IVTFilter.h"

using namespace std;
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
    
    /** VisageTracker2 is a head/facial features tracker capable of tracking facial features in video coming from a
     * video file, camera or other sources.
     *
     * The tracker is fully configurable through comprehensive tracker configuration files. visage|SDK contains optimal configurations for common uses such as head tracking and facial features tracking.
     * Please refer to the <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for the list of available configurations.
     * \if IOS_DOXY
     * Please read more details about configuration selection in the section <a href="../../doc/creatingxc.html#config_selection">Device-specific configuration selection</a>
     * \endif
     *
     * The <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> provides full detail
     * on all available configuration options, allowing to customize the tracker in terms of performance, quality, tracked features, range
     * of tracked facial actions and other options and produce in effect a variety of different trackers suited for specific applications.
     *
     * \if ANDROID_DOXY
     * trackFromCam() tracks the facial features from a video camera.
     * Tracking the facial features from raw image input is enabled by trackFromRawImages() and allows tracking
     * from any source. Due to instability of low-level camera input on some devices, the raw image input is currenty the recommended way to implement
     * tracking from camera. The sample project <a href="../tracker.html">VisageTrackerDemo</a> shows how to do this, with full source code.
     * \else
     * trackFromVideo() and trackFromCam() track the facial features from a video file
     * or from a video camera.
     * Tracking the facial features from raw image input is enabled by trackFromRawImages() and allows tracking
     * from any source.
     * \endif
     *
     * The tracker offers the following outputs, available through method getTrackingData():
     * - 3D head pose
     * - facial expression
     * - gaze direction
     * - eye closure
     * - facial feature points
     * - full 3D face model, textured
     * - screen space gaze point - see \ref screenSGT Screen Space Gaze Tracking
     * - detected credit card stripe for size adjustment -  see DetectStrip()
     *
     *
     * The tracker can apply a smoothing filter to tracking results to reduce the inevitable tracking noise. Smoothing factors
     * are adjusted separately for global face rotation, translation and different parts of the face. The smoothing settings
     * in the supplied tracker configurations are adjusted conservatively to avoid delay in tracking response, yet provide
     * reasonable smoothing. For further details please see the smoothing_factors parameter array in the <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>.
     *
     * The tracker works in its own thread and the functions trackFromVideo(), trackFromCam() and trackFromRawImages()
     * immediately return control. This means that the VisageTracker2::getTrackingData() function can
     * be called at any time to interrogate the tracker and get results. This is the simplest
     * way to obtain the results.
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
     * \else Three more mechanisms for obtaining outputs are provided for
     * convenience:
     * -# VisageTrackerObserver: During tracking, the VisageTrackerObserver can be used to obtain the tracking results.
     * To attach one or more observers, use the attach() method. The VisageTrackerObserver::Notify() function
     * shall then be called by the tracker after each processed video frame (even if the face was not found in the current frame).
     * This is particularly useful for synchronisation.
     * In the FaceTracker2 sample this mechanism is used to render the facial animation on a face model based on tracking
     * results.
     * -# FAPlayer: Because the VisageTracker2 is an FbaAction, it can be directly added as a track to a FAPlayer
     * and the player will show the animation based on the Face Animation parameters provided by the tracker.
     * The tracking results are treated as any other source of animation and the player can mix them
     * with other animation sources.
     * See @ref FAPlayer for details.
     * -# Writing into an FBA file: The tracking results in form of MPEG-4 Face and Body Animation Parameters can be saved in a .fba file (MPEG-FBA ecoded).
     * See @ref trackFromVideo(), @ref trackFromCam() and @ref trackFromRawImages() for details.
     *
     * \endif
     *
     *
     * In semi-automatic mode, the tracker requires access to certain manual interventions
     * by the user. For this purpose, the application that uses the tracker in semi-automatic mode must
     * implement an instance of the TrackerGUIInterface abstract class and provide
     * the necessary functions to the tracker by implementing the functions
     * in TrackerGUIInterface.
     *
     * If the OpenGL window access is provided through the optional TrackerOpenGLInterface, the tracker can display the video, the tracking results and
     * some auxiliary information. This is intended mainly as help for configuring the tracker. The display is controled through the <a href="#config">tracker configuration file</a>.
     *
     * \if IOS_DOXY
     * \elseif ANDROID_DOXY
     * \else
     *
     * The full implementation of these two optional classes is
     * provided (with source code) in the FaceTracker2 sample, where the
     * class COpenGLWnd is an implementation of both TrackerOpenGLInterface and TrackerGUIInterface.
     *
     * \endif
     *
     *
     *
     * The tracker requires the following data and configuration files \if IOS_DOXY (available in Samples/iOS/data) \elseif ANDROID_DOXY (available in Samples/Android/VisageTrackerUnityDemo/data) \else (available in Samples/OpenGL/data/FaceTracker2) \endif.
     * Please either copy the complete contents of this folder into your application's working folder, or consult <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for detailed settings.
     *
     * \section screenSGT Screen Space Gaze Tracking
     * Screen space gaze tracking feature estimates gaze position (the location on the screen where the user is looking) in normalized screen coordinates.
     * Screen space gaze tracking works in two phases: calibration and estimation.
     *
     * In the calibration phase, the system is calibrated for gaze estimation by passing the calibration data to the tracker. Calibration data consists of series of points displayed on screen. The user looks at the calibration point. During the calibration phase tracker collects calibration points and matching tracking data for each point.
     * After all calibration points have been passed to the tracker, the tracker performs calibration of gaze tracking system and switches to estimation phase.
     *
     * In the estimation phase the tracker estimates gaze location in screen space coordinates and returns the data as ScreenSpaceGazeData object for each frame.
     *
     * Screen space gaze tracking works in two different modes. Online mode is used when tracking in real time from camera.
     * Offline mode is used when tracking from video files. The key differences between the two modes are:
     * - In online mode each FaceData object returned by getTrackingData() contains screen space gaze data for the current frame.
     * - In offline mode, gaze estimation is done as a post process after the tracking is finished for the whole video sequence. FaceData objects returned by the tracker contain no screen space gaze data for current frame.
     * - In online mode, calibration data is passed to the tracker as it is shown on screen.
     * - In offline mode, calibration data is passed as a ScreenSpaceGazeRepository object for the whole video sequence.
     * - Online mode can be initalized at any time during tracking.
     * - Offline mode has to be initalized before tracking is started.
     * - In online mode the application is responsible for finalizing gaze tracking system calibration.
     * - In offline mode the calibration is finalized automatically.
     *
     * <h3> Online screen space gaze tracking</h3>
     * Online mode implies using the screen space gaze tracking when tracking from camera.
     *
     * It is initialized by calling InitOnlineGazeCalibration() method. This method prepares the tracker for real time gaze tracking calibration.
     * Each calibration point in normalized screen coordinates is passed to tracker by calling AddGazeCalibrationPoint(). It is expected that the point is displayed on the screen before calling the method and that the user looks at calibration points during the calibration.
     * Application is responsible for reading or generating the calibration data, displaying it on screen and synchronization with the tracker.
     * It is required to manually notify the tracker that calibration is finished (once all calibration points are used) by calling FinalizeOnlineGazeCalibration() method. Once this method is called the tracker performs calibration of screen space gaze tracking system using provided calibration data and tracking data collected during the calibration process.
     *
     * After the system is calibrated the estimation phase starts. Estimations are returned as part of FaceData objects obtained by calling getTrackingData() method specifically in FaceData::gazeData.
     *
     * @see InitOnlineGazeCalibration(), AddGazeCalibrationPoint() FinalizeOnlineGazeCalibration(), ScreenSpaceGazeData, FaceData
     *
     *<h3>Offline screen space gaze tracking</h3>
     * Offline mode implies using gaze tracking feature when tracking from video file.
     * It is assumend that calibration points have been displayed to the user while recording the video of the user's face; that user actually looked at the calibration points;
     * and that these calibration points have been stored, together with corresponding time stamps. Furthermore, it is assumed that calibration was performed during a part of the video sequence.
     * Gaze estimation is performed on the remaining parts of the video sequence. The calibration parts of the video need not be contiguous.
     *
     * Offline mode initialized by calling InitOfflineGazeCalibration() method. This method takes ScreenSpaceGazeRepository object as parameter.
     * This object contains calibration data for the tracked video sequence. Each calibration point consists of x and y coordinates given in normalized screen coordinates and the index of the frame in which calibration point was displayed to the user. The tracker reads calibration points from the provided repository and collects tracking data from corresponding frames of the tracked video sequence.
     * Once all calibration data from provided repository is used, the tracker automatically performs calibration of the gaze tracking system using provided calibration data and tracking data collected during the calibration process.
     *
     * Offline mode must be initalized before tracking is started, otherwise some of the calibration frames may be discarded.
     *
     * After the whole video file has been processed, tracking stops and gaze estimations are available as ScreenSpaceGazeRepository object, obtained by calling getGazeEstimations() method. The returned repository contains ScreenSpaceGazeData object with screen space gaze position for each non-calibration frame of the tracked video sequence.
     *
     * @see InitOfflineGazeCalibration(), getGazeEstimations(), ScreenSpaceGazeRepository, ScreenSpaceGazeData
     */
    class VISAGE_DECLSPEC VisageTracker2 : public FbaAction
    {
    public:
        
        /** Constructor.
         *
         * @param trackerConfigFile the name of the tracker configuration file (.cfg; default configuration files are provided in \if IOS_DOXY Samples/iOS/data \elseif ANDROID_DOXY Samples/Android/VisageTrackerUnityDemo/data
         * Constructor must be called with the full path to the configuration file. The configuration file is in the same directory as other data files needed by the tracker. Because it is in the same directory its path can be parsed to set the path for the other data files.
         * \else Samples/OpenGL/data/FaceTracker2 \endif folder; for further details see <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>\if IOS_DOXY and section on <a href="../../doc/creatingxc.html#config_selection">device-specific configuration selection</a>.\endif).
         */
        VisageTracker2(const char* trackerConfigFile);
        
        /** Constructor.
         *
         * @param oglInterface the TrackerOpenGLInterface object or NULL if not used.
         * @param guiInterface the TrackerGUIInterface object or NULL if not used; in that case the tracker will only function in fully automatic mode. Semi-automatic modes require TrackerGUIInterface to be implemented.
         * @param trackerConfigFile the name of the tracker configuration file (.cfg; default configuration files are provided in  \if IOS_DOXY <a href="../../Samples/iOS/data">Samples/iOS/data</a> \elseif ANDROID_DOXY  <a href="../../Samples/Android/VisageTrackerUnityDemo/data">Samples/Android/VisageTrackerUnityDemo/data</a>
         * Constructor must be called with the full path to the configuration file. The configuration file is in the same directory as other data files needed by the tracker. Because it is in the same directory its path can be parsed to set the path for the other data files.
         * \else Samples/OpenGL/data/FaceTracker2 \endif; for further details see <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>\if IOS_DOXY and section on <a href="../../doc/creatingxc.html#config_selection">device-specific configuration selection</a>.\endif).
         * @see TrackerOpenGLInterface
         * @see TrackerGUIInterface
         */
        VisageTracker2(TrackerOpenGLInterface *oglInterface, TrackerGUIInterface *guiInterface, const char* trackerConfigFile);
        
        /** Destructor.
         */
        ~VisageTracker2();
        
        /** Track the face and facial features from a digital camera \if ANDROID_DOXY (note: due to instability of low-level camera access on some devices it is currently recommended to use trackFromRawImages - see VisageTrackerDemo sample project for detailed implementation) \endif.
         *
         * The function opens a camera video stream and tracks the face and facial features (assuming that there is a face in the video).
         * If no camera is available or if it fails opening the camera video stream, the function
         * returns false. Choice between multiple cameras is made using the camera_device parametar in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration file</a>.
         *
         * The tracking is started in a separate thread so this function returns immediately and the
         * function getTrackingData() can be used
         * to obtain the tracking results at any time. For more details about the available
         * mechanisms for obtaining the tracking results see the VisageTracker2 main documentation text.
         *
         * Depending on the used tracker configuration file, the tracker may run fully automatically or with a semi-automatic
         * initial fitting of the facial shape mask. If semi-automatic mode is used, the tracker captures the first image with the face clearly visible and in
         * suitable front-facing pose, and fits the face shape
         * mask to the image. The automatic fitting may not be absolutely precise, so
         * at this point it is may be necessary to manually adjust the face shape mask. For this
         * purpose the tracker calls the TrackerGUIInterface::ManuallyAdjustShapeMask() function.
         * The face shape mask adjustment settings are written into a profile and
         * used next time when the same person is tracked, so manual adjustment is done only the first
         * time. The name of the profile is chosen by the user using the TrackerGUIInterface::ChooseSuFileName()
         * function; this function is called at the very beginning of tracking to let the user choose the
         * existing profile or the name for the new profile. After the initial adjustment the tracker tracks the facial features in the frames from the camera until it is stopped using the stop() function.
         *
         * The resulting animation can be saved in a .fba file (MPEG-FBA encoded); leave the parameter outFbaFileName NULL if you do not want any file to be created.
         *
         * \if IOS_DOXY
         * It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
         * functions in order to get a practical understanding of the functioning of the tracker.
         * \elseif ANDROID_DOXY
         * It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
         * functions in order to get a practical understanding of the functioning of the tracker.
         * \else
         * It is  recommended to run the FaceTracker2 sample and get familiar with all its
         * functions by reading the tutorial and watching the tutorial videos in order
         * to get a practical understanding of the functioning of the tracker.
         * \endif
         *
         * @param outFbaFileName the name of the output .fba file (can be full path). If NULL, the .fba file is not produced.
         * @param orientation camera orientation, VISAGE_CAMERA_UP (default), VISAGE_CAMERA_DOWN (camera turned upside-down), VISAGE_CAMERA_LEFT (camera rotated left), VISAGE_CAMERA_RIGHT (camera rotated right)
         * @param frameGrabberImageFormat image format of camera frame, used only on Android.
         * @return true if successful
         *
         * @see getTrackingData(), FaceData, trackFromVideo(), trackFromRawImages()
         */
        bool trackFromCam(const char* outFbaFileName = NULL, int orientation = VISAGE_CAMERA_RIGHT, int frameGrabberImageFormat = VISAGE_FRAMEGRABBER_FMT_RGB);
        
        /** \ifnot ANDROID_DOXY */
        /** Track the face and facial features in a video file.
         *
         * The function opens the video file and starts tracking the face and facial features (assuming that there is a face in the video).
         *
         * The tracking is started in a separate thread so this function returns immediately and
         * function getTrackingData() can be used
         * to obtain the tracking results at any time. For more details about the available
         * mechanisms for obtaining the tracking results see the VisageTracker2 main documentation text.
         *
         * Depending on the used tracker configuration file, the tracker may run fully automatically or with a semi-automatic
         * initial fitting of the facial shape mask. If semi-automatic mode is used, the tracker captures the first image with the face clearly visible and in
         * suitable front-facing pose, and fits the face shape
         * mask to the image. The automatic fitting may not be absolutely precise, so
         * at this point it is may be necessary to manually adjust the face shape mask. For this
         * purpose the tracker calls the TrackerGUIInterface::ManuallyAdjustShapeMask() function.
         * The face shape mask adjustment settings are written into a profile and
         * used next time when the same person is tracked, so manual adjustment is done only the first
         * time. The name of the profile is the same as the video file but with the extension .su; therefore for
         * a file video.avi the face shape mask adjustment settings are written into the file video.su.
         * After the initial adjustment the tracker tracks the facial features in the video until
         * the end of the video is reached or until it is stopped using the stop() function.
         *
         * The resulting animation can be saved in a .fba file (MPEG-FBA encoded); leave the parameter outFbaFileName NULL if you do not want any file to be created.
         *
         * \if IOS_DOXY
         * It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
         * functions in order to get a practical understanding of the functioning of the tracker.
         * \elseif ANDROID_DOXY
         * It is  recommended to run the VisageTrackerDemo sample and get familiar with all its
         * functions in order to get a practical understanding of the functioning of the tracker.
         * \else
         * It is  recommended to run the FaceTracker2 sample and get familiar with all its
         * functions by reading the tutorial and watching the tutorial videos in order
         * to get a practical understanding of the functioning of the tracker.
         * \endif
         *
         * @param inVideoFileName the name of the input video file (can be full path). It is recommended to use a DIVX encoded AVI file on Windows and MP4 or Quicktime MOV file on iOS.
         * @param outFbaFileName the name of the output .fba file (can be full path). If NULL, the .fba file is not produced.
         * @return true if successful
         *
         * @see getTrackingData(), FaceData, trackFromCam(), trackFromRawImages()
         */
        bool trackFromVideo(const char* inVideoFileName,
                            const char* outFbaFileName = NULL);
        
        /** DEPRECATED, replaced by trackFromVideo().
         */
        bool trackFromAvi(const char* inAviFileName,
                          const char* outFbaFileName = NULL);
        /** \endif */
        /** Track the face and facial features in images passed from the application using the raw image interface (VisageTrackerFrameGrabber)
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
         * Depending on the used tracker configuration file, the tracker may run fully automatically or with a semi-automatic
         * initial fitting of the facial shape mask. If semi-automatic mode is used, the tracker captures the first image with the face clearly visible and in
         * suitable front-facing pose, and fits the face shape
         * mask to the image. The automatic fitting may not be absolutely precise, so
         * at this point it is may be necessary to manually adjust the face shape mask. For this
         * purpose the tracker calls the TrackerGUIInterface::ManuallyAdjustShapeMask() function.
         * The face shape mask adjustment settings are written into a profile and
         * used next time when the same person is tracked, so manual adjustment is done only the first
         * time. The name of the profile is chosen by the user using the TrackerGUIInterface::ChooseSuFileName()
         * function; this function is called at the very beginning of tracking to let the user choose the
         * existing profile or the name for the new profile. After the initial adjustment the tracker tracks the facial features in the images until it is stopped using the stop() function.
         *
         * The resulting animation can be saved in a .fba file (MPEG-FBA encoded); leave the parameter outFbaFileName NULL if you do not want any file to be created.
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
         * This method fills the given face data structure and returns the tracking status.
         *
         * On first call of this function the memory for the required member variables of the passed object will be allocated and initialized automatically.
         *
         * @param data FaceData structure that will be filled with current face data
         * @return tracking status (TRACK_STAT_OFF, TRACK_STAT_OK, TRACK_STAT_RECOVERING and TRACK_STAT_INIT, see @ref FaceData for more details)
         *
         * @see FaceData
         */
        int getTrackingData(FaceData *data);
        
        /*
         * This method fills the gven face data structure and returns the tracking status.
         *
         * On first call of this function the memory for the required member variables of the passed object will be allocated and initialized automatically.
         *
         * @param Face data structure that will be filled with current tracking data
         * @return tracking status (TRACK_STAT_OFF, TRACK_STAT_OK, TRACK_STAT_RECOVERING and TRACK_STAT_INIT, see @ref TrackingData for more details)
         *
         */
        int getFaceData(FaceData *data);
        
        /** Set configuration file name.
         *
         * The tracker configuration file name is set and this configuration file will be used for next tracking session (i.e. when trackFromVideo(), trackFromCam() or trackFromRawImages() is called). Default configuration files (.cfg) are provided in \if IOS_DOXY Samples/iOS/data \elseif ANDROID_DOXY Samples/Android/VisageTrackerUnityDemo/data  \else Samples/OpenGL/data/FaceTracker2 \endif folder.
         * Please refer to the  <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for further details
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
        
        /* Is tracker correctly tracking the face?
         * This function is deprecated; please use @ref getTrackingStatus().
         *
         * Checks whether the tracker is currently tracking the face.
         * @return true if tracker is currently tracking the face correctly, false if the tracker can currently not detect the face.
         */
        bool isTrackingOK() {return this->tracking_ok;};
        
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
        
        
        /* Get facial feature points estimated by the tracker.
         *
         * The feature points are identified
         * according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
         * belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
         * illustrated in Figure 2 on Page 8 of the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
         *
         * Certain feature points, like the ones on the tongue and teeth, can not be reliably detected so they are not returned
         * and their coordinates are always set to zero. These points are:
         * 3.5, 3.6, 6.1, 6.2, 6.3, 6.4, 7.1, 9.8, 9.9, 9.10, 9.11, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 11.4, 11.5, 11.6
         * Several other points are estimated, rather than accurately detected, due to their specific locations. Examples of such points
         * are 11.4 (usually hidden in the hair), hairline points (10.1 - 10.3) as well as cheeck points 5.1 - 5.4 which can not be precisely defined due to lack of
         * features on the cheek.
         *
         * The resulting feature point coordinates are returned in form of an FDP object. This is a container clas used for storage of MPEG-4 feature points.
         * It provides functions to access each feature point by its group and index and to read its coordinates. Note that FDP can store 3D points but
         * in case of 2D return feature points only the x and y coordinates of each point are used.
         *
         * In case of absolute 3D coordinates (relative = false), the coordinate system is such that when looking towards the video image, the direction of x is to the
         * left, y iz up, and z points into the image, away from the viewer. The global origin (0,0,0) is placed at the camera.
         *
         * In case of relative 3D coordinates (relative = true), the coordinate system is such that when looking towards the video image, the direction of x is to the
         * right, y iz up, and z points out of the image, towards the viewer. The  origin (0,0,0) is placed at the left eye of the person (i.e. the eye that is on the right in the image).
         *
         * The 2D coordinates are normalised to image size so that the lower left corner of the image has coordinates 0,0 and upper right corner 1,1.
         *
         * @param initial if true, function returns the feature points from the initial frame of video, otherwise it returns the latest estimated points.
         * @param relative if true, function returns the coordinates relative to the head (i.e. coordinates in the coordinate system of the head), otherwise it returns global coordinates. this parameter is ignored if 2D coordinates are requested.
         * @param _3D if true, the function returns 3D coordinates of feature points. Otherwise, it returns 2D coordinates normalised to image size so that the upper left corner of the image has coordinates 0,0 and lower right corner 1,1.
         */
        FDP *getFeaturePoints(bool initial, bool relative, bool _3D);
        
        /* Draw the tracking results.
         *
         * Visualize the most recent tracking results in the current OpenGL window.
         *
         */
        void drawResults();
        
        //helper functions for drawResults()
        void drawPoint(int i,int j);
        void drawPoint3D(int i,int j);
        void drawPoints2D(int *points, int num, int drawType);
        void drawPoints3D(int *points, int num, int drawType);
        
        /* Get the current frame rate.
         *
         * This function returns the frame rate of the tracker, in frames per second,
         * measured ovber last 10 frames.
         *
         * @return The measured tracking frame rate, in frames per second.
         */
        float getFrameRate() {if(active) return trackingFrameRate; else return 0.0f;};
        
        /* Get the current tracking status.
         *
         * Returns the current status of the tracker, which can be one of the following:
         *
         * <table>
         * <tr><td width="100"><b>RETURN VALUE</b></td><td><b>DESCRIPTION</b></td></tr>
         * <tr><td>TRACK_STAT_OFF</td><td>Tracker is not active, i.e. it has not yet been started, or it has been stopped.</td></tr>
         * <tr><td>TRACK_STAT_OK</td><td>Tracker is tracking normally.</td></tr>
         * <tr><td>TRACK_STAT_RECOVERING</td><td>Tracker has lost the face and is attempting to recover and continue tracking. If it can not recover within the time defined by the parameter recovery_timeout in the tracker configuration file, the tracker will fully re-initialize (i.e. it will assume that a new user may be present).</td></tr>
         * <tr><td>TRACK_STAT_INIT</td><td>Tracker is initializing. The tracker enters this state immediately when it is started, or when it has lost the face and failed to recover (see TRACK_STAT_RECOVERING above). The initialization process is configurable through a number of parameters in the tracker configuration file.</td></tr>
         * </table>
         *
         * When the status is TRACK_STAT_INIT, the method returns additional initialization status information through its arguments (for other states the arguments are neither used nor modified by the method).
         * The purpose of this is to display the initialization status on the screen in order to aid the user in positioning their face in the best pose for initialization.
         *
         * When the status is TRACK_STAT_OK, the method returns an additional tracking quality information through its track_quality argument (for other states the argument is neither used nor modified by the method).
         *
         * @param display_status Output argument; set only while tracker is initializing. It indicates whether the display of initialization status is enabled or disabled by the init_display_status parameter in the tracker configuration file. If it is true, initialization status should be displayed. This allows the display to be controlled from the configuration file.
         * @param face_detected Output argument; set only while tracker is initializing. It returns true if a face and its main features are currently detected, false otherwise. If this value is false, no values are set for yaw, roll and velocity.
         * @param yaw Output argument; set only while tracker is initializing. It returns a measure of head yaw (left-right rotation of the head). It is expressed in meters, as the deviation of the nose tip position from the imaginary line drawn between the eyes perpendicular to left eye - right eye connecting line. Positive values are to the user's right (i.e. to the left in the image).
         * @param roll Output argument; set only while tracker is initializing. It returns head roll (tilt of the head to left or right). It is expressed in degrees. Positive values are to the user's left (i.e. to the right in the image).
         * @param velocity Output argument; set only while tracker is initializing. It returns the estimated velocity of nose tip motion, in meters per second.
         * @param track_quality Output argument; set only while tracker is tracking (TRACK_STAT_OK). It returns the estimate of tracking quality level for the current frame. The value is between 0 and 1, and it corresponds to the global_bad_match_threshold parameter in the tracker configuration file, i.e. the quality measure is checked against this threshold and when it falls below the tracker resets itself.
         * @return The current status of the tracker, see table above.
         */
        int getTrackingStatus(bool &display_status, bool &face_detected, float &yaw, float &roll, float &velocity, float &track_quality);
        
        /* Get the tracker image resolution.
         *
         * This function returns the resolution of tracker input image, working image and display.
         * The input resolution is the resolution of the input video file or camera.
         * Depending on the configuration file, the tracker may reduce the input image to working resolution.
         * Finally, the display resolution is the resolution of the image presented in the application window; this resolution is also set in the configuration file.
         *
         * @param ix returns the horizontal input resolution.
         * @param iy returns the vertical input resolution.
         * @param wx returns the horizontal work resolution.
         * @param wy returns the vertical work resolution.
         * @param dx returns the horizontal display resolution.
         * @param dy returns the vertical display resolution.
         */
        void getResolution(int &ix, int &iy, int &wx, int &wy, int &dx, int &dy);
        
        /* Get the rotation of the face (head).
         *
         * This function returns the estimated rotation of the head, in radians.
         * It can return either the initial rotation (the rotation the first frame
         * of the video) or the current rotation.
         * Rotation is expressed with three values determining the rotations
         * around the three axes x, y and z, in radians. This means that the values represent
         * the pitch, yaw and roll of the head, respectively. The zero rotation
         * (values 0, 0, 0) corresponds to the face looking straight ahead.
         * Positive values for pitch correspond to head turning down.
         * Positive values for yaw correspond to head turning left (from its own perspective).
         * Positive values for roll correspond to head rolling to the right (from its own perspective).
         * The values are in radians.
         *
         * @param initial if true, the function returns initial rotation; otherwise it returns the current rotation.
         * @return pointer to the array with the rotation values around x, y and z axes (yaw, pitch and roll).
         */
        float *getFaceRotation(bool initial);
        
        /* Get the translation of the face (head).
         *
         * This function returns the current estimated translation of the head,
         * either relative to its initial position in the first video frame or absolute (i.e. relative to the camera position).
         * A typical application using relative translation is character animation, while absolute translation is usefull for example in AR type of applications.
         *
         * Translation is expressed with three coordinates x, y, z.
         * When looking towards the video image, the direction of x is to the
         * left, y iz up, and z points forward into the image.
         *
         * If relative is TRUE, the function returns the translation of the head relative to its
         * position in the first video frame. The coordinates are normalised with respect to the eye separation
         * distance of the tracked face. This means that the value of 1
         * corresponds to the translation equal to the distance between the
         * person's eyes.
         *
         * If relative is TRUE, the function can return compensated or uncompensated
         * translation depending on the value of the parameter compensated.
         * Uncompensated translation is the raw translation estimated by the tracker.
         * Compensated translation values take into account the fact that the tracker
         * uses a relatively flat face model for tracking, so the center of rotation of this model
         * is in the front area of the head, while the anatomical center
         * of rotation is behind, in the base of the neck. Therefore, when the rotation
         * is applied to a 3D head model with anatomically correct center of rotation, the
         * face naturally translates as well. When this translation is compounded with
         * the translation values obtained from the tracker, the total resulting translation
         * is exaggerated. To avoid this exaggerated translation of the animated head,
         * the translation can be compensated. The compensation algorithm
         * estimates how much the translation would be exaggerated, and makes it that much
         * smaller. The compensated translation can directly be applied to animated head
         * models that use the neck base as the center of rotation, and is expected to give
         * better results than the uncompensated translation. The compensation can be fine-tuned
         * using the @ref translation_compensation_factor attribute.
         *
         * If relative is FALSE, the function returns the position of the head
         * relative to the camera, i.e. the absolute head position in the camera space. The parameter "compensated"
         * is ignored.
         * If the value set for the camera
         * focal length in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file
         * corresponds to the real camera used the scale of the coordinates shall be correct, in meters; otherwise the scale of the translation values is not known, but the relative values are still correct (i.e. moving towards the camera results in negative values of z coordinate).
         * The absolute translation, rotation returned by getFaceRotation() and the camera focus value FBFT::f together form the 3D coordinate system of the head in its current position
         * and they can be used to align 3D rendered objects with the head for AR or similar applications. The relative facial feature coordinates returned by getFeaturePoints()
         * can be used to align rendered 3D objects to the specific features of the face, like putting virtual eyeglasses on the eyes.
         *
         *
         * @param relative if true, function returns relative translation from the position in the initial video frame; otherwise it returns absolute translation (from the camera).
         * @param compensated if true, and if "relative" is also true, function returns compensated translation, otherwise it returns uncompensated translation.
         * @return pointer to the array with the x, y, z translation values.
         *
         * @see translation_compensation_factor
         * @see getFaceRotation()
         * @see getFeaturePoints()
         * @see FBFT
         */
        float *getFaceTranslation(bool relative, bool compensated);
        
        
        /* Get the textured 3D model of the face (head) in current pose.
         *
         * This method is currently experimental and has not been fully tested.
         *
         * This method returns the 3D model internally used by the tracker. The model is fitted in 3D to the
         * face in the current video frame. The model is a single textured 3D triangle mesh.
         *
         * The texture of the model is the current video frame. This means that, when the model is drawn using the correct
         * perspective (defined by focal length f, image width and height and model rotation and translation), it exactly recreates the facial part of the image. This can be used, for example, to
         * cut out the face from the rest of the image (NOTE: if this is the desired effect, the model with closed mouth should be used otherwise when the mouth opens a hole appears.
         * The model candide3-ClosedMouth.wfm is provided for this purpose and should simply be specified instead of the default Candide3.wfm in the model_filename statement in the tracker configuration file).
         *
         * There are multiple potential uses for this function. Some ideas include, but are not limited to:
         *
         * - Draw the model into the Z buffer to achieve correct occlusion of virtual objects by the head in AR applications.
         * - Use texture coordinates to cut out the face from the image.
         * - Draw the 3D model from a different perspective than the one in the actual video.
         * - Insert the model into another video or 3D scene.
         *
         * Note that smoothing parameters are not applied to any values returned by this function.
         *
         * @param trans 3D translation of the head (equivalent to calling getFaceTranslation(false, false) except that this function returns original values without any smoothing, while getFaceTranslation() returns smoothed values if smoothing filter is on).
         * @param rot 3D rotation of the head (equivalent to calling getFaceRotation(false) except that this function returns original values without any smoothing, while getFaceTranslation() returns smoothed values if smoothing filter is on).
         * @param f Camera focal lenght, to be used in setting the perspective if the 3D model has to be rendered correctly aligned with current video image.
         * @param img_width Width of the current video image, which is also the texture for the 3D model.
         * @param img_height Height of the current video image, which is also the texture for the 3D model.
         * @param tex_img The current video image, which is also the texture for the 3D model. IplImage is the image storage class from OpenCV, please refer to OpenCV documentation for details of accessing its data members; the basic members are the size of the image (frame->width, frame->height) and the pointer to the actual pixel data of the image (frame->imageData).
         * @param n_vert Number of vertices in the 3D model.
         * @param vert List of vertex coordinates of the 3D model, x y and z coordinate for each vertex.
         * @param n_tri Number of triangles in the model.
         * @param tri Triangles list Each triangle is described by three indices into the list of vertices vert. Counter-clockwise convention is used for normals direction.
         * @param tex_coord Texture coordinates for the model, a pair of u, v coordinates for each vertex.
         * @return true on success, false if the model can not be returned because the tracker is not active or the face is currently not detected in the video frame.
         *
         * @see getFaceTranslation(), getFaceRotation), VisageSDK::FBFT::f
         */
        bool getFaceModel(const float* &trans, const float* &rot, float &f, int &img_width, int &img_height, const IplImage* &tex_img, int &n_vert, const float* &vert,int &n_tri, const int* &tri, const float* &tex_coord);
        
        /* Get the current video frame.
         *
         * IplImage is the image storage class from OpenCV, please refer to OpenCV documentation for details of accessing its data members; the basic members are the size of the image (frame->width, frame->height) and the pointer to the actual pixel data of the image (frame->imageData).
         *
         * @return pointer to the current video frame, or NULL if tracker is not active.
         */
        const IplImage *getCurrentVideoFrame(){return(isActive() ? (const IplImage *)frame_input : NULL);};
        
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
         * @return inter pupillary distance (IPD) in meters.
         * @see setIPD()
         */
        float getIPD();
        
        /** Detects a credit card magnetic stripe in the current frame.
         *
         * This function attempts to detect a standard-size credit card stripe in the current frame.
         * The search is performed within a search region defined with respect to the location of the face in the frame.
         * The search is performed only while the tracker is tracking a face (tracking status returned by GetTrackingData() is TRACK_STAT_OK).
         * Various parameters of the search can be configured in the tracker configuration file - see the <a href="doc/VisageTracker Configuration Manual.pdf">Tracker Configuration Manual</a> for details.
         *
         * @param size detected stripe width in pixels.
         * @param ratioerror detected stripe ratio error in percentage of the ideal ratio.
         * @param angleerror detected stripe angle error expressed as a cos of the maximum angle deviation.
         * @return true if the stripe was found, false otherwise.
         */
        bool DetectStrip(double &size);
        
        /** Initializes online screen space gaze tracking. Online mode is used when tracking from camera.
         * This method starts the calibration phase of screen space gaze tracking. In the calibration phase the application displays the calibration data on the screen and passes it to the tracker using AddGazeCalibrationPoint().
         * Application is responsible for finishing the calibration phase by calling FinalizeOnlineGazeCalibration().
         * @see AddGazeCalibrationPoint(), FinalizeOnlineGazeCalibration()
         */
        void InitOnlineGazeCalibration();
        
        /** Passes a calibration point to the tracker in online screen space gaze tracking mode.
         * This method is used in online gaze tracking mode to pass the position of the currently displayed calibration point to the tracker. This method should be called once for each calibration point, after the calibration point is displayed on the screen.
         * Position of the calibration point is in normalized screen coordinates. The origin of the coordinate system is in the upper left corner of the screen; the lower right corner has coordinates (1, 1).
         *
         * NOTE:
         * Application is responsible for synchronization between the frequency of passing calibration points to the tracker and the frequency at which the tracker processes video frames.
         * If calibration points are passed faster than the tracker works, it may happen that two (or more) calibration points are passed while the tracker is processing a single video frame.
         * In such case, if the difference in speed is large enough, it is possible that the tracking data for the processed frame does not match to the calibration point. This reduces the quality of calibration and, consequently, estimation.
         *
         *@param x x coordinate of the calibration point in normalized screen coordinates
         *@param y y coordinate of the calibration point in normalized screen coordinates
         *@see ScreenSpaceGazeData, InitOnlineGazeCalibration(), FinalizeOnlineGazeCalibration()
         */
        void AddGazeCalibrationPoint(float x, float y);
        
        /** Finializes online screen space gaze tracking calibration.
         * This method should be called after all calibration data is displayed and passed to the tracker. After this method is called the tracker performs calibration of gaze tracking system using the provided calibration data and the tracking data collected during the calibration phase.
         *
         * After the calibration is finished, screen space gaze postion is obtained by calling getTrackingData() method. The returned FaceData object contains gaze position stored in ScreenSpaceGazeData object, specifically in FaceData::gazeData.
         * @see InitOnlineGazeCalibration(), getTrackingData(), ScreenSpaceGazeData, FaceData, AddGazeCalibrationPoint()
         */
        float FinalizeOnlineGazeCalibration();
        
        /**Initiallizes offline screen space gaze tracking. Offline mode is used when tracking from video file.
         * This method must be called before tracking is started using trackFromVideo. Calibration data is passed to the tracker as a ScreenSpaceGazeRepository object containing a number of calibration points, each consisting of the position of the calibration point in normalized screen coordinates and the frame index in which the calibration point was displayed to the user.
         * The data for each calibration point is stored as a ScreenSpaceGazeData object.
         *
         * During the calibration phase tracker reads calibration points from the provided ScreenSpaceGazeRepository and collects tracking data in corresponding frames of the tracked video sequence.
         * The gaze tracking system is calibrated automatically after all calibration points from the provided repository are used.
         * After the tracking of the whole video sequence is finished, the screen space gaze positions can be obtained by calling getGazeEstimations() method.
         *
         *@param calibrator Pointer to ScreenSpaceGazeRepository containing calibration data.
         *@return fitQuality Percentage of inliers in total sample count used for calibration.
         *@see ScreenSpaceGazeData, ScreenSpaceGazeRepository, getGazeEstimations()
         */
        void InitOfflineGazeCalibration(ScreenSpaceGazeRepository* calibrator);
        
        /**Returns screen space gaze estimation data obtained in offline gaze tracking mode
         *
         * This function is used for obtaining estimation data in offline mode (in online mode use getTrackingData()).
         * It returns the repository containing screen space gaze data for each non - calibration frame of processed sequence.
         *
         * The method returns an empty array if called during tracking.
         *
         * Note that each time this method is called, the memory for returned ScreenSpaceGazeRepository object is deallocated and reallocated.
         *
         * @param repository to be filled with scren sapce gaze data for last tracked sequence
         * @return fitQuality Percentage of inliers in total sample count used for calibration.
         * @see ScreenSpaceGazeRepository, ScreenSpaceGazeData, InitOfflineGazeCalibration()
         **/
        float getGazeEstimations(ScreenSpaceGazeRepository* repository);
        
        /** \if IOS_DOXY\elseif ANDROID_DOXY\else Show the camera settings dialog.
         *
         * Opens the camera settings dialog box, allowing the user to set parameters such as brightness, contrast, gain etc.
         * The dialog opens only if the tracking from camera is currently active and if the camera_input parameter in the tracker configuration file is set to 0 (this is the default setting).
         *
         * @return true on success, false if dialog is not shown
         * \endif
         */
        bool showCameraSettingsDialog();
        
        
        /** Get Face Animation Parameters (FbaAction implementation).
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
        
        /** Start the action (FbaAction implementation).
         *
         * Note: Do not call this function directly.
         *
         * This function implements the FbaAction interface.
         */
        void start(long globalTime);
        
        /** Stop tracking (FbaAction implementation).
         *
         * This function implements the FbaAction interface. It stops the tracking.
         *
         */
        void stop();
        
        /** Action name (FbaAction implementation).
         *
         */
        char* actionTypeName() {return "VisageTracker2";};
        
        void SetScalingFactor(float factor);
        
        void SetEstimatorGaze(bool state);
        
        void SetGazeOffset(float x, float y);
        void SetGazeScale(float x, float y);
        
        void SetGazeViewportSize(int w, int h);
        
        void reset();
        volatile bool doReset;
        
        void SetGazeCenter(float x, float y);
        
#ifdef IOS
        /** Set data bundle
         *
         * Used to set bundle from which data files will be read. Default is main bundle.
         *
         */
        void setDataBundle(NSBundle *bundle);
#endif
        
        TrackerOpenGLInterface *oglIface;
        TrackerOpenGLInterface *tmpOglIface;
        TrackerGUIInterface *guiIface;
        TrackerInternalInterface *internalIface;
        void display_func();
        
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
        
        int auto_init; /* Control the automatic initialisation. 0 = semi-automatic initialisation of the tracker; 1 = fully automatic tracking with fast initialisation. Initially it is set through the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file.*/
        
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
        
        bool display_video; /* Toggle video display on/off; initially it is set through the<a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file and usually it is on, so video will be visible while tracking. */
        bool display_model_texture; /* Toggle model texture display on/off; initially it is set through the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file. */
        bool display_tri_mask; /* Toggle triangle mask display on/off (triangle mask determines where track points are looked for); initially it is set through the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file. */
        bool display_model_wireframe; /* Toggle wireframe model display on/off; initially it is set through the <a href="#config">tracker configuration file</a>. */
        int display_results; /* Control the display of tracking results. 0 = no results display; 1 = display tracked feature points; 2 = display head pose; 4 = display eye locations; values can be added together (e.g. 6 = display head pose and eye locations). Initially it is set through the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file. */
        bool display_track_points; /* Toggle the display of track points on/off (track points are automatically found points in the face that are suitable for tracking - they are not the same as feature points and their display is mainly useful for monitoring the functioning of the tracker); initially it is set through the <a href="#config">tracker configuration file</a>.. */
        
        float translation_compensation_factor; /* Compensation factor for translation results. Default value is 1.0. Bigger values result in more compensation being applied. If it is set to 0, no compensation is applied. For details about translation compensation see getFaceTranslation()*/
        
        int smoothing_translation; /* Smoothing value for translation results. It must be set between 0 (no smoothing) and 10 (maximal smoothing). Initially it is set through the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file.. Smoothing reduces tracking noise and makes results smoother but it introduces delay so it should be used with caution.*/
        int smoothing_rotation; /* Smoothing value for rotation results. It must be set between 0 (no smoothing) and 10 (maximal smoothing). Initially it is set through the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file.  Smoothing reduces tracking noise and makes results smoother but it introduces delay so it should be used with caution.*/
        int smoothing_fp; /* Smoothing value for feature point results. It must be set between 0 (no smoothing) and 10 (maximal smoothing). Initially it is set through the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration</a> file.  Smoothing reduces tracking noise and makes results smoother but it introduces delay so it should be used with caution.*/
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
        
        FBFT *fbft; /* Tracker engine implementation. */
        IplImage* frame_input; // Current video frame, input; may be color or grayscale
        IplImage* frame_gray; // Current video frame converted to grayscale; all processing is done on grayscale images
        IplImage* best_frame; // Frame with best pose found during initialisation; grayscale; used only during initialisation
        IplImage* gl_image; // OpenGL window image, used for presenting the video image in the OpenGL window.
        IplImage* gl_image_gray; // OpenGL window image, grayscale, used for presenting grayscale video image in the OpenGL window.
        bool PoseTestTimedOut; //PoseTestTimedOut is set to true if pose test in WaitFrontalPose finished with a timeout or false if pose test found good pose.
        //Variables used for debugging, usually in batch processing mode from FaceTracker2; if set to -1 they are not used (that is the default)
        int startFrame; // start processing video from this frame
        int endFrame; // end processing at this frame
        int slowdownFrame; //insert pause to slow down for debugging from this frame
        int slowdownTime;  // slow down each frame by this time, in milliseconds
        
        bool IsAutoStopped; // reason for stopping
        float VideoFrameRate;
        
        int Update(void);
        void Finish(void);
        
        float targetFps;
        
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
        
        SmoothingFilter sf;
        //VisageFeaturesDetector *m_Detector;
        VisageDetector *m_Detector;
        
        long getCurrentTimeMs(bool init);
        
        bool grabFrame(bool init);
        
        IplImage *frame_buffers[3];
        int a_frame;
        int p_frame;
        int v_frame;
        
        bool waitFrontalPose(FDP *latestFP,FDP *bestFP, bool strict,bool &presenceTest,bool poseTest,int parts=VS_FACE);
        bool fitModelToFace(FDP *latestFP,bool init_tracker);
        bool fitModelToFaceNew(FDP *latestFP, bool init_tracker, bool reinit = false);
        bool verifyPose(FDP *f);
        bool testPose(FDP *f, FDP *best);
        bool refitModelToFace();
        void calculateScreenSpaceGaze();
        
        static void  trackInThread(void* vt);
        void writeBits(FILE *streamHandle, unsigned char *bits, int size);//void writeBits(int streamHandle, unsigned char *bits, int size);
        
        static void trackInThread2(void* vt);
        
        int Init(void);
        
        bool testFrontalPose(FDP *latestFP, FDP *bestFP,bool& reco_timeout, int parts=VS_FACE);
        
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
        volatile bool active;
        volatile bool inThread;
        bool toFile;
        bool editing; //used by display function; when true, display function ignores other display flags and displays only the wireframe model for editing
        bool fitting; //used by display function to display the wireframe model in different color while face fitting process is on, mainly for debugging purposes
        HANDLE trackingThreadHandle;/*!Handle to the thread used for tracking*/
        HANDLE detectorThreadHandle;/*!Handle to the thread used for detecting face*/
        CvCapture* capture;/*!<Structure for the capture of video stream, from OpenCV*/
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
        
        int frameCount; // frame count from beginning of tracking
        double frameTime; // duration of one frame in milliseconds
        long pts; // presentation time stamp, calculated by grabFrame()
        long pts_data; // time stamp
        int pts_frame;
        
        float gaze[2]; //gaze direction
#ifdef IOS
        NSBundle *dataBundle;
#endif
        
        //stuff copied from main.cpp in Nils' original project
        volatile bool tracking_ok; //tracking status OK
        f32 trackingFrameRate; // measured tracking frame rate
        iu32 cam_input; //0: videoinput library; 1: OpenCV
        iu32 cam_device; //camera device number
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
        void gl_clear (
		);
        
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
        void smoothGazeData(void);
        void smoothGazeEstimations(void);
        
        CvMat *smoothing_factors;
        
        volatile bool inSwap;
        volatile bool inRead;
        volatile int turn;
        
        TrackingData* trackingData;
#if defined(IOS) || defined(ANDROID) || defined(MAC_OS_X)
        pthread_mutex_t mutex;
#endif
        
#ifdef WIN32
        CRITICAL_SECTION trackingData_cs;
#endif
        
        string bdts_data_path; // path to the folder containing boosted decision trees
        VisageDetector* m_DetectorBDFS;
        
        static void detectInThread(void* arg);
        volatile bool inDetectorThread;
        volatile bool stopDetectorThread;
        
        IplImage* detector_frame;
        FDP* detector_fp;
        volatile int facialFeat;
        volatile int detector_parts;
        volatile int detect;
        volatile int new_detector_frame;
        int windowScaler;
        ScreenSpaceGazeRepository* calibrator;
        ScreenSpaceGazeData currentGazeData;
        ScreenSpaceGazeRepository* estimations;
        Estimator* estimator;
        Estimator* fpEstimator;
        int calibFrameCount;
        int calibIndex;
        int startFrameOffset;
        float initGazeX;
        float initGazeY;
        
        CvMat* projection;
        CvMat* neutral;
    };
    
}
#endif // __VisageTracker2_h__

