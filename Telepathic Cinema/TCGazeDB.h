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

-(void)addRegionID: (NSString *) _id
     atScene: (NSString *) _scene
   withValue: (float) _value;

-(float) getValueForID: (NSString *) _id;
-(void) addGazeDataPoint: (HistoricalGazeDataPoint *) data;

@end
