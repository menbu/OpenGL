/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Putting It All Together" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */


/*ideas:
	Smooth
	dice (randm number)
	Shadow
	Moving Light
	Sounds effect
	Screen Saver
	Cube with hole
	Inside a bubble
	texture mapping (one picture) Sphere
	transperent Cube
	Utility Cube. tv..watch..notepad,music..

*/


#include <iostream>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"


using namespace std;


//Stores information regarding a ball
struct Ball {
    float	v[3]; //Velocity
    float	pos[3]; //Position
};

Ball	MyBall;



float	Front;
float	Back;

float	Top;
float	Bottom;
float	Right;
float	Left;



const float	gravity = 9.8f;


const float	CUBE_SIZE = 2.0f;

const float	FLOOR_SIZE = 50.0f;

float	Boundry = 0.0f;
float	RotB = 0.0f;



float	Scene[3] = {0.0f, 0.0f, -15.0f};
float	CubePos[3] = {0.0f, 0.0f, 0.0f}; 
float	ALPHA = 0.7f; //The opacity of the cube
float	CubeX = 0.0f;
float	CubeY = 0.0f;
float	CubeZ = 0.0f;
float	RotX = 0.0f;
float	RotY = 0.0f;
float	RotZ = 0.0f;


float	StartX = 0.0f;
float	StartY = 0.0f;

float	LightX = 0.0f;
float	LightY = 0.0f;
	
float	angle = 0.0f;

bool		RotateCube = false;
bool		MoveCube = false;
bool		Shake = false;
bool		Reflection = false;
bool		Blend = false;
bool		ball = true;

float	ShakeStartX, 	ShakeStartY;



GLfloat lightPos0[] = {-3.0f, 0.0f, 2.0f, 1.0f}; //Positioned at (2, 0, 8)

GLuint _textureId;           //The OpenGL id of the texture

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return textureId;
}


static void SpecialKey(int key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
		case GLUT_KEY_UP:
			Scene[2] -= 0.5f;
			ALPHA -= 0.02f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			Scene[2] += 0.5f;
			ALPHA += 0.02f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			Scene[0] -= 0.5f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			Scene[0] += 0.5f;
			glutPostRedisplay();
			break;
		case 116:
			
			if (!Shake)
			{
				Shake = true;
				//RotX = 0;
				ShakeStartY = RotY;
				ShakeStartX = RotX;
				//RotX = 30.0f;
				//RotY = 30.0f;
			}
			else
			{
				Shake = false;
				//RotX = 0.0f;
				//RotY = 0.0f;
			}

			glutPostRedisplay();	
			break;
		case 114:
			
			if (!Reflection)
			{
				Reflection = true;
				Blend = false;
			}
			else
				Reflection = false;
			
			
			glutPostRedisplay();	
			break;
		case 97:
			if (!Blend)
			{
				Blend = true;
				Reflection = false;
			}
			else
				Blend = false;
		case 13:
			RotX = 0.0f;
			RotY = 0.0f;
			Boundry = 0.0f;
			break;
		case 'x':
			if (!ball)
				ball = true;
			else
				ball = false;
			break;

	}
	
	//printf("%d\n", key);
		
}


void MouseKey(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		StartX = x - RotY;
		StartY = -y + RotX;
		RotateCube = true;
				
	}
	else
		RotateCube = false;
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		StartX = x - (CubePos[0] * 100);
		StartY = -y + (CubePos[1] * 100);
		MoveCube = true;
	}
	else
		MoveCube = false;
	
	if (button == GLUT_WHEEL_UP)
	{
		Scene[2] -= 0.03f;
		ALPHA -= 0.001f;
	}
	
	if (button == GLUT_WHEEL_DOWN)
	{
		Scene[2] += 0.03f;
		ALPHA += 0.001f;
	}
	
}

void mouseMotion(int x, int y)
{
	if (RotateCube)
	{

		RotY = x - StartX;
		RotX = y + StartY;
		
		if (RotY > 360) 
			RotY -= 360;
		else
		{
		if (RotY < -360) 
			RotY += 360;
		}
		
		
		if (RotX > 360) 
			RotX -= 360;
		else
		{
			if (RotX < -360) 
				RotX += 360;
		}
		
		
		//printf("RotX = %f\n", RotX);
		//printf("RotY = %f\n", RotY);
		
		
		RotB = RotX;
		
		
		if (fabs (RotX) >= 0.0f && fabs (RotX) < 45.0f)
			RotB = RotX;
		else
			if (fabs (RotX) >= 45.0f && fabs (RotX) < 90.0f)
				RotB = fabs (RotX) - 45.0f;
			else
				if (fabs (RotX) >= 90.0f && fabs (RotX) < 135.0f)
					RotB = fabs (RotX) - 90.0f;
				else
					if (fabs (RotX) >= 135.0f && fabs (RotX) < 180.0f)
						RotB = fabs (RotX) - 135.0f;
					else	
						if (fabs (RotX) >= 180.0f && fabs (RotX) < 225.0f)
							RotB = fabs (RotX) - 180.0f;
						else
							if (fabs (RotX) >= 225.0f && fabs (RotX) < 270.0f)
								RotB = fabs (RotX) - 225.0f;
							else
								if (fabs (RotX) >= 270.0f && fabs (RotX) < 315.0f)
									RotB = fabs (RotX) - 270.0f;
								else
									if (fabs (RotX) >= 315.0f && fabs (RotX) < 360.0f)
										RotB = fabs (RotX) - 315.0f;
									else
										RotB = fabs (RotX) - 360.0f;
		
		
		
		Boundry = fabs (RotB) / 100;
				
		
		//Boundry = RotB;	
			
		
		
		//Boundry = RotB / 100;
		
		
		
		

		
		
		
		//printf("Boundry = %f\n", Boundry);
		
	}
	
	
	ShakeStartX = RotX;
	ShakeStartY = RotY;
	
	if (MoveCube)
	{
		CubePos[0] = (x - StartX) / 100;
		CubePos[1] = (y + StartY) / 100;
	}
	
	
	glutPostRedisplay();
}


