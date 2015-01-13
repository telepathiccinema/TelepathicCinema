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
//-(HistoricalGazeDataPoint *) getDataForTarget:(NSString *)_id

-(NSArray*) getDataForTarget:(NSString *)_id
{
    NSMutableArray *things = [[NSMutableArray alloc] initWithCapacity:0];
    for(HistoricalGazeDataPoint *point in self->_gazeData)
    {
        if([point.targetID isEqualToString: _id])
        {
            [things addObject:point];
        }
    }
    return [NSArray arrayWithArray:things];
}

-(bool) hasValueForID:(NSString *)_id
{
    for(HistoricalGazeDataPoint *point in self->_gazeData)
    {
        if([point.targetID isEqualToString:_id])
        {
            return true;
        }
    }
    return false;
}

-(void) saveForTarget: (NSString *)target
             withHref: (NSString *) href
            withCount:(int)count;

{
    [self->_gazeData addObject:[[HistoricalGazeDataPoint alloc] initTarget:target withCount:count withHref:href]];
}

-(void)dump
{
    NSLog(@"----------------------------------------");
    NSLog(@"Items: %i", [self->_gazeData count]);
    for(HistoricalGazeDataPoint *p in self->_gazeData)
    {
        NSLog(@"target: %@, href=%@, count: %i", p.targetID, p.href, p.count);
    }
    NSLog(@"----------------------------------------");
}

@end
