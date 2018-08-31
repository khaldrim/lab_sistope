#include "../../Bmp/Bmp.hpp"
#include "../../Buffer/Buffer.hpp"
#include "../../../../u++-7.0.0/inc/uC++.h"

#ifndef _READIMAGE_HPP_
#define _READIMAGE_HPP_

_Task ReadImage {
    int cflag;
    Buffer *buffer;

    private:
        void main();

    public:
    	ReadImage(int img, Buffer *m);
    	~ReadImage();
    	
        void setCflag(int c);
        int getCflag();
        void setMonitor(Buffer *m);
		Bmp* readBmpInfoHeader(Bmp *file, FILE *fp);
		Bmp* readBmpFileHeader(Bmp *file, FILE *fp);
		Bmp* readBmpFile(Bmp *file, int img);
};


#endif