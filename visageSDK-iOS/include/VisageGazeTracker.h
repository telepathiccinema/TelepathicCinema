#ifndef VISAGEGAZETRACKER_H
#define VISAGEGAZETRACKER_H

#include "ScreenSpaceGazeMapping.h"
#include "VisageGazeEstimation.h"
#include "TrackerGazeCalibrator.h"
#include "VisageTracker2.h"
#include "VisageEstimator.h"


#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif


namespace VisageSDK
{
/**
* VisageGazeTracker extends VisageTracker2 functionality, adding screen space gaze tracking on top of face tracking. 
* For information about using the facial/head tracking refer to VisageTracker2.
*
* Screen space gaze tracking estimates gaze position (the location on the screen where the user is looking) in normalized screen coordinates. 
* Screen space gaze tracking works in two phases: calibration and estimation. 
*
* In the calibration phase, the system is calibrated for gaze estimation by passing the calibration data to the tracker. Calibration data consists of series of points displayed on screen. The user looks at the calibration point. During the calibration phase tracker collects calibration points and matching tracking data for each point.
* After all calibration points have been passed to the tracker, the tracker performs calibration of gaze tracking system and switches to estimation phase. 
*
* In the estimation phase the tracker estimates gaze location in screen space coordinates and returns the data as ScreenSpaceGazeData object for each frame. 
*
* Screen space gaze tracking works in two different modes. Online mode is used when tracking in real time from camera. 
* Offline mode is used when tracking from video files. The key differences between the two modes are:
* <table>
* <tr><td width="600"><b>ONLINE</b></td><td width="600"><b>OFFLINE</b></td></tr>
* <tr><td>Each FaceData object returned by getTrackingData() contains screen space gaze data for the current frame.</td>
* <td>Gaze estimation is done as a post process after the tracking is finished for the whole video sequence and returned as ScreenSpaceGazeRepository object. FaceData objects returned by the tracker during tracking contain no screen space gaze data for current frame.</td></tr>
* <tr><td>Calibration data is passed to the tracker as it is shown on screen.</td>
* <td>Calibration data is passed as a ScreenSpaceGazeRepository object for the whole video sequence.</td></tr>
* <tr><td>Online mode can be initalized at any time during tracking.</td>
* <td>Offline mode has to be initalized before tracking is started. </td></tr>
* <tr><td>The application is responsible for finalizing gaze tracking system calibration.</td>
* <td>Calibration is finalized automatically.</td></tr>
* </table>
*
*
* <h3> Online screen space gaze tracking</h3>
* Online mode implies using the screen space gaze tracking when tracking from camera. 
*
* It is initialized by calling InitOnlineGazeCalibration() method. This method prepares the tracker for real time gaze tracking calibration.
* Each calibration point in normalized screen coordinates is passed to tracker by calling AddGazeCalibrationPoint(). It is expected that the point is displayed on the screen before calling the method and that the user looks at calibration points during the calibration.
* Application is responsible for reading or generating the calibration data, displaying it on screen and synchronization with the tracker.
* It is required to manually notify the tracker that calibration is finished (once all calibration points are used) by calling FinalizeOnlineGazeCalibration() method. Once this method is called the tracker performs calibration of screen space gaze tracking system using provided calibration data and tracking data collected during the calibration process.
* 
* After the system is calibrated the estimation phase starts. Estimations are returned as part of FaceData objects obtained by calling getTrackingData() or track() method, specifically in FaceData::gazeData.
*
* @see InitOnlineGazeCalibration(), AddGazeCalibrationPoint() FinalizeOnlineGazeCalibration(), ScreenSpaceGazeData, FaceData, VisageTracker2
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
* After the whole video file has been processed, tracking stops and gaze estimations are available as ScreenSpaceGazeRepository object, obtained by calling GetGazeEstimations() method. The returned repository contains ScreenSpaceGazeData object with screen space gaze position for each non-calibration frame of the tracked video sequence.
*
* @see InitOfflineGazeCalibration(), GetGazeEstimations(), ScreenSpaceGazeRepository, ScreenSpaceGazeData 
*/
class VISAGE_DECLSPEC VisageGazeTracker : public VisageTrackerObserver, public VisageTracker2
{
	public:
	
	/** Constructor.
	*
	* @param trackerConfigFile the name of the tracker configuration file (.cfg; default configuration files are provided in Samples/data folder.
	* Constructor must be called with the full path to the configuration file. The configuration file is in the same directory as other data files needed by the tracker. Because it is in the same directory its path can be parsed to set the path for the other data files.
	* For further details see <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>\if IOS_DOXY and section on <a href="../../doc/creatingxc.html#config_selection">device-specific configuration selection</a>.\endif).
	*/
		VisageGazeTracker(const char* trackerConfigFile);

