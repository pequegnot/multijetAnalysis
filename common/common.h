#pragma once

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <TVector3.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TF1.h>
#include <TH2F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TTree.h>
#include <TMath.h>
#include <TLegend.h>
#include <TStyle.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <stdio.h>
#include <TString.h>
#include "TMatrixF.h"
#include "TMatrixFSym.h"
#include "TMatrixFSparse.h"
#include "TMatrixFLazy.h"
#include "TVectorF.h"
#include <TRandom.h>
#include <THStack.h>
#include "TROOT.h"
#include<math.h>

#include "ptBinning.h"
#include "npvBinning.h"
#include "etaBinning.h"

int getDataColor () {
	return 1;
}

int getMcColor () {
	return 46;
}

float HLT_PFJet140_prescaleFactor () {
	return 368.852;
}

float HLT_PFJet200_prescaleFactor () {
	return 77.030;
}

float HLT_PFJet260_prescaleFactor () {
	return 19.048;
}

float HLT_PFJet320_prescaleFactor () {
	return 1.0;
}

std::string getFlavourBinName(int flavourBin) {
//****************************************************************************************
//                   !!!! WARNING !!!!
//        if you change the correspondance binName-binNumber
//        do not forget to change it in the getFlavourBin function of common.h
//****************************************************************************************
		if(flavourBin==0) {
			return "uds";
		}
		else if(flavourBin==1) {
			return "g";
		}
		else if(flavourBin==2) {
			return "c";
		}
		else if(flavourBin==3) {
			return "b";
		}
		else if(flavourBin==4) {
			return "noMatched";
		}
		else if(flavourBin==5) {
			return "all";
		}
		return ""; 
}

int getFlavourNumber () {
	return 6;
}


float getLumi () {
	return (876.225 + 4.412*pow(10.,3.) +  7.051*pow(10.,3.) + 7.369l*pow(10.,3.));
}

int getHistoNbinsx() {
	return 30;
}

double getHistoXlow() {
	return 0.;
}

double getHistoXup() {
	return 2.;
}

std::vector<TH1F*> buildPtVectorH1(ptBinning aPtBinning,const std::string& aName,Int_t nbinsx,Double_t xlow,Double_t xup)
{
	std::vector<TH1F*> myVector;
	for(int i=0; i<aPtBinning.getSize(); i++) {
		std::string histoName;
		histoName = aName + "_" + aPtBinning.getName(i) ;
		myVector.push_back(new TH1F(histoName.c_str(),histoName.c_str(),nbinsx, xlow, xup));
	}
	return myVector;
}

std::vector<TH1F*> buildBinnedDistriVectorH1(ptBinning aPtBinning, const std::string& aName, Double_t binrange)
{
	std::vector<TH1F*> myVector;
  Double_t xlow;
  Double_t xup;
  Int_t nbinsx;
	for(int i=0; i<aPtBinning.getSize(); i++) {
		std::string histoName;
		histoName = aName + "_" + aPtBinning.getName(i) ;
    xlow = aPtBinning.getBinValueInf(i);
    xup = aPtBinning.getBinValueSup(i);
    nbinsx = (xup - xlow)/binrange;
		myVector.push_back(new TH1F(histoName.c_str(),histoName.c_str(),nbinsx, xlow, xup));
	}
	return myVector;
}

std::vector<TH1F*> buildEtaVectorH1(etaBinning aEtaBinning,const std::string& aName,Int_t nbinsx,Double_t xlow,Double_t xup)
{
	std::vector<TH1F*> myVector;
	for(int i=0; i<aEtaBinning.getSize(); i++) {
		std::string histoName;
		histoName = aName + "_" + aEtaBinning.getName(i) ;
		myVector.push_back(new TH1F(histoName.c_str(),histoName.c_str(),nbinsx, xlow, xup));
	}
	return myVector;
}

