//#include "PupLocRuntime.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>


namespace VisageSDK {

struct Tree
{
	// dimension of the output
	int dim;

	// tree depth
	int depth;

	//
	int* features;
	char* tresholds;

	// output LUT
	float* lut;

};

struct Ensemble
{
	// dimension of the output
	int dim;

	//
	int numtrees;
	Tree* trees;

};

struct Cascade
{
	//
	int numensembles;	
	Ensemble* ensembles;

	// scale multiplier
	float smul;

};

struct odet
{
	int ncascades;

	float* tresholds;	
	Ensemble* dforests;
};

class LPLocRuntime
{

public:
	LPLocRuntime(void);
	~LPLocRuntime(void);

void SetFliph(int v);

void SetUseMacro(int m);

void TreeOutput(float o[], Tree* tree, float r, float c, float s, unsigned char pixels[], int R, int C, int ldim);

int TreeAllocate(Tree* tree, int depth, int dim);

int TreeLoad(Tree* tree, FILE* src);

void TreeDelete(Tree* tree);

void EnsembleOutput(float o[], Ensemble* e, float r, float c, float s, unsigned char pixels[], int R, int C, int ldim);

int EnsembleLoad(Ensemble* e, FILE* src);

void EnsembleDelete(Ensemble* e);

void dequantize(float* dst, short* src, int dim, float qIndex);

void dequantize(float* dst, signed char* src, int dim, float qIndex);

/*
	
*/


void CascadeOutput(Cascade* cascade, float* r, float* c, float s, unsigned char pixels[], int R, int C, int ldim, int stage_s = 0, int stage_e = -1);

int CascadeLoad(Cascade* cascade, const char* src);

void CascadeDelete(Cascade* cascade);

int odet_classify_region(struct odet* od, float* o, float r, float c, float s, unsigned char pixels[], int nrows, int ncols, int ldim);

int odet_load(struct odet* od, const char* src);

void odet_delete(odet* od);

int are_near(float r1, float c1, float s1, float r2, float c2, float s2, float p);

void ccdfs(int a[], int i, float rs[], float cs[], float ss[], int n, float p);

int find_connected_components(int a[], float rs[], float cs[], float ss[], int n, float p);

int cluster_detections(float rs[], float cs[], float ss[], float qs[], int n, float qcutoff);

int n3DetectObjects(float rs[], float cs[], float ss[], float qs[], int n,
					struct odet* od,
					unsigned char pixels[], int nrows, int ncols, int ldim,
					float scalefactor, float stridefactor, int smin, float qcutoff,
					int flags);

	
int fliph;

int use_macro;

float bdts_trees_factor;

void SetTreesFactor(float f);

private:
	int getfeature(int f, int r, int c, int s, unsigned char pixels[], int R, int C, int ldim);

};

}