#import "FaceDetectViewController.h"
#import "VisageFeaturesDetectorWrapper.h"
#import "MobileCoreServices/UTCoreTypes.h"

@implementation FaceDetectViewController

@synthesize detectImage;
@synthesize detectToolbar;
@synthesize imgPicker;
@synthesize popover;
@synthesize detectorWrapper;


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    
    [super viewDidLoad];

    // init image picker
    if (imgPicker == nil)
    {
        self.imgPicker = [[UIImagePickerController alloc] init];
        self.imgPicker.delegate = self;
        self.imgPicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        self.imgPicker.mediaTypes = [[NSArray alloc] initWithObjects: (NSString *) kUTTypeImage, nil];
    }
    
    // init face detector
    if (detectorWrapper == nil)
    {
        self.detectorWrapper = [[VisageFeaturesDetectorWrapper alloc] init];
        [self.detectorWrapper initDetector];
    }
}

-(IBAction)loadImageAction:(id)sender {
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad){
        self.popover = [[UIPopoverController alloc] initWithContentViewController:self.imgPicker];
        [self.popover presentPopoverFromRect:CGRectMake(0.0, 0.0, 400.0, 400.0) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
    }
    else {
        [self presentModalViewController:imgPicker animated:YES];
    }
}
     
- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image
                      editingInfo:(NSDictionary *)editingInfo
{
    
    [self dismissModalViewControllerAnimated:YES];
    
  
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        [popover dismissPopoverAnimated:YES];
    }
    
    // detect features from loaded image
    UIImage* featuresImage = [detectorWrapper drawFeatures:image];
    [imageView setImage:featuresImage];
}

- (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissModalViewControllerAnimated:YES];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [self.imgPicker release];
    [imageView release];
    [detectorWrapper release];
    [super dealloc];
}


@end
