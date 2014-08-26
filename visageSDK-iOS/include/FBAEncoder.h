
#ifndef __FBAEncoder_h__
#define __FBAEncoder_h__

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
#include "FBACodingParameters.h"

// C++
#include <stdio.h>  //	 FILE
#ifdef DEBUG
#include <assert.h>
#endif

// Win32
#ifdef WIN32
//#include <io.h>  //	_write method
//#include <direct.h>
#include <stdlib.h>
#endif

namespace VisageSDK
{

#define sign(x) 	((x) < 0 ? -1 : 1)


#define ENCODER_BUFFER_SIZE 5000

typedef struct 
{
  int frame_num;
  int quant;
  bool is_time_code;
  bool is_frame_rate;
  bool skip_frames;
  int frame_rate;
  int fr_seconds;
  int fr_frequency_offset;
  int number_of_frames_to_skip;
  int time_code_hours;
  int time_code_minutes;
  int time_code_seconds;

  bool is_i_new_max;
  bool is_i_new_min;
  bool is_p_new_max;
  bool is_p_new_min;
  bool is_fap_quant;
  bool new_mask_flag;
  bool havesetminmax;
  int ve_orig[7];
  int ve_prev[7]; 
  int ve_diff[7];
  int viseme_def; 
  int init_face;
  int expression_def;
  int viseme_blend_Q; 
  int expr_intensity1_Q, expr_intensity2_Q;
  int *freq_ve[7];
  int *cumul_ve[7];
  int *Ifreq_ve[7];
  int *Icumul_ve[7];
  int Irange_ve_max[7];
  int Irange_ve_min[7];
  int range_ve_max[7];
  int range_ve_min[7];
  int *fap_mask, *fap_mask2, *prev_fap_mask;
  int fap_mask_type[N_FAP_GROUP];
  int range_max[NFAP], range_min[NFAP];
  int Irange_max[NFAP], Irange_min[NFAP];
  int *freq[NFAP], *cumul_freq[NFAP];
  int *Ifreq[NFAP], *Icumul_freq[NFAP];
  int i_new_max[NFAP];
  int i_new_min[NFAP];
  int p_new_max[NFAP];
  int p_new_min[NFAP];
  int minmaxset[NFAP];
  int fap_orig[NFAP], fap_Q[NFAP];
  int fap_prev[NFAP], fap_diff[NFAP];
  FILE *fap_out_fp;
  int mask_index[11];
  int suggested_gender;
  int coding_type;
} Face_object_t;

/*
** 
** BAP Structure 
**
*/

typedef struct 
{
  int quant;
  int frame_num;
  bool is_time_code;
  bool is_frame_rate;
  bool skip_frames;
  int frame_rate;
  int fr_seconds;
  int fr_frequency_offset;
  int number_of_frames_to_skip;
  int time_code_hours;
  int time_code_minutes;
  int time_code_seconds;

  bool is_bap_quant;
  bool new_mask_flag;

  bool is_i_new_max;
  bool is_i_new_min;
  bool is_p_new_max;
  bool is_p_new_min;
  bool havesetminmax;

  int i_new_max[NUM_BAPS];
  int i_new_min[NUM_BAPS];
  int p_new_max[NUM_BAPS];
  int p_new_min[NUM_BAPS];
  int minmaxset[NUM_BAPS];
  int bap_orig[NUM_BAPS], bap_Q[NUM_BAPS];
  int bap_prev[NUM_BAPS], bap_diff[NUM_BAPS];

  int *bap_mask, *bap_mask2, *prev_bap_mask;
  int bap_mask_type[BAP_NUM_GROUPS];
  int range_max[NUM_BAPS], range_min[NUM_BAPS];
  int Irange_max[NUM_BAPS], Irange_min[NUM_BAPS];
  int *freq[NUM_BAPS], *cumul_freq[/*NUM_BAPS*/50000];
  int *Ifreq[NUM_BAPS], *Icumul_freq[/*NUM_BAPS*/50000];

  FILE *bap_out_fp;

  int bap_mask_index[BAP_NUM_GROUPS][MAX_BAPS];
  int baps_in_group[BAP_NUM_GROUPS];
  int suggested_gender;
  int coding_type;

} body_object_t;

typedef struct 
{
  bool is_startcode;/*-edp*/
  int fba_paramset;
  bool is_intra;
  bool marker_bit;
  int cur_frame_bit_cnt;
  FILE *stream_fh; //int stream_fh;
  Face_object_t fo;
  body_object_t bo;
} Fba_object_plane_t;



/**
*	FBA Encoder.
* Implements the FBA Encoder functions: encoding Face and Body Animation Parameters (FBAPs) into an FBA file either 
* frame-by-frame, or from an ASCII FAP and/or BAP files. 
*
* If encoding from an ASCII FAP and/or BAP files, it requires an appropriate EPF (Encoder Parameter File, .epf) file. <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>
* contains information on the EPF file format. Sample ASCII FAP and BAP files with corresponding EPF files are provided too.
*
* When encoding frame-by-frame, FBAPs structure is sent to the encoder for each frame.
*
* Implemented in visagefbaencoder.lib
*
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/FBACodec/doc/index.html">Example2</a>.
*
*/
class VISAGE_DECLSPEC CFBAEncoder {
public:

