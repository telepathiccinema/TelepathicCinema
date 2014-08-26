
#ifndef __FDP_h__
#define __FDP_h__

// C++
#include <string>

#include "Base.h"

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

class VisageCharModel;

/**
* Feature point.
* This struct represents a feature point definition. If the feature points are defined on a 3D model, each feature point maps to a vertex of the model.
* The following information is specified per each feature point:
* - vertex position (an array of 3 floats)
* - vertex identifier (mesh identifier and vertex index), if the feature point is defined on a 3D model
* - normalization factors (useful for algorithms that normalize the face, such as facial motion cloning)
*
* @see FDP
*
*/
struct VISAGE_DECLSPEC FeaturePoint
{

public:

	/**
	* Position.
	* x, y, z coordinates of the feature point. If the feature point is 2-dimensional (e.g. feature points in an image)
	* the z coordinate is ignored.
	*/
	float pos[3];

	/**
	* Indicator whether the feature point is defined. 0 means it is not defined and its values should not be used. 1 means it is defined.
	*/
	int defined;

	/**
	* Identifier of the polygon mesh (surface) to which the vertex corresponding to the feature point belongs. If
	* the feature points are not defined on a 3D model (e.g. if they represent points in a 2D image), this is not used.
	*/
	string surf;

	/**
	* Identifier of the vertex to which the vertex corresponding to the feature point belongs. This is the
	* vertex index within the polygon mesh (surface) identified by @ref surf. If
	* the feature points are not defined on a 3D model (e.g. if they represent points in a 2D image), this is not used.
	*/
	int vert;
	int animated;
	int normalizing;

	/**
	* Normalization factors (used for algorithms that normalize the face, such as facial motion cloning).
	*/
	float norm[3]; //normalization factors

	FeaturePoint()
	{
		defined = 0;
		surf = "";
		vert = -1;
		animated = normalizing = 0;
		pos[0] = 0.0f;
		pos[1] = 0.0f;
		pos[2] = 0.0f;
		norm[0] = 0.0f;
		norm[1] = 0.0f;
		norm[2] = 0.0f;
	}

	FeaturePoint( const FeaturePoint& fp )
	{
		pos[0] = fp.pos[0];
		pos[1] = fp.pos[1];
		pos[2] = fp.pos[2];
		defined = fp.defined;
		animated = fp.animated;
		normalizing = fp.normalizing;
		surf = fp.surf;
		vert = fp.vert;
		norm[0] = fp.norm[0];
		norm[1] = fp.norm[1];
		norm[2] = fp.norm[2];
	}

	FeaturePoint& operator=( const FeaturePoint& fp )
	{
		if( this == &fp )
			return *this;

		pos[0] = fp.pos[0];
		pos[1] = fp.pos[1];
		pos[2] = fp.pos[2];
		defined = fp.defined;
		animated = fp.animated;
		normalizing = fp.normalizing;
		surf = fp.surf;
		vert = fp.vert;
		norm[0] = fp.norm[0];
		norm[1] = fp.norm[1];
		norm[2] = fp.norm[2];

		return *this;
	}
};

