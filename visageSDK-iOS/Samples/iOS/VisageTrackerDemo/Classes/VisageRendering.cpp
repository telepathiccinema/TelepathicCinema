///////////////////////////////////////////////////////////////////////////////
// 
// (c) Visage Technologies AB 2002 - 2015  All rights reserved. 
// 
// This file is part of visage|SDK(tm). 
// Unauthorized copying of this file, via any medium is strictly prohibited. 
// 
// No warranty, explicit or implicit, provided. 
// 
// This is proprietary software. No part of this software may be used or 
// reproduced in any form or by any means otherwise than in accordance with
// any written license granted by Visage Technologies AB. 
// 
/////////////////////////////////////////////////////////////////////////////

#include "VisageRendering.h"

namespace VisageSDK
{

#define VS_PI 3.14159265f
#define VS_DEG2RAD(a) ((a) * VS_PI / 180.0f)
#define VS_RAD2DEG(a) ((a) * 180.0f / VS_PI)

#if defined (IOS) || defined(ANDROID)
void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
				GLfloat centerx, GLfloat centery, GLfloat centerz,
				GLfloat upx, GLfloat upy, GLfloat upz)
{
	GLfloat m[16];
	GLfloat x[3], y[3], z[3];
	GLfloat mag;
	GLfloat invmag;

	/* Make rotation matrix */

	/* Z vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
	if (mag) {          /* mpichler, 19950515 */
		invmag = 1.0f / mag;
		z[0] *= invmag;
		z[1] *= invmag;
		z[2] *= invmag;
	}

	/* Y vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;

	/* X vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];

	/* Recompute Y = Z cross X */
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];

	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	* non-perpendicular unit-length vectors; so normalize x, y here
	*/

	mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	if (mag) {
		invmag = 1.0f / mag;
		x[0] *= invmag;
		x[1] *= invmag;
		x[2] *= invmag;
	}

	mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
	if (mag) {
		invmag = 1.0f / mag;
		y[0] *= invmag;
		y[1] *= invmag;
		y[2] *= invmag;
	}

#define M(row,col)  m[col*4+row]
	M(0, 0) = x[0];
	M(0, 1) = x[1];
	M(0, 2) = x[2];
	M(0, 3) = 0.0;
	M(1, 0) = y[0];
	M(1, 1) = y[1];
	M(1, 2) = y[2];
	M(1, 3) = 0.0;
	M(2, 0) = z[0];
	M(2, 1) = z[1];
	M(2, 2) = z[2];
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	M(3, 3) = 1.0;
#undef M
	glMultMatrixf(m);

	/* Translate Eye to Origin */
	glTranslatef(-eyex, -eyey, -eyez);
}

#endif

static GLuint frame_tex_id   = 0;
static float tex_x_coord  = 0;
static float tex_y_coord  = 0;
static bool video_texture_inited = false;
static int video_texture_width  = 0;
static int video_texture_height = 0;

typedef struct CubicPoly
{
	float c0, c1, c2, c3;
	float eval(float t)
	{
		float t2 = t*t;
		float t3 = t2 * t;
		return c0 + c1*t + c2*t2 + c3*t3;
	}
} CubicPoly;

typedef struct Vec2D
{
	Vec2D(float _x, float _y) : x(_x), y(_y) {}
	float x, y;
} Vec2D;

static void InitCubicPoly(float x0, float x1, float t0, float t1, CubicPoly &p)
{
	p.c0 = x0;
	p.c1 = t0;
	p.c2 = -3*x0 + 3*x1 - 2*t0 - t1;
	p.c3 = 2*x0 - 2*x1 + t0 + t1;
}

static void InitCatmullRom(float x0, float x1, float x2, float x3, CubicPoly &p)
{
	InitCubicPoly(x1, x2, 0.5f*(x2-x0), 0.5f*(x3-x1), p);
}

