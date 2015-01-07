//
//  HistoricalGazeDataPoint.h
//  Telepathic Cinema
//
//  Created by user on 12/18/14.
//  Copyright (c) 2015 Ket.ai. All rights reserved.
//

#import "HistoricalGazeDataPoint.h"

@implementation HistoricalGazeDataPoint



- (instancetype)initTarget: (NSString *) targetid
                 withValue: (float) value
                  withHref: (NSString *) href
{
    self = [super init];
    if (self) {
        self->_targetID = targetid;
        self->_value = value;
        self->_href = href;
    }
    return self;
}

@end
