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
#define CATMULL_MAX 10

//Explorer Mode Camera
float mouse_x_init, mouse_y_init;
float mouse_x_prev, mouse_y_prev;
float px, vx, py, vy, pz, vz, alpha, beta, r;

//FPS Mode Camera
float dx, dy, dz;
float radium = 17.0f;
float speed = 0.05f;
float rotateSpeed = 0.0008f;
//Activations
bool fpsOn = false;
bool first = true;
bool front = false;
bool back = false;
bool left_fps = false;
bool right_fps = false;

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
	float translation_time;
	float catmull_points[10][3];
	int catmull_points_size;	
	float rotation_time;
	float rotation_coordinates[3];
	int trans_or_rot;
	GLfloat m[16];
};
vector<FIGURE> figures;

float translation_time = 0;
float catmull_points[10][3];
int catmull_points_size = 0; //number of points
float rotation_time = 0;
float rotation_coordinates[3];
int trans_or_rot; 

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


/*
 * Carregamento de uma imagem para uma textura RGB na placa gráfica
 */
int loadTexture(string s) {
    unsigned int t, tw, th;
    unsigned char *texData;
    unsigned int texID;

    // Iniciar o DevIL
    ilInit();

    // Colocar a origem da textura no canto inferior esquerdo
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    // Carregar a textura para memória
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)s.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    //Gerar a textura para a placa gráfica
    glGenTextures(1,&texID);

    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //Upload dos dados da imagem
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tw, th, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
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
		translation_time = 0;
		catmull_points_size = 0;
		rotation_time = 0;
		trans_or_rot = 0;
	}
	else if (strcmp(value.c_str(), "translate") == 0) {
		if (node_elem->DoubleAttribute("time") > 0) {
			translation_time = node_elem->DoubleAttribute("time");
			catmull_points_size = 0;
			for (XMLElement* children = node_elem->FirstChildElement(); children != nullptr && catmull_points_size < CATMULL_MAX; children = children->NextSiblingElement()) {
				x = children->DoubleAttribute("X");
				y = children->DoubleAttribute("Y");
				z = children->DoubleAttribute("Z");
				catmull_points[catmull_points_size][0] = x;
				catmull_points[catmull_points_size][1] = y;
				catmull_points[catmull_points_size][2] = z;
				catmull_points_size++;
			}
			trans_or_rot = 2;
		}
		else {
			x = node_elem->DoubleAttribute("X");
			y = node_elem->DoubleAttribute("Y");
			z = node_elem->DoubleAttribute("Z");
			glTranslatef(x, y, z);
		}
	}
	else if (strcmp(value.c_str(), "rotate") == 0) {		
		if (node_elem->DoubleAttribute("time") > 0) {
			rotation_time = node_elem->DoubleAttribute("time");
			x = node_elem->DoubleAttribute("axisX");
			y = node_elem->DoubleAttribute("axisY");
			z = node_elem->DoubleAttribute("axisZ");
			rotation_coordinates[0] = x;
			rotation_coordinates[1] = y;
			rotation_coordinates[2] = z;
			trans_or_rot = 1;
		}
		else{
			angle = node_elem->DoubleAttribute("angle");
			x = node_elem->DoubleAttribute("axisX");
			y = node_elem->DoubleAttribute("axisY");
			z = node_elem->DoubleAttribute("axisZ");
			glRotatef(angle, x, y, z);
		}
	}
	else if (strcmp(value.c_str(), "scale") == 0) {
		x = node_elem->DoubleAttribute("X");
		y = node_elem->DoubleAttribute("Y");
		z = node_elem->DoubleAttribute("Z");
		glScalef(x, y, z);
	}
	else if (strcmp(value.c_str(), "models") == 0) {
		for (XMLElement* children = node_elem->FirstChildElement(); children != nullptr; children = children->NextSiblingElement()) {
			/*
			if(modelsNode->Attribute("texture")){
                           string textureName = models_path + modelsNode->Attribute("texture");
                           t.figure.texture = loadTexture(textureName);
			*/	
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
			
			//catmull points
			new_figure.catmull_points_size = catmull_points_size;
			for (int i =0; i < catmull_points_size; i++) {
				for (int j = 0; j < 3; j++) {
					new_figure.catmull_points[i][j] = catmull_points[i][j];
				}				
			}
			new_figure.translation_time = translation_time;
			//rotation
			new_figure.rotation_time = rotation_time;
			new_figure.rotation_coordinates[0] = rotation_coordinates[0];
			new_figure.rotation_coordinates[1] = rotation_coordinates[1];
			new_figure.rotation_coordinates[2] = rotation_coordinates[2];
			new_figure.trans_or_rot = trans_or_rot;
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

void multMatrixVector(float* m, float* v, float* res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void multVectorVector(float* m, float* v, float* res) {
	res[0] = 0;
	for (int k = 0; k < 4; k++) {
		res[0] += v[k] * m[k];
	}
}

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {
	float m[16] = { -0.5f,  1.5f, -1.5f,  0.5f,
				     1.0f, -2.5f,  2.0f, -0.5f,
			    	-0.5f,  0.0f,  0.5f,  0.0f,
				     0.0f,  1.0f,  0.0f,  0.0f };
	float p_t[4][4] = { { p0[0],  p1[0], p2[0],  p3[0]},
						{ p0[1],  p1[1], p2[1],  p3[1]},
						{ p0[2],  p1[2], p2[2],  p3[2]},
	};
	float a[4];
	float t_vector[4] = { pow(t,3), pow(t,2), t, 1 };
	float t_vector_deriv[4] = { 3 * pow(t,2), 2 * t, 1, 0 };
	float res[16];
	for (int i = 0; i < 3; i++) {
		multMatrixVector(m, p_t[i], a);
		multVectorVector(t_vector, a, res);
		pos[i] = res[0];
		multVectorVector(t_vector_deriv, a, res);
		deriv[i] = res[0];
	}
}

void getGlobalCatmullRomPoint(float gt, float p[10][3], int p_size, float* pos, float* deriv) {

	float t = gt * (p_size ); // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + (p_size) - 1) % (p_size);
	indices[1] = (indices[0] + 1) % (p_size);
	indices[2] = (indices[1] + 1) % (p_size);
	indices[3] = (indices[2] + 1) % (p_size);

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}


void renderScene(void) {
	srand(1);
	float pos[5];
	float deriv[5];
	float angle, gt;
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// set the camera
	glLoadIdentity();
	//Explorer camera
	if (fpsOn == false) {
		if (first == true) {
			px = r * cos(beta) * sin(alpha); py = r * sin(beta); pz = r * cos(beta) * cos(alpha);
			vx = 0; vy = 0; vz = 0;
		}
		else if(first == false){
			vx = cos(beta) * sin(alpha), vy = sin(beta), vz = cos(beta) * cos(alpha);
			px = r * vx; py = r * vy; pz = r * vz;
		}
	}
	else {
		//FPS Camera
		dx = cos(beta) * sin(alpha), dy = sin(beta), dz = cos(beta) * cos(alpha);

		if (front == true) {
			px -= (dx * speed);
			py -= (dy * speed);
			pz -= (dz * speed);
		}
		if (back == true) {
			px += (dx * speed);
			py += (dy * speed);
			pz += (dz * speed);
		}
		if (right_fps == true) {
			px += (dz * speed);
			pz -= (dx * speed);
		}
		if (left_fps == true) {
			px -= (dz * speed);
			pz += (dx * speed);
		}
		vx = px - dx, vy = py - dy, vz = pz - dz;
	}
	gluLookAt(px, py, pz, vx, vy, vz, 0.0f, 1.0f, 0.0f);
	// put drawing instructions here
	drawAxis();
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColor3f(0.5, 0.5, 0.6);
	for (int i = 0; i < figures.size(); i++) {
		glPushMatrix();	
		if (figures[i].trans_or_rot == 1 || figures[i].trans_or_rot == 0){
			if (figures[i].catmull_points_size >= 4 && figures[i].translation_time > 0) {
				gt = fmod(glutGet(GLUT_ELAPSED_TIME), (float)(figures[i].translation_time * 1000)) / (figures[i].translation_time * 1000);
				getGlobalCatmullRomPoint(gt, figures[i].catmull_points, figures[i].catmull_points_size, pos, deriv);
				glTranslatef(pos[0], pos[1], pos[2]);
			}
			if (figures[i].rotation_time > 0) {
				angle = 360 * (fmod(glutGet(GLUT_ELAPSED_TIME), (float)(figures[i].rotation_time * 1000)) / (figures[i].rotation_time * 1000));
				glRotatef(-angle, figures[i].rotation_coordinates[0], figures[i].rotation_coordinates[1], figures[i].rotation_coordinates[2]);
			}
		}
		else if (figures[i].trans_or_rot == 2) {			
			if (figures[i].rotation_time > 0) {
				angle = 360 * (fmod(glutGet(GLUT_ELAPSED_TIME), (float)(figures[i].rotation_time * 1000)) / (figures[i].rotation_time * 1000));
				glRotatef(-angle, figures[i].rotation_coordinates[0], figures[i].rotation_coordinates[1], figures[i].rotation_coordinates[2]);
			}
			if (figures[i].catmull_points_size >= 4 && figures[i].translation_time > 0) {
				gt = fmod(glutGet(GLUT_ELAPSED_TIME), (float)(figures[i].translation_time * 1000)) / (figures[i].translation_time * 1000);
				getGlobalCatmullRomPoint(gt, figures[i].catmull_points, figures[i].catmull_points_size, pos, deriv);
				glTranslatef(pos[0], pos[1], pos[2]);
			}
		}

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
	if (state == GLUT_DOWN) {
		mouse_x_init = mouse_x;
		mouse_y_init = mouse_y;
		mouse_x_prev = mouse_x_init;
		mouse_y_prev = mouse_y_init;
	}
}

void mouseM(int mouse_x, int mouse_y) {
	if (fpsOn == false) {
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
	}
	else {
		alpha += ((mouse_x_prev - mouse_x) * rotateSpeed);
		beta += ((mouse_y_prev - mouse_y) * rotateSpeed);
		dx = cos(beta) * cos(alpha);
		dy = sin(beta);
		dz = sin(alpha) * cos(beta);
	}
	mouse_x_prev = mouse_x;
	mouse_y_prev = mouse_y;
	glutPostRedisplay();
}

void onKeyDown(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_END: {
		glutDestroyWindow(window);
		exit(0);
	}
	case GLUT_KEY_RIGHT: {
		right_fps = true;
		break;
	}
	case GLUT_KEY_LEFT: {
		left_fps = true;
		break;
	}
	case GLUT_KEY_UP: {
		if (fpsOn == false) {
			r -= R_JUMP;
			break;
		}
		front = true;
		break;
	}
	case GLUT_KEY_DOWN: {
		if (fpsOn == false) {
			r += R_JUMP;
		}
		back = true;
		break;
	}
	case GLUT_KEY_HOME: {
		fpsOn = !fpsOn;
		first = false;
		//rx = px;
		//ry = py;
		//rz = pz;
		break;
	}
	default: {
		break;
	}
	}
	glutPostRedisplay();
}

void onKeyUp(int key, int x, int y) {
	if (GLUT_KEY_UP == key) {
		front = false;
	}
	if (GLUT_KEY_RIGHT == key) {
		right_fps = false;
	}
	if (GLUT_KEY_LEFT == key) {
		left_fps = false;
	}
	if (GLUT_KEY_DOWN == key) {
		back = false;
	}
}

//Lights
void initGL() {
    // alguns settings para OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glClearColor(0, 0, 0, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
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
	#ifndef __APPLE__
        glewInit();
        #endif
        initGL();
	
	//FPS Counter
	timebase = glutGet(GLUT_ELAPSED_TIME);

	// put here the registration of the keyboard callbacks
	glutSpecialFunc(onKeyDown);
	glutSpecialUpFunc(onKeyUp);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseM);

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
