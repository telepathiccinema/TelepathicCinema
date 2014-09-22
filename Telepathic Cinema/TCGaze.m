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
    float* d = [tracker getGaze];
    float newx, newy;
    
    NSLog(@"Tracking data %f, %f", d[0], d[1]);
    if(d[0] < 0)
        newx = self.boundingBox.origin.x - 10;
    else if(d[0] > 0)
        newx = self.boundingBox.origin.x + 10;
    
    
    if(d[1] > 0)
        newy = self.boundingBox.origin.y + 10;
    else if(d[1] < 0)
        newy = self.boundingBox.origin.y - 10;

    //clamp values to screen
    if(newx < 0)
        newx = 0;
    if(newx > 1024)
        newx = 1024;
    if(newy < 0)
        newy = 0;
    if(newy > 768)
        newy = 768;
    self.boundingBox = CGRectMake(newx, newy, 100, 100);
}

@end
