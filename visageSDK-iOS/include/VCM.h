
#ifndef __VCM_h__
#define __VCM_h__

#define VCM_VERSION 1.0f
#define VCM_VERSION_STR "1.0"

#include "Base.h"
#include "FDP.h"
#include "FBAPMapping.h"

// forward declarations
struct _xmlTextReader;
typedef _xmlTextReader xmlTextReader;
typedef xmlTextReader* xmlTextReaderPtr;
struct _xmlTextWriter;
typedef _xmlTextWriter xmlTextWriter;
typedef xmlTextWriter* xmlTextWriterPtr;

namespace VisageSDK
{

// forward declarations
class VisageCharModel;

using namespace std;

/**
* Virtual character metadata file input/output.
*
* Metadata consists of the following information:
* - feature point definitions
* - vertex lip assignments
* - data on individual head parts
* - animation parameter mappings
* - neutral pose transformations
* 
* loadFromFile method loads contents of a VCM file and fills the VCM instance with metadata.
* This metadata is stored in public data fields.
*
* These data fields can also be filled with metadata. saveToFile method should be called
* to save the metadata to a VCM file.
*
* @see FBAPMapping
*/
class VCM
{

public:

	string charId; ///< Character model identifier.
	FDP featurePoints; ///< Feature point and upper/lower lip vertex mappings.
	StringArray leftEyeMeshes; ///< Vector containing identifiers of meshes that make up the left eye.
	string leftEyeNode; ///< Identifier of the scene node or bone which controls the motion of the left eye.
	///< Empty string means there is no controlling node.
	StringArray rightEyeMeshes; ///< Vector containing identifiers of meshes that make up the right eye.
	string rightEyeNode; ///< Identifier of the scene node or bone which controls the motion of the right eye.
	///< Empty string means there is no controlling node.
	StringArray upperTeethMeshes; ///< Vector containing identifiers of meshes that make up the upper teeth.
	string upperTeethNode; ///< Identifier of the scene node or bone which controls the motion of the upper teeth.
	///< Empty string means there is no controlling node.
	StringArray lowerTeethMeshes; ///< Vector containing identifiers of meshes that make up the lower teeth.
	string lowerTeethNode; ///< Identifier of the scene node or bone which controls the motion of the lower teeth.
	///< Empty string means there is no controlling node.
	StringArray tongueMeshes; ///< Vector containing identifiers of meshes that make up the tongue.
	string tongueNode; ///< Identifier of the scene node or bone which controls the motion of the tongue.
	///< Empty string means there is no controlling node.
	StringArray faceMeshes; ///< Vector containing identifiers of meshes that make up the face.
	string faceNode; ///< Identifier of the scene node or bone which controls the motion of the face.
	///< Empty string means there is no controlling node.
	string skeletonCOM; ///< Identifier of the center-of-mass node of the character model's skeleton.
	FBAPMappingsMap mappings; ///< Map containing animation parameter mappings.
	NodeTransMap neutralPosePositions; ///< Map of neutral pose position values, indexed by bone identifiers.
	NodeRotMap neutralPoseOrientations; ///< Map of neutral pose orientation values, indexed by bone identifiers.

	/// Constructor.
	VCM( VisageCharModel* charModel ) : featurePoints(charModel)
	{
		errorDesc = "";
	}

	/**
	* Load virtual character metadata from a file.
	*
	* @param filename Path and name of the VCM file.
	* @return true if metadata was successfully loaded, otherwise false.
	*/
	bool loadFromFile( const string& filename );

	/**
	* Save virtual character metadata to a file.
	*
	* @param filename Path and name of the VCM file.
	* @return true if metadata was successfully saved, otherwise false.
	*/
	bool saveToFile( const string& filename );

private:

	/// VCM parser functions:

	bool parseDocument( xmlTextReaderPtr vcmReader );
	bool parseNodeVcm( xmlTextReaderPtr vcmReader );
	bool parseNodeFeaturePoints( xmlTextReaderPtr vcmReader );
	bool parseNodeFeaturePoint( xmlTextReaderPtr vcmReader );
	bool parseNodeVertexLipAssignments( xmlTextReaderPtr vcmReader );
	bool parseNodeUpperLipVertices( xmlTextReaderPtr vcmReader );
	bool parseNodeLowerLipVertices( xmlTextReaderPtr vcmReader );
	bool parseNodeBodyParts( xmlTextReaderPtr vcmReader );
	bool parseNodeBodyPart( xmlTextReaderPtr vcmReader, const string& bodyPartId );
	bool parseNodeBodyPartMeshes( xmlTextReaderPtr vcmReader, StringArray& meshList );
	bool parseNodeBodyPartMesh( xmlTextReaderPtr vcmReader, string& meshId );
	bool parseNodeBodyPartNode( xmlTextReaderPtr vcmReader, string& nodeId );
	bool parseNodeSkeleton( xmlTextReaderPtr vcmReader );
	bool parseNodeCOM( xmlTextReaderPtr vcmReader );
	bool parseNodeAnimationParameterMappings( xmlTextReaderPtr vcmReader );
	bool parseNodeAnimationParameterMapping( xmlTextReaderPtr vcmReader );
	bool parseNodeMorphTarget( xmlTextReaderPtr vcmReader, string& meshId, string& mtId );
	bool parseNodeBoneTrans( xmlTextReaderPtr vcmReader, string& boneId, Vector3& refPosition );
	bool parseNodeBoneRot( xmlTextReaderPtr vcmReader, string& boneId, Quat& refOrientation );
	bool parseNodeNeutralPose( xmlTextReaderPtr vcmReader );
	bool parseNodeBoneTransformation( xmlTextReaderPtr vcmReader );
	bool parseInt( const char* valueStr, int& value );
	bool parseIntVector3Array( const char* valueStr, IntArray& value1, Vector3Array& value2 );
	bool parseVector3( const char* valueStr, Vector3& value );
	bool parseQuat( const char* valueStr, Quat& value );

