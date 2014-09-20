//
//  TCRegion.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TCRegion : NSObject

@property NSString* target;
@property CGRect box;
@property float starttime;
@property float endtime;
@property int count;
@property bool isCalibration;

-(id)initWithTarget: (NSString *) name
         withRect: (CGRect ) rect
    withStartTime: (float) start
      withEndTime: (float) end
      isCalibration: (BOOL) calibration;

-(void)checkHitWith: (CGRect ) target
             atTime: (float) t;

-(void)drawWithContext: (CGContextRef ) c
                  time: (float) t;

-(NSString *)getTargetFile;
-(NSString *)getTargetExtension;

@end
