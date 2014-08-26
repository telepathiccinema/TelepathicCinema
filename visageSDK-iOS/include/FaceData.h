#ifndef __FaceData_h__
#define __FaceData_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "cv.h"
#include "FDP.h"
#include "FBAPs.h"

namespace VisageSDK
{
/**
 * Face data structure, used as container for all face tracking and detection results.
 This structure is passed as parameter to the VisageTracker2::getTrackingData() or VisageFeaturesDetector::detectFacialFeatures() method. The method copies latest tracking or detection results into it.
 
When filling the structure with data some members are filled while some are left undefined depending on tracking/detection status. 

<h2>Obtaining tracking data</h2>

The tracker returns these main classes of data:
 - 3D head pose
 - facial expression
 - gaze direction
 - facial feature points
 - full 3D face model, textured

The tracker status is the return value of the VisageTracker2::getTrackingData() function. 
The following table describes the possible states of the tracker, and lists active member variables (those that are filled with data) for each status.
 <table>
 <tr><td width="100"><b>TRACKER STATUS</b></td><td><b>DESCRIPTION</b></td><td><b>ACTIVE VARIABLES</b></td></tr>
 <tr><td>TRACK_STAT_OFF</td><td>Tracker is not active, i.e. it has not yet been started, or it has been stopped.</td>
 <td>N/A</td></tr>
 <tr><td>TRACK_STAT_OK</td><td>Tracker is tracking normally.</td>
 <td>
 trackingQuality,
 frameRate,
 frame,
 cameraFocus,
 faceTranslation,
 faceRotation,
 faceAnimationParameters,
 actionUnitCount,
 actionUnitsUsed,
 actionUnits,
 actionUnitsNames,
 featurePoints3D,
 featurePoints3DRelative,
 featurePoints2D,
 faceModelVertexCount,
 faceModelVertices,
 faceModelVerticesProjected,
 faceModelTriangleCount,
 faceModelTriangles,
 faceModelTextureCoords
</td></tr>
 <tr><td>TRACK_STAT_RECOVERING</td><td>Tracker has lost the face and is attempting to recover and continue tracking. If it can not recover within the time defined by the parameter recovery_timeout in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration file</a>, the tracker will fully re-initialize (i.e. it will assume that a new user may be present).</td>
 <td>
 frameRate,
 frame,
 cameraFocus</td></tr>
 <tr><td>TRACK_STAT_INIT</td><td>Tracker is initializing. The tracker enters this state immediately when it is started, or when it has lost the face and failed to recover (see TRACK_STAT_RECOVERING above). The initialization process is configurable through a number of parameters in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration file.</a></td>
 <td>
 frameRate,
 frame,
 cameraFocus,
 initDisplayStatus,
 initFaceDetected
 
 If initFaceDetected is "true", the following member variables are also active:
 
 initYaw,
 initRoll,
 initVelocity</td></tr>
 </table>

 <h3>Smoothing</h3>

 Most of the tracking data can be smoothed to reduce the tracking noise. The tracker applies smoothing 
 separately on different groups of tracking results such as head translation, rotation, gaze, etc. The 
 smoothing factor is set separately for each group. Smoothing is adaptive, i.e. it is automatically 
 reduced when fast motion is detected, resulting in steady values when motion is slow or static, while 
 avoiding delay for fast motion. For more details, see the smoothing_factors parameter array in the 
 <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a>.

 <h2>Obtaining detection data</h2>

The detector returns these main classes of data:
 - 3D head pose
 - facial features points
 - full 3D face model, textured

Detection result is returned from VisageFeaturesDetector::detectFacialFeatures() function.
The following table describes possible output from the detector and the list of active variables (those that are filled with data). All other variables are left undefined.
<table>
<tr><td width="100"><b>DETECTION RESULT</b></td><td><b>DESCRIPTION</b></td><td><b>ACTIVE VARIABLES</b></td></tr>
<tr><td> 0 </td><td>Detector did not find any faces in the image</td>
<td>N/A</td></tr>
<tr><td> N > 0 </td><td>Detector detected N faces in the image.</td>
<td>
<b>For first N FaceData objects in the array:</b>
 cameraFocus,
 faceTranslation,
 faceRotation,
 featurePoints3D,
 featurePoints3DRelative,
 featurePoints2D,
 faceModelVertexCount,
 faceModelVertices,
 faceModelVerticesProjected,
 faceModelTriangleCount,
 faceModelTriangles,
 faceModelTextureCoords
 <br>
 <b>For other FaceData objects in the array:</b>
 N/A
 </td></tr>
</table>
 
<h2>Returned data</h2>

 The following sections give an overview of main classes of data that may be returned in FaceData, and pointers to specific data members.
 
 <h3>3D head pose</h3>
 
The 3D head pose consists of head translation and rotation. It is available as absolute pose of the head with respect to the camera.
 
 The following member variables return the head pose:
 - #faceTranslation
 - #faceRotation 

 Both facial features tracker and facial features detector return the 3D head pose.
 
 <h3>Facial expression</h3>
 
 Facial expression is available in two forms: MPEG-4 FAPs and Action Units.
 
MPEG-4 facial animation parameters (FAPs) are returned in #faceAnimationParameters; more information about MPEG-4 FAPs is available in <a href="doc/MPEG-4 FBA Overview.pdf">MPEG-4 FAPs overview</a>.
 
 Action Units (AUs) are the internal representation of facial motion used by the tracker, while MPEG-4 FAPs are derived. It is therefore more accurate to use the AUs directly. It should be noted that AUs are <a href="doc/VisageTracker Configuration Manual.pdf">fully configurable</a> in the tracker configuration files (specifically, in the 3D model file, .wfm).
 
 The following member variables return Action Units data:
 - #actionUnitCount
 - #actionUnitsUsed
 - #actionUnits
 - #actionUnitsNames

 Only facial features tracker returns the facial expression; facial features detector leaves these variables undefined.

 <h3>Gaze direction and eye closure</h3>

 Gaze direction is available in local coordinate system of the person's face or global coordinate system of the camera. Eye closure is available as binary information (OPEN/CLOSED).

 The following member variables return gaze direction and eye closure:
 - #gazeDirection
 - #gazeDirectionGlobal
 - #eyeClosure
 
 Only facial features tracker returns gaze direction and eye closure; facial features detector leaves these variables undefined.

 <h3>Facial feature points</h3>
 
2D or 3D coordinates of facial feature points, as defined by the <a href="doc/MPEG-4 FBA Overview.pdf">MPEG-4 FBA standard</a>, are available.
 
3D coordinates are available in global coordinate system or relative to the origin of the face (i.e. the point in the center between the eyes in the input image).
 
 Facial features are available through the following member variables:
 - #featurePoints3D
 - #featurePoints3DRelative
 - #featurePoints2D

  Both facial features tracker and facial features detector return facial feature points.
 
 <h3>3D face model</h3>
 
 This is the animatable face model used internally by the tracker or detector. It represents the richest and most detailed level of tracking/detection results. The model is fitted in 3D to the face in the current image/video frame. The model is a single textured 3D triangle mesh.
 The texture of the model is the current image/video frame. This means that, when the model is drawn using the correct perspective it exactly recreates the facial part of the image. The correct perspective is defined by camera focal length (#cameraFocus), 
 width and height of the input image or the video frame (#frame), model rotation (#faceRotation) and translation (#faceTranslation) - this <a href="doc/ar-notes.cpp">example code</a> shows how to do this using OpenGL.
 
 There are multiple potential uses for the face model. Some ideas include, but are not limited to:
 - Draw the 3D face model into the Z buffer to achieve correct occlusion of virtual objects by the head in AR applications.
 - Use texture coordinates to cut out the face from the image. (NOTE: if this is the desired effect, the model with closed mouth should be used otherwise when the mouth opens a hole appears. The model candide3-ClosedMouth.wfm is provided for this purpose and should simply be specified instead of the default Candide3.wfm in the model_filename statement in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker/detector configuration file</a>).
 - Draw the 3D face model from a different perspective than the one in the actual video.
 - Insert the 3D face model into another video or 3D scene.
 
 \if IOS_DOXY
 The sample projects <a href="doc/samples.html">VisageTrackerDemo</a> and <a href="doc/VisageTrackerUnityDemo_Documentation.pdf">VisageTrackerUnityDemo</a> both demonstrate how to access the 3D model information and display the model correctly aligned with the face image.
 \endif
 \if ANDROID_DOXY
 The sample projects <a href="doc/samples.html">VisageTrackerDemo</a> and <a href="doc/VisageTrackerUnityDemo_Documentation.pdf">VisageTrackerUnityDemo</a> both demonstrate how to access the 3D model information and display the model correctly aligned with the face image.
 \endif

 The 3D face model is contained in the following members:
 - #faceModelVertexCount
 - #faceModelVertices
 - #faceModelTriangleCount
 - #faceModelTriangles
 - #faceModelTextureCoords

  Both facial features tracker and facial features detector return the 3D face model.

*/
struct VISAGE_DECLSPEC FaceData
{		
	 //data while tracking
    
