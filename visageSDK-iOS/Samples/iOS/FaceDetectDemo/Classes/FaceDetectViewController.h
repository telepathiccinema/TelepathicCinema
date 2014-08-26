#import <UIKit/UIKit.h>

@class VisageFeaturesDetectorWrapper;

/**
 * Class that implements default view controller and GUI actions.
 *
 * Most method calls are forwarded to the VisageFeaturesDetectorWrapper object that contains specific implementions.
 *
 */
@interface FaceDetectViewController : UIViewController <UIAlertViewDelegate,UINavigationControllerDelegate, UIImagePickerControllerDelegate, UIPopoverControllerDelegate> {
    
    /**
     * Toolbar for controlling the app.
     * 
     */
	IBOutlet UIToolbar *detectToolbar;
    
    /**
     * Image picker for selecting an image file from album.
     *
     */
    UIImagePickerController *imgPicker;
    
    /**
     * Popover for the image picker.
     *
     */
    UIPopoverController *popover;

    /**
     * View for displaying results.
     * 
     */
    IBOutlet UIImageView *imageView;
    
    /**
     * Wrapper for the VisageFeaturesDetector.
     */
    VisageFeaturesDetectorWrapper *detectorWrapper;
}

@property (nonatomic, retain) UIImage *detectImage;
@property (nonatomic, retain) IBOutlet UIToolbar *detectToolbar;
@property (nonatomic, retain) UIImagePickerController *imgPicker;
@property (nonatomic, retain) UIPopoverController *popover;
@property (nonatomic, retain) VisageFeaturesDetectorWrapper *detectorWrapper;

/**
 * Method for startig tracking from an image.
 * 
 */
-(IBAction)loadImageAction:(id)sender;

@end
