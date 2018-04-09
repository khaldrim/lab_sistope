#include <stdio.h>
#include <stdint.h>

#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef unsigned short WORD; /* 2 bytes */
typedef unsigned int DWORD;  /* 4 bytes */
typedef unsigned long long LONG; /* 8 bytes */

/* Bitmap file header struct */
typedef struct  __attribute__((__packed__))
{
    unsigned char type[3]; /* 2 bytes + null char */
    DWORD size;
    WORD reserved1;
    WORD reserved2;
    DWORD offbits;

} BITMAPFILEHEADER;

/* Bitmap info header struct (Windows) */
typedef struct  __attribute__((__packed__))
{
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

} BITMAPINFOHEADER;



/* RGB struct */
typedef struct __attribute__((__packed__))
{
    unsigned char blue;
    unsigned char green;
    unsigned char red; 
    unsigned char alpha;

} RGB;

#endif