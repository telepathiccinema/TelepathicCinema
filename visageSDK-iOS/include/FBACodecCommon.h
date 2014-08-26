
#ifndef __FBACodecCommon_h__
#define __FBACodecCommon_h__

namespace VisageSDK
{

// COMMON DEFINES FOR CODING

#define MASK_FDP	0
#define	MASK_FAP	1
#define MASK_BAP	2
#define MASK_BDP	3

#define RANGE_DELTA	60

#define	FAP_PARAMSET_MASK	1
#define BAP_PARAMSET_MASK	2

#define FBA_OBJECT_START_CODE           442 /*0x100 + 0xBA*/
#define FBA_OBJECT_START_CODE_OLD       257 
#define FBA_OBJECT_PLANE_START_CODE     443 /*0x100 + 0xBB*/
#define FBA_OBJECT_PLANE_START_CODE_OLD 259
#define FBA_OBJECT_END_CODE             433 /*0x100 + 0xB1 visual object sequence end code*/
#define FBA_OBJECT_END_CODE_OLD         433 

#define PREDICTIVE_CODING_TYPE          0 
#define DCT_CODING_TYPE                   1 

#define SEG_LENGTH        16


#define fba_cooondef_q1 16384
#define fba_cooondef_q2 32768
#define fba_cooondef_q3 49152
#define fba_cooondef_top 65535
#define fba_cooondef_Max_frequency 16383

#define DEF_FRAME_RATE 30.0

// coder defines
#define MARKER_BIT                  1

// BAP defines
#define NBAP 186                     /* Number of BAPs excluding extension BAPs */
#define MAX_BAPS          30        /* Max no of BAPs that belong to any group */
#define BAP_NUM_GROUPS    24        /* Total no of BAP groups - changed from 19 to 24 for extended baps- edp*/
#define NUM_BAPS          296       /* Total no of BAPs - changed from 186 to 296 for extended baps - edp*/
#define MAX_NUM_EXTENDABLE_BAPS 256  /* 2^8 */


// FAP defines
#define N_FAP_GROUP  10
#define NFAP	68

// viseme defines
#define NVISEME 15
#define NEXP 7


#if 0 /*standard*/
static int fba_cooondef_BQP[NUM_BAPS] = {64,64,64,128,128,128,128,256,256,128,128,256,256,
128,128,256,256,256,256,256,256,256,256,128,128,128,128,128,128,128,128,64,64,64,64,256,256,
64,64,256,256,128,128,128,128,256,256,128,128,128,256,256,256,128,128,128,256,256,256,128,128,128,256,256,256,256,256,256,256,256,256,
128,128,128,256,256,256,256,256,256,256,256,256,256,256,256,128,128,128,256,256,256,256,256,256,256,256,256,
128,128,128,256,256,256,256,256,256,128,128,128,256,256,256,128,128,128,256,256,256,128,128,128,512,512,128,128,128,128,256,256,
128,128,128,128,256,256,128,128,128,128,256,256,128,128,128,128,256,256,128,128,128,128,256,256,128,128,128,128,256,256,128,128,128,128,256,256,
128,128,128,128,128,128,128,128,256,256,128,128,128,128,64,64,64,64,64,64,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
#else /*experiment -edp*/
static int fba_cooondef_BQP[NUM_BAPS] = {170,170,170,170,170,170,170,256,256,170,170,256,256,
170,170,256,256,256,256,256,256,256,256,170,170,170,170,170,170,170,170,170,170,170,170,256,256,
170,170,256,256,170,170,170,170,256,256,170,170,170,256,256,256,170,170,170,256,256,256,170,170,170,256,256,256,256,256,256,256,256,256,
170,170,170,256,256,256,256,256,256,256,256,256,256,256,256,170,170,170,256,256,256,256,256,256,256,256,256,
170,170,170,256,256,256,256,256,256,170,170,170,256,256,256,170,170,170,256,256,256,170,170,170,512,512,170,170,170,170,256,256,
170,170,170,170,256,256,170,170,170,170,256,256,170,170,170,170,256,256,170,170,170,170,256,256,170,170,170,170,256,256,170,170,170,170,256,256,
170,170,170,170,170,170,170,170,256,256,170,170,170,170,2,2,2,170,170,170,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
#endif


}

#endif // __FBACodecCommon_h__
