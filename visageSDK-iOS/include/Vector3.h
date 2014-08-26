
#ifndef __Vector3_h__
#define __Vector3_h__

#include "Base.h"
#include "MathMacros.h"

// C++
#include <cmath>

namespace VisageSDK
{

/**
* 3D vector class. Can also be used to represent a point in 3D space.
*
* Important note: operations which require normalized operands
* will not normalize these operands automatically, so make sure
* you normalize them before invoking the operations.
*/
class VISAGE_DECLSPEC Vector3
{

public:

	float x; ///< Vector x component.
	float y; ///< Vector x component.
	float z; ///< Vector x component.
	float w; ///< Vector w component (always 1).

	Vector3(); ///< Constructor. Create a nul-vector.
	Vector3( float x, float y, float z ); ///< Constructor.
	float magnitude() const; ///< Get vector magnitude.
	Vector3& normalize(); ///< Normalize the current vector.
	Vector3 getNormalized(); ///< Get the current vector in normalized form.
	float angle( const Vector3& v ) const; ///< Compute angle between vectors.
	bool operator ==( const Vector3& v ) const; ///< Determine if vectors are equal.
	bool operator !=( const Vector3& v ) const; ///< Determine if vectors are not equal.
	Vector3 operator +() const;
	Vector3 operator -() const; ///< Negate the vector.
	Vector3 operator *( float s ) const; ///< Scalar-vector multiplication.
	void operator *=( float s ); ///< Scalar-vector multiplication.
	Vector3 operator /( float s ) const; ///< Scalar-vector division.
	void operator /=( float s ); ///< Scalar-vector division.
	Vector3 operator +( const Vector3& v ) const; ///< Add two vectors.
	void operator +=( const Vector3& v ); ///< Add two vectors.
	Vector3 operator -( const Vector3& v ) const; ///< Subtract two vectors.
	void operator -=( const Vector3& v ); ///< Subtract two vectors.
	float operator *( const Vector3& v ) const; ///< Vector dot-product.
	Vector3 operator ^( const Vector3& v ) const; ///< Vector cross-product.
	void operator ^=( const Vector3& v ); ///< Vector cross-product.
	Vector3 operator*( const Matrix4& mat ) const; ///< Multiply the vector with a matrix.
	/// In this case, vector typically represents a point in 3D space.
	void operator*=( const Matrix4& mat ); ///< Multiply the vector with a matrix.
	/// In this case, vector typically represents a point in 3D space.
	bool equals( const Vector3& v ) const; ///< Determine if vectors are equal.
	Vector3& negate(); ///< Negate the vector.
	Vector3 mul( const float s ) const; ///< Scalar-vector multiplication.
	Vector3 div( const float s ) const; ///< Scalar-vector division.
	Vector3 add( const Vector3& v ) const; ///< Add two vectors.
	Vector3 sub( const Vector3& v ) const; ///< Subtract two vectors.
	float dot( const Vector3& v ) const; ///< Vector dot-product.
	Vector3 cross( const Vector3& v ) const; ///< Vector cross-product.
	Vector3 mul( const Matrix4& mat ) const; ///< Multiply the current vector with a matrix.
	/// In this case, the vector typically represents a point in 3D space.
	Vector3& rotate( const Quat& rot ); ///< Rotate the current vector using a quaternion.
	/// In this case, vector typically represents a point in 3D space.
	static Vector3 getXAxis(); ///< Get vector representing x-axis.
	static Vector3 getYAxis(); ///< Get vector representing y-axis.
	static Vector3 getZAxis(); ///< Get vector representing z-axis.

};

}

#endif // __Vector3_h__
