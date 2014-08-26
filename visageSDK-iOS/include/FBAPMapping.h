
#ifndef __FBAPMapping_h__
#define __FBAPMapping_h__

#include "Base.h"
#include "Math3D.h"

// C++
#ifdef _DEBUG
#include <assert.h>
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

/**
* Holds mapping data of an animation parameter to a morph target or a set of bones.
* Morph targets and bones are specified by identifiers. Actual morph target data
* can be accessed via implementations of VisageCharModel class.
*/
class VISAGE_DECLSPEC FBAPMapping
{

private:

	/// Animation parameter name.
	string name;

	/// Array of identifiers of meshes this parameter deforms
	/// via morph target animation.
	/// This enables iteration and indexed access of morph targets on these meshes.
	StringArray morphedMeshes;
	/// Map containing identifiers of all meshes this parameter deforms
	/// via morph target animation. A morph target identifier is specified for each mesh.
	StringMap morphTargets;

	/// Array of identifiers of translational nodes this parameter is mapped to.
	/// This enables iteration and indexed access of reference positions of these bones.
	StringArray transBones;
	/// Map containing identifiers of all translational bones this parameter is mapped to.
	/// For each bone, its reference position in local space is specified.
	/// This reference value determines which degree of freedom the parameter controls.
	NodeTransMap transBonesRefPositions;

	/// Array of identifiers of translational nodes this parameter is mapped to.
	/// This enables iteration and indexed access of reference orientations of these bones.
	StringArray rotBones;
	/// Map containing identifiers of all rotational bones this parameter is mapped to.
	/// For each bone, its reference rotation in local space is specified.
	/// This reference value determines which degree of freedom the parameter controls.
	NodeRotMap rotBonesRefOrientations;

	/// Animation parameter reference value.
	int refValue;

public:

	/**
	* Constructor. Creates an empty mapping for
	* @param name Animation parameter name.
	*/
	FBAPMapping();

	/**
	* Constructor. Creates an empty mapping for the specified parameter.
	* @param name Animation parameter name.
	*/
	FBAPMapping( const string& name );

	/**
	* Get animation parameter name.
	*/
	const string& getFBAPName() const;

	/**
	* Get the morph target identifier at the specified index.
	*/
	void getMorphTarget( int i, string& meshId, string& morphTargetId ) const;

	/**
	* Get the morph target identifier on the specified mesh.
	*
	* @return false if the parameter isn't mapped to a morph target
	* on the specified mesh, otherwise true.
	*/
	bool getMorphTarget( const string& meshId, string& morphTargetId ) const;

	/**
	* Map this parameter to the specified morph target. If the parameter already maps
	* to a morph target on the specified mesh, the existing mapping is overwritten.
	*/
	void mapToMorphTarget( const string& meshId, const string& morphTargetId );

	/**
	* Get number of morph targets.
	*/
	int numberOfMorphTargets() const;

	/**
	* Determine if this animation parameter maps to the specified morph target.
	*/
	bool isMappedToMorphTarget( const string &meshId, const string& morphTargetId ) const;

	/**
	* Get reference position of the bone at the specified index.
	*/
	void getRefPosition( int i, string& boneId, Vector3& position ) const;

	/**
	* Get reference position of the specified bone.
	*
	* @param boneId Bone identifier.
	* @param position Reference bone position.
	* @return false if the parameter isn't mapped to translation of the specified bone,
	* otherwise true.
	*/
	bool getRefPosition( const string& boneId, Vector3& position ) const;

	/**
	* Map this parameter to a translational degree of freedom 
	* of the specified bone.
	*
	* @param boneId Bone identifier.
	* @param position Reference bone position.
	*/
	void mapToBoneTranslation( const string& boneId, const Vector3& position );

	/**
	* Unmaps this parameter from the specified bone translation.
	* 
	* @param boneId Bone identifier.
	*/
	void unmapFromBoneTranslation( const string& boneId );

	/**
	* Get number of translational bones this animation parameter maps to.
	*/
	int numberOfTransBones() const;

	/**
	* Determine if this animation parameter maps to the specified bone translation.
	*/
	bool isMappedToTransBone( const string &boneId ) const;

	/**
	* Get reference orientation of the bone at the specified index.
	*/
	void getRefOrientation( int i, string& boneId, Quat& orientation ) const;

	/**
	* Get reference orientation of the specified bone.
	*
	* @param boneId Bone identifier.
	* @param position Reference bone orientation.
	* @return false if the parameter isn't mapped to rotation of the specified bone,
	* otherwise true.
	*/
	bool getRefOrientation( const string& boneId, Quat& orientation ) const;

	/**
	* Map this parameter to a rotational degree of freedom 
	* of the specified bone.
	*
	* @param boneId Bone identifier.
	* @param position Reference bone orientation.
	*/
	void mapToBoneRotation( const string& boneId, const Quat& orientation );

	/**
	* Unmaps this parameter from the specified bone rotation.
	* 
	* @param boneId Bone identifier.
	*/
	void unmapFromBoneRotation( const string& boneId );

	/**
	* Get number of rotational bones this animation parameter maps to.
	*/
	int numberOfRotBones() const;

	/**
	* Determine if this animation parameter maps to the specified bone rotation.
	*/
	bool isMappedToRotBone( const string &boneId ) const;

	/**
	* Get animation parameter reference value.
	*/
	int getRefValue() const;

	/**
	* Set animation parameter reference value.
	*/
	void setRefValue( int refValue );

	/**
	* Erases the mapping. Calling this method results in the parameter
	* not being mapped to a morph target or any bones.
	* Parameter reference value is set to zero.
	*/
	void unmap();
};

}


#endif // __FBAPMapping_h__
