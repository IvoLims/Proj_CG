#include <iostream>
#include <string>
#include <math.h>
#include <sstream>
#include <fstream>

#define _USE_MATH_DEFINES
#define PI 3.14159265358979323846  /* pi */
#define PLANE_WIDTH 4

using namespace std;

/*---------------Functions that calculates vertices---------------*/
string gen_Plane(float width){
    width /= 2;
    stringstream ss;
    ss << ":Plane\n";

    ss << width << " " << 0 << " " << width << "\n";
    ss << width << " " << 0 << " " << -width << "\n";
    ss << -width << " " << 0 << " " << -width << "\n";

    ss << "-------------------------------------\n";

    ss << width << " " << 0 << " " << width << "\n";
    ss << -width << " " << 0 << " " << -width << "\n";
    ss << -width << " " << 0 << " " << width << "\n";

    ss << "-------------------------------------\n";

    ss << width << " " << 0 << " " << -width << "\n";
    ss << width << " " << 0 << " " << width << "\n";
    ss << -width << " " << 0 << " " << -width << "\n";

    ss << "-------------------------------------\n";

    ss << -width << " " << 0 << " " << -width << "\n";
    ss << width << " " << 0 << " " << width << "\n";
    ss << -width << " " << 0 << " " << width << "\n";

    ss << "-------------------------------------\n";

    return "4" + ss.str();
}

string gen_Box(float x,float y,float z){
    x /= 2;
    y /= 2;
    z /= 2;
    stringstream ss;
    ss << ":Box\n";

    ss << "--Top--\n";

    ss << x << " " << y << " " << z << "\n";
    ss << x << " " << y << " " << -z << "\n";
    ss << -x << " " << y << " " << -z << "\n";

    ss << "-------------------------------------\n";

    ss << x << " " << y << " " << z << "\n";
    ss << -x << " " << y << " " << -z << "\n";
    ss << -x << " " << y << " " << z << "\n";

    ss << "--Bottom--\n";

    ss << x << " " << -y << " " << -z << "\n";
    ss << x << " " << -y << " " << z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";

    ss << "-------------------------------------\n";

    ss << -x << " " << -y << " " << -z << "\n";
    ss << x << " " << -y << " " << z << "\n";
    ss << -x << " " << -y << " " << z << "\n";

    ss << "--Right--\n";

    ss << x << " " << -y << " " << -z << "\n";
    ss << x << " " << y << " " << -z << "\n";
    ss << x << " " << y << " " << z << "\n";

    ss << "-------------------------------------\n";

    ss << x << " " << -y << " " << -z << "\n";
    ss << x << " " << y << " " << z << "\n";
    ss << x << " " << -y << " " << z << "\n";

    ss << "--Left--\n";

    ss << -x << " " << y << " " << -z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";
    ss << -x << " " << y << " " << z << "\n";

    ss << "-------------------------------------\n";

    ss << -x << " " << y << " " << z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";
    ss << -x << " " << -y << " " << z << "\n";

    ss << "--Front--\n";

    ss << x << " " << -y << " " << z << "\n";
    ss << x << " " << y << " " << z << "\n";
    ss << -x << " " << y << " " << z << "\n";

    ss << "-------------------------------------\n";

    ss << x << " " << -y << " " << z << "\n";
    ss << -x << " " << y << " " << z << "\n";
    ss << -x << " " << -y << " " << z << "\n";

    ss << "--Rear--\n";

    ss << x << " " << y << " " << -z << "\n";
    ss << x << " " << -y << " " << -z << "\n";
    ss << -x << " " << y << " " << -z << "\n";

    ss << "-------------------------------------\n";

    ss << -x << " " << y << " " << -z << "\n";
    ss << x << " " << -y << " " << -z << "\n";
    ss << -x << " " << -y << " " << -z << "\n";

    ss << "-------------------------------------\n";

    return "12" + ss.str();

}

