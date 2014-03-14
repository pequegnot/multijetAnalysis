#ifndef DEF_HLTPTBINNING
#define DEF_HLTPTBINNING

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <utility>

#include "ptBinning.h"

class HLTPtBinning : public ptBinning
{
    public:

    HLTPtBinning();
    ~HLTPtBinning();
    void fillHLTPtBins();
};

#endif



