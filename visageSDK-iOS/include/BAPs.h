
#ifndef __BAPS_h__
#define __BAPS_h__

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

static char BAPJointName[NUM_BAPS][30]=
{
	"sacroiliac", /**< Forward-backward motion of the pelvis in the sagittal plane. */
		"sacroiliac", /**< Rotation of the pelvis along the body  vertical axis (defined by skeleton root). */
		"sacroiliac", /**< Side to side swinging of the pelvis in the coronal plane. */
		"l_hip", /**< Forward-backward rotation in the sagittal plane. */
		"r_hip", /**< Forward-backward rotation in the sagittal plane. */
		"l_hip", /**< Sideward opening in the coronal plane  */
		"r_hip", /**< Sideward opening in the coronal plane  */
		"l_hip", /**< Rotation along the thigh axis. */
		"r_hip", /**< Rotation along the thigh axis. */
		"l_knee", /**< Flexion-extension of the leg in the sagittal plane. */
		"r_knee", /**< Flexion-extension of the leg in the sagittal plane. */
		"l_knee", /**< Rotation along the shank axis. */
		"r_knee", /**< Rotation along the shank axis. */
		"l_ankle", /**< Flexion-extension of the foot  in the sagittal plane  */
		"r_ankle", /**< Flexion-extension of the foot  in the sagittal plane  */
		"l_ankle", /**< Rotation along the knee axis. */
		"r_ankle", /**< Rotation along the knee axis. */
		"l_subtalar", /**< Sideward orientation of the foot   */
		"r_subtalar", /**< Sideward orientation of the foot   */
		"l_midtarsal", /**< Internal twisting of the foot (also called navicular joint in anatomy). */
		"r_midtarsal", /**< Internal twisting of the foot (also called navicular joint in anatomy). */
		"l_metatarsal", /**< Up and down rotation of the toe in the sagittal plane. */
		"r_metatarsal", /**< Up and down rotation of the toe in the sagittal plane. */
		"l_sternoclavicular", /**< Up and down motion in the coronal plane  */
		"r_sternoclavicular", /**< Up and down motion in the coronal plane  */
		"l_sternoclavicular", /**< Rotation in the transverse plane. */
		"r_sternoclavicular", /**< Rotation in the transverse plane. */
		"l_acromioclavicular", /**< Up and down motion in the coronal plane  */
		"r_acromioclavicular", /**< Up and down motion in the coronal plane  */
		"l_acromioclavicular", /**< Rotation in the transverse plane  */
		"r_acromioclavicular", /**< Rotation in the transverse plane  */
		"l_shoulder", /**< Forward-backward motion in the sagittal plane  */
		"r_shoulder", /**< Forward-backward motion in the sagittal plane  */
		"l_shoulder", /**< Sideward motion in the coronal plane  */
		"r_shoulder", /**< Sideward motion in the coronal plane  */
		"l_shoulder", /**< Rotation along the scapular axis. */
		"r_shoulder", /**< Rotation along the scapular axis. */
		"l_elbow", /**< Flexion-extension of the arm in the sagittal plane  */
		"r_elbow", /**< Flexion-extension of the arm in the sagittal plane  */
		"l_elbow", /**< Rotation of the forearm along the upper arm axis. */
		"r_elbow", /**< Rotation of the forearm along the upper arm axis. */
		"l_wrist", /**< Rotation of the hand in the coronal plane  */
		"r_wrist", /**< Rotation of the hand in the coronal plane  */
		"l_wrist", /**< Rotation of the hand in the sagittal planes. */
		"r_wrist", /**< Rotation of the hand in the sagittal planes. */
		"l_wrist", /**< Rotation of the hand along the forearm axis. */
		"r_wrist", /**< Rotation of the hand along the forearm axis. */
		"skullbase", /**< Sideward motion of the skull along the frontal axis. */
		"skullbase", /**< Twisting of the skull along the vertical axis. */
		"skullbase", /**< Forward-backward motion in the sagittal plane along a lateral axis  */
		"vc1", /**< Sideward motion of vertebra C1  */
		"vc1", /**< Twisting of vertebra C1  */
		"vc1", /**< Forward-backward motion of vertebra C1 in the sagittal plane. */ 
		"vc2", /**< Sideward motion of vertebra C2  */
		"vc2", /**< Twisting of vertebra C2  */
		"vc2", /**< Forward-backward motion of vertebra C2 in the sagittal plane  */
		"vc3", /**< Sideward motion of vertebra C3  */
		"vc3", /**< Twisting of vertebra C3  */
		"vc3", /**< Forward-backward motion of vertebra C3 in the sagittal plane  */
		"vc4", /**< Sideward motion of vertebra C4  */
		"vc4", /**< Twisting of vertebra C4  */
		"vc4", /**< Forward-backward motion of vertebra C4 in the sagittal plane  */
		"vc5", /**< Sideward motion of vertebra C5  */
		"vc5", /**< Twisting of vertebra C5  */
		"vc5", /**< Forward-backward motion of vertebra C5 in the sagittal plane  */
		"vc6", /**< Sideward motion of vertebra C6  */
		"vc6", /**< Twisting of vertebra C6  */
		"vc6", /**< Forward-backward motion of vertebra C6 in the sagittal plane  */
		"vc7", /**< Sideward motion of vertebra C7  */
		"vc7", /**< Twisting of vertebra C7  */
		"vc7", /**< Forward-backward motion of vertebra C7 in the sagittal plane  */
		"vt1", /**< Sideward motion of vertebra T1  */
		"vt1", /**< Twisting of vertebra T1  */
		"vt1", /**< Forward-backward motion of vertebra T1 in the sagittal plane  */
		"vt2", /**< Sideward motion of vertebra T2  */
		"vt2", /**< Twisting of vertebra T2  */
		"vt2", /**< Forward-backward motion of vertebra T2 in the sagittal plane  */
		"vt3", /**< Sideward motion of vertebra T3  */
		"vt3", /**< Twisting of vertebra T3  */
		"vt3", /**< Forward-backward motion of vertebra T3 in the sagittal plane  */
		"vt4", /**< Sideward motion of vertebra T4  */
		"vt4", /**< Twisting of vertebra T4  */
		"vt4", /**< Forward-backward motion of vertebra T4 in the sagittal plane  */
		"vt5", /**< Sideward motion of vertebra T5  */
		"vt5", /**< Twisting of vertebra T5  */
		"vt5", /**< Forward-backward motion of vertebra T5 in the sagittal plane  */
		"vt6", /**< Sideward motion of vertebra T6  */
		"vt6", /**< Twisting of vertebra T6  */
		"vt6", /**< Forward-backward motion of vertebra T6 in the sagittal plane  */
		"vt7", /**< Sideward motion of vertebra T7  */
		"vt7", /**< Twisting of vertebra T7  */
		"vt7", /**< Forward-backward motion of vertebra T7 in the sagittal plane  */
		"vt8", /**< Sideward motion of vertebra T8  */
		"vt8", /**< Twisting of vertebra T8  */
		"vt8", /**< Forward-backward motion of vertebra T8 in the sagittal plane  */
		"vt9", /**< Sideward motion of vertebra T9  */
		"vt9", /**< Twisting of vertebra T9  */
		"vt9", /**< Forward-backward motion of vertebra T9 in the sagittal plane  */
		"vt_10", /**< Sideward motion of vertebra T10  */
		"vt10", /**< Twisting of vertebra T10  */
		"vt10", /**< Forward-backward motion of vertebra T10 in sagittal plane  */
		"vt11", /**< Sideward motion of vertebra T11  */
		"vt11", /**< Twisting of vertebra T11  */
		"vt11", /**< Forward-backward motion of vertebra T11 in sagittal plane  */
		"vt12", /**< Sideward motion of vertebra T12  */
		"vt12", /**< Twisting of vertebra T12  */
		"vt12", /**< Forward-backward motion of vertebra T12 in sagittal plane  */
		"vl1", /**< Sideward motion of vertebra L1  */
		"vl1", /**< Twisting of vertebra L1  */
		"vl1", /**< Forward-backward motion of vertebra L1 in sagittal plane  */
		"vl2", /**< Sideward motion of vertebra L2  */
		"vl2", /**< Twisting of vertebra L2  */
		"vl2", /**< Forward-backward motion of vertebra L2 in sagittal plane  */
		"vl3", /**< Sideward motion of vertebra L3  */
		"vl3", /**< Twisting of vertebra L3  */
		"vl3", /**< Forward-backward motion of vertebra L3 in sagittal plane  */
		"vl4", /**< Sideward motion of vertebra L4  */
		"vl4", /**< Twisting of vertebra L4  */
		"vl4", /**< Forward-backward motion of vertebra L4 in sagittal plane  */
		"vl5", /**< Sideward motion of vertebra L5  */
		"vl5", /**< Twisting of vertebra L5  */
		"vl5", /**< Forward-backward motion of vertebra L5 in sagittal plane  */
		"l_pinky0", /**< Metacarpal flexing mobility of the pinky finger. */
		"r_pinky0", /**< Metacarpal flexing mobility of the pinky finger. */
		"l_pinky1", /**< First knukle of the pinky finger. */
		"r_pinky1", /**< First knukle of the pinky finger. */
		"l_pinky1", /**< Lateral mobility of the pinky finger. */
		"r_pinky1", /**< Lateral mobility of the pinky finger. */
		"l_pinky1", /**< Along the pinky finger axis. */
		"r_pinky1", /**< Along the pinky finger axis. */
		"l_pinky2", /**< Second knuckle of the pinky number  */
		"r_pinky2", /**< Second knuckle of the pinky number  */
		"l_pinky3", /**< Third knuckle of the pinky finger. */
		"r_pinky3", /**< Third knuckle of the pinky finger. */
		"l_ring0", /**< Metacarpal flexing mobility of the ring finger. */
		"r_ring0", /**< Metacarpal flexing mobility of the ring finger. */
		"l_ring1", /**< First knukle of the ring finger. */
		"r_ring1", /**< First knukle of the ring finger. */
		"l_ring1", /**< Lateral mobility of the ring finger. */
		"r_ring1", /**< Lateral mobility of the ring finger. */
		"l_ring1", /**< Along the ring finger axis. */
		"r_ring1", /**< Along the ring finger axis. */
		"l_ring2", /**< 	Second knuckle of the ring number  */
		"r_ring2", /**< 	Second knuckle of the ring number  */
		"l_ring3", /**< 	Third knuckle of the ring finger. */
		"r_ring3", /**< 	Third knuckle of the ring finger. */
		"l_middle0", /**< Metacarpal flexing mobility of the middle finger. */
		"r_middle0", /**< Metacarpal flexing mobility of the middle finger. */
		"l_middle1", /**< First knukle of the middle finger. */
		"r_middle1", /**< First knukle of the middle finger. */
		"l_middle1", /**< 	Lateral mobility of the middle finger. */
		"r_middle1", /**< 	Lateral mobility of the middle finger. */
		"l_middle1", /**< Along the middle finger axis. */
		"r_middle1", /**< Along the middle finger axis. */
		"l_middle2", /**< Second knuckle of the middle number  */
		"r_middle2", /**< Second knuckle of the middle number  */
		"l_middle3", /**< Third knuckle of the middle finger. */
		"r_middle3", /**< Third knuckle of the middle finger. */
		"l_index0", /**< Metacarpal flexing mobility of the index finger. */
		"r_index0", /**< Metacarpal flexing mobility of the index finger. */
		"l_index1", /**< First knukle of the index finger. */
		"r_index1", /**< First knukle of the index finger. */
		"l_index1", /**< Lateral mobility of the index finger. */
		"r_index1", /**< Lateral mobility of the index finger. */
		"l_index1", /**< Along the index finger axis. */
		"r_index1", /**< Along the index finger axis. */
		"l_index2", /**< Second knuckle of the index number  */
		"r_index2", /**< Second knuckle of the index number  */
		"l_index3", /**< Third knuckle of the index finger. */
		"r_index3", /**< Third knuckle of the index finger. */
		"l_thumb1", /**< First knukle of the thumb finger. */
		"r_thumb1", /**< First knukle of the thumb finger. */
		"l_thumb1", /**< Lateral mobility of the thumb finger. */
		"r_thumb1", /**< Lateral mobility of the thumb finger. */
		"l_thumb1", /**< Along the thumb finger axis. */
		"r_thumb1", /**< Along the thumb finger axis. */
		"l_thumb2", /**< Second knuckle of the thumb number  */
		"r_thumb2", /**< Second knuckle of the thumb number  */
		"l_thumb3", /**< Third knuckle of the thumb finger. */
		"r_thumb3", /**< Third knuckle of the thumb finger. */
		"HumanoidRoot", /**< Body origin translation in vertical direction. */
		"HumanoidRoot", /**< 	Body origin translation in lateral direction. */
		"HumanoidRoot", /**< 	Body origin translation in frontal direction. */
		"HumanoidRoot", /**< Rotation of the skeleton root along the body coordinate system's vertical axis. */
		"HumanoidRoot", /**< Rotation of the skeleton root along the body coordinate system's frontal axis. */
		"HumanoidRoot" /**< Rotation of the skeleton root along the body coordinate system's lateral axis. */
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
};


