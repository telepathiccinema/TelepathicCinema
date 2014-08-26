//
//  VisageTrackerUnityPlugin
//

#ifdef WIN32
#include "StdAfx.h"
#include "VisageTrackerUnityPlugin.h"
#include "FolderManager.h"

#include "VisageTracker2.h"
#include "TrackingData.h"

#define PLUGIN_TEXTURE_FORMAT GL_RGB
#define CHANNEL_COUNT 3
#elif defined (IOS)
#import "VisageTrackerUnityPlugin.h"

#import "Utilities.h"

#import "VisageTracker2.h"
#import "FaceData.h"

#define PLUGIN_TEXTURE_FORMAT GL_RGBA
#define CHANNEL_COUNT 4
#else
#define PLUGIN_TEXTURE_FORMAT GL_RGB
#define CHANNEL_COUNT 3
#endif

using namespace VisageSDK;

static VisageTracker2* m_Tracker = 0;

static const int MaxVertices = 256;
static const int MaxTriangles = 256;
static const float m_pi = 3.14159265f;
static const float inv_pi = 1.0f/m_pi;

// Helper method to create C string copy
static char* MakeStringCopy (const char* val)
{
	if (val == NULL)
		return NULL;
	
	char* res = (char*)malloc(strlen(val) + 1);
	strcpy(res, val);
	return res;
}

static unsigned int GetNearestPow2(unsigned int num)
{
    unsigned int n = num > 0 ? num - 1 : 0;
    
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    
    return n;
}
/** Implementaion of VisageTrackerObserver interface.
*
* This class recieves and stores tracking data from the tracker until it is needed by the client application.
*/
class SimpleObserver : public VisageTrackerObserver {
public:
	VisageTracker2 *tracker;
    FaceData data;
	IplImage *ImageA;
	IplImage *ImageB;
	int TextureID;
	char CurrentBuffer;
	char ReadBuffer;
	bool InNotify;
	bool InBindTexture;
	int LastTrackStatus;
	int TrackStatus;
	int frame_width;
	int frame_height;
    float xTexScale;
    float yTexScale;
	bool inited;
	
	char message[256];
	
	
	SimpleObserver()
	{
		TextureID = -1;
		
		TrackStatus = 0;
		
		CurrentBuffer = 'A';
		
		frame_width = 0;
		frame_height = 0;
		
		ImageA = NULL;
		ImageB = NULL;
        
		message[0] = '\0';

		inited = false;
	}
	
	SimpleObserver(int width, int height)
	{
		TextureID = -1;
		
		TrackStatus = 0;
		
		CurrentBuffer = 'A';
		
		frame_width = width;
		frame_height = height;
        
        xTexScale =  width / (float) GetNearestPow2(width);
        yTexScale = height / (float) GetNearestPow2(height);
		
		ImageA = cvCreateImage(cvSize(frame_width, frame_height), IPL_DEPTH_8U, CHANNEL_COUNT);
		ImageB = cvCreateImage(cvSize(frame_width, frame_height), IPL_DEPTH_8U, CHANNEL_COUNT);
        
		message[0] = '\0';

		inited = true;
	}
	
	~SimpleObserver() {
		cvReleaseImage(&ImageA);
		cvReleaseImage(&ImageB);
	}

	void InitImageBuffers(int width, int height, int depth)
	{
		frame_width = width;
		frame_height = height;
		xTexScale =  width / (float) GetNearestPow2(width);
		yTexScale = height / (float) GetNearestPow2(height);
		ImageA = cvCreateImage(cvSize(frame_width, frame_height), IPL_DEPTH_8U, depth);
		ImageB = cvCreateImage(cvSize(frame_width, frame_height), IPL_DEPTH_8U, depth);
		inited = true;
	}
	
