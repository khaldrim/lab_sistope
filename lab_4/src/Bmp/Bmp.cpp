#include <iostream>
#include "Bmp.hpp"

using namespace std;

   /* File header */
        char type[3];
        DWORD fileSize;
        WORD reserved1;
        WORD reserved2;
        DWORD offbits;

        /* Info header */
        DWORD size;
        LONG width;
        LONG height;
        WORD planes;
        WORD bitPerPixel;
        DWORD compression;
        DWORD sizeImage;
        LONG xPelsPerMeter;
        LONG yPelsperMeter;
        DWORD used;
        DWORD important;
        DWORD redMask;
        DWORD greenMask;
        DWORD blueMask;
        DWORD alphaMask;
        DWORD csType;
        DWORD ciexyzXRed;
        DWORD ciexyzYRed;
        DWORD ciexyzZRed;
        DWORD ciexyzXGreen;
        DWORD ciexyzYGreen;
        DWORD ciexyzZGreen;
        DWORD ciexyzXBlue;
        DWORD ciexyzYBlue;
        DWORD ciexyzZBlue;
        DWORD gammaRed;
        DWORD gammaGreen;
        DWORD gammaBlue;
        DWORD intent;
        DWORD profileData;
        DWORD profileSize;
        DWORD reserved;

/* Setters and Getters*/
char Bmp::setType(){ Bmp::type = 'BM\0'; }
char Bmp::getType(){ return Bmp::type; }

DWORD Bmp::setFileSize(DWORD s){ Bmp::fileSize = s; }
DWORD Bmp::getFileSize(){ return Bmp::fileSize; }


WORD Bmp::getWidth(){ return Bmp::width; } 