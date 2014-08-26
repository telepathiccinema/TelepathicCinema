
#ifndef __LLFAPs_h__
#define __LLFAPs_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "FBACodecCommon.h"

namespace VisageSDK
{

/**
* Low level Facial Animation Parameters.
* The storage class for the MPEG-4 Facial low level Animation Parameters (FAPs). It contains
* a full set of low level FAPs according to MPEG-4 specification.
* A summary of the MPEG-4 FAP specification can be found in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. 
* 
* The Low Level FAPs can be set directly by accessing the value array, or by using the convenience function FBAPs::setFAP().
*
* This class also provides functions for merging sets of low level FAPs.
*
* Implemented in visagefbadecoder.lib
*/
class VISAGE_DECLSPEC LLFAPs
{
public:
	/**
	* Constructor.
	* Constructs LLFAPs with a neutral facial expression.
	*/
	LLFAPs();

	/**
	* Constructor.
	* Constructs LLFAPs by copying given LLFAPs.
	* @param f the LLFAPs to be copied.
	*/
	LLFAPs(LLFAPs *f);

	/**
	* Constructor.
	* Constructs LLFAPs by copying given LLFAPs, then multiplying them by the factor in.
	* The factor multiplies the intensity. 
	* @param f the LLFAPs to be copied.
	* @param in the multiplication factor.
	*/
	LLFAPs(LLFAPs *f, double in);

	/**
	* Set the LLFAPs.
	* Set LLFAPs by copying the given LLFAPs, then multiplying them by the factor in.
	* The factor multiplies the intensity. 
	* @param f the LLFAPs to be copied.
	* @param in the multiplication factor.
	*/
	void set(LLFAPs *f, double in);

	/**
	* Set the LLFAPs.
	* Set LLFAPs by copying the given LLFAPs.
	* @param f the LLFAPs to be copied.
	*/
	void set(const LLFAPs *f);
	void filter(LLFAPs *f);

	/**
	* Destructor.
	*/
	virtual ~LLFAPs();

	/**
	* Merge FAPs.
	* Merge FAPs with the given FAPs.
	* The FAPs are merged as follows:
	*
	* The value of each FAP is averaged between the two FAPs if both are
	* non-zero. If either is zero, the other is used without averaging. This is done to allow merging
	* of independent FAPs. For example, if one FAPs has non-zero values for the mouth, and the other
	* for the eyes, the result should have the mouth values from one, and eye values from another,
	* without changing the values. Simple averaging would give halved values.
	*
	* In practice, merging gives best results for FAPs that are naturally independent, like
	* a mouth movement for speech, a FAPs containing eyeblinks and a third FAPs containing
	* some head motion. Such FAPs will merge naturally. Merging of FAPs that act on the same FAPs
	* may give unpredictable results.
	* @param f the LLFAPs to be merged with this FAPs.
	*/
	void merge(LLFAPs *f);

	/**
	* FAP values. The value of each MPEG-4 FAP.
	*
	* The indices in this array correspond to the list of FAPs in MPEG-4 documentation 
	* (see <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>),
	* but the
	* numbers are shifted by one because documentation starts with 1, and array starts with 0.
	* So, value[2] corresponds to FAP number 3 in 
	* the documentation (open_jaw).
	*
	* For convenience, the constants defined in @ref FAPNames can be used as indices to this array.
	*
	* @see FAPNames
	*/
	int value[NFAP];


	/**
	* Reset.
	* Reset the neutral position, i.e. all parameters are set to 0.
	*/
	void reset();
};

}

#endif // __LLFAPs_h__
