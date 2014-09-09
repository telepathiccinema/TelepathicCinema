//
//  TCScene.m
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TCScene.h"

@implementation TCScene

-(id)initWithName: (NSString *) filename{
    //load the XML file and get the regions w/targets
    self.name = filename;
    
    //we can fake it for now
    self.regions = [[NSMutableArray alloc] init];
    [self.regions addObject:[[TCRegion alloc] initWithTarget:@"left" withRect: CGRectMake(0, 0, 1024/2, 768) withStartTime:0.0 withEndTime:10.0]];

    [self.regions addObject:[[TCRegion alloc] initWithTarget:@"right" withRect: CGRectMake(1024/2, 0, 1024, 768) withStartTime:5.0 withEndTime:22.0]];
    
    return self;
}

@end
