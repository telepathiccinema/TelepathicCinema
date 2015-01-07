//
//  TCGazeDB.m
//  Telepathic Cinema
//
//  Created by ket.ai on 12/18/14.
//  Copyright (c) 2014 Telepathic Cinema. All rights reserved.
//

#import "TCGazeDB.h"
#import "HistoricalGazeDataPoint.h"

@implementation TCGazeDB

- (instancetype)init
{
    self = [super init];
    if (self) {
        self->_gazeData = [[NSMutableArray alloc] init ];
    }
    return self;
}

- (void) reset
{
    [self->_gazeData removeAllObjects];
}

-(void) addRegionID:(NSString *)_id forScene:(NSString *)_scene withValue:(float)_value withHref: (NSString *)href
{
    [self addGazeDataPoint:[[HistoricalGazeDataPoint alloc]
                            initTarget:_id
                            withValue:_value
                            withHref: href]];

}

-(float) getValueForID:(NSString *)_id
{
    float value = 0;

    for(HistoricalGazeDataPoint *point in self->_gazeData)
    {
        if(point.targetID == _id)
        {
            return point.value;
        }
    }
    
    return value;
}


-(void) addGazeDataPoint: (HistoricalGazeDataPoint *) data
{
    [self->_gazeData addObject:data];
}

@end
