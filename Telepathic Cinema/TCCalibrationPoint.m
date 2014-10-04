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
    //simply keep a running average
    avgVX  = (avgVX + vx) / 2;
    avgVY  = (avgVY + vy) / 2;
    avgVZ  = (avgVZ + vz) / 2;

    //maintain boundary information
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
    if(vx >= minVX && vx <= maxVX)
        c += 1 - fabsf(avgVX - vx)/fabsf(maxVX-minVX);

    if(vy >= minVY && vy <= maxVY)
        c += 1 - fabsf(avgVY - vy)/fabs(maxVX - minVY);

    if(vz >= minVZ && vz <= maxVZ)
        c += 1 - fabsf(avgVZ - vz)/fabsf(maxVZ - minVZ);
    
    NSLog(@"confidence for point %0.02f, %0.02f => %0.02f", self.x, self.y, c);

    return c;
}

-(NSString*) getInfoString
{
    return [NSString stringWithFormat:@"x:%0.02f,y:%0.02f,avgX:%0.02f, avgY:%0.02f, avgZ:%0.02f, count: %i",self.x,self.y,avgVX, avgVY, avgVZ, sampleCount];
}

@end
