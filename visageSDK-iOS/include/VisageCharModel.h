
#ifndef __VisageCharModel_h__
#define __VisageCharModel_h__

#include "Base.h"
#include "Logger.h"
#include "Math3D.h"
#include "Mesh.h"
#include "MorphTarget.h"
#include "Skeleton.h"
#include "Joint.h"
#include "FBAPs.h"
#include "FDP.h"
#include "FBAPMapping.h"

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

// C++
#ifdef _DEBUG
#include <assert.h>
#endif
#include <string>

namespace VisageSDK
{

/*
* Animatable character model. This class aggregates model data important
* for character animation. This data includes the following:
*
* <ul>
* <li>Character skeleton (see Skeleton), composed of animatable joints (see Joint).
* <li>A list of all meshes that comprise the model, for each mesh a list of morph targets
* defined on the mesh, and for each morph target its current weight.
* <li>Mappings of MPEG-4 face and body animation parameters (FBAPs) to skeletal joints
* and morph targets.
* <li>Standard MPEG-4 neutral pose (see MPEG-4 FBA Overview for more information).
* </ul>
* 
* VisageCharModel also exposes methods needed for animation:
* 
* <ul>
* <li>loadFromFile. Loads metadata needed for animation from the character's VCM
* (Visage Character Model) file. This metadata includes mappings of MPEG-4 FBAPs
* to skeletal joints and morph targets as well as MPEG-4 neutral pose of the character.
* <li>applyFBAPs. Pre-applies a set MPEG-4 face and body animation
* parameter values to the character. Multiple successive calls to this method
* will blend several FBAP value sets together.
* <li>updateGeometry. Applies the current set of blended FBAP values to the model. I.e.
* this method updates the actual geometry that corresponds to your character.
* </ul>
* 
* In general, you will not call these methods manually. Instead, you will
* have an instance of FAPlayer play animations on your model.
* 
* When integrating visage|SDK with a rendering engine (i.e. when you don't want
* to use the default Horde3D renderer), subclass this class and
* implement all the pure virtual methods. These include, in a nutshell:
*
* <ul>
* <li>Scene wrapper methods (_getModelRoot, _hasNode*, _getNode*, _hasMesh* etc.).
* VisageCharModel uses these methods to retrieve information
* about the model's structure and geometry directly from the scene,
* so their implementation is renderer-specific.
* <li>_updateGeometry. This protected method is used to update the scene geometry
* which corresponds to your character.
* </li>
* </ul>
*
* A detailed tutorial on how to integrate visage|SDK with a renderer of your choice
* can be found in visage|SDK documentation.
*
* @see FAPlayer
* @see Skeleton
* @see Joint
* @see Mesh
* @see MorphTarget
* @see Horde3DCharModel
*/
class VISAGE_DECLSPEC VisageCharModel
{

public:

	/**
	* Constructor.
	*/
	VisageCharModel( const string& id );

	/**
	* Destructor.
	*/
	virtual ~VisageCharModel();

	/**
	* Initializes the character model. Creates the skeleton and meshes
	* by using the scene wrapper functions.
	*/
	virtual bool init();

	/**
	* Gets the identifier of the character model.
	*/
	const string& getId() { return id; }

	/**
	* Gets a pointer to the character's skeleton.
	*/
	Skeleton* getSkeleton() const { return skeleton; }

	/**
	* Creates a new mesh on the model.
	*/
	Mesh* createMesh( const string& meshId );
	
	/**
	* Deletes the mesh at the specified index.
	*/
	void deleteMeshByIndex( int meshIndex );

	/**
	* Deletes the specified mesh.
	*/
	void deleteMeshById( const string& meshId );

	/**
	* Returns true if the specified mesh exists, otherwise false.
	*/
	bool hasMesh( const string& meshId ) const;

	/**
	* Returns a pointer to the specified mesh.
	*/
	Mesh* getMeshById( const string& meshId ) const;

