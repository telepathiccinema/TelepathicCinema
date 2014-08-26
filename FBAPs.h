
#ifndef __FBAPs_h__
#define __FBAPs_h__

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
#include "BAPs.h"

// C++
#include <string>
#ifdef _DEBUG
#include <assert.h>
#endif

namespace VisageSDK
{

/**
* FBAP name enumerators.
* FBAP name enumerators.
* \defgroup FBAPNames
*/

/**
* BAP names defined as constants.
*
* BAP names defined as constants corresponding to the BAP indices. This provides a convenient way to
* access BAPs.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* @see FBAPs
* @see BAPs
*
* \ingroup FBAPNames
*/
enum BAPNames
{
		sacroiliac_tilt, /**< Forward-backward motion of the pelvis in the sagittal plane. */
		sacroiliac_torsion, /**< Rotation of the pelvis along the body  vertical axis (defined by skeleton root). */
		sacroiliac_roll, /**< Side to side swinging of the pelvis in the coronal plane. */
		l_hip_flexion, /**< Forward-backward rotation in the sagittal plane. */
		r_hip_flexion, /**< Forward-backward rotation in the sagittal plane. */
		l_hip_abduct, /**< Sideward opening in the coronal plane  */
		r_hip_abduct, /**< Sideward opening in the coronal plane  */
		l_hip_twisting, /**< Rotation along the thigh axis. */
		r_hip_twisting, /**< Rotation along the thigh axis. */
		l_knee_flexion, /**< Flexion-extension of the leg in the sagittal plane. */
		r_knee_flexion, /**< Flexion-extension of the leg in the sagittal plane. */
		l_knee_twisting, /**< Rotation along the shank axis. */
		r_knee_twisting, /**< Rotation along the shank axis. */
		l_ankle_flexion, /**< Flexion-extension of the foot  in the sagittal plane  */
		r_ankle_flexion, /**< Flexion-extension of the foot  in the sagittal plane  */
		l_ankle_twisting, /**< Rotation along the knee axis. */
		r_ankle_twisting, /**< Rotation along the knee axis. */
		l_subtalar_flexion, /**< Sideward orientation of the foot   */
		r_subtalar_flexion, /**< Sideward orientation of the foot   */
		l_midtarsal_twisting, /**< Internal twisting of the foot (also called navicular joint in anatomy). */
		r_midtarsal_twisting, /**< Internal twisting of the foot (also called navicular joint in anatomy). */
		l_metatarsal_flexion, /**< Up and down rotation of the toe in the sagittal plane. */
		r_metatarsal_flexion, /**< Up and down rotation of the toe in the sagittal plane. */
		l_sternoclavicular_abduct, /**< Up and down motion in the coronal plane  */
		r_sternoclavicular_abduct, /**< Up and down motion in the coronal plane  */
		l_sternoclavicular_rotate, /**< Rotation in the transverse plane. */
		r_sternoclavicular_rotate, /**< Rotation in the transverse plane. */
		l_acromioclavicular_abduct, /**< Up and down motion in the coronal plane  */
		r_acromioclavicular_abduct, /**< Up and down motion in the coronal plane  */
		l_acromioclavicular_rotate, /**< Rotation in the transverse plane  */
		r_acromioclavicular_rotate, /**< Rotation in the transverse plane  */
		l_shoulder_flexion, /**< Forward-backward motion in the sagittal plane  */
		r_shoulder_flexion, /**< Forward-backward motion in the sagittal plane  */
		l_shoulder_abduct, /**< Sideward motion in the coronal plane  */
		r_shoulder_abduct, /**< Sideward motion in the coronal plane  */
		l_shoulder_twisting, /**< Rotation along the scapular axis. */
		r_shoulder_twisting, /**< Rotation along the scapular axis. */
		l_elbow_flexion, /**< Flexion-extension of the arm in the sagittal plane  */
		r_elbow_flexion, /**< Flexion-extension of the arm in the sagittal plane  */
		l_elbow_twisting, /**< Rotation of the forearm along the upper arm axis. */
		r_elbow_twisting, /**< Rotation of the forearm along the upper arm axis. */
		l_wrist_flexion, /**< Rotation of the hand in the coronal plane  */
		r_wrist_flexion, /**< Rotation of the hand in the coronal plane  */
		l_wrist_pivot, /**< Rotation of the hand in the sagittal planes. */
		r_wrist_pivot, /**< Rotation of the hand in the sagittal planes. */
		l_wrist_twisting, /**< Rotation of the hand along the forearm axis. */
		r_wrist_twisting, /**< Rotation of the hand along the forearm axis. */
		skullbase_roll, /**< Sideward motion of the skull along the frontal axis. */
		skullbase_torsion, /**< Twisting of the skull along the vertical axis. */
		skullbase_tilt, /**< Forward-backward motion in the sagittal plane along a lateral axis  */
		vc1_roll, /**< Sideward motion of vertebra C1  */
		vc1_torsion, /**< Twisting of vertebra C1  */
		vc1_tilt, /**< Forward-backward motion of vertebra C1 in the sagittal plane. */ 
		vc2_roll, /**< Sideward motion of vertebra C2  */
		vc2_torsion, /**< Twisting of vertebra C2  */
		vc2_tilt, /**< Forward-backward motion of vertebra C2 in the sagittal plane  */
		vc3_roll, /**< Sideward motion of vertebra C3  */
		vc3_torsion, /**< Twisting of vertebra C3  */
		vc3_tilt, /**< Forward-backward motion of vertebra C3 in the sagittal plane  */
		vc4_roll, /**< Sideward motion of vertebra C4  */
		vc4_torsion, /**< Twisting of vertebra C4  */
		vc4_tilt, /**< Forward-backward motion of vertebra C4 in the sagittal plane  */
		vc5_roll, /**< Sideward motion of vertebra C5  */
		vc5_torsion, /**< Twisting of vertebra C5  */
		vc5_tilt, /**< Forward-backward motion of vertebra C5 in the sagittal plane  */
		vc6_roll, /**< Sideward motion of vertebra C6  */
		vc6_torsion, /**< Twisting of vertebra C6  */
		vc6_tilt, /**< Forward-backward motion of vertebra C6 in the sagittal plane  */
		vc7_roll, /**< Sideward motion of vertebra C7  */
		vc7_torsion, /**< Twisting of vertebra C7  */
		vc7_tilt, /**< Forward-backward motion of vertebra C7 in the sagittal plane  */
		vt1_roll, /**< Sideward motion of vertebra T1  */
		vt1_torsion, /**< Twisting of vertebra T1  */
		vt1_tilt, /**< Forward-backward motion of vertebra T1 in the sagittal plane  */
		vt2_roll, /**< Sideward motion of vertebra T2  */
		vt2_torsion, /**< Twisting of vertebra T2  */
		vt2_tilt, /**< Forward-backward motion of vertebra T2 in the sagittal plane  */
		vt3_roll, /**< Sideward motion of vertebra T3  */
		vt3_torsion, /**< Twisting of vertebra T3  */
		vt3_tilt, /**< Forward-backward motion of vertebra T3 in the sagittal plane  */
		vt4_roll, /**< Sideward motion of vertebra T4  */
		vt4_torsion, /**< Twisting of vertebra T4  */
		vt4_tilt, /**< Forward-backward motion of vertebra T4 in the sagittal plane  */
		vt5_roll, /**< Sideward motion of vertebra T5  */
		vt5_torsion, /**< Twisting of vertebra T5  */
		vt5_tilt, /**< Forward-backward motion of vertebra T5 in the sagittal plane  */
		vt6_roll, /**< Sideward motion of vertebra T6  */
		vt6_torsion, /**< Twisting of vertebra T6  */
		vt6_tilt, /**< Forward-backward motion of vertebra T6 in the sagittal plane  */
		vt7_roll, /**< Sideward motion of vertebra T7  */
		vt7_torsion, /**< Twisting of vertebra T7  */
		vt7_tilt, /**< Forward-backward motion of vertebra T7 in the sagittal plane  */
		vt8_roll, /**< Sideward motion of vertebra T8  */
		vt8_torsion, /**< Twisting of vertebra T8  */
		vt8_tilt, /**< Forward-backward motion of vertebra T8 in the sagittal plane  */
		vt9_roll, /**< Sideward motion of vertebra T9  */
		vt9_torsion, /**< Twisting of vertebra T9  */
		vt9_tilt, /**< Forward-backward motion of vertebra T9 in the sagittal plane  */
		vt_10_roll, /**< Sideward motion of vertebra T10  */
		vt10_torsion, /**< Twisting of vertebra T10  */
		vt10_tilt, /**< Forward-backward motion of vertebra T10 in sagittal plane  */
		vt11_roll, /**< Sideward motion of vertebra T11  */
		vt11_torsion, /**< Twisting of vertebra T11  */
		vt11_tilt, /**< Forward-backward motion of vertebra T11 in sagittal plane  */
		vt12_roll, /**< Sideward motion of vertebra T12  */
		vt12_torsion, /**< Twisting of vertebra T12  */
		vt12_tilt, /**< Forward-backward motion of vertebra T12 in sagittal plane  */
		vl1_roll, /**< Sideward motion of vertebra L1  */
		vl1_torsion, /**< Twisting of vertebra L1  */
		vl1_tilt, /**< Forward-backward motion of vertebra L1 in sagittal plane  */
		vl2_roll, /**< Sideward motion of vertebra L2  */
		vl2_torsion, /**< Twisting of vertebra L2  */
		vl2_tilt, /**< Forward-backward motion of vertebra L2 in sagittal plane  */
		vl3_roll, /**< Sideward motion of vertebra L3  */
		vl3_torsion, /**< Twisting of vertebra L3  */
		vl3_tilt, /**< Forward-backward motion of vertebra L3 in sagittal plane  */
		vl4_roll, /**< Sideward motion of vertebra L4  */
		vl4_torsion, /**< Twisting of vertebra L4  */
		vl4_tilt, /**< Forward-backward motion of vertebra L4 in sagittal plane  */
		vl5_roll, /**< Sideward motion of vertebra L5  */
		vl5_torsion, /**< Twisting of vertebra L5  */
		vl5_tilt, /**< Forward-backward motion of vertebra L5 in sagittal plane  */
		l_pinky0_flexion, /**< Metacarpal flexing mobility of the pinky finger. */
		r_pinky0_flexion, /**< Metacarpal flexing mobility of the pinky finger. */
		l_pinky1_flexion, /**< First knukle of the pinky finger. */
		r_pinky1_flexion, /**< First knukle of the pinky finger. */
		l_pinky1_pivot, /**< Lateral mobility of the pinky finger. */
		r_pinky1_pivot, /**< Lateral mobility of the pinky finger. */
		l_pinky1_twisting, /**< Along the pinky finger axis. */
		r_pinky1_twisting, /**< Along the pinky finger axis. */
		l_pinky2_flexion, /**< Second knuckle of the pinky number  */
		r_pinky2_flexion, /**< Second knuckle of the pinky number  */
		l_pinky3_flexion, /**< Third knuckle of the pinky finger. */
		r_pinky3_flexion, /**< Third knuckle of the pinky finger. */
		l_ring0_flexion, /**< Metacarpal flexing mobility of the ring finger. */
		r_ring0_flexion, /**< Metacarpal flexing mobility of the ring finger. */
		l_ring1_flexion, /**< First knukle of the ring finger. */
		r_ring1_flexion, /**< First knukle of the ring finger. */
		l_ring1_pivot, /**< Lateral mobility of the ring finger. */
		r_ring1_pivot, /**< Lateral mobility of the ring finger. */
		l_ring1_twisting, /**< Along the ring finger axis. */
		r_ring1_twisting, /**< Along the ring finger axis. */
		l_ring2_flexion, /**< 	Second knuckle of the ring number  */
		r_ring2_flexion, /**< 	Second knuckle of the ring number  */
		l_ring3_flexion, /**< 	Third knuckle of the ring finger. */
		r_ring3_flexion, /**< 	Third knuckle of the ring finger. */
		l_middle0_flexion, /**< Metacarpal flexing mobility of the middle finger. */
		r_middle0_flexion, /**< Metacarpal flexing mobility of the middle finger. */
		l_middle1_flexion, /**< First knukle of the middle finger. */
		r_middle1_flexion, /**< First knukle of the middle finger. */
		l_middle1_pivot, /**< 	Lateral mobility of the middle finger. */
		r_middle1_pivot, /**< 	Lateral mobility of the middle finger. */
		l_middle1_twisting, /**< Along the middle finger axis. */
		r_middle1_twisting, /**< Along the middle finger axis. */
		l_middle2_flexion, /**< Second knuckle of the middle number  */
		r_middle2_flexion, /**< Second knuckle of the middle number  */
		l_middle3_flexion, /**< Third knuckle of the middle finger. */
		r_middle3_flexion, /**< Third knuckle of the middle finger. */
		l_index0_flexion, /**< Metacarpal flexing mobility of the index finger. */
		r_index0_flexion, /**< Metacarpal flexing mobility of the index finger. */
		l_index1_flexion, /**< First knukle of the index finger. */
		r_index1_flexion, /**< First knukle of the index finger. */
		l_index1_pivot, /**< Lateral mobility of the index finger. */
		r_index1_pivot, /**< Lateral mobility of the index finger. */
		l_index1_twisting, /**< Along the index finger axis. */
		r_index1_twisting, /**< Along the index finger axis. */
		l_index2_flexion, /**< Second knuckle of the index number  */
		r_index2_flexion, /**< Second knuckle of the index number  */
		l_index3_flexion, /**< Third knuckle of the index finger. */
		r_index3_flexion, /**< Third knuckle of the index finger. */
		l_thumb1_flexion, /**< First knukle of the thumb finger. */
		r_thumb1_flexion, /**< First knukle of the thumb finger. */
		l_thumb1_pivot, /**< Lateral mobility of the thumb finger. */
		r_thumb1_pivot, /**< Lateral mobility of the thumb finger. */
		l_thumb1_twisting, /**< Along the thumb finger axis. */
		r_thumb1_twisting, /**< Along the thumb finger axis. */
		l_thumb2_flexion, /**< Second knuckle of the thumb number  */
		r_thumb2_flexion, /**< Second knuckle of the thumb number  */
		l_thumb3_flexion, /**< Third knuckle of the thumb finger. */
		r_thumb3_flexion, /**< Third knuckle of the thumb finger. */
		HumanoidRoot_tr_vertical, /**< Body origin translation in vertical direction. */
		HumanoidRoot_tr_lateral, /**< 	Body origin translation in lateral direction. */
		HumanoidRoot_tr_frontal, /**< 	Body origin translation in frontal direction. */
		HumanoidRoot_rt_body_turn, /**< Rotation of the skeleton root along the body coordinate system's vertical axis. */
		HumanoidRoot_rt_body_roll, /**< Rotation of the skeleton root along the body coordinate system's frontal axis. */
		HumanoidRoot_rt_body_tilt /**< Rotation of the skeleton root along the body coordinate system's lateral axis. */
};                

/**
* BAP names as strings.
*
* BAP names in an array of strings. @ref BAPNames enumerator can be used to access each name.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* \ingroup FBAPNames
*/
static char BAPNameString[NBAP][50]=
{
	"sacroiliac_tilt", /**< Forward-backward motion of the pelvis in the sagittal plane. */
	"sacroiliac_torsion", /**< Rotation of the pelvis along the body  vertical axis (defined by skeleton root). */
	"sacroiliac_roll", /**< Side to side swinging of the pelvis in the coronal plane. */
	"l_hip_flexion", /**< Forward-backward rotation in the sagittal plane. */
	"r_hip_flexion", /**< Forward-backward rotation in the sagittal plane. */
	"l_hip_abduct", /**< Sideward opening in the coronal plane  */
	"r_hip_abduct", /**< Sideward opening in the coronal plane  */
	"l_hip_twisting", /**< Rotation along the thigh axis. */
	"r_hip_twisting", /**< Rotation along the thigh axis. */
	"l_knee_flexion", /**< Flexion-extension of the leg in the sagittal plane. */
	"r_knee_flexion", /**< Flexion-extension of the leg in the sagittal plane. */
	"l_knee_twisting", /**< Rotation along the shank axis. */
	"r_knee_twisting", /**< Rotation along the shank axis. */
	"l_ankle_flexion", /**< Flexion-extension of the foot  in the sagittal plane  */
	"r_ankle_flexion", /**< Flexion-extension of the foot  in the sagittal plane  */
	"l_ankle_twisting", /**< Rotation along the knee axis. */
	"r_ankle_twisting", /**< Rotation along the knee axis. */
	"l_subtalar_flexion", /**< Sideward orientation of the foot   */
	"r_subtalar_flexion", /**< Sideward orientation of the foot   */
	"l_midtarsal_twisting", /**< Internal twisting of the foot (also called navicular joint in anatomy). */
	"r_midtarsal_twisting", /**< Internal twisting of the foot (also called navicular joint in anatomy). */
	"l_metatarsal_flexion", /**< Up and down rotation of the toe in the sagittal plane. */
	"r_metatarsal_flexion", /**< Up and down rotation of the toe in the sagittal plane. */
	"l_sternoclavicular_abduct", /**< Up and down motion in the coronal plane  */
	"r_sternoclavicular_abduct", /**< Up and down motion in the coronal plane  */
	"l_sternoclavicular_rotate", /**< Rotation in the transverse plane. */
	"r_sternoclavicular_rotate", /**< Rotation in the transverse plane. */
	"l_acromioclavicular_abduct", /**< Up and down motion in the coronal plane  */
	"r_acromioclavicular_abduct", /**< Up and down motion in the coronal plane  */
	"l_acromioclavicular_rotate", /**< Rotation in the transverse plane  */
	"r_acromioclavicular_rotate", /**< Rotation in the transverse plane  */
	"l_shoulder_flexion", /**< Forward-backward motion in the sagittal plane  */
	"r_shoulder_flexion", /**< Forward-backward motion in the sagittal plane  */
	"l_shoulder_abduct", /**< Sideward motion in the coronal plane  */
	"r_shoulder_abduct", /**< Sideward motion in the coronal plane  */
	"l_shoulder_twisting", /**< Rotation along the scapular axis. */
	"r_shoulder_twisting", /**< Rotation along the scapular axis. */
	"l_elbow_flexion", /**< Flexion-extension of the arm in the sagittal plane  */
	"r_elbow_flexion", /**< Flexion-extension of the arm in the sagittal plane  */
	"l_elbow_twisting", /**< Rotation of the forearm along the upper arm axis. */
	"r_elbow_twisting", /**< Rotation of the forearm along the upper arm axis. */
	"l_wrist_flexion", /**< Rotation of the hand in the coronal plane  */
	"r_wrist_flexion", /**< Rotation of the hand in the coronal plane  */
	"l_wrist_pivot", /**< Rotation of the hand in the sagittal planes. */
	"r_wrist_pivot", /**< Rotation of the hand in the sagittal planes. */
	"l_wrist_twisting", /**< Rotation of the hand along the forearm axis. */
	"r_wrist_twisting", /**< Rotation of the hand along the forearm axis. */
	"skullbase_roll", /**< Sideward motion of the skull along the frontal axis. */
	"skullbase_torsion", /**< Twisting of the skull along the vertical axis. */
	"skullbase_tilt", /**< Forward-backward motion in the sagittal plane along a lateral axis  */
	"vc1_roll", /**< Sideward motion of vertebra C1  */
	"vc1_torsion", /**< Twisting of vertebra C1  */
	"vc1_tilt", /**< Forward-backward motion of vertebra C1 in the sagittal plane. */ 
	"vc2_roll", /**< Sideward motion of vertebra C2  */
	"vc2_torsion", /**< Twisting of vertebra C2  */
	"vc2_tilt", /**< Forward-backward motion of vertebra C2 in the sagittal plane  */
	"vc3_roll", /**< Sideward motion of vertebra C3  */
	"vc3_torsion", /**< Twisting of vertebra C3  */
	"vc3_tilt", /**< Forward-backward motion of vertebra C3 in the sagittal plane  */
	"vc4_roll", /**< Sideward motion of vertebra C4  */
	"vc4_torsion", /**< Twisting of vertebra C4  */
	"vc4_tilt", /**< Forward-backward motion of vertebra C4 in the sagittal plane  */
	"vc5_roll", /**< Sideward motion of vertebra C5  */
	"vc5_torsion", /**< Twisting of vertebra C5  */
	"vc5_tilt", /**< Forward-backward motion of vertebra C5 in the sagittal plane  */
	"vc6_roll", /**< Sideward motion of vertebra C6  */
	"vc6_torsion", /**< Twisting of vertebra C6  */
	"vc6_tilt", /**< Forward-backward motion of vertebra C6 in the sagittal plane  */
	"vc7_roll", /**< Sideward motion of vertebra C7  */
	"vc7_torsion", /**< Twisting of vertebra C7  */
	"vc7_tilt", /**< Forward-backward motion of vertebra C7 in the sagittal plane  */
	"vt1_roll", /**< Sideward motion of vertebra T1  */
	"vt1_torsion", /**< Twisting of vertebra T1  */
	"vt1_tilt", /**< Forward-backward motion of vertebra T1 in the sagittal plane  */
	"vt2_roll", /**< Sideward motion of vertebra T2  */
	"vt2_torsion", /**< Twisting of vertebra T2  */
	"vt2_tilt", /**< Forward-backward motion of vertebra T2 in the sagittal plane  */
	"vt3_roll", /**< Sideward motion of vertebra T3  */
	"vt3_torsion", /**< Twisting of vertebra T3  */
	"vt3_tilt", /**< Forward-backward motion of vertebra T3 in the sagittal plane  */
	"vt4_roll", /**< Sideward motion of vertebra T4  */
	"vt4_torsion", /**< Twisting of vertebra T4  */
	"vt4_tilt", /**< Forward-backward motion of vertebra T4 in the sagittal plane  */
	"vt5_roll", /**< Sideward motion of vertebra T5  */
	"vt5_torsion", /**< Twisting of vertebra T5  */
	"vt5_tilt", /**< Forward-backward motion of vertebra T5 in the sagittal plane  */
	"vt6_roll", /**< Sideward motion of vertebra T6  */
	"vt6_torsion", /**< Twisting of vertebra T6  */
	"vt6_tilt", /**< Forward-backward motion of vertebra T6 in the sagittal plane  */
	"vt7_roll", /**< Sideward motion of vertebra T7  */
	"vt7_torsion", /**< Twisting of vertebra T7  */
	"vt7_tilt", /**< Forward-backward motion of vertebra T7 in the sagittal plane  */
	"vt8_roll", /**< Sideward motion of vertebra T8  */
	"vt8_torsion", /**< Twisting of vertebra T8  */
	"vt8_tilt", /**< Forward-backward motion of vertebra T8 in the sagittal plane  */
	"vt9_roll", /**< Sideward motion of vertebra T9  */
	"vt9_torsion", /**< Twisting of vertebra T9  */
	"vt9_tilt", /**< Forward-backward motion of vertebra T9 in the sagittal plane  */
	"vt_10_roll", /**< Sideward motion of vertebra T10  */
	"vt10_torsion", /**< Twisting of vertebra T10  */
	"vt10_tilt", /**< Forward-backward motion of vertebra T10 in sagittal plane  */
	"vt11_roll", /**< Sideward motion of vertebra T11  */
	"vt11_torsion", /**< Twisting of vertebra T11  */
	"vt11_tilt", /**< Forward-backward motion of vertebra T11 in sagittal plane  */
	"vt12_roll", /**< Sideward motion of vertebra T12  */
	"vt12_torsion", /**< Twisting of vertebra T12  */
	"vt12_tilt", /**< Forward-backward motion of vertebra T12 in sagittal plane  */
	"vl1_roll", /**< Sideward motion of vertebra L1  */
	"vl1_torsion", /**< Twisting of vertebra L1  */
	"vl1_tilt", /**< Forward-backward motion of vertebra L1 in sagittal plane  */
	"vl2_roll", /**< Sideward motion of vertebra L2  */
	"vl2_torsion", /**< Twisting of vertebra L2  */
	"vl2_tilt", /**< Forward-backward motion of vertebra L2 in sagittal plane  */
	"vl3_roll", /**< Sideward motion of vertebra L3  */
	"vl3_torsion", /**< Twisting of vertebra L3  */
	"vl3_tilt", /**< Forward-backward motion of vertebra L3 in sagittal plane  */
	"vl4_roll", /**< Sideward motion of vertebra L4  */
	"vl4_torsion", /**< Twisting of vertebra L4  */
	"vl4_tilt", /**< Forward-backward motion of vertebra L4 in sagittal plane  */
	"vl5_roll", /**< Sideward motion of vertebra L5  */
	"vl5_torsion", /**< Twisting of vertebra L5  */
	"vl5_tilt", /**< Forward-backward motion of vertebra L5 in sagittal plane  */
	"l_pinky0_flexion", /**< Metacarpal flexing mobility of the pinky finger. */
	"r_pinky0_flexion", /**< Metacarpal flexing mobility of the pinky finger. */
	"l_pinky1_flexion", /**< First knukle of the pinky finger. */
	"r_pinky1_flexion", /**< First knukle of the pinky finger. */
	"l_pinky1_pivot", /**< Lateral mobility of the pinky finger. */
	"r_pinky1_pivot", /**< Lateral mobility of the pinky finger. */
	"l_pinky1_twisting", /**< Along the pinky finger axis. */
	"r_pinky1_twisting", /**< Along the pinky finger axis. */
	"l_pinky2_flexion", /**< Second knuckle of the pinky number  */
	"r_pinky2_flexion", /**< Second knuckle of the pinky number  */
	"l_pinky3_flexion", /**< Third knuckle of the pinky finger. */
	"r_pinky3_flexion", /**< Third knuckle of the pinky finger. */
	"l_ring0_flexion", /**< Metacarpal flexing mobility of the ring finger. */
	"r_ring0_flexion", /**< Metacarpal flexing mobility of the ring finger. */
	"l_ring1_flexion", /**< First knukle of the ring finger. */
	"r_ring1_flexion", /**< First knukle of the ring finger. */
	"l_ring1_pivot", /**< Lateral mobility of the ring finger. */
	"r_ring1_pivot", /**< Lateral mobility of the ring finger. */
	"l_ring1_twisting", /**< Along the ring finger axis. */
	"r_ring1_twisting", /**< Along the ring finger axis. */
	"l_ring2_flexion", /**< 	Second knuckle of the ring number  */
	"r_ring2_flexion", /**< 	Second knuckle of the ring number  */
	"l_ring3_flexion", /**< 	Third knuckle of the ring finger. */
	"r_ring3_flexion", /**< 	Third knuckle of the ring finger. */
	"l_middle0_flexion", /**< Metacarpal flexing mobility of the middle finger. */
	"r_middle0_flexion", /**< Metacarpal flexing mobility of the middle finger. */
	"l_middle1_flexion", /**< First knukle of the middle finger. */
	"r_middle1_flexion", /**< First knukle of the middle finger. */
	"l_middle1_pivot", /**< 	Lateral mobility of the middle finger. */
	"r_middle1_pivot", /**< 	Lateral mobility of the middle finger. */
	"l_middle1_twisting", /**< Along the middle finger axis. */
	"r_middle1_twisting", /**< Along the middle finger axis. */
	"l_middle2_flexion", /**< Second knuckle of the middle number  */
	"r_middle2_flexion", /**< Second knuckle of the middle number  */
	"l_middle3_flexion", /**< Third knuckle of the middle finger. */
	"r_middle3_flexion", /**< Third knuckle of the middle finger. */
	"l_index0_flexion", /**< Metacarpal flexing mobility of the index finger. */
	"r_index0_flexion", /**< Metacarpal flexing mobility of the index finger. */
	"l_index1_flexion", /**< First knukle of the index finger. */
	"r_index1_flexion", /**< First knukle of the index finger. */
	"l_index1_pivot", /**< Lateral mobility of the index finger. */
	"r_index1_pivot", /**< Lateral mobility of the index finger. */
	"l_index1_twisting", /**< Along the index finger axis. */
	"r_index1_twisting", /**< Along the index finger axis. */
	"l_index2_flexion", /**< Second knuckle of the index number  */
	"r_index2_flexion", /**< Second knuckle of the index number  */
	"l_index3_flexion", /**< Third knuckle of the index finger. */
	"r_index3_flexion", /**< Third knuckle of the index finger. */
	"l_thumb1_flexion", /**< First knukle of the thumb finger. */
	"r_thumb1_flexion", /**< First knukle of the thumb finger. */
	"l_thumb1_pivot", /**< Lateral mobility of the thumb finger. */
	"r_thumb1_pivot", /**< Lateral mobility of the thumb finger. */
	"l_thumb1_twisting", /**< Along the thumb finger axis. */
	"r_thumb1_twisting", /**< Along the thumb finger axis. */
	"l_thumb2_flexion", /**< Second knuckle of the thumb number  */
	"r_thumb2_flexion", /**< Second knuckle of the thumb number  */
	"l_thumb3_flexion", /**< Third knuckle of the thumb finger. */
	"r_thumb3_flexion", /**< Third knuckle of the thumb finger. */
	"HumanoidRoot_tr_vertical", /**< Body origin translation in vertical direction. */
	"HumanoidRoot_tr_lateral", /**< 	Body origin translation in lateral direction. */
	"HumanoidRoot_tr_frontal", /**< 	Body origin translation in frontal direction. */
	"HumanoidRoot_rt_body_turn", /**< Rotation of the skeleton root along the body coordinate system's vertical axis. */
	"HumanoidRoot_rt_body_roll", /**< Rotation of the skeleton root along the body coordinate system's frontal axis. */
	"HumanoidRoot_rt_body_tilt" /**< Rotation of the skeleton root along the body coordinate system's lateral axis. */
};

/**
* FAP names as strings.
*
* FAP names in an array of strings. @ref FAPNames enumerator can be used to access each name.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* \ingroup FBAPNames
*/
static char FAPNameString[NFAP][50]=
{
	"fap1_viseme", /**< FAP 1 - Viseme. */
	"fap2_expression", /**< FAP 2 - Expression. */
	"open_jaw", /**< Vertical jaw displacement (does not affect mouth opening). */
	"lower_t_midlip", /**< Vertical top middle inner lip displacement. */
	"raise_b_midlip", /**< Vertical  bottom middle inner lip displacement. */
	"stretch_l_cornerlip", /**< Horizontal displacement of left inner lip corner. */
	"stretch_r_cornerlip", /**< Horizontal displacement of right inner lip corner. */
	"lower_t_lip_lm", /**< Vertical  displacement of midpoint between left corner and middle of top inner lip. */
	"lower_t_lip_rm", /**< Vertical  displacement of midpoint between right corner and middle of top inner lip. */
	"raise_b_lip_lm", /**< Vertical  displacement of midpoint between left corner and middle of bottom inner lip. */
	"raise_b_lip_rm", /**< Vertical  displacement of midpoint between right corner and middle of bottom inner lip. */
	"raise_l_cornerlip", /**< Vertical  displacement of left inner lip corner. */
	"raise_r_cornerlip", /**< Vertical  displacement of right inner lip corner. */
	"thrust_jaw", /**< Depth displacement of jaw. */
	"shift_jaw", /**< Side to side displacement of jaw. */
	"push_b_lip", /**< Depth displacement of bottom middle lip. */
	"push_t_lip", /**< Depth displacement of top middle lip. */
	"depress_chin", /**< Upward and compressing movement of the chin (like in sadness). */
	"close_t_l_eyelid", /**< Vertical displacement of top left eyelid. */
	"close_t_r_eyelid", /**< Vertical displacement of top right eyelid. */
	"close_b_l_eyelid", /**< Vertical displacement of bottom left eyelid. */
	"close_b_r_eyelid", /**< Vertical displacement of bottom right eyelid. */
	"yaw_l_eyeball", /**< Horizontal orientation of left eyeball; NOTE: the unit for rotation in 1e-5 rad.*/
	"yaw_r_eyeball", /**< Horizontal orientation of right eyeball; NOTE: the unit for rotation in 1e-5 rad. */
	"pitch_l_eyeball",  /**< Vertical orientation of left eyeball; NOTE: the unit for rotation in 1e-5 rad. */
	"pitch_r_eyeball", /**< Vertical orientation of right eyeball; NOTE: the unit for rotation in 1e-5 rad. */ 
	"thrust_l_eyeball", /**< Depth displacement of left eyeball. */
	"thrust_r_eyebal",  /**< Depth displacement of right eyeball. */
	"dilate_l_pupil", /**< Dilation of left pupil. */
	"dilate_r_pupil", /**< Dilation of right pupil. */
	"raise_l_i_eyebrow", /**< Vertical displacement of left inner eyebrow. */
	"raise_r_i_eyebrow", /**< Vertical displacement of right inner eyebrow. */
	"raise_l_m_eyebrow", /**< Vertical displacement of left middle eyebrow. */
	"raise_r_m_eyebrow", /**< Vertical displacement of right middle eyebrow. */
	"raise_l_o_eyebrow", /**< Vertical displacement of left outer eyebrow. */
	"raise_r_o_eyebrow", /**< Vertical displacement of right outer eyebrow. */
	"squeeze_l_eyebrow", /**< Horizontal displacement of left eyebrow. */
	"squeeze_r_eyebrow", /**< Horizontal displacement of right eyebrow. */
	"puff_l_cheek", /**< Horizontal displacement of  left cheeck. */
	"puff_r_cheek", /**< Horizontal displacement of right cheeck. */
	"lift_l_cheek", /**< Vertical displacement of left cheek. */
	"lift_r_cheek", /**< Vertical displacement of right cheek. */
	"shift_tongue_tip", /**< Horizontal displacement of tongue tip. */
	"raise_tongue_tip", /**< Vertical displacement of tongue tip. */
	"thrust_tongue_tip", /**< Depth displacement of tongue tip. */
	"raise_tongue", /**< Vertical displacement of tongue. */
	"tongue_roll", /**< Rolling of the tongue into U shape; NOTE: the unit for rotation in 1e-5 rad. */
	"head_pitch", /**< Head pitch angle from top of spine; NOTE: the unit for rotation in 1e-5 rad. */
	"head_yaw", /**< Head yaw angle from top of spine; NOTE: the unit for rotation in 1e-5 rad. */
	"head_roll", /**< Head roll angle from top of spine; NOTE: the unit for rotation in 1e-5 rad. */
	"lower_t_midlip_o", /**< Vertical top middle outer lip displacement. */
	"raise_b_midlip_o", /**< Vertical  bottom middle outer lip displacement. */
	"stretch_l_cornerlip_o", /**< Horizontal displacement of left outer lip corner. */
	"stretch_r_cornerlip_o", /**< Horizontal displacement of right outer lip corner. */
	"lower_t_lip_lm_o", /**< Vertical  displacement of midpoint between left corner and middle of top outer lip. */
	"lower_t_lip_rm_o", /**< Vertical  displacement of midpoint between right corner and middle of top outer lip. */
	"raise_b_lip_lm_o", /**< Vertical  displacement of midpoint between left corner and middle of bottom outer lip. */
	"raise_b_lip_rm_o", /**< Vertical  displacement of midpoint between right corner and middle of bottom outer lip. */
	"raise_l_cornerlip_o", /**< Vertical  displacement of left outer lip corner. */
	"raise_r_cornerlip_o", /**< Vertical  displacement of right outer lip corner. */
	"stretch_l_nose", /**< Horizontal displacement of left side of nose. */
	"stretch_r_nose", /**< Horizontal displacement of right side of nose. */
	"raise_nose", /**< Vertical displacement of nose tip. */
	"bend_nose", /**< Horizontal displacement of nose tip. */
	"raise_l_ear", /**< Vertical displacement of left ear. */
	"raise_r_ear", /**< Vertical displacement of right ear. */
	"pull_l_ear", /**< Horizontal displacement of left ear. */
	"pull_r_ear"   /**< Horizontal displacement of right ear. */ 
};

/**
* FAP names defined as constants.
*
* FAP names defined as constants corresponding to the low level FAP indices. This provides a convenient way to
* access low level FAPs.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* @see FBAPs
* @see FAPs
* @see LLFAPs
*
* \ingroup FBAPNames
*/
enum FAPNames
{
		fap1_viseme, /**< FAP 1 - Viseme. */
		fap2_expression, /**< FAP 2 - Expression. */
		open_jaw, /**< Vertical jaw displacement (does not affect mouth opening). */
		lower_t_midlip, /**< Vertical top middle inner lip displacement. */
		raise_b_midlip, /**< Vertical  bottom middle inner lip displacement. */
		stretch_l_cornerlip, /**< Horizontal displacement of left inner lip corner. */
		stretch_r_cornerlip, /**< Horizontal displacement of right inner lip corner. */
		lower_t_lip_lm, /**< Vertical  displacement of midpoint between left corner and middle of top inner lip. */
		lower_t_lip_rm, /**< Vertical  displacement of midpoint between right corner and middle of top inner lip. */
		raise_b_lip_lm, /**< Vertical  displacement of midpoint between left corner and middle of bottom inner lip. */
		raise_b_lip_rm, /**< Vertical  displacement of midpoint between right corner and middle of bottom inner lip. */
		raise_l_cornerlip, /**< Vertical  displacement of left inner lip corner. */
		raise_r_cornerlip, /**< Vertical  displacement of right inner lip corner. */
		thrust_jaw, /**< Depth displacement of jaw. */
		shift_jaw, /**< Side to side displacement of jaw. */
		push_b_lip, /**< Depth displacement of bottom middle lip. */
		push_t_lip, /**< Depth displacement of top middle lip. */
		depress_chin, /**< Upward and compressing movement of the chin (like in sadness). */
		close_t_l_eyelid, /**< Vertical displacement of top left eyelid. */
		close_t_r_eyelid, /**< Vertical displacement of top right eyelid. */
		close_b_l_eyelid, /**< Vertical displacement of bottom left eyelid. */
		close_b_r_eyelid, /**< Vertical displacement of bottom right eyelid. */
		yaw_l_eyeball, /**< Horizontal orientation of left eyeball; NOTE: the unit for rotation in 1e-5 rad.*/
		yaw_r_eyeball, /**< Horizontal orientation of right eyeball; NOTE: the unit for rotation in 1e-5 rad. */
		pitch_l_eyeball,  /**< Vertical orientation of left eyeball; NOTE: the unit for rotation in 1e-5 rad. */
		pitch_r_eyeball, /**< Vertical orientation of right eyeball; NOTE: the unit for rotation in 1e-5 rad. */ 
		thrust_l_eyeball, /**< Depth displacement of left eyeball. */
		thrust_r_eyebal,  /**< Depth displacement of right eyeball. */
		dilate_l_pupil, /**< Dilation of left pupil. */
		dilate_r_pupil, /**< Dilation of right pupil. */
		raise_l_i_eyebrow, /**< Vertical displacement of left inner eyebrow. */
		raise_r_i_eyebrow, /**< Vertical displacement of right inner eyebrow. */
		raise_l_m_eyebrow, /**< Vertical displacement of left middle eyebrow. */
		raise_r_m_eyebrow, /**< Vertical displacement of right middle eyebrow. */
		raise_l_o_eyebrow, /**< Vertical displacement of left outer eyebrow. */
		raise_r_o_eyebrow, /**< Vertical displacement of right outer eyebrow. */
		squeeze_l_eyebrow, /**< Horizontal displacement of left eyebrow. */
		squeeze_r_eyebrow, /**< Horizontal displacement of right eyebrow. */
		puff_l_cheek, /**< Horizontal displacement of  left cheeck. */
		puff_r_cheek, /**< Horizontal displacement of right cheeck. */
		lift_l_cheek, /**< Vertical displacement of left cheek. */
		lift_r_cheek, /**< Vertical displacement of right cheek. */
		shift_tongue_tip, /**< Horizontal displacement of tongue tip. */
		raise_tongue_tip, /**< Vertical displacement of tongue tip. */
		thrust_tongue_tip, /**< Depth displacement of tongue tip. */
		raise_tongue, /**< Vertical displacement of tongue. */
		tongue_roll, /**< Rolling of the tongue into U shape; NOTE: the unit for rotation in 1e-5 rad. */
		head_pitch, /**< Head pitch angle from top of spine; NOTE: the unit for rotation in 1e-5 rad. */
		head_yaw, /**< Head yaw angle from top of spine; NOTE: the unit for rotation in 1e-5 rad. */
		head_roll, /**< Head roll angle from top of spine; NOTE: the unit for rotation in 1e-5 rad. */
		lower_t_midlip_o, /**< Vertical top middle outer lip displacement. */
		raise_b_midlip_o, /**< Vertical  bottom middle outer lip displacement. */
		stretch_l_cornerlip_o, /**< Horizontal displacement of left outer lip corner. */
		stretch_r_cornerlip_o, /**< Horizontal displacement of right outer lip corner. */
		lower_t_lip_lm_o, /**< Vertical  displacement of midpoint between left corner and middle of top outer lip. */
		lower_t_lip_rm_o, /**< Vertical  displacement of midpoint between right corner and middle of top outer lip. */
		raise_b_lip_lm_o, /**< Vertical  displacement of midpoint between left corner and middle of bottom outer lip. */
		raise_b_lip_rm_o, /**< Vertical  displacement of midpoint between right corner and middle of bottom outer lip. */
		raise_l_cornerlip_o, /**< Vertical  displacement of left outer lip corner. */
		raise_r_cornerlip_o, /**< Vertical  displacement of right outer lip corner. */
		stretch_l_nose, /**< Horizontal displacement of left side of nose. */
		stretch_r_nose, /**< Horizontal displacement of right side of nose. */
		raise_nose, /**< Vertical displacement of nose tip. */
		bend_nose, /**< Horizontal displacement of nose tip. */
		raise_l_ear, /**< Vertical displacement of left ear. */
		raise_r_ear, /**< Vertical displacement of right ear. */
		pull_l_ear, /**< Horizontal displacement of left ear. */
		pull_r_ear   /**< Horizontal displacement of right ear. */ 
};                

/**
* MPEG-4 facial expression names as strings.
*
* MPEG-4 facial expression names in an array of strings. @ref ExpressionNames enumerator can be used to access each name.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* \ingroup FBAPNames
*/
static char ExpressionNameString[NEXP][50]=
{
	"expression_neutral", /**< Neutral face, no expression. */
	"expression_joy", /**< Joy expression. The eyebrows are relaxed. The mouth is open and the mouth corners pulled back toward the ears. */
	"expression_sadness", /**<  Sadness expression. The inner eyebrows are bent upward. The eyes are slightly closed. The mouth is relaxed. */
	"expression_anger", /**<  Anger expression. The inner eyebrows are pulled downward and together. The eyes are wide open. The lips are pressed against each other or opened to expose the teeth. */
	"expression_fear", /**<  Fear expression. The eyebrows are raised and pulled together. The inner eyebrows are bent upward. The eyes are tense and alert. */
	"expression_disgust", /**<  Disgust expression. The eyebrows and eyelids are relaxed. The upper lip is raised and curled, often asymmetrically. */
	"expression_surprise" /**<  Surprise expression. The eyebrows are raised. The upper eyelids are wide open, the lower relaxed. The jaw is opened. */
};


/**
* MPEG-4 Facial expression names defined as constants.
*
* Facial expression names defined as constants corresponding to the expression indices. This provides a convenient way to
* access expressions.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* @see FBAPs
* @see FAPs
* @see FAP2
*
* \ingroup FBAPNames
*/
enum ExpressionNames
{
	expression_neutral, /**< Neutral face, no expression. */
	expression_joy, /**< Joy expression. The eyebrows are relaxed. The mouth is open and the mouth corners pulled back toward the ears. */
	expression_sadness, /**<  Sadness expression. The inner eyebrows are bent upward. The eyes are slightly closed. The mouth is relaxed. */
	expression_anger, /**<  Anger expression. The inner eyebrows are pulled downward and together. The eyes are wide open. The lips are pressed against each other or opened to expose the teeth. */
	expression_fear, /**<  Fear expression. The eyebrows are raised and pulled together. The inner eyebrows are bent upward. The eyes are tense and alert. */
	expression_disgust, /**<  Disgust expression. The eyebrows and eyelids are relaxed. The upper lip is raised and curled, often asymmetrically. */
	expression_surprise /**<  Surprise expression. The eyebrows are raised. The upper eyelids are wide open, the lower relaxed. The jaw is opened. */
};

/**
* MPEG-4 viseme names as strings.
*
* MPEG-4 viseme names in an array of strings. @ref VisemeNames enumerator can be used to access each name.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* \ingroup FBAPNames
*/
static char VisemeNameString[NVISEME][50]=
{
"viseme_sil", /**< none, neutral face */
"viseme_PP", /**< p, b, m, as in words Put, bed, mill */
"viseme_FF", /**< f, v, as in words Far, voice */
"viseme_TH", /**< T, D, as in words Think, that */
"viseme_DD", /**< t, d, as in words Tip, doll */
"viseme_kk", /**< k, g, as in words Call, gas */
"viseme_CH", /**< tS, dZ, S, as in words Chair, join, she */
"viseme_SS", /**< s, z, as in words Sir, zeal */
"viseme_nn", /**< n, l, as in words Lot, not */
"viseme_RR", /**< r, as in words Red */
"viseme_aa", /**< A:, as in words Car */
"viseme_E", /**< e, as in words Bed */
"viseme_ih", /**< I, as in words Tip */
"viseme_oh", /**< Q, as in words Top */
"viseme_ou" /**< U, as in words book */
};

/**
* MPEG-4 viseme names defined as constants.
*
* Viseme names defined as constants corresponding to the viseme indices. This provides a convenient way to
* access visemes.
* For more details about the parameters please refer to <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
*
* @see FBAPs
* @see FAPs
* @see FAP1
*
* \ingroup FBAPNames
*/
enum VisemeNames
{
viseme_sil, /**< none, neutral face */
viseme_PP, /**< p, b, m, as in words Put, bed, mill */
viseme_FF, /**< f, v, as in words Far, voice */
viseme_TH, /**< T, D, as in words Think, that */
viseme_DD, /**< t, d, as in words Tip, doll */
viseme_kk, /**< k, g, as in words Call, gas */
viseme_CH, /**< tS, dZ, S, as in words Chair, join, she */
viseme_SS, /**< s, z, as in words Sir, zeal */
viseme_nn, /**< n, l, as in words Lot, not */
viseme_RR, /**< r, as in words Red */
viseme_aa, /**< A:, as in words Car */
viseme_E, /**< e, as in words Bed */
viseme_ih, /**< I, as in words Tip */
viseme_oh, /**< Q, as in words Top */
viseme_ou /**< U, as in words book */
};

using namespace std;

/**
* Face and Body Animation Parameters.
* The storage class for the MPEG-4 Face and Body Animation Parameters (FAPs and BAPs) with high-level convenience functions
* for setting and manipulating the parameters. 
* A summary of the MPEG-4 %FBAPs specification can be found in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. 
*
* Implemented in visagefbadecoder.lib
*
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/SimplePlayer/doc/index.html">Example1</a>, <a href="../../../Samples/OpenGL/build/msvc90/SpeechSynthesis/doc/index.html">Example6</a>, and <a href="../../../Samples/OpenGL/build/msvc90/LipSync/doc/index.html">Example7</a>.
*
*/
class VISAGE_DECLSPEC FBAPs  
{
public:

