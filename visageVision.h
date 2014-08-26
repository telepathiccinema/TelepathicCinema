
#ifndef __VisageVision_h__
#define __VisageVision_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

// including only needed visageVision headers

#include "VisageTracker2.h"
#include "TrackingData.h"
#include "FaceData.h"
#include "VisageTrackerObserver.h"
#include "VisageTrackerFrameGrabber.h"
#include "TrackerGUIInterface.h"
#include "TrackerOpenGLInterface.h"

#endif // __VisageVision_h__

