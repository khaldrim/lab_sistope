#include "../../Bmp/Bmp.hpp"

#ifndef _READIMAGE_HPP_
#define _READIMAGE_HPP_

class ReadImage {
    private:

    public:

		Bmp* readBmpInfoHeader(Bmp *file, FILE *fp);
		Bmp* readBmpFileHeader(Bmp *file, FILE *fp);
		Bmp* readBmpFile(Bmp *file, int img);
};

#endif