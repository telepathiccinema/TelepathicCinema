//FBFT.h
//declaration of the face tracker class

#ifndef FBFT_H
#define FBFT_H

//#define _CRTDBG_MAP_ALLOC //MEMORY LEAK DEBUGGING
//#include <stdlib.h>
//#include <crtdbg.h>


// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the VISAGETRACKER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// VISAGE_DECLSPEC functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#define SOFTWARE_RENDERING
#ifdef SOFTWARE_RENDERING
#include "SwrTexture.h"
#endif
// define to use Extended Information Filter
#define USE_EIF

#include <cv.h>
#include <string>
#include <highgui.h>
#ifdef USE_EIF
/// n3 !!!
	// definitions from EKF.h
	#define LIMIT_R_VALUE 100000.0f //points whose R value is bigger or equal to this value are not processed

	//from types.h (FaceTracker 2)
	typedef float f32;
	typedef double f64;
	typedef char is8;
	typedef unsigned char iu8;
	typedef short int is16;
	typedef unsigned short int iu16;
	typedef int is32;
	typedef unsigned int iu32;
	typedef long int is64;
	typedef unsigned long int iu64;
	
#include "ExtendedInformationFilter.h"
#include "FaceModelNoVel.h"
/// !!!
#else
#include "FBFT/EKF.h"
#endif
#include "FDP.h"

//from func.h
#include <fstream>
#include <sstream>

//from func_gl.h

#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#define ENABLE_OPENGL
#ifdef ENABLE_OPENGL
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif

#if defined(MAC_OS_X) || defined(LINUX)
#define ENABLE_OPENGL
#ifdef ENABLE_OPENGL
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif
#endif

using namespace std;

namespace VisageSDK
{

class VisageDetector;
class VisageTracker2;

/** Low-level face tracker implementation used in VisageTracker2.
*
* Provides access to certain internal tracker information for advanced users,
* in particular the access the the current coordinates of the 3D model used in tracking.
*
*/
class VISAGE_DECLSPEC FBFT
{

friend class VisageDetector;
friend class VisageTracker2;
friend class VisageFeaturesDetector;
friend class ModelFitter;

public:
	//constructor
	FBFT (
		);

	//destructor
	~FBFT (
		);

private:

	//initialisation
	bool init (
		const string &settings //settings to be used
		);
	void init_image_sizes (
		const string &settings, //settings to be used
		iu32 frame_width, //width of video image
		iu32 frame_height //height of video image
		);

	void reInitialiseFromEyeAndMouthPos(IplImage* frame,FDP *fp);
	void fitModelToFeaturePoints(FDP *fp,int imageWidth, int imageHeight);
	void calculatePose(FDP *fp, float *rot, float *tran,float aspect,float &s,float &edi,float &end, float &emd);

	//initiate tracker with a first frame, update tracker with a new frame, or just project the vertices
	void update (
		/*const */IplImage* frame, //a frame, if 0 only projection is done
		bool track //0: initiate tracker with first frame, 1: update tracker with new frame
		);
	//check if r, t or au are out of their boundaries
	bool bad_states (
		);
	//initiate feature points using a frame, should be private
	bool init_feature_points (
		const IplImage* frame //frame used to extract feature points
		);
	//find feature points in a masked image
	int find_good_feature_points (
		const IplImage* image_color, //color image
		const IplImage* mask, //the mask
		iu32 ROI_index //current ROI
		);
	//set predefined feature points
	int set_feature_points (
		iu32 ROI_index //current ROI
		);

	//find_point for all feature points
	void find_points (
		CvMat* search_range,
		CvMat* method, // matching method to use
		f32* maximum_values, // array to store maximum match values for each point, normalised with respect to initial values
		iu32* use_tex, // flag for each point, if 1 textured image is used for search, if 0 previous frame is used
		iu32* point_visible, // flag for each point, if 1 point is visible, if 0 point is hidden so don't try to find it
		bool refine_step = false
		);

	// find iris in each eye, add it to track points to be processed by EKF
	void processEyes();

	// Estimate eye closure based on iris match values
	void processEyesClosure(IplImage* frame, FDP* fdp, float* out);

