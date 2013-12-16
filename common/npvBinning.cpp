#include "npvBinning.h"

npvBinning::npvBinning()
{
	fillNpvBins();
}


npvBinning::~npvBinning()
{

}

size_t npvBinning::getSize()
{
	return mNpvBins.size();
}

std::string npvBinning::getName(int bin) {
	std::stringstream NpvBinningName ;
	NpvBinningName << "Npv_" << (int) bin;
	return NpvBinningName.str();
}

int npvBinning::getBinValueInf()  
{
	return mNpvBins.front();
}

int npvBinning::getBinValueSup()  
{
	return mNpvBins.back();
}


void npvBinning::fillNpvBins() 
{
	for(int i=0; i<70; i++) {
		mNpvBins.push_back(i);
	}
}





