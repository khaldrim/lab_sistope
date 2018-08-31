#include <iostream>
#include "Buffer.hpp"

using namespace std;

Buffer::Buffer(){
	cout << "Object Buffer Started." << endl; 
}
Buffer::~Buffer(){ cout << "Object Buffer Delete." << endl; }

void Buffer::insertBmp(){
	cout << "getBmp!" << endl;
}

void Buffer::getBmp(){
	cout << "getBmp!" << endl;
}