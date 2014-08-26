#include <cv.h>
#import "DemoFrameGrabber.h"

using namespace VisageSDK;

// helper function for converting UIImage to IplImage
static IplImage *CreateIplImageFromUIImage(UIImage *image, int nChannels) {
	CGImageRef imageRef = image.CGImage;
    
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	IplImage *iplimage = cvCreateImage(cvSize(image.size.width, image.size.height), IPL_DEPTH_8U, 4);
	CGContextRef contextRef = CGBitmapContextCreate(iplimage->imageData, iplimage->width, iplimage->height,
													iplimage->depth, iplimage->widthStep,
													colorSpace, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault);
	CGContextDrawImage(contextRef, CGRectMake(0, 0, image.size.width, image.size.height), imageRef);
	CGContextRelease(contextRef);
	CGColorSpaceRelease(colorSpace);
    
	IplImage *ret = cvCreateImage(cvGetSize(iplimage), IPL_DEPTH_8U, nChannels); 
	switch (nChannels) {
		case 1:
			cvCvtColor(iplimage, ret, CV_RGBA2GRAY);
			break;
		case 4:
			cvCvtColor(iplimage, ret, CV_RGBA2BGRA);
			break;
		case 3:
			// fallthrough
		default:
			cvCvtColor(iplimage, ret, CV_RGBA2BGR);
			break;
	}
	
	cvReleaseImage(&iplimage);
	return ret;
}

DemoFrameGrabber::DemoFrameGrabber(void)
{
    NSString *path = [[NSBundle mainBundle] pathForResource:@"w_gr" ofType:@"jpg"];
	UIImage *image = [UIImage imageWithContentsOfFile:path];
    
    int w = image.size.width;
    w = (w / 4) * 4;
    int h = image.size.height;
    h = (h / 4) * 4;
    
    CGSize size;
    size.width = w;
    size.height = h;
    UIGraphicsBeginImageContextWithOptions(size, NO, 0.0);
    [image drawInRect:CGRectMake(0, 0, size.width, size.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    frame = CreateIplImageFromUIImage(newImage, 4);
    
    width = frame->width;
    height = frame->height;
    format = VISAGE_FRAMEGRABBER_FMT_BGRA;
    origin = VISAGE_FRAMEGRABBER_ORIGIN_TL;
    
};

unsigned char *DemoFrameGrabber::GrabFrame(long &timeStamp)
{
    timeStamp = -1;

    unsigned char* ret = (unsigned char*) frame->imageData;

    return ret;
}

DemoFrameGrabber::~DemoFrameGrabber(void)
{
    // release loaded image
    if (frame)
        cvReleaseImage(&frame);
}
