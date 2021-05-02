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

    return "4\n" + ss.str();
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

    return "12\n" + ss.str();

}

string gen_Sphere(float radius,int slices,int stacks){
    float a_interval = 2 * M_PI / slices;
    float b_interval = M_PI / stacks;
    float next_a, next_b;
    int triangles = 0;
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

            triangles += 2;
        }
    }
    return to_string(triangles) + "\n" + ss.str();
}

string gen_Cone(float radius,float height,int slices,int stacks){
    float next_a;
    float next_h;
    float interval = 2 * M_PI / slices;
    float stack_height = height / stacks;
    int triangles = 0;
    stringstream ss;

    for (float a = 0; a < 2 * M_PI; a += interval) {
        next_a = a + interval;
        if (next_a > 2 * M_PI) {
            next_a = 2 * M_PI;
        }
        ss << 0.0f << " " << 0 << " " << 0.0f << "\n";
        ss << radius * sin(next_a) << " " << 0 << " " << radius * cos(next_a) << "\n";
        ss << radius * sin(a) << " " << 0 << " " << radius * cos(a) << "\n";

        triangles += 1;

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

            triangles += 2;
        }
    }

    return to_string(triangles) + "\n" + ss.str();
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
    p = stoi(buffer);
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

string points_Bezier(float u, float v, vector<float> control){
  stringstream res;
  float bernsteinU[4] = { powf(1-u, 3), 3 * u * powf(1-u, 2), 3 * powf(u, 2) * (1-u), powf(u, 3) };
  float bernsteinV[4] = { powf(1-v, 3), 3 * v * powf(1-v, 2), 3 * powf(v, 2) * (1-v), powf(v, 3) };
  tuple <float,float,float> ponto;
  for (int j=0; j<4; j++){
      for (int i=0; i<4; i++) {
        int indexCP = j*4+i;
          get<0>(ponto) = get<0>(ponto) + control[indexCP] * bernsteinU[j] * bernsteinV[i];
          get<1>(ponto) = get<1>(ponto) + control[indexCP + 1] * bernsteinU[j] * bernsteinV[i];
          get<2>(ponto) = get<2>(ponto) + control[indexCP + 2] * bernsteinU[j] * bernsteinV[i];
      }
      res << get<0>(ponto) << " " << get<1>(ponto) << " " << get<2>(ponto) << "\n";
  }
  return res.str();
}


string gen_Bezier(string patch, int tesselation){
  vector <float> control = read_Bezier(patch, tesselation);
  stringstream res;
  ifstream myFile;
  string buffer;
  tuple <float,float,float> point;
  vector <tuple<float,float,float>> x;
  myFile.open(patch,ios::in|ios::out);
  getline(myFile, buffer);
  int npatches = stoi(buffer);
  for(int i=0;i<npatches;i+=1){
    for(int p=0;p<16;p+=1){
      getline(myFile, buffer, ',');
      get<0>(point) = control[stoi(buffer)];
      get<1>(point) = control[stoi(buffer)+1];
      get<2>(point) = control[stoi(buffer)+2];
      x.push_back(point);
    }
    for (int tv=0; tv<tesselation; tv++) {
           float v = (float) tv/tesselation;

           for (int tu = 0; tu < tesselation; tu++) {
               float u = (float) tu/tesselation;

               // t sup
               res << points_Bezier((u + (1.0f/tesselation)), (v + (1.0f/tesselation)),control) << points_Bezier(u, (v + (1.0f/tesselation)),control) << points_Bezier(u, v,control);
               // t inf
               res << points_Bezier(u, v,control) << points_Bezier((u + (1.0f/tesselation)), v,control) << points_Bezier((u + (1.0f/tesselation)), (v + (1.0f/tesselation)),control);
           }
       }
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
       res = gen_Plane(PLANE_WIDTH);
    } else if(fig == "box"){
              if(argc < 6){
                 cout << "\nError! Invalid number of arguments.\n";
                 exit(1);
              }
              float pBx = stof(argv[2]);
              float pBy = stof(argv[3]);
              float pBz = stof(argv[4]);
              fname = argv[5];
              res = gen_Box(pBx,pBy,pBz);
    } else if(fig == "sphere"){
              if(argc < 6){
                 cout << "\nError! Invalid number of arguments.\n";
                 exit(1);
              }
              float radius = stof(argv[2]);
              int slices = atoi(argv[3]);
              int stacks = atoi(argv[4]);
              fname = argv[5];
              res = gen_Sphere(radius,slices,stacks);
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
              res = gen_Cone(radius,height,slices,stacks);
    } else if(fig == "bezier"){
              if(argc < 5){
                 cout << "\nError! Invalid number of arguments.\n";
                 exit(1);
              }
              string patch = argv[2];
              int tesselation = atoi(argv[3]);
              fname = argv[4];
              res = gen_Bezier(patch,tesselation);
    } else if(fig == "comet"){
              cout << "E uma esfera mas o luis diz que nao.\n";
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
