#ifndef _BMP_HPP_
#define _BMP_HPP_

typedef unsigned short WORD;     /* 2 bytes */
typedef unsigned int DWORD;      /* 4 bytes */
typedef unsigned long long LONG; /* 8 bytes */

class Bmp {
    private:

        /* File header */
        char type[3];
        DWORD size;
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

    public:

    /* Setters and Getters */
    WORD Bmp::getWidth();
};

#endif