	bool getCurNodeName( xmlTextReaderPtr vcmeader, string& nodeName ) const;
	bool getCurElementValue( xmlTextReaderPtr vcmReader, string& nodeValue ) const;
	bool getCurAttributeValue( xmlTextReaderPtr vcmReader, const string& attribName, string& nodeValue ) const;

	bool meshListHasMesh( const StringArray& meshList, const string& meshId )
	{
		for( StringArray::const_iterator i = meshList.begin(); i != meshList.end(); ++i )
			if( *i == meshId )
				return true;

		return false;
	}

	/// VCM writer functions:

	bool writeDocument( xmlTextWriterPtr vcmWriter );
	bool writeNodeVcm( xmlTextWriterPtr vcmWriter );
	bool writeNodeFeaturePoints( xmlTextWriterPtr vcmWriter );
	bool writeNodeFeaturePoint( xmlTextWriterPtr vcmWriter, const string& featurePointId );
	bool writeNodeVertexLipAssignments( xmlTextWriterPtr vcmWriter );
	bool writeNodeUpperLipVertices( xmlTextWriterPtr vcmWriter, const string& meshId,
		const IntArray& vertexIndices, const Vector3Array& vertexPositions );
	bool writeNodeLowerLipVertices( xmlTextWriterPtr vcmWriter, const string& meshId,
		const IntArray& vertexIndices, const Vector3Array& vertexPositions );
	bool writeNodeBodyParts( xmlTextWriterPtr vcmWriter );
	bool writeNodeBodyPart( xmlTextWriterPtr vcmWriter, const string& bodyPartId,
		const StringArray& meshList, const string& nodeId );
	bool writeNodeBodyPartMeshes( xmlTextWriterPtr vcmWriter, const StringArray& meshList );
	bool writeNodeBodyPartMesh( xmlTextWriterPtr vcmWriter, const string& meshId );
	bool writeNodeBodyPartNode( xmlTextWriterPtr vcmWriter, const string& nodeId );
	bool writeNodeSkeleton( xmlTextWriterPtr vcmWriter );
	bool writeNodeCOM( xmlTextWriterPtr vcmWriter );
	bool writeNodeAnimationParameterMappings( xmlTextWriterPtr vcmWriter );
	bool writeNodeAnimationParameterMapping( xmlTextWriterPtr vcmWriter,
											 const FBAPMapping& mapping );
	bool writeNodeMorphTarget( xmlTextWriterPtr vcmWriter, const string& meshId, const string& mtId );
	bool writeNodeBoneTrans( xmlTextWriterPtr vcmWriter, const string& boneId,
		const Vector3& refPosition );
	bool writeNodeBoneRot( xmlTextWriterPtr vcmWriter, const string& boneId,
		const Quat& refOrientation );
	bool writeNodeNeutralPose( xmlTextWriterPtr vcmWriter );
	bool writeNodeBoneTransformation( xmlTextWriterPtr vcmWriter, const string& boneId );
	bool writeInt( int value, string& valueStr );
	bool writeIntVector3Array( const IntArray& value1, const Vector3Array& value2, string& valueStr );
	bool writeVector3( const Vector3& value, string& valueStr );
	bool writeQuat( const Quat& value, string& valueStr );

	/// Error handling:

	// TODO:
	// Add rudimentary error handling. Simple error codes or logging will do.
	//

	/// Error description.
	string errorDesc;
	void logFileReadError( const string& filename ) const;
	void logFileWriteError( const string& filename ) const;
	void logVCMParseError( int lineNo, const string& nodeName ) const;
	void logVCMSetIndentError() const;
	void logVCMWriteError( const string& nodeName ) const;

public:

	/// Get string description of the error.
	string getErrorDesc() const
	{
		return errorDesc;
	}

};

}

#endif // __VCM_h__