	/**
	* Constructor.
	* Constructs FBAPs with a neutral facial expression.
	*/
	FBAPs();

	/**
	* Constructor.
	* Constructs FBAPs by copying given FBAPs.
	* @param fbaps the FBAPs to be copied.
	*/
	FBAPs(const FBAPs *fbaps);


	/**
	* Constructor.
	* Constructs FBAPs by copying given FBAPs, then multiplying them by the factor in.
	* For the low level faps and for the expressions, the factor multiplies the intensity. 
	* The visemes and BAPs are just copied, the multiplication factor is ignored for them.
	* @param fbaps the FBAPs to be copied.
	* @param in the multiplication factor.
	*/
	FBAPs(FBAPs *fbaps, double in);

	/**
	* Destructor.
	*/
	virtual ~FBAPs();

	/**
	* Reset the FBAPs.
	* Reset the FBAPs to initial values, corresponding to the neutral position of the face and body.
	*/
	void reset();

	/**
	* Set the FBAPs.
	* Set FBAPs by copying the given FBAPs.
	* @param fbaps the FBAPs to be copied.
	*/
	void set(const FBAPs *fbaps);

	/**
	* Set the FBAPs.
	* Set FBAPs by copying the given FBAPs, then multiplying the FAPs by the factor in.
	* For the low level faps and for the expressions, the factor multiplies the intensity. 
	* The visemes and BAPs are just copied, the multiplication factor is ignored for them.
	* @param fbaps the FBAPs to be copied.
	* @param in the multiplication factor.
	*/
	void set(FBAPs *fbaps, double in);

