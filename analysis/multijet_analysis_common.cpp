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
#include <tclap/CmdLine.h>

#include "../common/common.h"

#include "../common/ptBinning.h"
#include "../common/npvBinning.h"


using namespace std;

int main (int argc, char** argv) 
{
//open input file

	bool isMC = false;	
	
	string typeName = "_data";
	int plotColor = getDataColor();
	string extension = ".pdf";
	std::vector<std::string> inputFiles;
	string outputName;
	string inputName;
	bool rmPU = false;		
  	


//********************************************************************************************************* 	

  try {
    // Define the command line object, and insert a message
    // that describes the program. The "Convert extractor tuples to histograms" 
    // is printed last in the help text. The second argument is the 
    // delimiter (usually space) and the last one is the version number. 
    // The CmdLine object parses the argv array based on the Arg objects
    // that it contains. 
    TCLAP::CmdLine cmd("Extract responses and histograms from weight plots", ' ', "1.0");
    
    // Define a value argument and add it to the command line.
    // A value arg defines a flag and a type of value that it expects,
    // such as "-n Bishop".

    TCLAP::ValueArg<std::string> inputFileArg("i", "input-file", "The input file", true, "", "string", cmd);
    TCLAP::ValueArg<std::string> outputFileArg("o", "output-file", "output file", true, "", "string", cmd);
    
    // Define a switch and add it to the command line.
    // A switch arg is a boolean argument and only defines a flag that
    // indicates true or false.  In this example the SwitchArg adds itself
    // to the CmdLine object as part of the constructor.  This eliminates
    // the need to call the cmd.add() method.  All args have support in
    // their constructors to add themselves directly to the CmdLine object.
    // It doesn't matter which idiom you choose, they accomplish the same thing.

    TCLAP::SwitchArg dataArg("", "data", "Is this data?", false);
    TCLAP::SwitchArg mcArg("", "mc", "Is this mc?", false);

    cmd.xorAdd(dataArg, mcArg);
    
    TCLAP::SwitchArg rmPUArg("", "rmPU", "Do you want to remove PU jets?", false);
    cmd.add(rmPUArg);

    
    // Parse the argv array.
    cmd.parse(argc, argv);
    
    // Get the value parsed by each arg.
    isMC = mcArg.getValue();
    outputName = outputFileArg.getValue();
    inputName = inputFileArg.getValue();
    rmPU = rmPUArg.getValue();
    
    if(rmPU) {
	extension = "_woPUJets.pdf";    
    }
    
    if (!isMC) {
      typeName = "_data";
      plotColor = getDataColor();    
    }
    else {
      typeName = "_mc";
      plotColor = getMcColor();    
    }
    
  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    return 1;
  }
   
    
    
  TFile *f=TFile::Open(inputName.c_str()); 
  	

//*********************************************************************************************************
//
//                                      histograms definition
//
//*********************************************************************************************************

	ptBinning myPtBinning;
	npvBinning myNpvBinning;
	
	int numberPtBins = myPtBinning.getSize();
	int numberNpvBins = myNpvBinning.getSize();
	
	string vectorName;
	string ptBinName;
	string npvBinName;

	
//usefull variables
	Double_t xlow = getHistoXlow();
	Double_t xup = getHistoXup();
	//Double_t    binrange = 0.1;
	//Int_t    nbinsx = (xup - xlow)/binrange;
	Int_t    nbinsx = getHistoNbinsx();

	int nflavours = getFlavourNumber(); //uds, g, c, b, noMatched, all

//vectors for multijet study
	//MJB per Npv
	vector<TH1F*> vMJB_Npv;
	vMJB_Npv.resize(numberNpvBins);

	//MJB per recoilpt
	vector<TH1F*> vMJB_RecoilPt;
	vMJB_RecoilPt.resize(numberPtBins);
	
	//Rmpf per recoilpt
	vector<TH1F*> vMPF_RecoilPt;
	vMPF_RecoilPt.resize(numberPtBins);

	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		ptBinName = myPtBinning.getName(j);
		vectorName = "MJB/MJB_" + ptBinName;
		vMJB_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
		vectorName = "MPF/MPF_" + ptBinName;
		vMPF_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());	
	}
	
	//if(isMC) {
		//recoilpt/firstjetgenpt ratio per firstjetgenpt
		vector<TH1F*> vPtRatio_GenPt;
		vPtRatio_GenPt.resize(numberPtBins);
		//Rtrue per recoilpt
		vector<TH1F*> vRtrue_RecoilPt;
		vRtrue_RecoilPt.resize(numberPtBins);
	
		//Rrecoil per recoilpt
		vector<TH1F*> vRrecoil_RecoilPt;
		vRrecoil_RecoilPt.resize(numberPtBins);
		
		for(int j=0; j<myPtBinning.getSize(); j++) {
			ptBinName = myPtBinning.getName(j);
			vectorName = "Rtrue/Rtrue_" + ptBinName;
			vRtrue_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "PtRatio/PtRatio_" + ptBinName;			
			vPtRatio_GenPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "Rtrue/Rrecoil_" + ptBinName;
			vRrecoil_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
		}		

	
	//}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		npvBinName = myNpvBinning.getName(j);
		vectorName = "MJB/MJB_" + npvBinName;
		vMJB_Npv[j] = (TH1F*)f->Get(vectorName.c_str());	
	}

