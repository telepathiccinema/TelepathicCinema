#include <cv.h>
#import "VisageFeaturesDetectorWrapper.h"
#import "Utilities.h"
#import "Common.h"
#define MAX_SIZE 768

using namespace VisageSDK;

@implementation VisageFeaturesDetectorWrapper

- (void)initDetector
{
    NSLog(@"init detector");
    
    //initialize licensing
    //example how to initialize license key
    //initializeLicenseManager("license-key-filename.vlc");
    
    visageFeaturesDetector = new VisageFeaturesDetector();
    visageFeaturesDetector->Initialize([Utilities bundlePath:@"bdtsdata"].UTF8String);
}

- (UIImage*)drawFeatures:(UIImage *)image
{
    // if detector not initialized return the input image
    if (visageFeaturesDetector == NULL)
        return image;
    
    // convert input UIImage to IplImage
    IplImage* input = IplImageFromUIImage(image, 4);
    
    // create the output image by copying the input image
    IplImage* output = cvCreateImage(cvGetSize(input), input->depth, input->nChannels);
    cvCopyImage(input, output);
    
    // resize input image
    float scale = (float)MAX_SIZE / MAX(input->width, input->height);
    IplImage* resizedInput = cvCreateImage(cvSize(input->width * scale, input->height * scale), input->depth, input->nChannels);
    cvResize(input, resizedInput);
 
    // detect features
    int maxFaces = 100;
    FaceData* data = new FaceData[maxFaces];
    int faces = -1;
    faces = visageFeaturesDetector->detectFacialFeatures(resizedInput, data, maxFaces);
    
    if(faces > 0)
    {
        visageFeaturesDetector->drawResults(resizedInput);
    }
    // draw features into the output image
    
    
    // convert the output IplImage to UIImage
    UIImage* returnImage = UIImageFromIplImage(resizedInput);
    
    // release the input and output IplImages
    cvReleaseImage(&input);
    cvReleaseImage(&resizedInput);
    cvReleaseImage(&output);
    
    delete[] data;
    // return
    return returnImage;
}

// helper function for converting UIImage to IplImage
static IplImage *IplImageFromUIImage(UIImage *image, int nChannels) {
    
    UIImage *rotatedImage = image;
    if (image.imageOrientation == UIImageOrientationRight)
        rotatedImage = [[UIImage alloc] initWithCGImage:image.CGImage scale:1.0 orientation:UIImageOrientationUp];
	
    CGImageRef imageRef = rotatedImage.CGImage;
    
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        
    IplImage *iplimage = cvCreateImage(cvSize(rotatedImage.size.width, rotatedImage.size.height), IPL_DEPTH_8U, 4);
    CGContextRef contextRef = CGBitmapContextCreate(iplimage->imageData, iplimage->width, iplimage->height,
                                       iplimage->depth, iplimage->widthStep,
                                       colorSpace, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault);
    CGContextDrawImage(contextRef, CGRectMake(0, 0, rotatedImage.size.width, rotatedImage.size.height), imageRef);

    
	    
    CGContextRelease(contextRef);
	CGColorSpaceRelease(colorSpace);
    
	IplImage *ret = cvCreateImage(cvGetSize(iplimage), IPL_DEPTH_8U, 3); 
	switch (nChannels) {
		case 1:
			cvCvtColor(iplimage, ret, CV_RGBA2GRAY);
			break;
		default:
			cvCvtColor(iplimage, ret, CV_RGBA2BGR);
			break;
	}
    
    cvReleaseImage(&iplimage);
    NSLog(@"Image orientation: %d", image.imageOrientation);
    
    if (image.imageOrientation == UIImageOrientationRight)
    {
        IplImage *transposedRet = cvCreateImage(cvSize(ret->height, ret->width), ret->depth, ret->nChannels);
        cvFlip(ret);
        cvTranspose(ret, transposedRet);
        
        cvReleaseImage(&ret);
        return transposedRet;
    }
    
     if (image.imageOrientation == UIImageOrientationRight)
     {
         [rotatedImage release];
     }
    
    return ret;
}

// helper function for creating an UIImage from an IplImage
static UIImage* UIImageFromIplImage(IplImage* image) {
    
    // convert to rgb
    IplImage* convertedImage = cvCreateImage(cvGetSize(image), image->depth, 4);
    cvCvtColor(image, convertedImage, CV_BGR2RGBA);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSData *data = [NSData dataWithBytes:convertedImage->imageData length:convertedImage->imageSize];
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((CFDataRef)data);
    CGImageRef imageRef = CGImageCreate(convertedImage->width, convertedImage->height,
                                        convertedImage->depth, convertedImage->depth * convertedImage->nChannels, convertedImage->widthStep,
                                        colorSpace, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault,
                                        provider, NULL, false, kCGRenderingIntentDefault);
    UIImage *ret = [UIImage imageWithCGImage:imageRef scale:1.0 orientation:UIImageOrientationUp];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    // release convertedImage
    cvReleaseImage(&convertedImage);
    return ret;
}

@end
