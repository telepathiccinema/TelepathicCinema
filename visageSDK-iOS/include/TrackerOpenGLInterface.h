
#ifndef __TrackerOpenGLInterface_h__
#define __TrackerOpenGLInterface_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif


#include <cstdlib>

using namespace std;

namespace VisageSDK
{

/** Optional OpenGL Interface for VisageTracker2.
*
* OpenGL Interface is used to provide an OpenGL context to the tracker, allowing tracker to draw the tracking results. 
*
* The main use is to display detailed tracking results as help while <a href="doc/VisageTracker Configuration Manual.pdf">configuring the tracker</a>.
* For other purposes, applications should implement their own display.
*
* All functions in this class are virtual and must be implemented in the 
* inherited class.
*
* \if IOS_DOXY
* \elseif ANDROID_DOXY
* \else
* The full implementation of an TrackerOpenGLInterface is
* provided, (with source code) in the FaceTracker2 sample, where the
* class COpenGLWnd is an implementation of the TrackerOpenGLInterface.
* \endif
*/
class VISAGE_DECLSPEC TrackerOpenGLInterface
{
public:
	/** A virtual function to set the OpenGL context.
	*
	* VisageTracker2 calls this function when it needs to clear the OpenGL
	* window and set the OpenGL context. The function sets to OpenGL context
	* to a context dedicated to the tracker, then clears the window.
	*/
	virtual void setOpenGLContext() = 0;

	/* A virtual function to verify the OpenGL window size.
	*
	* VisageTracker2 calls this function to verify the window size
	* of the OpenGL context. It passes the requested size to
	* this function. The function must return the actual size of the
	* window.
	* If the actual size is smaller than the requested size, the function may
	* do one of the following:
	*
	* a) Increase the size of the window to the requested size, then return this new size.
	*
	* a) Simply return the actual size without changing it. The tracker will then use the smaller
	* window. This may result in degradation of tracking performance so if the function does this it
	* is recommended to issue a warning to the user.
	*
	* @param width input: requested window width, output: actual window width
	* @param height input: requested window height, output: actual window height
	*/
	virtual void checkOpenGLSize(unsigned int &width, unsigned int &height) = 0;

	/** A virtual function to swap OpenGL buffers.
	*
	* VisageTracker2 calls this function when it needs to swap
	* OpenGL buffers. The function calls the OpenGL commands to swap buffers.
	*/
	virtual void swapOpenGLBuffers() = 0;

};

}
#endif // __TrackerOpenGLInterface_h__