	// Added by KStankovic
	/**
	* Destructor.
	*/
	~CFBAEncoder();
	//

	/**
	* Constructor.
	* Constructs the FBAEncoder. 
	*/
	CFBAEncoder();

	/**
	* Constructor.
	* Constructs the FBAEncoder with the possibility of outputing an ASCII FAP and BAP files in the format described in <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
	*
	* @param outFAPFileName the name of the FAP file name to be produced; if this parameter is NULL, no ASCII FAP file is produced
	* @param outBAPFileName the name of the BAP file name to be produced; if this parameter is NULL, no ASCII BAP file is produced
	*/
	CFBAEncoder(char *outFAPFileName,char *outBAPFileName);
	
	// objectFunctionality not implemented
	CFBAEncoder(int objectFunctionality);

	/**
	* Initialize encoding parameters. Must be called before encoding new stream.
	*/
	void initializeEncoding();

 	/**
	* Finish the bitstream and clean up the decoder. Returns the last bits of the bitstream.
    * @param pts returns current time stamp (after the frame)
    * @param outBuffer this buffer will be filled with encoded bitstream; note that the buffer must be allocated with appropriate size (at least 3000 is recommended)
    * @param buffer_size_bytes size in bytes of the outBuffer
	* @return size in bits (yes, bits not bytes) of returned bitstream,
    * or -1 on failure; size is also returned in the size parameter;
	*/
	int  finalizeBitstream(unsigned char *outBuffer, int buffer_size_bytes,long *pts);
	
	/**
	* Encode a frame of %FBAPs .
    * encode a frame of Fcae and Body Animation Parameters (FBAPs) and return the
    * resulting bitstream in outBuffer.
    * 
    * @param fbap the FBAPs to encode for this frame
    * @param pts returns current time stamp (after the frame)
    * @param outBuffer this buffer will be filled with encoded bitstream; note that the buffer must be allocated with appropriate size (at least 3000 is recommended)
    * @param buffer_size_bytes size in bytes of the outBuffer
	* @return size in bits (yes, bits not bytes) of returned bitstream,
    * or -1 on failure; size is also returned in the size parameter;
	*
	* @see FAPs
	*/
	int  encodeFrame( FBAPs *fbap,
                      unsigned char *outBuffer, int buffer_size_bytes, long *pts);
	
	/**
	* Utility for writing out bits. Handles non-byte-alligned cases, i.e. the cases 
	* when subsequent calls to this function contain numbers of bits that are not byte-aligned.
    * Call with 0,0,0 parameters to initialize before starting new stream.
	* @param streamHandle the file handle into which bits are written
	* @param bits the bits to write
	* @param size the number of bits to write
	*/
	void write_bits(FILE *streamHandle, unsigned char *bits, int size); //void write_bits(int streamHandle, unsigned char *bits, int size);