static void InitNonuniformCatmullRom(float x0, float x1, float x2, float x3, float dt0, float dt1, float dt2, CubicPoly &p)
{
	float t1 = (x1 - x0) / dt0 - (x2 - x0) / (dt0 + dt1) + (x2 - x1) / dt1;
	float t2 = (x2 - x1) / dt1 - (x3 - x1) / (dt1 + dt2) + (x3 - x2) / dt2;

	t1 *= dt1;
	t2 *= dt1;

	InitCubicPoly(x1, x2, t1, t2, p);
}

static float VecDistSquared(const Vec2D& p, const Vec2D& q)
{
	float dx = q.x - p.x;
	float dy = q.y - p.y;
	return dx*dx + dy*dy;
}

static void InitCentripetalCR(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2, const Vec2D& p3, CubicPoly &px, CubicPoly &py)
{
	float dt0 = powf(VecDistSquared(p0, p1), 0.25f);
	float dt1 = powf(VecDistSquared(p1, p2), 0.25f);
	float dt2 = powf(VecDistSquared(p2, p3), 0.25f);

	if (dt1 < 1e-4f)    dt1 = 1.0f;
	if (dt0 < 1e-4f)    dt0 = dt1;
	if (dt2 < 1e-4f)    dt2 = dt1;

	InitNonuniformCatmullRom(p0.x, p1.x, p2.x, p3.x, dt0, dt1, dt2, px);
	InitNonuniformCatmullRom(p0.y, p1.y, p2.y, p3.y, dt0, dt1, dt2, py);
}

static void SetupCamera(int width, int height, float f)
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
	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glFrustum(-frustum_x,frustum_x,-frustum_y,frustum_y,frustum_near,frustum_far);
	#else
	glFrustumf(-frustum_x,frustum_x,-frustum_y,frustum_y,frustum_near,frustum_far);
	#endif
	glMatrixMode(GL_MODELVIEW);
	//clear matrix
	glLoadIdentity();
	//camera in (0,0,0) looking at (0,0,1) up vector (0,1,0)
	gluLookAt(0,0,0,0,0,1,0,1,0);

}

static void ClearGL()
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
}

static void DrawSpline2D(int *points, int num, FaceData* trackingData)
{
	if (num < 2)
		return;

	vector<float> pointCoords;
	int n = 0;
	FDP *fdp = trackingData->featurePoints2D;

	for (int i = 0; i < num; i++)
	{
		const FeaturePoint &fp = fdp->getFP(points[2*i],points[2*i+1]);
		if(fp.defined && fp.pos[0]!=0 && fp.pos[1]!=0)
		{
			pointCoords.push_back(fp.pos[0]);
			pointCoords.push_back(fp.pos[1]);
			n++;
		}
	}

	if(pointCoords.size() == 0 || n <= 2)
		return;

	int factor = 10;
	vector<float> pointsToDraw;
	VisageRendering::CalcSpline(pointCoords, factor, pointsToDraw);
	int nVert = (int)pointsToDraw.size() / 2;
	float *vertices = new float[nVert*3];
	int cnt = 0;
	for (int i = 0; i < nVert; ++i)
	{
		vertices[3*i+0] = pointsToDraw.at(cnt++);
		vertices[3*i+1] = pointsToDraw.at(cnt++);
		vertices[3*i+2] = 0.0f;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINE_STRIP, 0, nVert);
	glDisableClientState(GL_VERTEX_ARRAY);

	//clean-up
	delete[] vertices;
}

static void DrawElipse(float x, float y, float radiusX, float radiusY)
{

#ifdef WIN32
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(x, y, 0.0f);
	static const int circle_points = 100;
	static const float angle = 2.0f * 3.1416f / circle_points;

	glBegin(GL_POLYGON);
	double angle1=0.0;
	glVertex2d(radiusX * cos(0.0) , radiusY * sin(0.0));
	for (int i=0; i<circle_points; i++)
	{
		glVertex2d(radiusX * cos(angle1), radiusY * sin(angle1));
		angle1 += angle;
	}
	glEnd();
	glPopMatrix();
#endif

}