void initRendering() {

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH); //Enable smooth shading
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
	
	
/*
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
*/	
	
	Image* image = loadBMP("vtr.bmp");
	
	_textureId = loadTexture(image);
	delete image;
	
	MyBall.v[0] = -0.05f;
	MyBall.v[1] = -0.05f;
	MyBall.v[2] = 0.05f;
	MyBall.pos[0] = 0.1f;
	MyBall.pos[1] = -0.5f;
	MyBall.pos[2] = -0.3f;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);	
}


void DrawCube(int Txt_ID)
{
	
	glPushMatrix();	
	
	glRotatef(RotX, 1.0f, 0.0f, 0.0f);
	glRotatef(RotY, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_QUADS);
	
	//Top face
	glColor4f(1.0f, 1.0f, 0.0f, ALPHA);
	glNormal3f(0.0, 1.0f, 0.0f);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	
	//Bottom face
	glColor4f(1.0f, 0.0f, 1.0f, ALPHA);
	glNormal3f(0.0, -1.0f, 0.0f);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	
	//Left face
	glNormal3f(-1.0, 0.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 1.0f, ALPHA);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	glColor4f(0.0f, 0.0f, 1.0f, ALPHA);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	
	//Right face
	glNormal3f(1.0, 0.0f, 0.0f);
	glColor4f(1.0f, 0.0f, 0.0f, ALPHA);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glColor4f(0.0f, 1.0f, 0.0f, ALPHA);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Txt_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor4f(1.0f, 1.0f, 1.0f, ALPHA);
	glBegin(GL_QUADS);
	
	//Front face
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	
	//Back face
	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	
	Front = CubePos[2] + (CUBE_SIZE / 2);
	Back = CubePos[2] - (CUBE_SIZE / 2);
	Top = CubePos[1] + (CUBE_SIZE / 2);
	Bottom = CubePos[1] - (CUBE_SIZE / 2);
	Right = CubePos[0] + (CUBE_SIZE / 2);
	Left = CubePos[0] - (CUBE_SIZE / 2);
	
}

void DrawFloor(int Txt_ID)
{
	glPushMatrix();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Txt_ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //If you do not want lighting and coloring to effect your texture and you would like to display the texture unchanged when coloring is applied replace GL_MODULATE with GL_DECAL. 
	
	
	// Draw a floor
	glBegin(GL_QUADS); 
	
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-FLOOR_SIZE / 2, 0, FLOOR_SIZE / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-FLOOR_SIZE / 2, 0, -FLOOR_SIZE / 2);
	glTexCoord2f(1, 1);
	glVertex3f(FLOOR_SIZE / 2, 0, -FLOOR_SIZE / 2);
	glTexCoord2f(1, 0);
	glVertex3f(FLOOR_SIZE / 2, 0, FLOOR_SIZE / 2);
	
	glEnd();
	
	glPopMatrix();
}