//*******************************************************************************************************
//
//                                      histograms definition
//
//*******************************************************************************************************
	
	string saveName;

	TH1F* hMJB_inclusive = (TH1F*)f->Get("variables/afterSel/hMJB_inclusive");
	TH1F* hNjet_JetPt = (TH1F*)f->Get("variables/afterSel/hNjet_JetPt");	
	TH1F* hNpujet_JetPt = (TH1F*)f->Get("variables/afterSel/hNpujet_JetPt");	
	TH1F* hNjet_Npv = (TH1F*)f->Get("variables/afterSel/hNjet_Npv");	
	TH1F* hNpujet_Npv = (TH1F*)f->Get("variables/afterSel/hNpujet_Npv");
	TH1F* hFracJetsPt = (TH1F*)f->Get("variables/afterSel/hFracJetsPt");
	TH1F* hHT = (TH1F*)f->Get("variables/afterSel/hHT");
	TH1F* hNjetsRecoil = (TH1F*)f->Get("variables/afterSel/hNjetsRecoil");
	TH1F* hNjetsTotal =  (TH1F*)f->Get("variables/afterSel/hNjetsTotal");	
	
	TH1F* hMet_beforeSel = (TH1F*)f->Get("variables/beforeSel/hMet_beforeSel");
	TH1F* hLeadingJetPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hLeadingJetPt_beforeSel");	
	TH1F* hRecoilPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hRecoilPt_beforeSel");	
	TH1F* hNpv_beforeSel = (TH1F*)f->Get("variables/beforeSel/hNpv_beforeSel");	
	TH1F* hAlpha_beforeSel = (TH1F*)f->Get("variables/beforeSel/hAlpha_beforeSel");
	TH1F* hBeta_beforeSel = (TH1F*)f->Get("variables/beforeSel/hBeta_beforeSel");	
	TH1F* hA_beforeSel = (TH1F*)f->Get("variables/beforeSel/hA_beforeSel");	
	TH1F* hRecoilJetsPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hRecoilJetsPt_beforeSel");	
		
	TH1F* hMet_afterSel = (TH1F*)f->Get("variables/afterSel/hMet_afterSel");	
	TH1F* hLeadingJetPt_afterSel = (TH1F*)f->Get("variables/afterSel/hLeadingJetPt_afterSel");		
	TH1F* hRecoilPt_afterSel = (TH1F*)f->Get("variables/afterSel/hRecoilPt_afterSel");	
	TH1F* hNpv_afterSel = (TH1F*)f->Get("variables/afterSel/hNpv_afterSel");				
	TH1F* hAlpha_afterSel = (TH1F*)f->Get("variables/afterSel/hAlpha_afterSel");	
	TH1F* hBeta_afterSel = (TH1F*)f->Get("variables/afterSel/hBeta_afterSel");	
	TH1F* hA_afterSel = (TH1F*)f->Get("variables/afterSel/hA_afterSel");
	TH1F* hRecoilJetsPt_afterSel = (TH1F*)f->Get("variables/afterSel/hRecoilJetsPt_afterSel");


//************************************************************************************************************
//
//                                      ptRecoil/ptjetstot
//
//************************************************************************************************************

	h1_style(hFracJetsPt);
	
	TCanvas* cFracJetsPt = new TCanvas();
	cFracJetsPt->cd();
	hFracJetsPt->Draw("hist");
	saveName = "images/Njets/hFracPtRecoil_ptTotJets" + typeName + extension;
	cFracJetsPt->SaveAs(saveName.c_str());


