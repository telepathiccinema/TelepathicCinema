#ifndef __MY_MATRIX_H__
#define __MY_MATRIX_H__


#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif


namespace VisageSDK
{

typedef float myMatrix[4][4];
typedef float myVector[4];

int VISAGE_DECLSPEC normalize(float *v);
void VISAGE_DECLSPEC cross(myVector r,myVector v1,myVector v2);
void VISAGE_DECLSPEC aa2quat(float *q,float *axis,float angle);
float VISAGE_DECLSPEC quat2aa(float *q,float *axis);
void VISAGE_DECLSPEC multQuat(float *qr, float *q1, float *q2);
void VISAGE_DECLSPEC multVectMat(myVector r, myVector v, myMatrix m);
void VISAGE_DECLSPEC multVect3Mat3(myVector r, myVector v, myMatrix m);
void VISAGE_DECLSPEC multVect3Mat(float* v, myMatrix m);
void VISAGE_DECLSPEC multVect3Mat(float *r, float *v, myMatrix m);
void VISAGE_DECLSPEC copyMat(myMatrix m1, myMatrix m2);
void VISAGE_DECLSPEC identityMat(myMatrix m);
void VISAGE_DECLSPEC multMat(myMatrix r, myMatrix m1, myMatrix m2);
int VISAGE_DECLSPEC invMat(myMatrix mr, myMatrix ma);
int VISAGE_DECLSPEC invMat3(myMatrix mr, myMatrix ma);
void VISAGE_DECLSPEC rotMatFromQuat(myMatrix r, float *quat);
void VISAGE_DECLSPEC quatFromRotMat(float *quat,myMatrix mat);
float VISAGE_DECLSPEC myfabs(float f);
int VISAGE_DECLSPEC fdiff(float a, float b);
float VISAGE_DECLSPEC vect3Dist(float *v);
void VISAGE_DECLSPEC vect3Diff(float *r, float *v1, float *v2);
float VISAGE_DECLSPEC vect3Dist(float *v1, float *v2);
void VISAGE_DECLSPEC uniformMatrix(myMatrix r, myMatrix in);
void VISAGE_DECLSPEC getMatrixComponents(myMatrix m,float *t,float *r,float *rq,float *s,float *so,float *soq);

}

#endif
