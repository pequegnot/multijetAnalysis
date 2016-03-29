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
    mPtBins.push_back(std::make_pair(360., 440.));
    mPtBins.push_back(std::make_pair(440., 3000.));
  } else {
    // QCD_HT_300to500 sample not available: starting the analysis at HT = 500 so pt recoil ~350-400
/*    mPtBins.push_back(std::make_pair(210., 290.));*/
    /*mPtBins.push_back(std::make_pair(290., 370.));*/
    mPtBins.push_back(std::make_pair(370., 470.));
    mPtBins.push_back(std::make_pair(470., 550.));
    mPtBins.push_back(std::make_pair(550., 610.));
    mPtBins.push_back(std::make_pair(610., 3000.));
  }
}

void HLTPtBinning::fillHLTName() {

  mPtNames.clear();
  // QCD_HT_300to500 sample not available: starting the analysis at HT = 500 so pt recoil ~350-400
/*  mPtNames.push_back("HLT_PFJet140");*/
  /*mPtNames.push_back("HLT_PFJet200");*/
  mPtNames.push_back("HLT_PFJet260");
  mPtNames.push_back("HLT_PFJet320");
  mPtNames.push_back("HLT_PFJet400");
  mPtNames.push_back("HLT_PFJet450");

}


