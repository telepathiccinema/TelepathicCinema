//
//  TCCalibrationPoint.m
//  Telepathic Cinema
//
//  Created by user on 9/30/14.
//  Copyright (c) 2014 Ket.ai. All rights reserved.
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
    
    minVX = vx;
    minVY = vy;
    minVZ = vz;
    maxVX = vx;
    maxVY = vy;
    maxVZ = vz;
    avgVX = vx;
    avgVY = vy;
    avgVZ = vz;
    
    sampleCount=1;
    
    return self;
}

-(void) addVectorX:(float)vx Y:(float)vy Z:(float)vz
{
    avgVX  = (avgVX + vx) / 2;
    avgVY  = (avgVY + vy) / 2;
    avgVZ  = (avgVZ + vz) / 2;

    if(vx < minVX)
        minVX = vx;
    if(vx > maxVX)
        maxVX = vx;
    
    if(vy < minVY)
        minVY = vy;
    if(vy > maxVY)
        maxVY = vy;
    
    if(vz < minVZ)
        minVZ = vz;
    if(vz > maxVZ)
        maxVZ = vz;
    
    sampleCount++;
}

-(float) getConfidenceRatingForVectorX: (float) vx
                                     Y: (float) vy
                                     Z: (float) vz
{
    float c = 0;
    if(vx >= minVX && vx <= maxVX &&
       vy >= minVY && vy <= maxVY &&
       vz >= minVZ && vz <= maxVZ)
    {
        c = fabsf(avgVX - vx)/fabsf(maxVX-minVX) + fabsf(avgVY - vy)/fabsf(maxVY - minVY) + fabsf(avgVZ - vz)/fabsf(maxVZ - minVZ);
    }
    
    NSLog(@"confidence for point %0.02f, %0.02f => %0.02f", self.x, self.y, c);
    return c;
}

-(NSString*) getInfoString
{
    return [NSString stringWithFormat:@"x:%0.02f,y:%0.02f,avgX:%0.02f, avgY:%0.02f, avgZ:%0.02f, count: %i",self.x,self.y,avgVX, avgVY, avgVZ, sampleCount];
}

@end
