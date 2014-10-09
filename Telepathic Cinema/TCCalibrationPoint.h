//
//  TCCalibrationPoint.h
//  Telepathic Cinema
//
//  Created by ketai on 9/30/14.
//  Copyright (c) 2014 Ket.ai. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TCCalibrationPoint : NSObject
{
    NSMutableArray* values;
}
@property float x;
@property float y;


-(id)initWithPointX: (float) x
                  Y:(float) y
        gazeVectorX:(float) vx
                  Y:(float) vy
                  Z:(float ) vz;

-(void) addVectorX:(float) vx
                 Y:(float) vy
                 Z:(float) vz;

-(float) getConfidenceRatingForVectorX: (float) vx
                                     Y: (float) vy
                                     Z: (float) vz;
-(NSString*) getInfoString;

@end
