
#ifndef __FBADecoder_h__
#define __FBADecoder_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "FBAPs.h"
#include "Fba_object_plane.h"
#include "FBACodingParameters.h"

namespace VisageSDK
{

/**
* The MPEG-4 FBA decoder.
* This class provides the low-level, frame-by-frame decoding of an MPEG-4 FBA file.
* The Face and Body Animation Parameters (FBAPs), CodingParameters and the time stamp can be obtained for each
* frame after it is decoded.
*
* For a higher-level decoding, allowing to decode the whole file at once and play it or access any
* frame randomly, use FbaFileAction.
*
* Implemented in visagefbadecoder.lib
*
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/FBACodec/doc/index.html">Example2</a>.
*
* @see FBAPs
* @see CodingParameters
* @see FbaFileAction
*/
class VISAGE_DECLSPEC CFBADecoder {
public:

	// Added by KStankovic
	/**
	* Destructor.
	*/
	~CFBADecoder();
	//

	/**
	* Constructor.
	* Opens an MPEG-4 FBA file and initialises the decoder.
	* @param fn the name of the MPEG-4 FBA file to be decoded
	*/
	CFBADecoder(char *fn);

	/**
	* Decode a frame.
	* Decode the next frame of animation.
	* @return the function returns 0 if this is the last frame of animation; otherwise it returns 1.
	*/
	int decodeFrame();

	/**
	* Get the Face and Body Animation Parameters (FBAPs).
	* Get the FBAPs from the last decoded frame.
	* @return the FBAPs from the last decoded frame..
	*/
	FBAPs *getFBAPs();

	/**
	* Get the time stamp.
	* Get the time stamp from the last decoded frame.
	* @return the time stamp from the last decoded frame, in milliseconds.
	*/
	long getTimeStamp();

	/**
	* Get the coding parameters.
	* Get the coding parameters from the last decoded frame. These parameters only deal with coding,
	* i.e. the way FBAPs are written into a file/bitstream. If you are decoding FBAPs to do animation and
	* do not intend to code them again using the CFBAEncoder, there is no need to read them.
	*
	* If the decoded FBAPs are going to be encoded again using the CFBAEncoder, the coding parameters
	* may be used. 
	*
	* @return the coding parameters.
	* @see CodingParameters,CFBAEncoder
	*/
	CodingParameters *getCodingParameters() {return &cp;};

private:
	int low, high, code_value, bit, length, sacindex, cum, zerorun;
	int is_intra;
	Fba_object_plane fop;
	int face_paramset_mask;
	int suggested_gender;
	int coding_type;
	double real_frame_rate;
	double NEXT_PTS;
	int first_intra_segment;
	FBAPs fbaps;
	long PTS;
	CodingParameters cp;
	int first_action;
	int fr_no;
    int no_frames;

	void bit_out_psc_layer();
	int decode_a_symbol(int cumul_freq[]);
	void decode_expression(int is_intra);
	void decode_fap_group_mask(int n);
	void decode_frame_rate();
	void decode_ifap();
	void decode_new_minmax();
	void decode_pfap();
	void decode_skip_frames();
	void decode_time_code();
	void decode_viseme(int is_intra);
	void decoder_reset();
	int dequant(int qfap, int qstep);
	void fba_object_plane_header();
	int fba_object_plane_data();
	void init();
	void bap_init_arith(int *bap_mask);
	void init_arith(int fap_mask[]);
	int recon(int fap_diff, int fap_prev);
	void update(int symbol, int index);
	void update_viseme_expression(int symbol, int index);

	void bap_decode_skip_frames(void);
	void bap_decode_frame_rate(void);
	void bap_decode_time_code(void);
	void bap_decode_group_mask(int);
	void bap_decode_new_minmax(void);
	void decode_ibap();
	void decode_pbap();
	void bap_update(int symbol, int index);

};

}

#endif // __FBADecoder_h__
