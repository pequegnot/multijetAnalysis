#include "HLTPtBinning.h"

HLTPtBinning::HLTPtBinning()
{
	fillHLTPtBins();
}

HLTPtBinning::~HLTPtBinning()
{

}

void HLTPtBinning::fillHLTPtBins() 
{
  mPtBins.clear();
  mPtBins.push_back(std::make_pair(170., 230.));
  mPtBins.push_back(std::make_pair(230., 290.));
  mPtBins.push_back(std::make_pair(290., 360.));
  mPtBins.push_back(std::make_pair(360., 3000.));
}







