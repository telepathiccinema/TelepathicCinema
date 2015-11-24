///////////////////////////////////////////////////////////////////////////////
// 
// (c) Visage Technologies AB 2002 - 2015  All rights reserved. 
// 
// This file is part of visage|SDK(tm). 
// Unauthorized copying of this file, via any medium is strictly prohibited. 
// 
// No warranty, explicit or implicit, provided. 
// 
// This is proprietary software. No part of this software may be used or 
// reproduced in any form or by any means otherwise than in accordance with
// any written license granted by Visage Technologies AB. 
// 
/////////////////////////////////////////////////////////////////////////////


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
