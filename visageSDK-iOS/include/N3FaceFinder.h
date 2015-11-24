#ifndef GUARD_N3FACEFINDER
#define GUARD_N3FACEFINDER

#include "Definitions.h"
#include <cv.h>
#include <stdint.h>

namespace VisageSDK
{
	class N3FaceFinder
	{
	public:
		N3FaceFinder(void);
		~N3FaceFinder(void);
		
		static int findFaces(IplImage* frame, CvRect rects[], float minFaceScale = 0.2f);
		static int findBiggestFace(IplImage* frame, CvRect* rect);
		static int findFaceFast(IplImage* frame, CvRect* rect);
		static int trackFace(IplImage* currentFrame, IplImage* previousFrame, const CvPoint2D32f* previousDetection, const int npoints, CvRect &boundingBox);
		static int trackFace_patches(IplImage* currentFrame, IplImage* previousFrame, const CvPoint2D32f* previousDetection, const int npoints, CvRect &boundingBox);

		static void setCurrentYaw(float yaw);
		static void setMinTrackingFacePercentage(float minTrackingFacePercentage) { m_minTrackingFacePercentage = minTrackingFacePercentage; };
		static void setFullFaceDetectorCutoff(float cutoff) {m_fullDetCutoff = cutoff;}
		static void setSobelFlag(bool useSobel) {m_useSobel = useSobel;}

	private:
		static float m_currentYaw;
		static float m_minTrackingFacePercentage;
		static float m_fullDetCutoff;
		static bool m_useSobel;

		static int fullRangeDetection(IplImage* pyr[], int pyrdepth, float rs[], float cs[], float ss[], float qs[], 
			int minsize, int maxsize, 
			int (*run_facefinder)(float*, int, int, int, uint8_t*, int, int, int), 
			int flip, float scaleFactor, float strideFactor);

		static int findObjects(float rs[], float cs[], float ss[], float qs[], int maxndetections,
			int (*run_detection_cascade)(float*, int, int, int, uint8_t*, int, int, int),
			void* pixels, int nrows, int ncols, int ldim,
			float scalefactor, float stridefactor, float minsize, float maxsize);

		static int findObjectsN3(float rs[], float cs[], float ss[], float qs[], int maxndetections,
			int (*run_detection_cascade)(float*, int, int, int, uint8_t*, int, int, int),
			void* pixels, int nrows, int ncols, int ldim, float minfacepercentage);

		static int fullRangeDetection_fast(IplImage* image, float rs[], float cs[], float ss[], float qs[], int faceSize, int flip,
			int (*run_facefinder)(float*, int, int, int, uint8_t*, int, int, int));

		static int findObjects_fast(float rs[], float cs[], float ss[], float qs[], int maxndetections,
			int (*run_detection_cascade)(float*, int, int, int, uint8_t*, int, int, int),
			void* pixels, int nrows, int ncols, int ldim, int faceSize);

		static void getDetectionQuality(IplImage* frame, CvRect* rect, float &resultQ);

		static int clusterDetections(float rs[], float cs[], float ss[], float qs[], int n, float qcutoff);

		static int clusterDetections_new(float rs[], float cs[], float ss[], float qs[], int n, float qcutoff, float &bestr, float &bestc, float &bests, float &bestq);

		static int findConnectedComponents(int a[], float rs[], float cs[], float ss[], int n, float o);

		static void ccdfs(int a[], int i, float rs[], float cs[], float ss[], int n, float o);

		static float getOverlap(float r1, float c1, float s1, float r2, float c2, float s2);

		static float getIntersectionNorm(float r1, float c1, float s1, float r2, float c2, float s2);
	};
}

#endif