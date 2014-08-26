
#ifndef __Face_object_h__
#define __Face_object_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "FAPs.h"

namespace VisageSDK
{

class VISAGE_DECLSPEC Face_object  
{
public:
	Face_object();
	virtual ~Face_object();

	int frame_rate, seconds, frequency_offset;
	int time_code_hours, time_code_minutes, marker_bit, time_code_seconds;
	int number_of_frames_to_skip;
	int fap_quant;
	int is_i_new_max, is_i_new_min, is_p_new_max, is_p_new_min;
	int fap_mask[NFAP];
	int fap_orig[NFAP], fap_Q[NFAP], fap_prev[NFAP], fap_diff[NFAP];
	int ve_orig[7], ve_prev[7], ve_diff[7];
	int viseme_blend_Q, expression_intensity1_Q, expression_intensity2_Q;
	int mask_index[11];
	int QP[NFAP];
	int IQP[NFAP];
	int Idefault_min[NFAP], Idefault_max[NFAP];
	int default_min[NFAP], default_max[NFAP];
	int Irange_ve_max[7], Irange_ve_min[7];
	int range_ve_max[7], range_ve_min[7];
	int *Ifreq_ve[7], *Icumul_ve[7];
	int *freq_ve[7], *cumul_ve[7];
	int Irange_max[NFAP], Irange_min[NFAP];
	int range_max[NFAP], range_min[NFAP];
	int *freq[NFAP], *cumul_freq[NFAP];
	int *Ifreq[NFAP], *Icumul_freq[NFAP];
	int suggested_gender;
	int coding_type;
	int fap_mask_type[N_FAP_GROUP];
	int is_time_code;
};

}

#endif // __Face_object_h__
