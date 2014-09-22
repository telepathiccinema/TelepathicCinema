//
//  TCScene.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TCScene.h"

@implementation TCScene

/*
    Parse Scene file and generate regions
*/



-(id)initWithName: (NSString *) filename{
    NSLog(@"initing TCScene...%@", filename);
    //load the XML file and get the regions w/targets
    self.name = filename;
    self.regions = [[NSMutableArray alloc] init];

    //get the file
    NSString *extension = [filename pathExtension];
    NSString *name = [filename substringToIndex: [filename rangeOfString:[@"." stringByAppendingString:extension]].location] ;

    if([extension compare: @"smil" options:NSCaseInsensitiveSearch] == NSOrderedSame )
    {
        NSError *error;
        NSString *content = [NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:name ofType:extension] encoding:NSUTF8StringEncoding error:&error];
        
        DDXMLDocument *theDocument = [[DDXMLDocument alloc] initWithXMLString:content options:0 error:&error];
        NSArray *parResult = [theDocument nodesForXPath:@"/smil/body/par" error:&error];
        
        DDXMLElement *par = [parResult firstObject];
        //get the video
        NSArray *vidResult = [par nodesForXPath:@"video" error:&error];

        DDXMLElement *vid = [vidResult firstObject];
        self.videoFile = [[vid attributeForName:@"src"] stringValue];

        //get anchors and make regions
        NSArray *anchorResult = [par nodesForXPath:@"area" error:&error];
        for (DDXMLElement *anchor in anchorResult) {

            // pull all the attributes
            //<anchor href="2.smil" coords="0%,0%,50%,50%" begin="5s" end="32s"/>
            NSString *target = [[anchor attributeForName:@"href"] stringValue];
            NSString *coords = [[anchor attributeForName:@"coords"] stringValue];
            NSString *startTime = [[anchor attributeForName:@"begin"] stringValue];
            NSString *endTime = [[anchor attributeForName:@"end"] stringValue];
            NSString *duration = [[anchor attributeForName:@"dur"] stringValue];
            NSString *aId = [[anchor attributeForName:@"id"] stringValue];
            NSString *title = [[anchor attributeForName:@"title"] stringValue];
            float start = [startTime floatValue];
            float end = [endTime floatValue];
            float dur = [duration floatValue];
            
            if(!title)
                title = target;

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
            else if([duration hasSuffix:@"s"] == YES)
            {
                duration = [duration substringToIndex:([duration length] - 1)];
                dur = [duration floatValue];
                end = start + dur;
            }
            
            
            NSArray *parts = [coords componentsSeparatedByString: @","];
            if([parts count] == 4)
            {
                NSString *tlx = [parts objectAtIndex:0];
                NSString *tly = [parts objectAtIndex:1];
                NSString *brx = [parts objectAtIndex:2];
                NSString *bry = [parts objectAtIndex:3];

                float topLeftX = [self coordToFloat:tlx];
                float topLeftY = [self coordToFloat:tly];
                float bottomRightX = [self coordToFloat:brx];
                float bottomRightY = [self coordToFloat:bry];

                NSLog(@"Adding Region: target: %@ =>id: %@  => title: %@ => coords=>%f,%f,%f,%f start=>%f end=>%f dur=>%f",
                      target, aId, title, topLeftX, topLeftY, bottomRightX, bottomRightY, start, end, dur);
                
                [self.regions addObject:[[TCRegion alloc] initWithTarget:target
                                                               withTitle:title
                                                                withRect: CGRectMake(topLeftX*1024, topLeftY*768,
                                                                                     (bottomRightX-topLeftX)*1024, (bottomRightY-topLeftY)*768)
                                                           withStartTime:start
                                                             withEndTime:end
                                                           isCalibration:false]];
            }
            else{
                NSLog(@"No area found");
            }
        }

    }
    
    NSLog(@"New Scene Created for video: %@ with %i regions.", self.videoFile, [self.regions count] );
    return self;
}

-(float)coordToFloat: (NSString *) c
{
    float value;
    
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


@end
