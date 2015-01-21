#import "TrackerWrapper.h"
#import "Utilities.h"

#import "visageVision.h"
#import "DemoObserver.h"
#import "Matrix4.h"
#import "Vector3.h"
#import "gluLookAt.h"
#import "UIDeviceHardware.h"


using namespace VisageSDK;

@implementation TrackerWrapper

@synthesize glView;

// comment this to use head tracking configuration
#define FACE_TRACKER

- (void)initTracker:(CustomGLView *)view
{
    //initialize multithreading
    BOOL isMT = [self startMultithread];
    //initialize licensing
    //example how to initialize license key
    char licenseFile[] = "637-055-282-782-699-137-004-446-835-604-044.vlc";
    initializeLicenseManager(licenseFile);
    
	glView = view;
    demoObserver = new DemoObserver();
    show = NO;
    calibrationState = CALIBRATION_UNCALIBRATED;
    showFacialFeatures = NO;
    isCalibrating = FALSE;
    fadeVideo = false;
    
    // choose configuration based on device at run-time
    NSString* deviceType = [UIDeviceHardware platform];
#ifdef FACE_TRACKER
    if ([deviceType hasPrefix:@"iPhone3"])
        tracker = new VisageTracker2("FFT - LowPerformance.cfg");      // iPhone4
    else if ([deviceType hasPrefix:@"iPhone4"])
        tracker = new VisageTracker2("FFT - MidPerformance.cfg");      // iPhone4S
    else if ([deviceType hasPrefix:@"iPhone5"])
        tracker = new VisageTracker2("FFT - HighPerformance.cfg");     // iPhone5
    else if ([deviceType hasPrefix:@"iPad2"])
        tracker = new VisageTracker2("FFT - MidPerformance.cfg");      // iPad2
    else if ([deviceType hasPrefix:@"iPad3"])
        tracker = new VisageTracker2("FFT - HighPerformance.cfg");     // iPad3
    else
        tracker = new VisageTracker2("FFT - MidPerformance.cfg");
#else
    if ([deviceType hasPrefix:@"iPhone3"])
        tracker = new VisageTracker2("HT - LowPerformance.cfg");       // iPhone4
    else if ([deviceType hasPrefix:@"iPhone4"])
        tracker = new VisageTracker2("HT - MidPerformance.cfg");       // iPhone4S
    else if ([deviceType hasPrefix:@"iPhone5"])
        tracker = new VisageTracker2("HT - HighPerformance.cfg");      // iPhone5
    else if ([deviceType hasPrefix:@"iPad2"])
        tracker = new VisageTracker2("HT - MidPerformance.cfg");       // iPad2
    else if ([deviceType hasPrefix:@"iPad3"])
        tracker = new VisageTracker2("HT - HighPerformance.cfg");      // iPad3
    else
        tracker = new VisageTracker2("HT - MidPerformance.cfg");
#endif
    
	//get OpenGL context size
	glWidth = glView.bounds.size.width;
	glHeight = glView.bounds.size.height;
	
	frameTexId = 0;
    instructionsTexId = 0;
    
    inGetTrackingResults = false;
}

- (void)startTrackingFromCam
{
    
#if TARGET_IPHONE_SIMULATOR
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                    message:@"No camera available on simulator."
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
#else
	[self stopTracker];
    
    while(inGetTrackingResults);
    
    tracker->attach(demoObserver);
    
    tracker->trackFromCam();
	isTracking = true;
#endif
}

- (void)startTrackingFromVideo:(NSString *)filename
{
	[self stopTracker];
    
    while(inGetTrackingResults)
        ;
    
    tracker->attach(demoObserver);
    
    if (filename) {
        tracker->trackFromVideo([filename UTF8String]);
    } else {
        NSString *bundlefile = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"jam1.mp4"];
        tracker->trackFromVideo([bundlefile UTF8String]);
    }
    
	isTracking = true;
}

- (void)startTrackingFromImage
{
	[self stopTracker];
    
    while(inGetTrackingResults)
        ;
    
    tracker->attach(demoObserver);
	
	isTracking = true;
}

- (void)stopTracker
{
	isTracking = false;
	tracker->stop();
    tracker->detach();
    
    if (demoObserver) {
        demoObserver->notifyCount = 0;
    }
    
    // delete texture used to display video
    if (frameTexId != 0) {
        glDeleteTextures(1, &frameTexId);
        frameTexId = 0;
    }
}

