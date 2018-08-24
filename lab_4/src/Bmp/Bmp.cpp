#include <iostream>
#include "Bmp.hpp"

using namespace std;

/* Setters and Getters*/
char Bmp::setType(){ Bmp::type = 'BM\0'; }
char Bmp::getType(){ return Bmp::type; }

DWORD Bmp::setFileSize(DWORD s){ Bmp::fileSize = s; }
DWORD Bmp::getFileSize(){ return Bmp::fileSize; }


WORD Bmp::getWidth(){ return Bmp::width; } 