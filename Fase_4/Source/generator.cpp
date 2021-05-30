#include <iostream>
#include <string>
#include <math.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include  <bits/stdc++.h>

#define _USE_MATH_DEFINES
#define PLANE_WIDTH 4

using namespace std;

/*---------------Functions that calculates vertices---------------*/
string gen_Plane(float width){
    width /= 2;
    stringstream ss;

    ss << width << " " << 0 << " " << width << "\n";
    ss << width << " " << 0 << " " << -width << "\n";
    ss << -width << " " << 0 << " " << -width << "\n";

    ss << width << " " << 0 << " " << width << "\n";
    ss << -width << " " << 0 << " " << -width << "\n";
    ss << -width << " " << 0 << " " << width << "\n";

    ss << width << " " << 0 << " " << -width << "\n";
    ss << width << " " << 0 << " " << width << "\n";
    ss << -width << " " << 0 << " " << -width << "\n";

    ss << -width << " " << 0 << " " << -width << "\n";
    ss << width << " " << 0 << " " << width << "\n";
    ss << -width << " " << 0 << " " << width << "\n";

    return ss.str();
}

string preparePlane(float width){
  width /= 2;
  stringstream ss;

  ss << width << " " << 0 << " " << width << " "
  << 0 << " " << 1 << " " << 0 << " "
  << 1 << " " << 0 << "\n";
  ss << width << " " << 0 << " " << -width << " "
  << 0 << " " << 1 << " " << 0 << " "
  << 1 << " " << 1 << "\n";
  ss << -width << " " << 0 << " " << -width << " "
  << 0 << " " << 1 << " " << 0 << " "
  << 0 << " " << 1 << "\n";

  ss << width << " " << 0 << " " << width << " "
  << 0 << " " << 1 << " " << 0 << " "
  << 1 << " " << 0 << "\n";
  ss << -width << " " << 0 << " " << -width << " "
  << 0 << " " << 1 << " " << 0 << " "
  << 0 << " " << 1 << "\n";
  ss << -width << " " << 0 << " " << width << " "
  << 0 << " " << 1 << " " << 0 << " "
  << 0 << " " << 0 << "\n";

  ss << width << " " << 0 << " " << -width << " "
  << 0 << " " << -1 << " " << 0 << " "
  << 1 << " " << 1 << "\n";
  ss << width << " " << 0 << " " << width << " "
  << 0 << " " << -1 << " " << 0 << " "
  << 1 << " " << 0 << "\n";
  ss << -width << " " << 0 << " " << -width << " "
  << 0 << " " << -1 << " " << 0 << " "
  << 0 << " " << 1 << "\n";

  ss << -width << " " << 0 << " " << -width << " "
  << 0 << " " << -1 << " " << 0 << " "
  << 0 << " " << 1 << "\n";
  ss << width << " " << 0 << " " << width << " "
  << 0 << " " << -1 << " " << 0 << " "
  << 1 << " " << 0 << "\n";
  ss << -width << " " << 0 << " " << width << " "
  << 0 << " " << -1 << " " << 0 << " "
  << 0 << " " << 0 << "\n";

  return ss.str();
}

string gen_Box(float x,float y,float z){
    x /= 2;
    y /= 2;
    z /= 2;
    stringstream ss;

    ss << x << " " << y << " " << z << "\n";
    ss << x << " " << y << " " << -z << "\n";
    ss << -x << " " << y << " " << -z << "\n";

    ss << x << " " << y << " " << z << "\n";
    ss << -x << " " << y << " " << -z << "\n";
    ss << -x << " " << y << " " << z << "\n";

    ss << x << " " << -y << " " << -z << "\n";
    ss << x << " " << -y << " " << z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";

    ss << -x << " " << -y << " " << -z << "\n";
    ss << x << " " << -y << " " << z << "\n";
    ss << -x << " " << -y << " " << z << "\n";

    ss << x << " " << -y << " " << -z << "\n";
    ss << x << " " << y << " " << -z << "\n";
    ss << x << " " << y << " " << z << "\n";

    ss << x << " " << -y << " " << -z << "\n";
    ss << x << " " << y << " " << z << "\n";
    ss << x << " " << -y << " " << z << "\n";

    ss << -x << " " << y << " " << -z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";
    ss << -x << " " << y << " " << z << "\n";

    ss << -x << " " << y << " " << z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";
    ss << -x << " " << -y << " " << z << "\n";

    ss << x << " " << -y << " " << z << "\n";
    ss << x << " " << y << " " << z << "\n";
    ss << -x << " " << y << " " << z << "\n";

    ss << x << " " << -y << " " << z << "\n";
    ss << -x << " " << y << " " << z << "\n";
    ss << -x << " " << -y << " " << z << "\n";

    ss << x << " " << y << " " << -z << "\n";
    ss << x << " " << -y << " " << -z << "\n";
    ss << -x << " " << y << " " << -z << "\n";

    ss << -x << " " << y << " " << -z << "\n";
    ss << x << " " << -y << " " << -z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";

    return ss.str();

}

