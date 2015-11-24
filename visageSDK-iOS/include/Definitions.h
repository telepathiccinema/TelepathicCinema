#ifndef GUARD_DEFINITIONS
#define GUARD_DEFINITIONS

//******GENERAL_BEHAVIOUR*****************************************************
#define D_USE_HYBRID_DETECTION
#define D_USE_VARIABLE_FD_QCUTOFF
#define D_USE_AUTO_RESIZING
#define D_USE_SOBEL_FOR_TRACKING	//if defined -> slower, more precise
#ifndef EMSCRIPTEN
#define D_USE_IMAGE_ROTATION		//if defined -> slower, wider roll angles 
#endif
#define CASCADE_SHORT_TWEAK         // if defined items in cascade table are on the fly converted to byte, and with that speedup is achieved
#if defined (IOS) || defined (ANDROID)
#define D_USE_DETECTOR_THREAD
#endif

//******DIAGNOSTICS***********************************************************
//#define D_FRAME_BY_FRAME_DIAGNOSTICS
//#define D_REDIRECT_CONSOLE_OUTPUT
//#define D_DISPLAY_CV_DIAGNOSTICS

//******HELPERS***************************************************************
#define D_FDP_LUT(a,b) ((a)*100+(b))
#define D_CHECK_BIT(var,pos) ((var) & (1<<(pos)))


//******ENUMERATIONS**********************************************************
enum FaceOrientation{Frontal, LProfile, LSemiProfile, RProfile, RSemiProfile};
enum TrackerStates{Initializing, Recovering, Tracking};


//******CONSTANTS*************************************************************
#define D_PI 3.14159265358979323846
const int d_groupSizes[13] = {0,0,14,14,6,4,4,1,10,15,10,6,47};

#endif //GUARD_DEFINITIONS