std::vector<TH1F*> buildNpvVectorH1(npvBinning aNpvBinning,const std::string& aName,Int_t nbinsx,Double_t xlow,Double_t xup)
{
	std::vector<TH1F*> myVector;
	for(int i=0; i<aNpvBinning.getSize(); i++) {
		std::string histoName;
		histoName = aName + "_" + aNpvBinning.getName(i) ;
		myVector.push_back(new TH1F(histoName.c_str(),histoName.c_str(),nbinsx, xlow, xup));
	}
	return myVector;
}

std::vector<TH1F*> buildNrecoiljetsVectorH1(const std::string& aName,Int_t nbinsx,Double_t xlow,Double_t xup)
{
	std::vector<TH1F*> myVector;
	for(int i=0; i<16; i++) {
		std::stringstream histoName;
		histoName << aName << "_NrecoilJets_" << i ;
		myVector.push_back(new TH1F(histoName.str().c_str(),histoName.str().c_str(),nbinsx, xlow, xup));
	}
	return myVector;
}


//****************************************************************************************
//                                 !!!! WARNING !!!!
//        if you change the correspondance binName-binNumber in binning.h
//            do not forget to change it in the getFlavourBin function 
//****************************************************************************************

int getFlavourBin(float afirstjetgenpdgid)
{
	if(TMath::Abs(afirstjetgenpdgid) == 1 || TMath::Abs(afirstjetgenpdgid) == 2 || TMath::Abs(afirstjetgenpdgid) == 3) {//uds quarks
		return 0;
	}	
	else if(TMath::Abs(afirstjetgenpdgid) == 21) {//gluons
		return 1;
		}
	else if(TMath::Abs(afirstjetgenpdgid) == 4) {//c
		return 2;
	}
	else if(TMath::Abs(afirstjetgenpdgid) == 5) {//b
		return 3;
	}
	else {//no matched
		return 4;
	}	
}

//*********************************************************************************************************

void TMultiGraph_style (TMultiGraph* h) {
	h->GetXaxis()->SetLabelOffset(0.005);
	h->GetXaxis()->SetLabelFont(42);
	h->GetXaxis()->SetLabelSize(0.055);
	h->GetXaxis()->SetTitleOffset(1.15);
	h->GetXaxis()->SetTitleSize(0.04);
	h->GetXaxis()->SetTitleFont(42);
	//h->GetYaxis()->SetRangeUser(0.,1.);
	h->GetYaxis()->SetLabelOffset(0.005);
	h->GetYaxis()->SetLabelFont(42);
	h->GetYaxis()->SetLabelSize(0.045);	
	h->GetYaxis()->SetTitleOffset(1.2);
	h->GetYaxis()->SetTitleFont(42);
	h->GetYaxis()->SetTitleSize(0.04);	
}

void TGraph_style (TGraph* h) {
	h->GetXaxis()->SetLabelOffset(0.005);
	h->GetXaxis()->SetLabelFont(42);
	h->GetXaxis()->SetLabelSize(0.055);
	h->GetXaxis()->SetTitleOffset(1.15);
	h->GetXaxis()->SetTitleSize(0.04);
	h->GetXaxis()->SetTitleFont(42);
	//h->GetYaxis()->SetRangeUser(0.,1.);
	h->GetYaxis()->SetLabelOffset(0.005);
	h->GetYaxis()->SetLabelFont(42);
	h->GetYaxis()->SetLabelSize(0.045);	
	h->GetYaxis()->SetTitleOffset(1.2);
	h->GetYaxis()->SetTitleFont(42);
	h->GetYaxis()->SetTitleSize(0.04);	
}

