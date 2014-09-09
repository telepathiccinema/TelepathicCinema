//
//  TCRegion.h
//  Telepathic Cinema
//
//  Created by ket.ai on 8/29/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TCRegion : NSObject

@property NSString* target;
@property CGRect box;
@property float starttime;
@property float endtime;
@property int count;

-(id)initWithTarget: (NSString *) name
         withRect: (CGRect ) r
    withStartTime: (float) start
      withEndTime: (float) end;

-(void)checkHitWith: (CGRect ) target
             atTime: (float) t;

-(void)drawWithContext: (CGContextRef ) c
                  time: (float) t;

@end