/**
* Feature points of a face.
* This class is a container for facial feature points as defined by <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 FBA standard</a>, as well as
* some additional feature points. Feature points are identified by their group (for example, feature points of
* the nose constitute their own group) and index. So, for example, the tip of the chin belongs to group 2 and has
* index 1, so it is identified as point 2.1. The identification of all MPEG-4 feature points is
* illustrated in the image below:
* \image html "../images/mpeg-4_fba.png"
* \image latex images/mpeg-4_fba.pdf "Facial Feature Points (FP)" width=14cm
* Groups 2 - 11 contain feature points defined according to the
* MPEG-4 FBA standard,
* and group 12 contains the following additional feature points:
* 
* <table>
* <tr>
* <td width="3cm"><b>Name</b></td>
* <td><b>Description</b></td>
* </tr>
* <tr><td>12.1</td><td>Midpoint between FP 3.5 and 3.6</td></tr>
* <tr><td>12.2</td><td>Midpoint between FP 8.7 and 8.2 in outer lower lip contour</td></tr>
* <tr><td>12.3</td><td>Midpoint between FP 8.8 and 8.2 in outer lower lip contour</td></tr>
* <tr><td>12.4</td><td>Chin recess</td></tr>
* <tr><td>12.5</td><td>Midpoint between FP 3.1 and 3.7 of upper inner left eyelid</td></tr>
* <tr><td>12.6</td><td>Midpoint between FP 3.1 and 3.11 of upper inner left eyelid</td></tr>
* <tr><td>12.7</td><td>Midpoint between FP 3.3 and 3.7 of lower inner left eyelid</td></tr>
* <tr><td>12.8</td><td>Midpoint between FP 3.3 and 3.11 of lower inner left eyelid</td></tr>
* <tr><td>12.9</td><td>Midpoint between FP 3.2 and 3.12 of upper inner right eyelid</td></tr>
* <tr><td>12.10</td><td>Midpoint between FP 3.2 and 3.8 of upper inner right eyelid</td></tr>
* <tr><td>12.11</td><td>Midpoint between FP 3.4 and 3.12 of lower inner right eyelid</td></tr>
* <tr><td>12.12</td><td>Midpoint between FP 3.4 and 3.8 of lower inner right eyelid</td></tr>
* <tr><td>12.13</td><td>Lower left cheekbone</td></tr>
* <tr><td>12.14</td><td>Lower right cheekbone</td></tr>
* <tr><td>12.15</td><td>Upper left cheekbone</td></tr>
* <tr><td>12.16</td><td>Upper right cheekbone</td></tr>
* <tr><td>12.17</td><td>Upper extreme of the nose</td></tr>
* <tr><td>12.18</td><td>Nose recess</td></tr>
* <tr><td>12.19</td><td>Left upper nostril border</td></tr>
* <tr><td>12.20</td><td>Right upper nostril border</td></tr>
* <tr><td>12.21</td><td>Back extreme of the left ear</td></tr>
* <tr><td>12.22</td><td>Internal extreme of the left ear (antitragus)</td></tr>
* <tr><td>12.23</td><td>Back extreme of the right ear</td></tr>
* <tr><td>12.24</td><td>Internal extreme of the right ear (antitragus)</td></tr>
* <tr><td>12.25</td><td>Extreme of the left head border close to the ear</td></tr>
* <tr><td>12.26</td><td>Extreme of the right head border close to the ear</td></tr>
* <tr><td>12.27</td><td>Extreme of the left ear between FP 12.20 and 10.1</td></tr>
* <tr><td>12.28</td><td>Extreme of the right ear between FP 12.22 and 10.2</td></tr>
* <tr><td>12.29</td><td>Left point of jaw bone lower but close to FP 2.13</td></tr>
* <tr><td>12.30</td><td>Right point of jaw bone lower but close to FP 2.14</td></tr>
* <tr><td>12.31</td><td>Left corner of the cheekbone close to the left ear</td></tr>
* <tr><td>12.32</td><td>Right corner of the cheekbone close to the right ear</td></tr>
* <tr><td>12.33</td><td>Back extreme of the head</td></tr>
* <tr><td>12.34</td><td>Left upper extreme of the head</td></tr>
* <tr><td>12.35</td><td>Right upper extreme of the head</td></tr>
* <tr><td>12.36</td><td>Internal extreme of the neck</td></tr>
* <tr><td>12.37</td><td>Center of the forehead</td></tr>
* <tr><td>12.38</td><td>Vertigo</td></tr>
* <tr><td>12.39</td><td>Midpoint between FP 12.37 and 11.4</td></tr>
* <tr><td>12.40</td><td>Midpoint between FP 12.37 and 12.32</td></tr>
* <tr><td>12.41</td><td>Midpoint between FP 12.32 and 7.1</td></tr>
* <tr><td>12.42</td><td>Midpoint between FP 11.4 and 12.36</td></tr>
* <tr><td>12.43</td><td>Back corner of the neck left from FP 7.1</td></tr>
* <tr><td>12.44</td><td>Back corner of the neck right from FP 7.1</td></tr>
* <tr><td>12.45</td><td>Left temple</td></tr>
* <tr><td>12.46</td><td>Right temple</td></tr>
* <tr><td>12.47</td><td>Middle point of left forehead border</td></tr>
* <tr><td>12.48</td><td>Middle point of right forehead border</td></tr>
* </table>
* 
* FDP class stores feature point information. It also provides functions for reading and writing the feature point data as files,
* as well as certain auxiliary members that deal with normalisation of feature points and their classification according to
* facial region.
*
* The actual data for each feature point is stored in the structure FeaturePoint. One such structure is allocated for each feature point. To
* access a feature point, use one of the functions getFP() that access a feature point by its group and index expressed either as integer values, or as a string (e.g. "2.1").
* Functions getFPPos() are available as a convenience, to access the feature point coordinates directly, without first accessing the FeaturePoint structure.
*
* The feature points may relate to a particular 3D model. In such a case it is interesting to know, for each feature point, to 
* which vertex it belongs. For this purpose, the FeaturePoint structure contains the mesh identifier
* and vertex index that correspond to the feature point and the FDP class provides functions to access this data.
* 
* <b><i>Left-right convention</i></b>
*  
* References to left and right in feature point definitions are given from the perspective of the face itself so "right eye" 
* can be imagined as "my right eye". When refering to feature points in an image, it is assumed that image is taken by camera and 
* not mirrored so "right eye" is on the left in the image, as shown in feature points illustration above.
*
*
* @see FeaturePoint
* \if IOS_DOXY
* \elseif ANDROID_DOXY
* \else @see AFM \endif
*
*/
class VISAGE_DECLSPEC FDP
{
public:
	
