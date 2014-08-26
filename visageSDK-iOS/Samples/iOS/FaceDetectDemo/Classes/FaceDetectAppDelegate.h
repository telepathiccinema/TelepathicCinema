#import <UIKit/UIKit.h>

@class FaceDetectViewController;

/*! \mainpage Face Detect Demo sample project
 *
 * The Face Detect sample project demonstrates facial features detection based on the VisageSDK::FacialFeaturesDetector class.
 *
 * \section building Bulding and running the project
 *
 * The FaceDetectDemo sample projects is located in the Samples/iOS/FaceDetectDemo subfolder of the visage|SDK for iOS folder.
 *
 * Prerequisites to running the sample project are installed Xcode (versions 4.x) and an iOS device (iPhone 4S or iPad 2 (or newer) recommended). Sample project will also work in the simulator.
 * 
 * Project is opened in Xcode by double-clicking on the FaceDetectDemo.xcodeproj project file. To run the project an iOS device first must be attached to the computer. The project sample app can than be run by chosing Run option in Xcode.
 *
 *
 * \section usage Using the sample app
 *
 * The sample app consists of a single button which loads an image from the device gallery. If any features were detected, they are drawn on the loaded image and shown on the screen.
 *
 * \section implementation Implementation overview
 *
 * The visage|SDK API is written in C++ and since development on iOS is done in Objective-C a special way of interfacing the two must be employed. In this sample project this is done through VisageFeaturesDetectorWrapper Objective-C class that exposes Objective-C methods for using the tracker, while the implementation of those methods is done in Objective-C++, a combination of Objective-C and C++.
 *
 * The FaceDetectViewController (Objective-C class) connects with GUI (that is defined in MainWindow.xib file) and implements actions that calls to methods from VisageFeaturesDetectorWrapper class to detect and display facial features.
 *
 * The specific classes and methods that demonstrate visage|SDK are:
 * - VisageFeaturesDetectorWrapper: Provides the Objective-C interface to main face detector functions.
 * - VisageFeaturesDetectorWrapper::drawFeatures: Method that is used to detect and display the detected features.
 
 * Other important classes are:
 * - FaceDetectAppDelegate: Implements the app delegate.
 * - FaceDetectViewController: Impements default view controller and app gui. This class calls methods that are implemented in VisageFeaturesDetectorWrapper class.
 *
 * More details about each class is available in documentation for specific class.
 *
 *  
 * Users are free to modify the sample project to test the functionalities of the Facial Features Detector. The sample project also serves as a starting point when creating new projects that will use visage|SDK.
 *
 */

/**
 * The app delegate.
 * Implements the app delegate that connects app window and view controller.
 */
@interface FaceDetectAppDelegate : NSObject <UIApplicationDelegate> {
    IBOutlet UIWindow *window;
	IBOutlet FaceDetectViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) FaceDetectViewController *viewController;
@end

