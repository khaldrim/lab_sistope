#include <iostream>
#include "Pipeline.hpp"
#include "../ReadImage/ReadImage.hpp"

using namespace std;

int Pipeline::getImgCount(){
    return Pipeline::imgCount;
}

void Pipeline::setImgCount(int value){
    this -> imgCount = value;
}

int Pipeline::start(){
    int i = 0;

    ReadImage r;
    
    cout << "Pipeline Start\n";

    while(i < getImgCount()){
        cout << "Pipeline imgCount: " << i << "\n";
        i++;
    }

    return 0;
}