	/**
	* Constructor.
	* Creates an empty FDP object.
	*/
	FDP();

	/*
	* Constructor.
	*/
	FDP( VisageCharModel* charModel );

	/**
	* Copy constructor.
	* Makes a copy of FDP object.
	*/
	FDP( const FDP& featurePoints );
	
	/**
	* Constructor.
	* @param fn FDP file.
	* @param charModel Visage Character Model.
	*/
	FDP( const char *fn,  VisageCharModel* charModel = NULL );

	/*
	* Destructor.
	*/
	~FDP();

	/*
	* Assignment operator.
	*/
	FDP& operator=( const FDP& featurePoints );

	/**
	* Read from an FDP file.
	* The FDP file format consists of one line of text for each feature point, in the following format:
	* <group>.<index> <x> <y> <z> <mesh_index>.<vertex_index>
	* @param name Filename.
	* @return 0 - failure, 1 - success, -1 - old version, needs reindexing.
	*/
	int readFromFile(const char *name);

	/**
	* Create a new (empty) FDP file.
	* @param name Filename.
	*/
	void createNewFile(const char *name);
	
	/**
	* Write feature point definitions to the current FDP file.
	* File name is the one currently used.
	*/
	void saveToFile() const;

	/**
	* Write feature point definitions to the FDP file, changing the FDP file name.
	* @param fileName Filename.
	*/
	void saveToFile(char *fileName);

	/*
	* Gets the character model this FDP is associated with.
	*/
	VisageCharModel* getCharModel() const { return charModel; }

	/*
	* Sets the character model this FDP is associated with.
	*/
	void setCharModel( VisageCharModel* charModel ) { this->charModel = charModel; }

	/**
	* Get a feature point by its group and index.
	* @param group Feature point group. Valid range is from 2 to 12.
	* @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
	* @return Feature point.
	*/
	const FeaturePoint& getFP(int group, int n) const;
	
	/**
	* Get a feature point by its name.
	* @param name Feature point name (e.g. "7.1").
	* @return Feature point.
	*/
	const FeaturePoint& getFP(const char *name) const;

	/**
	* Set a feature specified by its group and index.
	* @param group Feature point group. Valid range is from 2 to 12.
    * @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().

	* @param f The feature point to set.
	*/
	void setFP( int group, int n, const FeaturePoint& f );

	/**
	* Set a feature specified by its name.
	* @param name Feature point name (e.g. "7.1").
	* @param f The feature point to set.
	*/
	void setFP( const char* name, const FeaturePoint& f );
	
