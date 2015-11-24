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

//EKF.h
//declaration of the extended kalman filter

#ifndef EKF_H
#define EKF_H

#include <cv.h>

namespace VisageSDK
{

#define LIMIT_R_VALUE 100000.0f //points whose R value is bigger or equal to this value are not processed

//from types.h (FaceTracker 2)
typedef float f32;
typedef double f64;
typedef char is8;
typedef unsigned char iu8;
typedef short int is16;
typedef unsigned short int iu16;
typedef int is32;
typedef unsigned int iu32;
typedef long int is64;
typedef unsigned long int iu64;

class EKF
{

public:

	//constructor
	EKF (
		iu32 _au_num, //number of action units
		iu32 _points_num, //number of feature points
		CvMat *sensitivity //sensitivity values for rotation (3), translation (3) and AUs (number of AUs, currently 14)
		);
	//destructor
	~EKF (
		);
	//initiation
	void init (
		const f32* r, //3 sized array of xyz rotation angles
		const f32* t, //3 sized array of xyz translations
		const CvMat* au //action unit vector
		);
	//set measurement noise covariance matrix values, 2 values per feature point
	void set_R (
		iu32 point, //feature point number
		f32 value //variance
		);
	//update the ekf; return true on success, false if error occured
	bool update (
		const CvMat* vertex0, //model vertices after shape and action units have been added
		const CvMat* vertex1, //model vertices after scaling, rotation and translation
		const CvMat* projection, //projected model vertices
		const CvMat* points_coord, //image coordinates for all feature points
		f32 f, //focus
		f32 s, //scale
		const f32* r, //3 sized array of xyz rotation angles
		const CvMat* Au, //action unit matrix
		const CvMat* au_use, //which AUs are actually used?
		const CvMat* points_au, //points-AU correspondences
		const CvMat* points_Au, //points driving any AUs?
		const CvMat* au_gravity, //gravity factors for AUs
		f32 gravity_threshold //threshold value for Kalman gain below which gravity kicks in
		);
	//extract internal states
	void extract_states (
		f32* r, //3 sized array of xyz rotation angles, output
		f32* t, //3 sized array of xyz translations, output
		CvMat* au //action unit vector, output
		);
	// resize the data matrices for the current number of tracked featured points
	void resize (
		iu32 _points_num
		);

private:

	CvMat* x; //internal states
	CvMat* P; //internal states error covariance matrix
	CvMat* Q; //process noise covariance matrix
	CvMat* R; //measurement noise covariance matrix
	//the rest are temporary matrices/vectors
	CvMat* y_diff;
	CvMat* H;
	CvMat* K;
	CvMat* PHT;
	CvMat* S;
	CvMat* SI;
	CvMat* KH;
	
	iu32 points_num; //number of feature points
	iu32 au_num; //number of feature points

};

}

#endif