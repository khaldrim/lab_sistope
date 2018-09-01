#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <queue>
#include <iostream>
#include <uC++.h>
#include "../Bmp/Bmp.hpp"

using namespace std;

_Monitor Buffer {
	private:
		uCondition full, empty;
		int amount;
		queue<Bmp> img;

    public:
		Buffer(int m);
        ~Buffer();
		void insertBmp(Bmp bmp);
    	//void setBmpVector(Bmp *bmp, int stage);
};


#endif