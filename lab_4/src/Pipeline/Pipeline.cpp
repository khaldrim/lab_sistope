#include "Pipeline.hpp"
#include "../Buffer/Buffer.hpp"
#include "../Pipeline/ReadImage/ReadImage.hpp"

Pipeline::Pipeline(int c, int u, int n, int b){
    cout << "Object Pipeline created." << endl;

    setCflag(c);
    setUflag(u);
    setNflag(n);
    setBflag(b);
}

Pipeline::~Pipeline(){
    cout << "Object Pipeline deleted." << endl;
}

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
    
    /* Monitor , el argumento define el tamaño de la cola. */
    Buffer buffer { 5 };

    /* Stages */
    ReadImage read { this-> getCflag() , buffer };

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