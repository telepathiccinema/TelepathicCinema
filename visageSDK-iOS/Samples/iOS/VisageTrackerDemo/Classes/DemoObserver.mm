#include <cv.h>
#import "DemoObserver.h"
#include "Matrix4.h"
#include "Vector3.h"

using namespace VisageSDK;

DemoObserver::DemoObserver(void)
{
    notifyCount = 0;
    
    // write information header to console
    // here user can initialize data saving

    //NSLog(@"No. | Status | FPS | Position: (x, y, z) | Rotation: (pitch, yaw, roll)\n");
};

void DemoObserver::Notify(VisageTracker2 *vt, long timeStamp)
{
    notifyCount++;

	int trackingStatus = vt->getTrackingData(&data);
    
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

    float r[3] = {0};
    float t[3] = {0};
    if (trackingStatus == TRACK_STAT_OK) {
        for(int i=0;i<3;i++) {
            r[i] = data.faceRotation[i] * 180.0f / 3.14159f;  // rotation in rads, converting to degrees
            t[i] = data.faceTranslation[i] * 100.0f; //translation is expressed in meters so this gives approximate values in centimeters if camera focus distance parameter f in fbft is set correctly
        }
    }
    
    // since returned results are in the coordinate system with right eye in the center
    // they must be transformed to desired coordinate system

    Matrix4 eyeT;
    eyeT.translation(t[0], t[1], t[2]);
    Matrix4 eyeR(r[0], r[1], r[2], V_EULER_YXZ);

    // it is assumed by the tracker that distance between eyes is 6.5cm, so right eye is 3.25cm from face center
    Vector3 offset(3.25f, -3.f, 0.0f);
    
    // x is left-right, y is up-down, z is pointing away from the camera
    Vector3 headTranslation = offset.mul(eyeR*eyeT);
    
    // here user can save data to a file or custom data structure instead of displaying in the console
    //NSLog(@"%d | %s | %4.1f | %+5.1f %+5.1f %+5.1f | %+5.1f %+5.1f %+5.1f", notifyCount, trackingStatusString, data.frameRate, headTranslation.x, headTranslation.y, headTranslation.z, r[0], r[1], r[2]);
    
};