	/**
	* Returns a pointer to the mesh at the specified index.
	*/
	Mesh* getMeshByIndex( int meshIndex ) const;

	/**
	* Returns the number of meshes in the model.
	*/
	int numberOfMeshes() const;

	/**
	* Apply a set of face and body animation parameter values.
	* Deformation of the model won't occur until updateGeometry() is called.
	* Typical use: called by FAPlayer once per every animation frame.
	*
	* @see FAPlayer
	*/
	virtual void applyFBAPs( const FBAPs* fbaps );

	/**
	* Clear the currently applied sets of FBAP values.
	* Typical use: called by FAPlayer.
	*
	* @see FAPlayer
	*/
	virtual void clearFBAPs();

	/**
	* Deforms the model based on current sets of face and body animation
	* parameter values. All current sets are deleted after deformation.
	* Typical use: called by FAPlayer once per every animation frame.
	*
	* @see FAPlayer
	*/
	virtual void updateGeometry();

	/**
	* Loads the character model metadata from a VCM file.
	*
	* @param filename VCM filename.
	* @return true if the VCM file was successfully loaded, otherwise false.
	*/
	virtual bool loadFromFile( const string& filename );

	/**
	* Saves the character model animation metadata to a VCM file. Character model must be valid.
	*
	* @param filename VCM filename.
	* @return true if the VCM file was successfully saved, otherwise false.
	*/
	virtual bool saveToFile( const string& filename ) const;

	/**
	* Gets the feature point mappings.
	*/
	virtual FDP* getFeaturePoints();

	/**
	* Determine if specified mesh is part of the left eye.
	*/
	virtual bool isLeftEyeMesh( const string& id ) const;

	/**
	* Add specified mesh to the list of meshes belonging to the left eye.
	*/
	virtual void addLeftEyeMesh( const string& id );

	/**
	* Remove specified mesh from the list of meshes belonging to the left eye.
	*/
	virtual void removeLeftEyeMesh( const string& id );

	/**
	* Get number of left eye meshes.
	*/
	virtual int numberOfLeftEyeMeshes() const;

	/**
	* Get identifier of the left eye mesh at the specified index.
	*/
	virtual const string& getLeftEyeMesh( int i ) const;

	/**
	* Set identifier of the left eye mesh at the specified index.
	*/
	virtual void setLeftEyeMesh( int i, const string& id );

	/**
	* Get identifier of the node or bone which controls motion of the left eye.
	*/
	virtual const string& getLeftEyeNode() const;
	
	/**
	* Set identifier of the node or bone which controls motion of the left eye.
	*/
	virtual void setLeftEyeNode( const string& id );

	/**
	* Determine if specified mesh is part of the right eye.
	*/
	virtual bool isRightEyeMesh( const string& id ) const;

	/**
	* Add specified mesh to the list of meshes belonging to the right eye.
	*/
	virtual void addRightEyeMesh( const string& id );

	/**
	* Remove specified mesh from the list of meshes belonging to the right eye.
	*/
	virtual void removeRightEyeMesh( const string& id );

	/**
	* Get number of right eye meshes.
	*/
	virtual int numberOfRightEyeMeshes() const;

	/**
	* Get identifier of the right eye mesh at the specified index.
	*/
	virtual const string& getRightEyeMesh( int i ) const;

	/**
	* Set identifier of the right eye mesh at the specified index.
	*/
	virtual void setRightEyeMesh( int i, const string& id );

	/**
	* Get identifier of the node or bone which controls motion of the right eye.
	*/
	virtual const string& getRightEyeNode() const;
	
	/**
	* Set identifier of the node or bone which controls motion of the right eye.
	*/
	virtual void setRightEyeNode( const string& id );

	/**
	* Determine if specified mesh is part of the upper teeth.
	*/
	virtual bool isUpperTeethMesh( const string& id ) const;

