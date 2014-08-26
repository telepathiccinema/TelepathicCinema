
#include <cv.h>
// Requires software renderer source
#include "rasterizer_subdivaffine.h"
#include "span.h"
#include "geometry_processor.h"
#include "vector_math.h"
#include "SwrTexture.h"


// the software renderer stuff is located in the namespace "swr" so include
// that here
using namespace swr;
using namespace vmath;

typedef vec2<float> vec2x;
typedef vec3<float> vec3x;
typedef vec4<float> vec4x;
typedef mat4<float> mat4x;

// Our vertex structure which will be used to store our triangle data.
struct Vertex {
	float x, y, z;
};

struct TextureVertex {
	float x, y, z;
	float tx, ty;
};

typedef struct Color{
	int r, g, b;
} Color3i;





// This is the vertex shader which is executed for each individial vertex that
// needs to ne processed.
struct VertexShader : public GeometryProcessor::VertexShaderI {

	VertexShader(): VertexShaderI(1,0) {}

	
	mat4x viewprojection_matrix;

	// This static function is called for each vertex to be processed.
	// "in" is an array of void* pointers with the location of the individial
	// vertex attributes. The "out" structure has to be written to.
	void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out);

	
	Color3i color;

	void setColor(int r, int g, int b);
};



// This is the fragment shader
struct FragmentShader : public FragmentShaderI {


	FragmentShader(char *imageData,
	unsigned short *zBuffer16,
	int swrWidth,
	int swrHeight);
	

	// Per triangle callback. This could for instance be used to select the
	// mipmap level of detail. We don't need it but it still needs to be defined
	// for everything to work.
	void begin_triangle(
		const IRasterizer::Vertex& v1,
		const IRasterizer::Vertex& v2,
		const IRasterizer::Vertex& v3,
		int area2);

	// the fragment shader is called for each pixel and has read/write access to
	// the destination color and depth buffers.
	void single_fragment(int x, int y,
		const IRasterizer::FragmentData &fd,
		unsigned short &color,
		unsigned short &depth);

	// this is called by the span drawing function to get the location of the color buffer
	void* color_pointer(int x, int y);

	// this is called by the span drawing function to get the location of the depth buffer
	void* depth_pointer(int x, int y);

	unsigned short *zBuffer16;
	int swrWidth;
	int swrHeight;
	char *imageData;
};

void swrView(float *in, float *out, int n, float f, int width, int height);



void rgb888_2_rgb565(char *rgb888, unsigned short *rgb565);

void rgb565_2_rgb888(unsigned short rgb565, char *rgb888);


// loads an image file using SDL_image and converts it to a r5g5a1b5 format.
// this format can be directly copied to the screen and one can also use the
// embedded alpha bit for an alpha test.
unsigned short * load_surface_r5g5a1b5(IplImage* img);


// this is the vertex shader which is executed for each individual vertex that
// needs to ne processed.
struct TextureVertexShader : public VertexShader {


	TextureVertexShader(): VertexShader() {}

	// this static function is called for each vertex to be processed.
	// "in" is an array of void* pointers with the location of the individial
	// vertex attributes. The "out" structure has to be written to.
	void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out);

	SwrTexture *texture;
};

// this is the fragment shader
struct TextureFragmentShader : public FragmentShader {


	TextureFragmentShader(unsigned short *imageData16,
	char *imageData,
	unsigned short *zBuffer16,
	int swrWidth,
	int swrHeight);

	
	// the fragment shader is called for each pixel and has read/write access to
	// the destination color and depth buffers.
	void single_fragment(int x, int y, const IRasterizer::FragmentData &fd, unsigned short &color, unsigned short &depth);

	// this is called by the span drawing function to get the location of the color buffer
	void* color_pointer(int x, int y);


	SwrTexture *texture;

	unsigned short *imageData16;
	
};
