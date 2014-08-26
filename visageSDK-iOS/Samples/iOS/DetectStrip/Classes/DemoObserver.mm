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

double averageEyeDistance(std::list<double> samples)
{
	double a = 0.0;
	for (std::list<double>::iterator i = samples.begin(); i != samples.end(); i++)
		a += *i;
    
	return a / samples.size();
}

double eyeDistanceVariance(std::list<double> samples, double average)
{
	double a = 0.0;
	for (std::list<double>::iterator i = samples.begin(); i != samples.end(); i++)
		a += (*i - average) * (*i - average);
    
	return a / samples.size();
}

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
        
        // detect strip
        double size = 0;
        if (vt->DetectStrip(size))
        {
            // get eye image coordinates
            FDP* points = data.featurePoints2D;
            CvPoint rightEye;
            CvPoint leftEye;
            rightEye.x = points->getFP(3, 6).pos[0] * data.frame->width;
            rightEye.y = (1.0 - points->getFP(3, 6).pos[1]) * data.frame->height;
            leftEye.x = points->getFP(3, 5).pos[0] * data.frame->width;
            leftEye.y = (1.0 - points->getFP(3, 5).pos[1]) * data.frame->height;
            
            // get eye image distance
            float pixelEyeDistance = sqrt((rightEye.x - leftEye.x) * (rightEye.x - leftEye.x) + (rightEye.y - leftEye.y) * (rightEye.y - leftEye.y));
            
            // get ppm from strip pixel card width and real world card width
            float ppm = size / 0.0856;
            
            //********************************************************
            // WARNING: The code provided in this example of using
            // functions from visage|SDK to automatically compute
            // the Inter-Pupillary Distance (IPD) based on comparison
            // wih an object of known size. Such implementation may
            // infringe on existing patents, including US Patent no. 6,535,223.
            // Developers should be aware of this and/or other patents
            // if including this or similar IPD computation code
            // in commercial products. 
            // 
            // visage|SDK API and libraries do not in themselves infringe
            // on any patents.
            //********************************************************
           // get real eye distance
            float realEyeDistance = pixelEyeDistance / ppm;
            
            printf("eyed: %f\n", realEyeDistance);
            
            if (0.05 < realEyeDistance && realEyeDistance < 0.08)
            {
                // take sample
				samples.push_back(realEyeDistance);
				if (samples.size() > nSamples)
					samples.pop_front();
                
				if (samples.size() >= nSamples)
				{
					// get average
					double avg = averageEyeDistance(samples);
                    
					// get variance
					double variance = eyeDistanceVariance(samples, avg);
					printf("variance: %f\n", variance);
					if (variance < varianceThreshold)
					{
						vt->setIPD(avg);
						samples.clear();
					}
				}
            }
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

