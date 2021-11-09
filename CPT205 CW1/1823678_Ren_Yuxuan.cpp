#define FREEGLUT_STATIC
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <GL/freeglut.h> 
#define PI 3.1416
int r_toy = 0; // windmill rotate
int r_sunlight = 0;	//sunlights rotate
float r_earth = 0.0f; //Trees and flowers rotate
bool light_off = false; //True means night time
bool blow_candles = false; //whether the boy blows the candles
bool camera_trans = false; //whether the camera has moved
bool start = false; //when it is true, the heart start translating
bool left_mouse_control = true; //control whether the left mouse can be used
bool smile_sun = false; //control if the sun will give you a smile
float t_heart = 0.0;
float t_left = 0.0;
float s_heart = 1.0;
float t_stage_change = 0.0;
int effect_counter = 0; //control the effect time
GLdouble x = 0.0f, y = 0.0f, z = 0.0f;
GLfloat background_R = 1, background_G = 1, background_B = 1, background_ALPHA = 1; //background color
GLfloat ballon_rise = 0; //control the ballon height
int another_ballon = 0;	//control the ballon generation position
GLfloat firework_rise = 0; //control the fireworks height

void when_in_mainloop() { // idle callback function  
	glutPostRedisplay(); // force OpenGL to redraw the current window
} 

//Set string font
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_SWISS, face);
	HFONT holdFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(holdFont);
}

//draw string
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;
	if (isFirstCall) {
		isFirstCall = 0; 
		lists = glGenLists(128);  
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
	}
	for (; *str != '\0'; ++str) { 
		glCallList(lists + *str);
	}
}

//keyboard input
void keyboard_input(unsigned char key, int x, int y) {
	if (key == 'q' || key == 'Q')
		exit(0);
	else if (key == 's' || key == 'S')
		start = true;
	else if (key == 'z' || key == 'Z')
		smile_sun = true;
	else if (key == 'x' || key == 'X')
		smile_sun = false;
	else if (key == 'd' || key == 'D') {
		background_R = 25.0 / 255;
		background_G = 25.0 / 255;
		background_B = 112.0 / 255;
		light_off = true;
	}
	else if ((key == 'b' || key == 'B')&& light_off == true){
		blow_candles = true; 
	}

}

//mouse input
void mouse_input(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && t_heart == 250 && left_mouse_control == true) {
		s_heart += 15;
	}
}