	// find iris; eye is supposed to be within rect; irisTriangles is a list of triangles covering the iris region
	// returns:
	// - iris position in iris_x, iris_y (in image coordinates)
	// - relative iris position within the eyeRegion in rel_iris_x, rel_iris_y; both values are normalised to 0-1 range
	// - match values
	// - return value: ratio of the longest dominantly vertical edge to the height of the eye region
	float processEye(CvRect *eyeRegion,CvMat *irisTriangles, f32 &iris_x, f32 &iris_y, f32 &rel_iris_x, f32 &rel_iris_y, f32 &maximum_value);
	int traverseVertEdge(IplImage* img,int x, int y,int x0);
	int findLongestVertEdge(IplImage* img);
	float l_eye_vertical_edge, r_eye_vertical_edge; //length of longest vertical edge in left/right eye region

	// compute the valuse of the eye closing AU that performs full eye closing
	void compute_full_eye_closure();

	//set the ekf R values depending on the result of feature point matching
	void set_ekf_R (
		f32* _max_vals
	);
	// resize the data matrices for the current number of tracked featured points
	void resize (
		iu32 _points_num
		);

	//model
	CvMat* g; //fix model vertices
	CvMat* g0; //original version of fix model vertices, needed for re-initialisation
	CvMat* Su; //shape unit matrix
	CvMat* Au; //action unit matrix
	CvMat* tri; //model surface triangles
	CvMat* au_use; //list of action units actually to use, boolean behaviour
	string* au_names;//list of action units names, currently not used //can be used with au_num to determine max array size //added by Pilipovic
	size_t au_names_count;
	f32 f; // Camera focus
	f32 s; //scale
	f32 r[3]; //array of xyz rotation angles
	f32 t[3]; //array of xyz translations
	f32 tzinit; //z translation at initialisation
	f32 dIinit; //eye distance in projection space at initialisation
	CvMat* su; //shape unit vector
	CvMat* au; // action unit vector
	CvMat* vertex0; //model vertices after shape and action units have been added
	CvMat* vertex1; //model vertices after scaling, rotation and translation
	CvMat* vertexN; //model vertices in neutral face
public:
	CvMat* projection;  // Projected model vertices. The 2D coordinates of the model vertices projected onto the display window. The coordinates are normalised to the window height: 0,0 is in the center of the window; Y coordinate of the upper edge is 1, Y coordinate of the lower edge is -1, X coordinate of the left edge is equal to -aspect ratio of the image, and the X coordinate of the right edge is equal to the aspect ratio of the image.
#ifdef EMSCRIPTEN
public:
#else 
private:
#endif
	CvMat* current_tex_coord;  //current texture coordintes; calculated on demand by getTexCoord(). These coordinates are a simple normalisation of coordinates in projection
	//texture
	iu32 tex_size; //size of the texture (size times size pixels, size = power of 2)
	CvMat** tex_coord; //texture coordinates, one CvMat* for each used texture
	iu32 tex_num; //number of textures used
	CvMat* tri_tex; //texture name (index) for each triangle
	//point finding
	iu32 gl_width; //width of opengl buffer
	iu32 gl_height; //height of opengl buffer
	iu32 fast_image_resize;
	iu32 init_gl_width; //width of opengl buffer for initialisation (feature points choosing)
	iu32 init_gl_height; //height of opengl buffer for initialisation (feature points choosing)
	iu32 ROI_num; //number of Regions of Interest (ROI)
	CvMat* min_distance; //the wanted minimum distance between feature points (float, one per ROI)
	CvMat* block_size; //size of averaging block used in opencv func to extract feature points (int, one per ROI)
	CvMat** tri_index; //list of triangle indices forming the mask in which feature points may be inserted (list of integers, one list per ROI)
	CvMat* bound_rect_modifiers; //mask bounding rectangle modifiers, float, four per ROI
	CvMat* max_points_num_ROI; //maximum number of feature points to use for each ROI, int
	CvMat* ROI_init_angle; //ROI track points initialisation angle (float, one per ROI)
	CvMat* ROI_initialised; //Flag indicating if the ROI track points have been initialised (int, one per ROI)
	iu32 max_points_num; //maximum number of feature points to use in total, for all ROIs
	iu32 points_num; // number of feature points actually used
	iu32 model_points_num; // number of points (vertices) in the 3D model itself (without the track points)
	iu32 reserved_points_num; // number of feature points that are allocated but not yet used
	CvMat* preset_points_coord; //set feature point coordinates
	CvMat* points_preset; // indexes to array with preset points mapping
	CvMat* eye_points_coord;
	CvMat* preset_points_ROI;
	CvMat* points_coord; //feature point coordinates
	CvMat* points_ROI; //feature point ROI indices, one int per tracked feature point
	CvMat* search_range; //search range (int, two per ROI - x and y) when using prev image to search for feature points
	CvMat* refine_range; //search range (int, two per ROI - x and y) when using drawn model to search fro feature points
	CvMat* patch_size; //size of the patch (float, one per ROI) to match with (patch_size times patch_size pixels)
	CvMat* method; //matching method (int, one per ROI)
	CvMat* bad_match_threshold; // minimal match value for a track point (float, one per ROI); corrective action taken when match value falls below threshold
	f32 global_bad_match_threshold; // minimum average match value for all track points; if value is below this threshold the tracker will re-initialise
	iu32 visibility_check; // are we doing visibility check? 0 = no; 1 = normals; 2 = color coding; 3 = both
	CvMat* sensitivity_falloff; // a parameter that controls how fast the tracker sensitivity to a particular track point decreases when the match quality for that point decreases (float, one per ROI). Higher values result in sharper falloff, i.e. by increasing this value the tracker increasingly ignores points with bad match values.
	f32* max_vals; // match quality values for each feature point, expressed as ratio to the value from the first frame
	f32* tex_max_vals; // match quality values from texture-based matching for each feature point, expressed as ratio to the value from the first frame
	float tracking_quality; // match quality average for this frame
	float tracking_quality_bdts; // bdts quality average for this frame
	IplImage* prev_frame; //the previous frame
	IplImage* cur_frame; //the current frame
	iu32 n_recovery_frames; // number of recovery frames to be used
	iu32 recovery_frames; // recovery frames counter
	FDP *fp_corres; // FDP structure used to store correspondence between MPEG-4 Feature Points and vertices in the 3D model
	VisageTracker2 *vt;
	float* init_angle_hyst;

