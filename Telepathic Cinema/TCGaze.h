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
@property CGRect boundingBox;
@property float confidence;
@property BOOL active;

-(void) drawWithContext: (CGContextRef) context;
-(void) updateWithTracker: (TrackerWrapper *) tracker;

@end
