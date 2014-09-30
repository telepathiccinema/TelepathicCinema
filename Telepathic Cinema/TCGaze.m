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
{
    tracker = t;
    isCalibrating = false;
    calibrationpoints = [[NSMutableArray alloc]init];
    
    return self;
}

-(void)resetTimer
{
    self.inactiveTime = 0;
}

-(void) drawWithContext: (CGContextRef) context
{
    if(self.active)
        CGContextSetStrokeColorWithColor(context, [[UIColor greenColor] CGColor]);
    else
        CGContextSetStrokeColorWithColor(context, [[UIColor redColor] CGColor]);
    
    CGContextAddArc(context,self.boundingBox.origin.x+self.boundingBox.size.width/2, self.boundingBox.origin.y+self.boundingBox.size.height/2,self.boundingBox.size.width/2,0,M_PI*2,YES);
    CGContextStrokePath(context);
    
}

-(void) calibrationPointX:(float) x
                           Y:(float) y
{
    if(!isCalibrating)
        return;
    
    float* d = [tracker getGlobalGaze];
    
    NSLog(@"DATA,%0.02f,%0.02f,%0.02f", d[0], d[1], d[2]);

    NSString* key = [NSString stringWithFormat:@"%0.02f,%0.02f",x,y];

    //check for point in dictionary
    TCCalibrationPoint* pt = [self getCalibrationPointX:x Y:y];
    
    if( pt != nil)
    {
        [pt addVectorX:d[0] Y:d[1] Z:d[2]];
        NSLog(@"updating data for point: %@", key);
    }else
    {
        NSLog(@"adding new calibration point: %@", key);
        pt = [[TCCalibrationPoint alloc] initWithPointX:x Y:y gazeVectorX:d[0] Y:d[1] Z:d[2]];
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

    //[tracker initGazeCalibration];  //Visage beta gaze tracker call

}

-(void)finalizeCalibration
{
    isCalibrating = false;
    isCalibrated = true;
    NSLog(@"finalizing calibration, number of regions: %i", [calibrationpoints count]);
    //[tracker endGazeCalibration];
    
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
        return;
    }
    if(isCalibrating)
        return;
    
    NSLog(@"updating gaze");
    //update cursor
    float* d = [tracker getGlobalGaze];
    float newx, newy;
    
    TCCalibrationPoint * winner = nil;
    float confidence = 0;
    
    //calculate latest gaze position
    for (TCCalibrationPoint* item in calibrationpoints) {
        float c = [item getConfidenceRatingForVectorX:d[0] Y:d[1] Z:d[2]];
            if(c > confidence)
            {
                confidence = c;
                winner = item;
            }
    }
    
    if(confidence > 0)
    {
        newx = winner.x;
        newy = winner.y;
        NSLog(@"current position is %f, %f , confidence: %f", newx, newy, confidence);
        self.boundingBox = CGRectMake(newx, newy, 1024/2, 768/2);
        
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