	/**
	* Get the position of a feature point specified by its group and index.
	* @param group Feature point group. Valid range is from 2 to 12.
    * @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().

	* @return Vertex position (array of 3 floating point numbers).
	*/
	const float* getFPPos(int group, int n) const;
	
	/**
	* Get the position of a feature point specified by its name.
	* @param name Feature point name (e.g. "7.1").
	* @return Vertex position (array of 3 floating point numbers).
	*/
	const float* getFPPos(const char *name) const;

	/**
	* Set the position of a feature point specified by its group and index.
	* @param group Feature point group. Valid range is from 2 to 12.
	* @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
	* @param pos Vertex position (array of 3 floating point values).
	*/
	void setFPPos(int group, int n, const float *pos);
	
	/**
	* Set the position of a feature point specified by its name.
	* @param name Feature point name (e.g. "7.1").
	* @param pos Vertex position (array of 3 floating point values).
	*/
	void setFPPos(const char *name, const float *pos);
	
	/**
	* Set the position of a feature point specified by its group and index.
	* @param group Feature point group. Valid range is from 2 to 12.
	* @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
	* @param x Vertex x position.
	* @param y Vertex y position.
	* @param z Vertex z position.
	*/
	void setFPPos(int group, int n, float x, float y, float z);
	
	/**
	* Set the position of a feature point specified by its name.
	* @param name Feature point name (e.g. "7.1").
	* @param x Vertex x position.
	* @param y Vertex y position.
	* @param z Vertex z position.
	*/
	void setFPPos(const char *name, float x, float y, float z);

	/**
	* Get the mesh and vertex mapping for a feature point specified by its group and index.
	* @param group Feature point group. Valid range is from 2 to 12.
	* @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
	* @param surf Returned mesh identifier.
	* @param vert Returned vertex index.
	* @return True if feature point is defined, false if not.
	*/
	bool getFPSurfVert( int group, int n, string& surf, int& vert ) const;

	/**
	* Get the mesh and vertex mapping for a feature point specified by its name.
	* @param name Feature point name (e.g. "7.1").
	* @param surf Returned mesh identifier.
	* @param vert Returned vertex index.
	* @return True if feature point is defined, false if not.
	*/
	bool getFPSurfVert( const char* name, string& surf, int& vert ) const;

	/**
	* Assign a feature point, specified by its group and index, to a specific vertex.
	* @param group Feature point group. Valid range is from 2 to 12.
	* @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
	* @param surf Mesh id.
	* @param vert Vertex index.
	*/
	void setFPSurfVert(int group, int n, const string& surf, int vert);

	/**
	* Assign a feature point, specified by its name, to a specific vertex.
	* @param name Feature point name (e.g. "7.1").
	* @param surf Mesh id.
	* @param vert Vertex index.
	*/
	void setFPSurfVert(const char *name, const string& surf, int vert);
	
	/**
	* Resets all feature points.
	*
	* The value of all feature points is set to "undefined".
	*/
	void reset();

	/**
	* Returns true if the feature point is defined.
    *
    * @param group Feature point group. Valid range is from 2 to 12.
    * @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
     
    * @return True if the feature point is defined, false otherwise.
	*/
	bool FPIsDefined( int group, int n ) const;

	/**
	* Returns true if the feature point is defined.
    *
	* @param name Feature point name (e.g. "7.1").
     
    * @return True if the feature point is defined, false otherwise.
    */
	bool FPIsDefined( const string& name ) const;

	/*
	* Returns true if the face is normalized.
	*/
	bool isNormalized() const;

	/*
	* Returns true if the face is initialized.
	*/
	bool isInitialized() const;

	/*
	* Assigns a vertex to the upper lip region.
	*/
	void addUpperLipVertex( const FeaturePoint& vertex );

	/*
	* Gets a vertex assigned to the upper lip region.
	*/
	const FeaturePoint& getUpperLipVertex( int vertexIndex ) const;

	/*
	* Removes a vertex from the upper lip region.
	*/
	void removeUpperLipVertex( int vertexIndex );