string gen_Sphere(float radius,int slices,int stacks){
    float a_interval = 2 * PI / slices;
    float b_interval = PI / stacks;
    float next_a, next_b;

    int triangles = 0;
    stringstream ss;
    ss << ":Sphere\n";

    for (float a = 0; a < 2 * PI; a +=  a_interval) {
        for (float b = -PI/2; b < PI/2; b += b_interval) {
            next_a = a + a_interval;
            next_b = b + b_interval;
            if (next_a > 2 * PI) {
                next_a = 2 * PI;
            }
            if (next_b > PI / 2) {
                next_b = PI / 2;
            }
            ss << radius * cos(next_b) * sin(next_a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(next_a) << "\n";
            ss << radius * cos(next_b) * sin(a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(a) << "\n";
            ss << radius * cos(b) * sin(next_a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(next_a) << "\n";

            ss << "-------------------------------------\n";

            ss << radius * cos(b) * sin(next_a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(next_a) << "\n";
            ss << radius * cos(next_b) * sin(a) << " " << radius * sin(next_b) << " " << radius * cos(next_b) * cos(a) << "\n";
            ss << radius * cos(b) * sin(a) << " " << radius * sin(b) << " " << radius * cos(b) * cos(a) << "\n";

            ss << "-------------------------------------\n";
            triangles += 2;
        }
    }
    return to_string(triangles) + ss.str();
}

string gen_Cone(float radius,float height,int slices,int stacks){
    float interval = 2 * PI / slices;
    float next_a;
    float next_h;
    float stack_height = height / stacks;

    int triangles = 0;
    stringstream ss;
    ss << ":Cone\n";

    for (float a = 0; a < 2 * PI; a += interval) {
        next_a = a + interval;
        if (next_a > 2 * PI) {
            next_a = 2 * PI;
        }
        ss << "--Bottom--\n";
        ss << 0.0f << " " << 0 << " " << 0.0f << "\n";
        ss << radius * sin(next_a) << " " << 0 << " " << radius * cos(next_a) << "\n";
        ss << radius * sin(a) << " " << 0 << " " << radius * cos(a) << "\n";

        triangles += 1;

        for (float h = 0; h < height; h += stack_height) {
            next_h = h + stack_height;
            if (next_h > height) {
                next_h = height;
            }
            ss << "--Walls--\n";
            ss << radius * sin(next_a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(next_a) * ((height - next_h) / height) << "\n";
            ss << radius * sin(a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(a) * ((height - next_h) / height) << "\n";
            ss << radius * sin(next_a) * ((height - h) / height) << " " << h << " " << radius * cos(next_a) * ((height - h) / height) << "\n";

            ss << "-------------------------------------\n";

            ss << radius * sin(a) * ((height - next_h) / height) << " " << next_h << " " << radius * cos(a) * ((height - next_h) / height) << "\n";
            ss << radius * sin(a) * ((height - h) / height) << " " << h << " " << radius * cos(a) * ((height - h) / height) << "\n";
            ss << radius * sin(next_a) * ((height - h) / height) << " " << h << " " << radius * cos(next_a) * ((height - h) / height) << "\n";

            ss << "-------------------------------------\n";
            triangles += 2;
        }
    }

    return to_string(triangles) + ss.str();;
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
    string fname, fig, res; // res: armazenará os valores dos vertices
    stringstream ss;
    for (int i = 1; i< argc; i++){
        ss << argv[i];
        ss << " ";
    }
    ss >> fig;  // para identificar fig
    if (argc == 1){
      cout << "\nError! No arguments passed.\n";
      exit(1);
    }
    // Preocupação em ver se todos os parametros foram passados
    switch (fig.at(0)) {   // buscar primeira letra
        case 'p': {
            if(argc < 3){
                cout << "\nError! Invalid number of arguments.\n";
                exit(1);
            }
            ss >> fname; //para ir buscar o nome que foi atribuido ao ficheiro
            res = gen_Plane(PLANE_WIDTH);
        break;
        }
        case 'b': {
            if(argc < 6){
                cout << "\nError! Invalid number of arguments.\n";
                exit(1);
            }
            float pBx; ss >> pBx;
            float pBy; ss >> pBy;
            float pBz; ss >> pBz;
            ss >> fname;
            res = gen_Box(pBx,pBy,pBz);
        break;
        }
        case 's': {
            if(argc < 6){
              cout << "\nError! Invalid number of arguments.\n";
              exit(1);
            }
            float radius; ss >> radius;
            int slices; ss >> slices;
            int stacks; ss >> stacks;
            ss >> fname;
            res = gen_Sphere(radius,slices,stacks);
        break;
        }
        case 'c': {
            if(argc < 7){
              cout << "\nError! Invalid number of arguments.\n";
              exit(1);
            }
            float radius; ss >> radius;
            float height; ss >> height;
            int slices; ss >> slices;
            int stacks; ss >> stacks;
            ss >> fname;
            res = gen_Cone(radius,height,slices,stacks);
        break;
        }
        default:{
                  cout << "\nError! That figure doesn't exist.\n";
        break;
        }
    }
        // Fazer a criação do ficheiro:
        constructF(res,fname);
        return 0;
}
