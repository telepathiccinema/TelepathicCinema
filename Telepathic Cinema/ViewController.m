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
{
    UIImageView* calibrationView;
}
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
    [self initCalibrationViews];
    
    self->state = STATE_PLAYER;
    lastCalibrationState = [tracker getCalibrationState];
    
    self.tracker = [[TrackerWrapper alloc] init];
	[self.tracker initTracker:glView];
    
    resultsDisplayTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/30
														   target:self
														 selector:@selector(update:)
														 userInfo:nil
														  repeats:YES];
    [self.tracker startTrackingFromCam];
    [self setupVideoPlayer];
    
    //always start w/calibration
    tc = [[TelepathicCinema alloc] initWithView:glView
                                      withScene:@"7ptcalibration.smil"
                                     withPlayer:self.mPlayer
                                    withTracker:tracker
                                     withBounds: CGRectMake(self.view.bounds.origin.x, self.view.bounds.origin.y, self.view.bounds.size.width, self.view.bounds.size.height) ];
    [tc setCalibrationCompletedDelegate:self withSelector:@selector(completedCalibration:)];
    
    tc.overlay.zPosition = 0;
    [self.view.layer addSublayer:tc.overlay];
    
    [self.view addSubview:calibrationView];
    [self updateState];
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
    [self.tc update];
    
    if([self.tc.currentScene isCalibration] || [self.tracker getCalibrationState] != CALIBRATION_CALIBRATED)
    {
        [self.tracker updateCalibration];
        [self updateCalibration];
    }else
        calibrationView.hidden = YES;
    
    [self.tracker displayTrackingResults];
    [self.tc draw];
    
}

- (IBAction)onTouch:(id)sender {
    
    if(tc.currentScene.isCalibration)
        return;
    
    self->state = ++self->state % STATE_TOTAL;
    [self updateState];
}


-(void)updateState
{
    switch(state)
    {
        case STATE_PLAYER:
            [self.tracker blank];
            [self.tc display:NO];
            tc.renderDetails = NO;
            [self.tracker display:NO];
            [[self.view.layer.sublayers objectAtIndex:0] setOpacity:1.0];
            break;
        case STATE_DEBUG:
            [[self.view.layer.sublayers objectAtIndex:0] setOpacity:0.5];
            [self.tc display:YES];
            [self.tracker blank];
            [self.tracker display:NO];
            self.tc.renderDetails = NO;
            break;
        case STATE_TRACKER:
            [[self.view.layer.sublayers objectAtIndex:0] setOpacity:0.5];
            [self.tc display:YES];
            [self.tracker blank];
            [self.tracker display:YES];
            
            if([self.tc.currentScene isCalibration] == false)
                self.tc.renderDetails = YES;
            break;
    }
}

- (BOOL)prefersStatusBarHidden{
    return YES;
}

-(void)initCalibrationViews
{
    UIImage* calibration0 = [UIImage imageNamed:@"03FindFace.png"];
    
    calibrationView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    [calibrationView setImage:calibration0];
    calibrationView.alpha = .5;
    calibrationView.layer.zPosition = 1;
}

-(void) updateCalibration
{
    int currentCalibrationState = [self.tracker getCalibrationState];
    if(self->state != STATE_TRACKER &&  currentCalibrationState != CALIBRATION_CALIBRATED )
    {
        self->state = STATE_TRACKER;
        [self updateState];
    }
    
    if(lastCalibrationState != currentCalibrationState)
    {
        lastCalibrationState = currentCalibrationState;
        UIImage* calibrationImage;
        
        switch(lastCalibrationState)
        {
            case CALIBRATION_INSTRUCTIONS:
                calibrationImage = [UIImage imageNamed:@"03FindFace.png"];
                [calibrationView setImage:calibrationImage];
                break;
            case CALIBRATION_INITIAL_FACE_ERROR:
                calibrationImage = [UIImage imageNamed:@"04LostFace.png"];
                [calibrationView setImage:calibrationImage];
                break;
            case CALIBRATION_WATCHDOTS:
                calibrationImage = [UIImage imageNamed:@"05Success.png"];
                [calibrationView setImage:calibrationImage];
                break;
            case CALIBRATION_CALIBRATING:
                [calibrationView setImage:nil];
                break;
            case CALIBRATION_CALIBRATING_LOST:
                calibrationImage = [UIImage imageNamed:@"08CalibrationLostFace.png"];
                [calibrationView setImage:calibrationImage];
                break;
            case CALIBRATION_CALIBRATED:
                state = STATE_PLAYER;
                [self updateState];
                break;
            default:
                [calibrationView setImage:nil];
                break;
        }
    }
}

-(void) completedCalibration:(id)sender
{
    state = STATE_PLAYER;
    [self updateState];
}

@end
