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
    std::string getHLTName(int bin);
    void fillHLTPtBins(bool useRecoilPtBinHLTSel);
    void fillHLTName();
    int getHLTNumber() {
      return mPtNames.size();
    }

    private:
    std::vector<std::string> mPtNames;
};

#endif




