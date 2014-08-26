
#ifndef __MorphTarget_h__
#define __MorphTarget_h__

#include "Base.h"

namespace VisageSDK
{

/**
* Target morph shape of a mesh.
*/
class MorphTarget
{

public:

	/**
	* Constructor.
	*/
	MorphTarget( const string& id, Mesh* owner );

	/**
	* Destructor.
	*/
	~MorphTarget();

	/**
	* Gets the identifier of the morph target.
	*/
	const string& getId() const { return id; }

	/**
	* Gets the identifier of the mesh that owns the morph target.
	*/
	Mesh* getMesh() const { return owner; }

	/**
	* Gets the current weight of the morph target.
	*/
	float getWeight() const;

	/**
	* Sets the current weight of the morph target.
	*/
	void setWeight( float weight );

private:

	string id; ///< Morph target identifier.
	Mesh* owner; ///< Pointer to the mesh that owns this morph target.

	float weight; ///< Morph target weight.

};

}

#endif // __MorphTarget_h__
