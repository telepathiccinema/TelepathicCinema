#ifndef GUARD_SIMPLETRACKERCORE
#define GUARD_SIMPLETRACKERCORE

#include "FDP.h"
#include "cv.h"
#include "VisageDetector.h"

namespace VisageSDK
{
	class SimpleTrackerCore
	{

	friend class VisageTracker2;

	public:
		SimpleTrackerCore(VisageDetector* detector);
		
		~SimpleTrackerCore(void);

		void resetTrackerState();

		bool getTrackingPoints(const IplImage &grayFrame, FDP* featurePoints);

	private:
		bool findFace(IplImage* grayFrame, CvRect &boundingBox);

		bool trackFaceFastDetector(IplImage* grayFrame, CvRect* boundingBox);

		VisageDetector* m_detector;

		FDP* m_previousFeaturePoints;


		//temp
		float m_lastModelRollAngle;

		float m_workingResizeFactor;

		float m_workingXOffset;

		float m_workingYOffset;

		std::string m_diagnosticString;
	};
}

#endif
