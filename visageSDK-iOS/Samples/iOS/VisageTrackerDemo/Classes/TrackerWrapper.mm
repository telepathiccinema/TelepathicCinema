#import "TrackerWrapper.h"
#import "Utilities.h"

#import "visageVision.h"
#import "DemoObserver.h"
#import "DemoFrameGrabber.h"
#import "Matrix4.h"
#import "Vector3.h"
#import "gluLookAt.h"
#import "UIDeviceHardware.h"


using namespace VisageSDK;

@implementation TrackerWrapper

@synthesize glView;

// comment this to use head tracking configuration
#define FACE_TRACKER

- (void)initTracker:(CustomGLView *)view
{
    //initialize multithreading
    BOOL isMT = [self startMultithread];
    
    //initialize licensing
    //example how to initialize license key
    //initializeLicenseManager("license-key-filename.vlc");
    
    
	glView = view;
    demoFrameGrabber = 0;
    demoObserver = new DemoObserver();

    // choose configuration based on device at run-time
    NSString* deviceType = [UIDeviceHardware platform];
    
    //NSLog(deviceType);
#ifdef FACE_TRACKER
    if ([deviceType hasPrefix:@"iPhone3"] ||           // iPhone4
        [deviceType hasPrefix:@"iPhone4"] ||           // iPhone4S
        [deviceType hasPrefix:@"iPad2"]                // iPad2
        )
        tracker = new VisageTracker2("Facial Features Tracker - Low.cfg");
    else
        tracker = new VisageTracker2("Facial Features Tracker - High.cfg");      // all other devices
#else
    tracker = new VisageTracker2("Head Tracker.cfg");
#endif

	//get OpenGL context size
	glWidth = glView.bounds.size.width;
	glHeight = glView.bounds.size.height;
    
    inGetTrackingResults = false;
}

- (void)startTrackingFromCam
{
    NSString* deviceType = [UIDeviceHardware platform];
    
    // portrait mode
    int cam_width = 480;
    int cam_height = 640;
    
    // override for iPhone 4
    if ([deviceType hasPrefix:@"iPhone3"]) {	// iPhone4
        cam_width  = 144;
        cam_height = 192;
    }
    
#if TARGET_IPHONE_SIMULATOR
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                    message:@"No camera available on simulator."
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
#else
    [self stopTracker];
    
    while(inGetTrackingResults)
        ;
    
    tracker->attach(demoObserver);
    
    tracker->trackFromCam(NULL, VISAGE_CAMERA_UP, VISAGE_FRAMEGRABBER_FMT_RGB, true, -1, cam_width, cam_height);
#endif
}

- (void)startTrackingFromVideo:(NSString *)filename
{
	[self stopTracker];
    
    while(inGetTrackingResults)
        ;

    tracker->attach(demoObserver);

    if (filename) {
        tracker->trackFromVideo([filename UTF8String]);
    } else {
        NSString *bundlefile = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"jam1.mp4"];
        tracker->trackFromVideo([bundlefile UTF8String]);
    }
}

- (void)startTrackingFromImage
{
	[self stopTracker];
    
    while(inGetTrackingResults)
        ;

    tracker->attach(demoObserver);
	
    // creating custom implementation of the VisageFrameGrabber
	demoFrameGrabber = new DemoFrameGrabber();
	tracker->trackFromRawImages(demoFrameGrabber,
                                demoFrameGrabber->width,
                                demoFrameGrabber->height,
                                demoFrameGrabber->format,
                                demoFrameGrabber->origin);
}

- (void)stopTracker
{
	tracker->stop();
    tracker->detach();
	delete demoFrameGrabber;
    demoFrameGrabber = 0;
    if (demoObserver) {
        demoObserver->notifyCount = 0;
    }
}

- (void)dummyThread:(id)object
{
	int n = 0;
	n++;
}

- (BOOL)startMultithread
{
	[NSThread detachNewThreadSelector:@selector(dummyThread:) toTarget:self withObject: nil];
	return [NSThread isMultiThreaded];
}

#include <mach/mach_time.h>

#define MEASURE_FRAMES 10
uint64_t last_times[MEASURE_FRAMES];
int framecount = -1;
int last_pts = 0;

- (void)displayTrackingResults:(UILabel *)fpsLabel Status:(UILabel *)statusLabel Info:(UILabel *)infoLabel
{
    // results are gathered only when observer has been notified at least once
    if (demoObserver != 0 && demoObserver->notifyCount == 0) {
        return;
    }
    
    inGetTrackingResults = true;
    
	int trackingStatus = tracker->getTrackingData(&trackingData);
    
    int pts = (int)trackingData.timeStamp;
    
    if (last_pts == pts) {
        inGetTrackingResults = false;
        return;
    }

    last_pts = pts;
    
    mach_timebase_info_data_t timeBaseInfo;
	mach_timebase_info(&timeBaseInfo);

    //measure the frame rate
	uint64_t currentTime = mach_absolute_time() * timeBaseInfo.numer / (timeBaseInfo.denom * 1e6);
	if(framecount == -1)
	{
		framecount = 0;
		for(int i=0;i<10;i++)
			last_times[i]=0;
	}
	framecount++;
	if(framecount == MEASURE_FRAMES) framecount = 0;
	float displayFrameRate = MEASURE_FRAMES * 1000.0f / (float)(currentTime - last_times[framecount]);
	last_times[framecount] = currentTime;

    
	if (trackingStatus != TRACK_STAT_OFF) {
        
        //drawing results
		[glView setOpenGLContext];
        
        float videoAspect = trackingData.frame->width / (float) trackingData.frame->height;
        glHeight = glWidth / videoAspect;
        
        VisageRendering::DisplayResults(&trackingData, trackingStatus, glWidth, glHeight);
        
		[glView swapOpenGLBuffers];
	}
    
	const char *trackingStatusString;
	switch (trackingStatus) {
		case TRACK_STAT_OFF:
			trackingStatusString = "OFF";
			break;
		case TRACK_STAT_OK:
			trackingStatusString = "OK";
			break;
		case TRACK_STAT_RECOVERING:
			trackingStatusString = "RECOVERING";
			break;
		case TRACK_STAT_INIT:
			trackingStatusString = "INITIALIZING";
			break;
		default:
			trackingStatusString = "N/A";
			break;
	}
	
	// display the frame rate, position and other info
    
	float r[3] = {0};
	float t[3] = {0};
	if (trackingStatus == TRACK_STAT_OK)
    {
		for(int i=0;i<3;i++) {
			r[i] = trackingData.faceRotation[i] * 180.0f / 3.14159f; //rads to degs
			t[i] = trackingData.faceTranslation[i] * 100.0f; //translation is expressed in meters so this gives approximate values in centimeters if camera focus distance parameter is set correctly in configuration file
		}
	}
    
	[fpsLabel setText:[NSString stringWithFormat:@"%4.1f FPS (tracking)\n %4.1f FPS (display)",trackingData.frameRate,displayFrameRate]];
	[statusLabel setText:[NSString stringWithFormat:@"Status: %s",trackingStatusString]];
	[infoLabel setText:[NSString stringWithFormat:@"Head position %+5.1f %+5.1f %+5.1f \nRotation (deg) %+5.1f %+5.1f %+5.1f",t[0],t[1],t[2],r[0],r[1],r[2]]];
    
    inGetTrackingResults = false;
}



@end
