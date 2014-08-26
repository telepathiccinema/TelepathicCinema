#import <Foundation/Foundation.h>
#import "CustomGLView.h"

// declare C++ implementation for .mm (Obj-C++) files
#ifdef __cplusplus
#include "visageVision.h"
#include "DemoFrameGrabber.h"
#include "DemoObserver.h"

// neccessary prototype declaration for licensing
namespace VisageSDK
{
void initializeLicenseManager(char *licenseKeyFileFolder);
}
#endif

// declare C++ implementation for .m (Obj-C) files
#ifdef __OBJC__
#ifndef __cplusplus
typedef void VisageTracker2;
typedef void TrackingData;
typedef void DemoFrameGrabber;
typedef void FDP;
#endif
#endif

/**
 * Class that implements simple high-level Objective-C interface around visage|SDK VisageTracker2 functionallity.
 *
 * This makes connecting the iOS app GUI with visage|SDK tracker functionaly easier as primary programming language used in iOS apps is
 * Objective-C while primary programming language used in visage|SDK is C++. The class is a subclass of NSObject and
 * it uses Objective-C++ for implementation details.
 *
 * The sample application automatically chooses appropriate tracker configuration file based on device it is running on.
 *
 * Key members of this class are:
 * - tracker: the VisageTracker2 object
 * - trackingData: the TrackingData object used for retrieving and holding tracking data
 * - glView: the OpenGL view object used for setting and displaying video frame and tracking results
 * - displayTrackingResults: method that demonstrates how to acquire, use and display tracking data and 3D face model
 * 
 */
@interface TrackerWrapper : NSObject {
	CustomGLView *glView;
#ifdef __cplusplus
	VisageSDK::VisageTracker2* tracker;
    VisageSDK::TrackingData trackingData;
    DemoFrameGrabber *demoFrameGrabber;
	DemoObserver *demoObserver;
    
    /**
     * 
     */
	bool isTracking;
	
    /**
     * Texture ID for displaying frames from the tracker.
     */
	GLuint frameTexId;
    /**
     * Texture ID for displaying instructions image.
     */
	GLuint instructionsTexId;
    /**
     * Texture coordinates for displaying frames from the tracker.
     */
	float xTexCoord;
    /**
     * Texture coordinates for displaying frames from the tracker.
     */
	float yTexCoord;
    /**
     * Size of the texture for displaying frames from the tracker.
     */
	int xTexSize;
    /**
     * Size of the texture for displaying frames from the tracker.
     */
	int yTexSize;
	
    /**
     * Camera focal length for setting correct OpenGL perspective.
     */
	float focus;
	
    /**
     * Aspect of the video.
     */
	float videoAspect;
	
    /**
     * Size of the OpenGL view.
     */
	int glWidth;
    /**
     * Size of the OpenGL view.
     */
	int glHeight;
    
    /**
     *
     */
    bool inGetTrackingResults;
#endif
}

@property (nonatomic, retain) CustomGLView *glView;

/**
 * Method for initializing the tracker.
 *
 * This method creates a new VisageTracker2 objects and initializes tracking by choosing appropriate configuraion file based on device it is running on.
 */
- (void)initTracker:(CustomGLView *)view;
/**
 * Method for starting tracking from a camera.
 */
- (void)startTrackingFromCam;
/**
 * Method for starting tracking from a video.
 *
 * @param filename name of the video file
 */
- (void)startTrackingFromVideo:(NSString *)filename;
/**
 * Method for starting tracking from an image.
 */
- (void)startTrackingFromImage;
/**
 * Method for stoping the tracker.
 */
- (void)stopTracker;
/*
 * Method for forcing multithreded mode.
 *
 * @param object dummy object
 */
- (void)dummyThread:(id)object;
/*
 * Method for forcing multithreded mode.
 */
- (BOOL)startMultithread;
/**
 * Method for initiliazing and setting instructions texture through OpenGL.
 *
 */
- (void) initAndSetInstructionsTexture;
/**
 * Method for displaying instructions through OpenGL.
 */
- (void) displayInstructions;
/**
 * Method for initiliazing video texture through OpenGL.
 *
 * The texture must have dimensions that are power of 2, so video data will cover only part of the texture.
 *
 * @param image frame to init video display with
 */
- (void) initFrameTexture:(const IplImage *)image;
/**
 * Method for refreshing video texture through OpenGL.
 *
 * The part of the texture containing video data is refreshed with current frame data.
 *
 * @param image frame to refresh video display with
 */
- (void) setFrameTexture: (const IplImage *)image;

/**
 * Method for getting the inter pupillary distance.
 */
- (float) getIPD;

/**
 * Method for displaying video through OpenGL.
 */
- (void) displayVideo;
/**
 * Method for drawing point in 3D through OpenGL.
 *
 * @param point 3D coordinates of 3D point (x,y,z)
 * @param color array of four floats representing color as (r,g,b,a)
 * @param size size of the point
 */
- (void) drawPoint3D:(const float*) point withColor: (const float*) color andSize: (float) size;
/**
 * Method for drawing 3D face model.
 *
 * This method takes neccesery 3D model data (vertices, trinagles texture coordinates) and draws them using OpenGL calls.
 * As 3D model texture coordinates are normalized to frame and because frame is only in the part of the texture 
 * the texture coordinates must be scaled to account for that difference. Also the x-coordinate must be flipped.
 *
 * @param vertices array of 3D face model vertices 
 * @param vertexNum number of 3D face model vertices
 * @param triangles array of 3D face model triangles
 * @param triangleNum number of 3D face model trinagles
 * @param texCoords array of 3D face model texture coordinates for each vertex
 * @param width width of the video frame
 * @param height height of the video frame
 */
- (void) drawFaceModel: (const float *) vertices
				  vNum: (int) vertexNum
				  tris: (const int *)triangles
				  tNum:(int) triangleNum
			   tCoords:(const float *) texCoords
			frameWidth:(int)width
		   frameHeight:(int)height;
/**
 * Method for drawing gaze direction.
 *
 * This method takes neccesery gaze direction data and draws them using OpenGL calls.
 *
 * @param gazeDirection gaze direction
 * @param featurePoints3D FDP object containing the feature points
 */
- (void) drawGazeDirection: (const float *) gazeDirection
              withFeatures: (FDP*) featurePoints3D;
/**
 * Method for drawing results of tracking from feature points.
 *
 * @param featurePoints2D FDP object containing the feature points
 */
- (void) drawResultsFromFeaturePoints: (FDP*) featurePoints2D;
/**
 * Helper method for setting the camera and its view (frustum) in OpenGL.
 *
 * @param width width of the frame
 * @param height height of the frame
 * @param f focal length of the camera
 */
+ (void) setupFrustumWithWidth: (int) width
                     andHeight: (int) height
                      andFocus: (float) f;

/**
 * Method for displaying tracking results.
 *
 * This method is periodically called by the app timer to get and display tracking results. The results are retrieved using VisageSDK::TrackingData structure and VisageTracker2::getTrackingData() function and displayed using labels for textual data and OpenGL ES for visual data (frames from camera and 3D face model). It shows how to properly interpret tracking data and setup the OpenGL scene to display 3D face model retrieved from the tracker correctly aligned to the video frame. Using the tracking data the blue rectangle is drawn in the position of eye on the right side of the input image.
 *
 * @param fpsLabel label for displaying frames per second
 * @param statusLabel label for displaying tracking status
 * @param infoLabel label for displaying translation and rotation information
 */
- (void)displayTrackingResults:(UILabel *)fpsLabel Status:(UILabel *)statusLabel Info:(UILabel *)infoLabel;

@end
