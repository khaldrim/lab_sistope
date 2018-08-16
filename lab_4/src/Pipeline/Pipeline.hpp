#ifndef _PIPELINE_HPP_
#define _PIPELINE_HPP_

class Pipeline {
    private:
        int imgCount;
    public:

    /* Get and Set imgCount */
    int getImgCount();
    void setImgCount(int value);

    int start();
};

#endif