- (void)dummyThread:(id)object
{
	int n = 0;
	n++;
}

- (BOOL)startMultithread
{
	[NSThread detachNewThreadSelector:@selector(dummyThread:) toTarget:self withObject: nil];
	return [NSThread isMultiThreaded];
}

- (void) initAndSetInstructionsTexture
{
    // load image from file
    UIImage* image = [UIImage imageNamed:@"face.png"];
    GLubyte* imageData = (GLubyte*)malloc(image.size.width * image.size.height * 4);
    CGContextRef imageContext = CGBitmapContextCreate(imageData, image.size.width, image.size.height, 8, image.size.width * 4, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(imageContext, CGRectMake(0.0, 0.0, image.size.width, image.size.height), image.CGImage);
    CGContextRelease(imageContext);
    
    // Create The Texture
    glGenTextures(1, &instructionsTexId);
    
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, instructionsTexId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.size.width, image.size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

//display instructions as texture
- (void) displayInstructions
{
	glDisable(GL_DEPTH_TEST);
    
	glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();									// Reset The Projection Matrix
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();									// Reset The Projection Matrix
	glOrthof(-1.0f,1.0f,-1.0f,1.0f,-10.0f,10.0f);
	
	const GLfloat vertices[] = {
		-1.0f,-1.0f,-5.0f,
		1.0f,-1.0f,-5.0f,
		-1.0f,1.0f,-5.0f,
		1.0f,1.0f,-5.0f,
	};
	
	// tex coords are flipped upside down instead of an image
	const GLfloat texcoords[] = {
		0.0f,	1.0f,
		1.0f,	1.0f,
		0.0f,	0.0f,
		1.0f,	0.0f,
	};
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);					// Set Color To White
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	
	glViewport(0, glHeight-glWidth, glWidth, glWidth);
    
    glBindTexture(GL_TEXTURE_2D, instructionsTexId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
	
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    
	glClear(GL_DEPTH_BUFFER_BIT);
}

- (void) initFrameTexture:(const IplImage *)image
{
    // Find a texture size nearest to power of 2
    xTexSize = 1;
    yTexSize = 1;
    while((image->width / xTexSize) > 0) xTexSize <<= 1;
    while((image->height / yTexSize) > 0) yTexSize <<= 1;
    
	// Create The Texture
	glGenTextures(1, &frameTexId);
	
	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, frameTexId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// creating pow2 texture
	switch (image->nChannels) {
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, xTexSize, yTexSize, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
			break;
		case 3:
		case 4:
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xTexSize, yTexSize, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
			break;
	}
	
	xTexCoord = image->width / (float) xTexSize;
	yTexCoord = image->height / (float) yTexSize;
    
    videoAspect = image->width / (float) image->height;
    glHeight = glWidth / videoAspect;
}


- (void) setFrameTexture: (const IplImage *)image
{
	glBindTexture(GL_TEXTURE_2D, frameTexId);
    
	switch (image->nChannels) {
		case 3:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
			break;
		case 4:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_BGRA, GL_UNSIGNED_BYTE, image->imageData);
			break;
		case 1:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->imageData);
			break;
		default:
			return;
	}
}

//display video as texture
- (void) displayVideo
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glDisable(GL_DEPTH_TEST);
    
	glEnable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();									// Reset The Projection Matrix
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();									// Reset The Projection Matrix
	glOrthof(-1.0f,1.0f,-1.0f,1.0f,-10.0f,10.0f);
	
    //adjust to account for offset camera
	const GLfloat vertices[] = {
		-0.67,-1.0f,-5.0f,
		0.67,-1.0f,-5.0f,
		-0.67,1.0f,-5.0f,
		0.67,1.0f,-5.0f,
	};
	
	// tex coords are flipped upside down instead of an image
	const GLfloat texcoords[] = {
		(0.3f*xTexCoord),		yTexCoord,
		(xTexCoord),	yTexCoord,
		(0.3f*xTexCoord),		0.0f,
		(xTexCoord),	0.0f,
	};

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);					// Set Color To White
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	
	glViewport(0, 0, glWidth, glHeight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    
	glClear(GL_DEPTH_BUFFER_BIT);
    if(fadeVideo == YES)
    {
        videoOpacity -= .1;
        if(videoOpacity < 0)
        {
            videoOpacity = 0;
//            [self blank];
            [self display:NO];
        }
    }else{
        videoOpacity += .1;
        if(videoOpacity > 1)
        {
            videoOpacity = 1.0;
        }
        [self display:YES];
    }
    [self.glView.layer setOpacity:videoOpacity];
    //                [[self.view.layer.sublayers objectAtIndex:0] setOpacity:0.5];
//    [[self.glView.layer.sublayers objectAtIndex:0] setOpacity:videoOpacity];
}

