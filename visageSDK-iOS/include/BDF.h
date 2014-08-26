#ifndef __BDF_h__
#define __BDF_h__

#include <cv.h>
#include "bdts.h"

namespace VisageSDK
{

class BDF
{
public:
	BDF(const char* path);
	~BDF();

	float* Evaluate(IplImage* image);
    float* y;

private:
	DecisionForest forest;
};

}

#endif // __BDF_h__