	void Notify(VisageTracker2 *vt, long timeStamp)
	{
        
		InNotify = true;
        
		tracker = vt;
        
        int track_stat = tracker->getTrackingData(&data);

		if (!inited && data.frame)
			InitImageBuffers(data.frame->width, data.frame->height, data.frame->nChannels);
		
		const char *tstatus;
		switch (track_stat) {
			case TRACK_STAT_OFF:
				tstatus = "OFF";
				break;
			case TRACK_STAT_OK:
				tstatus = "OK";
				break;
			case TRACK_STAT_RECOVERING:
				tstatus = "RECOVERING";
				break;
			case TRACK_STAT_INIT:
				tstatus = "INITIALIZING";
				break;
			default:
				tstatus = "N/A";
				break;
		}
		
		//NSLog(@" %4.1f FPS \n Head position %+5.1f %+5.1f %+5.1f \n Rotation (deg) %+5.1f %+5.1f %+5.1f \n Resolution: input %dx%d  work %dx%d \n\n",trackingFrameRate,t1[0],t1[1],t1[2],r1[0],r1[1],r1[2],ix,iy,wx,wy);
		sprintf(message, " %4.1f FPS  Status: %s\n Head position %+5.1f %+5.1f %+5.1f \n Rotation (deg) %+5.1f %+5.1f %+5.1f \n\n",
                data.frameRate,
                tstatus,
                data.faceTranslation[0],
                data.faceTranslation[1],
                data.faceTranslation[2],
                data.faceRotation[0],
                data.faceRotation[1],
                data.faceRotation[2]);
		
		LastTrackStatus = TrackStatus;
		TrackStatus = track_stat;

		if (data.frame != NULL && CurrentBuffer == 'A' && ReadBuffer != 'B' && TrackStatus != TRACK_STAT_OFF)
		{
			if(data.frame->nChannels == 1) {
				cvCvtColor(data.frame,ImageB,CV_GRAY2RGB);
			} else {
				memcpy(ImageB->imageData, data.frame->imageData, data.frame->imageSize);
			}
			CurrentBuffer = 'B';
		}
		else if (data.frame != NULL && CurrentBuffer == 'B' && ReadBuffer != 'A' && TrackStatus != TRACK_STAT_OFF)
		{
			if(data.frame->nChannels == 1) {
				cvCvtColor(data.frame,ImageA,CV_GRAY2RGB);
			} else {
				memcpy(ImageA->imageData, data.frame->imageData, data.frame->imageSize);
			}
			CurrentBuffer = 'A';
		}
        
		InNotify = false;
        
	}
};

static SimpleObserver *observer = NULL;

namespace VisageSDK
{
	// prototype for licesning
    void initializeLicenseManager(char *licenseKeyFileFolder);
}