-(void)fadeVideo:(bool) fade
{
    fadeVideo = fade;
}

- (void) drawPoint3D:(const float*) point withColor: (const float*) color andSize: (float) size
{
	glShadeModel(GL_FLAT);
	glColor4f(color[0], color[1], color[2], color[3]);
	glPointSize(size);
    
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, point);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//set the camera and its view (frustum)
+ (void) setupFrustumWithWidth: (int) width //width of image to be drawn
                     andHeight: (int) height //height of image to be drawn
                      andFocus: (float) f //focus
{
	GLfloat x_offset = 1;
	GLfloat y_offset = 1;
	if (width > height)
		x_offset = ((GLfloat)width)/((GLfloat)height);
	else if (width < height)
		y_offset = ((GLfloat)height)/((GLfloat)width);
	//Note:
	// FOV in radians is: fov*0.5 = arctan ((top-bottom)*0.5 / near)
	// In this case: FOV = 2 * arctan(frustum_y / frustum_near)
	//set frustum specs
	GLfloat frustum_near = 0.001f;
	GLfloat frustum_far = 30; //hard to estimate face too far away
	GLfloat frustum_x = x_offset*frustum_near/f;
	GLfloat frustum_y = y_offset*frustum_near/f;
	//set frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
	glFrustumf(-frustum_x,frustum_x,-frustum_y,frustum_y,frustum_near,frustum_far);
    
	glMatrixMode(GL_MODELVIEW);
	//clear matrix
	glLoadIdentity();
	//camera in (0,0,0) looking at (0,0,1) up vector (0,1,0)
	gluLookAt(0,0,0,0,0,1,0,1,0);
}

- (void) drawFaceModel: (const float *) vertices
				  vNum: (int) vertexNum
				  tris: (const int *)triangles
				  tNum:(int) triangleNum
			   tCoords:(const float *) texCoords
			frameWidth:(int)width
		   frameHeight:(int)height
{
    
	//enable stuff
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
	glShadeModel(GL_FLAT);
	//clear buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	//color=white -> texture gets the right colors and lighting (no effects)
	//glColor4f(1.0f,1.0f,1.0f,1.0f);
	//tinted red to show difference
	glColor4f(1.0f,0.5f,0.5f,1.0f);
    
	//vertex list
	glVertexPointer(3,GL_FLOAT,0,vertices);
	
	//texture coordinates are normalized to frame
	//and because frame is only in the part of our texture
	//we must scale texture coordinates to match
	//frame_width/tex_width and frame_height/tex_height
	//also x-coord must be flipped
	const GLfloat xScale = width/(GLfloat)xTexSize;
	const GLfloat yScale = height/(GLfloat)yTexSize;
	GLfloat *tex = new GLfloat[vertexNum*2];
	for (int i = 0; i < vertexNum*2; i+=2) {
		tex[i+0] = (1.0f-texCoords[i+0]) * xScale;
		tex[i+1] = texCoords[i+1] * yScale;
	}
	//texture coordinate list
	glTexCoordPointer(2,GL_FLOAT,0,tex);
	
	//OGLES only supports short as triangle index
	GLushort *tris = new GLushort[triangleNum*3];
	for (int i = 0; i < triangleNum*3; i+=3) {
		tris[i+0] = triangles[i+0];
		tris[i+1] = triangles[i+1];
		tris[i+2] = triangles[i+2];
	}
	//glDrawElements(GL_TRIANGLES,triangleNum*3,GL_UNSIGNED_SHORT,tris);
    glDisable(GL_TEXTURE_2D);
    
	glColor4f(0.0f,1.0f,0.0f,1.0f);
    GLushort *lines = new GLushort[triangleNum*6];
    for (int i = 0, j = 0; i < triangleNum*6; i+=6, j+=3) {
		lines[i+0] = lines[i+5] = triangles[j+0];
		lines[i+1] = lines[i+2] = triangles[j+1];
		lines[i+3] = lines[i+4] = triangles[j+2];
	}
	glDrawElements(GL_LINES,triangleNum*6,GL_UNSIGNED_SHORT,lines);
    
	delete[] tex;
	delete[] tris;
    delete[] lines;
    
	//disable stuff
	glDisable(GL_DEPTH_TEST);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
}

