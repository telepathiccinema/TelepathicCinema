#import "VisageTrackerViewController.h"
#import "MobileCoreServices/UTCoreTypes.h"
#import "CustomGLView.h"
#import "TrackerWrapper.h"

@implementation VisageTrackerViewController

@synthesize trackImage;
@synthesize resultsDisplayTimer;
@synthesize glView;
@synthesize fpsLabel;
@synthesize statusLabel;
@synthesize infoLabel;
@synthesize trackerToolbar;
@synthesize imgPicker;
@synthesize popover;
@synthesize tracker;

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    
    [super viewDidLoad];

    self.imgPicker = [[UIImagePickerController alloc] init];
    self.imgPicker.delegate = self;
    self.imgPicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    
    self.imgPicker.mediaTypes = [[NSArray alloc] initWithObjects: (NSString *) kUTTypeMovie, nil];
	
	self.tracker = [[TrackerWrapper alloc] init];
	[self.tracker initTracker:glView];
    resultsDisplayTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/60
														   target:self
														 selector:@selector(showResults:)
														 userInfo:nil
														  repeats:YES];
}

-(IBAction)trackVideoAction:(id)sender {
#if TARGET_IPHONE_SIMULATOR
    [self.tracker startTrackingFromVideo:nil];
#else
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        self.popover = [[UIPopoverController alloc] initWithContentViewController:self.imgPicker];
        [self.popover presentPopoverFromRect:CGRectMake(0.0, 0.0, 400.0, 400.0) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
    } else {
        [self presentModalViewController:self.imgPicker animated:YES];
    }
#endif
}

-(IBAction)trackImageAction:(id)sender {
	[self.tracker startTrackingFromImage];
}

-(IBAction)trackStopAction:(id)sender {
	[self.tracker stopTracker];
}

-(IBAction)trackCameraAction:(id)sender {
	[self.tracker startTrackingFromCam];
}

-(IBAction)trackQuitAction:(id)sender {
	exit(0);
}

-(void) imagePickerController: (UIImagePickerController *) picker
didFinishPickingMediaWithInfo: (NSDictionary *) info
{
    NSString *mediaType = [info objectForKey: UIImagePickerControllerMediaType];
    
    NSString *moviePath = nil;

    if (CFStringCompare ((__bridge CFStringRef) mediaType, kUTTypeMovie, 0) == kCFCompareEqualTo)
    {
        moviePath = [[info objectForKey:UIImagePickerControllerMediaURL] path];
    }

    [self dismissModalViewControllerAnimated:YES];
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        [popover dismissPopoverAnimated:YES];
    }

    // if moviePath is remains nil, a default video bundled with app will play (jam1.mp4)
    [self.tracker startTrackingFromVideo:moviePath];

}


-(void)showResults:(NSTimer *)timer {
	[self.tracker displayTrackingResults:fpsLabel Status:statusLabel Info:infoLabel];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

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
	[self.tracker release];
    [self.imgPicker release];
	[resultsDisplayTimer invalidate];
    [super dealloc];
}


@end
