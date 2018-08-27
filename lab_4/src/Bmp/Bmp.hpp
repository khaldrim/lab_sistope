#ifndef _BMP_HPP_
#define _BMP_HPP_

typedef unsigned short WORD;     /* 2 bytes */
typedef unsigned int DWORD;      /* 4 bytes */
typedef unsigned long long LONG; /* 8 bytes */

class Bmp {
    private:

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
        LONG yPelsPerMeter;
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
        void setType(FILE *fp);
        char* getType();

        void setFileSize(FILE *fp);
        DWORD getFileSize();

        void setReserved1(FILE *fp);
        void setReserved2(FILE *fp);

        void setOffbits(FILE *fp);
        DWORD getOffbits();

        void setSize(FILE *fp);
        DWORD getSize();

        void setWidth(FILE *fp);
        DWORD getWidth();

        void setHeight(FILE *fp);
        DWORD getHeight();

        void setPlanes(FILE *fp);
        void setBitPerPixel(FILE *fp);
        void setCompression(FILE *fp);
        void setSizeImage(FILE *fp);
        void setxPelsPerMeter(FILE *fp);
        void setyPelsPerMeter(FILE *fp);
        void setUsed(FILE *fp);
        void setImportant(FILE *fp);
        void setRedMask(FILE *fp);
        void setGreenMask(FILE *fp);
        void setBlueMask(FILE *fp);
        void setAlphaMask(FILE *fp);
        void setCsType(FILE *fp);
        void setXRed(FILE *fp);
        void setYRed(FILE *fp);
        void setZRed(FILE *fp);
        void setXGreen(FILE *fp);
        void setYGreen(FILE *fp);
        void setZGreen(FILE *fp);
        void setXBlue(FILE *fp);
        void setYBlue(FILE *fp);
        void setZBlue(FILE *fp);
        void setGammaRed(FILE *fp);
        void setGammaGreen(FILE *fp);
        void setGammaBlue(FILE *fp);
        void setIntent(FILE *fp);
        void setProfileData(FILE *fp);
        void setProfileSize(FILE *fp);
        void setReserved(FILE *fp);



        /* Read Little-Endian 2,4 y 8 bytes */
        unsigned short ReadLE2(FILE *fp);
        unsigned int ReadLE4(FILE *fp);
        unsigned int ReadLE8(FILE *fp);

};

#endif