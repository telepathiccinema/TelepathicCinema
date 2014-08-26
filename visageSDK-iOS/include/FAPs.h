
#ifndef __FAPs_h__
#define __FAPs_h__

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
#include "LLFAPs.h"
#include "FAP1.h"
#include "FAP2.h"

namespace VisageSDK
{

/**
* Facial Animation Parameters.
* The storage class for the MPEG-4 Facial Animation Parameters (FAPs). It contains
* a full set of FAPs: visemes, expressions and low level FAPs according to MPEG-4 specification.
* A summary of the MPEG-4 FAP specification can be found in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. 
* 
* The attributes can be accessed directly to set the FAP values, or convenience functions FBAPs::setViseme, FBAPs::setExpression and FBAPs::setFAP can be used. 
* This class also provides functions for merging sets of FAPs and interpolating between left and
* right FAPs.
*
* Implemented in visagefbadecoder.lib
*/
class VISAGE_DECLSPEC FAPs  
{
public:

	/**
	* Reset.
	* Reset the neutral position, i.e. all parameters are set to 0.
	*/
	void reset();
	/**
	* Constructor.
	* Constructs FAPs with a neutral facial expression.
	*/
	FAPs();

	/**
	* Constructor.
	* Constructs FAPs by copying given FAPs.
	* @param faps the faps to be copied.
	*/
	FAPs(const FAPs *faps);

	/**
	* Constructor.
	* Constructs FAPs by copying given FAPs, then multiplying them by the factor in.
	* For the low level faps and for the expressions, the factor multiplies the intensity. 
	* The visemes are just copied, the multiplication factor is ignored for them.
	* @param faps the faps to be copied.
	* @param in the multiplication factor.
	*/
	FAPs(FAPs *faps, double in);

	/**
	* Destructor.
	*/
	virtual ~FAPs();

	/**
	* Set the FAPs.
	* Set this FAPs by copying the given FAPs.
	* @param faps the faps to be copied.
	*/
	void set(const FAPs *faps);
	void filter(FAPs *filter);

	/**
	* Set the FAPs.
	* Set this FAPs by copying the given FAPs, then multiplying them by the factor in.
	* For the low level faps and for the expressions, the factor multiplies the intensity. 
	* The visemes are just copied, the multiplication factor is ignored for them.
	* @param faps the faps to be copied.
	* @param in the multiplication factor.
	*/
	void set(FAPs *faps, double in);

	/**
	* Merge FAPs.
	* Merge FAPs with the given FAPs.
	* The FAPs are merged as follows:
	* - For the high level FAPs (visemes and expressions), if this FAPs has a viseme/expression
	* already set to non-zero value, its value is kept and merged FAPs have no influence on it. If this FAPs has no
	* viseme/expression, the values from the merged FAPs is taken.
	* - For the low-level FAPs, the value of each FAP is averaged between the two FAPs if both are
	* non-zero. If either is zero, the other is used without averaging. This is done to allow merging
	* of independent FAPs. For example, if one FAPs has non-zero values for the mouth, and the other
	* for the eyes, the result should have the mouth values from one, and eye values from another,
	* without changing the values. Simple averaging would give halved values.
	*
	* In practice, merging gives best results for FAPs that are naturally independent, like
	* a mouth movement for speech, a FAPs containing eyeblinks and a third FAPs containing
	* some head motion. Such FAPs will merge naturally. Merging of FAPs that act on the same FAPs
	* may give unpredictable results.
	* @param faps the faps to be merged with this FAPs.
	*/
	void merge(FAPs *faps);

	/**
	* Interpolate left/right FAPs.
	* If low-level FAPs for one side of the face are 0, they are interpolated by copying
	* their symetrical counterparts on the other side of the face.
	*/
	void interpolateLR();
	void interpolateLeftRightFAPs(int l, int r);

	/**
	* Set the mask.
	* Set the mask values (groupMask) for all animation parameters to 1 if maskValue is true, or to 0 if maskValue is false.
	* maskType for all groups is set to 1.
	*
	* @param maskValue the value for the mask to be set - 0 if false, 1 if true.
	*
	* @see FAPs::maskType
	* @see FAPs::groupMask
	*/
	void setMask(bool maskValue);

