
#include "visageVision.h"

using namespace VisageSDK;

/**
* The simple observer class demonstrating the use of the VisageTrackerObserver mechanism to obtain tracking data
* and display them in the debug console.
*
*/
class DemoObserver : public VisageTrackerObserver {
public:
	
    /**
     * Constructor
     *
     */
	DemoObserver(void);
    
    /**
     * Memeber variable that counts how many time Notify() function was called.
     */
    int notifyCount;
    
    /**
     * Member variable that serves for getting the tracking results.
     */
    FaceData data;
	
    /**
     * Implementation of a virtual function from VisageTrackerObserver.
     *
     * This function is called by VisageTracker2 during tracking 
     * whenever a new video frame is processed.
     *
     * In this implementation it gets the tracking data and status and displays them in the debug console.
     */
	void Notify(VisageTracker2 *vt, long timeStamp);
};