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


const float	CUBE_SIZE = 2.0f;
const float	FLOOR_SIZE = 10.0f;
//const float RotationDegree = 1.0f;

float	CubePos[3] = {0.0f, 0.0f, -15.0f}; 
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
	glShadeModel(GL_SMOOTH); //Enable smooth shading
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
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


void DrawCube(int Txt_ID)
{
	
	glPushMatrix();	
	
	glRotatef(RotX, 1.0f, 0.0f, 0.0f);
	glRotatef(RotY, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_QUADS);
	
	//Top face
	glColor3f(1.0f, 1.0f, 0.0f);
	glNormal3f(0.0, 1.0f, 0.0f);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	
	//Bottom face
	glColor3f(1.0f, 0.0f, 1.0f);
	glNormal3f(0.0, -1.0f, 0.0f);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	
	//Left face
	glNormal3f(-1.0, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(-CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	
	//Right face
	glNormal3f(1.0, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, -CUBE_SIZE / 2);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(CUBE_SIZE / 2, CUBE_SIZE / 2, CUBE_SIZE / 2);
	glVertex3f(CUBE_SIZE / 2, -CUBE_SIZE / 2, CUBE_SIZE / 2);
	
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Txt_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
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
	
	glTranslatef(0, -0.5f, 0); 
	
	// Draw a floor
	glBegin(GL_QUADS); 
	
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-FLOOR_SIZE / 2, -CUBE_SIZE / 2, FLOOR_SIZE / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-FLOOR_SIZE / 2, -CUBE_SIZE / 2, -FLOOR_SIZE / 2);
	glTexCoord2f(1, 1);
	glVertex3f(FLOOR_SIZE / 2, -CUBE_SIZE /2, -FLOOR_SIZE / 2);
	glTexCoord2f(1, 0);
	glVertex3f(FLOOR_SIZE / 2, -CUBE_SIZE / 2, FLOOR_SIZE / 2);
	
	glEnd();
	
	glPopMatrix();
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(CubePos[0], -CubePos[1], CubePos[2]);
	
	//glTranslatef(0.0f, 0.0f, -15.0f);
	glRotatef(30, 1, 0, 0);
		
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
	
	
	_textureId = 0;
	

	DrawCube(_textureId);
	glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
	glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
	glDisable(GL_DEPTH_TEST); //Disable depth testing
	glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
	//Make pixels in the stencil buffer be set to 1 when the stencil test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//Set all of the pixels covered by the floor to be 1 in the stencil buffer
	DrawFloor(_textureId);
	
	glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
	glEnable(GL_DEPTH_TEST); //Enable depth testing
	//Make the stencil test pass only when the pixel is 1 in the stencil buffer
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change
	
	//Draw the cube, reflected vertically, at all pixels where the stencil
	//buffer is 1
	glPushMatrix();
	glScalef(1, -1, 1);
	
	glTranslatef(0, CUBE_SIZE + 1.0f, 0); 
	
	DrawCube(_textureId);
	glPopMatrix();
	
	glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
	
	//Blend the floor onto the screen
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, 0.7f);
	DrawFloor(_textureId);
	glDisable(GL_BLEND);
	
	
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









