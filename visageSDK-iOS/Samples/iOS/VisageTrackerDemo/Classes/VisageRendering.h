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

#ifndef __VISAGERENDERING_H__
#define __VISAGERENDERING_H__

#include <vector>
#include <stdio.h>
#include "FaceData.h"

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef IOS
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#endif

#ifdef ANDROID
#include <EGL/egl.h> 
#include <GLES/gl.h>
#endif

#ifdef MAC_OS_X
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

namespace VisageSDK
{

#define D_PI 3.14159265358979323846

#define DISPLAY_FRAME 16
#define DISPLAY_FEATURE_POINTS 1
#define DISPLAY_SPLINES 2
#define DISPLAY_GAZE 4
#define DISPLAY_AXES 8
#define DISPLAY_WIRE_FRAME 32
#define DISPLAY_ALL DISPLAY_FEATURE_POINTS + DISPLAY_SPLINES + DISPLAY_GAZE + DISPLAY_AXES + DISPLAY_FRAME
#define TRACK_STAT_OK 1

#if defined (IOS) || defined(ANDROID)
void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
				GLfloat centerx, GLfloat centery, GLfloat centerz,
				GLfloat upx, GLfloat upy, GLfloat upz);
#endif
	
/** VisageRendering displays the current frame and the following tracking results using OpenGL
* - facial feature points
* - eye closure
* - gaze direction
* - model axes
* Adjacent facial feature points are connected with splines that are calculated using implementation of a Catmull-Rom method
*/
class VisageRendering
{
public:
	/** Method calls other methods for drawing the frame and tracking results
	* @param trackingData - tracking results
	* @param trackStat - tracker status
	* @param width - width of the OpenGL window, adjusted so that the aspect of the drawing frame is perserved
	* @param height - height of the OpenGL window, adjusted so that the aspect of the drawing frame is perserved
	* @param drawingOptions - enables user to choose what tracking results to display; by default all the tracking results are displayed
	*/
	static void DisplayResults(FaceData* trackingData, int trackStat, int width, int height, int drawingOptions = DISPLAY_ALL);

	/** Method draws the current frame
	* @param image - image for drawing
	* @param width - adjusted width of the OpenGL window 
	* @param height - adjusted height of the OpenGL window
	*/
	static void DisplayFrame (const IplImage *image, int width, int height);

	/** Method draws facial feature points
	* @param trackingData - tracking results
	* @param width - adjusted width of the OpenGL window 
	* @param height - adjusted height of the OpenGL window
	* @param _3D - use 3D feature points
	* @param relative - use relative 3D feature points
	*/
	static void DisplayFeaturePoints(FaceData* trackingData, int width, int height, bool _3D=false, bool relative=false);

	/** Method draws splines
	* @param trackingData - tracking results
	* @param width - adjusted width of the OpenGL window 
	* @param height - adjusted height of the OpenGL window
	*/
	static void DisplaySplines(FaceData* trackingData, int width, int height);

	/** Method draws the gaze direction
	* @param trackingData - tracking results
	* @param width - adjusted width of the OpenGL window 
	* @param height - adjusted height of the OpenGL window
	*/
	static void DisplayGaze(FaceData* trackingData, int width, int height);

	/** Method draws model axes
	* @param trackingData - tracking results
	* @param width - adjusted width of the OpenGL window 
	* @param height - adjusted height of the OpenGL window
	*/
	static void DisplayModelAxes(FaceData* trackingData, int width, int height);

	/** Method draws face model
	* @param trackingData - tracking results
	* @param width - adjusted width of the OpenGL window 
	* @param height - adjusted height of the OpenGL window
	*/
	static void DisplayWireFrame(FaceData* trackingData, int width, int height);

	/** Method returns vector of calculated spline points.
	* @param inputPoints - vector of points which need to be connected with a spline
	* @param ratio - number of spline points that need to be calculated between neighbouring input points
	* @param outputPoints - vector of calculated spline points
	*/
	static void CalcSpline(vector <float>& inputPoints, int ratio, vector<float>& outputPoints);
};

}

#endif //__VISAGERENDERING_H__
