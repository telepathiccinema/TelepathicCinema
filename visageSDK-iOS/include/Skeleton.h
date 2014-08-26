
#ifndef __Skeleton_h__
#define __Skeleton_h__

#include "Base.h"

namespace VisageSDK
{

/**
* Character skeleton, composed of joints.
* Skeleton is initialized upon creation by calling the _init function,
* which creates the root joint. Uninitialized skeleton has getRoot() == NULL.
* The root joint is always at index 0. It cannot be deleted or reassigned.
*/
class Skeleton
{

public:

	/**
	* Constructor.
	*/
	Skeleton( const string& id );

	/**
	* Destructor.
	*/
	~Skeleton();

	/**
	* Initializes the skeleton.
	*/
	void _init( const string& rootId );

	/**
	* Gets the identifier of the skeleton.
	*/
	const string& getId() const { return id; }

	/**
	* Gets a pointer to the skeleton's root joint.
	*/
	Joint* getRoot() const { return root; }

	/**
	* Creates a new joint.
	* Returns the pointer to the newly created joint or NULL
	* if the joint with that identifier already exists.
	*/
	Joint* createJoint( const string& jointId );

	/**
	* Deletes the specified joint.
	*/
	void deleteJointByIndex( int jointIndex );

	/**
	* Deletes the specified joint.
	*/
	void deleteJointById( const string& jointId );

	/**
	* Deletes all joints in the skeleton except root.
	*/
	void deleteAllJoints();

	/**
	* Returns true if the specified joint exists in the skeleton, otherwise false.
	*/
	bool hasJoint( const string& jointId ) const;

	/**
	* Gets a pointer to the specified joint.
	*/
	Joint* getJointByIndex( int jointIndex ) const;

	/**
	* Gets a pointer to the specified joint.
	* Returns NULL if the joint with that identifier doesn't exist in the skeleton.
	*/
	Joint* getJointById( const string& jointId ) const;

	/**
	* Gets the number of joints in the skeleton.
	*/
	int numberOfJoints() const;

	/**
	* Resets the skeleton to the initial pose.
	*/
	void resetToInitialPose();

private:

	string id; ///< Skeleton identifier.
	Joint* root; ///< Skeleton root joint.
	vector<Joint*> joints; ///< Array of pointers to the skeleton's joints.
	map<string, Joint*> jointsById; ///< Map of pointers to the skeleton's joints, indexed by joint identifiers.

};

}

#endif // __Skeleton_h__