static int BAPRotationAxis[NUM_BAPS]=
{
	    -1, /**< sacroiliac_tilt, /**< Forward-backward motion of the pelvis in the sagittal plane. */
        2, /**< sacroiliac_torsion, /**< Rotation of the pelvis along the body	vertical axis (defined by skeleton root). */
        3, /**< sacroiliac_roll, /**< Side to side swinging of the pelvis in the coronal plane. */
        -1, /**< l_hip_flexion, /**< Forward-backward rotation in the sagittal plane. */
        -1, /**< r_hip_flexion, /**< Forward-backward rotation in the sagittal plane. */
        3, /**< l_hip_abduct, /**< Sideward opening in the coronal plane  */
        3, /**< r_hip_abduct, /**< Sideward opening in the coronal plane  */
        -2, /**< l_hip_twisting, /**< Rotation along the thigh axis. */
        -2, /**< r_hip_twisting, /**< Rotation along the thigh axis. */
        -1, /**< l_knee_flexion, /**< Flexion-extension of the leg in the sagittal plane. */
        -1, /**< r_knee_flexion, /**< Flexion-extension of the leg in the sagittal plane. */
        -2, /**< l_knee_twisting, /**< Rotation along the shank axis. */
        -2, /**< r_knee_twisting, /**< Rotation along the shank axis. */
        -1, /**< l_ankle_flexion, /**< Flexion-extension of the foot  in the sagittal plane  */
        -1, /**< r_ankle_flexion, /**< Flexion-extension of the foot  in the sagittal plane  */
        -2, /**< l_ankle_twisting, /**< Rotation along the knee axis. */
        -2, /**< r_ankle_twisting, /**< Rotation along the knee axis. */
        3, /**< l_subtalar_flexion, /**< Sideward orientation of the foot   */
        3, /**< r_subtalar_flexion, /**< Sideward orientation of the foot   */
        3, /**< l_midtarsal_twisting, /**< Internal twisting of the foot (also called navicular joint in anatomy). */
        3, /**< r_midtarsal_twisting, /**< Internal twisting of the foot (also called navicular joint in anatomy). */
        -1, /**< l_metatarsal_flexion, /**< Up and down rotation of the toe in the sagittal plane. */
        -1, /**< r_metatarsal_flexion, /**< Up and down rotation of the toe in the sagittal plane. */
        3, /**< l_sternoclavicular_abduct, /**< Up and down motion in the coronal plane  */
        3, /**< r_sternoclavicular_abduct, /**< Up and down motion in the coronal plane  */
        2, /**< l_sternoclavicular_rotate, /**< Rotation in the transverse plane. */
        2, /**< r_sternoclavicular_rotate, /**< Rotation in the transverse plane. */
        3, /**< l_acromioclavicular_abduct, /**< Up and down motion in the coronal plane  */
        3, /**< r_acromioclavicular_abduct, /**< Up and down motion in the coronal plane  */
        2, /**< l_acromioclavicular_rotate, /**< Rotation in the transverse plane  */
        2, /**< r_acromioclavicular_rotate, /**< Rotation in the transverse plane  */
        -1, /**< l_shoulder_flexion, /**< Forward-backward motion in the sagittal plane  */
        -1, /**< r_shoulder_flexion, /**< Forward-backward motion in the sagittal plane  */
        3, /**< l_shoulder_abduct, /**< Sideward motion in the coronal plane  */
        3, /**< r_shoulder_abduct, /**< Sideward motion in the coronal plane  */
        -2, /**< l_shoulder_twisting, /**< Rotation along the scapular axis. */
        -2, /**< r_shoulder_twisting, /**< Rotation along the scapular axis. */
        -1, /**< l_elbow_flexion, /**< Flexion-extension of the arm in the sagittal plane  */
        -1, /**< r_elbow_flexion, /**< Flexion-extension of the arm in the sagittal plane  */
        -2, /**< l_elbow_twisting, /**< Rotation of the forearm along the upper arm axis. */
        -2, /**< r_elbow_twisting, /**< Rotation of the forearm along the upper arm axis. */
        -3, /**< l_wrist_flexion, /**< Rotation of the hand in the coronal plane  */
        -3, /**< r_wrist_flexion, /**< Rotation of the hand in the coronal plane  */
        -1, /**< l_wrist_pivot, /**< Rotation of the hand in the sagittal planes. */
        -1, /**< r_wrist_pivot, /**< Rotation of the hand in the sagittal planes. */
        -2, /**< l_wrist_twisting, /**< Rotation of the hand along the forearm axis. */
        -2, /**< r_wrist_twisting, /**< Rotation of the hand along the forearm axis. */
        3, /**< skullbase_roll, /**< Sideward motion of the skull along the frontal axis. */
        2, /**< skullbase_torsion, /**< Twisting of the skull along the vertical axis. */
        -1, /**< skullbase_tilt, /**< Forward-backward motion in the sagittal plane along a lateral axis  */
        3, /**< vc1_roll, /**< Sideward motion of vertebra C1  */
        2, /**< vc1_torsion, /**< Twisting of vertebra C1  */
        -1, /**< vc1_tilt, /**< Forward-backward motion of vertebra C1 in the sagittal plane. */ 
        3, /**< vc2_roll, /**< Sideward motion of vertebra C2  */
        2, /**< vc2_torsion, /**< Twisting of vertebra C2  */
        -1, /**< vc2_tilt, /**< Forward-backward motion of vertebra C2 in the sagittal plane  */
        3, /**< vc3_roll, /**< Sideward motion of vertebra C3  */
        2, /**< vc3_torsion, /**< Twisting of vertebra C3  */
        -1, /**< vc3_tilt, /**< Forward-backward motion of vertebra C3 in the sagittal plane  */
        3, /**< vc4_roll, /**< Sideward motion of vertebra C4  */
        2, /**< vc4_torsion, /**< Twisting of vertebra C4  */
        -1, /**< vc4_tilt, /**< Forward-backward motion of vertebra C4 in the sagittal plane  */
        3, /**< vc5_roll, /**< Sideward motion of vertebra C5  */
        2, /**< vc5_torsion, /**< Twisting of vertebra C5  */
        -1, /**< vc5_tilt, /**< Forward-backward motion of vertebra C5 in the sagittal plane  */
        3, /**< vc6_roll, /**< Sideward motion of vertebra C6  */
        2, /**< vc6_torsion, /**< Twisting of vertebra C6  */
        -1, /**< vc6_tilt, /**< Forward-backward motion of vertebra C6 in the sagittal plane  */
        3, /**< vc7_roll, /**< Sideward motion of vertebra C7  */
        2, /**< vc7_torsion, /**< Twisting of vertebra C7  */
        -1, /**< vc7_tilt, /**< Forward-backward motion of vertebra C7 in the sagittal plane  */
        3, /**< vt1_roll, /**< Sideward motion of vertebra T1  */
        2, /**< vt1_torsion, /**< Twisting of vertebra T1  */
        -1, /**< vt1_tilt, /**< Forward-backward motion of vertebra T1 in the sagittal plane  */
        3, /**< vt2_roll, /**< Sideward motion of vertebra T2  */
        2, /**< vt2_torsion, /**< Twisting of vertebra T2  */
        -1, /**< vt2_tilt, /**< Forward-backward motion of vertebra T2 in the sagittal plane  */
        3, /**< vt3_roll, /**< Sideward motion of vertebra T3  */
        2, /**< vt3_torsion, /**< Twisting of vertebra T3  */
        -1, /**< vt3_tilt, /**< Forward-backward motion of vertebra T3 in the sagittal plane  */
        3, /**< vt4_roll, /**< Sideward motion of vertebra T4  */
        2, /**< vt4_torsion, /**< Twisting of vertebra T4  */
        -1, /**< vt4_tilt, /**< Forward-backward motion of vertebra T4 in the sagittal plane  */
        3, /**< vt5_roll, /**< Sideward motion of vertebra T5  */
        2, /**< vt5_torsion, /**< Twisting of vertebra T5  */
        -1, /**< vt5_tilt, /**< Forward-backward motion of vertebra T5 in the sagittal plane  */
        3, /**< vt6_roll, /**< Sideward motion of vertebra T6  */
        2, /**< vt6_torsion, /**< Twisting of vertebra T6  */
        -1, /**< vt6_tilt, /**< Forward-backward motion of vertebra T6 in the sagittal plane  */
        3, /**< vt7_roll, /**< Sideward motion of vertebra T7  */
        2, /**< vt7_torsion, /**< Twisting of vertebra T7  */
        -1, /**< vt7_tilt, /**< Forward-backward motion of vertebra T7 in the sagittal plane  */
        3, /**< vt8_roll, /**< Sideward motion of vertebra T8  */
        2, /**< vt8_torsion, /**< Twisting of vertebra T8  */
        -1, /**< vt8_tilt, /**< Forward-backward motion of vertebra T8 in the sagittal plane  */
        3, /**< vt9_roll, /**< Sideward motion of vertebra T9  */
        2, /**< vt9_torsion, /**< Twisting of vertebra T9  */
        -1, /**< vt9_tilt, /**< Forward-backward motion of vertebra T9 in the sagittal plane  */
        3, /**< vt_10_roll, /**< Sideward motion of vertebra T10  */
        2, /**< vt10_torsion, /**< Twisting of vertebra T10  */
        -1, /**< vt10_tilt, /**< Forward-backward motion of vertebra T10 in sagittal plane  */
        3, /**< vt11_roll, /**< Sideward motion of vertebra T11  */
        2, /**< vt11_torsion, /**< Twisting of vertebra T11  */
        -1, /**< vt11_tilt, /**< Forward-backward motion of vertebra T11 in sagittal plane  */
        3, /**< vt12_roll, /**< Sideward motion of vertebra T12  */
        2, /**< vt12_torsion, /**< Twisting of vertebra T12  */
        -1, /**< vt12_tilt, /**< Forward-backward motion of vertebra T12 in sagittal plane  */
        3, /**< vl1_roll, /**< Sideward motion of vertebra L1  */
        2, /**< vl1_torsion, /**< Twisting of vertebra L1  */
        -1, /**< vl1_tilt, /**< Forward-backward motion of vertebra L1 in sagittal plane  */
        3, /**< vl2_roll, /**< Sideward motion of vertebra L2  */
        2, /**< vl2_torsion, /**< Twisting of vertebra L2  */
        -1, /**< vl2_tilt, /**< Forward-backward motion of vertebra L2 in sagittal plane  */
        3, /**< vl3_roll, /**< Sideward motion of vertebra L3  */
        2, /**< vl3_torsion, /**< Twisting of vertebra L3  */
        -1, /**< vl3_tilt, /**< Forward-backward motion of vertebra L3 in sagittal plane  */
        3, /**< vl4_roll, /**< Sideward motion of vertebra L4  */
        2, /**< vl4_torsion, /**< Twisting of vertebra L4  */
        -1, /**< vl4_tilt, /**< Forward-backward motion of vertebra L4 in sagittal plane  */
        3, /**< vl5_roll, /**< Sideward motion of vertebra L5  */
        2, /**< vl5_torsion, /**< Twisting of vertebra L5  */
        -1, /**< vl5_tilt, /**< Forward-backward motion of vertebra L5 in sagittal plane  */
        -1, /**< l_pinky0_flexion, /**< Metacarpal flexing mobility of the pinky finger. */
        -1, /**< r_pinky0_flexion, /**< Metacarpal flexing mobility of the pinky finger. */
        -1, /**< l_pinky1_flexion, /**< First knukle of the pinky finger. */
        -1, /**< r_pinky1_flexion, /**< First knukle of the pinky finger. */
        3, /**< l_pinky1_pivot, /**< Lateral mobility of the pinky finger. */
        3, /**< r_pinky1_pivot, /**< Lateral mobility of the pinky finger. */
        -2, /**< l_pinky1_twisting, /**< Along the pinky finger axis. */
        -2, /**< r_pinky1_twisting, /**< Along the pinky finger axis. */
        -1, /**< l_pinky2_flexion, /**< Second knuckle of the pinky number  */
        -1, /**< r_pinky2_flexion, /**< Second knuckle of the pinky number  */
        -1, /**< l_pinky3_flexion, /**< Third knuckle of the pinky finger. */
        -1, /**< r_pinky3_flexion, /**< Third knuckle of the pinky finger. */
        -1, /**< l_ring0_flexion, /**< Metacarpal flexing mobility of the ring finger. */
        -1, /**< r_ring0_flexion, /**< Metacarpal flexing mobility of the ring finger. */
        -1, /**< l_ring1_flexion, /**< First knukle of the ring finger. */
        -1, /**< r_ring1_flexion, /**< First knukle of the ring finger. */
        3, /**< l_ring1_pivot, /**< Lateral mobility of the ring finger. */
        3, /**< r_ring1_pivot, /**< Lateral mobility of the ring finger. */
        -2, /**< l_ring1_twisting, /**< Along the ring finger axis. */
        -2, /**< r_ring1_twisting, /**< Along the ring finger axis. */
        -1, /**< l_ring2_flexion, /**< 	Second knuckle of the ring number  */
        -1, /**< r_ring2_flexion, /**< 	Second knuckle of the ring number  */
        -1, /**< l_ring3_flexion, /**< 	Third knuckle of the ring finger. */
        -1, /**< r_ring3_flexion, /**< 	Third knuckle of the ring finger. */
        -1, /**< l_middle0_flexion, /**< Metacarpal flexing mobility of the middle finger. */
        -1, /**< r_middle0_flexion, /**< Metacarpal flexing mobility of the middle finger. */
        -1, /**< l_middle1_flexion, /**< First knukle of the middle finger. */
        -1, /**< r_middle1_flexion, /**< First knukle of the middle finger. */
        3, /**< l_middle1_pivot, /**< 	Lateral mobility of the middle finger. */
        3, /**< r_middle1_pivot, /**< 	Lateral mobility of the middle finger. */
        -2, /**< l_middle1_twisting, /**< Along the middle finger axis. */
        -2, /**< r_middle1_twisting, /**< Along the middle finger axis. */
        -1, /**< l_middle2_flexion, /**< Second knuckle of the middle number  */
        -1, /**< r_middle2_flexion, /**< Second knuckle of the middle number  */
        -1, /**< l_middle3_flexion, /**< Third knuckle of the middle finger. */
        -1, /**< r_middle3_flexion, /**< Third knuckle of the middle finger. */
        -1, /**< l_index0_flexion, /**< Metacarpal flexing mobility of the index finger. */
        -1, /**< r_index0_flexion, /**< Metacarpal flexing mobility of the index finger. */
        -1, /**< l_index1_flexion, /**< First knukle of the index finger. */
        -1, /**< r_index1_flexion, /**< First knukle of the index finger. */
        3, /**< l_index1_pivot, /**< Lateral mobility of the index finger. */
        3, /**< r_index1_pivot, /**< Lateral mobility of the index finger. */
        -2, /**< l_index1_twisting, /**< Along the index finger axis. */
        -2, /**< r_index1_twisting, /**< Along the index finger axis. */
        -1, /**< l_index2_flexion, /**< Second knuckle of the index number  */
        -1, /**< r_index2_flexion, /**< Second knuckle of the index number  */
        -1, /**< l_index3_flexion, /**< Third knuckle of the index finger. */
        -1, /**< r_index3_flexion, /**< Third knuckle of the index finger. */
        -1, /**< l_thumb1_flexion, /**< First knukle of the thumb finger. */
        -1, /**< r_thumb1_flexion, /**< First knukle of the thumb finger. */
        3, /**< l_thumb1_pivot, /**< Lateral mobility of the thumb finger. */
        3, /**< r_thumb1_pivot, /**< Lateral mobility of the thumb finger. */
        -2, /**< l_thumb1_twisting, /**< Along the thumb finger axis. */
        -2, /**< r_thumb1_twisting, /**< Along the thumb finger axis. */
        -1, /**< l_thumb2_flexion, /**< Second knuckle of the thumb number  */
        -1, /**< r_thumb2_flexion, /**< Second knuckle of the thumb number  */
        -1, /**< l_thumb3_flexion, /**< Third knuckle of the thumb finger. */
        -1, /**< r_thumb3_flexion, /**< Third knuckle of the thumb finger. */
        5, /**< HumanoidRoot_tr_vertical, /**< Body origin translation in vertical direction. */
        4, /**< HumanoidRoot_tr_lateral, /**< 	Body origin translation in lateral direction. */
        6, /**< HumanoidRoot_tr_frontal, /**< 	Body origin translation in frontal direction. */
        2, /**< HumanoidRoot_rt_body_turn, /**< Rotation of the skeleton root along the body coordinate system's vertical axis. */
        3, /**< HumanoidRoot_rt_body_roll, /**< Rotation of the skeleton root along the body coordinate system's frontal axis. */
        1, /**< HumanoidRoot_rt_body_tilt /**< Rotation of the skeleton root along the body coordinate system's lateral axis. */
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
};

