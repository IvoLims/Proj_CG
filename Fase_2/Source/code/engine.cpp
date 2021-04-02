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
vector<float> v;

//Figures
struct FIGURE {
	int beg;
	int count;
	GLfloat m[16];
};
vector<FIGURE> figures;

void timer(int value) {

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
	glColor3f(0.4f, 0.5f, 1.0f);
}

void readXML_aux(XMLNode* node) {
	string value = node->Value();
	float angle, x, y, z;
	string ch; ifstream file;
	XMLElement* node_elem = node->ToElement();
	if (strcmp(value.c_str(), "group") == 0) {
		glPushMatrix();
		readXML_aux(node->FirstChildElement());
		glPopMatrix();
	}
	else if (strcmp(value.c_str(), "translate") == 0) {
		x = node_elem->DoubleAttribute("X");
		y = node_elem->DoubleAttribute("Y");
		z = node_elem->DoubleAttribute("Z");
		glTranslatef(x, y, z);
	}
	else if (strcmp(value.c_str(), "rotate") == 0) {
		angle = node_elem->DoubleAttribute("angle");
		x = node_elem->DoubleAttribute("axisX");
		y = node_elem->DoubleAttribute("axisY");
		z = node_elem->DoubleAttribute("axisZ");
		glRotatef(angle, x, y, z);
	}
	else if (strcmp(value.c_str(), "scale") == 0) {
		x = node_elem->DoubleAttribute("X");
		y = node_elem->DoubleAttribute("Y");
		z = node_elem->DoubleAttribute("Z");
		glScalef(x, y, z);
	}
	else if (strcmp(value.c_str(), "models") == 0) {
		for (XMLElement* children = node_elem->FirstChildElement(); children != nullptr; children = children->NextSiblingElement()) {
			int model_size;
			FIGURE new_figure;
			new_figure.beg = v.size() / 3;
			file.open(children->Attribute("file"));
			if (!file) {
				cout << "There isn't one attribute 'file' in the XML file" << endl;
				continue;
			}
			file >> ch;
			model_size = stoi(ch) * 3;
			for (file >> ch; !file.eof(); file >> ch) {
				v.push_back(stof(ch));
			}
			file.close();
			new_figure.count = model_size;
			glGetFloatv(GL_MODELVIEW_MATRIX, new_figure.m);
			figures.push_back(new_figure);
		}
	}
	if (node->NextSiblingElement() != nullptr) {
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
	if (node->FirstChildElement() != nullptr) {
		node = node->FirstChildElement();
	}
	readXML_aux(node);
	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * v.size(),
		v.data(),
		GL_STATIC_DRAW);
}

void renderScene(void) {
	srand(1);
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// set the camera
	glLoadIdentity();
	px = r * cos(beta) * sin(alpha), py = r * sin(beta), pz = r * cos(beta) * cos(alpha);
	gluLookAt(px, py, pz, 0, 0, 0, 0, 1, 0);

	// put the geometric transformations here

	// put drawing instructions here
	drawAxis();
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColor3f(0.5, 0.5, 0.6);
	for (int i = 0; i < figures.size(); i++) {
		glPushMatrix();
		glMultMatrixf(figures[i].m);
		glDrawArrays(GL_TRIANGLES, figures[i].beg, figures[i].count);
		glPopMatrix();
	}
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
		alpha -= ALPHA_JUMP;
	}
	if (mouse_x < mouse_x_prev) {
		alpha += ALPHA_JUMP;
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
	//glutTimerFunc(0, timer, 0);

	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}
