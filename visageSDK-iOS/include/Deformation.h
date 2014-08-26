
#ifndef ___Deformation_h__
#define ___Deformation_h__

#include <vector>
#include "Vertex.h"

namespace VisageSDK
{

//////////////////////////////////////////////////////////////////////
//
//
//  Deformation
//
/// A class for deformations of 3D wireframes.
//
//
//////////////////////////////////////////////////////////////////////

class Deformation
  {
  public:

     // =======================================
    // Initialization

    Deformation ();
    Deformation ( int n, const std::string& name = "", int FAPNo = 0 );
    Deformation ( const Deformation& );
    void init ( int n, const std::string& name = "", int FAPNo = 0 );
	void init ( const std::string&, int, const std::vector<int>&, const std::vector<v3d>& );
    Deformation& operator = ( const Deformation& );

     // =======================================
    // Primitives

    Vertex& operator[] ( int i ) ;
    const Vertex& operator[] ( int i ) const ;

    int inline  nDisplacements    ()        const  { return int(_vertexNumbers.size()); }
    int         vertexNo          ( int i ) const  ;
    int         findVertex        ( int )   const  ;
    void inline set               ( int i, int v, double x, double y, double z ) { _vertexNumbers[i] = v; _vertexDisplacements[i].set(x, y, z); }
    void inline set               ( int i, int v, const Vertex& x ) { _vertexNumbers[i] = v; _vertexDisplacements[i] = x; }
	inline const std::string& getName() const { return _name; }

     // =======================================
    // File & stream I/O

    friend std::istream& operator>>(std::istream&, Deformation&);
    friend std::ostream& operator<<(std::ostream&, const Deformation&);

    void read ( std::istream&, const std::string&, int ) ;
    bool read ( const std::string& ) ;
    bool write ( const std::string& ) const;

	v2d ciao;

     // =======================================
    // Attributes

  private:
	  int                 _FAPNo;
	  std::vector<int>    _vertexNumbers;
      std::vector<Vertex> _vertexDisplacements;
	  std::string         _name;
  };


}

#endif // __Deformation_h__