	// eye processing stuff
	iu32 process_eyes; // process eye rotation and closure
	f32 full_eye_closure; // value of eye closure AU corresponding to full eye closure; set automatically when initialising, after the 3D model has been fitted
	iu32 eye_closing_au;
	iu32 leye_closing_au; // AU that closes the eyes; candide hardcoding - should be configurable
	iu32 reye_closing_au;
	iu32 eye_h_rotation_au; // AU that horizontaly the eyes; candide hardcoding - should be configurable
	iu32 eye_v_rotation_au; // AU that horizontaly the eyes; candide hardcoding - should be configurable
	float eye_closure[2];
	void fix_eye_points(int idx);

	//working images
	IplImage* gl_image_color;
	IplImage* image_color;
	IplImage* image_index;
	IplImage* image;
	IplImage* patch_image_tex; // patch image created by projecting the textured model

	iu32 frame_tex_id;
	f32 tex_x_coord;
	f32 tex_y_coord;

	IplImage* result_image_buffer;

 	// interpupillary distance
	float ipd;
	void setIPD(float value);
	float getIPD();

	float face_scale;

	void setTMSizes(FDP* featurePoints2D);

	static const float EYE_OPEN_THRESHOLD;

#ifdef USE_EIF
	// n3
	/*
		for EIF implementation
	*/
	SystemModel* n3Sys;
	FaceModel* n3Fm;
	ExtendedInformationFilter* n3Eif;
	unsigned int frame_num;

	void n3_eif_init();
	void n3_eif_reset(const f32* r, const f32* t, const CvMat* au);
	void n3_extract_data(float r[3], float t[3], CvMat* au, CvMat* x);

	/*
		...
	*/
#else
	//ekf
	EKF* ekf; //the extended kalman filter
#endif
	bool ekf_failure; // set to true when EKF update fails
	CvMat* ekf_sensitivity; //the ekf sensitivity for 3 rotations, 3 translations and 14 AUs
	CvMat* au_gravity; //A gravity factor, one per each Action Unit, that determines the behavior of the Action Unit in the situation when it is not driven by track points because they are not found (e.g. they are hidden or temporarily lost). In such case, AU value is multiplied by the gravity factor. Therefore, gravity factor 1 is neutral, less than 1 means that AU value will fall (faster if gravity factor is smaller), and a value larger than 1 would make the AU value rise.
	f32 gravity_threshold; //Threshold value that determines when the gravity factor starts to influence the Action Units. Then the Kalman gain for all track points influencing the AU falls below this threshold, gravity kicks in.
	CvMat* points_au; //An integer matrix of au_num rows and points_num columns determining which track points drive which AU by values of 1 or 0.
	CvMat* points_Au; //For each track point, a value of 0 indicates that it does not drive any AUs, value is 1 otherwise.
	CvMat* points_tri; //Triangle index for each track point.
	CvMat* r_limit; //rotation boundaries
	CvMat* t_limit; //translation boundaries
	CvMat* au_limit; //action unit boundaries

