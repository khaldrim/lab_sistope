#include <stdio.h>

#ifndef _STRUCT_H_
#define _STRUCT_H_

/*
 * Bitmap file header.
 */
typedef struct tagBMPFILEHEADER 
{
    char           fileType[3]; /* 2 bytes */
    unsigned int   fileSize;    /* 4 bytes */
    unsigned short reserved1;   /* 2 bytes */
    unsigned short reserved2;   /* 2 bytes */
    unsigned long  offBits;     /* 4 bytes */

} BMPFILEHEADER;

/*
 * Bitmap infor header (OS/2)
 */
// typedef struct _BMPINFOOSHEADER
// {
//     unsigned int   osSize;         /* 4 bytes */
//     short          osWidth;        /* 2 bytes */
//     short          osHeight;       /* 2 bytes */
//     unsigned short osColorPlanes;  /* 2 bytes */
//     unsigned short osBitsPerPixel; /* 2 bytes */

// } BMPINFOOSHEADER;

/*
 * Bitmap info header (Windows)
 */
typedef struct tagBMPINFOWINHEADER
{
    unsigned int   size;         /* 4 bytes */
    long           width;        /* 4 bytes */
    long           height;       /* 4 bytes */
    unsigned short colorPlanes;  /* 2 bytes */
    unsigned short bitsPerPixel; /* 2 bytes */
    unsigned int   compression;  /* 4 bytes */
    unsigned int   imgSize;      /* 4 bytes */
    long           xPixPerMeter; /* 4 bytes */
    long           yPixPerMeter; /* 4 bytes */
    unsigned long  colorPalette; /* 4 bytes */
    unsigned long  colorUsed;    /* 4 bytes */

} BMPINFOHEADER;

typedef struct _RGB
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} RGB;

#endif