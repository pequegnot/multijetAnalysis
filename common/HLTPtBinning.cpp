#include "HLTPtBinning.h"

HLTPtBinning::HLTPtBinning () {
  fillHLTName();
}

HLTPtBinning::~HLTPtBinning()
{

}

std::string HLTPtBinning::getHLTName(int bin) {
  return mPtNames[bin];
}


void HLTPtBinning::fillHLTPtBins(bool useRecoilPtBinHLTSel) 
{
  mPtBins.clear();

  if (!useRecoilPtBinHLTSel) {
    mPtBins.push_back(std::make_pair(170., 230.));
    mPtBins.push_back(std::make_pair(230., 290.));
    mPtBins.push_back(std::make_pair(290., 360.));
    mPtBins.push_back(std::make_pair(360., 3000.));
  } else {
/*    mPtBins.push_back(std::make_pair(210., 280.));*/
    //mPtBins.push_back(std::make_pair(280., 320.));
    //mPtBins.push_back(std::make_pair(320., 400.));
    /*mPtBins.push_back(std::make_pair(400., 3000.));*/
    
    //mPtBins.push_back(std::make_pair(250., 290.));
    mPtBins.push_back(std::make_pair(210., 290.));
    mPtBins.push_back(std::make_pair(290., 370.));
    mPtBins.push_back(std::make_pair(370., 470.));
    mPtBins.push_back(std::make_pair(470., 3000.));
  }
}

void HLTPtBinning::fillHLTName() {

  mPtNames.clear();
  mPtNames.push_back("HLT_PFJet140");
  mPtNames.push_back("HLT_PFJet200");
  mPtNames.push_back("HLT_PFJet260");
  mPtNames.push_back("HLT_PFJet320");

}


