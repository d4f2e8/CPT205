#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>
#include "vector"
#define PI 3.14
#define MAX_CHAR 128
#define checkImageWidth 64
#define checkImageHeight 64
#define clockImageWidth 64
#define clockImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3]; // check texture
GLubyte clockImage[clockImageWidth][clockImageHeight][3]; // clock texture
int intWinWidth = 800; // define the window size
int intWinHeight = 700;
GLint imagewidth[16];
GLint imageheight[16]; // these varaibles for reading image
GLint pixellength[16];
std::vector<GLubyte*>p;
GLuint universalTexture[16]; // store the texture for the first scene
GLuint roomTexture[2]; // store the texture for the second scene
float selfRotate[9] = { 0,0,0,0,0,0,0,0 };
float revolution[9] = { 0,0,0,0,0,0,0,0 }; // for the movement of planets
float radius[9] = { 140,160,185,210,260,320,370,415,15 };
int stopMoving = 0; // 0 for keeping moving
float fltRadius = 200; 
float fltXZAngle = 0;  // these three variables for the camera position
float fltY_XZAngle = 0;
float fltXRef = 0.0; 
float fltYRef = 0.0; // look-at point
float fltZRef = 0.0;
float fltRefAngle1 = 0.0; // camera movement parameters for the second scene
float fltRefAngle2 = 0.0;
float fltX0Offest = 0.0; // use under the planet mode for presenting the offset from the original point
float fltZ0Offest = 0.0;
int counter = 0; // counter for the planet mode
bool planetMode = false; // whether the planet mode is on or not
float fadeAwayZ = 0; // use when changes to the second scene, make the first scene out of sight
bool clockOn = false; // on means in the second scene
int timeCounter = 0; // enable effects after some time
bool doorOpen = 0; // door is not open
bool wireMode = 0; // wire mode is not on
int schoolOrBed = 0;// for store the choice, go to school or go to bed

void myReshapeFunc(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
}

// Set the light
void light1(void)
{
	GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_position[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spot_light_direction1[] = { -1, 0, 0 };
	GLfloat spot_light_direction2[] = { 1, 0, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_light_direction1);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_light_direction2);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
}

void light2(void)
{
	GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_position[] = { -0.0f, 298.0f, 75.0f, 1.0f };
	GLfloat spot_light_direction[] = { 0, -1, 0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_light_direction);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);
}