    /** Tracking quality level.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
	 * Facial features detector leaves this variable undefined.</i>
     * 
     * Estimated tracking quality level for 
     * the current frame. The value is between 0 and 1, and it corresponds to the global_bad_match_threshold parameter 
     * in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker configuration file</a>, i.e. the quality measure is checked against this threshold and when it 
     * falls below the tracker resets itself.
     */
    float trackingQuality;

	float trackingQualityBdts;
    
    /** The frame rate of the tracker, in frames per second, measured over last 10 frames.
     * <i>This variable is set while tracker is running., i.e. while tracking status is not TRACK_STAT_OFF. Facial features detector leaves this variable undefined.</i>
     */
    float frameRate;
    
    /** Timestamp of the current video frame.
     * <i>This variable is set while tracker is running., i.e. while tracking status is not TRACK_STAT_OFF.
	 * Facial features detector leaves this variable undefined.</i>
	 *
     * When tracking from camera, timeStamp returns time, in milliseconds, measured from the moment when tracking started. When tracking from video, timeStamp returns the time of the current video frame, in milliseconds. When tracking from raw images, timeStamp returns the value it received from the VisageTrackerFrameGrabber::GrabFrame() function if it is different than -1, otherwise it returns time, in milliseconds, measured from the moment when tracking started.
     *
     * @see VisageTrackerFrameGrabber
     */
    long timeStamp;

