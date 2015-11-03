//
//  TCScene.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TCScene.h"

@implementation TCScene

@synthesize isCalibration;

/*
 Parse Scene file and generate regions
 */



-(id)initWithName: (NSString *) filename
         withGaze:(TCGaze *)_gaze
       withBounds:(CGRect)bounds_

{
    if ((self = [super init]))
    {
        NSLog(@"Loading scene for %@", filename);
        //load the XML file and get the regions w/targets
        self.name = filename;
        bounds = bounds_;
        self.regions = [[NSMutableArray alloc] init];
        calibrationRegion = nil;
        gaze = _gaze;
        
        //get the file
        NSString *extension = [filename pathExtension];
        NSString *name = [filename substringToIndex: [filename rangeOfString:[@"." stringByAppendingString:extension]].location] ;
        
        if([extension compare: @"smil" options:NSCaseInsensitiveSearch] == NSOrderedSame )
        {
            NSError *error;
            NSString *content = [NSString stringWithContentsOfFile:[[NSBundle mainBundle]
                                                                    pathForResource:name
                                                                    ofType:extension]
                                                          encoding:NSUTF8StringEncoding
                                                             error:&error];
            
            DDXMLDocument *theDocument = [[DDXMLDocument alloc] initWithXMLString:content options:0 error:&error];
            NSArray *parResult = [theDocument nodesForXPath:@"/smil/body/par" error:&error];
            
            DDXMLElement *par = [parResult firstObject];
            //get the video
            NSArray *vidResult = [par nodesForXPath:@"video" error:&error];
            
            DDXMLElement *vid = [vidResult firstObject];
            self.videoFile = [[vid attributeForName:@"src"] stringValue];
            self->sceneID = [[vid attributeForName:@"id"] stringValue];
            
            if(!self->sceneID)
                self->sceneID = filename;
            
            defaultTarget = [[vid attributeForName:@"href"] stringValue];
            
            if([self->sceneID isEqualToString:@"calibration"])
                self->isCalibration = YES;
            else
                self->isCalibration = NO;
            
            
            //get anchors and make regions
            NSArray *anchorResult = [par nodesForXPath:@"area" error:&error];
            for (DDXMLElement *anchor in anchorResult) {
                
                // pull all the attributes
                //<anchor href="2.smil" coords="0%,0%,50%,50%" begin="5s" end="32s"/>
                NSString *href = [[anchor attributeForName:@"href"] stringValue];
                NSString *coords = [[anchor attributeForName:@"coords"] stringValue];
                NSString *startTime = [[anchor attributeForName:@"begin"] stringValue];
                NSString *endTime = [[anchor attributeForName:@"end"] stringValue];
                NSString *regionDuration = [[anchor attributeForName:@"dur"] stringValue];
                NSString *aId = [[anchor attributeForName:@"id"] stringValue];
                NSString *title = [[anchor attributeForName:@"title"] stringValue];
                NSString *targets = [[anchor attributeForName:@"target"] stringValue];
                
                float start = [startTime floatValue];
                float end = [endTime floatValue];
                float dur = [regionDuration floatValue];
                
                if(!title)
                    title = href;
                //if we have a scene with a region/area containing an href="*" then
                //  we have to create a region looked up from our gaze DB
                if([href  isEqual: @"*"])
                {
                    NSLog(@"Found area for deferred decisions");
                    //check if entry exists
                    if([self->gaze.gazeHistory hasValueForID:filename])
                    {
                        //get entries and add regions
                        NSArray *things = [self->gaze.gazeHistory getDataForTarget:filename];
                        
                        for (HistoricalGazeDataPoint *p in things)
                        {
                            TCRegion* r = [[TCRegion alloc] initWithTarget:p.href withTitle:filename withValue:p.count];
                            [self.regions addObject:r];
                        }
                    }else{
                        NSLog(@"Failed to find historic gaze data for target: %@", filename);
                    }
                }
                else    //process as usual
                {
                    //make sense of them all
                    if([startTime hasSuffix:@"s"] == YES)
                    {
                        startTime = [startTime substringToIndex:([startTime length]-1)];
                        start = [startTime floatValue];
                    }
                    if(endTime != nil && [endTime hasSuffix:@"s"] == YES)
                    {
                        endTime = [endTime substringToIndex:([endTime length] - 1)];
                        end = [endTime floatValue];
                    }
                    else if([regionDuration hasSuffix:@"s"] == YES)
                    {
                        regionDuration = [regionDuration substringToIndex:([regionDuration length] - 1)];
                        dur = [regionDuration floatValue];
                        end = start + dur;
                    }
                    
                    
                    NSArray *parts = [coords componentsSeparatedByString: @","];
                    if([parts count] == 4)
                    {
                        NSString *tlx = [parts objectAtIndex:0];
                        NSString *tly = [parts objectAtIndex:1];
                        NSString *w = [parts objectAtIndex:2];
                        NSString *h = [parts objectAtIndex:3];
                        
                        float topLeftX = [self coordToFloat:tlx];
                        float topLeftY = [self coordToFloat:tly];
                        float width = [self coordToFloat:w];
                        float height = [self coordToFloat:h];
                        
                        NSLog(@"Adding Region: target: %@ =>id: %@  => title: %@ => coords=>%0.2f,%0.2f,%0.2f,%0.2f start=>%0.2f end=>%0.2f dur=>%0.2f ",
                              href, aId, title, topLeftX, topLeftY, width, height, start, end, dur);
                        TCRegion* r = [[TCRegion alloc] initWithTarget:href
                                                             withTitle:title
                                                              withRect: CGRectMake(topLeftX*bounds.size.width, topLeftY*bounds.size.height, (width)*bounds.size.width, (height)*bounds.size.height)
                                                         withStartTime:start
                                                           withEndTime:end
                                                         isCalibration:false];
                        
                        //tease out the targets (if any)
                        if(targets && [targets length] > 0)
                        {
                            [r setSaveTarget:targets];
                        }
                        
                        if([aId isEqualToString:@"calibration"])
                            calibrationRegion = r;
                        
                        [self.regions addObject:r];
                    }
                    else{
                        NSLog(@"No area found, last scene?");
                    }
                }
            }
        }
        
        NSLog(@"New Scene Created for video: %@ with %i regions. isCalibration=>%d", self.videoFile, [self.regions count] , isCalibration);
    }
    [self->gaze.gazeHistory dump];
    return self;
}