/**
* Body Animation Parameters.
* The storage class for the MPEG-4 Body Animation Parameters (BAPs). It contains
* a full set of BAPs according to MPEG-4 specification. They can be set directly
* in the value array. Alternatively, the FBAPs class provides convenience functions
* to set the BAPs. For BAP names, see BAPNames.
*
* A summary of the MPEG-4 BAP specification can be found in the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>. 
* 
* This class also provides functions for merging sets of BAPs.
*
* @see FBAPs
* @see BAPNames
* Implemented in visagefbadecoder.lib
*/
class VISAGE_DECLSPEC BAPs  
{
public:
	/**
	* Reset the BAPs.
	* Reset BAPs to the neutral body position, i.e. all parameters are set to 0.
	*/
	void reset();

	/**
	* Constructor.
	* Constructs BAPs with a neutral body pose.
	*/
	BAPs();

	/**
	* Constructor.
	* Constructs BAPs by copying given BAPs.
	* @param BAPs the BAPs to be copied.
	*/
	BAPs(const BAPs *BAPs);

	/**
	* Destructor.
	*/
	virtual ~BAPs();

	/**
	* Set the BAPs.
	* Set this BAPs by copying the given BAPs.
	* @param BAPs the BAPs to be copied.
	*/
	void set(const BAPs *BAPs);


