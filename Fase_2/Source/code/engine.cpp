#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <ctime>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <array>

#include <cstdio>
#include "tinyxml2.h"
#include "tinyxml2.cpp"

using namespace std;
using namespace tinyxml2;

#define INIT_R 180
#define INIT_ALPHA M_PI/4
#define INIT_BETA M_PI/4
#define R_JUMP 4
#define ALPHA_JUMP 0.1
#define BETA_JUMP 0.08
#define UNIT 15
#define EARTH_SIZE 1.5
#define EARTH_ORBIT_SPEED 6

//Camera
float mouse_x_init, mouse_y_init;
float mouse_x_prev, mouse_y_prev;
float px, py, pz, alpha, beta, r;

//Window
int window;

//FPS counter
int timebase;
float frame;

//VBO
GLuint vertices, verticeCount;

//Rotations
float mercury_angle = 0;
float venus_angle = 0;
float earth_angle = 0;
float mars_angle = 0;
float asteroids_angle = 0;
float jupiter_angle = 0;
float saturn_angle = 0;
float uranus_angle = 0;
float neptune_angle = 0;

float mercury_jump = EARTH_ORBIT_SPEED / 0.238;
float venus_jump = EARTH_ORBIT_SPEED / 0.63;
float earth_jump = EARTH_ORBIT_SPEED;
float mars_jump = EARTH_ORBIT_SPEED / 1.87;
float asteroids_jump = 2 * M_PI / 3;
float jupiter_jump = EARTH_ORBIT_SPEED / 11.86;
float saturn_jump = EARTH_ORBIT_SPEED / 29.42;
float uranus_jump = EARTH_ORBIT_SPEED / 83.55;
float neptune_jump = EARTH_ORBIT_SPEED / 163.72;;

void timer(int value) {
	mercury_angle += mercury_jump;
	venus_angle += venus_jump;
	earth_angle += earth_jump;
	mars_angle += mars_jump;
	asteroids_angle += asteroids_jump;
	jupiter_angle += jupiter_jump;
	saturn_angle += saturn_jump;
	uranus_angle += uranus_jump;
	neptune_angle += neptune_jump;

	glutPostRedisplay();
	glutTimerFunc(1, timer, 0);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	//gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawAxis() {
	glBegin(GL_LINES);
	// X axis in Red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
}

void readXML_aux(XMLNode* node){
	string value = node->Value();
	XMLElement* children = node->ToElement();
	if (strcmp(value.c_str(), "group") == 0) {
	}
	else if (strcmp(value.c_str(), "translate") == 0) {
		int xt = children->DoubleAttribute("X");
		int yt = children->DoubleAttribute("Y");
		int zt = children->DoubleAttribute("Z");
		cout << "X = " << xt << " Y = " << yt << " Z = " << zt << "\n";
	}
	else if (strcmp(value.c_str(), "rotate") == 0) {
		int angler = children->DoubleAttribute("angle");
		int xr = children->DoubleAttribute("axisX");
		int yr = children->DoubleAttribute("axisY");
		int zr = children->DoubleAttribute("axisZ");
		cout << "angulo = " << angler << " X = " << xr << " Y = " << yr << " Z = " << zr;
	}
	else if (strcmp(value.c_str(), "scale") == 0) {
		int xs = children->DoubleAttribute("X");
		int ys = children->DoubleAttribute("Y");
		int zs = children->DoubleAttribute("Z");
		cout << "X = " << xs << " Y = " << ys << " Z = " << zs;
	}
	else if (strcmp(value.c_str(), "models") == 0) {
		/*for (XMLElement* child = children->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
			string ch; ifstream file; vector<float> v; int size;
			file.open(child->Attribute("file"), ios::in);
			if (!file) {
				cout << "There isn't one attribute 'file' in the XML file" << endl;
				return;
			}
			file >> ch;
			size = stoi(ch);
			for (int i = 0; !file.eof(); i++) {
				file >> ch;
				v.push_back(stof(ch));
				if (file.eof()) break;
			}
			file.close();

			verticeCount = size * 3;
			glGenBuffers(1, &vertices);
			glBindBuffer(GL_ARRAY_BUFFER, vertices);
			glBufferData(
				GL_ARRAY_BUFFER, //tipo do buffer, só é relevante na altura do desenho
				sizeof(float) * v.size(), //tamanho do vector em bytes
				v.data(), //os dados do array associado ao vector
				GL_STATIC_DRAW); //indicativo da utilização (estático e para desenho)
		}*/
	}
	if (node->FirstChildElement() != nullptr) {
		readXML_aux(node->FirstChildElement());
	}	
	if (node->NextSiblingElement() != nullptr){
		readXML_aux(node->NextSiblingElement());
	}
}
void readXML() {
	string value;
	XMLDocument doc;
	XMLError load = doc.LoadFile("xmlconf.xml");
	if (load != XML_SUCCESS) {
		cout << "Error in XML file\n" << endl;
		return;
	}
	XMLNode* node = doc.FirstChildElement("scene");
	if (node == nullptr) return;
	readXML_aux(node);
}