string prepareBox(float x,float y,float z){
    x /= 2;
    y /= 2;
    z /= 2;
    stringstream ss;

    ss << x << " " << y << " " << z << " "
    << 0 << " " << 1 << " " << 0 << " "
    << 0.3 << " " << 0.5 << "\n";
    ss << x << " " << y << " " << -z << " "
    << 0 << " " << 1 << " " << 0 << " "
    << 0.3 << " " << 1 << "\n";
    ss << -x << " " << y << " " << -z << " "
    << 0 << " " << 1 << " " << 0 << " "
    << 0 << " " << 1 << "\n";

    ss << x << " " << y << " " << z << " "
    << 0 << " " << 1 << " " << 0 << " "
    << 0.3 << " " << 0.5 << "\n";
    ss << -x << " " << y << " " << -z << " "
    << 0 << " " << 1 << " " << 0 << " "
    << 0 << " " << 1 << "\n";
    ss << -x << " " << y << " " << z << " "
    << 0 << " " << 1 << " " << 0 << " "
    << 0 << " " << 0.5 << "\n";

    ss << x << " " << -y << " " << -z << " "
    << 0 << " " << -1 << " " << 0 << " "
    << 1 << " " << 0.5 << "\n";
    ss << x << " " << -y << " " << z << " "
    << 0 << " " << -1 << " " << 0 << " "
    << 1 << " " << 1 << "\n";
    ss << -x << " " << -y << " " << -z << " "
    << 0 << " " << -1 << " " << 0 << " "
    << 0.6 << " " << 0.5 << "\n";

    ss << -x << " " << -y << " " << -z << " "
    << 0 << " " << -1 << " " << 0 << " "
    << 0.6 << " " << 0.5 << "\n";
    ss << x << " " << -y << " " << z << " "
    << 0 << " " << -1 << " " << 0 << " "
    << 1 << " " << 1 << "\n";
    ss << -x << " " << -y << " " << z << " "
    << 0 << " " << -1 << " " << 0 << " "
    << 0.6 << " " << 1 << "\n";

    ss << x << " " << -y << " " << -z << " "
    << 1 << " " << 0 << " " << 0 << " "
    << 0.6 << " " << 0.5 << "\n";
    ss << x << " " << y << " " << -z << " "
    << 1 << " " << 0 << " " << 0 << " "
    << 0.6 << " " << 1 << "\n";
    ss << x << " " << y << " " << z << " "
    << 1 << " " << 0 << " " << 0 << " "
    << 0.3 << " " << 1 << "\n";

    ss << x << " " << -y << " " << -z << " "
    << 1 << " " << 0 << " " << 0 << " "
    << 0.6 << " " << 0.5 << "\n";
    ss << x << " " << y << " " << z << " "
    << 1 << " " << 0 << " " << 0 << " "
    << 0.3 << " " << 1 << "\n";
    ss << x << " " << -y << " " << z << " "
    << 1 << " " << 0 << " " << 0 << " "
    << 0.3 << " " << 0.5 << "\n";

    ss << -x << " " << y << " " << -z << " "
    << -1 << " " << 0 << " " << 0 << " "
    << 0 << " " << 0.5 << "\n";
    ss << -x << " " << -y << " " << -z << " "
    << -1 << " " << 0 << " " << 0 << " "
    << 0 << " " << 0 << "\n";
    ss << -x << " " << y << " " << z << " "
    << -1 << " " << 0 << " " << 0 << " "
    << 0.3 << " " << 0.5 << "\n";

    ss << -x << " " << y << " " << z << " "
    << -1 << " " << 0 << " " << 0 << " "
    << 0.3 << " " << 0.5 << "\n";
    ss << -x << " " << -y << " " << -z << " "
    << -1 << " " << 0 << " " << 0 << " "
    << 0 << " " << 0 << "\n";
    ss << -x << " " << -y << " " << z << " "
    << -1 << " " << 0 << " " << 0 << " "
    << 0.3 << " " << 0 << "\n";

    ss << x << " " << -y << " " << z << " "
    << 0 << " " << 0 << " " << 1 << " "
    << 0.6 << " " << 0 << "\n";
    ss << x << " " << y << " " << z << " "
    << 0 << " " << 0 << " " << 1 << " "
    << 0.6 << " " << 0.5 << "\n";
    ss << -x << " " << y << " " << z << " "
    << 0 << " " << 0 << " " << 1 << " "
    << 0.3 << " " << 0.5 << "\n";

    ss << x << " " << -y << " " << z << " "
    << 0 << " " << 0 << " " << 1 << " "
    << 0.6 << " " << 0 << "\n";
    ss << -x << " " << y << " " << z << " "
    << 0 << " " << 0 << " " << 1 << " "
    << 0.3 << " " << 0.5 << "\n";
    ss << -x << " " << -y << " " << z << " "
    << 0 << " " << 0 << " " << 1 << " "
    << 0.3 << " " << 0 << "\n";

    ss << x << " " << y << " " << -z << " "
    << 0 << " " << 0 << " " << -1 << " "
    << 0.6 << " " << 0.5 << "\n";
    ss << x << " " << -y << " " << -z << " "
    << 0 << " " << 0 << " " << -1 << " "
    << 0.6 << " " << 0 << "\n";
    ss << -x << " " << y << " " << -z << " "
    << 0 << " " << 0 << " " << -1 << " "
    << 1 << " " << 0.5 << "\n";

    ss << -x << " " << y << " " << -z << " "
    << 0 << " " << 0 << " " << -1 << " "
    << 1 << " " << 0.5 << "\n";
    ss << x << " " << -y << " " << -z << " "
    << 0 << " " << 0 << " " << -1 << " "
    << 0.6 << " " << 0 << "\n";
    ss << -x << " " << -y << " " << -z << " "
    << 0 << " " << 0 << " " << -1 << " "
    << 1 << " " << 0 << "\n";

    return ss.str();

}


