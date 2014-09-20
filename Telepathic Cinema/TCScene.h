//
//  TCScene.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.

#import <Foundation/Foundation.h>
#import "TCRegion.h"
#import "DDXML.h"

@interface TCScene : NSObject
@property(strong, retain) NSString* name;
@property(strong, retain) NSMutableArray* regions;
@property(strong, retain) NSString* videoFile;

-(id)initWithName: (NSString *) filename;
-(float)coordToFloat: (NSString *) c;
-(NSString *)getTargetFile;
-(NSString *)getTargetExtension;

@end
