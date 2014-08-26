
#ifndef ___Projection_h__
#define ___Projection_h__

#include "Vertex.h"

namespace VisageSDK
{

//////////////////////////////////////////////////////////////////////
//
//
//  Projection
//
/// An abstract class for 3D-to-2D projections.
//
//
//////////////////////////////////////////////////////////////////////

  class Projection
  {
  public:
    virtual void apply( Vertex& imageCoord, const Vertex& worldCoord ) const = 0; //non posso istanziare 
																		//un oggetto della classe projection
  private:
    
	Projection& operator = ( const Projection& );
  
  protected:
    
	Projection( const Projection& p ) :
          _centerX( p._centerX ), _centerY( p._centerY ),
          _pixWidth( p._pixWidth ), _pixHeight( p._pixHeight ) {}
    
	Projection( double centerX, double centerY, double pixWidth, double pixHeight ) :
          _centerX( centerX ), _centerY( centerY ),
          _pixWidth( pixWidth ), _pixHeight( pixHeight ) {}
    
	double _centerX, _centerY;
    double _pixWidth, _pixHeight;
  };


//////////////////////////////////////////////////////////////////////
//
//
//  Perspective
//
/// Perspective projection.
//
//
//////////////////////////////////////////////////////////////////////

  class Perspective : public Projection
  {
  public:

    Perspective( double centerX = 0.0, double centerY = 0.0,
                 double pixWidth = 1.0, double pixHeight = 1.0,
                 double focalLength = 1.0 ) :
        Projection( centerX, centerY, pixWidth, pixHeight ),
        _focalLength( focalLength ) {}

    Perspective( const Perspective& pp ) :
        Projection( pp ),
        _focalLength( pp._focalLength ) {}

    void apply( Vertex& imageCoord, const Vertex& worldCoord ) const
    {
      imageCoord[0] = worldCoord[0] / (worldCoord[2] * _focalLength * _pixWidth) + _centerX;
      imageCoord[1] = worldCoord[1] / (worldCoord[2] * _focalLength * _pixHeight) + _centerY;
    }

  private:
    double _focalLength;
  };


//////////////////////////////////////////////////////////////////////
//
//
//  WeakPerspective
//
/// Weak perspective projection.
//
//
//////////////////////////////////////////////////////////////////////

  class WeakPerspective : public Projection
  {
  public :

    WeakPerspective( double centerX = 0.0, double centerY = 0.0,
                     double pixWidth = 1.0, double pixHeight = 1.0,
                     double focalLength = 1.0, double zRef = 1.0 ) :
        Projection( centerX, centerY, pixWidth, pixHeight ),
        _focalLength( focalLength ), _zRef( zRef ) {}

    WeakPerspective( const WeakPerspective& wpp ) :
        Projection( wpp ),
        _focalLength( wpp._focalLength ), _zRef( wpp._zRef ) {}

    void apply( Vertex& imageCoord, const Vertex& worldCoord ) const
    {

      imageCoord[0] = worldCoord[0] / (_zRef * _focalLength * _pixWidth) + _centerX;
      imageCoord[1] = worldCoord[1] / (_zRef * _focalLength * _pixHeight) + _centerY;

    }

  private:
    double _focalLength;
    double _zRef;
  };


//////////////////////////////////////////////////////////////////////
//
//
//  Ortographic
//
/// Ortographic projection.
//
//
//////////////////////////////////////////////////////////////////////

  class Ortographic : public Projection
  {
  public:

    Ortographic( double centerX = 0.0, double centerY = 0.0,
                 double pixWidth = 1.0, double pixHeight = 1.0 ) :
        Projection( centerX, centerY, pixWidth, pixHeight ) {}

    Ortographic( const Ortographic& op ) :
        Projection( op ) {}

    void apply( Vertex& imageCoord, const Vertex& worldCoord ) const
    {
      imageCoord[0] = worldCoord[0] / _pixWidth + _centerX;
      imageCoord[1] = worldCoord[1] / _pixHeight + _centerY;
    }
  };

}

#endif // ___Projection_h__