	/**
	* Merge BAPs.
	* Merge this BAPs with the given BAPs.
	* The BAPs are merged as follows:
	*
	* The value of each BAP is averaged between the two BAPs if both are
	* non-zero. If either is zero, the other is used without averaging. This is done to allow merging
	* of independent BAPs. For example, if one BAPs has non-zero values for the left arm, and the other
	* for the right arm, the result should have the left arm values from one, and right arm values from another,
	* without changing the values. Simple averaging would give halved values.
	*
	* In practice, merging gives best results for BAPs that are naturally independent, like
	* an arm movement and a leg movement. Such BAPs will merge naturally. Merging of BAPs that act on the same body joints
	* may give unpredictable results.
	* @param BAPs the BAPs to be merged with this BAPs.
	*/
	void merge(BAPs *BAPs);

	/**
	* Set the mask.
	* Set the mask values (groupMask) for all animation parameters to 1 if maskValue is true, or to 0 if maskValue is false.
	* maskType for all groups is set to 1.
	*
	* @param maskValue the value for the mask to be set - 0 if false, 1 if true.
	*
	* @see BAPs::maskType
	* @see BAPs::groupMask
	*/
	void setMask(bool maskValue);

	/**
	* Set the mask conditionally.
	* Set the mask values (groupMask) to 1 only for animation parameters 
	* whose value is different in the baps passed as parameter. Other mask values are not touched.
	*
	* @param baps the BAPs used for comparison.
	*
	* @see BAPs::maskType
	* @see BAPs::groupMask
	*/
	void setMaskIfDiff(BAPs *baps);