	/**
	* Set the mask conditionally.
	* Set the mask values (groupMask) to 1 only for animation parameters 
	* whose value is different in the faps passed as parameter. Other mask values are not touched.
	*
	* @param faps the FAPs used for comparison.
	*
	* @see FAPs::maskType
	* @see FAPs::groupMask
	*/
	void setMaskIfDiff(FAPs *faps);

	/**
	* Set the mask values.
	* Set the mask values (maskType and groupMask) by copying them from faps.
	*
	* @param faps the FAPs from which mask values will be copied.
	*
	* @see FAPs::maskType
	* @see FAPs::groupMask
	*/
	void setMask(FAPs *faps);

	/**
	* Low level FAPs.
	* Storage for low level FAPs.
	* @see LLFAPs
	*/
	LLFAPs llf;

	/**
	* FAP1 (viseme).
	* Storage for FAP1 (viseme).
	* @see FAP1
	*/
	FAP1 fap1;

	/**
	* FAP2 (expression).
	* Storage for FAP2 (expression).
	* @see FAP2
	*/
	FAP2 fap2;

	/**
	* Mask types.
	*
	* Note: masking is used only when FAPs are encoded into a file/bitstream. If you are not encoding
	* FAPs (i.e. writing them into FBA files, e.g. using CFBAEncoder or FbaFileAction::write()), 
	* you do not need to change the groupMask or maskType. Even if you do write files, the default values
	* for groupMask or maskType will produce correct results. However, by changing these parameters you can achieve
	* much better coding efficiency, i.e. make the FBA files much smaller.
	*
	* Mask types for FAP groups.
	*
	* The masking mechanism is explained in detail in <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a> (section "FAP grouping and masking").
	* If that looks a bit complicated, you can just leave the default values, with all maskType values to 3, so all %FAPs
	* will be encoded. In this case you do not need to touch @ref groupMask or @ref maskType at all. To improve efficiency, you may
	* set maskType for some groups to 0 (e.g. to disable group 10 (ears) set maskType[9] to 0). You still do not
	* need to set the @ref groupMask.
	*
	* The indices in the groupMask array correspond to the list of FAP groups the MPEG-4 documentation,
	* but the
	* numbers are shifted by one i.e. maskType[0] corresponds to group 1 in 
	* the documentation (visemes and expressions). 
	*
	* @see groupMask
	*/
	int maskType[N_FAP_GROUP];

	/**
	* Group mask.
	*
	* Note: masking is used only when FAPs are encoded into a file/bitstream. If you are not encoding
	* FAPs (i.e. writing them into FBA files, e.g. using CFBAEncoder or FbaFileAction::write()), 
	* you do not need to change the groupMask or maskType. Even if you do write files, the default values
	* for groupMask or maskType will produce correct results. However, by changing these parameters you can achieve
	* much better coding efficiency, i.e. make the FBA files much smaller.
	*
	* The masking mechanism is explained in detail in <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a> (section "FAP grouping and masking").
	* If that looks a bit complicated, you can just leave the default values, with all @ref maskType values to 3, so all %FAPs
	* will be encoded - these are the default values. In this case you do not need to touch @ref groupMask or @ref maskType at all. To improve efficiency, you may
	* set maskType for some groups to 0 (e.g. to disable group 10 (ears) set maskType[9] to 0). You still do not
	* need to set the @ref groupMask.
	*
	* The indices in the groupMask array correspond to the list of %FAPs in the MPEG-4 documentation,
	* but the
	* numbers are shifted by one i.e. groupMask[0] corresponds to FAP number 1 in 
	* the documentation (viseme). 
	*
	* For convenience, the enumerator values defined in FAPNames can be used as index values for the groupMask array, e.g. groupMask[open_jaw].
	*
	* Note that the only groupMask values used are those for FAPs for which @ref maskType is set to 1 or 2. 
	* Other values in the groupMask array are not used and do not need to be set.
	*
	* @see maskType
	* @see FAPNames
	*/
	int groupMask[NFAP];
};

}

#endif // __FAPs_h__
