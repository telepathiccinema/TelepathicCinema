
#ifndef __FbaAction_h__
#define __FbaAction_h__

#include "FBADecoder.h"
#include "FBAEncoder.h"

// C++
#include <stdlib.h>
#include <stdio.h>

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else
	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif
#endif
namespace VisageSDK
{

class VisageCharModel;

/**
* A generic face and body animation action.
* The FbaAction is an abstract class representing a face and body animation action, i.e. a face and body
* animation sequence. Essentially, an implementation of this class must be capable of returning
* a set of Face and Body Animation Parameters (FBAPs) for a given time. Some implementations of 
* FbaAction are provided in the SDK (e.g. FbaFileAction, Visagesapi5tts, VisageLipSync), others can be implemented by the
* user.
*
* Tha FbaAction is passed to the FAPlayer, which takes one or more FbaActions and plays them. This
* provides a simple but powerful mechanism for mixing and playing multiple animations either in real
* time or off line.
*
* FbaAction is the base class for various kinds
* of animations that can be implemented as subclasses of FbaAction. A good example of
* such a subclass is FbaFileAction, a special type of FbaAction that reads the animation
* sequence from a file. Other types can be implemented by the user, e.g. to generate
* procedural animations, handle motion capture or facial tracking devices, lip sync, speech
* synthesis etc. Any kind of animation source can be an FbaAction, and thus it can be played in the FAPlayer
* in the standard way. Another example of an FbaAction is Visagesapi5tts class, which implements full SAPI-5
* speech synthesis and generates animation synshronised to the speech. Because it is implemented as a subclass
* of FbaAction, it can simply be added to the FAPlayer and played as any other animation.
* The lip sync mechanism in visage|SDK is also implemented in this way.
*
* The main responsability of an FbaAction implementation is to be able to provide
* the set of face and body animation parameters for a given time of animation. To make the implementation
* of FbaAction subclasses more convenient, the basic class SimpleFbaAction is provided. It implements all
* required functions of the FbaAction, so the user just needs to extend it by inserting the mechanism
* for setting the Face and Body Animation Parameters in the getFBAPs() function. For convenience, full source code of
* the SimpleFbaAction class can be found in the src folder.
* 
* Additionally, an FbaAction may also be used to filter or in other way influence the result of other FbaActions
* played in the player. For example SimpleVORAction does this to provide a gaze following behavior where the
* eye rotation counters the head rotation, so that it appears that the face is always gazing straight agead at the user.
*
* Implemented in visagefaplayer.lib
*
* Demonstrated in <a href="../../../Samples/OpenGL/build/msvc90/SimplePlayer/doc/index.html">Example1</a>, <a href="../../../Samples/OpenGL/build/msvc90/SpeechSynthesis/doc/index.html">Example6</a>, and <a href="../../../Samples/OpenGL/build/msvc90/LipSync/doc/index.html">Example7</a>.
*
* @see FbaFileAction
* @see SimpleFbaAction
* @see Visagesapi5tts
* @see VisageLipSync
* @see SimpleVORAction
*/
class VISAGE_DECLSPEC FbaAction  
{
public:

	// Added by KStankovic
	/**
	* Destructor.
	*/
	virtual ~FbaAction() {}
	//

	/**
	* Get FBAPs. This function should return the Face and Body Animation Parameters (FBAPs) for the time globalTime, or NULL to indicate that the action has finished.
	* This function is called by the FAPlayer when the FbaAction is played in the FAPlayer.
	*
	* If the player is in real time mode, the passed globalTime will correspond to the real time. If the player is in the off-line mode,
	* this will typically not correspond to the real time (it may be slower or faster, or even nonlinear). Therefore, if the FbaAction implements a live source of animation
	* (e.g. real-time motion capture, speech synthesis, on-the-fly lip sync etc.) it should be used only with the FAPlayer set to
	* real time mode, otherwise the results may be unpredictable.
	*
	* In most cases, the implementation of getFBAPs() will calculate the FBAPs based only on time. However, two more
	* parameters are provided and can be used if necessary.
	*
	* lastFBAPs contains the final FBAPs from the previous frame of animation. They are the results from all FbaActions
	* currently played by the player. By using this information, a special FbaActions may be implemented to act as
	* a sort of filter or modifier on other actions. One example is the implementation of automatic gaze following
	* where the eye rotation is set oposite of head rotation, in such a way that the eyes always look straight
	* ahead even when the head is turning. This kind of effect can be implemented by finding the head rotation in the 
	* lastFBAPs parameter, and using it to counter-rotate the eyes to achieve steady gaze, as illustrated in SimpleVORAction.
	*
	* The afm parameter is the Animatable Face and Body Model (AFM) currently used in the player. It may be useful in case
	* the FbaAction implementation needs to get some specific information about the model, e.g. size etc.
	*
	* @param globalTime the time for which the FBAPs are requested, in milliseconds
	* @param lastFBAPs the final FBAPs from the previous frame of animation
	* @param model the character model currently used in the player; this may be NULL so it must be checked before using it
	*
	* @return the Face and Body Animation Parameters (FBAPs) for the requested time, or NULL to indicate that the action has finished.
	*/
	virtual FBAPs *getFBAPs(long globalTime, FBAPs *lastFBAPs, VisageCharModel* model)=0;


	/**
	* Start the action.
	* This function is called by the FAPlayer when the playing starts. It may be called many times. The current global time
	* is passed, so the FbaAction may use this as the reference start time. Typically,
	* this reference time is subtracted from the time passed to getFBAPs() in order to obtain the
	* time elapsed since the start of the action.
	*
	* @param globalTime the reference time when the action is started, in milliseconds
	*/
	virtual void start(long globalTime)=0;

	/**
	* Stop playing the action. Stop and reset the animation action.
	* This is called by the FAPlayer when the animation in the player needs to stop.
	*/
	virtual void stop()=0;

	/**
	* Get the name of the action type.
	* This may be used to distinguish between various types of actions. It is recommended to return the name of the class, e.g. "SimpleFbaAction"
	*
	* @return the name of the action type.
	*/
	virtual char* actionTypeName()=0;



private:
};

}

#endif // __FbaAction_h__
