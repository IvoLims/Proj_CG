#include <iostream>
#include <fstream>
using namespace std;

#include <string>
#include <array>

#include <cstdio>
#include "tinyxml2.h"
#include "tinyxml2.cpp"
using namespace tinyxml2;



int main(){
	XMLDocument doc;
	doc.LoadFile( "xmlconf.xml" );
	const char* title = doc.FirstChildElement( "scene" )->FirstChildElement("model")->Attribute("file");

	string ch;
	string conteudo = "";
	ifstream file;
	int i=0;
	file.open(title, ios::in);
	if (!file) {
		cout << "No such file";
		exit(1);
	}
	file >> ch;
	int size = stoi(ch);
	float array[size*9];
	while(1){
		file >> ch;
		cout << ch << " ";
		array[i] = stof(ch);
		i++;
		if(file.eof()) break;
	}
	file.close();
	return 0;
}
