#include "ptBinning.h"

ptBinning::ptBinning()
{
	fillPtBins();
}

ptBinning::ptBinning(bool isLowPtBinning)
{	
	if(isLowPtBinning) {
		fillLowPtBins();
	}
	else {
		fillPtBins();
	}
}

ptBinning::~ptBinning()
{

}

int ptBinning::getPtBin(float pt)
{
	for(int i=0; i<mPtBins.size(); i++)
	{
		std::pair<float, float> bin = mPtBins[i];
		if (pt >= bin.first && pt < bin.second)
		{
			return i;
		}	
	}
	
	return -1;
}

size_t ptBinning::getSize()
{
	return mPtBins.size();
}

std::pair<float, float> ptBinning::getBinValue(int bin)  
{
	return mPtBins[bin];
}

float ptBinning::getBinValueInf(int bin)  
{
    if(bin >= mPtBins.size())
        return -1;
    else
	    return mPtBins[bin].first;
}

float ptBinning::getBinValueSup(int bin)  
{
    if(bin >= mPtBins.size())
        return -1;
    else
	    return mPtBins[bin].second;
}

std::string ptBinning::getName(int bin) {
	std::stringstream PtBinningName ;
	PtBinningName << "pt_" << (int) getBinValueInf(bin) << "_" << (int) getBinValueSup(bin);
	return PtBinningName.str();
}

void ptBinning::fillLowPtBins() 
{
	mPtBins.push_back(std::make_pair(40., 50.));
	mPtBins.push_back(std::make_pair(50., 60.));
	mPtBins.push_back(std::make_pair(60., 75.));
	mPtBins.push_back(std::make_pair(75., 100.));
	mPtBins.push_back(std::make_pair(100., 125.));
	mPtBins.push_back(std::make_pair(125., 155.));
	mPtBins.push_back(std::make_pair(155., 180.));
	mPtBins.push_back(std::make_pair(180., 210.));
	mPtBins.push_back(std::make_pair(210., 250.));
	mPtBins.push_back(std::make_pair(250., 300.));
	mPtBins.push_back(std::make_pair(300., 360.));
	mPtBins.push_back(std::make_pair(360., 400.));
	mPtBins.push_back(std::make_pair(400., 450.));
	mPtBins.push_back(std::make_pair(450., 500.));
	mPtBins.push_back(std::make_pair(500., 550.));
	mPtBins.push_back(std::make_pair(550., 600.));
	mPtBins.push_back(std::make_pair(600., 700.));
	mPtBins.push_back(std::make_pair(700., 800.));
	mPtBins.push_back(std::make_pair(800., 1000.));
	mPtBins.push_back(std::make_pair(1000., 1200.));
	mPtBins.push_back(std::make_pair(1200., 1500.));
	mPtBins.push_back(std::make_pair(1500., 2000.));
	mPtBins.push_back(std::make_pair(2000., 3000.));
}

void ptBinning::fillPtBins() 
{
  mPtBins.push_back(std::make_pair(150., 200.));
  mPtBins.push_back(std::make_pair(200., 250.));
  mPtBins.push_back(std::make_pair(250., 300.));
  mPtBins.push_back(std::make_pair(300., 360.));
	mPtBins.push_back(std::make_pair(360., 400.));
	mPtBins.push_back(std::make_pair(400., 450.));
	mPtBins.push_back(std::make_pair(450., 500.));
	mPtBins.push_back(std::make_pair(500., 550.));
	mPtBins.push_back(std::make_pair(550., 600.));
	mPtBins.push_back(std::make_pair(600., 700.));
	mPtBins.push_back(std::make_pair(700., 800.));
	mPtBins.push_back(std::make_pair(800., 1000.));
	mPtBins.push_back(std::make_pair(1000., 1200.));
	mPtBins.push_back(std::make_pair(1200., 1500.));
	mPtBins.push_back(std::make_pair(1500., 2000.));
	mPtBins.push_back(std::make_pair(2000., 3000.));
}