	/* Constructor.
	*
	* @param oglInterface the TrackerOpenGLInterface object or NULL if not used.
	* @param guiInterface the TrackerGUIInterface object or NULL if not used; in that case the tracker will only function in fully automatic mode. Semi-automatic modes require TrackerGUIInterface to be implemented.
	* @param trackerConfigFile the name of the tracker configuration file (.cfg; default configuration files are provided in  \if IOS_DOXY <a href="../../Samples/iOS/data">Samples/iOS/data</a> \elseif ANDROID_DOXY  <a href="../../Samples/Android/data">Samples/Android/VisageTrackerUnityDemo/data</a>
	* Constructor must be called with the full path to the configuration file. The configuration file is in the same directory as other data files needed by the tracker. Because it is in the same directory its path can be parsed to set the path for the other data files.
	* \else Samples/OpenGL/data/FaceTracker2 \endif; for further details see <a href="../VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>\if IOS_DOXY and section on <a href="../../doc/creatingxc.html#config_selection">device-specific configuration selection</a>.\endif).
	* @see TrackerOpenGLInterface
	* @see TrackerGUIInterface
	*/
		//VisageGazeTracker(TrackerOpenGLInterface *oglInterface, TrackerGUIInterface *guiInterface, const char* trackerConfigFile);
	
	/** Destructor.
	*/
		~VisageGazeTracker();


	/** Initializes online screen space gaze tracking. Online mode is used when tracking from camera.
	* This method starts the calibration phase of screen space gaze tracking. In the calibration phase the application displays the calibration data on the screen and passes it to the tracker using AddGazeCalibrationPoint(). 
	* Application is responsible for finishing the calibration phase by calling FinalizeOnlineGazeCalibration().
	* 
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
	* This method must be called before tracking is started using VisageTracker2::trackFromVideo(). Calibration data is passed to the tracker as a ScreenSpaceGazeRepository object containing a number of calibration points, each consisting of the position of the calibration point in normalized screen coordinates and the frame index in which the calibration point was displayed to the user. 
	* The data for each calibration point is stored as a ScreenSpaceGazeData object.
	* 
	* During the calibration phase tracker reads calibration points from the provided ScreenSpaceGazeRepository and collects tracking data in corresponding frames of the tracked video sequence.
	* The gaze tracking system is calibrated automatically after all calibration points from the provided repository are used.
	* After the tracking of the whole video sequence is finished, the screen space gaze positions can be obtained by calling GetGazeEstimations() method.
	*
	*@param calibrator Pointer to ScreenSpaceGazeRepository containing calibration data.
	*@return fitQuality Percentage of inliers in total sample count used for calibration.
	*@see ScreenSpaceGazeData, ScreenSpaceGazeRepository, GetGazeEstimations() 
	*/
		void InitOfflineGazeCalibration(ScreenSpaceGazeRepository* calibrator, bool normalized = true);

	/**Returns screen space gaze estimation data obtained in offline gaze tracking mode
	*
	* This function is used for obtaining estimation data in offline mode (in online mode use getTrackingData()). 
	* It returns the repository containing screen space gaze data for each non - calibration frame of processed sequence.
	* 
	* The method returns an empty array if called during tracking. 
	*
	* Note that each time this method is called, the memory for returned ScreenSpaceGazeRepository object is deallocated and reallocated.
	*
	* @param repository to be filled with screen space gaze data for last tracked sequence
	* @return fitQuality Percentage of inliers in total sample count used for calibration. This is a measure of the quality of the calibration process, and thus the expected quality of the gaze estimations.
	* @see ScreenSpaceGazeRepository, ScreenSpaceGazeData, InitOfflineGazeCalibration() 
	**/
		float GetGazeEstimations(ScreenSpaceGazeRepository* repository);

		void Notify(VisageTracker2* vt, long timestamp);

		void SetResolution(int width, int height);

		void SetGazeViewportSize(int w, int h);
		void SetScalingFactor(float factor);
		void SetEstimatorGaze(bool state);
		void SetGazeOffset(float x, float y);
		void SetGazeScale(float x, float y);
		void SetGazeCenter(float x, float y);

	private:
	
		ScreenSpaceGazeData gazeData;

		ScreenSpaceGazeRepository* estimations;
		ScreenSpaceGazeRepository* calibrator;

		GazeEstimationPredictor* lxpredictor;
		GazeEstimationPredictor* rxpredictor;

		GazeEstimationPredictor* lypredictor;
		GazeEstimationPredictor* rypredictor;

		VisageEstimator* estimator;

		bool isRealTime;
		bool isCalibrated;
		bool predicting;

		void addOfflineScreenSpaceGazePoint();
		void calculateOnlineScreenSpaceGaze();

		ScreenSpaceGazeData Predict(EyeData& data, float q35, float q36);
		void CalculateQuality(float& error_x, float& error_y);
		void smoothGazeData();
		void smoothGazeEstimations();

		int frameIndex;
		int startFrameOffset;

		float quality;
		long lastTimestamp;

		static int lew;
		static int leh;
		static int rew;
		static int reh;
		
		int width; 
		int height;

		int nCalib;

		float prev_x;
		float prev_y;

		float initGazeX;
		float initGazeY;

		float bestD[2];

		int resolution[2];

		bool normalize;

};
}
#endif