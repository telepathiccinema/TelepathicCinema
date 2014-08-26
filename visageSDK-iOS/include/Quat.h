
#ifndef __Quat_h__
#define __Quat_h__

#include "Base.h"
#include "Matrix4.h"

namespace VisageSDK
{

/**
* Quaternion class. Typically used to represent rotations.
*
* Important note: operations which require normalized operands
* will not normalize these operands automatically, so make sure
* you normalize them before invoking the operations.
*/
class VISAGE_DECLSPEC Quat
{

public:

	float w; ///< Quaternion w component.
	float x; ///< Quaternion x component.
	float y; ///< Quaternion y component.
	float z; ///< Quaternion z component.

	Quat(); ///< Constructor.
	Quat( float w, float x, float y, float z ); ///< Constructor.
	Quat( float ax, float ay, float az, EulerRotOrder order = V_EULER_XYZ ); ///< Constructor. Create quaternion rotation from Euler angles.
	Quat( const Vector3& axis, float angle ); ///< Constructor. Create quaternion rotation from axis-angle rotation.
	Quat& conjugate(); ///< Conjugate the quaternion.
	Quat getConjugate() const; ///< Get conjugate of the current quaternion.
	float magnitude() const; ///< Get quaternion magnitude.
	Quat& normalize(); ///< Normalize the quaternion.
	Quat getNormalized(); ///< Get the current quaternion in normalized form.
	Quat& invert(); ///< Invert the quaternion.
	Quat getInverse() const; ///< Get inverse of the current quaternion.
	void getEuler( float& ax, float& ay, float& az, EulerRotOrder order = V_EULER_XYZ ) const; ///< Get Euler angles from the current quaternion.
	float getAngle() const; ///< Get angle represented by the current quaternion.
	void getAxisAngle( Vector3& axis, float& angle ) const; ///< Get axis-angle rotation from the current quaternion.
	bool operator ==( const Quat& q ) const; ///< Determine if quaternions are equal.
	bool operator !=( const Quat& q ) const; ///< Determine if quaternions are not equal.
	Quat operator +() const;
	Quat operator -() const; ///< Negate the quaternion.
	Quat operator +( const Quat& q ) const; ///< Add two quaternions.
	void operator +=( const Quat& q ); ///< Add two quaternions.
	Quat operator -( const Quat& q ) const; ///< Subtract two quaternions.
	void operator -=( const Quat& q ); ///< Subtract two quaternions.
	Quat operator *( float s ) const; ///< Scalar-quaternion multiplication.
	void operator *=( float s ); ///< Scalar-quaternion multiplication.
	Quat operator /( float s ) const; ///< Scalar-quaternion division.
	void operator /=( float s ); ///< Scalar-quaternion division.
	float operator *( const Quat& q ) const; ///< Quaternion dot-product.
	Quat operator ^( const Quat& q ) const; ///< Quaternion wedge-product.
	void operator ^=( const Quat& q ); ///< Quaternion wedge-product.
	bool equals( const Quat& q ) const; ///< Determine if quaternions are equal.
	Quat& negate(); ///< Negate the quaternion.
	Quat add( const Quat& q ) const; ///< Add two quaternions.
	float dot( const Quat& q ) const; ///< Quaternion dot-product.
	Quat wedge( const Quat& q ) const; ///< Quaternion wedge-product.
	Quat slerp( const Quat& q, float t ) const; ///< Spherical linear interpolation between quaternions.
	Quat lerp( const Quat& q, float t ) const; ///< Linear interpolation between quaternions.

};

}

#endif // __Quat_h__