	VisageDetector* m_DetectorBDF;
	FDP *featurePoints2D;

	int bdts_points_num;
	iu32 use_bdts_points;
	int bdts_points_inited;

	CvMat* bdts_points_use;
	CvMat* bdts_points_sensitivity;
	CvMat* bdts_points_angle;
	CvMat* bdts_points_nptrb;

	void find_bdts_points(IplImage* frame, FDP* fdp, CvPoint2D32f* points, float *vals);
	void init_bdts_points(IplImage* frame, FDP* fdp);

	FILE *log_file; //log file - used for debugging, set via log_filename configuration parameter

	//stuff copied from main.cpp in Nils' original project
	//bool pause_video = 0; //is video paused?


	//from func.h

	//read a file into a string, returns the string
	string read_file (
		const string &filename //the filename
		);

	//write the contents of a string to a file
	void write_file (
		const string &filestring, //the string to be written
		const string &filename //the filename
		);

	//read a wfm file containing face model data
	bool read_gSuAu_wfm (
		const string &file_name //the filename
		);

	//read profile from string
	void read_profile (
		const string &filestring, //the string
		string &texture // texture image file name, output
		);



	//write scale and shape units to file
	void write_profile (
		const string &filename,
		IplImage* texture_image // texture image
		);

	//read rotation, translation and action units from string
	void read_states (
		const string &filestring, //the string
		f32* r, //3 sized array of xyz rotation angles
		f32* t, //3 sized array of xyz translations
		CvMat* au //action unit vector
		);

	//write rotation, translation and action units to string, returns the string
	string write_states (
		const f32* r, //3 sized array of xyz rotation angles, output
		const f32* t, //3 sized array of xyz translations, output
		const CvMat* au //action unit vector, output
		);

	//read a setting from a setting string, integer
	void read_setting (
		const string &settings, //setting string
		const string name, //setting name
		iu32 &value //setting value, output
		);

	//read a setting from a setting string, floating point
	void read_setting (
		const string &settings, //setting string
		const string name, //setting name
		f32 &value //setting value, output
		);

	//read a setting from a setting string, text
	void read_setting (
		const string &settings, //setting string
		const string name, //setting name
		string &value //setting value, output
		);

	//read a setting list (vector) from a setting string, floating point
	void read_settings_list_f32 (
		const string &settings, //setting string
		const string name, //setting name
		CvMat* list //list (vector) of values, output
		);

	//read a setting list (vector) from a setting string, integer
	void read_settings_list_iu32 (
		const string &settings, //setting string
		const string name, //setting name
		CvMat* list //list (vector) of values, output
		);

	//read a setting array from a setting string, integer
	void read_setting_array (
		const string &settings, //setting string
		const string name, //setting name
		const iu32 n, //size of array
		int *value //setting value array, output
		);

	//read a setting array from a setting string, floating point
	void read_setting_array (
		const string &settings, //setting string
		const string name, //setting name
		const iu32 n, //size of array
		const iu32 l, //size of each row
		float *value, //setting value array, output
		bool default_first //if true, default value for any setting that was not found is taken from the first member of array; if false, default values are 0
		);

	//read a setting array from a setting string, returns string array of @param size elements
	void read_settings_list_string(
		const string &settings, //setting string
		const string name, //setting name
		int size,
		string *list //list of names, output
		);


	//from func_gl.h

	void gl_swap_rb (
		IplImage* image
		);

	//set the camera and its view (frustum)
	void gl_view (
		iu32 width, //width of image to be drawn
		iu32 height, //height of image to be drawn
		f32 f //focus
		);

	//initialize frame texture
	void gl_init_frame_tex (
		iu32 x_size,
		iu32 y_size,
		const IplImage *image
		);

	//display video as texture
	void gl_display_video (
		iu32 width,
		iu32 height,
		const IplImage *image
		);

	//load an image to the buffer
	void gl_load_image (
		const IplImage* image //the image to load
		);