 	/**
	* Encode a FAP and a BAP file into a bitstream. 
	* @param epffile the EPF file corresponding to the FAP/BAP files (the EPF file format is documented in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>)
	* @param fapfile the ASCII FAP file to be encoded, or NULL if no FAP file is encoded (the ASCII FAP file format is documented in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>)
	* @param bapfile  the ASCII BAP file to be encoded, or NULL if no BAP file is encoded (the ASCII BAP file format is documented in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>)
	* @param outfile the name of the output file which will be encoded
	* @param fap_outfile the name of the output ASCII FAP file which will be written out for control purposes
	*/
	void encodeFBAPFiles( char *epffile, char *fapfile, char *bapfile, char *outfile, char *fap_outfile);

 	/**
	* Get coding parameters. 
	* Get the pointer to the Coding Parameters class that holds the parameters used for frame-by-frame encoding. 
	* The parameters can be set directly in the returned class. The parameters are automatically set to default
	* values which work fine in most cases, so they need to be changed only in special cases. The coding parameters are 
	* documented in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
	*
	* @return pointer to the Coding Parameters class; its members are public and can be directly set
	* @see CodingParameters
	*/
	CodingParameters *getCodingParameters(){return &ep;};

private:
	void assure_fap_frame(void);
	void assure_bap_frame(void);
	void set_ve_ranges(void);
	void setEncoderParameters(void);
	void initialize_fop(void);
	void reset_frame(void);
	int  bitcount();
	void initbits_frame();
	int  expand_mask();
	void init_arith();
	void fba_object_plane_header();
	void fba_object_plane_data();
	bool get_line(FILE *ifp);
	void initbits();
	int  putbits(int n, int val);
	int  next_start_code();
	int  encode(int argc, char **argv);
	void initialize(int argc, char **argv);
	void cleanup(void);
	void finish_frame(void);
	void code_frame(void);
	void code_new_minmax();
	void code_fap_group_mask(int n);
	void code_skip_frames();
	void code_time_code();
	void code_frame_rate();
	void code_ifap();
	void code_pfap();
	void code_viseme(int is_intra);
	void code_expression(int is_intra);
	void check_range(int symbol, int i, int inter);
	int  AR_Encode(int index, int cumul_freq[]);
	int  bit_opp_bits(int bit);
	int  encoder_flush();
	int  bit_in_psc_layer(int bit);
	void update(int symbol, int index);
	void update_viseme_expression(int symbol, int index);
	int  quant(int fap, int qstep);
	int  dequant(int qfap, int qstep);
	int  pred(int fap_curr, int fap_prev);
	int  recon(int fap_diff, int fap_prev);
	void bap_code_frame_rate() ;
	void bap_code_time_code() ;
	void bap_code_skip_frames() ;
	void bap_code_new_minmax();
	void code_ibap() ;
	void code_pbap() ;
	void bap_check_range(int symbol, int i, int inter) ;
	void bap_init_arith();
	void code_bap_group_mask(int n);
	void bap_update(int symbol, int index);
	
	bool produceOutFapFile;
	bool produceOutBapFile;
	bool fapMaskWarningGiven;
	bool bapMaskWarningGiven;
	char fap_out_file[300];
	char bap_out_file[300];
    long low;
	long high;
	long opposite_bits;
	long length;
	long zerorun;
	FILE *fap_fp;
	FILE *bap_fp;
	FILE *epf_fp;
	int head_cnt;
	int ibit_cnt;
	int pbit_cnt;
	int total_bit_cnt;
	int frame_time;
	unsigned char outbfr[ENCODER_BUFFER_SIZE];
	int outcnt;
	int outcnt_frame;
	int bytecnt;
	Fba_object_plane_t fop;
	CodingParameters ep;
	CodingParameters ep_prev;
	float fap_version, bap_version;
};

}

#endif // __FBAEncoder_h__