    // translation and rotation data
    
    /** Translation of the head from the camera.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     *
     * Translation is expressed with three coordinates x, y, z.
     * The coordinate system is such that when looking towards the camera, the direction of x is to the
     * left, y iz up, and z points towards the viewer - see illustration below. The global origin (0,0,0) is placed at the camera. The reference point on the head is in the center between the eyes.
	 *
	 * \image html "./../images/coord-camera.png"
     * \image latex images/coord-camera.png "Coordinate system" width=10cm
     *
     * If the value set for the camera focal length in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker/detector configuration</a> file
     * corresponds to the real camera used, the returned coordinates shall be in meters; otherwise the scale of the translation values is not known, but the relative values are still correct (i.e. moving towards the camera results in smaller values of z coordinate).
     *
	 * <b>Aligning 3D objects with the face</b>
	 *
     * The translation, rotation and the camera focus value together form the 3D coordinate system of the head in its current position
     * and they can be used to align 3D rendered objects with the head for AR or similar applications.
	 * This <a href="doc/ar-notes.cpp">example code</a> shows how to do this using OpenGL.
	 *
	 * The relative facial feature coordinates (featurePoints3DRelative)
     * can then be used to align rendered 3D objects to the specific features of the face, like putting virtual eyeglasses on the eyes. Samples projects demonstrate how to do this, including full source code.
     *
     * @see faceRotation
     */
    float faceTranslation[3];
    
