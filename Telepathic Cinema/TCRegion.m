//
//  TCRegion.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TCRegion.h"

@implementation TCRegion
@synthesize count;
@synthesize target;

-(id)initWithTarget: (NSString *) name
           withRect: (CGRect) r
      withStartTime: (float) start
        withEndTime: (float) end{
    
    self.target = name;
    self.box = r;
    self.starttime = start;
    self.endtime = end;
    return self;
}

-(void)checkHitWith: (CGRect) collider
         atTime:(float)t
{
    if(t < self.starttime || t > self.endtime)
        return;
    
    if(CGRectIntersectsRect(self.box, collider))
    {
        self.count++;
    }
}

-(void)drawWithContext: (CGContextRef ) context
                  time:(float) t
{
    if(t < self.starttime || t > self.endtime)
        return;
    
    CGContextMoveToPoint(context, self.box.origin.x, self.box.origin.y);    //tl
    CGContextAddLineToPoint(context, self.box.origin.x+self.box.size.width, self.box.origin.y);   //tr
    CGContextAddLineToPoint(context, self.box.origin.x+self.box.size.width, self.box.origin.y+self.box.size.height);//br
    CGContextAddLineToPoint(context, self.box.origin.x, self.box.origin.y + self.box.size.height); //bl
    CGContextAddLineToPoint(context, self.box.origin.x, self.box.origin.y);    //tl
    
    CGContextStrokePath(context);
    
}


@end
