#import <UIKit/UIKit.h>

@class VisageTrackerViewController;

/*! \mainpage VisageTrackerDemo sample project
 *
 * The VisageTrackerDemo sample project demonstrates real-time head/facial features tracking from camera, video file or image based on the VisageSDK::VisageTracker2 tracker class.
 *
 * \section building Bulding and running the project
 *
 * The VisageTrackerDemo sample projects is located in the Samples/iOS/VisageTrackerDemo subfolder of the visage|SDK for iOS folder.
 *
 * Prerequisites to running the sample project are installed Xcode (versions 4.x) and an iOS device (iPhone 4S or iPad 2 (or newer) recommended; iPhone 4 works with reduced performance). Sample project will also work in simulator but without possibility of tracking from camera as there is no support for camera in simulator.
 * 
 * Project is opened in Xcode by double-clicking on the VisageTrackerDemo.xcodeproj project file. To run the project an iOS device first must be attached to the computer. The project sample app can than be run by chosing Run option in Xcode.
 *
 *
 * \section usage Using the sample app
 *
 * The sample app consists of a toolbar with five buttons labeled: Camera, Video, Image, Stop and Quit:
 * - Camera button starts tracking from front camera.
 * - Video button starts tracking from the sample video file bundled with the sample app.
 * - Image button starts tracking from a single image bundled with the sample app.
 * - Stop button stops the tracker.
 * - Quit buton quits the sample app.
 *
 * The following images show how sample app looks like when it is started and when "Image" button is pressed:
 *
 * \image latex images/sample1.png "App started" width=5cm
 * \image latex images/sample2.png "App is tracking" width=5cm
 *
 *
 * \section implementation Implementation overview
 *
 * The visage|SDK API is written in C++ and since development on iOS is done in Objective-C a special way of interfacing the two must be employed. In this sampe project this is done through TrackerWrapper Objective-C class that exposes Objective-C methods for using the tracker, while the implementation of those methods is done in Objective-C++, a combination of Objective-C and C++.
 *
 * The VisageTrackerViewController (Objective-C class) connects with GUI (that is defined in MainWindow.xib file) and implements actions that calls to methods from TrackerWrapper class to start and stop the tracker and display the tracking results using CustomGLView object.
 *
 * The specific classes and methods that demonstrate visage|SDK are:
 * - TrackerWrapper: Provides and Objective-C interface to main tracker functions (start, stop, get results); displays tracking results.
 * - DemoObserver: Demonstrates how to use the VisageTrackerObserver mechanism to obtain tracking data
 * and display them in the debug console.
 * - DemoFrameGrabber: Demonstrates how to implement a custom source of frames to feed to tracker through the raw image input mechanism.
 * - TrackerWrapper::displayTrackingResults: Method that is used to get and display tracking results, video frame and 3D face model.
 
 * Other important classes are:
 * - VisageTrackerAppDelegate: Implements the app delegate.
 * - VisageTrackerViewController: Impements default view controller and app gui. This class calls methods that are implemented in TrackerWrapper class.
 * - CustomGLView: Impements OpenGL ES view for results display.
 *
 * More details about each class is available in documentation for specific class.
 *
 *  
 * Users are free to modify the sample project to test the functionalities of the Visage tracker. The sample project also serves as a starting point when creating new projects that will use visage|SDK.
 *
 */

/**
 * The app delegate.
 * Implements the app delegate that connects app window and view controller.
 */
@interface VisageTrackerAppDelegate : NSObject <UIApplicationDelegate> {
    IBOutlet UIWindow *window;
	IBOutlet VisageTrackerViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) VisageTrackerViewController *viewController;
@end

