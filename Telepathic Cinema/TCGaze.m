//
//  TCGaze.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TCGaze.h"
#import "TCCalibrationPoint.h"

@implementation TCGaze

-(id) initWithTracker:(TrackerWrapper *)t
           withBounds:(CGRect)bounds_
{
    if ((self = [super init]))
    {
        bounds = bounds_;
        tracker = t;
        isCalibrating = false;
        calibrationpoints = [[NSMutableArray alloc]init];
    }
    return self;
}

-(void)resetTimer
{
    self.inactiveTime = 0;
}

-(void) drawWithContext: (CGContextRef) context
{
    if(isCalibrating)
        return;
    
    if(self.active)
        CGContextSetStrokeColorWithColor(context, [[UIColor greenColor] CGColor]);
    else
        CGContextSetStrokeColorWithColor(context, [[UIColor redColor] CGColor]);
    
    CGContextAddArc(context,
                    self.boundingBox.origin.x + self.boundingBox.size.width*.5,
                    self.boundingBox.origin.y + self.boundingBox.size.height*.5,
                    self.boundingBox.size.width/2 , 0 , M_PI*2, YES);
    CGContextStrokePath(context);
    
}

-(void) calibrationPointX:(float) x
                        Y:(float) y
{
    
    NSLog(@"Calling calibration point with %.2f, %.2f",x,y);
    if(!isCalibrating)
        return;
    
    float* d = [tracker getGlobalGaze];
    //float* d = [tracker getGaze];
    
    //    NSLog(@"DATA,%0.02f,%0.02f,%0.02f", d[0], d[1], d[2]);
    
    //check for point in dictionary
    TCCalibrationPoint* pt = [self getCalibrationPointX:x Y:y];
    
    if( pt != nil)
    {
        [pt addVectorX:d[0] Y:d[1] Z:d[2]];
    }else
    {
        pt = [[TCCalibrationPoint alloc] initWithPointX:x Y:y gazeVectorX:d[0] Y:d[1] Z:d[2]];
        
        //add normalized x/y calibration point to the visage model only ONCE
        [tracker addGazeCalibrationPoint:x/bounds.size.width andY:y/bounds.size.height];
        NSLog(@"Adding this: %@", [pt getInfoString]);
        [calibrationpoints addObject:pt];
    }
    
}

-(TCCalibrationPoint*) getCalibrationPointX:(float)x
                                          Y:(float)y
{
    for(TCCalibrationPoint* item in calibrationpoints)
    {
        if(item.x ==x && item.y == y)
            return item;
    }
    return nil;
}

-(void) initCalibration
{
    [calibrationpoints removeAllObjects];
    isCalibrating = true;
    NSLog(@"preparing calibration");
    
    [tracker initGazeCalibration];  //Visage beta gaze tracker call
    
}

-(void)finalizeCalibration
{
    isCalibrating = false;
    isCalibrated = true;
    NSLog(@"finalizing calibration, number of regions: %i", [calibrationpoints count]);
    [tracker endGazeCalibration];  //Visage beta gaze tracker call
    
    for(id item in calibrationpoints)
    {
        NSLog(@"Calibration Point values: %@",[item getInfoString]);
    }
    
}

-(void) update
{
    
    if([tracker getTrackingStatus] == 1)
        self.active = YES;
    else
    {
        self.active = NO;
        self.inactiveTime++;
    }
    if(isCalibrating)
        return;
    
    float size = 20;
    float dx,dy,ds;
    
    if(self.active)
    {
        float higestConfidence = 0;
        float targetx = 0;
        float targety = 0;
        
        //update cursor
        CGPoint p = [tracker getScreenSpaceGazeLocation];
        
        //float* d = [tracker getGaze];
        float* d = [tracker getGlobalGaze];
        
        if([calibrationpoints count] > 0 )
        {
            targetx = p.x*bounds.size.width;
            targety = p.y*bounds.size.height;
            higestConfidence = .5;
        }else{
            //let's bias a bit left....for no calibration
            //
            if(d[1] < 0.13)
            {
                targetx = bounds.size.width*.25;
                targety = bounds.size.height * .5;
            }else{
                targetx = bounds.size.width*.75;
                targety = bounds.size.height * .5;
            }
            higestConfidence = .50;
        }
        
        if(higestConfidence > 0)
        {
            size = bounds.size.height/4;
            dx = targetx - (self.boundingBox.origin.x + size * .5);
            dy = targety - (self.boundingBox.origin.y + size * .5);
            ds = size - self.boundingBox.size.width;
            
            self.boundingBox = CGRectMake(self.boundingBox.origin.x + dx *.1,
                                          self.boundingBox.origin.y + dy *.1,
                                          size + ds*.1, size + ds*.1);
        }
    }
    else
    {
        size = 30;
        dx = bounds.size.width*.5 - (self.boundingBox.origin.x + size*.5);
        dy = bounds.size.height*.5 - (self.boundingBox.origin.y + size*.5);
        ds = size - self.boundingBox.size.width;
        
        self.boundingBox = CGRectMake(self.boundingBox.origin.x + dx *.1,
                                      self.boundingBox.origin.y + dy *.1,
                                      size + ds*.1, size + ds*.1);
        
    }
}


-(float) mapValue: (float) value
       startRange: (float) istart
        stopRange: (float) istop
 startTargetRange: (float) ostart
   endTargetRange: (float) ostop
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}
@end
