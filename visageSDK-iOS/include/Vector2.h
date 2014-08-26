
#ifndef __Vector2_h__
#define __Vector2_h__

#include "Base.h"
#include "Vector3.h"

namespace VisageSDK
{

/**
* 2D vector class.
*/
class VISAGE_DECLSPEC Vector2
{

public:

	float x; ///< Vector x component;
	float y; ///< Vector y component;

	Vector2(); ///< Constructor. Creates a nul-vector.
	Vector2( float x, float y ); ///< Constructor.
	float magnitude() const; ///< Returns vector magnitude.
	Vector2& normalize(); ///< Normalizes the current vector.
	float angle( const Vector2& v ) const; ///< Computes angle between vectors.
	bool operator ==( const Vector2& v ) const; ///< Determines if vectors are equal.
	bool operator !=( const Vector2& v ) const; ///< Determines if vectors are not equal.
	Vector2 operator +() const;
	Vector2 operator -() const; ///< Negates the vector.
	Vector2 operator *( float s ) const; ///< Scalar-vector multiplication.
	void operator *=( float s ); ///< Scalar-vector multiplication.
	Vector2 operator /( float s ) const; ///< Scalar-vector division.
	void operator /=( float s ); ///< Scalar-vector division.
	Vector2 operator +( const Vector2& v ) const; ///< Adds two vectors.
	void operator +=( const Vector2& v ); ///< Adds two vectors.
	Vector2 operator -( const Vector2& v ) const; ///< Subtracts two vectors.
	void operator -=( const Vector2& v ); ///< Subtracts two vectors.
	float operator *( const Vector2& v ) const; ///< Vector dot-product.
	bool equals( const Vector2& v ) const; ///< Determines if vectors are equal.
	Vector2& negate(); ///< Negates the vector.
	Vector2 mul( const float s ) const; ///< Scalar-vector multiplication.
	Vector2 div( const float s ) const; ///< Scalar-vector division.
	Vector2 add( const Vector2& v ) const; ///< Adds two vectors.
	Vector2 sub( const Vector2& v ) const; ///< Subtracts two vectors.
	float dot( const Vector2& v ) const; ///< Vector dot-product.
	static Vector2 GetXAxis(); ///< Get vector representing x-axis.
	static Vector2 GetYAxis(); ///< Get vector representing y-axis.

};

}

#endif // __Vector2_h__