	/**
	* Set the FAPs.
	* Set FAPs by copying the given FAPs.
	* @param _faps the faps to be copied.
	*/
	void setFAPs(const FAPs *_faps);

	/**
	* Set the BAPs.
	* Set BAPs by copying the given BAPs.
	* @param _baps the baps to be copied.
	*/
	void setBAPs(const BAPs *_baps);

	/**
	* Set facial expression (FAP2).
	* A convenience method to set the facial expression. Following the MPEG-4 specification, the expression
	* is a blend of two expressions, each with its own intensity.
	* @param expression1 the expression 1. Use the enumerator ExpressionNames to set the expression.
	* @param expression2 the expression 2. Use the enumerator ExpressionNames to set the expression.
	* @param intensity1 the intensity of the expression 1; the valid range is 0 to 63.
	* @param intensity2 the intensity of the expression 2; the valid range is 0 to 63.
	* @see ExpressionNames
	* @see FAP2
	*/
	void setExpression(int expression1, int expression2,int intensity1, int intensity2);

	/**
	* Set the viseme (FAP1).
	* A convenience method to set the viseme. Following the MPEG-4 specification, the expression
	* is a blend of two visemes, using a blend factor.
	* @param viseme1 the viseme 1. Use the enumerator VisemeNames to set the viseme.
	* @param viseme2 the viseme 2. Use the enumerator VisemeNames to set the viseme.
	* @param blend the viseme blend; the valid range is 0 to 63; the value 63 means that viseme 1 is fully applied, and viseme 2 is not applied at all; value 0 is vice versa; value of 32 is an equal blend of visemes 1 and 2.
	* @see VisemeNames
	* @see FAP1
	*/
	void setViseme(int viseme1, int viseme2,int blend);