void draw_stage() {
	float x, y;
	glLineWidth(1.0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(780, 30);
	glVertex2f(780, 80);
	glVertex2f(10, 80);
	for (int t = 10; t <= 780; t += 5) {
		x = (float)t;
		y = 10 * sin(5 * (t - 10.0) * PI / 180) + 40;
		glVertex2f(x, y);
	}
	glEnd();
}

void door_text_area() {
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(135.0/255, 206.0/255, 235.0/255);
	glVertex2f(300, 500);
	glVertex2f(700, 500);
	glVertex2f(700, 400);
	glVertex2f(300, 400);
	glEnd();
}

//the three parameters for the RGB value of the circle.
void draw_wall(float a, float b, float c) {
	float x, y;
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(232.0 / 255, 242.0 / 255, 212.0 / 255);
	glVertex2f(700, 100);
	glVertex2f(700, 400);
	glVertex2f(300, 400);
	glVertex2f(300, 100);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(a,b,c);
	for (int t = 0; t < 360; t++) {
		x = 25 * cos(t * PI / 180) + 600;
		y = 25 * sin(t * PI / 180) + 350;
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(a, b, c);
	for (int t = 0; t < 360; t++) {
		x = 25 * cos(t * PI / 180) + 400;
		y = 25 * sin(t * PI / 180) + 350;
		glVertex2f(x, y);
	}
	glEnd();
	if (t_heart == 250) {
		glLineWidth(10.0);
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		for (int t = 180; t < 360; t++) {
			x = 100 * cos(t * PI / 180) + 500;
			y = 100 * sin(t * PI / 180) + 250;
			glVertex2f(x, y);
		}
		glEnd();
	}
}

void draw_people() {
	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	double x, y;
	double t;
	double r = 50;
	//head
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= 360; t++) {
		x = r * cos(t * PI / 180)+150;
		y = r * sin(t * PI / 180) + 250;
		glVertex2f(x, y);
	}
	glEnd();

	//emotion
	glPointSize(5.0);
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex2f(0.6 * r * cos(60 * PI / 180) + 150, 0.6 * r * sin(60 * PI / 180) + 250);
	glVertex2f(0.6 * r * cos(120 * PI / 180) + 150, 0.6 * r * sin(120 * PI / 180) + 250);
	glEnd();
	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (t = 220; t <= 320; t++) {
		x = 0.5 * r * cos(t * PI / 180) + 150;
		y = 0.5 * r * sin(t * PI / 180) + 250;
		glVertex2f(x, y);
	}
	glEnd();

	//body
	glBegin(GL_LINE_STRIP);
	for (t = 260; t <= 300; t++) {
		x = r * cos(t * PI / 180) + 150;
		y = r * sin(t * PI / 180) + 250;
		glVertex2f(x, y);
	}
	glVertex2f(x, y - 100);
	glVertex2f(x + 5, y - 100);
	glVertex2f(x + 5, y - 110);
	glVertex2f(x -15, y - 110);
	glVertex2f(150, y - 75);
	glVertex2f(315-x, y - 110);
	glVertex2f(295-x, y - 110);
	glVertex2f(295-x, y - 100);
	glVertex2f(300-x, y - 100);
	glVertex2f(r * cos(240 * PI / 180) + 150, r * sin(240 * PI / 180) + 250);
	glEnd();

	//left hands
	glBegin(GL_LINE_LOOP);
	x = r * cos(240 * PI / 180) + 150;
	y = r * sin(240 * PI / 180) + 250;
	glVertex2f(x, y);
	glVertex2f(x - 30, y - 45);
	glVertex2f(x, y - 45);
	glEnd();

	glLineWidth(3.0);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(x-30,y-45);
	glVertex2f(x - 80, y +5);
	glEnd();
	

	//right hands
	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	x = r * cos(300 * PI / 180) + 150;
	y = r * sin(300 * PI / 180) + 250;
	glVertex2f(x, y);
	glVertex2f(x + 70, y + 20);
	glVertex2f(x, y - 30);
	glEnd();
}

//can draw hearts with different colors and at different positions
void draw_heart(float a, float b, float i, float j ,float k) {
	double x, y;
	double t;
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(i, j, k);
	for (t = 0; t <= 360; t++) {
		x = 16*pow(sin(t*PI/180),3) + a;
		y = 13 * cos(t * PI / 180) - 5 * cos(2 * t * PI / 180) - 2 * cos(3 * t * PI / 180) - cos(4 * t * PI / 180) + b;
		glVertex2f(x, y);
	}
	glEnd();
}

void draw_windmill() {
	double r = 50;
	int x = r * cos(240 * PI / 180) + 70;
	int y = r * sin(240 * PI / 180) + 255;
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x - 10, y - 10);
	glVertex2f(x, y - 10);
	glEnd();
	glColor3f(212.0 / 255, 242.0 / 255, 232.0 / 255);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x, y + 10);
	glVertex2f(x + 10, y + 10);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 1);
	glVertex2f(x, y);
	glColor3f(1, 1, 0);
	glVertex2f(x - 10, y);
	glColor3f(0, 1, 1);
	glVertex2f(x - 10, y + 10);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 1);
	glVertex2f(x, y);
	glColor3f(1, 0, 0);
	glVertex2f(x + 10, y);
	glColor3f(0, 1, 0);
	glVertex2f(x + 10, y - 10);
	glEnd();
}

