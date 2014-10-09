//
//  TCCalibrationPoint.m
//  Telepathic Cinema
//
//  Created by ketai on 9/30/14.
//  Copyright (c) 2014 Telepathic Cinema All rights reserved.
//

#import "TCCalibrationPoint.h"

@implementation TCCalibrationPoint

-(id)initWithPointX:(float) px
                  Y:(float) py
        gazeVectorX:(float) vx
                  Y:(float) vy
                  Z:(float) vz;
{
    self.x = px;
    self.y = py;
    values = [[NSMutableArray alloc]init];
    
    return self;
}

//Adding a vector position to our running data
//  currently only 2D
-(void) addVectorX:(float)vx Y:(float)vy Z:(float)vz
{
    struct CGVector *v = malloc(sizeof(struct CGVector));
    v->dx = vx;
    v->dy = vy;
    
    NSValue *value = [NSValue valueWithBytes:v objCType:@encode(CGVector)];

    [values addObject:value];
}


//calculate the minimum distance to any point we have in our training set

-(float) getConfidenceRatingForVectorX: (float) vx
                                     Y: (float) vy
                                     Z: (float) vz
{
    float minDistance = -1;
    
    //calculate the closest point to the current values
    for (NSValue* item in values)
    {
        struct CGVector v;
        [item getValue:&v];

        
        float d = hypotf(v.dx - vx, v.dy - vy);
        if(minDistance < 0)
            minDistance = d;
        
        if(d < minDistance)
            minDistance = d;
    }
    
    
    return minDistance;
}

-(NSString*) getInfoString
{
    return [NSString stringWithFormat:@"Dataset for %0.2f, %0.2f with %i points", self.x, self.y, [values count]];
}

@end