void drawFeaturePoints2D(int* points,
                         int count,
                         int drawType,
                         FDP* featurePoints2D)
{
	float *vertices;
	vertices = new float[count*3];
	int n = 0;
	FeaturePoint* fp;
	for (int i = 0; i < count; i++) {
		fp = const_cast<FeaturePoint*>( &featurePoints2D->getFP(points[2*i],points[2*i+1]) );
		if(fp->defined)
			if(fp->pos[0]!=0 && fp->pos[1]!=0)
			{
				vertices[3*n+0] = fp->pos[0];
				vertices[3*n+1] = fp->pos[1];
				vertices[3*n+2] = 0.0f;
				n++;
			}
	}
    
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(drawType, 0, n);
	glDisableClientState(GL_VERTEX_ARRAY);
    
	//clean-up
	delete[] vertices;
}

- (void) drawResultsFromFeaturePoints: (FDP*) featurePoints2D
{
	//sizes of the fdp's groups
    //    int GrSize[12] = {0,0,14,14,6,4,4,1,10,15,10,5};
    
    glPointSize(3);
    
    //draw chin points
    glColor4ub(128,128,255,255);
    
    static int chinPoints[] = {
        2,	14,
        2,	12,
        2,	1,
        2,	11,
        2,	13,
        2,	10,
    };
    static int chinLinesPoints[] = {
        2,	14,
        2,	12,
        2,	12,
        2,	1,
        2,	1,
        2,	11,
        2,	11,
        2,	13,
        2,	1,
        2,	10,
    };
    
    drawFeaturePoints2D(chinPoints, 6, GL_POINTS, featurePoints2D);
    drawFeaturePoints2D(chinLinesPoints, 10, GL_LINES, featurePoints2D);
    
    // draw inner lip points
    glColor4ub(236,0,0,255);
    
    static int innerLipPoints[] = {
        2,	2,
        2,	6,
        2,	4,
        2,	8,
        2,	3,
        2,	9,
        2,	5,
        2,	7,
    };
    
    drawFeaturePoints2D(innerLipPoints, 8, GL_POINTS, featurePoints2D);
    drawFeaturePoints2D(innerLipPoints, 8, GL_LINE_LOOP, featurePoints2D);
    
    // draw outer lip points
    glColor4ub(236,0,0,255);
    
    static int outerLipPoints[] = {
        8,	1,
        8,	10,
        8,	5,
        8,	3,
        8,	7,
        8,	2,
        8,	8,
        8,	4,
        8,	6,
        8,	9,
    };
    drawFeaturePoints2D(outerLipPoints, 10, GL_POINTS, featurePoints2D);
    drawFeaturePoints2D(outerLipPoints, 10, GL_LINE_LOOP, featurePoints2D);
    
    //draw nose
    glColor4ub(100,100,100,255);
    
    static int nosePoints[] = {
        9,	1,
        9,	2,
        9,	3,
        9,	4,
        9,	5,
        9,	6,
        9,	7,
        9,	8,
        9,	9,
        9,	10,
        9,	11,
        9,	12,
        9,	13,
        9,	14,
        9,	15,
    };
    static int noseLinesPoints1[] = {
        9,	15,		//part one
        9,	4,
        9,	2,
        9,	3,
        9,	1,
        9,	5,
    };
    static int noseLinesPoints2[] = {
        9,	6,		//part two
        9,	7,
        9,	13,
        9,	12,
        9,	14,
    };
    static int noseLinesPoints3[] = {
        9,	14,		//part three
        9,	2,
        9,	13,
        9,	1,
    };
    
    drawFeaturePoints2D(nosePoints, 15, GL_POINTS, featurePoints2D);
    drawFeaturePoints2D(noseLinesPoints1, 6, GL_LINE_LOOP, featurePoints2D);
    drawFeaturePoints2D(noseLinesPoints2, 5, GL_LINE_LOOP, featurePoints2D);
    drawFeaturePoints2D(noseLinesPoints3, 4, GL_LINES, featurePoints2D);
    
    //draw eyes
    //if eye is open, draw the iris
    if (trackingData.eyeClosure[1])
    {
        //if eye is open, draw the iris
        glColor4ub(255,255,0,255);
        
        static int irisPoints[] = {
            3,	5,
            3,	6,
        };
        drawFeaturePoints2D(irisPoints, 2, GL_POINTS,featurePoints2D);
    }
    if (trackingData.eyeClosure[1]){
        glColor4ub(255,143,32,255);
    }
    else
    {
        glColor4ub(255,0,0,255);
    }
    
    static int eyesPoints[] = {
        3,	1,
        3,	2,
        3,	3,
        3,	4,
        3,	7,
        3,	8,
        3,	9,
        3,	10,
        3,	11,
        3,	12,
        3,	13,
        3,	14,
    };
    static int eyesLinesPoints1[] = {
        3,	12,
        3,	14,
        3,	8,
        3,	10,
    };
    static int eyesLinesPoints2[] = {
        3,	12,
        3,	2,
        3,	8,
        3,	4,
    };
    static int eyesLinesPoints3[] = {
        3,	11,
        3,	13,
        3,	7,
        3,	9,
    };
    static int eyesLinesPoints4[] = {
        3,	11,
        3,	1,
        3,	7,
        3,	3,
    };
    
    drawFeaturePoints2D(eyesPoints, 12, GL_POINTS, featurePoints2D);
    drawFeaturePoints2D(eyesLinesPoints1, 4, GL_LINE_LOOP, featurePoints2D);
    drawFeaturePoints2D(eyesLinesPoints2, 4, GL_LINE_LOOP, featurePoints2D);
    drawFeaturePoints2D(eyesLinesPoints3, 4, GL_LINE_LOOP, featurePoints2D);
    drawFeaturePoints2D(eyesLinesPoints4, 4, GL_LINE_LOOP, featurePoints2D);
    
    
    //draw cheeks
    glColor4ub(100,100,100,255);
    
    static int cheekPoints[] = {
        5,	1,
        5,	2,
        5,	3,
        5,	4,
    };
    
    drawFeaturePoints2D(cheekPoints, 4, GL_POINTS, featurePoints2D);
    
    //draw ears
    static int earPoints[] = {
        10,	1,
        10,	2,
        10,	3,
        10,	4,
        10,	5,
        10,	6,
        10,	7,
        10,	8,
        10,	9,
        10,	10,
    };
    
    drawFeaturePoints2D(earPoints, 10, GL_POINTS, featurePoints2D);
    
    //draw lines connecting ears and cheeks
    static int earcheekLinesPoints1[] = {
        5,	2,
        5,	4,
        10,	10,
        10,	8,
    };
    static int earcheekLinesPoints2[] = {
        5,	1,
        5,	3,
        10,	9,
        10,	7,
    };
    
    drawFeaturePoints2D(earcheekLinesPoints1, 4, GL_LINE_LOOP, featurePoints2D);
    drawFeaturePoints2D(earcheekLinesPoints2, 4, GL_LINE_LOOP, featurePoints2D);
    
    //draw eyebrows
    glColor4ub(227,254,73,255);
    static int eyebrowPoints[] = {
        4,	1,
        4,	2,
        4,	3,
        4,	4,
        4,	5,
        4,	6,
    };
    static int eyebrowLinesPoints[] = {
        4,	6,
        4,	4,
        4,	4,
        4,	2,
        4,	1,
        4,	3,
        4,	3,
        4,	5,
    };
    drawFeaturePoints2D(eyebrowPoints, 6, GL_POINTS, featurePoints2D);
    drawFeaturePoints2D(eyebrowLinesPoints, 8, GL_LINES, featurePoints2D);
    
    //draw head/hair
    glColor4ub(100,100,100,255);
    
    static int hairPoints[] = {
        11,	1,
        11,	2,
        11,	3,
        11,	4,
        11,	5,
        11,	6,
    };
    static int hairLinesPoints[] = {
        11,	2,
        11,	1,
        11,	1,
        11,	3,
    };
    
    drawFeaturePoints2D(hairPoints, 6, GL_POINTS, featurePoints2D);
    drawFeaturePoints2D(hairLinesPoints, 4, GL_LINES, featurePoints2D);
	
}
- (float* ) getGaze
{
    tracker->getTrackingData(&trackingData);
    
    return trackingData.gazeDirection;
}