	/**
	* Set the mask values.
	* Set the mask values (maskType and groupMask) by copying them from baps.
	*
	* @param baps the BAPs from which mask values will be copied.
	*
	* @see BAPs::maskType
	* @see BAPs::groupMask
	*/
	void setMask(BAPs *baps);

	/**
	* BAP values. The value of each MPEG-4 BAP.
	*
	* The indices in this array correspond to the list of BAPs in the MPEG-4 documentation(see <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>), but the
	* numbers are shifted by one because documentation starts with 1, and array starts with 0.
	*
	* It is convenient to use the BAPNames enumerator values when indexing this array.
	*/
	int value[NUM_BAPS];

	/**
	* Mask types.
	*
	* Mask types for BAP groups.
	*
	* Note: masking is used only when BAPs are encoded into a file/bitstream. If you are not encoding
	* BAPs (i.e. writing them into FBA files, e.g. using CFBAEncoder or FbaFileAction::write()), you do not need to change the groupMask or maskType.
	* Even if you do write files, the default values
	* for groupMask or maskType will produce correct results. However, by changing these parameters you can achieve
	* much better coding efficiency, i.e. make the FBA files much smaller.
	*
	* The masking mechanism is explained in detail in <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a> (section "BAP grouping and masking").
	* If that looks a bit complicated, you can just leave the default values, with all maskType values set to 3, so all %BAPs
	* will be encoded. In this case you do not need to touch @ref groupMask or @ref maskType at all. To improve efficiency, you may
	* set maskType for some groups to 0. You still do not
	* need to set the @ref groupMask.
	*
	* The indices in the groupMask array correspond to the list of BAP groups in the MPEG-4 documentation,
	* but the
	* numbers are shifted by one i.e. maskType[0] corresponds to group 1 in 
	* the documentation. 
	*
	* @see groupMask
	*/
	int maskType[BAP_NUM_GROUPS];