void TGraph_style2 (TGraph* h, const std::string& Xtitle, const std::string& Ytitle, int color, int style, int size=1) {
	h->GetXaxis()->SetLabelOffset(0.005);
	h->GetXaxis()->SetLabelFont(42);
	h->GetXaxis()->SetLabelSize(0.055);
	h->GetXaxis()->SetTitleOffset(1.15);
	h->GetXaxis()->SetTitleSize(0.04);
	h->GetXaxis()->SetTitleFont(42);
	h->GetXaxis()->SetTitle(Xtitle.c_str());
	//h->GetYaxis()->SetRangeUser(0.,1.);
	h->GetYaxis()->SetLabelOffset(0.005);
	h->GetYaxis()->SetLabelFont(42);
	h->GetYaxis()->SetLabelSize(0.045);	
	h->GetYaxis()->SetTitleOffset(1.2);
	h->GetYaxis()->SetTitleFont(42);
	h->GetYaxis()->SetTitleSize(0.04);
	h->GetYaxis()->SetTitle(Ytitle.c_str());	
		
	h->SetMarkerColor(color);
	h->SetMarkerStyle(style);
	h->SetMarkerSize(size);
}

void TGraph_data_style (TGraph* g) {
	g->SetMarkerStyle(20);
	g->SetMarkerColor(getDataColor());
	g->SetLineColor(getDataColor());
	g->SetMarkerSize(1.0);	
}

void TGraph_mc_style (TGraph* g) {
	g->SetMarkerStyle(20);
	g->SetMarkerColor(getMcColor());
	g->SetLineColor(getMcColor());
	g->SetMarkerSize(1.0);	
}

void h1_style(TH1 *h, int optstat=0) {
	h->SetStats(optstat);	
	h->SetLabelFont(42,"X");       // 42
	h->SetLabelFont(42,"Y");       // 42
	h->SetLabelOffset(0.005,"X");  // D=0.005
	h->SetLabelOffset(0.005,"Y");  // D=0.005
	h->SetLabelSize(0.045,"X");
	h->SetLabelSize(0.045,"Y");
	h->SetTitleOffset(1.15,"X");
	h->SetTitleOffset(1.15,"Y");
	h->SetTitleSize(0.04,"X");
	h->SetTitleSize(0.04,"Y");
	//h->SetTitle(0);
	h->SetTitleFont(42, "XYZ");
	h->SetMarkerSize(0.8);
}

void h1_style_lo(TH1 *h, int optstat=0) {
	h->SetStats(optstat);
    	h->SetXTitle("Photon p_{T} [GeV/c]");
    	h->SetYTitle("Data / MC");
    	h->GetXaxis()->SetTitleOffset(1.2);
    	h->GetYaxis()->SetTitleOffset(0.55);
    	h->GetXaxis()->SetTickLength(0.06);
    	h->GetXaxis()->SetMoreLogLabels();
    	h->GetXaxis()->SetNoExponent();
    	//h->GetXaxis()->SetLabelSize(0.);
   	h->GetXaxis()->SetLabelSize(0.085);
   	h->GetYaxis()->SetLabelSize(0.07);
   	h->GetXaxis()->SetTitleSize(0.09);
    	h->GetYaxis()->SetTitleSize(0.08);	
	//h->SetTitleFont(42, "XYZ");
}

void h1_style_hi(TH1 *h, int optstat=0) {
  h->GetXaxis()->SetTitleOffset(1.1);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetTitleSize(0.045);
  //h->GetXaxis()->SetMoreLogLabels();
  //h->GetXaxis()->SetNoExponent();
  h->GetXaxis()->SetLabelSize(0.);
}

std::vector<int> MarkerStyle() {
	std::vector<int> MarkerStyle;
	MarkerStyle.resize(getFlavourNumber());
	//uds
	MarkerStyle[0] = 20;
	//g
	MarkerStyle[1] = 20;
	//c
	MarkerStyle[2] = 20;
	//b
	MarkerStyle[3] = 20;
	//no matched
	MarkerStyle[4] = 20;
	//all
	MarkerStyle[5] = 20;

	return MarkerStyle;
}

std::vector<int> HistoColor() {
	std::vector<int> HistoColor;
	HistoColor.resize(getFlavourNumber());
	//uds
	HistoColor[0] = 4;
	//g
	HistoColor[1] = 2;
	//c
	HistoColor[2] = 7;
	//b
	HistoColor[3] = 5;
	//no matched
	HistoColor[4] = 3;
	//all
	HistoColor[5] = 6;

	return HistoColor;
}


