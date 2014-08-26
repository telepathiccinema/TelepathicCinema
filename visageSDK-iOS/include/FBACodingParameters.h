
#ifndef __FBACodingParameters_h__
#define __FBACodingParameters_h__

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
* Coding Parameters.
* These parameters are used when FBAPs are encoded into a file or bitstream.
* The decoder reads them when decoding a bitstream and sets them in this structure. They are useful when
* the FBAPs are encoded again.
*
* Default parameter values are set by the constructor. These default parameters will provide good coding
* in most cases, but not necessarily the most efficient one. If the size of the encoded files/bitstreams
* is of particular concern, the coding parameters may be optimized to the encoded animation in order to achieve the best efficiency.
*
* Details about these parameters and their functioning can be found in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* Implemented in visagefbadecoder.lib
*
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/FBACodec/doc/index.html">Example2</a>.
*
* @see CFBADecoder,CFBAEncoder
*/
class VISAGE_DECLSPEC CodingParameters
{

public:
	/**
	* frame_rate.
	* This is an 8 bit unsigned integer indicating the reference frame rate of the sequence. Range: 0-255. Default value is 25.
	*
	*/
	int frame_rate;

	/**
	* seconds.
	* This is a 4 bit unsigned integer indicating the fractional reference frame rate. The frame rate is computed as follows: frame rate  = (frame_rate + seconds/16). Range: 0-15. Default value is 0.
	*
	*/
	int seconds;

	/**
	* frequency_offset.
	* This is a flag which when set to '1' indicates that the frame rate uses the NTSC frequency offset of 1000/1001. This bit would typically be set when frame_rate = 24, 30 or 60, in which case the resulting frame rate would be 23.97, 29.94 or 59.97 respectively. When set to '0' no frequency offset is present. I.e. if (frequency_offset ==1) frame rate  = (1000/1001) * (frame_rate + seconds/16). Range: 0-1. Default value is 0.
	*
	*/
	int frequency_offset;

	/**
	* fap_quant.
	* This is an integer which is the quantization scale factor used to compute the FAP table step size. Range: 0-31. Default value is 1.
	* Larger value decreases the encoded bitstream size but also introduces quantization error.
	*
	*/
	int fap_quant;

	/**
	* bap_quant.
	* This is an integer which is the quantization scale factor used to compute the BAP table step size.
	* Range: 0-31. Default value is 1. Larger value decreases the encoded bitstream size but also introduces quantization error.
	*
	*/
	int bap_quant;

	/**
	* i_max.
	* This is an array of  73 integer values setting the maximum value that each FAP can take when an I frame is encoded. If these values are not big enough, i.e. if a FAP goes out of range while encoding, error will be reported. The first 7 values correspond to viseme1, viseme2, viseme blend, expression1 , expression1 intensity, expression2 , expression2 intensity. The rest of the parameters correspond to the low level FAPs.
	*
	* Default values are:
	*
	* 14 14 63 6 63 6 63 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860
	*/
	int i_max[73];

	/**
	* i_min.
	* This is an array of  73 integer values setting the minimum value that each FAP can take when an I frame is encoded. If these values are not low enough, i.e. if a FAP goes out of range while encoding, error will be reported. The first 7 values correspond to viseme1, viseme2, viseme blend, expression1 , expression1 intensity, expression2 , expression2 intensity. The rest of the parameters correspond to the low level FAPs.
	*
	* Default values are:
	*
	* 14 14 63 6 63 6 63 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860
	*/
	int i_min[73];

	/**
	* p_max.
	* This is an array of  73 integer values setting the maximum value that each FAP can take when an P frame is encoded. If these values are not big enough, i.e. if a FAP goes out of range while encoding, error will be reported. The first 7 values correspond to viseme1, viseme2, viseme blend, expression1 , expression1 intensity, expression2 , expression2 intensity. The rest of the parameters correspond to the low level FAPs.
	*
	* Default values are:
	*
	* 14 14 63 6 63 6 63 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860 1860
	*/
	int p_max[73];

	/**
	* p_min.
	* This is an array of  73 integer values setting the minimum value that each FAP can take when an P frame is encoded. If these values are not low enough, i.e. if a FAP goes out of range while encoding, error will be reported. The first 7 values correspond to viseme1, viseme2, viseme blend, expression1 , expression1 intensity, expression2 , expression2 intensity. The rest of the parameters correspond to the low level FAPs
	*
	* Default values are:
	*
	* -14 -14 -63 -6 -63 -6 -63 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860 -1860
	*/
	int p_min[73];

	/**
	* bap_i_max.
	* This is an array of  296 (constant NUM_BAPS) integer values setting the maximum value that each BAP can take when an I frame is encoded. If these values are not big enough, i.e. if a BAP goes out of range while encoding, error will be reported.
	* Default value is 1859 for each member of this array.
	*
	*/
	int bap_i_max[NUM_BAPS];

	/**
	* bap_i_min.
	* This is an array of  296 (constant NUM_BAPS) integer values setting the minimum value that each BAP can take when an I frame is encoded. If these values are not big enough, i.e. if a BAP goes out of range while encoding, error will be reported.
	* Default value is -1859 for each member of this array.
	*
	*/
	int bap_i_min[NUM_BAPS];

	/**
	* bap_p_max.
	* This is an array of  296 (constant NUM_BAPS) integer values setting the maximum value that each BAP can take when a P frame is encoded. If these values are not big enough, i.e. if a BAP goes out of range while encoding, error will be reported.
	* Default value is 1859 for each member of this array.
	*
	*/
	int bap_p_max[NUM_BAPS];

	/**
	* bap_p_min.
	* This is an array of  296 (constant NUM_BAPS) integer values setting the minimum value that each BAP can take when a P frame is encoded. If these values are not big enough, i.e. if a BAP goes out of range while encoding, error will be reported.
	* Default value is -1859 for each member of this array.
	*
	*/
	int bap_p_min[NUM_BAPS];

	/**
	* paramset_mask.
	* This mask indicates whether FAPs, BAPs or both are coded. The value can be 1 (FAPs coded), 2 (BAPs coded) or 3 (both FAPs and BAPs coded)
	*
	*/
	int paramset_mask;

	/**
	* Constructor.
	* Initialises the coding parameters to default values.
	*/
	CodingParameters();

	/** Copy the coding parameters.
	* Copy the coding parameters from another coding parameters instance.
	* @param cp coding parameters to copy.
	*/
	void copy(CodingParameters *cp);

	/** Compare the coding parameters.
	* Compare the coding parameters with another coding parameters instance.
	* @param cp coding parameters to compare with.
	* @return if the coding parameters are the same, return 1, if there is any difference return 0
	*/
	int isEqual(CodingParameters *cp);
};

}

#endif // __FBACodingParameters_h__
