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
#include "VisageGazeTracker.h"
#include "FaceData.h"
#include "VisageTrackerObserver.h"
#include "VisageTrackerFrameGrabber.h"
#include "TrackerGUIInterface.h"
#include "TrackerOpenGLInterface.h"

#endif // __VisageVision_h__