    /* Translation of the head, compensated.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).</i>
     *
     * Translation is expressed with three coordinates x, y, z.
     * The coordinate system is such that when looking towards the camera, the direction of x is to the
     * left, y iz up, and z points towards the viewer.  The global origin (0,0,0) is placed at the camera.
     *
     * Translation of the head relative to its position in the first video frame.
     * This is compensated translation estimated by the tracker.
     * Compensated translation values take into account the fact that the tracker
     * uses a relatively flat face model for tracking, so the center of rotation of this model
     * is in the front area of the head, while the anatomical center
     * of rotation is behind, in the base of the neck. Therefore, when the rotation
     * is applied to a 3D head model with anatomically correct center of rotation, the
     * face naturally translates as well. When this translation is compounded with
     * the translation values obtained from the tracker, the total resulting translation
     * is exaggerated. To avoid this exaggerated translation of the animated head,
     * the translation can be compensated. The compensation algorithm
     * estimates how much the translation would be exaggerated, and makes it that much
     * smaller. The compensated translation can directly be applied to animated head
     * models that use the neck base as the center of rotation, and is expected to give
     * better results than the uncompensated translation.
     *
     * @see faceTranslation, faceRotation
     */
    float faceTranslationCompensated[3];

    /** Rotation of the head.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * This is the estimated rotation of the head, in radians.
     * Rotation is expressed with three values determining the rotations
     * around the three axes x, y and z, in radians. This means that the values represent
     * the pitch, yaw and roll of the head, respectively. The zero rotation
     * (values 0, 0, 0) corresponds to the face looking straight ahead along the camera axis.
     * Positive values for pitch correspond to head turning down.
     * Positive values for yaw correspond to head turning right in the input image.
     * Positive values for roll correspond to head rolling to the left in the input image, see illustration below.
     * The values are in radians.
     *
     * \image html "./../images/coord-rotation.png"
     * \image latex images/coord-rotation.png "Rotations" width=10cm
     *
     * @see faceTranslation
     */
    float faceRotation[3];

	 /** Gaze direction.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
	 * Facial features detector leaves this variable undefined.</i>
     *
     * This is the current estimated gaze direction relative to the person's head.
     * Direction is expressed with two values x and y, in radians. Values (0, 0) correspond to person looking straight.
	 * X is the horizontal rotation with positive values coresponding to person looking to his/her left.
	 * Y is the vertical rotation with positive values coresponding to person looking down.
	 *
	 * @see gazeDirectionGlobal
     */
    float gazeDirection[2];

	/** Global gaze direction, taking into account both head pose and eye rotation.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
	 * Facial features detector leaves this variable undefined.</i>
     *
     * This is the current estimated gaze direction relative to the camera axis.
     * Direction is expressed with three values determining the rotations
     * around the three axes x, y and z, i.e. pitch, yaw and roll. Values (0, 0, 0) correspond to the gaze direction parallel to the camera axis.
	 * Positive values for pitch correspond to gaze turning down.
     * Positive values for yaw correspond to gaze turning right in the input image.
     * Positive values for roll correspond to face rolling to the left in the input image, see illustration below.
	 * 
     * The values are in radians.
	 *
	 * \image html "./../images/coord-rotation-eye.png"
     * \image latex images/coord-rotation-eye.png "Rotations" width=10cm
	 *
	 * The global gaze direction can be combined with eye locations to determine the line(s) of sight in the real-world coordinate system with the origin at the camera.
	 * To get eye positions use #featurePoints3D and FDP::getFP() function, e.g.:
	 *
	 * \code
	 * FeaturePoint *left_eye_fp = const_cast<FeaturePoint*>( &featurePoints3D->getFP(3,5) );
	 * FeaturePoint *right_eye_fp = const_cast<FeaturePoint*>( &featurePoints3D->getFP(3,6) );
	 *
	 * float left_eye_pos[3], right_eye_pos[3];
	 *
	 * left_eye_pos[0]  = left_eye_fp->pos[0];  // x
	 * left_eye_pos[1]  = left_eye_fp->pos[1];  // y
	 * left_eye_pos[2]  = left_eye_fp->pos[2];  // z
	 * right_eye_pos[0] = right_eye_fp->pos[0]; // x
	 * right_eye_pos[1] = right_eye_fp->pos[1]; // y
	 * right_eye_pos[2] = right_eye_fp->pos[2]; // z
	 * \endcode
	 *
	 * @see gazeDirection, featurePoints3D
     */
	float gazeDirectionGlobal[3];
    