/*
void drawSphere(int Txt_ID, float radius, int slices, int stacks) 
{

	glPushMatrix();
	
	
	
	//glRotatef(RotY, 0.0f, 1.0f, 0.0f);
	
	
	GLUquadricObj * g_sphere = NULL;
	
	// initialize our sphere
    g_sphere = gluNewQuadric( );
    gluQuadricNormals( g_sphere, ( GLenum )GLU_SMOOTH );
    gluQuadricTexture( g_sphere, GL_TRUE );
	
	
	// set texture state
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	
	
	// generate OpenGL texture
	glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, Txt_ID );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	
	
	
	gluSphere( g_sphere, radius, slices, stacks );
	
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	
}
*/

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(Scene[0], Scene[1], Scene[2]);
	

	GLfloat ambientLight[] = {0.2, 0.2, 0.2, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat lightColor[] = {0.6f, 0.6f, 0.6f, 1.0f};
	
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor); //Diffuse (non-shiny) light component
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor); //Specular (shiny) light component
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
	
	
	//====================== MATERIAL ======================
	float specularity = 1.0f;
	float shininess = 25;
	
	GLfloat materialColor[] = {0.2f, 0.2f, 2.0f, 1.0f}; //The color of the sphere	
	GLfloat materialSpecular[] = {specularity, specularity, specularity, 1.0f}; //The specular (shiny) component of the material
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess); //The shininess parameter
	
	
	//_textureId = 0;

	glRotatef(30, 1, 0, 0);
	glPushMatrix();
	
	
	
	
	GLfloat y = -CubePos[1] + (CUBE_SIZE);
	
	//printf("%f\n", y);
	
	// cna't be dragged under the floor
	if (Reflection)
	{
		if ( y <= Boundry + 1)
		{
			printf("Old Y = %f\n", y);
			CubePos[1] = -Boundry + 1.0f;
			y = -CubePos[1] + (CUBE_SIZE);
			printf("New Y = %f\n", y);
			//RotX = 0;
		}
	}
	
	
	

	//printf("%f\n",((CUBE_SIZE -1) + (-CubePos[1])));
	
	
	glTranslatef(CubePos[0], y, 0); 
	
	
	// draw the ball
	glPushMatrix();
	
	
	glTranslatef(MyBall.pos[0], MyBall.pos[1], MyBall.pos[2]);
	
	glutSolidSphere(0.1, 180, 90);
	
	
	glPopMatrix();
	
	
	
	
	
	if (Blend)
		glEnable(GL_BLEND); //Enable alpha blending
	
	
		
	DrawCube(_textureId);
	
	
	
	glDisable(GL_BLEND); //Disable alpha blending
	glPopMatrix();

	
	
	if (Reflection)
	{
		
		// Don't update color or depth.
		glDisable(GL_DEPTH_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		
		// Draw 1 into the stencil buffer.
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		
		DrawFloor(_textureId);
		
		// Re-enable update of color and depth. 
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Enable drawing colors to the screen
		glEnable(GL_DEPTH_TEST); //Enable depth testing
		
		// Now, only render where stencil is set to 1. 
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);  // draw if stencil ==1
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		
		
		// Draw reflected ninja, but only where floor is. 
		glPushMatrix();
		glScalef(1, -1, 1);
		
		glTranslatef(CubePos[0], y, 0); 
		
		if (Blend)
			glEnable(GL_BLEND); //Enable alpha blending
		
		DrawCube(0);
		glDisable(GL_BLEND); //Enable alpha blending
		glPopMatrix();
		
		glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
		
		
		//Blend the floor onto the screen
		glEnable(GL_BLEND);
		glColor4f(1, 1, 1, 0.7f);
		DrawFloor(0);
		glDisable(GL_BLEND);
	}
	
	
	//RotX -= 1.0;
	//RotY += 1.0;
	
	
	
	glFlush();
	glutSwapBuffers();
}


//Called every 25 milliseconds
void update(int value) {

	if (Shake)
	{
		
		if (RotY > ShakeStartY + 5)
			RotY = ShakeStartY;
		RotY += 2.0f;
		
		if (RotX > ShakeStartX + 5)
			RotX = ShakeStartX;
		RotX += 1.0f;
		
		CubePos[0] += 0.003f;
	}
	
	/*
	float	Front = CubePos[2] + (CUBE_SIZE / 2);
	float	Back = CubePos[2] - (CUBE_SIZE / 2);
	
	float	Top = CubePos[1] + (CUBE_SIZE / 2);
	float	Bottom = CubePos[1] - (CUBE_SIZE / 2);
	
	float	Right = CubePos[0] + (CUBE_SIZE / 2);
	float	Left = CubePos[0] - (CUBE_SIZE / 2);
	*/

	//printf("Top = %f\n", Top);
	
	//printf("Ball.Y = %f\n", MyBall.pos[1] + 0.1);
	
	if (MyBall.pos[0] + 0.1 >= Right)
		MyBall.v[0] *= -1;
	if (MyBall.pos[0] - 0.1 <= Left)
		MyBall.v[0] *= -1;
	if (MyBall.pos[1] + 0.1 >= Top)
		MyBall.v[1] *= -1;
	if (MyBall.pos[1] - 0.1 <= Bottom)
		MyBall.v[1] *= -1;
	if (MyBall.pos[2]  + 0.1 >= Front)
		MyBall.v[2] *= -1;
	if (MyBall.pos[2]  - 0.1 <= Back)
		MyBall.v[2] *= -1;
	
	MyBall.pos[0] += MyBall.v[0];
	MyBall.pos[1] += MyBall.v[1];
	MyBall.pos[2] += MyBall.v[2];
	
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	
	glutCreateWindow("My Project");
	initRendering();
	//glutFullScreen();
	
	
	
	
	glutDisplayFunc(drawScene);
	glutMouseFunc(MouseKey);
	glutMotionFunc(mouseMotion);
	glutSpecialFunc(SpecialKey);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	
	glutMainLoop();
	return 0;
}









