//
//  TCGaze.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TrackerWrapper.h"

@interface TCGaze : NSObject
{
    TrackerWrapper* tracker;
    NSMutableArray* calibrationpoints;
    bool isCalibrating;
    bool isCalibrated;
}
@property CGRect boundingBox;
@property float confidence;
@property BOOL active;
@property int inactiveTime;

-(id)   initWithTracker: (TrackerWrapper*) t;
-(void) drawWithContext: (CGContextRef) context;
-(void) update;
-(void) resetTimer;
-(void) initCalibration;
-(void) calibrationPointX:(float) x
                        Y:(float) y;
-(void) finalizeCalibration;

@end
