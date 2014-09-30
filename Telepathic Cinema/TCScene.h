//
//  TCScene.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.

#import <Foundation/Foundation.h>
#import "TCRegion.h"
#import "TCGaze.h"
#import "DDXML.h"

@interface TCScene : NSObject
{
    BOOL isCalibration;
    NSString* sceneID;
    NSString* defaultTarget;
    TCRegion* calibrationRegion;
    TCGaze* gaze;
}
@property(strong, retain) NSString* name;
@property(strong, retain) NSMutableArray* regions;
@property(strong, retain) NSString* videoFile;

-(id)initWithName: (NSString *) filename
       withGaze: (TCGaze *) gaze;
-(void) drawWithContext: (CGContextRef) context withTime:(float) t;
-(float)coordToFloat: (NSString *) c;
-(NSString *)getTargetFile;
-(NSString *)getTargetExtension;
-(void) updateWithTime:(float) t;
-(NSString*) getNextScene;
-(void) makeActive;//:(TrackerWrapper*) tracker;
-(void)deactivateWithTracker:(TrackerWrapper *) tracker;

@end
