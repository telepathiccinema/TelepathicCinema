#import <UIKit/UIKit.h>

@class CustomGLView;
@class TrackerWrapper;

/**
 * Class that implements default view controller and GUI actions.
 *
 * Most method calls are forwarded to the TrackerWrapper object that contains specific implementions.
 *
 */
@interface VisageTrackerViewController : UIViewController <UIAlertViewDelegate,UINavigationControllerDelegate, UIImagePickerControllerDelegate, UIPopoverControllerDelegate> {
	
    /**
     * Timer for periodically refreshing of the results display.
     * 
     */
	NSTimer *resultsDisplayTimer;
	
    /**
     * Label for FPS display.
     * 
     */
	IBOutlet UILabel *fpsLabel;
    
    /**
     * Label for tracker status display.
     * 
     */
	IBOutlet UILabel *statusLabel;
    
    /**
     * Label for tracker information display.
     * 
     */
	IBOutlet UILabel *infoLabel;
    
    /**
     * Toolbar for controlling the app.
     * 
     */
	IBOutlet UIToolbar *trackerToolbar;
    
    /**
     * Image picker for selectiong video file from album.
     *
     */
    UIImagePickerController *imgPicker;
    
    /**
     * Popover for video picker.
     *
     */
    UIPopoverController *popover;

    /**
     * View for displaying tracking results frame and 3D face wireframe
     * 
     */
	CustomGLView *glView;
    
    /**
     * Wrapper class around C++ calls controlling the tracker.
     * 
     */
	TrackerWrapper *tracker;
}

@property (nonatomic, retain) UIImage *trackImage;
@property (nonatomic, retain) NSTimer *resultsDisplayTimer;
@property (nonatomic, retain) IBOutlet UILabel *fpsLabel;
@property (nonatomic, retain) IBOutlet UILabel *statusLabel;
@property (nonatomic, retain) IBOutlet UILabel *infoLabel;
@property (nonatomic, retain) IBOutlet UIToolbar *trackerToolbar;
@property (nonatomic, retain) UIImagePickerController *imgPicker;
@property (nonatomic, retain) UIPopoverController *popover;

@property (nonatomic, retain) IBOutlet CustomGLView *glView;
@property (nonatomic, retain) TrackerWrapper *tracker;


/**
 * Method for starting tracking from a video.
 * 
 */
-(IBAction)trackVideoAction:(id)sender;
/**
 * Method for startig tracking from an image.
 * 
 */
-(IBAction)trackImageAction:(id)sender;
/**
 * Method for startig tracking from a camera.
 * 
 */
-(IBAction)trackCameraAction:(id)sender;
/**
 * Method for stopinig tracking.
 * 
 */
-(IBAction)trackStopAction:(id)sender;
/**
 * Method for quitting app.
 * 
 */
-(IBAction)trackQuitAction:(id)sender;

@end
