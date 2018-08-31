#include "../../u++-7.0.0/inc/uC++.h"

#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

_Monitor Buffer {
    uCondition full, empty;

    private:

    public:

        Buffer();
        ~Buffer();
        void insertBmp();
        void getBmp();
};


#endif