	/**
	* Group mask.
	*
	* Note: masking is used only when BAPs are encoded into a file/bitstream. If you are not encoding
	* BAPs (i.e. writing them into FBA files, e.g. using CFBAEncoder or FbaFileAction::write()), you do not need to change the groupMask or maskType.
	* Even if you do write files, the default values
	* for groupMask or maskType will produce correct results. However, by changing these parameters you can achieve
	* much better coding efficiency, i.e. make the FBA files much smaller.
	*
	* The masking mechanism is explained in detail in <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a> (section "BAP grouping and masking").
	* If that looks a bit complicated, you can just leave the default values, with all maskType values set to 3, so all %BAPs
	* will be encoded. In this case you do not need to touch @ref groupMask or @ref maskType at all. To improve efficiency, you may
	* set maskType for some groups to 0. 
	*
	* Note that the only groupMask values used are those for BAPs for which @ref maskType is set to 1. 
	* Other values in the groupMask array are not used and do not need to be set.
	*
	* The indices in the groupMask array correspond to the list of %BAPs in the MPEG-4 documentation,
	* but the
	* numbers are shifted by one i.e. groupMask[0] corresponds to BAP number 1 in 
	* the documentation. 
	*
	* It is convenient to use the BAPNames enumerator values when indexing this array.
	*
	* @see maskType
	* @see BAPNames
	*/
	int groupMask[NUM_BAPS];

private:
};

}


#endif // __BAPS_h__
