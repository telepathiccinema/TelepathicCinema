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
@synthesize defaultScene;

-(id)initWithView:(CustomGLView *)view
         andScene:(NSString * )filename
        andPlayer: (AVQueuePlayer *) avplayer
{
    overlay = [CALayer layer];
    overlay.frame = view.frame;
    overlay.opacity = .25;
    UIGraphicsBeginImageContextWithOptions(CGSizeMake(view.frame.size.width, view.frame.size.height), NO, 0.0);
    image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    overlay.contents = (__bridge id)(image.CGImage);
    overlay.hidden = false;
    currentScene = [[TCScene alloc] initWithName:filename];
    self.cursor = [[TCGaze alloc] init];
    self.cursor.boundingBox = CGRectMake(overlay.frame.size.width/2, overlay.frame.size.height/2, 100,100);
    self.cursor.confidence = 0;
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
    
    
    return self;
}


-(void)queueScene{
    
    TCRegion* winner = [currentScene getNextScene];
    
    if(!winner)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Fin."
                                                        message:@"Fin."
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        isDone = YES;
        [alert show];
        return;
    }

    queuedScene= [[TCScene alloc] initWithName: winner.target];
    
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
    }else{
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could not find file."
                                                        message:@"Failed to find resource."
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
    
    [self.cursor updateWithTracker:tracker];
    
    //check collisions on regions
    if(self.cursor.active == YES)
        [currentScene updateWithTracker:tracker withGaze:self.cursor withTime:CMTimeGetSeconds([self->player currentTime])];
}


-(void)draw{
    if(overlay.hidden)
        return;
    
    UIGraphicsBeginImageContext(overlay.bounds.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetStrokeColorWithColor(context, [[UIColor yellowColor] CGColor]);
    CGContextFillRect(context, CGRectMake(0.0f, 0.0f, image.size.width, image.size.height));
    CGContextSetLineWidth(context, 4.0f);
    
    [currentScene drawWithContext: context withTime: CMTimeGetSeconds([[self->player currentItem] currentTime])];
    [self.cursor drawWithContext:context];
    
    UIImage* result = UIGraphicsGetImageFromCurrentImageContext();
    overlay.contents = (__bridge id)(result.CGImage);
    UIGraphicsEndImageContext();
    
}

- (void)sceneEnded:(NSNotification *)notification
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:[self->player currentItem]];
    [self->player advanceToNextItem];
    currentScene = queuedScene;
    [self.cursor resetTimer];
}

@end
