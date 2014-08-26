
#ifndef __Body_object_h__
#define __Body_object_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "BAPs.h"

namespace VisageSDK
{

class VISAGE_DECLSPEC Body_object  
{
public:
	Body_object();
	virtual ~Body_object();
	
	int is_time_code;
	int is_frame_rate;
	int skip_frames;
	int frame_rate, seconds, frequency_offset;
	int time_code_hours, time_code_minutes, marker_bit, time_code_seconds;
	int number_of_frames_to_skip;
	int bap_quant;
	int is_i_new_max, is_i_new_min, is_p_new_max, is_p_new_min;
	int bap_mask[NUM_BAPS];
	int bap_orig[NUM_BAPS], bap_Q[NUM_BAPS],
		bap_prev[NUM_BAPS], bap_diff[NUM_BAPS];
	int bap_mask_type[BAP_NUM_GROUPS];
	
	int bap_mask_index[BAP_NUM_GROUPS][MAX_BAPS];
	int baps_in_group[BAP_NUM_GROUPS];
	
	int BQP[NUM_BAPS];
	int IBQP[NUM_BAPS];
	
	int  Irange_max[NUM_BAPS], Irange_min[NUM_BAPS];
	int  range_max[NUM_BAPS], range_min[NUM_BAPS];
	int *freq[NUM_BAPS], *cumul_freq[NUM_BAPS];
	int *Ifreq[NUM_BAPS], *Icumul_freq[NUM_BAPS];
	
	int suggested_gender;
	int coding_type;
	
	int has_extendable_baps;
	int num_extendable_baps;
	int extendable_baps[MAX_NUM_EXTENDABLE_BAPS];
};

}

#endif // __Body_object_h__