//************************************************************************************************************
//
//                                      Njet and NpuJets as a function of ptjet
//
//************************************************************************************************************

	h1_style(hNjet_JetPt);
	h1_style(hNpujet_JetPt);
	
	TCanvas* cNjets_JetPt = new TCanvas();
	cNjets_JetPt->cd();
	hNjet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/Njets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	cNjets_JetPt->Clear();
	hNpujet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/Npujets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
	TH1F *hFracPuJets_JetPt = new TH1F("hFracPuJets_JetPt","hFracPuJets_JetPt",30,0,600);
	h1_style(hFracPuJets_JetPt);
	hFracPuJets_JetPt = (TH1F*)hNpujet_JetPt->Clone();
	hFracPuJets_JetPt->Divide(hNjet_JetPt);
	hFracPuJets_JetPt->SetNameTitle("hFracPuJets_JetPt","PU jets fraction wrt p_{t}^{jet}");
	hFracPuJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hFracPuJets_JetPt->SetYTitle("PU jets fraction");
	cNjets_JetPt->Clear();
	hFracPuJets_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/FracPuJets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuJets as a function of Npv
//
//************************************************************************************************************

	h1_style(hNjet_Npv);
	h1_style(hNpujet_Npv);
	
	TCanvas* cNjets_Npv = new TCanvas();
	cNjets_Npv->cd();
	hNjet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/Njets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	cNjets_Npv->Clear();
	hNpujet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/Npujets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
	TH1F *hFracPuJets_Npv = new TH1F("hFracPuJets_Npv","hFracPuJets_Npv",250,0,2500);
	h1_style(hFracPuJets_Npv);
	hFracPuJets_Npv = (TH1F*)hNpujet_Npv->Clone();
	hFracPuJets_Npv->Divide(hNjet_Npv);
	hFracPuJets_Npv->SetNameTitle("hFracPuJets_Npv","PU jets fraction wrt N_{PV}");
	hFracPuJets_Npv->SetXTitle("N_{PV}");
	hFracPuJets_Npv->SetYTitle("PU jets fraction");
	cNjets_Npv->Clear();
	hFracPuJets_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/FracPuJets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
 
//************************************************************************************************************
//
//                                      recoilpt/leadingjetgenpt as a function of leadingjetgenpt
//
//************************************************************************************************************	

	TGraphErrors *gPtRatio_GenPt = NULL;
	if(isMC) {
		float aPtRatio_Mean[numberPtBins];
		float aPtRatio_MeanError[numberPtBins];
		float aGenPtBins_Mean[numberPtBins];
		float aGenPtBins_MeanError[numberPtBins];
	
		for(int i=0; i<numberPtBins; i++) {
			aPtRatio_Mean[i] = vPtRatio_GenPt[i]->GetMean();
			aPtRatio_MeanError[i] = vPtRatio_GenPt[i]->GetMeanError();
			aGenPtBins_Mean[i] = ( myPtBinning.getBinValueInf(i)+myPtBinning.getBinValueSup(i) )/2.;
			aGenPtBins_MeanError[i]=0.;
		}
	
		TCanvas *cPtRatio_GenPt = new TCanvas("cPtRatio_GenPt","cPtRatio_GenPt");
		cPtRatio_GenPt->cd();
	
		gPtRatio_GenPt = new TGraphErrors(numberPtBins,aGenPtBins_Mean, aPtRatio_Mean, aGenPtBins_MeanError, aPtRatio_MeanError);
		gPtRatio_GenPt->SetName("PtRatio");
		gPtRatio_GenPt->SetTitle("p_{T}^{Recoil}/p_{T}^{Leading Jet Gen} as a function of p_{T}^{Leading Jet Gen}");
		gPtRatio_GenPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet Gen} (GeV)");
		gPtRatio_GenPt->GetYaxis()->SetTitle("p_{T}^{Recoil}/p_{T}^{Leading Jet Gen}");
		gPtRatio_GenPt->SetMarkerStyle(20);
		gPtRatio_GenPt->SetMarkerColor(plotColor);
		gPtRatio_GenPt->SetLineColor(plotColor);
		//gPtRatio_GenPt->SetMarkerSize(0.5);	
		cPtRatio_GenPt->cd();
		//gPtRatio_GenPt->SetLogx(1);
		//gPtRatio_GenPt->GetYaxis()->SetRangeUser(0.9,1.2);
		gPtRatio_GenPt->Draw("ape");
		TGraph_style (gPtRatio_GenPt);
		saveName = "images/PtRatio/cPtRatio_GenPt" + typeName + extension;
		cPtRatio_GenPt->SaveAs(saveName.c_str());
	}

