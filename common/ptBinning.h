#ifndef DEF_PTBINNING
#define DEF_PTBINNING

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <utility>

class ptBinning
{
    public:

    ptBinning();
    ptBinning(bool isLowPtBinning);
    ~ptBinning();
    void fillPtBins();
    void fillLowPtBins();
    int getPtBin(float pt);
    size_t getSize();
    std::string getName(int bin);
    std::pair<float, float> getBinValue(int bin);
    float getBinValueInf(int bin);
    float getBinValueSup(int bin);


    private:
    std::vector<std::pair<float,float> > mPtBins;


};

#endif


