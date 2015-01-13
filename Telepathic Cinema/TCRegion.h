//
//  TCRegion.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>
#import "TCGaze.h"

@interface TCRegion : NSObject
{
    bool isHit;
    NSString *saveTargetString;
}

@property NSString* target;
@property NSString* title;
@property CGRect box;
@property float starttime;
@property float endtime;
@property int count;
@property bool isCalibration;

-(id)initWithTarget: (NSString *) name
          withTitle: (NSString *)title
         withRect: (CGRect ) rect
    withStartTime: (float) start
      withEndTime: (float) end
      isCalibration: (BOOL) calibration;

-(void)setSaveTarget:(NSString *) targetString;

-(id)initWithTarget: (NSString *)name
          withTitle: (NSString *)title
          withValue: (float) value;

-(bool)checkHitWith: (CGRect ) target
             atTime: (float) t;

-(bool)drawWithContext: (CGContextRef ) c
                  time: (float) t;

-(void)archiveDataWithTCGaze: (TCGaze *)gaze;

-(NSString *)getTargetFile;
-(NSString *)getTargetExtension;
-(bool)isActiveAtTime: (float) time;
-(bool) isSetForArchival;
@end
