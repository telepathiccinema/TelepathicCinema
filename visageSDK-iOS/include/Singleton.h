
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
