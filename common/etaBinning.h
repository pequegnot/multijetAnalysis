#ifndef DEF_ETABINNING
#define DEF_ETABINNING

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <utility>

class etaBinning
{
    public:

    etaBinning();
    etaBinning(bool isLowPtBinning);
    ~etaBinning();
    void fillPtBins();
    void fillLowPtBins();
    int getPtBin(float eta);
    size_t getSize();
    std::string getName(int bin);
    std::pair<float, float> getBinValue(int bin);
    float getBinValueInf(int bin);
    float getBinValueSup(int bin);


    private:
    std::vector<std::pair<float,float> > mPtBins;


};

#endif


