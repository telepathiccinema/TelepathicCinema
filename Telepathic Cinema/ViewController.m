//
//  ViewController.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/20/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "ViewController.h"
#import "CustomGLView.h"
#import "TrackerWrapper.h"
#import "TelepathicCinema.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize tracker;
@synthesize glView;
@synthesize resultsDisplayTimer;
@synthesize tc;
@synthesize movieLayer;


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    self->state = STATE_VIDEO;
    
    self.tracker = [[TrackerWrapper alloc] init];
	[self.tracker initTracker:glView];
    
    resultsDisplayTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/30
														   target:self
														 selector:@selector(update:)
														 userInfo:nil
														  repeats:YES];
    [self.tracker startTrackingFromCam];
    [self setupVideoPlayer];
    tc = [[TelepathicCinema alloc] initWithView:glView withScene:@"calibration.smil" withPlayer:self.mPlayer withTracker:tracker];
    [self.view.layer addSublayer:tc.overlay];
    
}

-(void) setupVideoPlayer
{
    
    self.mPlayer = [AVQueuePlayer queuePlayerWithItems:[NSArray arrayWithObjects:nil]];
    
    self.mPlayer.actionAtItemEnd = AVPlayerActionAtItemEndNone;
    
    movieLayer = [AVPlayerLayer playerLayerWithPlayer:self.mPlayer];
    
    movieLayer.frame = self.view.bounds;
    
    [self.view.layer addSublayer: movieLayer];
    
}


-(void)update:(NSTimer *)timer {
    if(state == DEBUG)
    {
        [self.tracker displayTrackingResults];
        [self.tc draw];
    }
    [self.tc update: self.mPlayer withTracker:self.tracker];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)onTouch:(id)sender {
    if(self->state == STATE_VIDEO)
    {
        [[self.view.layer.sublayers objectAtIndex:0] setOpacity:0.5];
        [self.tracker display:YES];
        self->state = STATE_DEBUG;
        [self.tc display:YES];
        
    }else{
        [self.tracker blank];
        [self.tracker display:NO];
        [[self.view.layer.sublayers objectAtIndex:0] setOpacity:1.0];
        
        self->state = STATE_VIDEO;
        [self.mPlayer play];
        [self.tc display:NO];
    }
    
}
@end
