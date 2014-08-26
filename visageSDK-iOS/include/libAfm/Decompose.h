
#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

/**** Decompose.h - Basic declarations ****/
#ifndef _H_Decompose
#define _H_Decompose


namespace VisageSDK
{

typedef struct {float x, y, z, w;} Quater; /* Quaternion */
enum QuatPart {X, Y, Z, W};
typedef Quater HVect; /* Homogeneous 3D vector */
typedef float HMatrix[4][4]; /* Right-handed, for column vectors */
typedef struct {
    HVect t;	/* Translation components */
    Quater  q;	/* Essential rotation	  */
    Quater  u;	/* Stretch rotation	  */
    HVect k;	/* Stretch factors	  */
    float f;	/* Sign of determinant	  */
} AffineParts;
float polar_decomp(HMatrix M, HMatrix Q, HMatrix S);
HVect spect_decomp(HMatrix S, HMatrix U);
Quater snuggle(Quater q, HVect *k);
void VISAGE_DECLSPEC decomp_affine(HMatrix A, AffineParts *parts);
void invert_affine(AffineParts *parts, AffineParts *inverse);

}

#endif
