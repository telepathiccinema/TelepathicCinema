#ifndef SCREENSPACEGAZE_H
#define SCREENSPACEGAZE_H
#include <vector>
#include "cv.h"

#ifdef EMSCRIPTEN
#include <unistd.h>
#include <stdio.h>
#endif
#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

using namespace std;

namespace VisageSDK
{

/** QualityParams is container structure used to hold a number of parameters related to estimating screen space gaze estimation quality.
**/
struct VISAGE_DECLSPEC QualityParams
{
	///** Estimated quality of calibration. Higher values indicate lower number of outliers and better fitting.
	//* This value is set after successful calibration.
	//**/
	//float fitQuality;

	///** Variation of head rotation in estimation frames. 
	//* This value is set after successfully calibration and is updated for each new estimation frame.
	//* In offline mode this value is calculated after processing is finished. In online mode this value is currently not set.
	//**/
	//float varEstimRotation;
	//
	///** Variation of tracking quality in all processed frames.
	//* This value is updated for each new frame.
	//* In offline mode This value is calculated after processing is finished if running in offline mode. In online mode this value is currently not set.
	//**/
	//float varTrackQuality;

	///** Variation of tracking quality in calibration frames.
	//* In offline mode this value is calculated after processing is finished. In online mode this value is updated for each new calibration frame.
	//**/
	//float varCalibTrackQuality;

	///** Variation of head rotation in calibration frames. 
	//* In offline mode this value is calculated after processing is finished. In online mode this value is updated for each new calibration frame.
	//**/
	//float varCalibRotation;
	//
	//
	///** Variation of head translation in calibration frames. 
	//* In offline mode this value is calculated after processing is finished. In online mode this value is updated for each new calibration frame.
	//**/
	//float varCalibTranslation;
	//
	///** Average tracking quality in calibration frames.
	//* In offline mode this value is calculated after processing is finished. In online mode this value is updated for each new calibration frame.
	//**/
	//float avgCalibTrackQuality;
	//
	///** Average detection quality in calibration frames.
	//* In offline mode this value is calculated after processing is finished. In online mode this value is updated for each new calibration frame.
	//**/
	//float avgCalibDetectQuality;

	///** Percentage of calibration frames in which tracking status was TRACK_STAT_OK.
	//* In offline mode this value is calculated after processing is finished. In online mode this value is updated for each new calibration frame.
	//**/
	//float calib_perc;

	///** Percentage of estimation frames in which tracking status was TRACK_STAT_OK.
	//* In offline mode this value is calculated after processing is finished. In online mode this value is updated for each new estimation frame.
	//**/
	//float estim_perc;	

	QualityParams()
	{
		CalibTrackQuality = 0;
		EstimTrackQuality = 0;
		EstimCumulativeRotation = 0;
		CalibCumulativeRotation = 0;
		CalibCumulativeTranslation = 0;
		CalibFramesStatus = 0;
		EstimFramesStatus = 0;
		CalibDetectQuality = 0;
		CalibFramesCount = 0;
		CalibOutliersCount = 0;
		CalibDefinedCount = 0;
		EstimFramesCount = 0;

	}
	
	/** 
	* Array holding tracking quality for each calibration frame.
	**/
	float* CalibTrackQuality;
	
	/** Array holding tracking quality for each estimation frame.
	* This value is currently not set in online gaze tracking mode.
	**/
	float* EstimTrackQuality;
	
	/** Array holding cumulative head rotation for each estimation frame. 
	* Cumulative rotation is calculated as a sum of x, y and z head rotations divided by 3.
	* This value is currently not set in online gaze tracking mode.
	**/
	float* EstimCumulativeRotation;
	
	/** Array holding cumulative head rotation for each calibration frame. 
	* Cumulative rotation is calculated as a sum of x, y and z head rotations divided by 3.
	**/
	float* CalibCumulativeRotation;

	/** Array holding cumulative head translation for each calibration frame. 
	* Cumulative translation is calculated as a sum of x, y and z head translations divided by 3.
	**/
	float* CalibCumulativeTranslation;
	
	/** Total number of defined or undefined calibration frames.
	*
	**/
	int CalibFramesCount;
	
	/**Number of defined calibration frames discarded from calibration.
	*
	**/
	int CalibOutliersCount;
	
	/**Number of defined calibration frames,
	*
	**/
	int CalibDefinedCount;
	
