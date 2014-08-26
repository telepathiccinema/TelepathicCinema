/*
APPLICATION NOTES: AUGMENTED REALITY
------------------------------------

These notes and bits of code may serve as a guide for implementing Augmented Reality applications using VisageTracker2. More specifically, they may help application developers that want to display virtual elements (e.g. eyeglasses, hats etc.) that appear correctly fixed to the head in 3D.

NOTE: this is not a full implementation and can not be used as such; it also comes strictly as-is, without any guarantees.

The code is taken from the functions that actually implement the drawing of the tracking results
overlayed on the face. The code requires modifications to be integrated into another project, but
it contains all essential calls correctly adjusted to overlay 3D objects on the head in 3D using OpenGL.
*/

TrackingData data;

// main function to draw 3D overlays
void drawResults()
{
		tracker->getTrackingData(&data);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		//set image specs
		glViewport(0,0,data.frame->width,data.frame->height);
		//enable stuff
		glShadeModel(GL_FLAT);
		//set camera
		setGlView(data.frame->width,data.frame->height,data.cameraFocus);
		//rotate and translate into the current coordinate system of the head
		glMatrixMode(GL_MODELVIEW);
		float *r = data.faceRotation;
		float *t = data.faceTranslation;
		glTranslatef(-t[0],t[1],-t[2]); //because x and z axes in OpenGL are opposite from the ones used by visage|SDK - see FaceData::faceTranslation
		glRotatef(r[1]*180.0f/3.1415926f, 0.0f, 1.0f, 0.0f);
		glRotatef(r[0]*180.0/3.1415926f, 1.0f, 0.0f, 0.0f);
		glRotatef(r[2]*180.0/3.1415926f, 0.0f, 0.0f, 1.0f);

		//NOW WE ARE IN THE 3D COORDINATE SYSTEM OF THE HEAD AND WE CAN DRAW WHAT WE WANT

		glPointSize(3);

		//draw nose
		glColor3ub(255,255,32);
		glBegin(GL_POINTS);
		for(int j = 0; j < 15; j++)
		{
			drawPoint3D(9,j+1);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		drawPoint3D(9,15);
		drawPoint3D(9,4);
		drawPoint3D(9,2);
		drawPoint3D(9,3);
		drawPoint3D(9,1);
		drawPoint3D(9,5);
		glEnd();
		glBegin(GL_LINE_LOOP);
		drawPoint3D(9,6);
		drawPoint3D(9,7);
		drawPoint3D(9,13);
		drawPoint3D(9,12);
		drawPoint3D(9,14);
		glEnd();
		glBegin(GL_LINES);
		drawPoint3D(9,14);
		drawPoint3D(9,2);
		drawPoint3D(9,13);
		drawPoint3D(9,1);
		glEnd();

		//draw eyes
		glBegin(GL_POINTS);
		for(int j = 6; j < 14; j++)
		{
			drawPoint3D(3,j+1);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		drawPoint3D(3,12);
		drawPoint3D(3,14);
		drawPoint3D(3,8);
		drawPoint3D(3,10);
		glEnd();
		glBegin(GL_LINE_LOOP);
		drawPoint3D(3,11);
		drawPoint3D(3,13);
		drawPoint3D(3,7);
		drawPoint3D(3,9);
		glEnd();

		//draw cheeks
		glBegin(GL_POINTS);
		for(int j = 0; j < 4; j++)
		{
			drawPoint3D(5,j+1);
		}
		glEnd();
		glBegin(GL_LINES);
		drawPoint3D(5,2);
		drawPoint3D(5,4);
		drawPoint3D(5,1);
		drawPoint3D(5,3);
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
}

// access a specific feature point and draw it
// for feature point group and index numbers (i,j) see the documentation. In the MPEG-4 introduction
// there is the schema showing all feature points on the face.
void drawPoint3D(int i,int j)
{
	FDP *fdp = data.featurePoints3DRelative; //get local 3D coordinates of the feature points in the coordinate system of the head
	FeaturePoint* fp;
	fp = const_cast<FeaturePoint*>( &fdp->getFP(i,j) );
	if(fp->defined)
		glVertex3f(fp->pos[0],fp->pos[1],fp->pos[2]); //actually access and draw the point
}


//set the camera and its view (frustum)
void setGlView (
	iu32 width, //width of image to be drawn
	iu32 height, //height of image to be drawn
	f32 f //focus
	)
{
	GLfloat x_offset = 1;
	GLfloat y_offset = 1;
	if (width > height)
		x_offset = ((GLfloat)width)/((GLfloat)height);
	else if (width < height)
		y_offset = ((GLfloat)height)/((GLfloat)width);
	//set frustum specs
	GLfloat frustum_near = 0.001f;
	GLfloat frustum_far = 30; //hard to estimate face too far away
	GLfloat frustum_x = x_offset*frustum_near/f;
	GLfloat frustum_y = y_offset*frustum_near/f;
	//set frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-frustum_x,frustum_x,-frustum_y,frustum_y,frustum_near,frustum_far);
	glMatrixMode(GL_MODELVIEW);
	//clear matrix
	glLoadIdentity(); //camera in (0,0,0) looking at (0,0,-1) up vector (0,1,0)	
}