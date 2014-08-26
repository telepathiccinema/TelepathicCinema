
#ifndef __Base_h__
#define __Base_h__

typedef unsigned long vhandle; ///< Resource handle.
typedef unsigned long vflags; ///< A field of bits, for storing binary flags.
#define V_NULL_RESOURCE 0xFFFFFFFF

#ifdef _DEBUG
#define V_ASSERT(a) ( assert((a)) )
#else
#define V_ASSERT(a)
#endif

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

// forward declarations
class Vector2;
class Vector3;
class Vector;
class Quat;
class Matrix4;
class Matrix;
class Logger;
class VisageCharModel;
class VCM;
class FDP;
class FBAPMapping;
class Mesh;
class MorphTarget;
class Skeleton;
class Joint;

}

// C++
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <assert.h>
#include <algorithm>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

using namespace std;

namespace VisageSDK
{

// aliases for some containers
typedef vector<int> IntArray; ///< Array of integers.
typedef vector<string> StringArray; ///< Array of strings.
typedef map<string, string> StringMap; ///< Map of strings, indexed by strings.
typedef map<string, Vector3> NodeTransMap; ///< Map of node translations / positions, indexed by node identifiers.
typedef map<string, Quat> NodeRotMap; ///< Map of node rotations / orientations, indexed by node identifiers.
typedef vector<Vector3> Vector3Array; ///< Array of 3D vectors.
typedef vector<Vector2> Vector2Array; ///< Array of 2D vectors.
typedef map<string, IntArray> VertexIndicesMap; ///< Map of vertex index lists, indexed by mesh identifiers.
typedef map<string, FBAPMapping> FBAPMappingsMap; ///< Map of animation parameter mappings, indexed by parameter names.
typedef map<int, Vector3> VertexPositionsMap; ///< Map of vertex positions, indexed by vertex indices in the mesh.
typedef map<string, VertexPositionsMap> MorphTargetsMap; ///< Map containing a group of morph targets, indexed by morph target names.
typedef map<string, MorphTargetsMap> MTMeshMap; ///< Map of morph target groups, indexed by mesh identifiers.
typedef map<string, Vector3Array> MeshVertexData; ///< Map of vertex arrays, indexed by mesh identifiers.
typedef map<string, Vector2Array> MeshTexCoordData; ///< Map of texture coord. arrays, indexed by mesh identifiers.

// some handy string function templates
template < class T >
string toString( T x )
{
	ostringstream oss;
	oss << x;
	return oss.str();
}
template < class T >
T fromString( string xstr )
{
	T x;
	istringstream iss( xstr );
	iss >> x;
	return x;
}

}

#endif // __Base_h__