- (float* ) getFaceTranslation
{
    tracker->getTrackingData(&trackingData);
    return trackingData.faceTranslation;
}

-(float *) getGlobalGaze
{
    return trackingData.gazeDirectionGlobal;
}

-(CGPoint) getScreenSpaceGazeLocation
{
    return CGPointMake(trackingData.gazeData.x, trackingData.gazeData.y);
}

- (void) drawGazeDirection: (const float *) gazeDirection
              withFeatures: (FDP *) featurePoints3D
{
    if (!trackingData.eyeClosure[1])
        return;
    static float vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.05f
    };
    
    FeaturePoint* fp1;
    FeaturePoint* fp2;
    
    float tr[6];
    
    fp1 = const_cast<FeaturePoint*>( &featurePoints3D->getFP(3,5) );
    fp2 = const_cast<FeaturePoint*>( &featurePoints3D->getFP(3,6) );
    if(fp1->defined && fp2->defined)
    {
        tr[0] = fp1->pos[0];
        tr[1] = fp1->pos[1];
        tr[2] = fp1->pos[2];
        tr[3] = fp2->pos[0];
        tr[4] = fp2->pos[1];
        tr[5] = fp2->pos[2];
    }
    
    //float h_rot = trackingData->gazeDirection[0] * 180.0f/3.1415926f;
    //float v_rot = trackingData->gazeDirection[1] * 180.0f/3.1415926f;
    float h_rot = (gazeDirection[1] + 3.1415926f) * 180.0f/3.1415926f;
    float v_rot = gazeDirection[0] * 180.0f/3.1415926f;
    float roll = gazeDirection[2] * 180.0f/3.1415926f;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glPushMatrix();
    
    glTranslatef(tr[0],tr[1],tr[2]);
    glRotatef(h_rot, 0.0f, 1.0f, 0.0f);
    glRotatef(v_rot, 1.0f, 0.0f, 0.0f);
    glRotatef(roll, 0.0f, 0.0f, 1.0f);
    
    glLineWidth(2);
    
    glColor4ub(0,255,0,255);
    
    
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);
    
    glPopMatrix();
    
    glPushMatrix();
    
    glTranslatef(tr[3],tr[4],tr[5]);
    glRotatef(h_rot, 0.0f, 1.0f, 0.0f);
    glRotatef(v_rot, 1.0f, 0.0f, 0.0f);
    glRotatef(roll, 0.0f, 0.0f, 1.0f);
    
    
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
    
    glLineWidth(1);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