/*
void drawSphere(float radius, int slices, int stacks) {
	float a = 0;
	float b = 0;
	float a_interval = 2 * M_PI / slices;
	float b_interval = M_PI / stacks;
	float next_a, next_b;

	for (; a < 2 * M_PI; a += a_interval) {
		for (b = -M_PI / 2; b < M_PI / 2; b += b_interval) {
			next_a = a + a_interval;
			next_b = b + b_interval;
			if (next_a > 2 * M_PI) {
				next_a = 2 * M_PI;
			}
			if (next_b > M_PI / 2) {
				next_b = M_PI / 2;
			}
			glBegin(GL_TRIANGLES);
			//Squares
			glVertex3f(radius * cos(next_b) * sin(next_a), radius * sin(next_b), radius * cos(next_b) * cos(next_a));
			glVertex3f(radius * cos(next_b) * sin(a), radius * sin(next_b), radius * cos(next_b) * cos(a));
			glVertex3f(radius * cos(b) * sin(next_a), radius * sin(b), radius * cos(b) * cos(next_a));

			glVertex3f(radius * cos(b) * sin(next_a), radius * sin(b), radius * cos(b) * cos(next_a));
			glVertex3f(radius * cos(next_b) * sin(a), radius * sin(next_b), radius * cos(next_b) * cos(a));
			glVertex3f(radius * cos(b) * sin(a), radius * sin(b), radius * cos(b) * cos(a));
			glEnd();

		}
	}
}
void drawSun() {
	glColor3f(1, 0.8, 0);
	drawSphere(2.5 * EARTH_SIZE, 30, 30);
}

void drawMercury() {
	int dist = 0.4 * UNIT;
	glColor3f(0.3, 0.3, 0.3);
	glPushMatrix();
	glRotatef(mercury_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	drawSphere(EARTH_SIZE * 1 / 3, 20, 20);
	glPopMatrix();
	//glRotatef();
}

void drawVenus() {
	int dist = 0.7 * UNIT;
	glColor3f(0.73, 0.714, 0.667);
	glPushMatrix();
	glRotatef(venus_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	drawSphere(0.9 * EARTH_SIZE, 20, 20);
	glPopMatrix();
	//glRotatef();
}

void drawEarth() {
	int dist = 1 * UNIT;
	glPushMatrix();
	glRotatef(earth_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	glColor3f(0.269, 0.507, 0.776);
	drawSphere(EARTH_SIZE, 20, 20);
	glPopMatrix();
	//glRotatef();
}

void drawMars() {
	int dist = 1.5 * UNIT;
	glPushMatrix();
	glRotatef(mars_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	glColor3f(0.52, 0.26, 0.089);
	drawSphere(EARTH_SIZE / 2, 20, 20);
	glPopMatrix();
	//glRotatef();
}

void drawAsteroidBelt(float speed) {
	int dist1 = 2.3 * UNIT;
	int dist2 = 3.3 * UNIT;
	int x, z, d;
	float c;
	for (float a = 0; a < 2 * M_PI; a += M_PI / 300) {
		d = (rand() % (dist2 - dist1)) + dist1;
		x = d * sin(a);
		z = d * cos(a);
		c = rand() % 1;
		glColor3f(0.55, 0.5, 0.51);
		glPushMatrix();
		glRotatef(speed * asteroids_angle, 0, 1, 0);
		glTranslatef(x, 0, z);
		drawSphere(EARTH_SIZE * 0.1, 3, 3);
		glPopMatrix();
	}
}

void drawJupiter() {
	int dist = 5.2 * UNIT;
	glPushMatrix();
	glRotatef(jupiter_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	glColor3f(0.62, 0.6, 0.49);
	drawSphere(2 * EARTH_SIZE, 20, 20);
	glPopMatrix();
	//glRotatef();
}

void drawRing(float radius, int slices, int stacks) {
	float interval = 2 * M_PI / slices;
	float next_a;

	for (float a = 0; a < 2 * M_PI; a += interval) {
		next_a = a + interval;
		if (next_a > 2 * M_PI) {
			next_a = 2 * M_PI;
		}
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0, 0.0f);
		glVertex3f(radius * sin(next_a), 0, radius * cos(next_a));
		glVertex3f(radius * sin(a), 0, radius * cos(a));

		glVertex3f(0.0f, 0, 0.0f);
		glVertex3f(radius * sin(a), 0, radius * cos(a));
		glVertex3f(radius * sin(next_a), 0, radius * cos(next_a));
		glEnd();
	}
}
void drawSaturn() {
	int dist = 9.2 * UNIT;
	glPushMatrix();
	glRotatef(saturn_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	glColor3f(0.52, 0.5, 0.39);
	drawSphere(1.4 * EARTH_SIZE, 20, 20);
	glColor3f(0.42, 0.26, 0.189);
	drawRing(1.4 * EARTH_SIZE * 2, 5, 5);
	glPopMatrix();
}

void drawUranus() {
	int dist = 19.2 * UNIT;
	glPushMatrix();
	glRotatef(uranus_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	glColor3f(0.369, 0.607, 0.896);
	drawSphere(1.2 * EARTH_SIZE, 10, 10);
	glPopMatrix();
	//glRotatef();
}

void drawNeptune() {
	int dist = 30.1 * UNIT;
	glPushMatrix();
	glRotatef(neptune_angle, 0, 1, 0);
	glTranslatef(dist, 0, 0);
	glColor3f(0.369, 0.607, 0.976);
	drawSphere(1.2 * EARTH_SIZE, 10, 10);
	glPopMatrix();
	//glRotatef();
}
*/
void renderScene(void) {
	srand(1);
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	px = r * cos(beta) * sin(alpha), py = r * sin(beta), pz = r * cos(beta) * cos(alpha);
	gluLookAt(px, py, pz, 0, 0, 0, 0, 1, 0);

	// put the geometric transformations here

	// put drawing instructions here	
	/*
	drawSun();
	drawMercury();
	drawVenus();
	drawEarth();
	drawMars();
	drawAsteroidBelt(1);
	drawAsteroidBelt(1.7);
	drawJupiter();
	drawSaturn();
	drawUranus();
	drawNeptune();
	*/
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, verticeCount);


	//FPS counter
	frame++;
	int final_time = glutGet(GLUT_ELAPSED_TIME);

	if (final_time - timebase > 1000) {
		int fps = frame * 1000.0f / (final_time - timebase);
		char title[(((sizeof fps) * CHAR_BIT) + 2) / 3 + 2];
		sprintf_s(title, "FPS: %d", fps);
		glutSetWindowTitle(title);
		timebase = final_time;
		frame = 0;
	}

	// End of frame
	glutSwapBuffers();
}

