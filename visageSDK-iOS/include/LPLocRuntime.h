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
#ifndef LPLOCRUNTIME_H
#define LPLOCRUNTIME_H

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "Definitions.h"

#include <stdio.h>
#include <cv.h>

#ifdef EMSCRIPTEN
#include <cstddef>
#include <vector>
#else
#include <highgui.h>
#endif

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
	float smod;
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
};

struct odet
{
	int ncascades;

	float* tresholds;	
	Ensemble* dforests;
};

struct LBFRegressor
{
	bool mirror;
	int dim;
	int numStages;
	int dimFeatures;
	//Next four pointers are used in the case when we are dealing with quantised Ws
	float *qIndexRow;
	float *qIndexCol;
	short** rowStageW;
	short** colStageW;
	//Next two pointers are used in the case when we are dealing with nonquantised Ws
	float** FrowStageW;
	float** FcolStageW;
	float meanRow;
	float meanCol;
};

struct LBFStageRegressor
{
	int numFeatures;
	int numLandmarks;
	short** sW;
	float** fW;
	
	//CASCADE_SHORT_TWEAK
	float *scale;           // scaling factor for iW ( to get char from float)
	signed char** iW;
};

class VISAGE_DECLSPEC LPLocRuntime
{
public:
	LPLocRuntime(void);
	~LPLocRuntime(void);

void SetFliph(int v);

void SetStageModifier(int cnt);

int TreeOutput(float o[], Tree* tree, float r, float c, float s, unsigned char pixels[], int R, int C, int ldim);

int TreeOutput(Tree* tree, float r, float c, float s, unsigned char pixels[], int R, int C, int ldim);

int TreeAllocate(Tree* tree, int depth, int dim);
int TreeAllocate_LBF(Tree* tree, int depth, int dim);

int TreeLoad(Tree* tree, FILE* src);

int TreeLoad_LBF(Tree* tree, FILE* src);

void TreeDelete(Tree* tree);

void EnsembleOutput(float o[], Ensemble* e, float r, float c, float s, unsigned char pixels[], int R, int C, int ldim);

void EnsembleOutput(int leafIndexes[], Ensemble* e, float r, float c, float s, unsigned char pixels[], int R, int C, int ldim);

int EnsembleLoad(Ensemble* e, FILE* src);

void EnsembleDelete(Ensemble* e);

void dequantize(float* dst, short* src, int dim, float qIndex);

void dequantize(float* dst, signed char* src, int dim, float qIndex);

void dequantizeTiny(float* dst, unsigned char* src, float *lookup, int dim, int QUANT_TEST);

/*
	
*/


void CascadeOutput(Cascade* cascade, float* r, float* c, float s, unsigned char pixels[], int R, int C, int ldim, int stage_s = 0, int stage_e = -1);

int CascadeLoad(Cascade* cascade, FILE* file);

int CascadeLoad(Cascade* cascade, const char* src);

void CascadeDelete(Cascade* cascade);

int LBFLoad(LBFRegressor* regressor, const char* src, bool& isQuantized);

void LBFDelete(LBFRegressor* regressor);

void LBFDelete(LBFStageRegressor* regressor);

void LBFOutput(const std::vector<LBFRegressor*>& shapeRegressors, const std::vector<LBFStageRegressor*>& stageRegressors, const std::vector<Cascade*>& shapeCascades, float* rows, float* cols, float s, unsigned char pixels[], int R, int C, int ldim, bool lprofile = false);

void LBFEstimateYawFloat(float &decisionValue, const std::vector<Cascade*>& shapeCascades, double *SVM_W, float* rows, float* cols, float s, unsigned char pixels[], int R, int C, int ldim, bool lprofile);

int fliph;

int m_stage_modifier;

float bdts_trees_factor;

void SetTreesFactor(float f);

void LBFExtractBinaryFeatures(int* binaryFeatures, const std::vector<Cascade*>& shapeCascades, bool flipped[], int stage, float* rows, float* cols, float s, unsigned char pixels[], 
		int R, int C, int ldim, bool leftProfile);

static bool m_useShortAdditionTweak;

private:
	int getfeature(int f, int r, int c, int s, unsigned char pixels[], int R, int C, int ldim);
	int LBFLoadFloat(LBFRegressor* regressor, FILE *fp);
	int LBFLoadShort(LBFRegressor* regressor, FILE *fp);
	int LBFLoadTiny(LBFRegressor* regressor, FILE *fp);
	void LBFOutputShort(const std::vector<LBFRegressor*>& shapeRegressors, const std::vector<LBFStageRegressor*>& stageRegressors, const std::vector<Cascade*>& shapeCascades, float* rows, float* cols, float s, unsigned char pixels[], int R, int C, int ldim, bool lprofile = false);
	void LBFOutputFloat(const std::vector<LBFRegressor*>& shapeRegressors, const std::vector<LBFStageRegressor*>& stageRegressors, const std::vector<Cascade*>& shapeCascades, float* rows, float* cols, float s, unsigned char pixels[], int R, int C, int ldim, bool lprofile = false);
};

}
#endif