void heart_effect() {
	float x1, y1, x2, y2;
	glColor3f(0,0,0);
	glBegin(GL_LINES);
	for (int t = 0; t < 360; t += 60) {
		x1 = 30 * sin(t * PI / 180) + 500;
		y1 = 30 * cos(t * PI / 180) + 250;
		x2 = 60 * sin(t * PI / 180) + 500;
		y2 = 60 * cos(t * PI / 180) + 250;
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}
	glEnd();
	effect_counter++;
}

void draw_sun() {
	int r = 25;
	float x, y;
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glColor3f(218.0/255, 165.0/255, 32.0/255);
	if (smile_sun == false) {
		for (int t = 0; t < 360; t++) {
			x = r * cos(t * PI / 180) + 1250;
			y = r * sin(t * PI / 180) + 500;
			glVertex2f(x, y);
		}
	}
	else {
		for (int t = 360; t >0; t--) {
			x = r * cos(t * PI / 180) + 1250;
			y = r * sin(t * PI / 180) + 500;
			glVertex2f(x, y);
		}
	}
	glEnd();
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2f(0.6 * r * cos(60 * PI / 180) + 1250, 0.6 * r * sin(60 * PI / 180) + 500);
	glVertex2f(0.6 * r * cos(120 * PI / 180) + 1250, 0.6 * r * sin(120 * PI / 180) + 500);
	glEnd();
	glLineWidth(1.0);
	glBegin(GL_LINE_STRIP);
	for (int t = 220; t <= 320; t++) {
		x = 0.5 * r * cos(t * PI / 180) + 1250;
		y = 0.5 * r * sin(t * PI / 180) + 500;
		glVertex2f(x, y);
	}
	glEnd();

	
}

