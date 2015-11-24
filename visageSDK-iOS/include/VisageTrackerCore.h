#ifndef GUARD_VISAGETRACKERCORE
#define GUARD_VISAGETRACKERCORE

#include "FDP.h"
#include "cv.h"
#include "VisageDetector.h"

namespace VisageSDK
{
	class VisageTrackerCore
	{

	friend class VisageTracker2;

	public:
		VisageTrackerCore(VisageDetector* detector);
		
		~VisageTrackerCore(void);

		void resetTrackerState();

		bool getTrackingPoints(const IplImage &grayFrame, FDP* featurePoints);

		bool getTrackingPoints_prevFrameOL(const IplImage &grayFrame, FDP* featurePoints);

		bool getTrackingPoints_trackerless(const IplImage &grayFrame, FDP* featurePoints);

	private:
		bool findFace(IplImage* grayFrame, CvRect &boundingBox);

		bool trackFaceFastDetector(IplImage* grayFrame, CvRect* boundingBox);

		bool trackFaceOpticalFlow(IplImage* previousGrayFrame, IplImage* currentGrayFrame, FDP* previousDetections, CvRect* boundingBox, CvRect* calculatedBoundingBox);
		
#ifdef WIN32
		static unsigned int __stdcall findFacesInThread(void* arg);
#else
		static void findFacesInThread(void* arg);
#endif
		void createDetectorThread();

		void stopDetectorThread();

		HANDLE m_detectorThreadHandle;

		CvRect m_threadBoundingBox;

		IplImage* m_threadFrame;
		
		volatile bool m_stopDetectorThread;

		volatile bool m_useDetectorThread;

		volatile bool m_faceDetected;
	
		volatile bool m_detectFaces;

		VisageDetector* m_detector;

		TrackerStates m_trackerState;

		IplImage* m_previousFullFrame;

		CvRect* m_previousBoundingBox;

		CvRect* m_previousCalculatedBoundingBox;

		FDP* m_previousFeaturePoints;

		float m_lastModelRollAngle;

		float m_lastModelYawAngle;

		float m_workingResizeFactor;

		float m_workingXOffset;

		float m_workingYOffset;

		std::string m_diagnosticString;

		int m_frameIndex;
	};
}

#endif
