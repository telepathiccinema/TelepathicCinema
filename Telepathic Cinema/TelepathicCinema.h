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
}

@property (nonatomic, retain) CALayer* overlay;
@property (nonatomic, retain) TCScene* currentScene;
@property (nonatomic, retain) TCScene* queuedScene;
@property (nonatomic, retain) TCScene* calibrationScene;
@property (nonatomic, retain) TCScene* defaultScene;
@property (nonatomic, retain) TCGaze* cursor;
@property (nonatomic) float gazeLimit;
@property (nonatomic) float calibrationLimit;

-(id)initWithView:(CustomGLView *)view
         andScene:(NSString * ) filename
        andPlayer: (AVQueuePlayer *) player;

-(void)queueScene;

-(void)draw;

-(void)display: (BOOL) show;
-(void)update: (AVQueuePlayer *) player withTracker:(TrackerWrapper *)tracker;


@end