static void DrawPoints2D(int *points, int num, bool singleColor, FaceData* trackingData)
{

#ifdef IOS
	float radius = (trackingData->faceScale / (float)trackingData->frame->width)*10;
#else
	float radius = (trackingData->faceScale / (float)trackingData->frame->width)*30;
#endif

	float radiusX = (trackingData->faceScale / (float)trackingData->frame->width) * 0.017f;
	float radiusY = (trackingData->faceScale / (float)trackingData->frame->height) * 0.017f;

	FDP *fdp = trackingData->featurePoints2D;

#ifdef WIN32
	for (int i = 0; i < num; i++)
	{
		const FeaturePoint &fp =  fdp->getFP(points[2*i],points[2*i+1]);

		if(fp.defined && fp.pos[0]!=0 && fp.pos[1]!=0)
		{
			float x = fp.pos[0];
			float y = fp.pos[1];

			glColor4ub(0,0,0,255);
			DrawElipse(x, y, radiusX, radiusY);

			if(!singleColor)
			{
				glColor4ub(0,255,255,255);
				DrawElipse(x, y, 0.6f * radiusX, 0.6f * radiusY);
			}
		}
	}
#else
	float *vertices = new float[num*2];
	int n = 0;
	for (int i = 0; i < num; i++)
	{
		const FeaturePoint &fp =  fdp->getFP(points[2*i],points[2*i+1]);
		if(fp.defined && fp.pos[0]!=0 && fp.pos[1]!=0)
		{
			vertices[2*n+0] = fp.pos[0];
			vertices[2*n+1] = fp.pos[1];
			n++;
		}
	}

	glEnable(GL_POINT_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glPointSize(radius);
	glColor4ub(0,0,0,255);
	glDrawArrays(GL_POINTS, 0, n);
	if (!singleColor)
	{
		glPointSize(0.8f*radius);
		glColor4ub(0,255,255,255);
		glDrawArrays(GL_POINTS, 0, n);
	}
	glDisableClientState(GL_VERTEX_ARRAY);

	//clean-up
	delete[] vertices;

#endif
}

static void DrawPoints3D(int *points, int num, bool singleColor, FaceData* trackingData, bool relative)
{

#ifdef IOS
	float radius = (trackingData->faceScale / (float)trackingData->frame->width)*10;
#else
	float radius = (trackingData->faceScale / (float)trackingData->frame->width)*30;
#endif

	FDP *fdp = relative ? trackingData->featurePoints3DRelative : trackingData->featurePoints3D;

	float *vertices = new float[num*3];
	int n = 0;
	for (int i = 0; i < num; i++)
	{
		const FeaturePoint &fp =  fdp->getFP(points[2*i],points[2*i+1]);
		if(fp.defined && fp.pos[0]!=0 && fp.pos[1]!=0)
		{
			vertices[3*n+0] = fp.pos[0];
			vertices[3*n+1] = fp.pos[1];
			vertices[3*n+2] = fp.pos[2];
			n++;
		}
	}

	glEnable(GL_POINT_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glPointSize(radius);
	glColor4ub(0,0,0,255);
	glDrawArrays(GL_POINTS, 0, n);
	if (!singleColor)
	{
		glPointSize(0.8f*radius);
		glColor4ub(0,255,255,255);
		glDrawArrays(GL_POINTS, 0, n);
	}
	glDisableClientState(GL_VERTEX_ARRAY);

	//clean-up
	delete[] vertices;
}

static int NearestPow2(int n)
{
	unsigned int v; // compute the next highest power of 2 of 32-bit v

	v = n;
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

static void InitFrameTex(int x_size, int y_size, const IplImage *image)
{
	//Create The Texture
	glGenTextures(1, &frame_tex_id);

	//Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, frame_tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Creating pow2 texture
	#ifdef IOS
	switch (image->nChannels) {
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, x_size, y_size, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
			break;
		case 3:
		case 4:
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x_size, y_size, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
			break;
	}
	#elif ANDROID
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, x_size, y_size, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
	#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x_size, y_size, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
	#endif

	tex_x_coord = (float) image->width / (float) x_size;
	tex_y_coord = (float) image->height / (float) y_size;
}

void VisageRendering::DisplayFrame(const IplImage *image, int width, int height)
{
	if (video_texture_inited && (video_texture_width!=image->width || video_texture_height!=image->height))
	{
		glDeleteTextures(1, &frame_tex_id);
		video_texture_inited = false;
	}

	if (!video_texture_inited )
	{
		InitFrameTex(NearestPow2(image->width), NearestPow2(image->height), image);
		video_texture_width = image->width;
		video_texture_height = image->height;
		video_texture_inited = true;
	}

	glBindTexture(GL_TEXTURE_2D, frame_tex_id);

	switch (image->nChannels) {
		case 3:
			#ifdef IOS
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
			#else
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_BGR, GL_UNSIGNED_BYTE, image->imageData);
			#endif
			break;
		case 4:
			#if defined(IOS) || defined(MAC_OS_X)
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_BGRA, GL_UNSIGNED_BYTE, image->imageData);
			#else
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->imageData);
			#endif
			break;
		case 1:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->imageData);
			break;
		default:
			return;
	}

	#if defined(WIN32) || defined(LINUX)
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_FOG_BIT | GL_STENCIL_BUFFER_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT );
	#endif

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND );
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glOrtho(0.0f,1.0f,0.0f,1.0f,-10.0f,10.0f);
	#else
	glOrthof(0.0f,1.0f,0.0f,1.0f,-10.0f,10.0f);
	#endif

	static GLfloat vertices[] = {
		0.0f,0.0f,-5.0f,
		1.0f,0.0f,-5.0f,
		0.0f,1.0f,-5.0f,
		1.0f,1.0f,-5.0f,
	};

	// tex coords are flipped upside down instead of an image
	GLfloat texcoords[] = {
		0.0f,			tex_y_coord,
		tex_x_coord,	tex_y_coord,
		0.0f,			0.0f,
		tex_x_coord,	0.0f,
	};

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	glViewport(0, 0, width, height);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	#if defined(WIN32) || defined(LINUX)
	glPopAttrib();
	#endif

	glClear(GL_DEPTH_BUFFER_BIT);
}