string gen_Sphere(float radius,int slices,int stacks){
    float a_interval = 2 * M_PI / slices;
    float b_interval = M_PI / stacks;
    float next_a, next_b;
    stringstream ss;

    for (float a = 0; a < 2 * M_PI; a +=  a_interval) {
        for (float b = -M_PI/2; b < M_PI/2; b += b_interval) {
            next_a = a + a_interval;
            next_b = b + b_interval;
            if (next_a > 2 * M_PI) {
                next_a = 2 * M_PI;
            }
            if (next_b > M_PI / 2) {
                next_b = M_PI / 2;
            }
            ss << radius * cos(next_b) * sin(next_a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(next_a) << "\n";
            ss << radius * cos(next_b) * sin(a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(a) << "\n";
            ss << radius * cos(b) * sin(next_a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(next_a) << "\n";

            ss << radius * cos(b) * sin(next_a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(next_a) << "\n";
            ss << radius * cos(next_b) * sin(a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(a) << "\n";
            ss << radius * cos(b) * sin(a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(a) << "\n";

        }
    }
    return ss.str();
}

string prepareSphere(float radius, int slices, int stacks) {

	int vertex = 0;

	float a_interval = 2 * M_PI / slices;
	float b_interval = M_PI / stacks;
	float next_a, next_b;

	float pi_div_2 = M_PI / 2;
	float pi_mul_2 = 2 * M_PI;

  stringstream ss;

	for (float a = 0; a < 2 * M_PI; a += a_interval) {
		for (float b = -M_PI / 2; b < M_PI / 2; b += b_interval) {
			next_a = a + a_interval;
			next_b = b + b_interval;
			if (next_a > 2 * M_PI) {
				next_a = 2 * M_PI;
			}
			if (next_b > M_PI / 2) {
				next_b = M_PI / 2;
			}

      ss << radius * cos(next_b) * sin(next_a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(next_a) << " "
      << (radius * cos(next_b) * sin(next_a))/radius << " " << (radius * sin(next_b))/radius << " " << (radius * cos(next_b) * cos(next_a))/radius << " "
      << next_a / pi_mul_2 << " " << (next_b + pi_div_2)/ M_PI << "\n";

      ss << radius * cos(next_b) * sin(a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(a) << " "
      << (radius * cos(next_b) * sin(next_a))/radius << " " <<  (radius * sin(next_b))/radius << " " << (radius * cos(next_b) * cos(next_a))/radius << " "
      << a / pi_mul_2 << " " << (next_b + pi_div_2)/ M_PI << "\n";

      ss << radius * cos(b) * sin(next_a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(next_a) << " "
      << (radius * cos(next_b) * sin(next_a))/radius << " " << (radius * sin(next_b))/radius << " " << (radius * cos(next_b) * cos(next_a))/radius << " "
      << next_a / pi_mul_2 << " " << (b + pi_div_2) / M_PI << "\n";

			ss << radius * cos(b) * sin(next_a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(next_a) << " "
			<< (radius * cos(next_b) * sin(next_a))/radius << " " <<  (radius * sin(next_b))/radius << " " << (radius * cos(next_b) * cos(next_a))/radius << " "
      << next_a / pi_mul_2 << " " << (b + pi_div_2)/ M_PI << "\n";

			ss << radius * cos(next_b) * sin(a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(a) << " "
			<< (radius * cos(next_b) * sin(next_a))/radius << " " <<  (radius * sin(next_b))/radius << " " << (radius * cos(next_b) * cos(next_a))/radius << " "
			<< a / pi_mul_2 << " " << (next_b + pi_div_2)/ M_PI << "\n";

			ss << radius * cos(b) * sin(a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(a) << " "
			<< (radius * cos(next_b) * sin(next_a))/radius << " " <<  (radius * sin(next_b))/radius << " " << (radius * cos(next_b) * cos(next_a))/radius << " "
			<< a / pi_mul_2 << " " << (b + pi_div_2) / M_PI << "\n";

			vertex += 6;
		}
	}
  return ss.str();
}



string gen_Cone(float radius,float height,int slices,int stacks){
    float next_a;
    float next_h;
    float interval = 2 * M_PI / slices;
    float stack_height = height / stacks;
    stringstream ss;

    for (float a = 0; a < 2 * M_PI; a += interval) {
        next_a = a + interval;
        if (next_a > 2 * M_PI) {
            next_a = 2 * M_PI;
        }
        ss << 0.0f << " " << 0 << " " << 0.0f << "\n";
        ss << radius * sin(next_a) << " " << 0 << " " << radius * cos(next_a) << "\n";
        ss << radius * sin(a) << " " << 0 << " " << radius * cos(a) << "\n";

        for (float h = 0; h < height; h += stack_height) {
            next_h = h + stack_height;
            if (next_h > height) {
                next_h = height;
            }
            ss << radius * sin(next_a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(next_a) * ((height - next_h) / height) << "\n";
            ss << radius * sin(a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(a) * ((height - next_h) / height) << "\n";
            ss << radius * sin(next_a) * ((height - h) / height) << " " << h << " " << radius * cos(next_a) * ((height - h) / height) << "\n";

            ss << radius * sin(a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(a) * ((height - next_h) / height) << "\n";
            ss << radius * sin(a) * ((height - h) / height) << " " << h << " " << radius * cos(a) * ((height - h) / height) << "\n";
            ss << radius * sin(next_a) * ((height - h) / height) << " " << h << " " << radius * cos(next_a) * ((height - h) / height) << "\n";

        }
    }

    return ss.str();
}


string prepareCone(float radius,float height,int slices,int stacks){
    float next_a;
    float next_h;
    float interval = 2 * M_PI / slices;
    float stack_height = height / stacks;
    stringstream ss;

    for (float a = 0; a < 2 * M_PI; a += interval) {
        next_a = a + interval;
        if (next_a > 2 * M_PI) {
            next_a = 2 * M_PI;
        }
        ss << 0.0f << " " << 0 << " " << 0.0f << " "
        << 0 << " " << -1 << " " << 0 << " "
        << 1/6 << " " << 1/6 << "\n";
        ss << radius * sin(next_a) << " " << 0 << " " << radius * cos(next_a) << " "
        << 0 << " " << -1 << " " << 0 << " "
        << 1/6 * sin(next_a) << " " << 1/6 * cos(next_a) << "\n";
        ss << radius * sin(a) << " " << 0 << " " << radius * cos(a) << " "
        << 0 << " " << -1 << " " << 0 << " "
        << 1/6 * sin(a) << " " << 1/6 * cos(a) << "\n";

        for (float h = 0; h < height; h += stack_height) {
            next_h = h + stack_height;
            if (next_h > height) {
                next_h = height;
            }
            ss << radius * sin(next_a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(next_a) * ((height - next_h) / height) << " "
            << cos(atan(radius/height))*cos(next_a) << " " << sin(atan(radius/height)) << " " << cos(atan(radius/height))*sin(next_a) << " "
            << sin(next_a) * ((height - next_h) / height) * cos(next_a)  << " " << next_h/height << "\n";
            ss << radius * sin(a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(a) * ((height - next_h) / height) << " "
            << cos(atan(radius/height))*cos(a) << " " << sin(atan(radius/height)) << " " << cos(atan(radius/height))*sin(a) << " "
            << sin(a) * ((height - next_h) / height) * cos(a)  << " " << next_h/height << "\n";
            ss << radius * sin(next_a) * ((height - h) / height) << " " << h << " " << radius * cos(next_a) * ((height - h) / height) << " "
            << cos(atan(radius/height))*cos(next_a) << " " << sin(atan(radius/height)) << " " << cos(atan(radius/height))*sin(next_a) << " "
            << sin(next_a) * ((height - next_h) / height) * cos(next_a)  << " " << next_h/height << "\n";

            ss << radius * sin(a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(a) * ((height - next_h) / height) << " "
            << cos(atan(radius/height))*cos(a) << " " << sin(atan(radius/height)) << " " << cos(atan(radius/height))*sin(a) << " "
            << sin(a) * ((height - next_h) / height) * cos(a)  << " " << next_h/height << "\n";
            ss << radius * sin(a) * ((height - h) / height) << " " << h << " " << radius * cos(a) * ((height - h) / height) << " "
            << cos(atan(radius/height))*cos(a) << " " << sin(atan(radius/height)) << " " << cos(atan(radius/height))*sin(a) << " "
            << sin(a) * ((height - next_h) / height) * cos(a)  << " " << next_h/height << "\n";
            ss << radius * sin(next_a) * ((height - h) / height) << " " << h << " " << radius * cos(next_a) * ((height - h) / height) << " "
            << cos(atan(radius/height))*cos(next_a) << " " << sin(atan(radius/height)) << " " << cos(atan(radius/height))*sin(next_a) << " "
            << sin(next_a) * ((height - next_h) / height) * cos(next_a)  << " " << next_h/height << "\n";  

        }
    }

    return ss.str();
}



/*---------Functions that calculate model based on Bezier---------*/

vector <float> read_Bezier(string patch, int tesselation){
    string buffer;
    string buffer2;
    ifstream myFile;
    myFile.open(patch,ios::in|ios::out);
    getline(myFile, buffer);
    for (int p = stoi(buffer); p != 0; p -= 1) {
      getline(myFile, buffer);
    }
    getline(myFile, buffer);
    int p = stoi(buffer);
    vector<float> control;
    for(int j = 0; j < p; j+=1){
      for (int i = 0; i < 2; i += 1) {
          getline(myFile, buffer, ',');
          control.push_back(stof(buffer));
      }
      getline(myFile, buffer);
      control.push_back(stof(buffer));
    }
    myFile.close();
    return control;
}

tuple <float,float,float> getBezierPoint(float u, float v, vector<float> x){
  float bernsteinU[4] = { powf(1-u, 3), 3 * u * powf(1-u, 2), 3 * powf(u, 2) * (1-u), powf(u, 3) };
  float bernsteinV[4] = { powf(1-v, 3), 3 * v * powf(1-v, 2), 3 * powf(v, 2) * (1-v), powf(v, 3) };
  tuple <float,float,float> ponto;
  get<0>(ponto) = 0.0;
  get<1>(ponto) = 0.0;
  get<2>(ponto) = 0.0;
  for (int j=0; j<4; j++){
      for (int i=0; i<4; i++) {
        int indexCP = j*12+i*3;
          get<0>(ponto) = get<0>(ponto) + x[indexCP] * bernsteinU[j] * bernsteinV[i];
          get<1>(ponto) = get<1>(ponto) + x[indexCP + 1] * bernsteinU[j] * bernsteinV[i];
          get<2>(ponto) = get<2>(ponto) + x[indexCP + 2] * bernsteinU[j] * bernsteinV[i];
      }
  }
  return ponto;
}

string gen_Bezier(string patch, int tesselation){
  vector <float> control = read_Bezier(patch, tesselation);
  stringstream res;
  ifstream myFile;
  string buffer;
  vector <tuple <float,float,float>> pontos ;
  vector <float> x;
  myFile.open(patch,ios::in|ios::out);
  getline(myFile, buffer);
  int npatches = stoi(buffer);
  for(int i=0;i<npatches;i+=1){
    for(int p=0;p<15;p+=1){
      getline(myFile, buffer, ',');
      x.push_back(control[stoi(buffer)*3]);
      x.push_back(control[stoi(buffer)*3+1]);
      x.push_back(control[stoi(buffer)*3+2]);
    }
    getline(myFile,buffer);
    x.push_back(control[stoi(buffer)*3]);
    x.push_back(control[stoi(buffer)*3+1]);
    x.push_back(control[stoi(buffer)*3+2]);
    for (int tv=0; tv<tesselation; tv++) {
           float v = (float) tv/tesselation;

           for (int tu = 0; tu < tesselation; tu++) {
               float u = (float) tu/tesselation;
                // triângulo superior
                pontos.push_back(getBezierPoint((u + (1.0f/tesselation)), (v + (1.0f/tesselation)),x));
                pontos.push_back(getBezierPoint(u, (v + (1.0f/tesselation)),x));
                pontos.push_back(getBezierPoint(u, v,x));
                // triângulo inferior
                pontos.push_back(getBezierPoint(u, v,x));
                pontos.push_back(getBezierPoint((u + (1.0f/tesselation)), v,x));
                pontos.push_back(getBezierPoint((u + (1.0f/tesselation)), (v + (1.0f/tesselation)),x));
                for(int k=0;k<6;k++){
                  res << get<0>(pontos[k]) << " " << get<1>(pontos[k]) << " " << get<2>(pontos[k]) << "\n";
                }
                pontos.clear();
           }
       }
       x.clear();
    }
    myFile.close();
    return res.str();
}

/*-------------------Construction file function-------------------*/
void constructF(string res ,string fname){
    //file creation
    ofstream fileWrite(fname, ios::out | ios::trunc);
    fileWrite.write((char*) res.data(), res.size() * sizeof(char));
    fileWrite.close();
}

/*------------------Main that will iniciate all------------------*/
int main(int argc, char* argv[]){
    string fname, fig = argv[1], res; // res: armazenará os valores dos vertices
    if (argc == 1) {
        cout << "\nError! No arguments passed.\n";
        exit(1);
    }
    if(fig == "plane"){
       if(argc < 3){
                cout << "\nError! Invalid number of arguments.\n";
                exit(1);
       } fname = argv[2];
       res = preparePlane(PLANE_WIDTH);
    } else if(fig == "box"){
              if(argc < 6){
                 cout << "\nError! Invalid number of arguments.\n";
                 exit(1);
              }
              float pBx = stof(argv[2]);
              float pBy = stof(argv[3]);
              float pBz = stof(argv[4]);
              fname = argv[5];
              res = prepareBox(pBx,pBy,pBz);
    } else if(fig == "sphere"){
              if(argc < 6){
                 cout << "\nError! Invalid number of arguments.\n";
                 exit(1);
              }
              float radius = stof(argv[2]);
              int slices = atoi(argv[3]);
              int stacks = atoi(argv[4]);
              fname = argv[5];
              res = prepareSphere(radius,slices,stacks);
    } else if(fig == "cone"){
              if(argc < 7){
              cout << "\nError! Invalid number of arguments.\n";
              exit(1);
              }
              float radius = stof(argv[2]);
              float height = stof(argv[3]);
              int slices = atoi(argv[4]);
              int stacks = atoi(argv[5]);
              fname = argv[6];
              res = prepareCone(radius,height,slices,stacks);
    } else if(fig == "bezier"){
              if(argc < 5){
                 cout << "\nError! Invalid number of arguments.\n";
                 exit(1);
              }
              string patch = argv[2];
              int tesselation = atoi(argv[3]);
              fname = argv[4];
              res = gen_Bezier(patch,tesselation);
    } else if(fig == "info"){
      cout << "\nplane: generator plane <outfile>\n";
            cout << "\nbox: generator box <X> <Y> <Z> <outfile>\n";
            cout << "\nsphere: generator sphere <radius> <slices> <stacks> <outfile>\n";
            cout << "\ncone: generator cone <radius> <height> <slices> <stacks> <outfile>.\n";
            cout << "\nbezier: generator bezier <in-file> <tessellation-level> <outfile>.\n";
    } else {
      cout << "\nError! That feature doesn't exist.\n";
    }
        // Fazer a criação do ficheiro:
        constructF(res,fname);
        return 0;
}
