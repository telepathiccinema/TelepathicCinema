#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

#include <cstddef>

namespace VisageSDK
{
	class MathUtils
	{
	public:
		static const float PI;
		static void smwcrand(unsigned int w, unsigned int z);
		static unsigned int mwcrand();

		static void doPerturbation(float s0, float ds, float dp, float& pertPosRow, float& pertPosCol, float& pertSize);
		static void calculateVarAndStd(float meanRow, float meanCol, float rows[], float cols[], int numOfPerturbs, float* var, 
			float* stdRows = NULL, float* stdCols = NULL);
		static float calculateVectorVariance(float* meanRows, float* meanCols, float** perturbRows, float** perturbCols, int numOfPerturbs, int numOfPoints);
		static int getVectorMedian(float** perturbRows, float** perturbCols, int numOfPerturbs, int numOfPoints);
		static float computeEuclidDistanceSquared(float rows1[], float cols1[], float rows2[], float cols2[], int numOfPoints);
	};
}

#endif // __MATHUTILS_H__