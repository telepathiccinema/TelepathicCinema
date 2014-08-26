
#ifndef __CandideModel_h__
#define __CandideModel_h__

#include "VertexSet.h"
#include "Deformation.h"
#include "Projection.h"
#include <sstream>
#include <cv.h>
#include <highgui.h>

namespace VisageSDK
{

//typedef unsigned char  BBYTE;
typedef std::vector<Deformation> DefVec;
typedef std::vector<double> ParamVec; 
typedef std::vector<Face> FaceVec;


//////////////////////////////////////////////////////////////////////
//
//
//  CandideModel
//
/// A class for 3D triangle models (wireframes, facet models).
//
//
//////////////////////////////////////////////////////////////////////

	class CandideModel
	{
		//////////////////////////////////////////////////////////////////
		// Construction/destruction
	public:

		/// Constructor
		CandideModel();

		/// Destructor.
		virtual ~CandideModel();

	public:

		/// \brief Returns a vertex in world coordinates.
		///
		/// updateGlobal() should be called first.
		inline Vertex& vertex( int v ) { return _transformedCoords[v]; }

		inline int getNumFaces(){return nFaces;}
		inline FaceVec& getFaces(){return _faces;}

		inline Vertex& base_vertex( int v ) { return _referenceCoords[v]; }
		
		inline Vertex& b_vertex( int v ) { return _baseCoords[v]; }
		
		inline Vertex& globalCoord( int v ) { return _transformedCoords[v]; }

		inline Vertex& dynCoord( int v ) { return _dynamicCoords[v]; }

		/// \brief Returns a vertex in image coordinates.
		///
		/// updateImageCoords() should be called first.
		inline Vertex& imageCoord( int v ) { return _imageCoords[v]; }

		inline VertexSet& getDynamicCoordinates() { return _dynamicCoords; }


		inline VertexSet& getImageCoordinates() { return _imageCoords; }
		inline void setImageCoordinates(const VertexSet& vs){_imageCoords = vs;}

		inline VertexSet& getGlobalCoordinates() { return _transformedCoords; }


		/// \brief Returns a vertex in image coordinates.
		///
		/// updateImageCoords() should be called first.
		//inline gFace::Vertex imageCoord( int v ) const { return _imageCoords[v]; }

//		inline Vertex& meanImageCoord( int v ) { return _imageMeanCoords[v]; }

		/// Returns a face (surface, triangle) of the CandideModel.
		inline Face& getFace( int f ) { return _faces[f]; }

	//////////////////////////////////////////////////////////////////
		// Deformations/parameters

		inline const Deformation& dynamicDeformation( int n ) const { return _dynamicDeformations[n]; }
		inline Deformation& dynamicDeformation( int n ) { return _dynamicDeformations[n]; }
		inline const Deformation& staticDeformation( int n ) const { return _staticDeformations[n]; }
		inline Deformation& staticDeformation( int n ) { return _staticDeformations[n]; }

		/// Set a dynamic parameter (animation parameter, action unit activation level, FAP values).
		void       setDynamicParam( int, double );

		/// Get a dynamic parameter (animation parameter, action unit activation level, FAP values).
		double     getDynamicParam( int ) const;
		/// Returns the number of dynamic deformations (animation modes, action units, FAPs).
		int        nDynamicDeformations() const;

		/// Set a static parameter (shape parameter).
		void       setStaticParam( int, double );

		/// Get a static parameter (shape parameter).
		double     getStaticParam( int ) const;
		/// Returns the number of static deformations.
		int        nStaticDeformations() const;

		void		GetStaticParams(double *g) const;
		void		SetStaticParams(const double *g);
		void		GetDynamicParams(double *g) const;
		void		SetDynamicParams(const double *g);
	
		/// Recalculates (if necessary) the static shape of the CandideModel.
		bool       updateStatic();

		/// \brief Recalculates (if necessary) the dynamic (animated) shape of the CandideModel.
		///
		/// updateStatic() are called if necessary.
		bool       updateDynamic();

		/// \brief Recalculate (if necessary) the vertex coordinates after global pose change.
		///
		/// updateDynamic() and updateStatic() are called if necessary.
		void       updateGlobal();

		/// \brief Recalculate (if necessary) the image coordinates after projection.
		///
		/// updateGlobal(), updateDynamic(), and updateStatic() are called if necessary.
		void       updateImageCoords( const Projection& projection = WeakPerspective() );

		//////////////////////////////////////////////////////////////////
		// Global transform (pose)

		/// Set global parameters (rotation, scale, translation).
		void       setGlobal          (const v3d& r, double s, const v3d& t){ _rotation = r; _scale = s; _translation = t; }
		/// Set global parameters (rotation, scale, translation).
		void       setGlobal          (const v3d& r, const v3d& s, const v3d& t){ _rotation = r; _scale = s; _translation = t; }