	/** Discrete eye closure value. 
	 * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
	 * Facial features detector leaves this variable undefined.</i>
	 *
	 * Index 0 represents closure of left eye. Index 1 represents closure of right eye.
	 * Value of 1 represents open eye. Value of 0 represents closed eye.
     */
    float eyeClosure[2];

    // action units data
    /** Number of facial Action Units.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
     * Facial features detector leaves this variable undefined.</i>
	 *
     * Number of action units that are defined for current face model.
     *
     * @see actionUnits, actionUnitsUsed, actionUnitsNames
     */
    int actionUnitCount;
    
    /** Used facial Action Units.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
	 * Facial features detector leaves this variable undefined.</i>
     *
     * List of values, one for each action unit, to determine if specific action unit is actually used in the current tracker configuration.
     * Values are as follows: 1 if action unit is used, 0 if action unit is not used.
     *
     * @see actionUnits, actionUnitCount, actionUnitsNames
     */
    int *actionUnitsUsed;
    
    /** List of current values for facial Action Units, one value for each action unit.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
	 * Facial features detector leaves this variable undefined.</i>
     *
	 * The action units used by the tracker are defined in the
     * 3D face model file (currently Candide3.wfm; tracker can be configured to use another file see the
	 * <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for details).
	 * Furthermore, the tracker configuration file defines the names of action units and these names can be accessed through actionUnitsNames.
	 * Please refer to section or <a href="doc/VisageTracker Configuration Manual.pdf">VisageTracker Configuration Manual</a> for full list of action units for each tracker configuration.
     *
     * @see actionUnitsUsed, actionUnitCount, actionUnitsNames
     *
     */
    float *actionUnits;


	/**
	* List of facial Action Units names.
	*
	* <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
	* Facial features detector leaves this variable undefined.</i>
	*
	* @see actionUnitsUsed, actionUnitCount, actionUnits
	*/
	const char **actionUnitsNames;
    
    /** MPEG-4 facial animation parameters.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK).
     * Facial features detector leaves this variable undefined.</i>
	 *
     * The face animation parameters are returned as FBAPs class that contains the Face Animation Parameters (FAPs)
     * currently estimated by the tracker.
     * The global translation of the face is stored in the global translation Body Animation Parameters (BAPs).
     * The specification of the FAP and BAP parameters
     * is contained in the the <a href="../MPEG-4 FBA Overview.pdf">MPEG-4 Face and Body Animation Introduction</a>.
     *
     * Certain parameters, like the ones on the tongue, teeth, nose and ears, can currently not be reliably estimated so they are not returned
     * and their values are always set to zero. These parameters are:
     * - FAPs 14 - 18 (jaw thrust and shift, lips forward push)
     * - FAPs 27 - 30 (eyeball thrust and pupil dilation)
     * - FAPs 39 - 40 (puff cheeks)
     * - FAPs 43 - 47 (tongue motion)
     * - FAPs 61 - 68 (nose and ear motion)
     *
     * Furthermore, the parameters of the outer lip contour (51 - 60) and the corresponding parameters of the inner lip
     * contour (4 -13) are both set to the mean value of the outer and inner lip displacement. For example, parameters 4 (vertical displacement of top inner lip) and 51 (vertical displacement of top outer lip)
     * are both set to the same value, and this value is the mean displacement of the upper lip midpoint.
     *
     */
    FBAPs *faceAnimationParameters;

