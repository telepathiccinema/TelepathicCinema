//
//  TCGazeDB.h
//  Telepathic Cinema
//
//  Created by ket.ai on 12/18/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HistoricalGazeDataPoint.h"

@interface TCGazeDB : NSObject
{
    
}
@property(strong, retain, readonly) NSMutableArray* gazeData;

-(void) reset;

-(bool) hasValueForID: (NSString *) _id;

-(void) saveForTarget: (NSString *)target
             withHref: (NSString *) href
            withCount:(int)count;

-(NSArray *) getDataForTarget:(NSString *)_target;

-(void) dump;

@end
