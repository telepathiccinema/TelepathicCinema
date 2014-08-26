#import <Foundation/Foundation.h>

#define NSStringToString(s) (std::string([s cStringUsingEncoding: NSASCIIStringEncoding]))
#define StringToNSString(s) ([NSString stringWithUTF8String: (s).c_str()])

@interface Utilities : NSObject {

}

+(NSString *)bundlePath:(NSString *)fileName;
+(NSString *)documentsPath:(NSString *)fileName;

@end