	/**
	* Add specified mesh to the list of meshes belonging to the upper teeth.
	*/
	virtual void addUpperTeethMesh( const string& id );

	/**
	* Remove specified mesh from the list of meshes belonging to the upper teeth.
	*/
	virtual void removeUpperTeethMesh( const string& id );

	/**
	* Get number of upper teeth meshes.
	*/
	virtual int numberOfUpperTeethMeshes() const;

	/**
	* Get identifier of the upper teeth mesh at the specified index.
	*/
	virtual const string& getUpperTeethMesh( int i ) const;

	/**
	* Set identifier of the upper teeth mesh at the specified index.
	*/
	virtual void setUpperTeethMesh( int i, const string& id );

	/**
	* Get identifier of the node or bone which controls motion of the upper teeth.
	*/
	virtual const string& getUpperTeethNode() const;
	
	/**
	* Set identifier of the node or bone which controls motion of the upper teeth.
	*/
	virtual void setUpperTeethNode( const string& id );

	/**
	* Determine if specified mesh is part of the lower teeth.
	*/
	virtual bool isLowerTeethMesh( const string& id ) const;

	/**
	* Add specified mesh to the list of meshes belonging to the lower teeth.
	*/
	virtual void addLowerTeethMesh( const string& id );

	/**
	* Remove specified mesh from the list of meshes belonging to the lower teeth.
	*/
	virtual void removeLowerTeethMesh( const string& id );

	/**
	* Get number of lower teeth meshes.
	*/
	virtual int numberOfLowerTeethMeshes() const;

	/**
	* Get identifier of the lower teeth mesh at the specified index.
	*/
	virtual const string& getLowerTeethMesh( int i ) const;

	/**
	* Set identifier of the lower teeth mesh at the specified index.
	*/
	virtual void setLowerTeethMesh( int i, const string& id );

	/**
	* Get identifier of the node or bone which controls motion of the lower teeth.
	*/
	virtual const string& getLowerTeethNode() const;
	
	/**
	* Set identifier of the node or bone which controls motion of the lower teeth.
	*/
	virtual void setLowerTeethNode( const string& id );

	/**
	* Determine if specified mesh is part of the tongue.
	*/
	virtual bool isTongueMesh( const string& id ) const;

	/**
	* Add specified mesh to the list of meshes belonging to the tongue.
	*/
	virtual void addTongueMesh( const string& id );

	/**
	* Remove specified mesh from the list of meshes belonging to the tongue.
	*/
	virtual void removeTongueMesh( const string& id );

	/**
	* Get number of tongue meshes.
	*/
	virtual int numberOfTongueMeshes() const;

	/**
	* Get identifier of the tongue mesh at the specified index.
	*/
	virtual const string& getTongueMesh( int i ) const;

	/**
	* Set identifier of the tongue mesh at the specified index.
	*/
	virtual void setTongueMesh( int i, const string& id );

	/**
	* Get identifier of the node or bone which controls motion of the tongue.
	*/
	virtual const string& getTongueNode() const;
	
	/**
	* Set identifier of the node or bone which controls motion of the tongue.
	*/
	virtual void setTongueNode( const string& id );

	/**
	* Determine if specified mesh is part of the face.
	*/
	virtual bool isFaceMesh( const string& id ) const;

	/**
	* Add specified mesh to the list of meshes belonging to the face.
	*/
	virtual void addFaceMesh( const string& id );

	/**
	* Remove specified mesh from the list of meshes belonging to the face.
	*/
	virtual void removeFaceMesh( const string& id );

	/**
	* Get number of face meshes.
	*/
	virtual int numberOfFaceMeshes() const;

	/**
	* Get identifier of the face mesh at the specified index.
	*/
	virtual const string& getFaceMesh( int i ) const;

	/**
	* Set identifier of the face mesh at the specified index.
	*/
	virtual void setFaceMesh( int i, const string& id );