//************************************************************************************************************
//
//                                      MJB as a function of Npv
//
//************************************************************************************************************	
	

	float aMJB_Npv_Mean[numberNpvBins];
	float aMJB_Npv_MeanError[numberNpvBins];
	float aNpvBins_Mean[numberNpvBins];
	float aNpvBins_MeanError[numberNpvBins];
	
	for(int i=0; i<numberNpvBins; i++) {
		aMJB_Npv_Mean[i] = vMJB_Npv[i]->GetMean();
		aMJB_Npv_MeanError[i] = vMJB_Npv[i]->GetMeanError();
		aNpvBins_Mean[i] = i;
		aNpvBins_MeanError[i]=0.;
	}
	
	TCanvas *cMJB_Npv = new TCanvas("cMJB_Npv","cMJB_Npv");
	cMJB_Npv->cd();
	
	TGraphErrors *gMJB_Npv = new TGraphErrors(numberNpvBins,aNpvBins_Mean, aMJB_Npv_Mean, aNpvBins_MeanError, aMJB_Npv_MeanError);
	gMJB_Npv->SetName("MJB");
	
	gMJB_Npv->SetTitle("MJB as a function of N_{PV}");
	
	gMJB_Npv->GetXaxis()->SetTitle("N_{PV}");
	gMJB_Npv->GetYaxis()->SetTitle("MJB");
	gMJB_Npv->SetMarkerStyle(20);
	gMJB_Npv->SetMarkerColor(plotColor);
	gMJB_Npv->SetLineColor(plotColor);
	//gMJB_Npv->SetMarkerSize(0.5);	
	cMJB_Npv->cd();
	//gMJB_Npv->SetLogx(1);
	//gMJB_Npv->GetYaxis()->SetRangeUser(0.7,1.1);	
	gMJB_Npv->Draw("ape");
	TGraph_style (gMJB_Npv);
	saveName = "images/MJB/cMJB_Npv" + typeName + extension;
	cMJB_Npv->SaveAs(saveName.c_str());
	
	 
    
//************************************************************************************************************
//
//                                      MJB as a function of recoilpt
//
//************************************************************************************************************	

	float aMJB_RecoilPt_Mean[numberPtBins];
	float aMJB_RecoilPt_MeanError[numberPtBins];
	float aRecoilPtBins_Mean[numberPtBins];
	float aRecoilPtBins_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aMJB_RecoilPt_Mean[i] = vMJB_RecoilPt[i]->GetMean();
		aMJB_RecoilPt_MeanError[i] = vMJB_RecoilPt[i]->GetMeanError();
		aRecoilPtBins_Mean[i] = ( myPtBinning.getBinValueInf(i)+myPtBinning.getBinValueSup(i) )/2.;
		aRecoilPtBins_MeanError[i]=0.;
	}
	
	TCanvas *cMJB_RecoilPt = new TCanvas("cMJB_RecoilPt","cMJB_RecoilPt");
	cMJB_RecoilPt->cd();
	
	TGraphErrors *gMJB_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aMJB_RecoilPt_Mean, aRecoilPtBins_MeanError, aMJB_RecoilPt_MeanError);
	gMJB_RecoilPt->SetName("MJB");
	gMJB_RecoilPt->SetTitle("MJB as a function of p_{T}^{Recoil}");
	gMJB_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gMJB_RecoilPt->GetYaxis()->SetTitle("MJB");
	gMJB_RecoilPt->SetMarkerStyle(20);
	gMJB_RecoilPt->SetMarkerColor(plotColor);
	gMJB_RecoilPt->SetLineColor(plotColor);
	//gMJB_RecoilPt->SetMarkerSize(0.5);	
	cMJB_RecoilPt->cd();
	//gMJB_RecoilPt->SetLogx(1);
	//gMJB_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMJB_RecoilPt->Draw("ape");
	TGraph_style (gMJB_RecoilPt);
	saveName = "images/MJB/cMJB_RecoilPt" + typeName + extension;
	cMJB_RecoilPt->SaveAs(saveName.c_str());
	
	
