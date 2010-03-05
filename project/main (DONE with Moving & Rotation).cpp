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
 Light Moving
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

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

using namespace std;

const float	pi = 3.14;
const float	CUBE_SIZE = 1.0f;
//const float RotationDegree = 1.0f;

float	CubePos[3] = {0.0f, 0.0f, -8.0f}; 
float		CubeX = 0.0f;
float		CubeY = 0.0f;
float		CubeZ = 0.0f;
float		RotX = 0.0f;
float		RotY = 0.0f;

float	StartX = 0.0f;
float	StartY = 0.0f;

float	angle = 0.0f;

bool		RotateCube = false;
bool		MoveCube = false;
bool		Shake = false;

float	ShakeStartX, 	ShakeStartY;

GLfloat lightPos0[] = {0.0f, 0.0f, 5.0f, 1.0f}; //Positioned at (2, 0, 8)

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
			lightPos0[2] -= 1.0f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			lightPos0[2] += 1.0f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			lightPos0[0] -= 1.0f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			lightPos0[0] += 1.0f;
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
	}
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
		CubePos[2] -= 0.03f;
	}
	
	if (button == GLUT_WHEEL_DOWN)
	{
		CubePos[2] += 0.03f;
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
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	
	Image* image = loadBMP("vtr.bmp");
	_textureId = loadTexture(image);
	delete image;
	
	
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);	
}

GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; 

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(CubePos[0], -CubePos[1], CubePos[2]);
	
	//Add ambient light
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	
	//Add directed light 0
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	//lightPos0[] = {2.0f, 0.0f, 8.0f, 0.0f}; //Positioned at (2, 0, 8)
	//glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
	glRotatef(RotX, 1.0f, 0.0f, 0.0f);
	
	glRotatef(RotY, 0.0f, 1.0f, 0.0f);
	
	//Front Face	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //If you do not want lighting and coloring to effect your texture and you would like to display the texture unchanged when coloring is applied replace GL_MODULATE with GL_DECAL. 
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f);
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, 1.5f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, 1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, 1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 1.5f);
	
	
	glEnd();
	
	glBegin(GL_QUADS); 
	
	//Right
    //glNormal3f(1.0f, 0.0f, 0.0f);
    glNormal3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.5f, -1.0f, -1.5f);
    glNormal3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.5f, 1.0f, -1.5f);
    glNormal3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.5f, 1.0f, 1.5f);
    glNormal3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.5f, -1.0f, 1.5f);
    
    //Back
    //glNormal3f(0.0f, 0.0f, -1.0f);
    glNormal3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.5f, -1.0f, -1.5f);
    glNormal3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.5f, 1.0f, -1.5f);
    glNormal3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.5f, 1.0f, -1.5f);
    glNormal3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.5f, -1.0f, -1.5f);
    
    //Left
    //glNormal3f(-1.0f, 0.0f, 0.0f);
    glNormal3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.5f, -1.0f, -1.5f);
    glNormal3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.5f, -1.0f, 1.5f);
    glNormal3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.5f, 1.0f, 1.5f);
    glNormal3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.5f, 1.0f, -1.5f);
	
	glEnd();
	
	/*
	 
	 glBegin(GL_QUADS);
	 //Back Face
	 glColor3f(0.1f, 0.5f, 0.9f);
	 glVertex3f( ( -CUBE_SIZE / 2) + CubeX , (CUBE_SIZE / 2) + CubeY, (-CUBE_SIZE / 2) + CubeZ);		// Top Left
	 glVertex3f( ( CUBE_SIZE / 2) + CubeX , (CUBE_SIZE / 2) + CubeY, (-CUBE_SIZE / 2) + CubeZ);		// Top Right
	 glVertex3f( ( CUBE_SIZE / 2) + CubeX , (-CUBE_SIZE / 2) + CubeY, (-CUBE_SIZE / 2) + CubeZ);		// Bottom Right
	 glVertex3f( ( -CUBE_SIZE / 2) + CubeX , (-CUBE_SIZE / 2) + CubeY, (-CUBE_SIZE / 2) + CubeZ);		// Bottom Left
	 
	 glEnd();
	 */
	glDisable(GL_TEXTURE_2D);
	
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









