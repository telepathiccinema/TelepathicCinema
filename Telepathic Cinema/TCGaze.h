//
//  TCGaze.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TrackerWrapper.h"
#import "TCGazeDB.h"

@interface TCGaze : NSObject
{
    TrackerWrapper* tracker;
    NSMutableArray* calibrationpoints;
    bool isCalibrating;
    bool isCalibrated;
    CGRect bounds;
}
@property CGRect boundingBox;
@property float confidence;
@property BOOL active;
@property int inactiveTime;
@property TCGazeDB* gazeHistory;

-(id)   initWithTracker: (TrackerWrapper*) t
             withBounds: (CGRect)bounds;
-(void) drawWithContext: (CGContextRef) context;
-(void) update;
-(void) resetTimer;
-(void) initCalibration;
-(void) calibrationPointX:(float) x
                        Y:(float) y;
-(void) finalizeCalibration;

@end