-(float)coordToFloat: (NSString *) c
{
    float value = 0.0;
    
    if([c hasSuffix:@"%"] == YES)
    {
        NSString* v = [c substringToIndex:([c length] - 1)];
        value = [v floatValue]/100;
    }
    return value;
}

-(NSString *)getTargetFile
{
    return [self.videoFile substringToIndex: [self.videoFile rangeOfString:[@"." stringByAppendingString:[self getTargetExtension]]].location];
}

-(NSString *)getTargetExtension
{
    return [self.videoFile pathExtension];
}

-(void) updateWithTime:(float) t
{
    if(isCalibration)
        return [self processCalibrationWthTime: t];
    
    for(TCRegion* r in self.regions)
    {
        [r checkHitWith: gaze.boundingBox atTime:t];
    }
}

-(void) processCalibrationWthTime: (float) time
{
    for(TCRegion* r in self.regions)
    {
        //only allow one active gaze point at a time
        if([r isActiveAtTime:time])
        {
            //use the center of the region as our calibration point
            [gaze calibrationPointX:r.box.origin.x + r.box.size.width*.5
                                  Y:r.box.origin.y + r.box.size.height*.5];
            return;
        }
    }
}

-(void) drawWithContext:(CGContextRef)context
               withTime:(float)t
{
    
    for(TCRegion* region in self.regions)
    {
        
        if([region drawWithContext:context time: t])
            if(isCalibration)
                return;
    }
}
-(NSString* )getNextScene
{
    //calibration only supports a single target
    
    if(isCalibration)
        return defaultTarget;
    
    TCRegion* winner;
    
    for(TCRegion* r in self.regions)
    {
        //regions with targets must save their data & we can omit them from being the next scene..
        if([r isSetForArchival])
            [r archiveDataWithTCGaze:self->gaze];
        else if(!winner)
            winner = r;
        else if(r.count > winner.count)
            winner = r;
    }
    
    if(winner)
        return winner.target;
    
    return defaultTarget;   //no place to go
}

-(void) makeActive
{
    if(isCalibration)
    {
        [gaze initCalibration];
    }
}

-(void)deactivateWithTracker:(TrackerWrapper *) tracker
{
    if(isCalibration)
        [gaze finalizeCalibration];
}

-(void)setDuration:(float)d
{
    duration = d;
}
-(NSString*) getSceneID
{
    return sceneID;
}


@end
