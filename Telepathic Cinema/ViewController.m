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

@interface ViewController ()

@end

@implementation ViewController
@synthesize tracker;
@synthesize glView;
@synthesize resultsDisplayTimer;


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    self->state = STATE_VIDEO;
    
    self.tracker = [[TrackerWrapper alloc] init];
	[self.tracker initTracker:glView];
    
    resultsDisplayTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/60
														   target:self
														 selector:@selector(showResults:)
														 userInfo:nil
														  repeats:YES];
    [self.tracker startTrackingFromCam];
    [self setupMovie];
    
}

-(void) setupMovie
{
    NSBundle *bundle = [NSBundle mainBundle];
    NSURL *moviePath1 = [bundle URLForResource:@"sample" withExtension:@"mov"];
    NSURL *moviePath2 = [bundle URLForResource:@"tree" withExtension:@"mov"];
    
    AVPlayerItem *video1 = [AVPlayerItem playerItemWithURL: moviePath1];
    AVPlayerItem *video2 = [AVPlayerItem playerItemWithURL: moviePath2];
    
    self.mPlayer = [AVQueuePlayer queuePlayerWithItems:[NSArray arrayWithObjects:video2, video1, nil]];
    
    self.mPlayer.actionAtItemEnd = AVPlayerActionAtItemEndNone;
    AVPlayerLayer *layer = [AVPlayerLayer playerLayerWithPlayer:self.mPlayer];
    layer.frame = self.view.bounds;
    
    [self.view.layer addSublayer: layer];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(videoEnded:)
                                                 name:AVPlayerItemDidPlayToEndTimeNotification
                                               object:[self.mPlayer currentItem]];
    [self.mPlayer play];
}


- (void)videoEnded:(NSNotification *)notification
{
    if([[self.mPlayer items] count] <= 1)
    {
        [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:[self.mPlayer currentItem]];
        
        [self.mPlayer advanceToNextItem];
        NSBundle *bundle = [NSBundle mainBundle];
        NSURL *moviePath1 = [bundle URLForResource:@"sample" withExtension:@"mov"];
        NSURL *moviePath2 = [bundle URLForResource:@"tree" withExtension:@"mov"];
        
        AVPlayerItem *video1 = [AVPlayerItem playerItemWithURL: moviePath1];
        AVPlayerItem *video2 = [AVPlayerItem playerItemWithURL: moviePath2];
        if([self.mPlayer canInsertItem:video1 afterItem:nil])
            [self.mPlayer insertItem:video1 afterItem:nil];
        
        if([self.mPlayer canInsertItem:video2 afterItem:nil])
            [self.mPlayer insertItem:video2 afterItem:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(videoEnded:)
                                                     name:AVPlayerItemDidPlayToEndTimeNotification
                                                   object:[self.mPlayer currentItem]];
        [self.mPlayer play];
        
    }else
    {
        
        [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:[self.mPlayer currentItem]];
        [self.mPlayer advanceToNextItem];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(videoEnded:)
                                                     name:AVPlayerItemDidPlayToEndTimeNotification
                                                   object:[self.mPlayer currentItem]];
        [self.mPlayer play];
        
    }
}

-(void)showResults:(NSTimer *)timer {
    
    
    [self.tracker displayTrackingResults];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    
}

- (IBAction)onTouch:(id)sender {
    if(self->state == STATE_VIDEO)
    {
        [[self.view.layer.sublayers objectAtIndex:0] removeFromSuperlayer];
        
        self.tracker.glView = glView;
        self->state = STATE_DEBUG;
        
    }else{
        self.tracker.glView = nil;
        
        AVPlayerLayer *layer = [AVPlayerLayer playerLayerWithPlayer:self.mPlayer];
        layer.frame = self.view.bounds;
        
        [self.view.layer addSublayer: layer];
        self->state = STATE_VIDEO;
        [self.mPlayer play];
    }
    
    
}
@end
