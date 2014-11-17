#ifndef WEIGHTER_H
#define WEIGHTER_H

#include <vector>
#include <string>
#include "TrackerGazeCalibrator.h"
#include <cmath>

namespace VisageSDK
{
class Weighter
{
public:
	Weighter(void);
	~Weighter(void);

	void AddWeight(ScreenSpaceGazeData* current, ScreenSpaceGazeData* next);
	int GetWeight(int index);
private:
	std::vector<int> weights;
};
}
#endif