//************************************************************************************************************
//
//                                      Rmpf as a function of recoilpt
//
//************************************************************************************************************	

	float aMPF_Mean[numberPtBins];
	float aMPF_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aMPF_Mean[i] = vMPF_RecoilPt[i]->GetMean();
		aMPF_MeanError[i] = vMPF_RecoilPt[i]->GetMeanError();
	}
	
	TCanvas *cMPF_RecoilPt = new TCanvas("cMPF_RecoilPt","cMPF_RecoilPt");
	cMPF_RecoilPt->cd();
	
	TGraphErrors *gMPF_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aMPF_Mean, aRecoilPtBins_MeanError, aMPF_MeanError);
	gMPF_RecoilPt->SetName("MPF");
	gMPF_RecoilPt->SetTitle("R_{MPF} as a function of p_{T}^{Recoil}");
	gMPF_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gMPF_RecoilPt->GetYaxis()->SetTitle("R_{MPF}");
	gMPF_RecoilPt->SetMarkerStyle(20);
	gMPF_RecoilPt->SetMarkerColor(plotColor);
	gMPF_RecoilPt->SetLineColor(plotColor);
	//gMPF_RecoilPt->SetMarkerSize(0.5);	
	cMPF_RecoilPt->cd();
	//gMPF_RecoilPt->SetLogx(1);
	//gMPF_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMPF_RecoilPt->Draw("ape");
	TGraph_style (gMPF_RecoilPt);
	saveName = "images/MPF/cMPF_RecoilPt" + typeName + extension;
	cMPF_RecoilPt->SaveAs(saveName.c_str());
	
	
	TGraphErrors *gRtrue_RecoilPt = NULL;
	TGraphErrors *gRrecoil_RecoilPt = NULL;
	
	if(isMC) {
//************************************************************************************************************
//
//                                      Rtrue as a function of recoilpt
//
//************************************************************************************************************	

	
		float aRtrue_Mean[numberPtBins];
		float aRtrue_MeanError[numberPtBins];
	
		for(int i=0; i<numberPtBins; i++) {
			aRtrue_Mean[i] = vRtrue_RecoilPt[i]->GetMean();
			aRtrue_MeanError[i] = vRtrue_RecoilPt[i]->GetMeanError();
		}
	
		TCanvas *cRtrue_RecoilPt = new TCanvas("cRtrue_RecoilPt","cRtrue_RecoilPt");
		cRtrue_RecoilPt->cd();
	
		gRtrue_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aRtrue_Mean, aRecoilPtBins_MeanError, aRtrue_MeanError);
		gRtrue_RecoilPt->SetName("Rtrue");
		gRtrue_RecoilPt->SetTitle("R_{true} as a function of p_{T}^{Recoil}");
		gRtrue_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
		gRtrue_RecoilPt->GetYaxis()->SetTitle("R_{true}");
		gRtrue_RecoilPt->SetMarkerStyle(20);
		gRtrue_RecoilPt->SetMarkerColor(plotColor);
		gRtrue_RecoilPt->SetLineColor(plotColor);
		//gRtrue_RecoilPt->SetMarkerSize(0.5);	
		cRtrue_RecoilPt->cd();
		//gRtrue_RecoilPt->SetLogx(1);
		//gRtrue_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
		gRtrue_RecoilPt->Draw("ape");
		TGraph_style (gRtrue_RecoilPt);
		saveName = "images/Rtrue/cRtrue_RecoilPt" + typeName + extension;
		cRtrue_RecoilPt->SaveAs(saveName.c_str());
	

//************************************************************************************************************
//
//                                      Rrecoil as a function of recoilpt
//
//************************************************************************************************************	

		float aRrecoil_Mean[numberPtBins];
		float aRrecoil_MeanError[numberPtBins];
	
		for(int i=0; i<numberPtBins; i++) {
			aRrecoil_Mean[i] = vRrecoil_RecoilPt[i]->GetMean();
			aRrecoil_MeanError[i] = vRrecoil_RecoilPt[i]->GetMeanError();
		}
	
		TCanvas *cRrecoil_RecoilPt = new TCanvas("cRrecoil_RecoilPt","cRrecoil_RecoilPt");
		cRrecoil_RecoilPt->cd();
	
		gRrecoil_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aRrecoil_Mean, aRecoilPtBins_MeanError, aRrecoil_MeanError);
		gRrecoil_RecoilPt->SetName("Rrecoil");
		gRrecoil_RecoilPt->SetTitle("R_{recoil} as a function of p_{T}^{Recoil}");
		gRrecoil_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
		gRrecoil_RecoilPt->GetYaxis()->SetTitle("R_{recoil}");
		gRrecoil_RecoilPt->SetMarkerStyle(20);
		gRrecoil_RecoilPt->SetMarkerColor(plotColor);
		gRrecoil_RecoilPt->SetLineColor(plotColor);
		//gRrecoil_RecoilPt->SetMarkerSize(0.5);	
		cRrecoil_RecoilPt->cd();
		//gRrecoil_RecoilPt->SetLogx(1);
		//gRrecoil_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
		gRrecoil_RecoilPt->Draw("ape");
		TGraph_style (gRrecoil_RecoilPt);
		saveName = "images/Rrecoil/cRrecoil_RecoilPt" + typeName + extension;
		cRrecoil_RecoilPt->SaveAs(saveName.c_str());	
	
	}

