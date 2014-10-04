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
    self->state = STATE_PLAYER;
    
    self.tracker = [[TrackerWrapper alloc] init];
	[self.tracker initTracker:glView];
    
    resultsDisplayTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/30
														   target:self
														 selector:@selector(update:)
														 userInfo:nil
														  repeats:YES];
    [self.tracker startTrackingFromCam];
    [self setupVideoPlayer];
    tc = [[TelepathicCinema alloc] initWithView:glView withScene:@"alpha.smil" withPlayer:self.mPlayer withTracker:tracker withBounds: CGRectMake(self.view.bounds.origin.x, self.view.bounds.origin.y, self.view.bounds.size.width, self.view.bounds.size.height) ];
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
    [self.tc update: self.mPlayer withTracker:self.tracker];
    [self.tracker displayTrackingResults];
    [self.tc draw];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)onTouch:(id)sender {
    
    
    self->state = ++self->state % STATE_TOTAL;
    
    switch(state)
    {
        case STATE_PLAYER:
            [self.tracker blank];
            [self.tc display:NO];
            [self.tracker display:NO];
            [[self.view.layer.sublayers objectAtIndex:0] setOpacity:1.0];
            break;
        case STATE_DEBUG:
            [[self.view.layer.sublayers objectAtIndex:0] setOpacity:0.5];
            [self.tc display:YES];
            [self.tracker blank];
            [self.tracker display:NO];
            break;
        case STATE_TRACKER:
            [[self.view.layer.sublayers objectAtIndex:0] setOpacity:0.5];
            [self.tc display:YES];
            [self.tracker blank];
            [self.tracker display:YES];
            break;
    }
}
@end
