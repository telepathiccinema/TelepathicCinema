
#ifndef __Mesh_h__
#define __Mesh_h__

#include "Base.h"

namespace VisageSDK
{

/**
* Simple class representing an animated mesh. The mesh may be attached to a joint.
* It may have one or more morph targets defined.
*/
class Mesh
{

	friend class Joint;

public:

	/**
	* Constructor.
	*/
	Mesh( const string& id );

	/**
	* Destructor.
	*/
	~Mesh();

	/**
	* Gets the identifier of the mesh.
	*/
	const string& getId() const { return id; }

	/**
	* Gets a pointer to the joint to which the mesh is attached.
	*/
	Joint* getParent() const;

	/**
	* Creates a new morph target.
	* Returns the pointer to the newly created morph target or NULL
	* if the morph target with that identifier already exists.
	*/
	MorphTarget* createMorphTarget( const string& mtId );

	/**
	* Deletes the specified morph target.
	*/
	void deleteMorphTargetByIndex( int mtIndex );

	/**
	* Deletes the specified morph target.
	*/
	void deleteMorphTargetById( const string& mtId );

	/**
	* Returns true if the specified morph target exists in the mesh, otherwise false.
	*/
	bool hasMorphTarget( const string& mtId ) const;

	/**
	* Gets a pointer to the specified morph target.
	*/
	MorphTarget* getMorphTargetByIndex( int mtIndex ) const;

	/**
	* Gets a pointer to the specified morph target.
	* Returns NULL if the morph target with that identifier doesn't exist in the mesh.
	*/
	MorphTarget* getMorphTargetById( const string& mtId ) const;

	/**
	* Gets the number of morph targets in the mesh.
	*/
	int numberOfMorphTargets() const;

	/**
	* Resets all morph target weights to zero.
	*/
	void resetMorphTargetWeights();

private:

	string id; ///< Mesh identifier.

	Joint* parent; ///< Joint to which the mesh is attached.
	vector<MorphTarget*> morphTargets; ///< Array of pointers to morph targets defined on this mesh.
	map<string, MorphTarget*> morphTargetsById; ///< Map of pointers to morph targets defined on this mesh, indexed by morph target identifiers.

};

}

#endif // __Mesh_h__