// write function to process keyboard events
void mouse(int button, int state, int mouse_x, int mouse_y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouse_x_init = mouse_x;
		mouse_y_init = mouse_y;
		mouse_x_prev = mouse_x_init;
		mouse_y_prev = mouse_y_init;
	}
}

void mouse_motion(int mouse_x, int mouse_y) {
	if (mouse_x > mouse_x_prev) {
		alpha += ALPHA_JUMP;
	}
	if (mouse_x < mouse_x_prev) {
		alpha -= ALPHA_JUMP;
	}
	if (mouse_y > mouse_y_prev) {
		if (beta < M_PI / 2 - BETA_JUMP) {
			beta += BETA_JUMP;
		}
	}
	if (mouse_y < mouse_y_prev) {
		if (beta > -M_PI / 2 + BETA_JUMP) {
			beta -= BETA_JUMP;
		}
	}

	mouse_x_prev = mouse_x;
	mouse_y_prev = mouse_y;
	glutPostRedisplay();

}

void keyboard_func(unsigned char c, int mouse_x, int mouse_y) {

	switch (c) {
	case 27: {                    //this is escape key
		glutDestroyWindow(window);
		exit(0);
	}
	case ' ': {
		alpha = INIT_ALPHA;
		beta = INIT_BETA;
		r = INIT_R;
		break;
	}
	case 'w': {
		r -= R_JUMP;
		break;
	}
	case 's': {
		r += R_JUMP;
		break;
	}
	default: {
		break;
	}
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	alpha = INIT_ALPHA;
	beta = INIT_BETA;
	r = INIT_R;


	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	window = glutCreateWindow("Window");


	glewInit();
	//getGroup();
	readXML();
	// Required callback registry 
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	//FPS Counter
	timebase = glutGet(GLUT_ELAPSED_TIME);

	// put here the registration of the keyboard callbacks
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutKeyboardFunc(keyboard_func);

	//  OpenGL settings
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
	glutTimerFunc(0, timer, 0);

	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}