	/**
	* Set a low-level FAP.
	* A convenience method to set a low-level Face Animation Parameter.
	*  Note that the rotation unit for rotation FAPs (e.g. head_pitch) is 1E-5 radians, so typical values are fairly big.
	* @param fap the fap to be set. Use the FAPNames enumerator for convenience, e.g. setFAPs(open_jaw, 100)
	* @param value the value of the fap to be set.
	* @see FAPNames
	* @see LLFAPs
	*/
	void setFAP(int fap, int value);

	/**
	* Set a BAP.
	* A convenience method to set a Body Animation Parameter. Note that the rotation unit is 1E-5 radians, so typical values are fairly big.
	* @param bap the bap to be set. Use the BAPNames enumerator for convenience, e.g. setBAPs(r_shoulder_flexion, 10000)
	* @param value the value of the bap to be set.
	* @see BAPNames
	*/
	void setBAP(int bap, int value);

	/**
	* Set a low-level FAP or BAP.
	* A convenience method to set a low-level Face Animation Parameter or a Body Animation Parameter by passing the name of the parameter. The parameter name can be any of the parameter names defined in @ref FAPNameString or @ref BAPNameString, e.g "open_jaw", "  sacroiliac_tilt" etc.
	*
	* N.B. This function involves searching for the parameter name and is therefore slower than setFAP and setBAP functions; therefore, if speed is essential, it is recommended to use setFAP and setBAP.
	*  
	* @param fbap the name of FAP or BAP to be set. Use the FAPNames enumerator for convenience, e.g. setFAPs(open_jaw, 100)
	* @param value the value of the fap to be set.
	* @see FAPNames
	* @see BAPNames
	* @see FAPNameString
	* @see BAPNameString
	*/
	void setFBAP(char *fbap, int value);

