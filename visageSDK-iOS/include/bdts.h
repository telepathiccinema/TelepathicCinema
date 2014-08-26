#pragma once

//
typedef unsigned char pixel;

/*
	
*/
typedef struct _node
{
	struct _node* leq;
	struct _node* g;

	float p1[2], p2[2];
	float t;

	int leaf;

	int outDim;
	float out[2];

} node;

typedef struct
{
	int outDim;

	int numTrees;

	float weights[1024];
	
	node* trees[1024];
	
} DecisionForest;

/*
	
*/
int LoadForest(DecisionForest* df, const char path[]);

void DeleteForest(DecisionForest* df);

void EvaluateForest(float out[], DecisionForest* df, pixel img[], int I, int J, int ldim);
void EvaluateForestPartial(float out[], DecisionForest* df, int maxT, pixel img[], int I, int J, int ldim);