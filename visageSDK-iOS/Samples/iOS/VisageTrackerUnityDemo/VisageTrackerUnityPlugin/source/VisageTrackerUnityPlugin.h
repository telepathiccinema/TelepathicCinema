//
//  VisageTrackerUnityPlugin.h
//  VisageTrackerUnityPlugin
//
//

#ifdef WIN32
#if _MSC_VER
// ensures valid dll function export
#define EXPORT_API __declspec(dllexport)
#endif
#elif defined (IOS)
#import <Foundation/Foundation.h>
#define EXPORT_API 
#endif

/** \file VisageTrackerUnityPlugin.h
 \brief Provides high-level functions for using visage|SDK face tracking capabilities in Unity game engine.
 
 It implements high-level wrapper functions around key functionalities from visage|SDK that enable usage of visage|SDK face tracking capabilities in Unity C# scripts.
 */
extern "C" {
    /** This functions initialises the tracker.
     */
	EXPORT_API void _initTracker(char* config);
    /** This functions starts the tracker.
     */
	EXPORT_API void _startTracker();
    /** This functions starts tracking from cam.
     */
    EXPORT_API bool _trackFromCam();
    /** This functions stops the tracker.
     */
	EXPORT_API void _stopTracker();
    /** This functions returns tracking reults as a string.
     */
	EXPORT_API const char* _getTrackingResults();
    /** This functions binds a texture with the given native hardware texture id through OpenGL.
     */
	EXPORT_API void _bindTexture(int texID);
    /** This functions returns the current head translation, rotation and tracking status.
     */
	EXPORT_API void _get3DData(float* tx, float* ty, float* tz,float* rx, float* ry, float* rz, int* status);
    /** This functions returns camera info.
     */
    EXPORT_API bool _getCameraInfo(float *focus, int *width, int *height);
    /** This functions returns data needed to draw 3D face model.
     */
	EXPORT_API bool _getFaceModel(int* vertexNumber, float* vertices, int* triangleNumber, int* triangles, float* texCoord);
	
}
