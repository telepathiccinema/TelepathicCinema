//
//  TelepathicCinema.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/26/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "TrackerWrapper.h"
#import "CustomGLView.h"
#import "TCScene.h"
#import "TCGaze.h"
#import "DDXML.h"



@interface TelepathicCinema : NSObject
{
    CALayer *overlay;
    UIImage *image;
    TCScene *currentScene, *queueScene, *calibrationScene, *defaultScene;
    AVQueuePlayer *player;
    float gazeLimit;
    float calibrationLimit;
    bool isDone;
    TrackerWrapper* tracker;
    CGRect bounds;
    CFTimeInterval lastUpdateTime;
    id calibrationCompetedDelegate;
    SEL calibrationCompletedSelector;
}

@property (nonatomic, retain) CALayer* overlay;
@property (nonatomic, retain) TCScene* currentScene;
@property (nonatomic, retain) TCScene* queuedScene;
@property (nonatomic, retain) TCGaze* cursor;
@property (nonatomic) float gazeLimit;
@property (nonatomic) float calibrationLimit;
@property (nonatomic) bool renderDetails;

-(id)initWithView:(CustomGLView *)view
        withScene:(NSString * ) filename
       withPlayer: (AVQueuePlayer *) player
      withTracker: (TrackerWrapper*) tracker
       withBounds: (CGRect) bounds;

-(void)queueScene;

-(void)draw;

-(void)display: (BOOL) show;
-(void)update;
- (void)setCalibrationCompletedDelegate:(id)delegate withSelector:(SEL)selector;

@end