#include <mach/mach_time.h>

#define MEASURE_FRAMES 10
uint64_t last_times[MEASURE_FRAMES];
int framecount = -1;
int last_pts = 0;

- (int) getTrackingStatus
{
	int trackingStatus = tracker->getTrackingData(&trackingData);
    return trackingStatus;
}

//- (void)displayTrackingResults:(UILabel *)fpsLabel Status:(UILabel *)statusLabel Info:(UILabel *)infoLabel
- (void)displayTrackingResults
{
    // results are gathered only when observer has been notified at least once
    if (demoObserver != 0 && demoObserver->notifyCount == 0) {
        return;
    }
    
    if(!show)
        return;
    
    inGetTrackingResults = true;
    
	int trackingStatus = tracker->getTrackingData(&trackingData);
    
    int pts = trackingData.timeStamp;
    
    if (last_pts == pts) {
        inGetTrackingResults = false;
        return;
    }
    
    last_pts = pts;
    
    mach_timebase_info_data_t timeBaseInfo;
	mach_timebase_info(&timeBaseInfo);
    
    //measure the frame rate
	uint64_t currentTime = mach_absolute_time() * timeBaseInfo.numer / (timeBaseInfo.denom * 1e6);
	if(framecount == -1)
	{
		framecount = 0;
		for(int i=0;i<10;i++)
			last_times[i]=0;
	}
	framecount++;
	if(framecount == MEASURE_FRAMES) framecount = 0;
	float displayFrameRate = MEASURE_FRAMES * 1000.0f / (float)(currentTime - last_times[framecount]);
	last_times[framecount] = currentTime;
    //    if(framecount%30 == 0)
    //        NSLog(@"Framerate: %f", displayFrameRate);
    
	if (trackingStatus == TRACK_STAT_OK) {
        
        //drawing results
		[glView setOpenGLContext];
        
		//video texture is also used for face model texture
		if (!frameTexId && trackingData.frame != 0) {
			[self initFrameTexture: trackingData.frame];
		}
        
        if (trackingData.frame != 0) {
            [self setFrameTexture:trackingData.frame];
            [self displayVideo];
        }
        
		//set image specs
		glViewport(0,0,glWidth,glHeight);
        
        //enable stuff
		glShadeModel(GL_FLAT);
        
        
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();									// Reset The Projection Matrix
        
        glOrthof(0.0f,1.0f,0.0f,1.0f,-10.0f,10.0f);
        
        glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();									// Reset The ModelView Matrix
        
        if(showFacialFeatures)
            [self drawResultsFromFeaturePoints: trackingData.featurePoints2D];
        
        //tracker->EyeDetectorTest(glWidth,glHeight);
        
        
		//set camera frustum
        [TrackerWrapper setupFrustumWithWidth:glWidth andHeight:glHeight andFocus:trackingData.cameraFocus];
        if(showFacialFeatures)
            [self drawGazeDirection:trackingData.gazeDirectionGlobal withFeatures:trackingData.featurePoints3D];
		
        //		const GLfloat rightEye[] = {0.0f, 0.0f, 0.0f};
        //		const GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f,
        //            0.0f, 0.0f, 1.0f, 1.0f,
        //            0.0f, 0.0f, 1.0f, 1.0f,
        //            0.0f, 0.0f, 1.0f, 1.0f
        //		};
        
		glTranslatef(trackingData.faceTranslation[0],trackingData.faceTranslation[1],trackingData.faceTranslation[2]);
		glRotatef((trackingData.faceRotation[1]+3.1415926f)*180.0f/3.1415926f, 0.0f, 1.0f, 0.0f);
		glRotatef(trackingData.faceRotation[0]*180.0/3.1415926f, 1.0f, 0.0f, 0.0f);
		glRotatef(trackingData.faceRotation[2]*180.0/3.1415926f, 0.0f, 0.0f, 1.0f);
        
		//now we are in the 3d coordinate system of the head and we can draw what we want
		//note: origin is the right eye
        
        // draw 3D face model from the tracking data
        if (trackingData.frame != 0) {
            //[self drawFaceModel:trackingData.faceModelVertices vNum:trackingData.faceModelVertexCount tris:trackingData.faceModelTriangles tNum:trackingData.faceModelTriangleCount tCoords:trackingData.faceModelTextureCoords frameWidth:trackingData.frame->width frameHeight:trackingData.frame->height];
        }
        
        glColor4ub(255,255,0,255);
        
        //        static int irisPoints[] = {
        //            3,	5,
        //            3,	6,
        //        };
        //tracker->drawPoints2D(irisPoints, 2, GL_POINTS);
        
        // draw tracking data from origin (right eye)
		//[self drawPoint3D:rightEye withColor:blue andSize:10.0f];
		[glView swapOpenGLBuffers];
	}
	// only disply video
	else if (trackingStatus != TRACK_STAT_OFF && isTracking) {
		//drawing results
		[glView setOpenGLContext];
		
		//video texture is also used for face model texture
		if (!frameTexId && trackingData.frame != 0) {
			[self initFrameTexture: trackingData.frame];
		}
		
        if (trackingData.frame != 0) {
            [self setFrameTexture:trackingData.frame];
            [self displayVideo];
        }
        
        // display instructions during initialization
        if (trackingStatus == TRACK_STAT_INIT) {
            [self displayVideo];
        }
        
		[glView swapOpenGLBuffers];
	}
    
	const char *trackingStatusString;
	switch (trackingStatus) {
		case TRACK_STAT_OFF:
			trackingStatusString = "OFF";
			break;
		case TRACK_STAT_OK:
			trackingStatusString = "OK";
			break;
		case TRACK_STAT_RECOVERING:
			trackingStatusString = "RECOVERING";
			break;
		case TRACK_STAT_INIT:
			trackingStatusString = "INITIALIZING";
			break;
		default:
			trackingStatusString = "N/A";
			break;
	}
	
	// display the frame rate, position and other info
    
	float r[3] = {0};
	float t[3] = {0};
	if (trackingStatus == TRACK_STAT_OK)
    {
		for(int i=0;i<3;i++) {
			r[i] = trackingData.faceRotation[i] * 180.0f / 3.14159f; //rads to degs
			t[i] = trackingData.faceTranslation[i] * 100.0f;
            //translation is expressed in meters so this gives approximate values in centimeters if camera focus distance parameter is set correctly in configuration file
		}
	}
    
    inGetTrackingResults = false;
}