		/// Get global parameters (rotation, scale, translation).
		void       getGlobal          (v3d& r, v3d& s, v3d& t) const { r = _rotation; s = _scale; t = _translation; }
		/// Get global parameters (rotation, scale, translation).
		void       getGlobal          (v3d& r, double& s, v3d& t)   const { r = _rotation; s = _scale[0]; t = _translation; }

		void	   getScaleXYZ(double* scale) const {scale[0] = _scale[0]; scale[1] = _scale[1]; scale[2] = _scale[2];}

		double	   getScale() const {return _scale[0];}
		
		void       getGlobalRxyzSTxy  (double *g) const;
		void       setGlobalRxyzSTxy  (double *g);

		void       setRotation        (const Vertex& r);      ///< Set rotation parameters.
		void       setScale           (const Vertex& s);      ///< Set scaling factor(s).
		void       setTranslation     (const Vertex& t);      ///< Set translation parameters.
		void       setRotation        (double x, double y, double z);    ///< Set rotation parameters.
		void       setScale           (double x, double y, double z);    ///< Set scaling factor(s).
		void       setTranslation     (double x, double y, double z);    ///< Set translation parameters.

		void       rotate             (const Vertex& dr);     ///< Rotate the CandideModel.
		void       scale              (const Vertex& ds);     ///< Scale the CandideModel.
		void       translate          (const Vertex& dt);     ///< Translate the CandideModel.
		void       rotate             (double dx, double dy, double dz); ///< Rotate the CandideModel.
		void       scale              (double dx, double dy, double dz); ///< Scale the CandideModel.
		void       translate          (double dx, double dy, double dz); ///< Translate the CandideModel.
		void       scale              (double ds);                       ///< Scale the CandideModel.

		void       setAllParams( const std::vector<double>& );
		void       getAllParams( std::vector<double>& );
		int        nGlobalParams() { return 9; }
		int        nParamsarams() { return ( nGlobalParams() + nDynamicDeformations() + nStaticDeformations() ); }

		//////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
		// Graphics

		/// \brief Draw the CandideModel on an image.
		///
		/// updateImageCoords() should be called first.
		///
		/// \todo Other drawing modes (textured, feature points) should be supported.
		void draw (IplImage* img, unsigned char r, unsigned char g, unsigned char b);

		void drawPoints();

		void drawBaseCoordinates( IplImage* img );

	
		bool read (const std::string& );
		bool read ( std::istream& is );

		bool write( const std::string& , bool trans = false);

	protected:

		int nFaces, nVerts;


#pragma region Read/write functions

		// =======================================
		// Internal File & stream I/O

		bool       readAMD            ( std::istream& );
		bool       readSMD            ( std::istream& );

		void       readFaces          ( std::istream& );
		void       readDynamicDeformations( std::istream& );
		void       readStaticDeformations( std::istream& );
		void       readDeformations   ( std::vector<Deformation>&, const std::string&, std::istream& );
		void       readParams         ( std::vector<double>&, std::istream& );
		void       readGlobal         ( std::istream& );

		bool       writeBaseCoords    ( std::ostream& os ) const { os << _baseCoords; return os.good(); }
		bool       writeTransCoords   ( std::ostream& os ) const 
		{
			os << _transformedCoords; 
			return os.good(); 
		}

		bool       writeFaces         ( std::ostream& os ) const;
		bool       writeDeformations  ( std::ostream&, const std::string&, const DefVec& ) const;
		bool       writeDynamicDeformations( std::ostream& ) const;
		bool       writeStaticDeformations( std::ostream& ) const;
		bool       writeParams        ( std::ostream&, const std::string&, const ParamVec& ) const;
		bool       writeDynamicParams ( std::ostream& ) const;
		bool       writeStaticParams  ( std::ostream& ) const;
		bool       writeGlobal        ( std::ostream& ) const;

#pragma endregion

		public:
		
		//VertexSet _imageMeanCoords;
		VertexSet _referenceCoords;   
		

		VertexSet _baseCoords;        
		VertexSet _staticCoords;     
		VertexSet _dynamicCoords;    
		VertexSet _transformedCoords;


		VertexSet _refTransCoords;   //transformed coords in respect of _referenceCoords
		VertexSet _refStaticCoords;
		VertexSet _refDynCoords;

		VertexSet _imageCoords;

	private:

		//tot num ofn parameters
		int nParams;
		
		std::vector<Face> _faces;

		DefVec _dynamicDeformations;
		ParamVec _dynamicParams;
		DefVec _staticDeformations;
		ParamVec _staticParams;
		Vertex _rotation, _scale, _translation;

		bool _staticParamsModified;
		bool _dynamicParamsModified;

		std::string mSmdFilename;


	};

}

#endif // __CandideModel_h__