void draw_moon() {
	int r = 25;
	float x, y;
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(214.0 / 255, 236.0 / 255, 240.0 / 255);
	for (int t = 0; t < 360; t++) {
		x = r * cos(t * PI / 180) + 1250;
		y = r * sin(t * PI / 180) + 500;
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(25.0 / 255, 25.0 / 255, 112.0 / 255);
	for (int t = 0; t < 360; t++) {
		x =0.8*r * cos(t * PI / 180) + 1225;
		y =0.8*r * sin(t * PI / 180) + 500;
		glVertex2f(x, y);
	}
	glEnd();
}

//sunlight around the sun
void draw_sunlight() {
	float x1, y1, x2, y2;
	glLineWidth(1.0);
	glColor3f(218.0 / 255, 165.0 / 255, 32.0 / 255);
	glBegin(GL_LINES);
	for (int t = 0; t < 360; t += 60) {
		x1 = 30 * sin(t * PI / 180) + 1250;
		y1 = 30 * cos(t * PI / 180) + 500;
		x2 = 60 * sin(t * PI / 180) + 1250;
		y2 = 60 * cos(t * PI / 180) + 500;
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}
	glEnd();
}

void draw_earth() {
	float x, y;
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(212.0 / 255, 242.0 / 255, 232.0 / 255);
	for (int t = 0; t < 360; t++) {
		x = 300 * cos(t * PI / 180) + 1500;
		y = 300 * sin(t * PI / 180);
		glVertex2f(x, y);
	}
	glEnd();
}

//parameter for rotating the tree
void draw_tree(int tree_degree) {
	glPushMatrix();
	glTranslatef(1500, 0, 0);
	glRotatef(-r_earth, 0, 0, 1);
	glTranslatef(-1500, 0, 0);
	glTranslatef(300 * cos(tree_degree * PI / 180) + 1500, 300 * sin(tree_degree * PI / 180),0);
	glRotatef(tree_degree - 90, 0, 0, 1);
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(128.0 / 255, 64.0 / 255, 0.0 / 255);
	glVertex2f(-5, -5);
	glVertex2f(5, -5);
	glVertex2f(5, 20);
	glVertex2f(-5, 20);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(60.0 / 255, 179.0 / 255, 113.0 / 255);
	glVertex2f(-20, 20);
	glVertex2f(20, 20);
	glVertex2f(0, 65);

	glVertex2f(-15, 40);
	glVertex2f(15, 40);
	glVertex2f(0, 85);
	glEnd();
	glPopMatrix();
}

//parameters for deciding whether the candles should be lit or not
void draw_birthday_cake(bool light_off, bool blow_candles) {
	glPushMatrix();
	glTranslatef(1450, 25, 0);
	glLineWidth(1.0);
	glBegin(GL_POLYGON);
	glColor3f(221.0 / 255, 160.0 / 255, 221.0 / 255);
	glVertex2f(-100, 0);
	glVertex2f(-100, 50);
	glVertex2f(100, 50);
	glVertex2f(100, 0);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(218.0 / 255, 112.0 / 255, 214.0 / 255);
	glVertex2f(-70, 50);
	glVertex2f(-70, 100);
	glVertex2f(70, 100);
	glVertex2f(70, 50);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(255.0 / 255, 192.0 / 255, 203.0 / 255);
	glVertex2f(-50, 100);
	glVertex2f(-50, 150);
	glVertex2f(50, 150);
	glVertex2f(50, 100);
	glEnd();
	glLineWidth(7.0);
	glBegin(GL_LINES);
	glColor3f(255.0 / 255, 222.0 / 255, 173.0 / 255);
	glVertex2f(-30, 150);
	glVertex2f(-30, 180);

	glVertex2f(0, 150);
	glVertex2f(0, 180);

	glVertex2f(30, 150);
	glVertex2f(30, 180);
	glEnd();

	if (light_off == true && blow_candles == false) {
		glPointSize(10.0);
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex2f(-30, 181);
		glVertex2f(0, 181);
		glVertex2f(30, 181);
		glEnd();
	}

	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	int r = 30;
	
	//the smile face next to the cake
	glBegin(GL_LINE_STRIP);
	for (int t = 0; t <= 360; t++) {
		glVertex2f(r * cos(t * PI / 180) + 100, r * sin(t * PI / 180) + 190);
	}
	glEnd();
	glPointSize(5.0);
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex2f(0.6 * r * cos(60 * PI / 180) + 100, 0.6 * r * sin(60 * PI / 180) + 190);
	glVertex2f(0.6 * r * cos(120 * PI / 180) + 100, 0.6 * r * sin(120 * PI / 180) + 190);
	glEnd();
	glLineWidth(1.0);
	glColor3f(0, 0, 0);
	if (blow_candles == false) {
		glBegin(GL_LINE_STRIP);
		for (int t = 220; t <= 320; t++) {
			glVertex2f(0.5 * r * cos(t * PI / 180) + 100, 0.5 * r * sin(t * PI / 180) + 190);
		}
		glEnd();
	}
	else {
		glBegin(GL_LINE_STRIP);
		for (int t = 0; t <360; t++) {
			glVertex2f(5 * cos(t * PI / 180) + 100, 5 * sin(t * PI / 180) + 180);
		}
		glEnd();
	}
	glPopMatrix();
}

void draw_flower(int flower_degree) {
	glPushMatrix();
	glTranslatef(1500, 0, 0);
	glRotatef(-r_earth, 0, 0, 1);
	glTranslatef(-1500, 0, 0);
	glTranslatef(300 * cos(flower_degree * PI / 180) + 1500, 300 * sin(flower_degree * PI / 180), 0);
	glRotatef(flower_degree - 90, 0, 0, 1);
	glColor3f(0, 1, 0);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex2f(0,0);
	glVertex2f(0,20);
	glEnd();
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(rand() % 10 * 0.05+0.5, rand()%10*0.05, rand() % 10 * 0.05);
	for (int t = 0; t <360; t++) {
		glVertex2f(4 * (2 + sin(6 * (t+10.0) * PI / 180)) * cos((t + 10.0) * PI / 180), 4 * (2 + sin(6 * (t+10.0) * PI / 180)) * sin((t + 10.0) * PI / 180) + 20);
	}
	glEnd();
	glPopMatrix();
}

void draw_ballon(GLint ballon_x, GLint ballon_y,GLfloat r,GLfloat g,GLfloat b) {
	
	glPushMatrix();
	glTranslatef(1300+(ballon_x+another_ballon)%550,ballon_y+ballon_rise,0);
	glRotatef(-90, 0, 0, 1);
	glColor3f(r, g, b);
	glBegin(GL_LINE_STRIP);
	for (int t = 0; t < 40; t++) {
		glVertex2f(t, 5 * sin(9.0 * t * PI / 180.0));
	}
	glEnd();
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	for (int t = 0; t < 360; t++) {
		glVertex2f(20*sin(t*PI/180.0)-20,15*cos(t*PI/180.0));
	}
	glEnd();
	glPopMatrix();
}

void draw_fireworks(int radius,GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(r, g, b);
	glLineWidth(1.0);
	glBegin(GL_LINES);
	for (int t = 0; t < 360; t+=10) {
		glVertex2f(0, 0);
		glVertex2f(radius * sin(t * PI / 180), radius * cos(t * PI / 180));
	}
	glEnd();
	glPopMatrix();

}

//the track of the firework
void draw_fireworks_track(GLint x, GLint y ,GLfloat r, GLfloat g, GLfloat b) {
	glPushMatrix();
	glTranslatef(x, y + firework_rise, 0);
	glLineWidth(5.0);
	glColor3f(255.0 / 255, 70.0 / 255, 81.0 / 255);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, -5);
	glEnd();
	glPopMatrix();
}

