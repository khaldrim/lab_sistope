#include <iostream>
#include "Pipeline.hpp"
#include "../Bmp/Bmp.hpp"
#include "../Buffer/Buffer.hpp"
#include "../Pipeline/ReadImage/ReadImage.hpp"

using namespace std;

/* Getter and Setter Cflag */
int Pipeline::getCflag(){
    return Pipeline::cflag;
}

void Pipeline::setCflag(int value){
    this -> cflag = value;
}

/* Getter and Setter Uflag */
int Pipeline::getUflag(){
    return Pipeline::uflag;
}

void Pipeline::setUflag(int value){
    this -> uflag = value;
}

/* Getter and Setter Nflag */
int Pipeline::getNflag(){
    return Pipeline::nflag;
}

void Pipeline::setNflag(int value){
    this -> nflag = value;
}

/* Getter and Setter Bflag */
int Pipeline::getBflag(){
    return Pipeline::bflag;
}

void Pipeline::setBflag(int value){
    this -> bflag = value;
}

int Pipeline::start(){
    cout << "Pipeline Start\n";
    
    /* Monitor */
    Buffer buffer;

    /* Stages */
    ReadImage read { this-> getCflag() , &buffer };

    /*

    while(i < this -> getCflag()){
        cout << "Pipeline imgCount: " << i+1 << "\n";
        cout << "Pipeline readImage\n";

        r -> readBmpFile(&img[i], i+1);

        i++;
    }

    */

    return 0;
}