//*********************************************************************************************************
//
//                                      Output file
//
//*********************************************************************************************************

	//create the output file and write into it
	TFile *out = new TFile(outputName.c_str(), "recreate");
	
	out->cd();	

	TDirectory *mjbDir = out->mkdir("MJB","MJB");
	mjbDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Write();
	}
	gMJB_RecoilPt->Write("gMJB_RecoilPt");	
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Write();
	}
	gMJB_Npv->Write("gMJB_Npv");	
	
	TDirectory *mpfDir = out->mkdir("MPF","MPF");
	mpfDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RecoilPt[j]->Write();
	}
	gMPF_RecoilPt->Write("gMPF_RecoilPt");	
	
	if(isMC) {
		TDirectory *trueDir = out->mkdir("Rtrue","Rtrue");
		trueDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_RecoilPt[j]->Write();
			vRrecoil_RecoilPt[j]->Write();
		}
		gRtrue_RecoilPt->Write("gRtrue_RecoilPt");
	
		TDirectory *ptratioDir = out->mkdir("PtRatio","PtRatio");
		ptratioDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Write();
		}
		gPtRatio_GenPt->Write("gPtRatio_GenPt");
	}	
	
	TDirectory *variablesDir = out->mkdir("variables","variables");
	variablesDir->cd();
	
	TDirectory *beforeSelDir = variablesDir->mkdir("beforeSel","beforeSel");
	beforeSelDir->cd();
	hMet_beforeSel->Write();
	hLeadingJetPt_beforeSel->Write();
	hRecoilPt_beforeSel->Write();
	hRecoilJetsPt_beforeSel->Write();
	hNpv_beforeSel->Write();
	hAlpha_beforeSel->Write();
	hBeta_beforeSel->Write();
	hA_beforeSel->Write();
	
	TDirectory *afterSelDir = variablesDir->mkdir("afterSel","afterSel");
	afterSelDir->cd();
	hNjetsRecoil->Write();
	hNjetsTotal->Write();
	hMet_afterSel->Write();
	hLeadingJetPt_afterSel->Write();
	hRecoilPt_afterSel->Write();
	hRecoilJetsPt_afterSel->Write();
	hNpv_afterSel->Write();
	hAlpha_afterSel->Write();
	hBeta_afterSel->Write();
	hA_afterSel->Write();
	hMJB_inclusive->Write();
	hNjet_JetPt->Write();
	hNpujet_JetPt->Write();
	hNjet_Npv->Write();
	hNpujet_Npv->Write();
	hHT->Write();
	hFracJetsPt->Write();
	
	
	out->Close();
	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Delete();
	}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Delete();
	}

	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RecoilPt[j]->Delete();
	}	
	
	if(isMC) {
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_RecoilPt[j]->Delete();
		}
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Delete();
		}
	}
	hMJB_inclusive->Delete();
	hNjet_JetPt->Delete();
	hNpujet_JetPt->Delete();
	hNjet_Npv->Delete();
	hNpujet_Npv->Delete();
	hHT->Delete();
	hFracJetsPt->Delete();	
	hMet_beforeSel->Delete();
	hLeadingJetPt_beforeSel->Delete();
	hRecoilPt_beforeSel->Delete();
	hNpv_beforeSel->Delete();
	hAlpha_beforeSel->Delete();
	hBeta_beforeSel->Delete();
	hA_beforeSel->Delete();	
	hMet_afterSel->Delete();
	hLeadingJetPt_afterSel->Delete();
	hRecoilPt_afterSel->Delete();
	hNpv_afterSel->Delete();
	hAlpha_afterSel->Delete();
	hBeta_afterSel->Delete();
	hA_afterSel->Delete();
	hNjetsRecoil->Delete();
	hNjetsTotal->Delete();
	hRecoilJetsPt_beforeSel->Delete();
	hRecoilJetsPt_afterSel->Delete();
	
	return 0;
}









