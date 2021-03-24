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

#define INIT_R 7
#define INIT_ALPHA M_PI/4
#define INIT_BETA M_PI/4
#define ALPHA_JUMP 0.05
#define BETA_JUMP 0.04

float mouse_x_init, mouse_y_init;
float mouse_x_prev, mouse_y_prev;

float px, py, pz, alpha, beta, r;

//For Window
int window;

//For FPS counter
int timebase;
float frame;

GLuint vertices, verticeCount;

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
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glEnd();
}

void draw() {	
	XMLDocument doc;
	XMLError load = doc.LoadFile( "xmlconf.xml" );
        if (load != XML_SUCCESS){
	        cout << "Error in XML file\n"<<endl;
		return;
        }
	XMLNode* pRoot = doc.FirstChildElement("scene");
	if (pRoot == nullptr) return;

	string ch;
	ifstream file;
	vector<float> v;
	int size;
	const char* title = doc.FirstChildElement("scene")->FirstChildElement("model")->Attribute("file");

	file.open(title, ios::in);
	if (!file) {
		cout << "No such file" << endl;
		return;
	}
	file >> ch;
	size = stoi(ch);

	for(int i = 0; !file.eof(); i++) {
		file >> ch;
		v.push_back(stof(ch));
		if(file.eof()) break;
	}
	file.close();
	
	verticeCount = size*3;
	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(
		GL_ARRAY_BUFFER, //tipo do buffer, só é relevante na altura do desenho
		sizeof(float) * v.size(), //tamanho do vector em bytes
		v.data(), //os dados do array associado ao vector
		GL_STATIC_DRAW); //indicativo da utilização (estático e para desenho)
}


void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	px = r * cos(beta) * sin(alpha), py = r * sin(beta), pz = r * cos(beta) * cos(alpha);
	gluLookAt(px, py, pz, 0, 0, 0, 0, 1, 0);

	// put the geometric transformations here

	// put drawing instructions here

	drawAxis();
		
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, verticeCount);

	//FPS counter
	frame++;
	int final_time = glutGet(GLUT_ELAPSED_TIME);

	if (final_time - timebase > 1000) {
		int fps = frame * 1000.0f / (final_time - timebase);
		char title[(((sizeof fps) * CHAR_BIT) + 2) / 3 + 2];
		sprintf(title, "FPS: %d", fps);
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
		r -= 0.2;
		break;
	}
	case 's': {
		r += 0.2;
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
	draw();
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
	
	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}