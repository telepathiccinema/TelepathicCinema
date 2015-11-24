///////////////////////////////////////////////////////////////////////////////
// 
// (c) Visage Technologies AB 2002 - 2015  All rights reserved. 
// 
// This file is part of visage|SDK(tm). 
// Unauthorized copying of this file, via any medium is strictly prohibited. 
// 
// No warranty, explicit or implicit, provided. 
// 
// This is proprietary software. No part of this software may be used or 
// reproduced in any form or by any means otherwise than in accordance with
// any written license granted by Visage Technologies AB. 
// 
/////////////////////////////////////////////////////////////////////////////

// VertexSet.h: interface for the VertexSet class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __VertexSet_h__
#define __VertexSet_h__

#include "Vertex.h"
#include <vector>
#include <iterator>
#include <numeric>
#include "Deformation.h"
#include "Projection.h"
#include "cxcore.h"

namespace VisageSDK
{

class VertexSet
{

	std::vector<Vertex> _vertices;
	typedef std::vector<Vertex>::iterator it;
	typedef std::vector<Vertex>::const_iterator c_it;
    it begin() { return _vertices.begin(); }
    it end() { return _vertices.end(); }
    c_it begin() const { return _vertices.begin(); }
    c_it end() const { return _vertices.end(); }

public:
	VertexSet();
	VertexSet(int);
	VertexSet(int,double);
	virtual ~VertexSet();
	void init(int);
	void clear();

	void normalize();

	double getNorm();

	v3d mean();
	void translate(const Vertex&);

	int nVertices() const { return static_cast<int>(_vertices.size()); }

	void        transform          ( const CvMat* );
         
	void        rotate             ( const Vertex& );
	void        rotate             ( double, double, double );
	void        rotateAround       ( double, double, double, int );

	void inline translate          ( double x, double y, double z = 0 )  { Vertex v(x,y,z); translate(v); }

	void        scale              ( const Vertex& )  ;
	void inline scale              ( double f )                         { Vertex v(f,f,f); scale(v); }
	void inline scale              ( double x, double y, double z = 1 )  { Vertex v(x,y,z); scale(v); }
	
	void        applyDeformation   ( const Deformation&, double );
	void        applyDeformations  ( const VertexSet&, const std::vector<Deformation>&, const std::vector<double>& );         
	  
    void project( const VertexSet& vs, const Projection& projection )
    {
      init( vs.nVertices() );
      it dst = begin();
	  c_it src = vs.begin();
      while( dst != end() )
      {
        projection.apply( *dst, *src );
        dst++;src++;
      }
    }

	friend std::istream& operator>>( std::istream&, VertexSet& );
    friend std::ostream& operator<<( std::ostream&, const VertexSet& );

	
    v3d& operator[] ( int );
    const v3d& operator[] ( int ) const;

};

}

#endif // __VertexSet_h__
