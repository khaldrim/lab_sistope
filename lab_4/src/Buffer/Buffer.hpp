#include <vector>
#include "../../u++-7.0.0/inc/uC++.h"
#include "../Bmp/Bmp.hpp"

#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

_Monitor Buffer {	

    public:
    	uCondition full, empty;
	    constexpr std::vector<Bmp> imagesRead;
	    std::vector<Bmp> imagesGrey;
	    std::vector<Bmp> imagesBin;
	    std::vector<Bmp> imagesWrite;

        Buffer();
        ~Buffer();
        void insertBmp(Bmp *bmp, int stage);
    	void setBmpVector(Bmp *bmp, int stage);
        //std::vector getBmpVector(int stage);
};


#endif