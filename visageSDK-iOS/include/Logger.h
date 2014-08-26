
#ifndef __Logger_h__
#define __Logger_h__

#include "Base.h"
#include "Singleton.h"
#include "Timer.h"

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
#include <fstream>
#include <sstream>

using namespace std;

namespace VisageSDK
{

#ifdef _DEBUG
	#define V_LOG ( Logger::getInstance()->write )
#else
	#define V_LOG
#endif

/**
* Logger of events and errors.
*/
class VISAGE_DECLSPEC Logger : public Singleton<Logger>
{

	friend class Singleton<Logger>;

private:

	/**
	* Constructor.
	*/
	Logger();

	/**
	* Destructor.
	*/
	~Logger();

public:

	/**
	* Writes an entry into the log.
	*/
	void write( const char* msg, ... );

private:

	FILE* logfile;
	Timer tmr;

};

}

#endif
