#include "logPtPrimeBinning.h"

logPtPrimeBinning::logPtPrimeBinning()
{
	fillLogPtPrimeBins();
}

logPtPrimeBinning::~logPtPrimeBinning()
{

}

std::string logPtPrimeBinning::getName(int bin) {
	std::stringstream PtBinningName ;
	PtBinningName << "logPtPrime_" << (int) (getBinValueInf(bin)*10) << "_" << (int) (getBinValueSup(bin)*10);
	return PtBinningName.str();
}


void logPtPrimeBinning::fillLogPtPrimeBins() 
{
  mPtBins.clear();

  //mPtBins.push_back(std::make_pair(3.0, 3.2));
  //mPtBins.push_back(std::make_pair(3.2, 3.4));
  //mPtBins.push_back(std::make_pair(3.4, 3.6));
  //mPtBins.push_back(std::make_pair(3.6, 3.8));
  mPtBins.push_back(std::make_pair(3.8, 4.0));
  mPtBins.push_back(std::make_pair(4.0, 4.2));
  mPtBins.push_back(std::make_pair(4.2, 4.4));
  mPtBins.push_back(std::make_pair(4.4, 4.6));
  mPtBins.push_back(std::make_pair(4.6, 4.8));
  mPtBins.push_back(std::make_pair(4.8, 5.0));
  mPtBins.push_back(std::make_pair(5.0, 5.2));
  mPtBins.push_back(std::make_pair(5.2, 5.4));
  mPtBins.push_back(std::make_pair(5.4, 5.6));
  mPtBins.push_back(std::make_pair(5.6, 5.8));
  mPtBins.push_back(std::make_pair(5.8, 6.0));
}


