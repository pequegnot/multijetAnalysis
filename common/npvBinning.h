#ifndef DEF_NPVBINNING
#define DEF_NPVBINNING

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <utility>

class npvBinning
{
    public:

    npvBinning();
    ~npvBinning();
    void fillNpvBins();
    size_t getSize();
    std::string getName(int bin);
    int getBinValueInf();
    int getBinValueSup();

    private:
    std::vector<int> mNpvBins;


};

#endif


