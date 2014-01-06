#include "etaBinning.h"

etaBinning::etaBinning()
{
	fillEtaBins();
}


etaBinning::~etaBinning()
{

}

int etaBinning::getEtaBin(float eta)
{
	for(int i=0; i<mEtaBins.size(); i++)
	{
		std::pair<float, float> bin = mEtaBins[i];
		if (eta >= bin.first && eta < bin.second)
		{
			return i;
		}	
	}
	
	return -1;
}

size_t etaBinning::getSize()
{
	return mEtaBins.size();
}

std::pair<float, float> etaBinning::getBinValue(int bin)  
{
	return mEtaBins[bin];
}

float etaBinning::getBinValueInf(int bin)  
{
	return mEtaBins[bin].first;
}

float etaBinning::getBinValueSup(int bin)  
{
	return mEtaBins[bin].second;
}

std::string etaBinning::getName(int bin) {
	std::stringstream EtaBinningName ;
	EtaBinningName << "eta_" << (int) getBinValueInf(bin) << "_" << (int) getBinValueSup(bin);
	return EtaBinningName.str();
}


void etaBinning::fillEtaBins() 
{
	mEtaBins.push_back(std::make_pair(0., 0.8));
	mEtaBins.push_back(std::make_pair(0.8, 1.3));
	mEtaBins.push_back(std::make_pair(1.3, 1.9));
	mEtaBins.push_back(std::make_pair(1.9, 2.5));
	mEtaBins.push_back(std::make_pair(2.5, 3.0));
	mEtaBins.push_back(std::make_pair(3.0, 3.2));
	mEtaBins.push_back(std::make_pair(3.2, 5.2));

}





