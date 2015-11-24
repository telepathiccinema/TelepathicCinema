//
//  UIDeviceHardware.h
//
//  Used to determine EXACT version of device software is running on.

#import <Foundation/Foundation.h>

/**
 * Class that implements methods that are used to determine specific version of device software is running on.
 */
@interface UIDeviceHardware : NSObject 

/**
 * Returns exact device name.
 */
+ (NSString *) platform;
/**
 * Returns more descriptive device name.
 */
+ (NSString *) platformString;

@end