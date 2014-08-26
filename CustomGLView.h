//
//  Abstract: This class wraps the CAEAGLLayer from CoreAnimation into a convenient
//  UIView subclass. The view content is basically an EAGL surface you render your
//  OpenGL scene into.  Note that setting the view non-opaque will only work if the
//  EAGL surface has an alpha channel.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

/**
 * Class that implements OpenGL ES View.
 *
 * It is used by the TrackerWrapper to display video frame and wireframe of the 3D face model acquired from the tracker using OpenGL calls. 
 * 
 */
@interface CustomGLView : UIView
{
@private
    /**
     * The pixel dimensions of the backbuffer
     * 
     */
	GLint backingWidth;
	/**
     * The pixel dimensions of the backbuffer
     * 
     */
    GLint backingHeight;
	
    /**
     * OpenGL context
     * 
     */
	EAGLContext *context;

    /**
     * OpenGL names for the renderbuffer and framebuffers used to render to this view
     * 
     */
	GLuint viewRenderbuffer, viewFramebuffer;

    /**
     * OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist)
     * 
     */
	GLuint depthRenderbuffer;
}

@property (nonatomic) GLint backingWidth;
@property (nonatomic) GLint backingHeight;

/*
 * Method for drawing the view.
 * 
 * 
 */
-(void)drawView;
/**
 * Method for setting the OpenGL context.
 * 
 * This method binds the framebuffer and clears the buffer. It is called before calling any OpenGL drawing functions.
 */
-(void)setOpenGLContext;
/**
 * Method for swapping OpenGL buffers.
 *
 * Displays the frambuffer. It is called after OpenGL drawing functions to show the results. 
 */
-(void)swapOpenGLBuffers;

@end