	/**
	* Get identifier of the node or bone which controls motion of the face.
	*/
	virtual const string& getFaceNode() const;
	
	/**
	* Set identifier of the node or bone which controls motion of the face.
	*/
	virtual void setFaceNode( const string& id );

	/**
	* Get identifier of the skeleton center-of-mass node.
	*/
	virtual const string& getSkeletonCOM() const;

	/**
	* Set identifier of the skeleton center-of-mass node.
	*/
	virtual void setSkeletonCOM( const string& id );

	/**
	* Get a facial animation parameter mapping.
	*
	* @param id FAP id. Valid range is 1 to 68, but values 1 and 2 correspond to
	* visemes and expression and will return an empty mapping.
	*/
	virtual const FBAPMapping& getFAPMapping( int id ) const;

	/**
	* Get a viseme parameter mapping.
	*
	* @param no Viseme number. Valid range is 0 to 14.
	*/
	virtual const FBAPMapping& getVisemeMapping( int no ) const;

	/**
	* Get an expression parameter mapping.
	*
	* @param no Expression number. Valid range is 0 to 6.
	*/
	virtual const FBAPMapping& getExpressionMapping( int no ) const;

	/**
	* Get a body animation parameter mapping.
	*
	* @param id BAP id. Valid range is 1 to 186.
	*/
	virtual const FBAPMapping& getBAPMapping( int id ) const;

	/**
	* Get an animation parameter mapping on this character model.
	*
	* @param name Animation parameter name.
	*/
	virtual const FBAPMapping& getFBAPMapping( const string& name ) const;

	/**
	* Remaps an animation parameter on this character model.
	*/
	virtual void setFBAPMapping( const FBAPMapping& mapping );

	/**
	* Gets the identifier of the character model's root node. It can be used to rotate or
	* translate the entire character model. If the model is a face model,
	* this is the node which controls motion of the entire face. If the model has a skeleton,
	* this node is typically the skeleton's center-of-mass node.
	*/
	virtual string _getModelRoot() const = 0;

	/**
	* Determines if the character model has a node with the specified identifier.
	*/
	virtual bool _hasNode( const string& id ) const = 0;

	/**
	* Gets the array of identifiers of all nodes in the character model.
	*/
	virtual void _getNodes( StringArray& nodes ) const = 0;

	/**
	* Gets the identifier of the specified node's / bone's parent. If the node doesn't have a parent,
	* empty string is returned. If the node is model root, empty string should be returned.
	*/
	virtual string _getNodeParent( const string& id ) const = 0;

	/**
	* Gets an array of identifiers of the specified node's children.
	*/
	virtual void _getNodeChildren( const string& id, StringArray& children ) const = 0;

	/**
	* Gets the initial position of the specified node in the node's local space.
	*/
	virtual Vector3 _getNodeInitialLocalPosition( const string& id ) const = 0;

	/**
	* Gets the initial orientation of the specified node in the node's local space.
	*/
	virtual Quat _getNodeInitialLocalOrientation( const string& id ) const = 0;

	/**
	* Gets the initial scale of the specified node in the node's local space.
	*/
	virtual Vector3 _getNodeInitialLocalScale( const string& id ) const = 0;

	/**
	* Gets the initial position of the specified node in the node's world space.
	*/
	virtual Vector3 _getNodeInitialWorldPosition( const string& id ) const;

	/**
	* Gets the initial orientation of the specified node in the node's world space.
	*/
	virtual Quat _getNodeInitialWorldOrientation( const string& id ) const;

	/**
	* Gets the initial scale of the specified node in the node's world space.
	*/
	virtual Vector3 _getNodeInitialWorldScale( const string& id ) const;

	/**
	* Gets the current position of the specified node in the node's local space.
	*/
	virtual Vector3 _getNodeLocalPosition( const string& id ) const = 0;

