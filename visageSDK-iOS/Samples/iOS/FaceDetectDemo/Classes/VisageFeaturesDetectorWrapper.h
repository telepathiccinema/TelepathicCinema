#import <Foundation/Foundation.h>

// declare C++ implementation for .mm (Obj-C++) files
#ifdef __cplusplus
#include "VisageFeaturesDetector.h"

// neccessary prototype declaration for licensing
namespace VisageSDK
{
    void initializeLicenseManager(char *licenseKeyFileFolder);
}
#endif

// declare C++ implementation for .m (Obj-C) files
#ifdef __OBJC__
#ifndef __cplusplus
typedef void VisageFeaturesDetector;
#endif
#endif

/**
 * Class that implements simple high-level Objective-C interface around visage|SDK Visage Features Detector functionallity.
 *
 * This makes connecting the iOS app GUI with visage|SDK features detector functionaly easier as primary programming language used in iOS apps is
 * Objective-C while primary programming language used in visage|SDK is C++. The class is a subclass of NSObject and
 * it uses Objective-C++ for implementation details.
 */
@interface VisageFeaturesDetectorWrapper : NSObject {
#ifdef __cplusplus
    VisageSDK::VisageFeaturesDetector *visageFeaturesDetector;
#endif
}

/**
 * Method for initializing the detector.
 *
 * This method creates a new VisageFeaturesDetector object  tracking by choosing appropriate configuraion file based on device it is running on.
 */
- (void)initDetector;

/**
 * Method for drawing features on an image.
 *
 * This method detects the features on the input image and returns a new image with the features drawn. 
 */
- (UIImage*)drawFeatures:(UIImage*)image;
@end
