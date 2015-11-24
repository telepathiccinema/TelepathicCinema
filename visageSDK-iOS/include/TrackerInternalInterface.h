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


#ifndef __TrackerInternalInterface_h__
#define __TrackerInternalInterface_h__

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
#ifdef WIN32
#include <windows.h>
#endif
#include "TrackerOpenGLInterface.h"
#include "TrackerGUIInterface.h"

using namespace std;

namespace VisageSDK
{

//internal implementation of TrackerOpenGLInterface and TrackerGUIInterface
class TrackerInternalInterface : public TrackerOpenGLInterface, public TrackerGUIInterface
{
public:

	//constructor
	TrackerInternalInterface();

	//destructor
	~TrackerInternalInterface();

	// display error message and exit - this is the only thing the internal TrackerGUIInterface implementation does
	void errorAndExit(const char *message);

	//functions from TrackerGUIInterface
	char* ChooseSuFileName(){errorAndExit(0);return 0;};
	void displayMessage(char *userMessage,char *caption,int type){errorAndExit(userMessage);};
	void ManuallyAdjustShapeMask(float *face_translation, float *face_rotation, float *face_scale, float *face_shape){errorAndExit(0);};
	void signalWhenUserReady(char *userMessage,bool *readyFlag){errorAndExit(0);};
	int selectMainFacialFeatures(IplImage* input, FDP* output){errorAndExit(0); return 0;};

	//funtions from TrackerOpenGLInterface
	void setOpenGLContext();
	void checkOpenGLSize(unsigned int &width, unsigned int &height);
	void swapOpenGLBuffers();

private:
	int width, height;
	int glutMainWindow;
};

}
#endif // __TrackerInternalInterface_h__