// scene one
void draw_scene_one() {
	glPushMatrix();
	glTranslatef(t_stage_change, 0, 0);
	draw_stage();
	draw_people();
	door_text_area();
	//two types of doors(different situations)
	if (t_heart < 250) {
		draw_wall(1, 0, 0);
	}
	else {
		draw_wall(0, 1, 0);
	}
	//content on the door
	glColor3f(72.0 / 255, 61.0 / 255, 139.0 / 255);
	selectFont(48, ANSI_CHARSET, "Comic Sans MS");
	glRasterPos2f(375, 440);
	drawString("Birthday World");
	glColor3f(255.0 / 255, 106.0 / 255, 106.0 / 255);
	selectFont(48, ANSI_CHARSET, "Comic Sans MS");

	glRasterPos2f(373, 438);
	drawString("Birthday World");
	draw_heart(500, 250, 1, 1, 1);
	glTranslatef(50 * cos(240 * PI / 180) + 70, 50 * sin(240 * PI / 180) + 255, 0);
	glRotatef(r_toy, 0, 0, 1);
	glTranslatef(-50 * cos(240 * PI / 180) - 70, -50 * sin(240 * PI / 180) - 255, 0);
	draw_windmill();
	glPopMatrix();

	//the transformations of the red heart
	glPushMatrix();
	glTranslatef(t_heart, 0, 0);
	glTranslatef(250, 250, 0);
	glScalef(s_heart, s_heart, 0);
	glTranslatef(-250, -250, 0);
	draw_heart(250, 250, 1, 0, 0);
	glPopMatrix();

	//for set the parameters
	r_toy = (r_toy + 1) % 360;
	if (s_heart >= 80) {
		left_mouse_control = false;
		t_stage_change = -1000;
	}
	if (left_mouse_control == false) {
		if (s_heart > 0) {
			s_heart -= 0.5;
		}
	}
	//if press 's', start to move the heart
	if (start == true && t_heart < 250) {
		t_heart += 0.5;
	}
	// if heart is moving in place, play the effect
	if (t_heart == 250 && effect_counter < 20) {
		heart_effect();
	}
}

