//
//  TCGaze.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TCGaze.h"

@implementation TCGaze

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

-(void) updateWithTracker: (TrackerWrapper *) tracker
{
    if([tracker getTrackingStatus] == 1)
        self.active = YES;
    else
    {
        self.active = NO;
        self.inactiveTime++;
        return;
    }
    
    //update cursor
    float* d = [tracker getGlobalGaze];
    float* f = [tracker getFaceTranslation];
    float newx, newy;
    
    NSLog(@"DATA,%0.02f,%0.02f,%0.02f,%0.02f,%0.02f,%0.02f", d[0], d[1], d[2], f[0], f[1], f[2]);
//    
//
//    
//    
//    NSLog(@"Smallest error: %f", smallest);
//    if(smallest == e1)
//    {
//        newx =  0;
//        newy = 0;
//    }
//    else if(smallest == e2)
//    {
//        newx =  1024/2;
//        newy = 0;
//        
//    }
//    else if(smallest == e3)
//    {
//        newx =  0;
//        newy = 768/2;
//        
//    }else if(smallest == e4)
//    {
//        newx =  1024/2;
//        newy = 768/2;
//        
//    }
//    else
//    {
//        newx = - 1024;
//        newy = - 1024;
//    }
/*
 //clamp values to screen
    if(newx < 0)
        newx = 0;
    if(newx > 1024-100)
        newx = 1024-100;
    if(newy < 0)
        newy = 0;
    if(newy > 768-100)
        newy = 768-100;
*/
    self.boundingBox = CGRectMake(newx, newy, 1024/2, 768/2);
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