	/** Number of estimation frames.
	*	This value is currently not set in online gaze tracking mode.
	**/
	int EstimFramesCount;
	
	/** Array holding tracking status for each calibration frame.
	*
	**/
	int* CalibFramesStatus;

	/** Array holding tracking status for each estimation frame.
	* This value is currently not set in online gaze tracking mode.
	**/
	int* EstimFramesStatus;

	/** Array holding detection quality for each calibration frame.
	*
	**/
	float* CalibDetectQuality;

};

/** ScreenSpaceGazeData is a container structure used to hold the gaze point location, current state of screen space gaze tracking system and gaze tracking estimation quality parameters for the current frame.
* This class is used both to store the calibration points during the calibration phase and the estimation results during the estimation phase.
* 
* In online mode the frame index corresponds to the sum of the number of calibration frames passed to the tracker and the number of frames processed in the estimation phase. 
* 
* In offline mode the index is the frame number in the processed video sequence.
**/
struct VISAGE_DECLSPEC ScreenSpaceGazeData
{
	/**Index of the video frame. 
	* In online mode the frame index corresponds to the sum of the number of calibration frames passed to the tracker and the number of frames processed in the estimation phase. 
	* In offline mode the index is the frame number in the processed video sequence.
	**/
	int index;
	
	/** X coordinate of screen space gaze normalized to screen width. 
	* 0 corresponds to the left-most point and 1 to the right-most points of the screen. Defaults to 0.5.
	**/
	float x;
	
	/**Y coordinate of screen space gaze normalized to screen height.
	* 0 corresponds to the top-most point and 1 to the bottom-most points of the screen. Defaults to 0.5.
	**/
	float y;
	
	/**
	Flag indicating the state of the gaze estimator for the current frame. 
	* If inState is 0 the estimator is off and position is default. 
	*
	* If inState is 1 the estimator is calibrating and returns calibration data for the current frame. 
	* 
	* If inState is 2 the estimator is estimating and returns the estimated screen space gaze coordinates.  
	**/
	int inState;

	bool isFix;

	IplImage* lEyeImage;
	IplImage* rEyeImage;
	int usedEye;

	int calibrationGroup;
	double regularizationWeight;

	ScreenSpaceGazeData();
	~ScreenSpaceGazeData();
};

/**
* Data repository holding ScreenSpaceGazeData objects in offline screen space gaze tracking mode.
* The repository is used provide ScreenSpaceGazeData objects holding calibration data for the tracked video sequence to the tracker, as well as to obtain the results.
* Each ScreenSpaceGazeData object stored in the repository holds the calibration data for a single frame.
* Starting calibration index should be equal or larger than 1. 
*
**/
class VISAGE_DECLSPEC ScreenSpaceGazeRepository
{

public:
	ScreenSpaceGazeRepository();
	~ScreenSpaceGazeRepository();

	/**
	* Returns ScreenSpaceGazeData object for the frame denoted by index.
	**/
	ScreenSpaceGazeData* Get(int index);

	/**
	* Returns the number of ScreenSpaceGazeData objects in repository
	**/
	int GetCount();

	/** 
	* Returns the frame index of the first element in the ScreenSpaceGazeRepository object
	**/
	int GetFirst();

	/**
	* Returns the frame index of the last element in the ScreenSpaceGazeRepository object
	**/
	int GetLast();
	
	/**
	* Add ScreenSpaceGazeData object containing calibration data to repository.
	*@param data ScreenSpaceGazeData object containing calibration data
	**/
	void Add(ScreenSpaceGazeData* data);

	/**
	* Add calibration data consisting of frame index and point location to repository.
	*@param index frame index of calibration data.
	*@param x x coordinate of calibration point.
	*@param y y coordinate of calibration point.
	*
	**/
	void Add(int index, float x, float y);

	void Clear();


private:

	std::vector<ScreenSpaceGazeData*>* repository;

};


//class ScreenSpaceGazeCalibrator : public ScreenSpaceGazeRepository
//{
//public:
//
//	ScreenSpaceGazeCalibrator(void);
//
//	void Add(ScreenSpaceGazeData* data);
//	void Add(int index, float x, float y, int inState);
//
//	ScreenSpaceGazeData* Get(int index);
//
//	int GetCount();
//	
//	int GetFirst();
//
//	int GetLast();
//
//	void Clear();
//
//
//private:
//
//	std::vector<ScreenSpaceGazeData*> repository;
//	
//};
}
#endif
