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
@synthesize isCalibration;

-(id)initWithTarget: (NSString *) name
           withRect: (CGRect) r
      withStartTime: (float) start
        withEndTime: (float) end
        isCalibration:(BOOL)calibrationFlag{
    
    self.target = name;
    self.box = r;
    self.starttime = start;
    self.endtime = end;
    self.isCalibration = calibrationFlag;
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
    if( self.starttime != 0 && self.endtime != 0)
        if(t < self.starttime || t > self.endtime)
            return;
    
    CGContextMoveToPoint(context, self.box.origin.x, self.box.origin.y);    //tl
    CGContextAddLineToPoint(context, self.box.origin.x+self.box.size.width, self.box.origin.y);   //tr
    CGContextAddLineToPoint(context, self.box.origin.x+self.box.size.width, self.box.origin.y+self.box.size.height);//br
    CGContextAddLineToPoint(context, self.box.origin.x, self.box.origin.y + self.box.size.height); //bl
    CGContextAddLineToPoint(context, self.box.origin.x, self.box.origin.y);    //tl
    CGContextStrokePath(context);
    
    // Create text attributes
    NSDictionary *textAttributes = @{NSFontAttributeName: [UIFont systemFontOfSize:18.0]};
    
    // Create string drawing context
    NSStringDrawingContext *drawingContext = [[NSStringDrawingContext alloc] init];
    drawingContext.minimumScaleFactor = 0.5; // Half the font size
    
    CGRect drawRect = CGRectMake(0.0, 0.0, 200.0, 100.0);
    [self.target drawWithRect:drawRect
                 options:NSStringDrawingUsesLineFragmentOrigin
              attributes:textAttributes
                 context:drawingContext];
}

-(NSString *)getTargetFile
{
    return [self.target substringToIndex: [self.target rangeOfString:[@"." stringByAppendingString:[self getTargetExtension]]].location];
}

-(NSString *)getTargetExtension
{
    return [self.target pathExtension];
}



@end
