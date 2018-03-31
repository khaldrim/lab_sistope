#include <stdio.h>

#ifndef _STRUCT_H_
#define _STRUCT_H_

/*
 * Bitmap file header.
 */
typedef struct _BMPFILEHEADER 
{
    char           fileType[3]; /* 2 bytes + null char */
    unsigned int   filesize;    /* 4 bytes */
    unsigned short reserved1;   /* 2 bytes */
    unsigned short reserved2;  /* 2 bytes */
    unsigned long  offBits;     /* 4 bytes */
    unsigned int   headersize;  /* 12 or 40 */

} BMPFILEHEADER;

/*
 * Bitmap infor header (OS/2)
 */
typedef struct _BMPINFOOSHEADER
{
    unsigned int   osSize;         /* 4 bytes */
    short          osWidth;        /* 2 bytes */
    short          osHeight;       /* 2 bytes */
    unsigned short osColorPlanes;  /* 2 bytes */
    unsigned short osBitsPerPixel; /* 2 bytes */

} BMPINFOOSHEADER;

/*
 * Bitmap info header (Windows)
 */
typedef struct _BMPINFOWINHEADER
{
    unsigned int   winSize;         /* 4 bytes */
    long           winWidth;        /* 4 bytes */
    long           winHeight;       /* 4 bytes */
    unsigned short winColorPlanes;  /* 2 bytes */
    unsigned short winBitsPerPixel; /* 2 bytes */
    unsigned int   winCompression;  /* 4 bytes */
    unsigned int   winImgSize;      /* 4 bytes */
    long           winXPixPerMeter; /* 4 bytes */
    long           winYPixPerMeter; /* 4 bytes */
    unsigned long  winColorPalette; /* 4 bytes */
    unsigned long  winColorUsed;    /* 4 bytes */

} BMPINFOWINHEADER;



#endif