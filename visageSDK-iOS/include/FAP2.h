
#ifndef __FAP2_h__
#define __FAP2_h__

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
* MPEG-4 FAP2 (expression).
* The storage class for the MPEG-4 expression (FAP2). It contains
* the full definition of FAP2 according to MPEG-4 specification.
* A summary of the MPEG-4 FAP specification can be found in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. 
* 
* This class also provides functions for merging expressions.
*
* The value of the expressions can be set directly by accessing the public attributes of this class, or through convenience function
* FBAPs::setExpression(). 
*
* Implemented in visagefbadecoder.lib
*/
class VISAGE_DECLSPEC FAP2  
{
public:
	/**
	* Constructor.
	* Constructs a FAP2 with a neutral facial expression.
	*/
	FAP2();

	/**
	* Constructor.
	* Constructs a FAP2 by copying a given FAP2.
	* @param f the FAP2 to be copied.
	*/
	FAP2(FAP2 *f);

	/**
	* Constructor.
	* Constructs a FAP2 by copying a given FAP2, then multiplying it by the factor in.
	* The factor multiplies the intensities. 
	* @param f the FAP2 to be copied.
	* @param in the multiplication factor.
	*/
	FAP2(FAP2 *f, double in);

	/**
	* Set the FAP2.
	* Set the FAP2 by copying a given FAP2, then multiplying it by the factor in.
	* The factor multiplies the intensities. 
	* @param f the FAP2 to be copied.
	* @param in the multiplication factor.
	*/
	void set(FAP2 *f, double in);

	/**
	* Set the FAP2.
	* Set the FAP2 by copying a given FAP2 
	* @param f the FAP2 to be copied.
	*/
	void set(const FAP2 *f);

	/**
	* Destructor.
	*/
	virtual ~FAP2();


	/**
	* Merge with another FAP2.
	* Merge FAP2 with the given FAP2.
	* The FAP2 are merged as follows:
	* if this FAP2 has an expression
	* already set to non-zero value, its value is kept and merged FAP2 have no influence on it. If this FAP2 has no
	* active expression, the values from the merged FAP2 is taken.
	*
	* @param f the FAP2 to be merged with this FAP2.
	*/
	void merge(FAP2 *f);

	/**
	* Test if the FAP2 is void.
	* Test whether the FAP2 is void, i.e. if there is no expression (both expression1 and expression2 are 0.
	* @return 1 if it is void (no expression), 0 otherwise.
	*/
	int isVoid();

	/**
	* Test if this FAP2 is equal to another FAP2.
	* Test whether the this FAP1 is equal to another FAP2.
	* @param f the FAP2 to be compared with this FAP2.
	* @return true if equal, false otherwise.
	*/
	bool isEqual(FAP2 *f);

	/**
	* Expression 1.
	* Expression 1, according to MPEG-4 FBA specification
	*/
	int expression1;

	/**
	* Expression 2.
	* Expression 2, according to MPEG-4 FBA specification
	*/
	int expression2;

	/**
	* Intensity 1.
	* Intensity 1, according to MPEG-4 FBA specification
	*/
	int intensity1;

	/**
	* Intensity 2.
	* Intensity 2, according to MPEG-4 FBA specification
	*/
	int intensity2;

	/**
	* init_bit.
	* init_bit, according to MPEG-4 FBA specification
	*/
	int initBit;

	/**
	* def_bit.
	* def_bit, according to MPEG-4 FBA specification
	*/
	int defBit;

	/**
	* Reset.
	* Reset the neutral position, i.e. all parameters are set to 0.
	*/
	void reset();
};

}

#endif // __FAP2_h__
