//
//  HistoricalGazeDataPoint.h
//  Telepathic Cinema
//
//  Created by user on 12/18/14.
//  Copyright (c) 2015 Ket.ai. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HistoricalGazeDataPoint : NSObject

@property (strong, nonatomic, readonly) NSString *targetID;
@property (strong, nonatomic, readonly) NSString *href;
@property (nonatomic, readonly) float value;

- (instancetype)initTarget: (NSString *) targetid
                   withValue: (float) value
                    withHref: (NSString *) href;
@end
