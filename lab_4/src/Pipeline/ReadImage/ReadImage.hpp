#ifndef _READIMAGE_HPP_
#define _READIMAGE_HPP_

class ReadImage {
    private:
        int stage;

    public:
        int saludo;

    void printRead();
    void readBmpInfoHeader();
    void readBmpFileHeader();
    void readBmpFile();
};

#endif