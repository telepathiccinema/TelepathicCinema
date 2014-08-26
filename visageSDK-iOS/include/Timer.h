
#ifndef __Timer_h__
#define __Timer_h__

#include "Base.h"

using namespace std;

namespace VisageSDK
{

class Timer
{

public:

	struct Time
	{
		unsigned long hr;
		unsigned long min;
		unsigned long s;
		unsigned long ms;

		Time()
		{
			hr = min = s = ms = 0;
		}
	};

	Timer(); ///< Constructor.
	void start(); ///< Start the timer.
	void stop(); ///< Stop the timer.
	void reset(); ///< Reset the timer.
	bool isRunning() const; ///< Returns true if the timer is running, otherwise false.
	Timer::Time getTime() const; ///< Get elapsed time.
	string getTimeStr() const; ///< Get elapsed time as a string.

private:

	unsigned long getTickCount() const; // get number of ms since system was started

	unsigned long ticks0;
	bool stopped;

};

}

#endif // __Timer_h__