	//save the buffer to an image
	void gl_save_image (
		IplImage* image //image to save to, output
		);

	//fix model vertices + shape action units + scale rotation translation + projection -> image coordinates
	void gl_project (
		f32 f, //focus
		f32 s, //scale
		const f32* r, //3 sized array of xyz rotation angles
		const f32* t, //3 sized array of xyz translations
		const CvMat* su, //shape unit vector
		const CvMat* au, //action unit vector
		const CvMat* g, //fix model vertex list
		const CvMat* Su, //shape unit matrix
		const CvMat* Au, //action unit matrix
		CvMat* vertex0, //model vertices after shape and action units have been added, output
		CvMat* vertex1, //model vertices after scaling, rotation and translation, output
		CvMat* projection //projected model vertices, output
		);

	//compute and return texture coordinates
	const float *getTexCoord(float aspect);

	//load an image as an opengl texture and calculate texture coordinates from projection
	void gl_set_texture (
		const IplImage* image, //the texture image
		iu32 tex_size, //size of the texture (size times size pixels, size = power of 2)
		iu32 tex_name, //number to bind texture with
		const CvMat* projection, //projected model vertices
		CvMat** tex_coord //texture coordinates, output
		);

	//draw selected model triangles using their index number (32bit using 3x8bit)
	void gl_draw_model_tri_index (
		iu32 width, //width of drawn image
		iu32 height, //height of drawn image
		f32 f, //focus
		const CvMat* vertex1, //model vertices after scaling, rotation and translation
		const CvMat* tri, //model surface triangles
		const CvMat* tri_index //list of triangle indices
		);

	//draw selected model triangles using a specific color
	void gl_draw_model_tri (
		iu32 width, //width of drawn image
		iu32 height, //height of drawn image
		f32 f, //focus
		const CvMat* vertex1, //model vertices after scaling, rotation and translation
		const CvMat* tri, //model surface triangles
		const CvMat* tri_index, //list of triangle indices
		const f32* color //3 sized array containing the color
		);

	//draw textured model
	void gl_draw_model_texture (
		iu32 width, //width of drawn image
		iu32 height, //height of drawn image
		f32 f, //focus
		const CvMat* vertex1, //model vertices after scaling, rotation and translation
		const CvMat* tri_index, //list of triangles to draw, indicated by triangle index, or NULL to draw all triangles
		const CvMat** tex_coord //list of texture coordinates for the vertices; one list for each used texture
		);

	//draw model as a wireframe using a specific color
	void gl_draw_model_wireframe (
		iu32 width, //width of drawn image
		iu32 height, //height of drawn image
		f32 f, //focus
		const CvMat* vertex1, //model vertices after scaling, rotation and translation
		const CvMat* tri, //model surface triangles
		const f32* color //3 sized array containing the color
		);

	//draw model vertices starting from a specific vertex index using a specific color
	void gl_draw_points (
		iu32 width, //width of drawn image
		iu32 height, //height of drawn image
		f32 f, //focus
		const CvMat* vertex1, //model vertices after scaling, rotation and translation
		iu32 first_point, //the first vertex index
		iu32 num_points, //number of points to draw
		const f32* color, //3 sized array containing the color
		f32 point_size, //size in pixels of the drawn points representing the vertices
		f32 *color_multiplier // one value per point, multiplies the color of that point; ignored if NULL

		);

	#ifdef SOFTWARE_RENDERING
	#define MAX_SWR_TEX 30
	SwrTexture *swr_texture[MAX_SWR_TEX];

	unsigned short *zBuffer16;
	int swrWidth;
	int swrHeight;
	char *imageData;
	unsigned short *imageData16;

	void swr_load_image (
		const IplImage* image
		);

	void swr_save_image (
		IplImage* image
		);

	void swr_set_texture (
		const IplImage* image,
		iu32 tex_size,
		iu32 tex_name,
		const CvMat* projection,
		CvMat** tex_coord
		);

	void swr_draw_model_tri_index (
		iu32 width,
		iu32 height,
		f32 f,
		const CvMat* vertex1,
		const CvMat* tri,
		const CvMat* tri_index
		);

	void swr_draw_model_texture (
		iu32 width,
		iu32 height,
		f32 f,
		const CvMat* vertex1,
		const CvMat* tri_index,
		const CvMat** tex_coord
		);

	#endif

};

}

#endif
