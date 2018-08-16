#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include "ReadImage.cpp"
//readImage
class ReadImage {
    private:
        int stage;

    public:
        ReadImage();
        int getStage();
        void setStage(int i);
};


#endif