-(void)blank{
    [glView setOpenGLContext];
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    [glView swapOpenGLBuffers];
}

-(void)display:(BOOL)flag
{
    show = flag;
}

-(float) getCameraBrightness
{
    if(trackingData.frame)
    {
        CvRect temp = cvGetImageROI(trackingData.frame);
        cvSetImageROI(trackingData.frame, cvRect(0 ,trackingData.frame->height/2,trackingData.frame->width,20));
        CvScalar c = cvAvg(trackingData.frame);
        cvSetImageROI(trackingData.frame,temp); // reset old roi
        return c.val[0];
    }
    return 0.0f;
}

- (void) initGazeCalibration
{
    //start the tracker calibration via visage beta
    tracker->InitOnlineGazeCalibration();
    NSLog(@"Initializing online screen gaze calibration");
    isCalibrating = true;
    calibrationState = CALIBRATION_INSTRUCTIONS;
    initialCalibrationFaceFound = false;
    calibrationStartTime = CACurrentMediaTime();
    NSLog(@"Calibration StartTime: %f", calibrationStartTime);
}

- (void) addGazeCalibrationPoint: (float) x andY: (float) y
{
    //add tracker calibration via visage beta
    NSLog(@"Adding online screen gaze calibration point");
    tracker->AddGazeCalibrationPoint(x, y) ;
}