	/**
	* Get a low-level FAP.
	* A convenience method to get the value of a low-level Face Animation Parameter.
	* @param fap the fap whose value we want. Use the FAPNames enumerator for convenience, e.g. getFAPs(open_jaw)
	* @see FAPNames
	*/
	int getFAP(int fap);

	/**
	* Get a BAP.
	* A convenience method to get the value of a Body Animation Parameter.
	* @param bap the bap whose value we want. Use the BAPNames enumerator for convenience, e.g. getBAPs(r_shoulder_flexion)
	* @see BAPNames
	*/
	int getBAP(int bap);

	/**
	* Merge FBAPs.
	* Merge FBAPs with the given FBAPs. This is done by merging their respective FAPs and BAPs. 
	* For details on the merging operation see the merge() functions
	* in FAPs and BAPs classes.
	*
	* @param fbaps the fbaps to be merged with this FBAPs.
	*
	* @see FAPs::merge(), BAPs::merge()
	*/
	void merge(FBAPs *fbaps);

	/**
	* Set the mask.
	* Set the mask values (groupMask) for all animation parameters to 1 if maskValue is true, or to 0 if maskValue is false.
	* maskType for all groups is set to 1.
	*
	* @param maskValue the value for the mask to be set - 0 if false, 1 if true.
	*
	* @see FAPs::maskType
	* @see FAPs::groupMask
	* @see BAPs::maskType
	* @see BAPs::groupMask
	*/
	void setMask(bool maskValue);

	/**
	* Set the mask conditionally.
	* Set the mask values (groupMask) to 1 only for animation parameters 
	* whose value is different in the fbaps passed as parameter. Other mask values are not touched. This
	* can be useful for the optimization of FBAPs coding by setting optimal mask values.
	*
	* @param fbaps the FBAPs used for comparison.
	*
	* @see FAPs::maskType
	* @see FAPs::groupMask
	* @see BAPs::maskType
	* @see BAPs::groupMask
	*/
	void setMaskIfDiff(FBAPs *fbaps);

	/**
	* Set the mask values.
	* Set the mask values (maskType and groupMask) by copying them from fbaps.
	*
	* @param fbaps the FBAPs from which mask values will be copied.
	*
	* @see FAPs::maskType
	* @see FAPs::groupMask
	* @see BAPs::maskType
	* @see BAPs::groupMask
	*/
	void setMask(FBAPs *fbaps);

	/**
	* FAPs.
	* Facial Animation Parameters.
	* @see _FAPs
	*/
	FAPs *faps; 

	/**
	* BAPs.
	* Body Animation Parameters.
	* @see _BAPs
	*/
	BAPs *baps; 


};

}

#endif // __FBAPs_h__