void VisageRendering::DisplayFeaturePoints(FaceData* trackingData, int width, int height, bool _3D, bool relative)
{
	glViewport(0, 0, width, height);

	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_FOG_BIT | GL_STENCIL_BUFFER_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT );
	#endif

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glOrtho(0.0f,1.0f,0.0f,1.0f,-10.0f,10.0f);
	#else
	glOrthof(0.0f,1.0f,0.0f,1.0f,-10.0f,10.0f);
	#endif

	if (_3D) {
		SetupCamera(width, height, trackingData->cameraFocus);

		if (relative) {
			const float *r = trackingData->faceRotation;
			const float *t = trackingData->faceTranslation;

			glTranslatef(t[0], t[1], t[2]);
			glRotatef(VS_RAD2DEG(r[1] + D_PI), 0.0f, 1.0f, 0.0f);
			glRotatef(VS_RAD2DEG(r[0]), 1.0f, 0.0f, 0.0f);
			glRotatef(VS_RAD2DEG(r[2]), 0.0f, 0.0f, 1.0f);
		}
	}

	static int chinPoints[] = {
		2,	1
	};

	if (_3D) DrawPoints3D(chinPoints, 1, false, trackingData, relative);
	else DrawPoints2D(chinPoints, 1, false, trackingData);

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
	if (_3D) DrawPoints3D(innerLipPoints, 8, false, trackingData, relative);
	else DrawPoints2D(innerLipPoints, 8, false, trackingData);

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
	if (_3D) DrawPoints3D(outerLipPoints, 10, false, trackingData, relative);
	else DrawPoints2D(outerLipPoints, 10, false, trackingData);

	static int nosePoints[] = {
		9,	1,
		9,	2,
		9,	3,
		9,	15
	};
	if (_3D) DrawPoints3D(nosePoints, 4, false, trackingData, relative);
	else DrawPoints2D(nosePoints, 4, false, trackingData);

	if(trackingData->eyeClosure[1] > 0.5f)
	{
		//if eye is open, draw the iris
		glColor4ub(200,80,0,255);
		static int irisPoints[] = {
			3,	6
		};
		if (_3D) DrawPoints3D(irisPoints, 1, false, trackingData, relative);
		else DrawPoints2D(irisPoints, 1, false, trackingData);
	}

	if(trackingData->eyeClosure[0] > 0.5f)
	{
		glColor4ub(200, 80, 0, 255);
		static int irisPoints[] = {
			3,	5
		};
		if (_3D) DrawPoints3D(irisPoints, 1, false, trackingData, relative);
		else DrawPoints2D(irisPoints, 1, false, trackingData);
	}

	static int eyesPointsR[] = {
		3,	2,
		3,	4,
		3,	8,
		3,	10,
		3,	12,
		3,	14,
		12, 6,
		12, 8,
		12, 10,
		12, 12
	};
	if (_3D) DrawPoints3D(eyesPointsR, 10, trackingData->eyeClosure[1] <= 0.5f, trackingData, relative);
	else DrawPoints2D(eyesPointsR, 10, trackingData->eyeClosure[1] <= 0.5f, trackingData);

	static int eyesPointsL[] = {
		3,	1,
		3,	3,
		3,	7,
		3,	9,
		3,	11,
		3,	13,
		12, 5,
		12, 7,
		12, 9,
		12, 11
	};
	if (_3D) DrawPoints3D(eyesPointsL, 10, trackingData->eyeClosure[0] <= 0.5f, trackingData, relative);
	else DrawPoints2D(eyesPointsL, 10, trackingData->eyeClosure[0] <= 0.5f, trackingData);

	static int eyebrowPoints[] = {
		4,	1,
		4,	2,
		4,	3,
		4,	4,
		4,	5,
		4,	6,
	};
	if (_3D) DrawPoints3D(eyebrowPoints, 6, false, trackingData, relative);
	else DrawPoints2D(eyebrowPoints, 6, false, trackingData);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glPopAttrib();
	#endif
}

