
#ifndef __Shape2D_h__
#define __Shape2D_h__

#include "cxcore.h"
#include "VertexSet.h"

namespace VisageSDK
{

typedef CvPoint2D32f PointType;
typedef unsigned int uint;

/**	
	A class for two-dimensional shapes. 
	It provides funcionalities for creating, copying, centering, translating, scaling, multiplying by a scalar, etc...

*/
class Shape2D
{
public:

	/**Constructor*/
	Shape2D(void);

	/** Destructor */
	~Shape2D(void);

	/** Constructs a shape with a given number of objects */
	Shape2D(uint size);
	
	/** Constructs a shape object from a .ann file containing the number of points and the coordinates of each point */
	Shape2D(const char* ann_filename);


	/** Constructs a shape object from a CvMat object, containing the point coordinates
	///			 The matrix should contain floats and can be on the form (2*nPoints, 1) (1-dimensional array) or on the 
	///			 form (nPoints, 2) (2-dimensional). The inner representation however, will always be in the second form */
	Shape2D(const CvMat& vec);

	/** Constructs a shape object from a VertexSet object */
	Shape2D(const VertexSet& v);

	
	/** copy contructor */
	Shape2D(const Shape2D& shape);

	/** assignment operator */
	Shape2D& operator=(const Shape2D& shape);


	/** Sets a specific point in the shape */
	void SetPoint(unsigned int i, float x, float y);

	/** Sets the number or points in the shape, if this has a different num of points, it is deallocated and a new shape (of size n) is created  */
	void SetNumPoints(unsigned int n);

	/** Gets the number of points in the shape */
	inline unsigned int GetNumPoints() const {return mNPoints;}

	/** Computes the L2 norm of the shape and divides each element by it, setting thus the norm to 1 */
	void Normalize();

	/** Getr a specific point in the shape 
	* @param i Index of the desired point
	* @returns The selected point */
	PointType GetPoint(unsigned int i) const;

	/** Get the coordinates of a specific point in the shape 
	* @param i Index of the desired point
	* @param x X coordinate of the desired point
	* @param y Y coordinate of the desired point*/
	void GetPointCoords(unsigned int i, float &x, float &y) const;

	/** Get center of gravity of the shape 
	* @param centre COG of the shape*/
	void GetGravityCentre(PointType& centre) const;


	/** Centres the shape on (0,0). The COG is first computed and then subtracted frm each pixel 
	* @param x X coordinate of the COG (before the translation)
	* @param y Y coordinate of the COG (before the translation)*/
	void CentreOnOrigin(float &x, float &y);

	/** Centres the shape on (0,0) and then normalize. 
	* @param x X coordinate of the COG (before the translation)
	* @param y Y coordinate of the COG (before the translation)*/
	float MoveToOrigoNormalize(float &x, float &y);


	/** Aligns this to 'ref' with respect to pose. 
	* @param ref The reference shape.
	* @param pTheta The angle between this and ref
	* @param x The X translation between this and ref
	* @param y The Y translation between this and ref
	* @param scale The scaling factor between this and ref
	*/
	void AlignTo(const Shape2D &ref, float &pTheta, float &x, float &y, float &scale);


	/** Sets all the points to (0,0) */
	void Zero();

	/** Subtract a shape from this */
	void Sub(const Shape2D& shape);

	/** Add a shape to this */
	void Add(const Shape2D& shape);

	/** Multiply a shape to this with a optional scaling */
	void Mul(const Shape2D& shape, float scale = 1.0);

	/** Multiply all the pints in this by a scalar */
	void MulS(float scale, Shape2D* dest = NULL);

	/** Multiply this by a shape and add a scalar to each point coordinate */
	void MultiplyAdd(const Shape2D& shape, float val = 1.0, Shape2D* dest = NULL);

	/** Divides each point coordinate by the provided scalar 
	* @param value The divisor*/
	void DivS(float value);

	/** Subctract a 2D point coordinate from each point in the shape 
	* @param point The point to be subtracted */
	void SubS(const PointType& point);

	/** Add a point to each point in the shape 
	* @param point The point to be added*/
	void AddS(const PointType& point);

	/** Compute the sum of square differences betweenh the object and a given shape */
	float SSD(const Shape2D& shape) const;

	/** Translates the shape by the provided coordinates  
	* @param x The x displacement
	* @param y The y displacement*/
	void Translate(float x, float y);

	/** Guess what? It scales the shape! 
	* @param scaling The scaling factor (should always be positive)*/
	void Scale(float scaling);

	/**Rotate the shape by the angle theta*/
	void Rotate(float theta);

	/** Computes dot product between this and the shape passed as argument 
	* @param shape The scaling factor (should always be positive)*/
	float DotProduct(const Shape2D& shape) const;

	/** Returns the L2 norm of the shape */
	float GetShapeNorm() const;


	/** Returns the inner CvMat* representation of the shape, not the best for encapsulation! (I needed it somewhere sometimes /gab) */
	const CvMat* GetVector() const;


	/** Computes the width and the height of the smallest rectangle enclosing the shape. 
	///			 The rectangle is, however, parallel to the image
	/// @param width The width of the bounding box
	/// @param height The height of the bounding box
	/// @returns The average between width and height of bbox */
	float GetShapeBoundingBox(float* width = 0, float* height = 0);


	/** Draws the shape on the provided image, default color to white*/
	void Draw(IplImage* img, unsigned int c1 = 255, unsigned int c2 = 255, unsigned int c3 = 255) const;

	void DrawAnnotations(IplImage* img) const;

protected:

	/** The CvMat* containing the shape, it is a Nx2 vector of floats. */ 
	CvMat* mPointCoords;

private:

	/** Number of points in the shape. */
	unsigned int mNPoints;

};

}

#endif // __Shape2D_h__
