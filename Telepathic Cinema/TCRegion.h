//
//  TCRegion.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>

@interface TCRegion : NSObject
{
    bool isHit;
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

-(bool)checkHitWith: (CGRect ) target
             atTime: (float) t;

-(bool)drawWithContext: (CGContextRef ) c
                  time: (float) t;

-(NSString *)getTargetFile;
-(NSString *)getTargetExtension;
-(bool)isActiveAtTime: (float) time;
@end
