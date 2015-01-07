
#ifndef __Common_h__
#define __Common_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include <fstream>
#include <sstream>
#include <cv.h>
#include <highgui.h>
#include <string>
#include <stdio.h>

#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
	#define ChangeDir _chdir
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
	#define ChangeDir chdir
 #endif


namespace VisageSDK
{

class FDP;

/**
* Provides common functionalities for VisageTrack
*/

class VISAGE_DECLSPEC Common
{
public:

	/**	Given a greyscale, 32f image, compute the mean pixel value and subtract it from each pixel, then normalize
		to one.
		@param inputImage the input image
		@param mask 

	*/
	static void SubtractAvgNorm(IplImage* inputImage, IplImage* mask = NULL);

	/**	Given a greyscale, 32f image, compute the mean pixel value and subtract it from each pixel.
		@param inputImage the input image
		@param mask 

	*/
	static void SubtractAvg(IplImage* inputImage, IplImage* mask = NULL);

	/**	Given a greyscale, 32f image, compute the mean pixel value and subtract it from each pixel, setting the 
		mean to zero, then set the variance to one.

		@param inputImage the input image
		@param mask 

	*/
	static void SubtractAvgNormalizeStd(IplImage* inputImage, IplImage* mask);

	/**	Performs bilinear interploation of image img at location (x,y), works both for greyscale,
		BGR and Normalized BGR color images. Whe the input image is greyscale, the function returns the 
		interpolated value, while the arguments b,g,r are set to -1. On the other hand, when the image has 3 channels,
		the reuqested values are stored in b,g,r and the function returns 1.
		If (x,y) is on the image border, the pixel value is replicated.
		If all the neighbours of the pixel containing (x,y) are outside the image, -1 is returned and bgr are also set to -1
	
		@param img IplImage to be sampled, can be greyscale or BGR or NBGR, only float32 represenation
		@param x X coordinate of the requested position>
		@param y Y coordinate of the requested position
		@param b Interpolated value for the Blue channel, set to -1 if the image is greyscale or if we try to sample outside the image borders
		@param g Interpolated value for the Green channel, set to -1 if the image is greyscale or if we try to sample outside the image borders
		@param r Interpolated value for the Red channel, set to -1 if the image is greyscale or if we try to sample outside the image borders
		@Returns The interpolated value if the image in input is greyscale, 1 for a color image and -1 if the wished position for samplign is entirely outside the image
	*/
	static float Interpolate2d(const IplImage& img, float x, float y, float* b = NULL, float* g = NULL, float* r = NULL);

	/**	Bilinear interpolation at point (x,y) for a greyscale 32f image
	*/
	static float Interpolate2dGRAY32(const IplImage& img, float x, float y);

	/**	Bilinear interpolation at point (x,y) for a greyscale 8uchar image
	*/
	static uchar Interpolate2dGRAY8(const IplImage& img, float x, float y);

	/**	Convert a BGR unsiged char image to a NormnalizedBGR float32 image
		 Input and output images shall have the right depth and number of channels
		@param in The input BGR 8u image
		@param out The outpuyt NBGR 32f image		
	*/
	static void ConvertImageToNormalizedBGR32f(const IplImage* in, IplImage* out);


	/**	Normalizes a grey 32f image, i.e., pixel values are normalized between 0 and 1
		@param in The input 32f grey image
		@param out The output 32f grey image, all the pixels will be between 0 and 1
	*/
	static void NormalizeImg32f(IplImage* in, IplImage* out, IplImage* mask = NULL);


	/**	Converts a BGR 8u image into a grey 32f one, the resulting image is normalized (all the pixels are between 0 and 1)
				 The images passed to the function should be already allocated and with the right depth/num of channels
		@param in The BGR 8u input image
		@param out The greyscale float32 oputput image
		@param temp Optional temporary image, if preallocated speeds up the processing, especially if the conversion has to be performed 
							for each frame in a sequence and the frames have all the same size
	*/
	static void ConvertImageFrom8u3To32f1(const IplImage* in, IplImage* out, IplImage* temp = 0);


	/**	Converts a generic image to a grey 32f one, the resulting image is normalized (all the pixels are between 0 and 1)
		The images passed to the function should be already allocated and with the right depth/num of channels
		@param in Input image (can be BGR or RGB 8u or 32f)
		@param out The greyscale float32 oputput image
		*/
	static void ConvertImageTo32f1(const IplImage* in, IplImage* out);

	/**	Converts a grey 32f image to a grey 8u image, the resulting image is normalized (all the pixels are between 0 and 255)
		The images passed to the function should be already allocated and with the right depth/num of channels
		@param in Input image (should be grey 32f)
		@param out The greyscale 8u oputput image
		@param mask permits to consider only part of the pixels
		*/
	static void ConvertImageFrom32fTo8u(const IplImage* in, IplImage* out, IplImage* mask);


