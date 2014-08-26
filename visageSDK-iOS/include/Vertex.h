

// Vertex.h: interface for the Vertex class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Vertex_h__
#define __Vertex_h__

#include <iostream>

//using namespace std;

#ifndef _WINDOWS_
template<class T> T min(const T& a, const T& b) { return(a<b?a:b); }
template<class T> T max(const T& a, const T& b) { return(a>b?a:b); }
///< If not already defined by Windows, min and max are defined here.
#endif

namespace VisageSDK
{

template<class T, size_t N>
class Vec 
{

   protected:
	T data[N];

public:

	typedef T* _Tptr;    //ma era proprio necessario?
	typedef T& _Tref;
	typedef const T _Ct;
	typedef const T& _Ctref;
	typedef const T* _Ctptr;

	typedef Vec<T,N> _V;  
	typedef _V* _Vptr;
	typedef _V& _Vref;
	typedef const _V _Cv;
	typedef const _V& _Cvref;
	typedef const _V* _Cvptr;


	#define _iter( v, k ) for( size_t k = 0; k < v.size(); k++ )
	#define _Ak for( size_t k = 0; k < size(); k++ ) 

	size_t size() {return N;}
	const size_t size() const {return N;}
	

    void  set( _Cvref v )    { _Ak data[k] = v[k];}
	void  set( const T& x )  { _Ak data[k] = x;}
	void  set( const T& x, const T& y ) { data[0] = x; if(size()>1) data[1]=y; }
	void  set( const T& x, const T& y, const T& z ) { set(x,y); if(size()>2) data[2]=z; }
   // void  set( const T& x, const T& y, const T& z, const T& zz ) { set(x,y,z); if(size()>3) data[3]=zz; }

	Vec() { _Ak data[k] = T(0);}  //costruisce vertice con tutti 0
	Vec( _Cvref v )    { set( v ); }
	Vec( _Ctref x )    { set( x ); }
	Vec( _Ctref x, _Ctref y )  { set( x, y ); }
	Vec( _Ctref x, _Ctref y, _Ctref z ) { set( x, y, z ); }


	_Vref  operator =  ( _Cvref v )      { set( v ); return *this; }
	_Vref  operator =  ( _Ctref t )      { set( t ); return *this; }
	//_Vref  operator =  ( _Ctptr p )      { set( p ); return *this; }

	_Tref  operator [] ( size_t d )      { return data[d]; }
	_Ctref operator [] ( size_t d )  const    { return data[d]; }

    _Tptr   ptr()                                  { return data; }
	_Ctptr  ptr()                       const           { return data; }

	//macros used for operators overloading
#define _CopCvref( _op) _V operator _op ( _Cvref v ) const { _V w; _Ak w[k] = data[k] _op v[k]; return w; }
#define _CopCtref( _op) _V operator _op ( _Ctref t ) const { _V w; _Ak w[k] = data[k] _op t; return w; }
#define _opCvref( _op ) _Vref operator _op ( _Cvref v ) { _Ak data[k] _op v[k]; return *this; }
#define _opCtref( _op ) _Vref operator _op ( _Ctref t ) { _Ak data[k] _op t; return *this; }

		_CopCvref( + )
		_CopCvref( - )
		_CopCvref( * )
		_CopCvref( / ) //nella divisione c'è throw exception

		_CopCtref( + )
		_CopCtref( - )
		_CopCtref( * )
		_CopCtref( / )

		_opCvref( += )
		_opCvref( -= )
		_opCvref( *= )

		_opCtref( += )
		_opCtref( -= )
		_opCtref( *= )

		_Vref operator /= ( _Cvref v )
		{
			
			_Ak if(v[k]==0) {}//error division by zero
				 else data[k] /= v[k];
			return *this;
		}

		_Vref  operator /= ( _Ctref t )
		{
			
			if( t == 0 ) {}//error} 
			_Ak data[k] /= t;
			return *this;
		}


		//friend _Vref operator * ( _Ctref t, _Cvref v ) _noth { _V w(v); return(w *= t); }

		//T  operator &  ( _Cvref v ) { T t(0); _Ak t += data[k] * v[k]; return t; }

		bool   operator == ( _Cvref v ) { _Ak if(data[k] != v[k]) return false; return true; }
		bool   operator == ( _Ctref t ) { _Ak if(data[k] != t) return false; return true; }
		bool   operator != ( _Cvref v ) { return(!(*this == v)); }
		bool   operator != ( _Ctref t ) { return(!(*this == t)); }

		friend std::ostream& operator << ( std::ostream& s, _Cvref v )
		{
			for(size_t i=0;i<v.size();i++)
				s << v[i] << " ";
			s << std::endl;
			return s;
		}

		/// \brief Input a vector from a stream.
		friend std::istream& operator >> ( std::istream& s, _Vref v )
		{
			T t;
			_iter( v, k ) { s >> t ; v[k] = t; } return s;
		}
};


typedef Vec<double,3> v3d;
typedef Vec<double,2> v2d;
typedef Vec<int,3> v3i;
//typedef Vec<int,4> v4i;
typedef Vec<int,2> v2i;
typedef Vec<float,2> v2f;
typedef Vec<double,3> Vertex;
typedef Vec<int,3> Face;

}

#endif // __Vertex_h__
