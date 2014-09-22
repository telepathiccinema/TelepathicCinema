//
//  ViewController.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/20/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//


#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreText/CoreText.h>

#define STATE_VIDEO 0
#define STATE_DEBUG 1

@class CustomGLView;
@class TrackerWrapper;
@class AVQueueplayer;
@class TelepathicCinema;

@interface ViewController : UIViewController
{
    /**
     * Timer for periodically refreshing of the results display.
     *
     */
	NSTimer *resultsDisplayTimer;

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
    int state;
}
@property (nonatomic, retain) NSTimer *resultsDisplayTimer;
@property (strong, nonatomic) IBOutlet CustomGLView *glView;
@property (nonatomic, retain) TrackerWrapper *tracker;
@property (nonatomic, retain) AVQueuePlayer* mPlayer;
@property(nonatomic, retain) TelepathicCinema* tc;
@property(nonatomic, retain) AVPlayerLayer* movieLayer;

- (IBAction)onTouch:(id)sender;

@end
