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
@synthesize title;

-(id)initWithTarget: (NSString *) name
          withTitle: (NSString *) t
           withRect: (CGRect) r
      withStartTime: (float) start
        withEndTime: (float) end
        isCalibration:(BOOL)calibrationFlag{
    
    self.target = name;
    self.box = r;
    self.starttime = start;
    self.endtime = end;
    self.isCalibration = calibrationFlag;
    self.title = t;
    NSLog(@"Created Region: %@ => %f,%f,%f,%f => start: %f => end: %f", name, r.origin.x, r.origin.y, r.size.width, r.size.height, start, end);
    return self;
}

-(bool)checkHitWith: (CGRect) collider
         atTime:(float)t
{
    if( self.starttime != 0 && self.endtime != 0)
        if(t < self.starttime || t > self.endtime)
            return NO;
    
    NSLog(@"Checking hit between %0.02f, %0.02f, %0.02f, %0.02f and \n%0.02f, %0.02f, %0.02f, %0.02f", self.box.origin.x, self.box.origin.y, self.box.size.width, self.box.size.height, collider.origin.x, collider.origin.y, collider.size.width, collider.size.height);
    if(CGRectIntersectsRect(self.box, collider))
    {
        self.count++;
        isHit = YES;
    }
    else
        isHit = NO;
    
    return YES;
}

-(bool)isActiveAtTime:(float)time
{
    if(self.endtime == 0 || (time > self.starttime && time < self.endtime))
        return true;
    
    return false;
}

-(bool)drawWithContext: (CGContextRef ) context
                  time:(float) t
{
    if(self.endtime != 0)
        if(t < self.starttime || t > self.endtime)
            return NO;
    
    CGContextSaveGState(context);
    if(isHit)
        CGContextSetStrokeColor(context, CGColorGetComponents([UIColor greenColor].CGColor));
    else
        CGContextSetStrokeColor(context, CGColorGetComponents([UIColor yellowColor].CGColor));

    CGContextBeginPath(context);

    CGContextMoveToPoint(context, self.box.origin.x-self.box.size.width*.5, self.box.origin.y-self.box.size.height*.5);     //tl
    CGContextAddLineToPoint(context, self.box.origin.x+self.box.size.width*.5, self.box.origin.y-self.box.size.height*.5);  //tr
    CGContextAddLineToPoint(context, self.box.origin.x+self.box.size.width*.5, self.box.origin.y+self.box.size.height*.5);  //br
    CGContextAddLineToPoint(context, self.box.origin.x-self.box.size.width*.5, self.box.origin.y+self.box.size.height*.5);  //bl
    CGContextAddLineToPoint(context, self.box.origin.x-self.box.size.width*.5, self.box.origin.y-self.box.size.height*.5);  //tl
    CGContextStrokePath(context);

    CGContextSaveGState(context);
    NSString *text = [NSString stringWithFormat: @"%@ (%0.1f)", self.title, self.count*1.0/30.0];
    
 
    NSDictionary *attributesDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                    CFBridgingRelease(CTFontCreateWithName((CFStringRef) @"HelveticaNeue", 28.0, NULL)),
                                    (NSString *)kCTFontAttributeName,
                                    [UIColor whiteColor].CGColor,
                                    (NSString *)kCTForegroundColorAttributeName,
                                    nil];
    
    
    NSAttributedString *stringToDraw = [[NSAttributedString alloc] initWithString:text
                                                                       attributes:attributesDict];
    
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef)stringToDraw);
    CGMutablePathRef path = CGPathCreateMutable();
    CGAffineTransform transform = CGAffineTransformMakeScale(1, -1);
    transform = CGAffineTransformTranslate(transform, 0, - self.box.size.height);

    CGRect frameText = self.box;
    CGRect newRectForUIKit = CGRectApplyAffineTransform(frameText, transform);
    CGPathAddRect(path, NULL, newRectForUIKit);
    
    CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), path, NULL);

    CGContextSetTextMatrix(context, CGAffineTransformIdentity);
    CGContextTranslateCTM(context, 0, ([self box]).size.height );
    CGContextScaleCTM(context, 1.0, -1.0);
 
    CTFrameDraw(frame, context);
    CFRelease(path);
    CGContextRestoreGState(context);
    return YES;
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
