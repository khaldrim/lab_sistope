#include "Buffer.hpp"

Buffer::Buffer(int m){
	this -> amount = m;
	cout << "Object Buffer Started." << endl; 
};

Buffer::~Buffer(){ cout << "Object Buffer Delete." << endl; };


void Buffer::insertBmp(Bmp bmp){
	cout << "Start Insert Bmp in queue" << endl;

	if(this -> amount == img.size())
		this -> empty.wait();

	cout << "Cola no llena" << endl;

	img.push(bmp);
	amount++;

	cout << "End Insert Bmp in queue" << endl;
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