	/**
	* Gets the current orientation of the specified node in the node's local space.
	*/
	virtual Quat _getNodeLocalOrientation( const string& id ) const = 0;

	/**
	* Gets the current scale of the specified node in the node's local space.
	*/
	virtual Vector3 _getNodeLocalScale( const string& id ) const = 0;

	/**
	* Gets the current position of the specified node in the node's world space.
	*/
	virtual Vector3 _getNodeWorldPosition( const string& id ) const;

	/**
	* Gets the current orientation of the specified node in the node's world space.
	*/
	virtual Quat _getNodeWorldOrientation( const string& id ) const;

	/**
	* Gets the current scale of the specified node in the node's world space.
	*/
	virtual Vector3 _getNodeWorldScale( const string& id ) const;

	/**
	* Determine if the character model has a mesh with the specified identifier.
	*/
	virtual bool _hasMesh( const string& id ) const = 0;

	/**
	* Get an array of identifiers of all meshes in the character model.
	*/
	virtual void _getMeshes( StringArray& meshes ) const = 0;

	/**
	* Determine if specified mesh has a morph target with the specified identifier.
	*/
	virtual bool _hasMorphTarget( const string& meshId, const string& morphTargetId ) const = 0;

	/**
	* Get list of identifiers of all morph targets on the specified mesh.
	*/
	virtual void _getMorphTargets( const string& meshId, StringArray& morphTargets ) const = 0;

protected:

	/**
	* Updates the geometry of the model by copying joint transformations
	* and morph weights onto the model.
	*/
	virtual void _updateGeometry() = 0;

	string id; ///< Character model identifier.

	Skeleton* skeleton; ///< The character's skeleton.
	vector<Mesh*> meshes; ///< Array of meshes that comprise the character.
	map<string, Mesh*> meshesById; /// Map of meshes that comprise the character, indexed by identifiers.
	
	FBAPs curFBAPs; ///< Set of current FBAP values.

	FDP* fdp; ///< Feature point mappings, vertex lip assignments...
	StringArray leftEyeMeshes; ///< Vector containing identifiers of meshes that make up the left eye.
	string leftEyeNode; ///< Identifier of the scene node or bone which controls the motion of the left eye.
	StringArray rightEyeMeshes; ///< Vector containing identifiers of meshes that make up the right eye.
	string rightEyeNode; ///< Identifier of the scene node or bone which controls the motion of the right eye.
	StringArray upperTeethMeshes; ///< Vector containing identifiers of meshes that make up the upper teeth.
	string upperTeethNode; ///< Identifier of the scene node or bone which controls the motion of the upper teeth.
	StringArray lowerTeethMeshes; ///< Vector containing identifiers of meshes that make up the lower teeth.
	string lowerTeethNode; ///< Identifier of the scene node or bone which controls the motion of the lower teeth.
	StringArray tongueMeshes; ///< Vector containing identifiers of meshes that make up the tongue.
	string tongueNode; ///< Identifier of the scene node or bone which controls the motion of the tongue.
	StringArray faceMeshes; ///< Vector containing identifiers of meshes that make up the face.
	string faceNode; ///< Identifier of the scene node or bone which controls the motion of the face.
	string skeletonCOM; ///< Identifier of the center-of-mass node of the character model's skeleton.

	map<int, FBAPMapping> visemeMappings; ///< Map of viseme mappings, indexed by viseme numbers.
	map<int, FBAPMapping> expressionMappings; ///< Map of expression mappings, indexed by expression numbers.
	map<int, FBAPMapping> fapMappings; ///< Map of low-level FAP mappings, indexed by FAP identifiers.
	map<int, FBAPMapping> bapMappings; ///< Map of BAP mappings, indexed by BAP identifiers.

private:

	void _initSkeleton( const string& jointId );
	void _applyFBAPToGeometry( const FBAPMapping& mapping, float t );

};

}

#endif // __VisageCharModel_h__