- (void) endGazeCalibration
{
    //finalize the online calibration via visage beta
    NSLog(@"Finalizing online screen gaze calibration");
    tracker->FinalizeOnlineGazeCalibration();
    isCalibrating = FALSE;
    calibrationState = CALIBRATION_CALIBRATED;
}
-(float) getFrameRate
{
    return tracker->getFrameRate();
}

- (void)updateCalibration
{
    if(!isCalibrating || calibrationState == CALIBRATION_CALIBRATED)
        return;
    
    //handle the states...
    switch (calibrationState) {
        case CALIBRATION_INIT:
            if(CACurrentMediaTime() > (calibrationStartTime + CALIBRATION_INTRO_TIMEOUT))
            {
                calibrationState = CALIBRATION_INSTRUCTIONS;
                calibrationStartTime = CACurrentMediaTime();
            }
            break;
            
        case CALIBRATION_INSTRUCTIONS:
            if(CACurrentMediaTime() > (calibrationStartTime + CALIBRATION_INSTRUCTIONS_TIME))
            {
                if([self getTrackingStatus] != TRACK_STAT_OK)
                    calibrationState = CALIBRATION_INITIAL_FACE_ERROR;
                else
                {
                    calibrationState = CALIBRATION_WATCHDOTS;
                    calibrationStartTime = CACurrentMediaTime();
                }
            }
            break;
        case CALIBRATION_INITIAL_FACE_ERROR:
            if([self getTrackingStatus] == TRACK_STAT_OK)
            {
                calibrationState = CALIBRATION_WATCHDOTS;
                calibrationStartTime = CACurrentMediaTime();
            }
            break;
        case CALIBRATION_WATCHDOTS:
            if(CACurrentMediaTime() > (calibrationStartTime + CALIBRATION_WATCHDOTS_TIME))
            {
                calibrationState = CALIBRATION_CALIBRATING;
            }
            break;
        case CALIBRATION_CALIBRATING:
            if([self getTrackingStatus] != TRACK_STAT_OK)
            {
                calibrationState = CALIBRATION_CALIBRATING_LOST;
            }
            break;
        case CALIBRATION_CALIBRATING_LOST:
            if([self getTrackingStatus] == TRACK_STAT_OK)
            {
                calibrationState = CALIBRATION_CALIBRATING;
            }
            break;
    }
    
}

-(int)getCalibrationState
{
    return calibrationState;
}
@end
