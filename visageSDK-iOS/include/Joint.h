
#ifndef __Joint_h__
#define __Joint_h__

#include "Base.h"
#include "Math3D.h"

namespace VisageSDK
{

/**
* Skeleton joint.
*/
class VISAGE_DECLSPEC Joint
{

public:

	/**
	* Constructor.
	*/
	Joint( const string& id, Skeleton* owner );

	/**
	* Destructor.
	*/
	~Joint();

	/**
	* Gets the identifier of the joint.
	*/
	const string& getId() const { return id; }

	/**
	* Gets a pointer to the skeleton that owns the joint.
	*/
	Skeleton* getSkeleton() const { return owner; }

	/**
	* Gets the joint's local position.
	*/
	const Vector3& getPosition() const;

	/**
	* Sets the joint's local position.
	*/
	void setPosition( const Vector3& pos );

	/**
	* Gets the joint's world position.
	*/
	Vector3 getWorldPosition() const;

	/**
	* Gets the joint's initial local position.
	*/
	const Vector3& getInitialPosition() const;

	/**
	* Sets the joint's initial local position.
	*/
	void setInitialPosition( const Vector3& pos );

	/**
	* Gets the joint's local orientation.
	*/
	const Quat& getOrientation() const;

	/**
	* Sets the joint's local orientation.
	*/
	void setOrientation( const Quat& orient );

	/**
	* Gets the joint's world orientation.
	*/
	Quat getWorldOrientation() const;

	/**
	* Gets the joint's initial local orientation.
	*/
	const Quat& getInitialOrientation() const;

	/**
	* Sets the joint's initial local orientation.
	*/
	void setInitialOrientation( const Quat& orient );

	/**
	* Gets the joint's local scale.
	*/
	const Vector3& getScale() const;

	/**
	* Sets the joint's local scale.
	*/
	void setScale( const Vector3& scal );

	/**
	* Gets the joint's world scale.
	*/
	Vector3 getWorldScale() const;

	/**
	* Gets the joint's initial local scale.
	*/
	const Vector3& getInitialScale() const;

	/**
	* Sets the joint's initial local scale.
	*/
	void setInitialScale( const Vector3& scal );

	/**
	* Translates the joint.
	* The joint can be translated relative to its current position
	* or to its initial position.
	*/
	void translate( const Vector3& trans, bool relToInitial = false );

	/**
	* Rotates the joint.
	* The joint can be rotated relative to its current position
	* or to its initial position.
	*/
	void rotate( const Quat& rot, bool relToInitial = false );

	/**
	* Rotates the joint.
	* The joint can be rotated relative to its current orientation
	* or to its initial orientation.
	*/
	void rotate( float yaw, float pitch, float roll, bool relToInitial = false );

	/**
	* Scales the joint.
	* The joint can be scaled relative to its current scale
	* or to its initial scale.
	*/
	void scale( const Vector3& scal, bool relToInitial = false );

	/**
	* Gets a pointer to the joint's parent.
	*/
	Joint* getParent() const;

	/**
	* Attaches a joint to this one as a child joint.
	*/
	void addChild( Joint* joint );

	/**
	* Removes one of this joint's children.
	* The joint won't be deleted - to actually delete it,
	* use Skeleton::deleteJointById().
	*/
	void removeChild( int childIndex );

	/**
	* Detaches the specified child joint from this one
	* and attaches it to another joint.
	*/
	void moveChild( int childIndex, Joint* joint );

	/**
	* Gets a pointer to the specified child joint.
	*/
	Joint* getChild( int childIndex ) const;

	/**
	* Gets the number of the joint's children.
	*/
	int numberOfChildren() const;

	/**
	* Attaches the specified mesh to this joint.
	*/
	void attachMesh( Mesh* mesh );

	/**
	* Detaches the specified mesh from this joint.
	*/
	void detachMesh( int meshIndex );

	/**
	* Gets a pointer to a mesh attached to the joint.
	*/
	Mesh* getAttachedMesh( int meshIndex );

	/**
	* Gets the number of meshes attached to this joint.
	*/
	int numberOfAttachedMeshes();

private:

	string id; ///< Joint identifier.
	Skeleton* owner; ///< Pointer to the skeleton that owns this joint.

	Vector3 pos; ///< Current position (in local space).
	Quat orient; ///< Current orientation (in local space).
	Vector3 scal; ///< Current scale (in local space).
	Vector3 initPos; ///< Initial position (in local space).
	Quat initOrient; ///< Initial orientation (in local space).
	Vector3 initScal; ///< Initial scale (in local space).

	Joint* parent; ///< Pointer to the joint's parent. NULL if the joint is root.
	vector<Joint*> children; ///< Array of pointers to the joint's children.
	vector<Mesh*> attachedMeshes; ///< Array of pointers to meshes attached to this joint (if there are any).

};

}

#endif // __Joint_h__