    // feature points data
    /** Facial feature points (global 3D coordinates).
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * The coordinate system is such that when looking towards the camera, the direction of x is to the
     * left, y iz up, and z points towards the viewer.  The global origin (0,0,0) is placed at the camera, see illustration.
   	 *
	 * \image html "../images/coord-camera.png"
     * \image latex images/coord-camera.png "Coordinate system" width=10cm
	 *
	 * If the value set for the camera focal length in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker/detector configuration</a> file
     * corresponds to the real camera used, the returned coordinates shall be in meters; otherwise the scale is not known, but the relative values are still correct (i.e. moving towards the camera results in smaller values of z coordinate).
     *
     * The feature points are identified
     * according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
     * belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
     * illustrated in FDP class documentation.
     *
     * Certain feature points, like the ones on the tongue and teeth, can not be reliably detected so they are not returned
     * and their coordinates are always set to zero. These points are:
     * 6.1, 6.2, 6.3, 6.4, 7.1, 9.8, 9.9, 9.10, 9.11, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 11.4, 11.5, 11.6
     * Several other points are estimated, rather than accurately detected, due to their specific locations. Examples of such points
     * are 11.4 (usually hidden in the hair), hairline points (10.1 - 10.3) as well as cheeck points 5.1 - 5.4 which can not be precisely defined due to lack of
     * features on the cheek.
     *
     * The resulting feature point coordinates are returned in form of an FDP object. This is a container class used for storage of MPEG-4 feature points.
     * It provides functions to access each feature point by its group and index and to read its coordinates.
     *
     * @see featurePoints3DRelative, featurePoints2D
     *
     */
    FDP *featurePoints3D;
    /** Facial feature points (3D coordinates relative to the origin, placed at the center between eyes).
     *
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
	 * The coordinates are in the local coordinate system of the face, with the origin (0,0,0) placed at the center between the eyes.
     * The x-axis points laterally towards the side of the face, y-xis points up and z-axis points into the face - see illustration below.
	 *
	 * \image html "../images/coord.png"
     * \image latex images/coord.png "Coordinate system" width=10cm
     *
     * The feature points are identified
     * according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
     * belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
     * illustrated in FDP class documentation.
     *
     * Certain feature points, like the ones on the tongue and teeth, can not be reliably detected so they are not returned
     * and their coordinates are always set to zero. These points are:
     * 6.1, 6.2, 6.3, 6.4, 7.1, 9.8, 9.9, 9.10, 9.11, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 11.4, 11.5, 11.6
     * Several other points are estimated, rather than accurately detected, due to their specific locations. Examples of such points
     * are 11.4 (usually hidden in the hair), hairline points (10.1 - 10.3) as well as cheeck points 5.1 - 5.4 which can not be precisely defined due to lack of
     * features on the cheek.
     *
     * The resulting feature point coordinates are returned in form of an FDP object. This is a container class used for storage of MPEG-4 feature points.
     * It provides functions to access each feature point by its group and index and to read its coordinates.
     *
     * @see featurePoints3D featurePoints2D
     */
    FDP *featurePoints3DRelative;

