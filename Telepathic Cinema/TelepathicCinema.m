//
//  TelepathicCinema.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/26/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TelepathicCinema.h"

@implementation TelepathicCinema
@synthesize overlay;
@synthesize currentScene;
@synthesize queuedScene;

-(id)initWithView:(CustomGLView *)view
        withScene:(NSString * )filename
       withPlayer: (AVQueuePlayer *) avplayer
      withTracker:(TrackerWrapper *)_tracker
       withBounds:(CGRect)bounds_
{
    bounds = bounds_;
    overlay = [CALayer layer];
    overlay.frame = view.frame;
    overlay.opacity = .25;
    tracker = _tracker;
    UIGraphicsBeginImageContextWithOptions(CGSizeMake(view.frame.size.width, view.frame.size.height), NO, 0.0);
    image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    overlay.contents = (__bridge id)(image.CGImage);
    overlay.hidden = false;
    self.cursor = [[TCGaze alloc] initWithTracker:_tracker];
    self.cursor.boundingBox = CGRectMake(overlay.frame.size.width/2, overlay.frame.size.height/2, 100,100);
    self.cursor.confidence = 0;
    currentScene = [[TCScene alloc] initWithName:filename withGaze:self.cursor];
    self->player = avplayer;
    isDone = NO;
    
    
    NSBundle *bundle = [NSBundle mainBundle];
    NSURL *moviePath1 = [bundle URLForResource: [currentScene getTargetFile] withExtension:[currentScene getTargetExtension]];
    if(moviePath1)
    {
        
        AVPlayerItem *video1 = [AVPlayerItem playerItemWithURL: moviePath1];
        [self->player insertItem:video1 afterItem:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(sceneEnded:)
                                                     name:AVPlayerItemDidPlayToEndTimeNotification
                                                   object:video1 ];
        [self->player play];
    }else{
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could not find file."
                                                        message:[@"Failed to load resource: " stringByAppendingString: currentScene.videoFile]
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        isDone = YES;
        [alert show];
    }
    [currentScene makeActive];
    
    return self;
}


-(void)queueScene{
    
    NSString* winner = [currentScene getNextScene];
    
    if(!winner)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"End."
                                                        message:@"End."
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        isDone = YES;
        [alert show];
        return;
    }

    queuedScene= [[TCScene alloc] initWithName: winner withGaze:self.cursor];
    
    NSBundle *bundle = [NSBundle mainBundle];
    NSURL *moviePath1 = [bundle URLForResource: [queuedScene getTargetFile] withExtension:[queuedScene getTargetExtension]];
    if(moviePath1)
    {
        
        AVPlayerItem *video1 = [AVPlayerItem playerItemWithURL: moviePath1];
        [self->player insertItem:video1 afterItem:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(sceneEnded:)
                                                     name:AVPlayerItemDidPlayToEndTimeNotification
                                                   object:video1 ];
        [currentScene setDuration:CMTimeGetSeconds(self->player.currentItem.duration)];
    }else{
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could not find file."
                                                        message:[NSString stringWithFormat: @"Failed to find resource:%@.%@ \nwhen loading scene: %@", [queuedScene getTargetFile], [queuedScene getTargetExtension], queuedScene.name ]
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        isDone = YES;
        [alert show];
    }
}



-(void)display: (BOOL) show
{
    overlay.hidden = !show;
}

-(void)update: (AVQueuePlayer *) player withTracker:(TrackerWrapper *)tracker
{
    if(isDone)
        return;
    
    float currentTime = CMTimeGetSeconds([[self->player currentItem] currentTime]);
    float duration = CMTimeGetSeconds([[self->player currentItem] duration]);
    
    //queue up next scene
    if(duration - currentTime <= 1.0 && duration-currentTime > 0 && [[self->player items] count] <= 1)
    {
        [self queueScene];
    }
    
    [self.cursor update];
    
    //check collisions on regions
    if(self.cursor.active == YES)
        [currentScene updateWithTime:CMTimeGetSeconds([self->player currentTime])];
}


-(void)draw{
    if(overlay.hidden)
        return;
    
    UIGraphicsBeginImageContext(overlay.bounds.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetStrokeColorWithColor(context, [[UIColor yellowColor] CGColor]);
    CGContextFillRect(context, CGRectMake(0.0f, 0.0f, image.size.width, image.size.height));
    CGContextSetLineWidth(context, 4.0f);
    float currentSceneTime = CMTimeGetSeconds([[self->player currentItem] currentTime]);
    
    [currentScene drawWithContext: context withTime: currentSceneTime];
    [self.cursor drawWithContext:context];

//--
    CGContextSaveGState(context);
    NSString *header = [NSString stringWithFormat: @"Current Scene: %@ \nCurrent Time: %0.1f / %0.1f \nCamera Brightness: %0.1f", [currentScene getSceneID],currentSceneTime, CMTimeGetSeconds(player.currentItem.duration), [tracker getCameraBrightness]];
    
    
    NSDictionary *attributesDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                    CFBridgingRelease(CTFontCreateWithName((CFStringRef) @"HelveticaNeue", 28.0, NULL)),
                                    (NSString *)kCTFontAttributeName,
                                    [UIColor whiteColor].CGColor,
                                    (NSString *)kCTForegroundColorAttributeName,
                                    nil];
    
    
    NSAttributedString *headerString = [[NSAttributedString alloc] initWithString:header
                                                                       attributes:attributesDict];
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef)headerString);
    CGMutablePathRef path = CGPathCreateMutable();
    CGAffineTransform transform = CGAffineTransformMakeScale(1, -1);
    transform = CGAffineTransformTranslate(transform, 0, - bounds.size.height);
    
    CGRect frameText = bounds;
    CGRect newRectForUIKit = CGRectApplyAffineTransform(frameText, transform);
    CGPathAddRect(path, NULL, newRectForUIKit);
    
    CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), path, NULL);
    
    CGContextSetTextMatrix(context, CGAffineTransformIdentity);
    CGContextTranslateCTM(context, 0, bounds.size.height );
    CGContextScaleCTM(context, 1.0, -1.0);
    
    CTFrameDraw(frame, context);
    CFRelease(path);
    
//--
    
    
    UIImage* result = UIGraphicsGetImageFromCurrentImageContext();
    overlay.contents = (__bridge id)(result.CGImage);
    UIGraphicsEndImageContext();
    
}

- (void)sceneEnded:(NSNotification *)notification
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:[self->player currentItem]];
    [self->player advanceToNextItem];
    [currentScene deactivateWithTracker:tracker];
    currentScene = queuedScene;
    [currentScene makeActive];
    [self.cursor resetTimer];
}

@end
