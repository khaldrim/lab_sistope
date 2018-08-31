#include "../../Bmp/Bmp.hpp"
#include "../../../../u++-7.0.0/inc/uC++.h"

#ifndef _READIMAGE_HPP_
#define _READIMAGE_HPP_

_Task ReadImage {
    private:
    	void main(Bmp *file, int turn);

    public:
    	ReadImage();
    	~ReadImage();
    	
    	void start(int cflag);
		Bmp* readBmpInfoHeader(Bmp *file, FILE *fp);
		Bmp* readBmpFileHeader(Bmp *file, FILE *fp);
		void readBmpFile(Bmp *file, int img);
};


#endif