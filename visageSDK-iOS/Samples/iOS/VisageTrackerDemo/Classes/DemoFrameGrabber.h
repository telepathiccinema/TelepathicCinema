
#include "visageVision.h"

using namespace VisageSDK;

/**
 * Class that implements a simple frame grabber that serves a single image to the tracker in order to
 * demonstrate the raw image input mechanism. The image is read from a file.
 * 
 */
class DemoFrameGrabber : public VisageTrackerFrameGrabber {
public:
	
    /**
     * Contructor.
     *
     * Initialises the frame grabber by reading an image from a file.
     * 
     */
	DemoFrameGrabber(void);
    
    /**
     * Destructor.
     * 
     * Releases the frame.
     *
     */
    ~DemoFrameGrabber(void);
    
    /**
     * Frame that is served to the tracker.
     * 
     */
    IplImage *frame;
    
    int width;
    int height;
    int format;
    int origin;

    /**
	 * Implementation of a virtual function from VisageTrackerFrameGrabber.
	 * 
	 * VisageTracker2 calls this function periodically to get the new frame to process.
     * In this simple implementation same frame is returned every time.
	 */
	unsigned char *GrabFrame(long &timeStamp);
};