	/*
	* Removes all vertices from the upper lip region.
	*/
	void clearUpperLipVertices();

	/*
	* Gets the number of vertices assigned to the upper lip region.
	*/
	int numberOfUpperLipVertices() const;

	/*
	* Assigns a vertex to the lower lip region.
	*/
	void addLowerLipVertex( const FeaturePoint& vertex );

	/*
	* Gets a vertex assigned to the lower lip region.
	*/
	const FeaturePoint& getLowerLipVertex( int vertexIndex ) const;

	/*
	* Removes a vertex from the lower lip region.
	*/
	void removeLowerLipVertex( int vertexIndex );

	/*
	* Removes all vertices from the lower lip region.
	*/
	void clearLowerLipVertices();

	/*
	* Gets the number of vertices assigned to the lower lip region.
	*/
	int numberOfLowerLipVertices() const;
	
	/**
	* Set normalization factors for all defined feature points (see FeaturePoint).
	*/
	void setNormFactors();

	/**
	* Print feature point definitions to console.
	* @deprecated May be removed in the next release.
	*/
	void print() const;

private:

	VisageCharModel* charModel;

	/*
	* FDP file name.
	*/
	char mFilename[200];
public:

	/*
	* Sizes of the twelve feature point groups.
	* @deprecated Will be private in the next release. Use getFPGroupSize instead.
	*/
	int grSize[12];

	/*
	* Some characteristic distances between feature points (defined in <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 FBA standard</a>).
	* @deprecated Will be private in the next release. Use appropriate get functions instead.
	*/
	float MNS0,ENS0,ES0,MW0,IRISD0;

	/*
	* true if the face is normalized, false otherwise.
	* @deprecated Will be private in the next release. Use isNormalized instead.
	*/
	bool normalized;
	
	/*
	* Feature points.
	* @deprecated Will be private in the next release. Use getFP* functions instead.
	*/
	FeaturePoint *fp[13];
	
	/*
	* Vertices that are part of the upper lip region.
	*/
	vector<FeaturePoint>* ul;

	/*
	* Vertices that are part of the lower lip region.
	*/
	vector<FeaturePoint>* ll;

	/*
	* true if FDP is initialized, false otherwise.
	* @deprecated Will be private in the next release. Use isInitialized instead.
	*/
	bool initialized;


	/**
	* Get feature point group and index from its name.
    *
	* @param name Feature point name (e.g. "7.1").
    * @param group Returned feature point group.
    * @param n Returned feature point index.
	*/
	static void parseFPName( const string& name, int& group, int& n );

	/**
	* Get feature point name from group and index.
    *
    * @param group Feature point group. Valid range is from 2 to 12.
    * @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
    *
    * @return Feature point name.
	*/
	static string getFPName( int group, int n );

	/**
	* Returns true if specified feature point identifier is valid.
    *
    * @param group Feature point group. Valid range is from 2 to 12.
    * @param n Feature point index. Valid range is from 1 to the size of particular group. Group sizes can be obtained using groupSize().
    *
    * @return True if specified feature point identifier is valid, false otherwise.
	*/
	static bool FPIsValid( int group, int n );

	/**
	* Returns true if specified feature point identifier is valid.
    *
    * @param name Feature point name (e.g. "7.1").
    *
    * @return True if specified feature point identifier is valid, false otherwise.
	*/
	static bool FPIsValid( const string& name );

	/// Index of the first feature point group.
	static const int FP_START_GROUP_INDEX = 2;

	/// Index of the last feature point group.
	static const int FP_END_GROUP_INDEX = 12;

	/// Number of groups.
	static const int FP_NUMBER_OF_GROUPS = 11;

private:

	/**
	* Sizes of feature point groups.
	*/
	static const int groupSizes[FP_NUMBER_OF_GROUPS];

public:

	/**
	* Get the size of the specified feature point group.
    * Valid range for group is from 2 to 12.
    *
    * @param group Feature point group. Valid range is from 2 to 12.
    *
    * @return Size of the specified feature point group.
	*/
	static int groupSize( int group );
};

}

#endif // __FDP_h__
