
#ifndef __Fba_in_buffer_h__
#define __Fba_in_buffer_h__

// C++
#include <stdio.h>

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

#define fbaBufSize 1000

class VISAGE_DECLSPEC Fba_in_buffer  
{
public:
	Fba_in_buffer();
	Fba_in_buffer(FILE *inputstream); //Fba_in_buffer(int inputstream);
	virtual ~Fba_in_buffer();
	void next_start_code() ;
	int nextbits_bytealigned();
	int getbits(int n);
	void init(FILE *inputstream); //void init(int inputstream);
	void adjust_bits();
	void resetbits();
	int cntbits();


private:
	int getbit();
	void initbits();

	FILE *inStream;//int inStream;
	unsigned char buf[fbaBufSize];
	unsigned char inbfr;
	int p;
	int inp;
	int incnt;
	int bitcnt;
};

} // __Fba_in_buffer_h__

#endif // __Fba_in_buffer_h__