	static void FillImageSides(const IplImage* in, IplImage* out, IplImage* mask, IplImage* mask2, bool show);


	#pragma region Utility functions for I/O

	/// \brief Extracts the filename extension from a path+filename.
	///
	/// Ruthlessly stolen from osgDB.
	inline static std::string getFileExtension(const std::string& fileName)
	{
			std::string::size_type dot = fileName.find_last_of('.');
			if (dot==std::string::npos) return std::string("");
			return std::string(fileName.begin()+dot+1,fileName.end());
	}

	/// \brief Extracts the filename extension (in lower case) from a path+filename.
	///
	/// Ruthlessly stolen from osgDB.
	inline static std::string getLowerCaseFileExtension(const std::string& filename)
	{
			std::string ext = Common::getFileExtension(filename);
			for(std::string::iterator itr=ext.begin();
					itr!=ext.end();
					++itr)
			{
					*itr = static_cast<char>(tolower(*itr));
			}
			return ext;
	}
	/** Sets the path to where the config file is located. Copies the
		path in the local char array _cfgDirPath
		@param fname Input char array containing path to the configuration file
		*/
	inline static void setCfgDirPath(const char* cfgPath) {
		char cfgPathCopy[200];
		strcpy(cfgPathCopy, cfgPath);
		_cfgDirPath[0] = '\0';
		std::string fnstr((const char*)cfgPathCopy);
		int slashPos = (int)fnstr.find_last_of( "/\\" );
		if( slashPos != std::string::npos )
			strcpy( _cfgDirPath, fnstr.substr( 0, slashPos + 1).c_str() );
	}

	/** Gets the path to where the config file is located.
		Copies the data from local char array to the argument char array.
		@param outPath Pointer to the char array where the path will get copied to
		*/
	inline static void getCfgDirPath(char* outPath) {
		if (_cfgDirPath == NULL || _cfgDirPath[0] == '\0'){
			outPath[0] = '\0';
		}
		else
			strcpy(outPath, _cfgDirPath);
	}

	inline static void getPath(const char* fname, char* path) {
		// this code doesn't work right
		// see, that's what happens when you use pointers and arrays
		// instead of strings and containers
		/*char *p1 = strrchr(path,'\\');
		char *p2 = strrchr(path,'/');
		if(p2 > p1) p1 = p2;
		if(p1) *p1 = 0; else path[0] = 0;*/
		std::string fnstr( fname );
		int pi = (int)fnstr.find_last_of( "/\\" );
		if( pi == std::string::npos ) path[0] = 0;
		else strcpy( path, fnstr.substr( 0, pi ).c_str() );
	}

	///gets the next line in a stream
	static inline std::string getNextLine( std::ifstream& is )
	{
		std::string line;
		is.ignore(10000, '\n');
		do
		{
			std::getline( is, line );
		}
		while( line.length() == 0 );
		return line;
	}

	///Int to string 
	static inline void IntToString( std::string &s, int n ){
		std::ostringstream oss;
		oss << n;
		std::string tmp = oss.str();
		s.swap( tmp );
	}


	/// A commented line is a line starting with a given character (defaults to '#').
	/// Empty lines are skipped as well.
	static inline void skipComments( std::istream& is, char comment = '#' )
	{
		char next = static_cast<char>(is.peek());
		while( next == '\n' || next == comment ) {
			is.ignore( 10000, '\n' );
			next = static_cast<char>(is.peek());
		} 
	}


	static inline void skipComments( FILE* is, char comment = '#' )
	{

		int ch;
	
		ch = getc( is );        
		if ( ch==comment ) {
					
			do { 
				 ch=fgetc( is ); 
			 } 
			while( ch!=EOF );
			ungetc( ch, is );

			/*do { 
				ch=fgetc( is ); 
			 } while( ch==' ' || ch=='\t' );
			 ungetc( ch, is );*/

			//SkipRestOfLine();                        
			//SkipComments();
			//skipComments(is);

		} else {
			ungetc( ch, is );
		}

	}

	inline static void replaceSlashes(const std::string* source, std::string* dest) {
		
		*dest = *source;

		int index = 0;
		source->find( "\\", index );

		while(index){

			dest->replace(index, 1, "//");
			source->find( "\\", index );

		}


	}

#pragma endregion

	/**Show an image, normalizing it between 0 and 1 if necessary and specifying the time of display

	*/
	static void Show(IplImage* img, bool norm = false, int t = 1, const char *window_name = 0);	
    
    static void CLAHE(IplImage* dest, IplImage* src, int di = 8, int dj = 8, unsigned char s = 2);

    static void DrawFDPs(IplImage* img, FDP* fdps, int type = 0, bool flip = false);

	static char _cfgDirPath[200];
private:
    static void RCLAHEM(unsigned char imap[], unsigned char im[], int i0, int j0, int i1, int j1, int ldim, unsigned char s);
};

}

#endif // __Common_h__
