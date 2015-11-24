#ifndef GUARD_CASCADEMANAGER
#define GUARD_CASCADEMANAGER

#include "Definitions.h"
#include "LPLocRuntime.h"
#include "LBFShape.h"
#include <map>

enum LBFShapeType {MouthShape, LEyeShape, REyeShape, NoseShape, FullShape};

namespace VisageSDK
{
	class CascadeManager
	{
	public:
		CascadeManager(const char *dataPath, LPLocRuntime* LpLoc);

		~CascadeManager(void);

		static int getMirrorPointIndex(int group, int index);

		FaceOrientation getOrientationFromRadians(float headYawRadians);

		FaceOrientation getOrientationFromAngle(float headYawAngle);

		int getLBFPointsCount(FaceOrientation orientation = Frontal);

		std::vector<int> getLBFGroupIndexes(FaceOrientation orientation = Frontal);

		std::vector<int> getLBFPointIndexes(FaceOrientation orientation = Frontal);

		int getLBFPointArrayIndex(int group, int index, FaceOrientation orientation = Frontal);

		LBFShape* getLBFShape(LBFShapeType type = FullShape, FaceOrientation orientation = Frontal);

		bool isLBFDataLoaded();

		LBFShape* m_faceShapeFront;

		//LBFShape* m_faceShapeSemiProf;

		LBFShape* m_faceShapeProf;

		LBFShape* m_yawEstimator;

	private:
		std::pair<int,int> getCascadeParams(std::string cascadeName);

		static std::string getPointType(int group, int index);

		LPLocRuntime* m_lpLoc;

		std::vector<int> m_LBFGroupIndexesFront;
		std::vector<int> m_LBFGroupIndexesSemiProf; 
		std::vector<int> m_LBFGroupIndexesProf;

		std::vector<int> m_LBFPointIndexesFront;
		std::vector<int> m_LBFPointIndexesLSemiProf;
		std::vector<int> m_LBFPointIndexesRSemiProf;
		std::vector<int> m_LBFPointIndexesLProf;
		std::vector<int> m_LBFPointIndexesRProf;

		float m_lastHistAngle;

		bool m_faceShapeFrontLoaded;
		//bool m_faceShapeSemiProfLoaded;
		bool m_faceShapeProfLoaded;
		bool m_yawEstimatorLoaded;
	};
}

#endif