void VisageRendering::DisplaySplines(FaceData* trackingData, int width, int height)
{
	glViewport(0,0,width,height);

	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_FOG_BIT | GL_STENCIL_BUFFER_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT );
	#endif

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glOrtho(0.0f,1.0f,0.0f,1.0f,-10.0f,10.0f);
	#else
	glOrthof(0.0f,1.0f,0.0f,1.0f,-10.0f,10.0f);
	#endif

	glPointSize(3);
	glLineWidth(2);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glColor4ub(176,196,222,160);
	static int outerUpperLipPoints[] = {
		8, 4,
		8, 6,
		8, 9,
		8, 1,
		8, 10,
		8, 5,
		8, 3,
	};
	DrawSpline2D(outerUpperLipPoints, 7, trackingData);

	static int outerLowerLipPoints[] = {
		8, 4,
		8, 8,
		8, 2,
		8, 7,
		8, 3,
	};
	DrawSpline2D(outerLowerLipPoints, 5, trackingData);

	static int innerUpperLipPoints[] = {
		2, 5,
		2, 7,
		2, 2,
		2, 6,
		2, 4,
	};
	DrawSpline2D(innerUpperLipPoints, 5, trackingData);

	static int innerLowerLipPoints[] = {
		2, 5,
		2, 9,
		2, 3,
		2, 8,
		2, 4,
	};
	DrawSpline2D(innerLowerLipPoints, 5, trackingData);

	static int noseLinePoints[] = {
		9,	1,
		9,	3,
		9,	2
	};
	DrawSpline2D(noseLinePoints, 3, trackingData);

	static int outerUpperEyePointsR[] = {
		3,	12,
		3,	14,
		3,	8
	};
	DrawSpline2D(outerUpperEyePointsR, 3, trackingData);

	static int outerLowerEyePointsR[] = {
		3,	8,
		3,	10,
		3,	12
	};
	DrawSpline2D(outerLowerEyePointsR, 3, trackingData);

	static int innerUpperEyePointsR[] = {
		3,	12,
		12, 10,
		3,	2,
		12, 6,
		3,	8
	};
	DrawSpline2D(innerUpperEyePointsR, 5, trackingData);

	static int innerLowerEyePointsR[] = {
		3,	8,
		12, 8,
		3,	4,
		12, 12,
		3,	12
	};
	DrawSpline2D(innerLowerEyePointsR, 5,trackingData);

	static int outerUpperEyePointsL[] = {
		3,	11,
		3,	13,
		3,	7
	};
	DrawSpline2D(outerUpperEyePointsL, 3, trackingData);

	static int outerLowerEyePointsL[] = {
		3,	7,
		3,	9,
		3,	11
	};
	DrawSpline2D(outerLowerEyePointsL, 3, trackingData);

	static int innerUpperEyePointsL[] = {
		3,	11,
		12, 9,
		3,	1,
		12, 5,
		3,	7
	};
	DrawSpline2D(innerUpperEyePointsL, 5, trackingData);

	static int innerLowerEyePointsL[] = {
		3,	7,
		12, 7,
		3,	3,
		12, 11,
		3,	11
	};
	DrawSpline2D(innerLowerEyePointsL, 5, trackingData);

	static int eyebrowLinesPointsR[] = {
		4,	6,
		4,	4,
		4,	2
	};
	DrawSpline2D(eyebrowLinesPointsR, 3, trackingData);

	static int eyebrowLinesPointsL[] = {
		4,	1,
		4,	3,
		4,	5
	};
	DrawSpline2D(eyebrowLinesPointsL, 3, trackingData);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glPopAttrib();
	#endif
}

