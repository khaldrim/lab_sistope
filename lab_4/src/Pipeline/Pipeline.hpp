#ifndef _PIPELINE_HPP_
#define _PIPELINE_HPP_

#include <iostream>

using namespace std;

class Pipeline {
    private:
        int cflag;
        int uflag;
        int nflag;
        int bflag;

    public:

    /* Constructor and Destructor */
    Pipeline(int c, int u, int n, int b);
    ~Pipeline();

    /* Get and Set cflag */
    int getCflag();
    void setCflag(int value);

    /* Get and Set uflag */
    int getUflag();
    void setUflag(int value);

    /* Get and Set nflag */
    int getNflag();
    void setNflag(int value);

    /* Get and Set bflag */
    int getBflag();
    void setBflag(int value);

    int start();
};

#endif