#include <iostream>
#include "Buffer.hpp"

using namespace std;

Buffer::Buffer(){
	std::vector<Bmp> imagesRead;
    std::vector<Bmp> imagesGrey;
    std::vector<Bmp> imagesBin;
    std::vector<Bmp> imagesWrite;

	cout << "Object Buffer Started." << endl; 
}
Buffer::~Buffer(){ cout << "Object Buffer Delete." << endl; }

void insertBmp(Bmp *bmp, int stage){
	cout << "Start Insert Bmp in vector" << endl;

	if(stage == 1){
		imagesRead.push_back(bmp);
	} else {
		cout << "Vector de imagenes Bmp desconocido." << endl;
	}
	
	cout << "End Insert Bmp in vector" << endl;
}

/*
void Buffer::setBmpVector(Bmp *bmp, int stage){
	if(stage == 1){
		Buffer::imagesRead -> push_back(bmp);
	} else {
		cout << "Vector de imagenes Bmp desconocido." << endl;
	}
}
*/

/*
std::vector Buffer::getBmpVector(int stage){
	if(stage == 1){
		return imagesRead;
	} else {
		cout << "Vector de imagenes Bmp desconocido." << endl;
	}
}
*/