// A method for setting the material
void material(GLfloat difR, GLfloat difG, GLfloat difB, GLfloat specR, GLfloat specG, GLfloat specB, GLfloat emR, GLfloat emG, GLfloat emB, GLfloat shininess)
{
	GLfloat emission[] = { emR, emG, emB,1.0f };
	GLfloat diffuse[] = { difR,difG,difB,1.0f };
	GLfloat specular[] = { specR,specG,specB,1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

// Read a bmp image
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength)
{
	GLubyte* pixeldata;
	FILE* pfile;
	fopen_s(&pfile, path, "rb");
	if (pfile == 0) exit(0);
	fseek(pfile, 0x0012, SEEK_SET);
	fread(&imagewidth, sizeof(imagewidth), 1, pfile);
	fread(&imageheight, sizeof(imageheight), 1, pfile);
	pixellength = imagewidth * 3;
	while (pixellength % 4 != 0)pixellength++;
	pixellength *= imageheight;
	pixeldata = (GLubyte*)malloc(pixellength);
	if (pixeldata == 0) exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(pixeldata, pixellength, 1, pfile);
	p.push_back(pixeldata);
	fclose(pfile);
}

// Make the check texture
void makeCheckImage(void) {
	int i, j, c;
	for (i = 0; i < checkImageWidth; i++) {
		for (j = 0; j < checkImageHeight; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255; // bitwise AND and OR operations,
			checkImage[i][j][0] = (GLubyte)c; // which are faster than normal calculations
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
		}
	}
}

// Make the clock texture
void makeTextureForClock(void) {
	int i, j, c;
	for (i = 0; i < clockImageWidth; i++) {
		for (j = 0; j < clockImageHeight; j++) {
			if (!(j >= 30 && j <= 34 && i <= 50)) {
				clockImage[i][j][0] = (GLubyte)255;
				clockImage[i][j][1] = (GLubyte)255;
				clockImage[i][j][2] = (GLubyte)255;
			}
			else {
				clockImage[i][j][0] = (GLubyte)0;
				clockImage[i][j][1] = (GLubyte)0;
				clockImage[i][j][2] = (GLubyte)0;
			}
		}
	}
}

// Initalize all the textures
void initTexture(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	makeCheckImage();
	makeTextureForClock();
	ReadImage("Star1.bmp", imagewidth[0], imageheight[0], pixellength[0]);
	ReadImage("Sun.bmp", imagewidth[1], imageheight[1], pixellength[1]);
	ReadImage("Mercury.bmp", imagewidth[2], imageheight[2], pixellength[2]);
	ReadImage("Venus.bmp", imagewidth[3], imageheight[3], pixellength[3]);
	ReadImage("Earth.bmp", imagewidth[4], imageheight[4], pixellength[4]);
	ReadImage("Mars.bmp", imagewidth[5], imageheight[5], pixellength[5]);
	ReadImage("Jupiter.bmp", imagewidth[6], imageheight[6], pixellength[6]);
	ReadImage("Saturn.bmp", imagewidth[7], imageheight[7], pixellength[7]);
	ReadImage("Uranus.bmp", imagewidth[8], imageheight[8], pixellength[8]);
	ReadImage("Neptune.bmp", imagewidth[9], imageheight[9], pixellength[9]);
	ReadImage("Moon.bmp", imagewidth[10], imageheight[10], pixellength[10]);
	ReadImage("Star2.bmp", imagewidth[11], imageheight[11], pixellength[11]);
	ReadImage("Star3.bmp", imagewidth[12], imageheight[12], pixellength[12]);
	ReadImage("Star4.bmp", imagewidth[13], imageheight[13], pixellength[13]);
	ReadImage("Star5.bmp", imagewidth[14], imageheight[14], pixellength[14]);
	ReadImage("Star6.bmp", imagewidth[15], imageheight[15], pixellength[15]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set pixel storage mode (in the memory)
	glGenTextures(16, &universalTexture[0]); // number of texture names to be generated and array of texture names
	glGenTextures(2, &roomTexture[0]);
	glBindTexture(GL_TEXTURE_2D, universalTexture[0]); // target to which texture is bound and name of a texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[0], imageheight[0], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[1], imageheight[1], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[2], imageheight[2], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[3], imageheight[3], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[3]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[4], imageheight[4], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[4]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[5], imageheight[5], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[5]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[6], imageheight[6], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[6]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[7], imageheight[7], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[7]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[8], imageheight[8], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[8]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[9], imageheight[9], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[9]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[10], imageheight[10], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[10]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[11], imageheight[11], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[11]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[12], imageheight[12], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[12]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[13], imageheight[13], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[13]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[14]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[14], imageheight[14], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[14]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, universalTexture[15]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth[15], imageheight[15], 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[15]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, roomTexture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, roomTexture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, clockImageWidth, clockImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &clockImage[0][0][0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

// Draw string
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;
	if (isFirstCall) {
		isFirstCall = 0;
		lists = glGenLists(MAX_CHAR);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}

// Set string font
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void myIdleFunc(void)
{
	if (stopMoving == 0 && planetMode == false)
	{
		revolution[0] += 0.47;
		selfRotate[0] += 0.008;
		revolution[1] += 0.35;
		selfRotate[1] += 0.002;
		revolution[2] += 0.29;
		selfRotate[2] += 0.4;
		revolution[3] += 0.24;
		selfRotate[3] += 0.4;
		revolution[4] += 0.13;
		selfRotate[4] += 2;
		revolution[5] += 0.09;
		selfRotate[5] += 2;
		revolution[6] += 0.06;
		selfRotate[6] += 1;
		revolution[7] += 0.05;
		selfRotate[7] += 1;
		revolution[8] += 1;
		selfRotate[8] += 1;
	}
	light1();
	glutPostRedisplay();
}

// The heart image, can change the color of the heart
void draw_heart(float i, float j, float k, float a, float b, float c) {
	double x, y;
	double t;
	glLineWidth(1.0);
	glBegin(GL_POLYGON);
	glColor3f(i, j, k);
	for (t = 0; t <= 360; t++) {
		x = 16 * pow(sin(t * PI / 180), 3) + a;
		y = 13 * cos(t * PI / 180) - 5 * cos(2 * t * PI / 180) - 2 * cos(3 * t * PI / 180) - cos(4 * t * PI / 180) + b;
		glVertex3f(x, y, c);
	}
	glEnd();
}

// Draw sky box
void universalBox(void)
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, universalTexture[14]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-500.0, -500.0, -500.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-500.0, 500.0, -500.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(500.0, 500.0, -500.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(500.0, -500.0, -500.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, universalTexture[11]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-500.0, -500.0, 500.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-500.0, -500.0, -500.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(500.0, -500.0, -500.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(500.0, -500.0, 500.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, universalTexture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-500.0, 500.0, 500.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-500.0, 500.0, -500.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-500.0, -500.0, -500.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-500.0, -500.0, 500.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, universalTexture[13]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(500.0, 500.0, 500.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-500.0, 500.0, 500.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-500.0, 500.0, -500.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(500.0, 500.0, -500.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, universalTexture[12]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(500.0, -500.0, 500.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(500.0, -500.0, -500.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(500.0, 500.0, -500.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(500.0, 500.0, 500.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, universalTexture[15]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-500.0, 500.0, 500.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(500.0, 500.0, 500.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(500.0, -500.0, 500.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-500.0, -500.0, 500.0);
	glEnd();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

// Draw the planets and sun
void drawSunAndPlanets(void)
{
	light1();
	//sun
	material(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1, 1, 1,0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[1]);
	GLUquadric* quadricObj = gluNewQuadric(); // Create a quadric surface object
	gluQuadricTexture(quadricObj, GL_TRUE); // Set texture mode to true
	gluSphere(quadricObj, 90, 80, 80);
	gluDeleteQuadric(quadricObj);

	material(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.4, 0.4, 0.4,0);
	//mercury
	glPushMatrix();
	glRotatef(revolution[0], 0, 1, 0);
	glTranslatef(radius[0], 0, 0);
	glRotatef(selfRotate[0], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[2]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 6, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//venus
	glPushMatrix();
	glRotatef(revolution[1], 0, 1, 0);
	glTranslatef(radius[1], 0, 0);
	glRotatef(3.0, 0, 0, 1);
	glRotatef(-selfRotate[1], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[3]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 6, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//earth
	glPushMatrix();
	glRotatef(revolution[2], 0, 1, 0);
	glTranslatef(radius[2], 0, 0);
	glPushMatrix();
	glRotatef(-23.0, 0, 0, 1);
	glRotatef(selfRotate[2], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[4]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 10, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//moon
	glRotatef(revolution[8], 0, 1, 0);
	glTranslatef(radius[8], 0, 0);
	glRotatef(selfRotate[8], 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[10]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 3, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//mars
	glPushMatrix();
	glRotatef(revolution[3], 0, 1, 0);
	glTranslatef(radius[3], 0, 0);
	glRotatef(-30.0, 0, 0, 1);
	glRotatef(selfRotate[3], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[5]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 8, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//jupiter
	glPushMatrix();
	glRotatef(revolution[4], 0, 1, 0);
	glTranslatef(radius[4], 0, 0);
	glRotatef(-10.0, 0, 0, 1);
	glRotatef(selfRotate[4], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[6]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 25, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//saturn
	glPushMatrix();
	glRotatef(revolution[5], 0, 1, 0);
	glTranslatef(radius[5], 0, 0);
	glPushMatrix();
	glRotatef(-26.0, 0, 0, 1);
	glRotatef(selfRotate[5], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[7]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 20, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//asteroid belt
	glRotatef(-26, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(selfRotate[5], 0, 0, 1);
	material(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.4, 0.4, 0.4, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[7]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluDisk(quadricObj,23, 35, 30, 30);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//uranus
	glPushMatrix();
	glRotatef(revolution[6], 0, 1, 0);
	glTranslatef(radius[6], 0, 0);
	glRotatef(-28, 0, 0, 1);
	glRotatef(selfRotate[6], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[8]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 15, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	//neptune
	glPushMatrix();
	glRotatef(revolution[7], 0, 1, 0);
	glTranslatef(radius[7], 0, 0);
	glRotatef(83.0, 0, 0, 1);
	glRotatef(selfRotate[7], 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, universalTexture[9]);
	quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 15, 80, 80);
	gluDeleteQuadric(quadricObj);
	glPopMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
}

// Draw the man outside the door
void draw_people()
{
	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	double x, y;
	double t;
	double r = 50;
	//head
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= 360; t++)
	{
		x = r * cos(t * PI / 180) + 150;
		y = r * sin(t * PI / 180) + 250;
		glVertex3f(x, y, 0);
	}
	glEnd();

	//emotion
	glPointSize(5.0);
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex3f(0.6 * r * cos(60 * PI / 180) + 150, 0.6 * r * sin(60 * PI / 180) + 250, 0);
	glVertex3f(0.6 * r * cos(120 * PI / 180) + 150, 0.6 * r * sin(120 * PI / 180) + 250, 0);
	glEnd();
	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (t = 220; t <= 320; t++)
	{
		x = 0.5 * r * cos(t * PI / 180) + 150;
		y = 0.5 * r * sin(t * PI / 180) + 250;
		glVertex3f(x, y, 0);
	}
	glEnd();

	//body
	glBegin(GL_LINE_STRIP);
	for (t = 260; t <= 300; t++)
	{
		x = r * cos(t * PI / 180) + 150;
		y = r * sin(t * PI / 180) + 250;
		glVertex3f(x, y, 0);
	}
	glVertex3f(x, y - 100, 0);
	glVertex3f(x + 5, y - 100, 0);
	glVertex3f(x + 5, y - 110, 0);
	glVertex3f(x - 15, y - 110, 0);
	glVertex3f(150, y - 75, 0);
	glVertex3f(315 - x, y - 110, 0);
	glVertex3f(295 - x, y - 110, 0);
	glVertex3f(295 - x, y - 100, 0);
	glVertex3f(300 - x, y - 100, 0);
	glVertex3f(r * cos(240 * PI / 180) + 150, r * sin(240 * PI / 180) + 250, 0);
	glEnd();

	//left hands
	glBegin(GL_LINE_LOOP);
	x = r * cos(240 * PI / 180) + 150;
	y = r * sin(240 * PI / 180) + 250;
	glVertex3f(x, y, 0);
	glVertex3f(x - 30, y - 45, 0);
	glVertex3f(x, y - 45, 0);
	glEnd();

	//right hands
	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	x = r * cos(300 * PI / 180) + 150;
	y = r * sin(300 * PI / 180) + 250;
	glVertex3f(x, y, 0);
	glVertex3f(x + 70, y + 20, 0);
	glVertex3f(x, y - 30, 0);
	glEnd();
}

// Draw the second scene
void realWorld(void)
{
	light2();
	glDisable(GL_TEXTURE_2D);
	//floor, ceil, ground
	material(0, 0, 0, 0, 0, 0, 220.0/255, 220.0/255, 220.0/255, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-200, 0, -100);
	glVertex3f(100, 0, -100);
	glVertex3f(100, 0, 250);
	glVertex3f(-200, 0, 250);
	glEnd();
	material(0, 0, 0, 0, 0, 0, 135.0 / 255, 206.0 / 255, 250.0 / 255,0);
	glBegin(GL_POLYGON);
	glVertex3f(-200, 0, -100);
	glVertex3f(-200, 300, -100);
	glVertex3f(100, 300, -100);
	glVertex3f(100, 0, -100);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-200, 0, -100);
	glVertex3f(-200, 300, -100);
	glVertex3f(-200, 300, 250);
	glVertex3f(-200, 0, 250);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(100, 0, -100);
	glVertex3f(100, 300, -100);
	glVertex3f(100, 300, 250);
	glVertex3f(100, 0, 250);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-200, 0, 250);
	glVertex3f(-200, 300, 250);
	glVertex3f(100, 300, 250);
	glVertex3f(100, 0, 250);
	glEnd();
	material(0, 0, 0, 0, 0, 0, 1, 0, 0,0);
	draw_heart(1, 0, 0, 20, 100, -99.5);
	material(0, 0, 0, 0, 0, 0, 132.0 / 255, 112.0 / 255, 255.0 / 255,0);
	glBegin(GL_POLYGON);
	glVertex3f(-200, 300, -100);
	glVertex3f(100, 300, -100);
	glVertex3f(100, 300, 250);
	glVertex3f(-200, 300, 250);
	glEnd();

	//bed
	material(0, 0, 0, 0, 0, 0, 1, 0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-50, 25, 50);
	glVertex3f(100, 25, 50);
	glVertex3f(100, 25, 250);
	glVertex3f(-50, 25, 250);
	glEnd();

	//table
	glPushMatrix();
	glTranslatef(-160, 35, 100);
	glPushMatrix();
	glScalef(0.8, 0.1, 1.0);
	material(0, 0, 0, 0, 0, 0, 255.0 / 255, 211.0 / 255, 155.0 / 255,0);
	glutSolidCube(100);
	glPopMatrix();
	glTranslatef(0, 5, 0);
	glPushMatrix();
	glTranslatef(0, 0, 25);
	material(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.4, 0.4, 0.4, 100);
	glBegin(GL_POLYGON);
	glVertex3f(-15, 0, 15);
	glVertex3f(-15, 2, 15);
	glVertex3f(-15, 2, -15);
	glVertex3f(-15, 0, -15);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-15, 0, -15);
	glVertex3f(-15, 2, -15);
	glVertex3f(15, 2, -15);
	glVertex3f(15, 0, -15);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(15, 0, -15);
	glVertex3f(15, 2, -15);
	glVertex3f(15, 2, 15);
	glVertex3f(15, 0, 15);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-15, 0, 15);
	glVertex3f(-15, 2, 15);
	glVertex3f(15, 2, 15);
	glVertex3f(15, 0, 15);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roomTexture[0]);
	material(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.4, 0.4, 0.4, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-15.0, 2.0, -15.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(15.0, 2.0, -15.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(15.0, 2.0, 15.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-15.0, 2.0, 15.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPushMatrix();
	material(0, 0, 0, 1, 1, 1, 1.0, 1.0, 1.0,100);
	glutSolidSphere(4, 30, 30);
	glTranslatef(10, 0, 0);
	glutSolidSphere(4, 30, 30);
	glTranslatef(-20, 0, 0);
	glutSolidSphere(4, 30, 30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -10);
	material(0, 0, 0, 1, 1, 1, 0.0, 0.0, 0.0,100);
	glutSolidSphere(4, 30, 30);
	glTranslatef(10, 0, 0);
	glutSolidSphere(4, 30, 30);
	glTranslatef(-20, 0, 0);
	glutSolidSphere(4, 30, 30);
	glPopMatrix();
	material(0, 0, 0, 210.0 / 255, 105.0 / 255, 30.0 / 255, 210.0 / 255, 105.0 / 255, 30.0 / 255, 50);
	glTranslatef(0, 5, -30);
	glRotatef(90, 0, 1, 0);
	glutSolidTeapot(7);
	glPopMatrix();
	//chair
	glPushMatrix();
	glTranslatef(-100, 17.5, 100);
	glRotatef(90, 1, 0, 0);
	material(0, 0, 0, 1, 1, 1, 139.0 / 255, 71.0 / 255, 38.0 / 255, 100);
	glutSolidCylinder(10, 35, 30, 30);
	glPopMatrix();
	//clock
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(25, 200, -98);
	glColor3f(0, 0, 0);
	glutSolidCylinder(15, 4, 30, 30);
	glTranslatef(0, 0, 5);
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roomTexture[1]);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		glTexCoord2f(0.5 + 0.5 * sin(PI * i / 180), 0.5 + 0.5 * cos(PI * i / 180));
		glVertex3f(12 * sin(PI * i / 180), 0, 12 * cos(PI * i / 180));
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//door
	glPushMatrix();
	glTranslatef(-120, 100, -98);
	glPushMatrix();
	glTranslatef(0, 0, -1.9);
	glColor3f(255.0 / 255, 228.0 / 255, 225.0 / 255);
	glBegin(GL_POLYGON);
	glVertex3f(-50, 100, 0);
	glVertex3f(50, 100, 0);
	glVertex3f(50, -100, 0);
	glVertex3f(-50, -100, 0);
	glEnd();
	glTranslatef(-75, -125, 0.1);
	glScalef(0.5, 0.5, 1);
	draw_people();
	glPopMatrix();
	if (doorOpen == 1) {
		glTranslatef(-50, 0, 0);
		glRotatef(-90, 0, 1, 0);
		glTranslatef(50, 0, 0);
	}
	glPushMatrix();
	glScalef(0.5, 1, 0.02);
	glColor3f(255.0 / 255, 211.0 / 255, 155.0 / 255);
	glutSolidCube(200);
	glPopMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(35, 0, -3);
	glutSolidCylinder(2.5, 8, 30, 30);
	glPopMatrix();
	//light
	glEnable(GL_LIGHTING);
	glPushMatrix();
	material(1, 0, 0, 1, 0, 0, 238.0 / 255, 238.0 / 255, 0.0 / 255,100);
	glTranslatef(-50, 297, 75);
	glScalef(15, 1, 15);
	glutSolidCube(6);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHTING);
}

void displayObject(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1.0, 0.1, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// two cameras. use on different situations
	if (timeCounter > 1600)
	{
		gluLookAt(4025, 100, 150, 4025 + 100 * cos(fltRefAngle2 / 180 * PI) * sin((fltRefAngle1 + 180) / 180 * PI), 100 + 100 * sin(fltRefAngle2 / 180 * PI), 150 + 100 * cos(fltRefAngle2 / 180 * PI) * cos((fltRefAngle1 + 180) / 180 * PI), 0, 1, 0);
	}
	else
	{
		gluLookAt(abs(fltRadius * cos(fltY_XZAngle / 180 * PI)) * sin(fltXZAngle / 180 * PI) + fltX0Offest, fltRadius * sin(fltY_XZAngle / 180 * PI), abs(fltRadius * cos(fltY_XZAngle / 180 * PI)) * cos(fltXZAngle / 180 * PI) + fltZ0Offest, fltXRef, fltYRef, fltZRef, 0, 1, 0);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0, 0, fadeAwayZ);
	universalBox();
	drawSunAndPlanets();
	if (clockOn == true)
	{
		if (timeCounter <= 700)
		{
			glDisable(GL_TEXTURE_2D);
			glColor3f(1, 0, 0);
			glTranslatef(-5, 0, 190 - fadeAwayZ);
			selectFont(48, ANSI_CHARSET, "Comic Sans MS");
			glRasterPos2f(0.0, 0.0);
			drawString("Dlinglinglinglingling!!!");
			glEnable(GL_TEXTURE_2D);
		}
		if (timeCounter >= 1000 && timeCounter <= 1600)
		{
			glDisable(GL_TEXTURE_2D);
			glColor3f(1, 1, 1);
			glTranslatef(-5, 0, 190 - fadeAwayZ);
			selectFont(48, ANSI_CHARSET, "Comic Sans MS");
			glRasterPos2f(0.0, 0.0);
			drawString("OH, it's just a dream");
			glEnable(GL_TEXTURE_2D);
		}
		if (timeCounter < 4000) {
			timeCounter++;
		}
		if (fadeAwayZ > -3000)
		{
			fadeAwayZ -= 5;
		}
	}
	glPopMatrix();
	glTranslatef(4000, 0, 0);
	realWorld();
	if (timeCounter >= 2000 && timeCounter <= 3000)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(0, 0, 0);
		glTranslatef(-10 + 20 * cos(fltRefAngle2 / 180 * PI) * sin((fltRefAngle1 + 180) / 180 * PI), 100 + 20 * sin(fltRefAngle2 / 180 * PI), 100 + 20 * cos(fltRefAngle2 / 180 * PI) * cos((fltRefAngle1 + 180) / 180 * PI));
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(0.0, 0.0);
		drawString("6 o'clock, time to go to school");
		glEnable(GL_TEXTURE_2D);
	}
	if (timeCounter > 3000 && timeCounter < 4000)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(0, 0, 0);
		glTranslatef(-10 + 20 * cos(fltRefAngle2 / 180 * PI) * sin((fltRefAngle1 + 180) / 180 * PI), 100 + 20 * sin(fltRefAngle2 / 180 * PI), 100 + 20 * cos(fltRefAngle2 / 180 * PI) * cos((fltRefAngle1 + 180) / 180 * PI));
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(0.0, 0.0);
		drawString("School or bed, make a choice");
		glEnable(GL_TEXTURE_2D);
	}
	if (timeCounter == 4000 && schoolOrBed == 0)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(0, 0, 0);
		glTranslatef(-10 + 20 * cos(fltRefAngle2 / 180 * PI) * sin((fltRefAngle1 + 180) / 180 * PI), 100 + 20 * sin(fltRefAngle2 / 180 * PI), 100 + 20 * cos(fltRefAngle2 / 180 * PI) * cos((fltRefAngle1 + 180) / 180 * PI));
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(0.0, 0.0);
		drawString("Y for school, N for bed");
		glEnable(GL_TEXTURE_2D);
	}
	else if (schoolOrBed == 1)
	{
		timeCounter++; //break the condition, no choice can be done later
		glDisable(GL_TEXTURE_2D);
		glColor3f(0, 0, 0);
		glTranslatef(-10 + 20 * cos(fltRefAngle2 / 180 * PI) * sin((fltRefAngle1 + 180) / 180 * PI), 100 + 20 * sin(fltRefAngle2 / 180 * PI), 100 + 20 * cos(fltRefAngle2 / 180 * PI) * cos((fltRefAngle1 + 180) / 180 * PI));
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(0.0, 0.0);
		drawString("Learning makes me happy. Bye!");
		glEnable(GL_TEXTURE_2D);
		if (timeCounter >= 5000)
			exit(0);
	}
	else if (schoolOrBed == 2)
	{
		if (timeCounter < 5000)
			timeCounter++;
		if (timeCounter >= 4000 && timeCounter < 5000)
		{
			glDisable(GL_TEXTURE_2D);
			glColor3f(0, 0, 0);
			glTranslatef(-5 + 20 * cos(fltRefAngle2 / 180 * PI) * sin((fltRefAngle1 + 180) / 180 * PI), 100 + 20 * sin(fltRefAngle2 / 180 * PI), 100 + 20 * cos(fltRefAngle2 / 180 * PI) * cos((fltRefAngle1 + 180) / 180 * PI));
			selectFont(48, ANSI_CHARSET, "Comic Sans MS");
			glRasterPos2f(0.0, 0.0);
			drawString("Back to dream!!!");
			glEnable(GL_TEXTURE_2D);
		}
		// change back to the first scene, the beginning
		else if (timeCounter == 5000) {
			schoolOrBed = 0;
			planetMode = false;
			timeCounter = 0;
			clockOn = false;
			counter = 0;
			fltRadius = 200;
			fltXZAngle = 0;
			fltY_XZAngle = 0;
			fltXRef = 0.0;
			fltYRef = 0.0;
			fltZRef = 0.0;
			fltX0Offest = 0.0;
			fltZ0Offest = 0.0;
			fadeAwayZ = 0.0;
			stopMoving = 0;
		}

	}
	glutSwapBuffers();
}

// Keyboard input
void myKeyboardFunc(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	//zoom
	if ((key == 'q' || key == 'Q') && clockOn == false)
	{
		if (fltRadius > 5) {
			fltRadius -= 5;
		}
	}
	if ((key == 'e' || key == 'E') && clockOn == false)
	{
		fltRadius += 5;
	}
	//pan
	if ((key == 'a' || key == 'A') && clockOn == false)
	{
		fltXZAngle -= 5;
	}
	if ((key == 'd' || key == 'D') && clockOn == false)
	{
		fltXZAngle += 5;
	}
	if ((key == 'w' || key == 'W') && clockOn == false)
	{
		if (fltY_XZAngle < 90) {
			fltY_XZAngle += 5;
		}
	}
	if ((key == 's' || key == 'S') && clockOn == false)
	{
		if (fltY_XZAngle > -90) {
			fltY_XZAngle -= 5;
		}
	}
	//planets stop moving
	if ((key == 'z' || key == 'Z') && clockOn == false && fltXRef == 0.0 && fltYRef == 0.0 && fltZRef == 0.0)
	{
		stopMoving = (stopMoving xor 1) and 1;
	}
	// open planet mode, for watching every planet.
	if ((key == 'r' || key == 'R') && stopMoving == 1 && clockOn == false)
	{
		planetMode = true;
		fltXRef = radius[counter] * sin((revolution[counter] + 90) / 180 * PI);
		fltX0Offest = fltXRef;
		fltYRef = 0;
		fltZRef = radius[counter] * cos((revolution[counter] + 90) / 180 * PI);
		fltZ0Offest = fltZRef;
		fltRadius = 50;
		fltXZAngle = 0;
		fltY_XZAngle = 0;
		if (counter == 7) {
			counter = -1;
		}
		counter++;
	}
	//cancel planet mode
	if ((key == 'f' || key == 'F') && stopMoving == 1 && clockOn == false)
	{
		planetMode = false;
		counter = 0;
		fltRadius = 200;
		fltXZAngle = 0;
		fltY_XZAngle = 0;
		fltXRef = 0.0;
		fltYRef = 0.0;
		fltZRef = 0.0;
		fltX0Offest = 0.0;
		fltZ0Offest = 0.0;
	}
	// clock on, move to the second scene
	if (key == 't' || key == 'T' && clockOn == false)
	{
		clockOn = true;
		wireMode = 0;
		glPolygonMode(GL_BACK, GL_FILL);
		glPolygonMode(GL_FRONT, GL_FILL);
		counter = 0;
		fltRadius = 200;
		fltXZAngle = 0;
		fltY_XZAngle = 0;
		fltXRef = 0.0;
		fltYRef = 0.0;
		fltZRef = 0.0;
		fltX0Offest = 0.0;
		fltZ0Offest = 0.0;
	}
	//pan in the second scene
	if ((key == 'u' || key == 'U') && clockOn == true)
	{
		if (fltRefAngle2 < 90)
			fltRefAngle2 += 5;
	}
	if ((key == 'j' || key == 'J') && clockOn == true)
	{
		if (fltRefAngle2 > 0)
			fltRefAngle2 -= 5;
	}
	if ((key == 'h' || key == 'H') && clockOn == true)
	{
		if (fltRefAngle1 < 90)
			fltRefAngle1 += 5;
	}
	if ((key == 'k' || key == 'K') && clockOn == true)
	{
		if (fltRefAngle1 > 0)
			fltRefAngle1 -= 5;
	}
	//open and close the door
	if ((key == 'o' || key == 'O') && clockOn == true)
	{
		if (doorOpen == 0)
			doorOpen = 1;
		else if (doorOpen == 1)
			doorOpen = 0;
	}
	//make everything wire
	if ((key == 'l' || key == 'L') && clockOn == false)
	{
		if (wireMode == 0)
			wireMode = 1;
		else if (wireMode == 1)
			wireMode = 0;
		if (wireMode == 1)
		{
			glPolygonMode(GL_BACK, GL_LINE);
			glPolygonMode(GL_FRONT, GL_LINE);
		}
		else if (wireMode == 0)
		{
			glPolygonMode(GL_BACK, GL_FILL);
			glPolygonMode(GL_FRONT, GL_FILL);
		}
	}
	//go to school
	if ((key == 'y' || key == 'Y') && clockOn == true && timeCounter == 4000)
	{
		schoolOrBed = 1;
	}
	//go to bed
	if ((key == 'n' || key == 'N') && clockOn == true && timeCounter == 4000)
	{
		schoolOrBed = 2;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(intWinWidth, intWinHeight);
	glutCreateWindow("Dream World");
	initTexture();
	glutDisplayFunc(displayObject);
	glutIdleFunc(myIdleFunc);
	glutReshapeFunc(myReshapeFunc);
	glutKeyboardFunc(myKeyboardFunc);
	glutMainLoop();
}