//scene two
void draw_scene_two() {
	glTranslatef(t_stage_change, 0, 0);
	//draw things for night or things for day
	if (light_off == false) {
		draw_sun();
		draw_ballon(200, 300, 1, 0, 0);
		draw_ballon(400, 100, 240.0 / 255, 230.0 / 255, 140.0 / 255);
		draw_ballon(600, 200, 255.0 / 255, 127.0 / 255, 80.0 / 255);
		draw_ballon(500, 400, 0, 1, 1);
	}
	else {
		draw_moon();
	}
	ballon_rise += 0.2;
	if (ballon_rise >= 600) {
		ballon_rise = 0.0;
		another_ballon = rand() % 100 + 100;
	}
	draw_earth();
	draw_birthday_cake(light_off,blow_candles);

	//draw flowers and trees
	for (int t = 0; t < 360; t += 30) {
		draw_tree(t);
	}
	for (int t = 15; t < 360; t += 30) {
		draw_flower(t);
	}
	//for rotate the earth
	r_earth += 0.01;
	
	glColor3f(72.0 / 255, 61.0 / 255, 139.0 / 255);
	selectFont(48, ANSI_CHARSET, "Comic Sans MS");
	glRasterPos2f(1375, 500);
	drawString("Happy Birthday!");
	glRasterPos2f(1450, 450);
	drawString("Happy Everyday!");
	
	//draw sunlight
	if (light_off == false) {
		glPushMatrix();
		glTranslatef(1250, 500, 0);
		glRotatef(r_sunlight, 0, 0, 1);
		glTranslatef(-1250, -500, 0);
		draw_sunlight();
		glPopMatrix();
		r_sunlight = (r_sunlight + 1) % 360;
	}

	if (blow_candles == true) {
		if (firework_rise < 200) {
			//several fireworks track, They will disappear if they reach a certain height
			draw_fireworks_track(1250, 200, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1470, 350, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1530, 350, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1500, 225, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1450, 235, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1550, 235, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1400, 275, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1600, 275, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1750, 240, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			draw_fireworks_track(1800, 275, 205.0 / 255, 55.0 / 255, 0.0 / 255);
			firework_rise += 0.5;
		}
		else {
			//After fireworks track reach a certain height, real fireworks appear
			draw_fireworks(30, 1250, 400, 238.0 / 255, 201.0 / 255, 0.0 / 255);
			draw_fireworks(15, 1470, 550, 192.0 / 255, 255.0 / 255, 238.0 / 255);
			draw_fireworks(15, 1530, 550, 192.0 / 255, 255.0 / 255, 238.0 / 255);
			draw_fireworks(20, 1500, 425, 192.0 / 255, 255.0 / 255, 238.0 / 255);
			draw_fireworks(20, 1450, 435, 192.0 / 255, 255.0 / 255, 238.0 / 255);
			draw_fireworks(20, 1550, 435, 192.0 / 255, 255.0 / 255, 238.0 / 255);
			draw_fireworks(20, 1400, 475, 192.0 / 255, 255.0 / 255, 238.0 / 255);
			draw_fireworks(20, 1600, 475, 192.0 / 255, 255.0 / 255, 238.0 / 255);
			draw_fireworks(40, 1750, 440, 238.0 / 255, 118.0 / 255, 0.0 / 255);
			draw_fireworks(70, 1800, 475, 205.0 / 255, 55.0 / 255, 0.0 / 255);
		}
	}
}

void display(void) {
	glClearColor(background_R, background_G, background_B, background_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 800, 600);//the viewport should always be 800*600
	gluLookAt(x, y, z, x, y, z - 1.0, 0.0f, 1.0f, 0.0f);
	gluOrtho2D(0, 800, 0, 600);
	//draw scene one
	draw_scene_one();
	//draw scene two
	draw_scene_two();
	//increase x to make the camera shifting
	if (t_heart >= 250 && x < 0.3 && effect_counter == 20) {
		x += 0.001;
	}
	glFlush();
}

int main(int argc, char** argv) { 
	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("BirthdayWorld.cpp");
	glutDisplayFunc(display);
	glutIdleFunc(when_in_mainloop);
	glutKeyboardFunc(keyboard_input);
	glutMouseFunc(mouse_input);
	glutMainLoop(); 
}