void VisageRendering::DisplayGaze(FaceData* trackingData, int width, int height)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	SetupCamera(width, height, trackingData->cameraFocus);

	glShadeModel(GL_FLAT);

	static float vertices[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.04f
	};

	float tr[6] = {0, 0, 0, 0, 0, 0};

	FDP *fdp = trackingData->featurePoints3D;

	const FeaturePoint &leye = fdp->getFP(3,5);
	const FeaturePoint &reye = fdp->getFP(3,6);

	if(leye.defined && reye.defined)
	{
		tr[0] = leye.pos[0];
		tr[1] = leye.pos[1];
		tr[2] = leye.pos[2];
		tr[3] = reye.pos[0];
		tr[4] = reye.pos[1];
		tr[5] = reye.pos[2];
	}

	float h_rot = VS_RAD2DEG(trackingData->gazeDirectionGlobal[1] + VS_PI);
	float v_rot = VS_RAD2DEG(trackingData->gazeDirectionGlobal[0]);
	float roll  = VS_RAD2DEG(trackingData->gazeDirectionGlobal[2]);

	glEnableClientState(GL_VERTEX_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(tr[0],tr[1],tr[2]);
	glRotatef(h_rot, 0.0f, 1.0f, 0.0f);
	glRotatef(v_rot, 1.0f, 0.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);

	glLineWidth(2);

	glColor4ub(240,96,0,255);

	if(trackingData->eyeClosure[0] > 0.5f)
	{
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glPopMatrix();

	glPushMatrix();

	glTranslatef(tr[3],tr[4],tr[5]);
	glRotatef(h_rot, 0.0f, 1.0f, 0.0f);
	glRotatef(v_rot, 1.0f, 0.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);

	if(trackingData->eyeClosure[1] > 0.5f)
	{
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void VisageRendering::DisplayModelAxes(FaceData* trackingData, int width, int height)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	SetupCamera(width, height, trackingData->cameraFocus);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glShadeModel(GL_FLAT);

	//rotate and translate into the current coordinate system of the head
	const float *r = trackingData->faceRotation;
	const float *t = trackingData->faceTranslation;
	FDP *fdp = trackingData->featurePoints3D;

	const FeaturePoint &fp1 = fdp->getFP(4, 1);
	const FeaturePoint &fp2 = fdp->getFP(4, 2);

	glTranslatef((fp1.pos[0] + fp2.pos[0])/2.0f, (fp1.pos[1] + fp2.pos[1])/2.0f, (fp1.pos[2] + fp2.pos[2])/2.0f);
	glRotatef(VS_RAD2DEG(r[1] + D_PI), 0.0f, 1.0f, 0.0f);
	glRotatef(VS_RAD2DEG(r[0]), 1.0f, 0.0f, 0.0f);
	glRotatef(VS_RAD2DEG(r[2]), 0.0f, 0.0f, 1.0f);

	static float coordVertices[] = {
		0.0f,	0.0f,	0.0f,
		0.07f,	0.0f,	0.0f,
		0.0f,	0.0f,	0.0f,
		0.0f,	0.07f,	0.0f,
		0.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	0.07f,
	};

	static float coordColors[] = {
		1.0f, 0.0f, 0.0f, 0.25f,
		1.0f, 0.0f, 0.0f, 0.25f,
		0.0f, 0.0f, 1.0f, 0.25f,
		0.0f, 0.0f, 1.0f, 0.25f,
		0.0f, 1.0f, 0.0f, 0.25f,
		0.0f, 1.0f, 0.0f, 0.25f,
	};

	glLineWidth(2);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coordVertices);
	glColorPointer(4, GL_FLOAT, 0, coordColors);
	glDrawArrays(GL_LINES, 0, 6);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
	glPopAttrib();
	#endif
}

void VisageRendering::DisplayWireFrame(FaceData* trackingData, int width, int height)
{
	//set image specs
	glViewport(0,0,width,height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	SetupCamera(width, height, trackingData->cameraFocus);

	glEnableClientState(GL_VERTEX_ARRAY);
	glShadeModel(GL_FLAT);
	//set the color for the wireframe
	glColor4f(0.0f,1.0f,0.0f,1.0f);
	//vertex list
	glVertexPointer(3,GL_FLOAT,0,trackingData->faceModelVertices);

	glLineWidth(1);

	const float *r = trackingData->faceRotation;
	const float *t = trackingData->faceTranslation;

	glTranslatef(t[0], t[1], t[2]);
	glRotatef(VS_RAD2DEG(r[1] + D_PI), 0.0f, 1.0f, 0.0f);
	glRotatef(VS_RAD2DEG(r[0]), 1.0f, 0.0f, 0.0f);
	glRotatef(VS_RAD2DEG(r[2]), 0.0f, 0.0f, 1.0f);

	//draw the wireframe
	for (int i = 0; i < trackingData->faceModelTriangleCount; i++) {
#if defined(WIN32) || defined(LINUX) || defined(MAC_OS_X)
		glDrawElements(GL_LINE_LOOP,3,GL_UNSIGNED_INT,trackingData->faceModelTriangles+3*i);
#else
		GLushort triangle[] = {
			static_cast<GLushort>(trackingData->faceModelTriangles[3*i+0]),
			static_cast<GLushort>(trackingData->faceModelTriangles[3*i+1]),
			static_cast<GLushort>(trackingData->faceModelTriangles[3*i+2]),
		};
		glDrawElements(GL_LINE_LOOP,3,GL_UNSIGNED_SHORT,triangle);
#endif
	}
	glDisableClientState(GL_VERTEX_ARRAY);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void VisageRendering::CalcSpline(vector <float>& inputPoints, int ratio, vector <float>& outputPoints) {

	int nPoints, nPointsToDraw, nLines;

	nPoints = inputPoints.size()/2 + 2;
	nPointsToDraw = inputPoints.size()/2 + (inputPoints.size()/2 - 1) * ratio;
	nLines = nPoints - 1 - 2;

	inputPoints.insert(inputPoints.begin(), inputPoints[1] + (inputPoints[1] - inputPoints[3]));
	inputPoints.insert(inputPoints.begin(), inputPoints[1] + (inputPoints[1] - inputPoints[3]));
	inputPoints.insert(inputPoints.end(), inputPoints[inputPoints.size()/2-2] + (inputPoints[inputPoints.size()/2-2] - inputPoints[inputPoints.size()/2-4]));
	inputPoints.insert(inputPoints.end(), inputPoints[inputPoints.size()/2-1] + (inputPoints[inputPoints.size()/2-1] - inputPoints[inputPoints.size()/2-3]));

	Vec2D p0(0,0), p1(0,0), p2(0,0), p3(0,0);
	CubicPoly px, py;

	outputPoints.resize(2*nPointsToDraw);

	for(int i = 0; i < nPoints - 2; i++) {
		outputPoints[i*2*(ratio+1)] = inputPoints[2*i+2];
		outputPoints[i*2*(ratio+1)+1] = inputPoints[2*i+1+2];
	}

	for(int i = 0; i < 2*nLines; i=i+2) {
		p0.x = inputPoints[i];
		p0.y = inputPoints[i+1];
		p1.x = inputPoints[i+2];
		p1.y = inputPoints[i+3];
		p2.x = inputPoints[i+4];
		p2.y = inputPoints[i+5];
		p3.x = inputPoints[i+6];
		p3.y = inputPoints[i+7];

		InitCentripetalCR(p0, p1, p2, p3, px, py);

		for(int j = 1; j <= ratio; j++) {
			outputPoints[i*(ratio+1)+2*j] = (px.eval(1.00f/(ratio+1)*(j)));
			outputPoints[i*(ratio+1)+2*j+1] =(py.eval(1.00f/(ratio+1)*(j)));
		}

	}

	inputPoints.erase(inputPoints.begin(), inputPoints.begin()+2);
	inputPoints.erase(inputPoints.end()-2, inputPoints.end());
}

void VisageRendering::DisplayResults(FaceData* trackingData, int trackStat, int width, int height, int drawingOptions)
{
	if(drawingOptions & DISPLAY_FRAME)
	{
		ClearGL();
		DisplayFrame(trackingData->frame, width, height);
	}

	if(trackStat == TRACK_STAT_OK)
        
	{
        if(drawingOptions & DISPLAY_SPLINES)
        {
            DisplaySplines(trackingData, width, height);
        }
        
		if(drawingOptions & DISPLAY_FEATURE_POINTS)
		{
			DisplayFeaturePoints(trackingData, width, height); // draw 2D feature points
			//DisplayFeaturePoints(trackingData, width, height, true); // draw 3D feature points
			//DisplayFeaturePoints(trackingData, width, height, true, true); // draw relative 3D feature points
		}
		
		if(drawingOptions & DISPLAY_GAZE)
		{
			DisplayGaze(trackingData, width, height);
		}
		if(drawingOptions & DISPLAY_AXES)
		{
			DisplayModelAxes(trackingData, width, height);
		}
		if(drawingOptions & DISPLAY_WIRE_FRAME)
		{
			DisplayWireFrame(trackingData, width, height);
		}
	}

}

}
