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


#ifndef __Singleton_h__
#define __Singleton_h__

#include "Base.h"

// C++
#include <string>

namespace VisageSDK
{

/**
* Template for a singleton class.
*/
template <class T> class Singleton
{

protected:

	/// The singleton instance.
	static T* inst;

	/// Constructor.
	Singleton(){}

	/// Destructor.
	virtual ~Singleton()
	{
		delete inst;
	}

public:

	/// Get the singleton instance.
	static T* getInstance()
	{
		if( inst == NULL )
			inst = new T();

		return inst;
	}

};

template <class T> T* Singleton<T>::inst = NULL;

}

#endif // __Singleton_h__
