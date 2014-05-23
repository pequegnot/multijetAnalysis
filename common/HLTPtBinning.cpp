#include "HLTPtBinning.h"

HLTPtBinning::HLTPtBinning()
{
	fillHLTPtBins();
}

HLTPtBinning::~HLTPtBinning()
{

}

std::string HLTPtBinning::getHLTName(int bin) {
  return mPtNames[bin];
}


void HLTPtBinning::fillHLTPtBins() 
{
  mPtBins.clear();
  mPtBins.push_back(std::make_pair(170., 230.));
  mPtBins.push_back(std::make_pair(230., 290.));
  mPtBins.push_back(std::make_pair(290., 360.));
  mPtBins.push_back(std::make_pair(360., 3000.));

  mPtNames.clear();
  mPtNames.push_back("HLT_PFJet140");
  mPtNames.push_back("HLT_PFJet200");
  mPtNames.push_back("HLT_PFJet260");
  mPtNames.push_back("HLT_PFJet320");
}