// When native code plugin is implemented in .mm / .cpp file, then functions
// should be surrounded with extern "C" block to conform C function naming rules
extern "C" {
    
	void _initTracker (char* config)
	{
#ifdef WIN32
		// simple test to check if a valid OpenGL context exists
		GLint redBits=0;
		glGetIntegerv (GL_RED_BITS, &redBits);
		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
			MessageBox( 0, "No OpenGL context detected. Video texture will not be displayed.\nStart the application with -force-opengl argument. If you are running the VisageTrackerUnity.exe, simply run VisageTrackerUnity.cmd instead.", "OpenGL Error",
			MB_OK | MB_ICONERROR );

		visageFolderManager.init();
#endif
        //initialize licensing
        //example how to initialize license key
        //initializeLicenseManager("license-key-filename.vlc");

		m_Tracker = new VisageTracker2(config);

		observer = new SimpleObserver();
		
		m_Tracker->attach(observer);
	}

	// starts tracking from the camera
	bool _trackFromCam()
	{
		if (m_Tracker == NULL)
			return false;

		return m_Tracker->trackFromCam();
	}
    
	void _startTracker()
	{
        _trackFromCam();
	}

	void _stopTracker ()
	{
		m_Tracker->stop();
	}
	
	const char* _getTrackingResults ()
	{
#ifdef WIN32
		char _allLabel[200];
        
		if (observer == NULL)
			_allLabel[0] = '\0';
		else
			sprintf(_allLabel, "%s", observer->message);
		
		char* allLabel = MakeStringCopy(_allLabel);
#elif defined(IOS)
		NSString* _allLabel;
        
		if (observer == NULL)
			_allLabel = @"";
		else
			_allLabel = [NSString stringWithUTF8String: observer->message];
		
		char* allLabel = MakeStringCopy([_allLabel UTF8String]);
#endif
		return allLabel;
	}
	
	// binds a texture with the given native hardware texture id through opengl
	void _bindTexture(int texID)
	{
		observer->InBindTexture = true;
		
		if (observer->CurrentBuffer == 'A' && observer->ImageA != NULL)
		{
			observer->ReadBuffer = 'A';
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, observer->frame_width, observer->frame_height, PLUGIN_TEXTURE_FORMAT, GL_UNSIGNED_BYTE, observer->ImageA->imageData);
			observer->ReadBuffer = 'N';
		}
		else if (observer->CurrentBuffer == 'B' && observer->ImageB != NULL)
		{
			observer->ReadBuffer = 'B';
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, observer->frame_width, observer->frame_height, PLUGIN_TEXTURE_FORMAT, GL_UNSIGNED_BYTE, observer->ImageB->imageData);
			observer->ReadBuffer = 'N';
		}
		
		observer->InBindTexture = false;
	}
	
	// gets the current translation
	void _get3DData(float* tx, float* ty, float* tz,float* rx, float* ry, float* rz, int* status)
	{
		if (observer->TrackStatus != TRACK_STAT_OK) {
			*tx = -10000.0f;
			*ty = -10000.0f;
			*tz = 0.0f;
		} else {
			*tx = -observer->data.faceTranslation[0];
			*ty = observer->data.faceTranslation[1];
			*tz = observer->data.faceTranslation[2];
		}
		*rx = observer->data.faceRotation[0]*180.0f*inv_pi;
		*ry = -(observer->data.faceRotation[1]+m_pi)*180.0f*inv_pi;
		*rz = -observer->data.faceRotation[2]*180.0f*inv_pi;
		*status = observer->TrackStatus;
	}
    
    bool _getCameraInfo(float *focus, int *width, int *height)
    {
        *focus = 3.0f;
		*width = observer->frame_width;
		*height = observer->frame_height;

		return observer->inited;
    }
	
	// a helper function to get all the needed info in one native call
	bool _getFaceModel(int* vertexNumber, float* vertices, int* triangleNumber, int* triangles, float* texCoord)
	{
		if (observer == NULL || observer->tracker == NULL || observer->TrackStatus == TRACK_STAT_OFF)
			return false;
		
		// get vertex number
		*vertexNumber = observer->data.faceModelVertexCount;
		
		// get vertices
		memcpy(vertices, observer->data.faceModelVertices, 3*(*vertexNumber)*sizeof(float));
        
		// get triangle number
		*triangleNumber = observer->data.faceModelTriangleCount;
		
		// get triangles in reverse order
		for(int i = 0; i < *triangleNumber * 3; i++)
		{
			triangles[*triangleNumber * 3 - 1 - i] = observer->data.faceModelTriangles[i];
		}
        
        //texture coordinates are normalized to frame
        //and because frame is only in the part of our texture
        //we must scale texture coordinates to match
        //frame_width/tex_width and frame_height/tex_height
        //also x-coord must be flipped
        for (int i = 0; i < *vertexNumber*2; i+=2) {
            texCoord[i+0] = (1.0f - observer->data.faceModelTextureCoords[i+0]) * observer->xTexScale;
            texCoord[i+1] = observer->data.faceModelTextureCoords[i+1] * observer->yTexScale;
        }
        
		return true;
	}
	
}