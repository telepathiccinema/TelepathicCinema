
#ifndef __Matrix4_h__
#define __Matrix4_h__

#ifdef VISAGE_STATIC
	#define VISAGE_DECLSPEC
#else

	#ifdef VISAGE_EXPORTS
		#define VISAGE_DECLSPEC __declspec(dllexport)
	#else
		#define VISAGE_DECLSPEC __declspec(dllimport)
	#endif

#endif

#include "Base.h"

namespace VisageSDK
{

enum EulerRotOrder
{
	V_EULER_XYZ,
	V_EULER_XZY,
	V_EULER_YXZ,
	V_EULER_YZX,
	V_EULER_ZXY,
	V_EULER_ZYX
};

/**
* 4x4 matrix class, typically used to represent affine transformations in 3D space.
*
* Important note: operations which require normalized operands
* will not normalize these operands automatically, so make sure
* you normalize them before invoking the operations.
*/
class VISAGE_DECLSPEC Matrix4
{

public:

	float m[4][4]; ///< Matrix components.
	Matrix4(); ///< Constructor. Create an identity matrix.
	Matrix4( float ax, float ay, float az, EulerRotOrder order = V_EULER_XYZ ); ///< Constructor. Create a rotation matrix from Euler angles.
	Matrix4( const Vector3& axis, float angle ); ///< Constructor. Create a rotation matrix from axis-angle rotation.
	Matrix4( const Quat& rot ); ///< Constructor. Create a rotation matrix from quaternion.
	float getElement( int i, int j ) const; ///< Get an element of the matrix by index.
	void setElement( int i, int j, float x ); ///< Set an element of the matrix by index.
	Matrix4& identity(); ///< Set matrix to identity.
	Matrix4& translation( float x, float y, float z ); ///< Set matrix to translation matrix.
	Matrix4& translation( const Vector3& trans ); ///< Set current matrix to translation matrix.
	Matrix4& euler( float ax, float ay, float az, EulerRotOrder order = V_EULER_XYZ ); ///< Set current matrix to rotation matrix for Euler angles.
	Matrix4& axisAngle( const Vector3& axis, float angle ); ///< Set current matrix to rotation matrix from axis-angle rotation.
	Matrix4& quat( const Quat& rot ); ///< Set current matrix to rotation matrix from quaternion.
	Matrix4& scale( float sx, float sy, float sz ); ///< Set current matrix to scale matrix.
	Matrix4& scale( const Vector3& scal ); ///< Set current matrix to scale matrix.
	Matrix4& transpose(); ///< Transpose the matrix.
	Matrix4 getTranspose() const; ///< Get transpose of the current matrix.
	float trace() const; ///< Get matrix trace.
	float determinant() const; ///< Get matrix determinant.
	Matrix4& invert(); ///< Invert the matrix.
	Matrix4 getInverse() const; ///< Get inverse of the current matrix.
	bool operator ==( const Matrix4& mat ) const; ///< Determine if matrices are equal.
	bool operator !=( const Matrix4& mat ) const; ///< Determine if matrices are not equal.
	Matrix4 operator*( const Matrix4& mat ) const; ///< Multiply two matrices.
	void operator *=( const Matrix4& mat ); ///< Multiply two matrices.
	Vector3 operator*( const Vector3& v ) const; ///< Multiply the matrix with a vector.
	bool equals( const Matrix4& mat ) const; ///< Determine if matrices are equal.
	Matrix4 mul( const Matrix4& mat ) const; ///< Multiply two matrices.
	Vector3 mul( const Vector3& v ) const; ///< Multiply the current matrix with a vector.
	void getEuler( float& ax, float& ay, float& az, EulerRotOrder order = V_EULER_XYZ ) const; ///< Extract Euler angles from the current matrix.
	Matrix4& setEuler( float ax, float ay, float az, EulerRotOrder order = V_EULER_XYZ ); ///< Set current matrix rotation from Euler angles.
	void getAxisAngle( Vector3& axis, float& angle ) const; ///< Extract axis-angle rotation from the current matrix.
	Matrix4& setAxisAngle( const Vector3& axis, float angle ); ///< Set current matrix rotation from axis-angle rotation.
	Quat getQuat() const; ///< Extract quaternion rotation from the current matrix.
	Matrix4& setQuat( const Quat& rot ); ///< Set current matrix rotation from quaternion rotation.
	Matrix4 getRotationMatrix() const; ///< Extract rotation matrix from the current matrix.
	void setRotationMatrix( const Matrix4& mat ); ///< Set current matrix rotation from rotation matrix.
	void getTranslation( float& x, float& y, float& z ) const; ///< Extract translation from the current matrix.
	Matrix4& setTranslation( float x, float y, float z ); ///< Set current matrix translation.
	Vector3 getTranslation() const; ///< Extract translation from the current matrix.
	Matrix4& setTranslation( const Vector3& trans ); ///< Set current matrix translation.
	void getScale( float& sx, float& sy, float& sz ) const; ///< Extract scale from the current matrix.
	Matrix4& setScale( float sx, float sy, float sz ); ///< Set current matrix scale.
	Vector3 getScale() const; ///< Extract scale from the current matrix.
	Matrix4& setScale( const Vector3& scal ); ///< Set current matrix scale.

private:

	float determinant3( const float* mat ) const; ///< Get determinant of a 3x3 matrix.
	void getSubMatrix4( int i, int j, float* subMat ) const; ///< Get a 3x3 sub-matrix of the current matrix.
};

}

#endif // __Matrix4_h__