    /** Facial feature points (2D coordinates).
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * The 2D feature point coordinates are normalised to image size so that the lower left corner of the image has coordinates 0,0 and upper right corner 1,1.
     *
     * The feature points are identified
     * according to the MPEG-4 standard, so each feature point is identified by its group and index. For example, the tip of the chin
     * belongs to group 2 and its index is 1, so this point is identified as point 2.1. The identification of all MPEG-4 feature points is
     * illustrated in FDP class documentation.
     *
     * Certain feature points, like the ones on the tongue and teeth, can not be reliably detected so they are not returned
     * and their coordinates are always set to zero. These points are:
     * 6.1, 6.2, 6.3, 6.4, 7.1, 9.8, 9.9, 9.10, 9.11, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 11.4, 11.5, 11.6
     * Several other points are estimated, rather than accurately detected, due to their specific locations. Examples of such points
     * are 11.4 (usually hidden in the hair), hairline points (10.1 - 10.3) as well as cheeck points 5.1 - 5.4 which can not be precisely defined due to lack of
     * features on the cheek.
	 *
	 * When FaceData is obtained from tracker and process_eyes parameter is set to '1' in the tracker configuration file, points 3.5 and 3.6 (pupils) are set to their detected values. If process_eyes is set to 0,
	 * points 3.5 and 3.6 are not detected and their coordinates are set to zero. 
	 *
	 * Note that the 2D pupil points may slightly differ from the projection of the pupil 
	 * points in featurePoints3D; this is because the 3D points are obtained from the 3D model while the 2D points are directly detected in the image.
     *
     * The resulting feature point coordinates are returned in form of an FDP object. This is a container class used for storage of MPEG-4 feature points.
     * It provides functions to access each feature point by its group and index and to read its coordinates. Note that FDP stores 3D points and in the case of 2D feature points only the x and y coordinates of each point are used.
     *
     * @see featurePoints3D, featurePoints3DRelative
     */
    FDP *featurePoints2D;
    
    // face model mesh data
    /** Number of vertices in the 3D face model.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * @see faceModelVertices, faceModelVerticesProjected, faceModelTriangleCount, faceModelTriangles, faceModelTextureCoords
     */
    int faceModelVertexCount; 
    
    /** List of vertex coordinates of the 3D face model.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * The format of the list is x, y, z coordinate for each vertex.
	 *
	 * The coordinates are in the local coordinate system of the face, with the origin (0,0,0) placed at the center between the eyes.
     * The x-axis points laterally  towards the side of the face, y-axis points up and z-axis points into the face - see illustration below.
	 *
	 * \image html "../images/coord.png"
     * \image latex images/coord.png "Coordinate system" width=10cm
     *
     * To transform the coordinates into the coordinate system of the camera, use faceTranslation and faceRotation.
     *
     * If the value set for the camera focal length in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker/detector configuration</a> file
     * corresponds to the real camera used, the scale of the coordinates shall be in meters; otherwise the scale is not known.
     *
     * @see faceModelVertexCount, faceModelVerticesProjected, faceModelTriangleCount, faceModelTriangles, faceModelTextureCoords
     */
    float* faceModelVertices;
    
    /** List of projected (image space) vertex coordinates of the 3D face model.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * The format of the list is x, y coordinate for each vertex.
     * The 2D coordinates are normalised to image size so that the lower left corner of the image has coordinates 0,0 and upper right corner 1,1.
     *
     * @see faceModelVertexCount, faceModelVertices, faceModelTriangleCount, faceModelTriangles,faceModelTextureCoords
     */
    float* faceModelVerticesProjected;
    
    /** Number of triangles in the 3D face model. 
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * @see faceModelVertexCount, faceModelVertices, faceModelVerticesProjected, faceModelTriangles, faceModelTextureCoords
     */
    int faceModelTriangleCount;
    
    /** Triangles list for the 3D face model.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK), or if the detector has detected a face.</i>
     *
     * Each triangle is described by three indices into the list of vertices @ref faceModelVertices (counter-clockwise convention is used for normals direction).
     *
     * @see faceModelVertexCount, faceModelVertices, faceModelVerticesProjected, faceModelTriangleCount, faceModelTextureCoords
     */
    int* faceModelTriangles;
    
