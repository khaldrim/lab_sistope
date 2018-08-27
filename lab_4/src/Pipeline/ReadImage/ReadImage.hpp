#ifndef _READIMAGE_HPP_
#define _READIMAGE_HPP_

#include "../../Bmp/Bmp.hpp"

class ReadImage {
    private:

    public:

	Bmp* ReadImage::readBmpInfoHeader(Bmp *file, FILE *fp);
	Bmp* ReadImage::readBmpFileHeader(Bmp *file, FILE *fp);
	Bmp* ReadImage::readBmpFile(Bmp *img, int img);
};

#endif