    /** Texture coordinates for the 3D face model.
     * <i>This variable is set only while tracker is tracking (TRACK_STAT_OK) or if the detector has detected a face.</i>
     *
     * A pair of u, v coordinates for each vertex. When FaceData is obtained from the tracker, the texture image is the current video frame, accessible through #frame.
	 * When FaceData is obtained from detector, the texture image is the input image of the detector.
     *
     * @see faceModelVertexCount, faceModelVertices, faceModelVerticesProjected, faceModelTriangleCount, faceModelTriangles
     */
    float* faceModelTextureCoords;

    // frame and camera data
    /** Pointer to the video frame associated with the current data, or NULL if tracker is not active.
     * <i>This variable is set while tracker is running., i.e. while tracking status is not TRACK_STAT_OFF.
	 * Facial features detector leaves this variable undefined.</i>
     *
     * IplImage is the image storage class from OpenCV, please refer to OpenCV documentation for details of accessing its data members; the basic members are the size of the image (frame->width, frame->height) and the pointer to the actual pixel data of the image (frame->imageData).
     *
     * Note: For performance reasons frame is passed only as a pointer, i.e. actual image data is not copied into FaceData structure. The tracker uses a triple buffer mechanism, so the frame will be valid for at least the duration of two video frames. If frame is used later than that, it may point to a new image, not corresponding to the rest of the tracking data.
     */
    IplImage* frame;
    
    /** Focal distance of the camera, as configured in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker/detector configuration</a> file.
     * <i>This variable is set while tracker is running (any status other than TRACK_STAT_OFF), or if the detector has detected a face.</i>
     *
     * The camera focal distance mainly used for 3D scene set-up and accurate interpretation of tracking data.
     * This member corresponds to the camera_focus parameter in the <a href="doc/VisageTracker Configuration Manual.pdf">tracker/detector configuration</a> file.
     */
    float cameraFocus;
    
    //data while initializing
    /** True if a face is detected during tracker initialization, false otherwise.
     * <i>This variable is set only while tracker is initializing (TRACK_STAT_INIT).
	 * Facial features detector leaves this variable undefined.</i>
     *
     * During tracking initialization, this value is true if a face and its main features are currently detected, false otherwise. If this value is false, no values are set for @ref initYaw, @ref initRoll and @ref initVelocity.
     */
    bool initFaceDetected;
    /** Initial head yaw.
     * <i>This variable is set only while tracker is initializing (TRACK_STAT_INIT).
	 * Facial features detector leaves this variable undefined.</i>
     *
     * Estimated initial measure of head yaw (left-right rotation of the head). It is expressed in meters, as the deviation of the nose tip position from the imaginary line drawn between the eyes perpendicular to left eye - right eye connecting line. Positive values are to the left in the input image.
     */
    float initYaw;
    /** Inital head roll.
     * <i>This variable is set only while tracker is initializing (TRACK_STAT_INIT) and initFaceDetected is true.
	 * Facial features detector leaves this variable undefined.</i>
     *
     * Estimated initial head roll (tilt of the head to left or right). It is expressed in degrees. Positive values are to the right in the input image.
     */
    float initRoll;
    /** Estimated velocity of nose tip motion, in meters per second.
     * <i>This variable is set only while tracker is initializing (TRACK_STAT_INIT) and initFaceDetected is true.
	 * Facial features detector leaves this variable undefined.</i>
     *
     */
    float initVelocity;
    /** Initialization status display enabled/disabled.
     * <i>This variable is set only while tracker is initializing (TRACK_STAT_INIT).
	 * Facial features detector leaves this variable undefined.</i>
     *
     * It indicates whether the display of initialization status is enabled or disabled by the init_display_status parameter in the tracker configuration file. If it is true, initialization status should be displayed. This allows the display to be controlled from the configuration file.
     */
    bool initDisplayStatus;

    
    bool isDataInitialized; // true if tracking data is initialized, false otherwise

	FaceData();
	~FaceData();
};
}

#endif