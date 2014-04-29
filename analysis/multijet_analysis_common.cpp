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
#include "../common/HLTPtBinning.h"
#include "../common/npvBinning.h"
#include "../common/etaBinning.h"


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
    TCLAP::ValueArg<std::string> plotNameArg("", "plotName", "plotName", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> extensionArg("", "extension", "extension", false, ".pdf", "string", cmd);
    
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
    extension = plotNameArg.getValue() + extensionArg.getValue();
    rmPU = rmPUArg.getValue();
    
    if(rmPU) {
	extension = "_woPUJets" + plotNameArg.getValue() + extensionArg.getValue();    
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
  HLTPtBinning myHLTPtBinning;
	npvBinning myNpvBinning;
	ptBinning myLowPtBinning(true);
	etaBinning myEtaBinning;
	
	int numberPtBins = myPtBinning.getSize();
  int numberHLTPtBins = myHLTPtBinning.getSize();
	int numberNpvBins = myNpvBinning.getSize();
	int numberEtaBins = myEtaBinning.getSize();
	
	string vectorName;
	string ptBinName;
	string npvBinName;
	string etaBinName;

	
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

	//leadingJetPt per recoilpt
	vector<TH1F*> vLeadingJetPt_RecoilPt;
	vLeadingJetPt_RecoilPt.resize(numberPtBins);

  //leadingJetPt per recoilpt
	vector<TH1F*> vLeadingJetPt_LeadingJetPtHLT;
	vLeadingJetPt_LeadingJetPtHLT.resize(numberHLTPtBins);

 	//RecoilPt per recoilpt
	vector<TH1F*> vRecoilPt_RecoilPt;
	vRecoilPt_RecoilPt.resize(numberPtBins);

  //MJB per recoileta
	vector<TH1F*> vMJB_RecoilEta;
	vMJB_RecoilEta.resize(numberEtaBins);
	
	//Rmpf per recoilpt
	vector<TH1F*> vMPF_RecoilPt;
	vMPF_RecoilPt.resize(numberPtBins);
	
	//NjetsRecoil per recoilpt
	vector<TH1F*> vNjetsRecoil_RecoilPt;
	vNjetsRecoil_RecoilPt.resize(numberPtBins);
	vector<TH1F*> vNjetsRecoil_068E_RecoilPt;
	vNjetsRecoil_068E_RecoilPt.resize(numberPtBins);
	vector<TH1F*> vNjetsRecoil_095E_RecoilPt;
	vNjetsRecoil_095E_RecoilPt.resize(numberPtBins);

	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		ptBinName = myPtBinning.getName(j);
		vectorName = "MJB/recoilPtBin/MJB_" + ptBinName;
		vMJB_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
    vectorName = "leadingJet/recoilPtBin/LeadingJetPt_" + ptBinName;
		vLeadingJetPt_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
    vectorName = "recoil/recoilPtBin/RecoilPt_" + ptBinName;
		vRecoilPt_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
		vectorName = "MPF/MPF_" + ptBinName;
		vMPF_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());	
	}

  for(int j=0; j<myHLTPtBinning.getSize(); j++) {
		ptBinName = myHLTPtBinning.getName(j);
    vectorName = "leadingJet/1stJetPtHLTBin/LeadingJetPt_" + ptBinName;
		vLeadingJetPt_LeadingJetPtHLT[j] = (TH1F*)f->Get(vectorName.c_str());
	}

	for(int j=0; j<myEtaBinning.getSize(); j++) {
		etaBinName = myEtaBinning.getName(j);
		vectorName = "MJB/recoilEtaBin/MJB_" + etaBinName;
		vMJB_RecoilEta[j] = (TH1F*)f->Get(vectorName.c_str());
	}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		npvBinName = myNpvBinning.getName(j);
		vectorName = "MJB/npvBin/MJB_" + npvBinName;
		vMJB_Npv[j] = (TH1F*)f->Get(vectorName.c_str());	
	}
	
	//if(isMC) {
		//recoilpt/firstjetgenpt ratio per firstjetgenpt
		vector<TH1F*> vPtRatio_GenPt;
		vPtRatio_GenPt.resize(numberPtBins);
		//Rtrue per recoilpt
		vector<TH1F*> vRtrue_leadingJet_RecoilPt;
		vRtrue_leadingJet_RecoilPt.resize(numberPtBins);
		
		vector<TH1F*> vRtrue_allJets_JetPt;
		vRtrue_allJets_JetPt.resize(myLowPtBinning.getSize());
	
		//Rrecoil per recoilpt
		vector<TH1F*> vRrecoil_RecoilPt;
		vRrecoil_RecoilPt.resize(numberPtBins);
		
		for(int j=0; j<myPtBinning.getSize(); j++) {
			ptBinName = myPtBinning.getName(j);
			vectorName = "Rtrue/Rtrue_leadingJet_" + ptBinName;
			vRtrue_leadingJet_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "PtRatio/PtRatio_" + ptBinName;			
			vPtRatio_GenPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "Rtrue/Rrecoil_" + ptBinName;
			vRrecoil_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
			
			vectorName = "recoilJets/NjetsRecoil_" + ptBinName;
			vNjetsRecoil_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "recoilJets/NjetsRecoil_068E_" + ptBinName;
			vNjetsRecoil_068E_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "recoilJets/NjetsRecoil_095E_" + ptBinName;
			vNjetsRecoil_095E_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
		}
		
		for(int j=0; j<myLowPtBinning.getSize(); j++) {
			ptBinName = myLowPtBinning.getName(j);
			vectorName = "Rtrue/Rtrue_allJets_" + ptBinName;
			vRtrue_allJets_JetPt[j] = (TH1F*)f->Get(vectorName.c_str());
		}		

	
	//}
	


//*******************************************************************************************************
//
//                                      histograms definition
//
//*******************************************************************************************************
	
	string saveName;

	TH1F* hMJB_inclusive = (TH1F*)f->Get("variables/afterSel/hMJB_inclusive");
	TH1F* hNjet_JetPt = (TH1F*)f->Get("variables/afterSel/hNjet_JetPt");	
	TH1F* hNpuLoosejet_JetPt = (TH1F*)f->Get("variables/afterSel/hNpuLoosejet_JetPt");
	TH1F* hNpuMediumjet_JetPt = (TH1F*)f->Get("variables/afterSel/hNpuMediumjet_JetPt");
	TH1F* hNpuTightjet_JetPt = (TH1F*)f->Get("variables/afterSel/hNpuTightjet_JetPt");
	TH1F* hNpuAlljet_JetPt = (TH1F*)f->Get("variables/afterSel/hNpuAlljet_JetPt");	
	TH1F* hNjet_Npv = (TH1F*)f->Get("variables/afterSel/hNjet_Npv");	
	TH1F* hNpuLoosejet_Npv = (TH1F*)f->Get("variables/afterSel/hNpuLoosejet_Npv");
	TH1F* hNpuMediumjet_Npv = (TH1F*)f->Get("variables/afterSel/hNpuMediumjet_Npv");
	TH1F* hNpuTightjet_Npv = (TH1F*)f->Get("variables/afterSel/hNpuTightjet_Npv");
	TH1F* hNpuAlljet_Npv = (TH1F*)f->Get("variables/afterSel/hNpuAlljet_Npv");
	TH1F* hFracJetsPt = (TH1F*)f->Get("variables/afterSel/hFracJetsPt");
  TH1F* hHT_beforeSel = (TH1F*)f->Get("variables/beforeSel/hHT_beforeSel");
	TH1F* hHT_afterSel = (TH1F*)f->Get("variables/afterSel/hHT_afterSel");
	TH1F* hNjetsRecoil = (TH1F*)f->Get("variables/afterSel/hNjetsRecoil");
	TH1F* hNjetsTotal =  (TH1F*)f->Get("variables/afterSel/hNjetsTotal");	
  TH1F* hJetsPt_afterSel = (TH1F*)f->Get("variables/afterSel/hJetsPt_afterSel");
  TH1F* hJetsEta_afterSel = (TH1F*)f->Get("variables/afterSel/hJetsEta_afterSel");
  TH1F* hJetsPhi_afterSel = (TH1F*)f->Get("variables/afterSel/hJetsPhi_afterSel");

	TH1F* hMet_beforeSel = (TH1F*)f->Get("variables/beforeSel/hMet_beforeSel");
	TH1F* hLeadingJetPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hLeadingJetPt_beforeSel");	
	TH1F* hRecoilPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hRecoilPt_beforeSel");	
	TH1F* hNpv_beforeSel = (TH1F*)f->Get("variables/beforeSel/hNpv_beforeSel");	
	TH1F* hAlpha_beforeSel = (TH1F*)f->Get("variables/beforeSel/hAlpha_beforeSel");
	TH1F* hBeta_beforeSel = (TH1F*)f->Get("variables/beforeSel/hBeta_beforeSel");	
	TH1F* hA_beforeSel = (TH1F*)f->Get("variables/beforeSel/hA_beforeSel");	
	TH1F* hRecoilJetsPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hRecoilJetsPt_beforeSel");	
  TH1F* hJetsPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hJetsPt_beforeSel");
  TH1F* hJetsEta_beforeSel = (TH1F*)f->Get("variables/beforeSel/hJetsEta_beforeSel");
  TH1F* hJetsPhi_beforeSel = (TH1F*)f->Get("variables/beforeSel/hJetsPhi_beforeSel");
  TH1F* hNjets_ptSup25_etaInf5_beforeSel = (TH1F*)f->Get("variables/beforeSel/hNjets_ptSup25_etaInf5_beforeSel");
		
	TH1F* hMet_afterSel = (TH1F*)f->Get("variables/afterSel/hMet_afterSel");	
	TH1F* hLeadingJetPt_afterSel = (TH1F*)f->Get("variables/afterSel/hLeadingJetPt_afterSel");		
	TH1F* hRecoilPt_afterSel = (TH1F*)f->Get("variables/afterSel/hRecoilPt_afterSel");	
	TH1F* hNpv_afterSel = (TH1F*)f->Get("variables/afterSel/hNpv_afterSel");				
	TH1F* hAlpha_afterSel = (TH1F*)f->Get("variables/afterSel/hAlpha_afterSel");	
	TH1F* hBeta_afterSel = (TH1F*)f->Get("variables/afterSel/hBeta_afterSel");	
	TH1F* hA_afterSel = (TH1F*)f->Get("variables/afterSel/hA_afterSel");
	TH1F* hRecoilJetsPt_afterSel = (TH1F*)f->Get("variables/afterSel/hRecoilJetsPt_afterSel");
	TH1F* hNrmPuJets_JetPt = (TH1F*)f->Get("variables/afterSel/hNrmPuJets_JetPt");
	TH1F* hDeltaPhi_METRecoil_afterSel = (TH1F*)f->Get("variables/afterSel/hDeltaPhi_METRecoil_afterSel");
	TH1F* hDeltaPhi_METJet1_afterSel = (TH1F*)f->Get("variables/afterSel/hDeltaPhi_METJet1_afterSel");
	
	TH1F* hRecoilEta = (TH1F*)f->Get("recoil/hRecoilEta");
	TH1F* hRecoilWidth = (TH1F*)f->Get("recoil/hRecoilWidth");


//************************************************************************************************************
//
//                compute fraction of removed jets from the selection because of PU ID cut
//
//************************************************************************************************************

	TH1F* hFracRmPuJets_JetPt=(TH1F*)hNrmPuJets_JetPt->Clone();
	hFracRmPuJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hFracRmPuJets_JetPt->SetYTitle("N_{removed PU jets}/N_{total jets}");
	hFracRmPuJets_JetPt->Sumw2();
	// compute binomial errors for efficiency
	hFracRmPuJets_JetPt->Divide(hNrmPuJets_JetPt, hNjet_JetPt, 1, 1, "B");
	//hFracRmPuJets_JetPt->Divide(hNjet_JetPt);


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
//                                      Njet and NpuLooseJets as a function of ptjet
//
//************************************************************************************************************

	h1_style(hNjet_JetPt);
	h1_style(hNpuLoosejet_JetPt);
	
	TCanvas* cNjets_JetPt = new TCanvas();
	cNjets_JetPt->cd();
	hNjet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/Njets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	cNjets_JetPt->Clear();
	hNpuLoosejet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/NpuLoosejets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
	TH1F *hFracPuLooseJets_JetPt = new TH1F("hFracPuLooseJets_JetPt","hFracPuLooseJets_JetPt",30,0,600);
	h1_style(hFracPuLooseJets_JetPt);
	hFracPuLooseJets_JetPt = (TH1F*)hNpuLoosejet_JetPt->Clone();
	hFracPuLooseJets_JetPt->Divide(hNjet_JetPt);
	hFracPuLooseJets_JetPt->SetNameTitle("hFracPuLooseJets_JetPt","PU Loose jets fraction wrt p_{t}^{jet}");
	hFracPuLooseJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hFracPuLooseJets_JetPt->SetYTitle("PU Loose jets fraction");
	cNjets_JetPt->Clear();
	hFracPuLooseJets_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/FracPuLooseJets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuMediumJets as a function of ptjet
//
//************************************************************************************************************

	h1_style(hNjet_JetPt);
	h1_style(hNpuMediumjet_JetPt);
	
	cNjets_JetPt->cd();
	cNjets_JetPt->Clear();
	hNjet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/Njets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	cNjets_JetPt->Clear();
	hNpuMediumjet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/NpuMediumjets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
	TH1F *hFracPuMediumJets_JetPt = new TH1F("hFracPuMediumJets_JetPt","hFracPuMediumJets_JetPt",30,0,600);
	h1_style(hFracPuMediumJets_JetPt);
	hFracPuMediumJets_JetPt = (TH1F*)hNpuMediumjet_JetPt->Clone();
	hFracPuMediumJets_JetPt->Divide(hNjet_JetPt);
	hFracPuMediumJets_JetPt->SetNameTitle("hFracPuMediumJets_JetPt","PU Medium jets fraction wrt p_{t}^{jet}");
	hFracPuMediumJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hFracPuMediumJets_JetPt->SetYTitle("PU Medium jets fraction");
	cNjets_JetPt->Clear();
	hFracPuMediumJets_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/FracPuMediumJets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuTightJets as a function of ptjet
//
//************************************************************************************************************

	h1_style(hNjet_JetPt);
	h1_style(hNpuTightjet_JetPt);
	
	cNjets_JetPt->cd();
	cNjets_JetPt->Clear();
	hNjet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/Njets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	cNjets_JetPt->Clear();
	hNpuTightjet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/NpuTightjets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
	TH1F *hFracPuTightJets_JetPt = new TH1F("hFracPuTightJets_JetPt","hFracPuTightJets_JetPt",30,0,600);
	h1_style(hFracPuTightJets_JetPt);
	hFracPuTightJets_JetPt = (TH1F*)hNpuTightjet_JetPt->Clone();
	hFracPuTightJets_JetPt->Divide(hNjet_JetPt);
	hFracPuTightJets_JetPt->SetNameTitle("hFracPuTightJets_JetPt","PU Tight jets fraction wrt p_{t}^{jet}");
	hFracPuTightJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hFracPuTightJets_JetPt->SetYTitle("PU Tight jets fraction");
	cNjets_JetPt->Clear();
	hFracPuTightJets_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/FracPuTightJets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuAllJets as a function of ptjet
//
//************************************************************************************************************

	h1_style(hNjet_JetPt);
	h1_style(hNpuAlljet_JetPt);
	
	cNjets_JetPt->cd();
	cNjets_JetPt->Clear();
	hNjet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/Njets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	cNjets_JetPt->Clear();
	hNpuAlljet_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/NpuAlljets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
	TH1F *hFracPuAllJets_JetPt = new TH1F("hFracPuAllJets_JetPt","hFracPuAllJets_JetPt",30,0,600);
	h1_style(hFracPuAllJets_JetPt);
	hFracPuAllJets_JetPt = (TH1F*)hNpuAlljet_JetPt->Clone();
	hFracPuAllJets_JetPt->Divide(hNjet_JetPt);
	hFracPuAllJets_JetPt->SetNameTitle("hFracPuAllJets_JetPt","PU All jets fraction wrt p_{t}^{jet}");
	hFracPuAllJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hFracPuAllJets_JetPt->SetYTitle("PU All jets fraction");
	cNjets_JetPt->Clear();
	hFracPuAllJets_JetPt->Draw("hist");
	cNjets_JetPt->SetLogy();
	saveName = "images/Njets/FracPuAllJets_JetPt" + typeName + extension;
	cNjets_JetPt->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuLooseJets as a function of Npv
//
//************************************************************************************************************

	h1_style(hNjet_Npv);
	h1_style(hNpuLoosejet_Npv);
	
	TCanvas* cNjets_Npv = new TCanvas();
	cNjets_Npv->cd();
	hNjet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/Njets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	cNjets_Npv->Clear();
	hNpuLoosejet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/NpuLoosejets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
	TH1F *hFracPuLooseJets_Npv = new TH1F("hFracPuLooseJets_Npv","hFracPuLooseJets_Npv",250,0,2500);
	h1_style(hFracPuLooseJets_Npv);
	hFracPuLooseJets_Npv = (TH1F*)hNpuLoosejet_Npv->Clone();
	hFracPuLooseJets_Npv->Divide(hNjet_Npv);
	hFracPuLooseJets_Npv->SetNameTitle("hFracPuJets_Npv","PU Loose jets fraction wrt N_{PV}");
	hFracPuLooseJets_Npv->SetXTitle("N_{PV}");
	hFracPuLooseJets_Npv->SetYTitle("PU Loose jets fraction");
	cNjets_Npv->Clear();
	hFracPuLooseJets_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/FracPuLooseJets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuMediumJets as a function of Npv
//
//************************************************************************************************************

	h1_style(hNjet_Npv);
	h1_style(hNpuMediumjet_Npv);
	
	cNjets_Npv->cd();
	cNjets_Npv->Clear();
	hNjet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/Njets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	cNjets_Npv->Clear();
	hNpuMediumjet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/NpuMediumjets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
	TH1F *hFracPuMediumJets_Npv = new TH1F("hFracPuMediumJets_Npv","hFracPuMediumJets_Npv",250,0,2500);
	h1_style(hFracPuMediumJets_Npv);
	hFracPuMediumJets_Npv = (TH1F*)hNpuMediumjet_Npv->Clone();
	hFracPuMediumJets_Npv->Divide(hNjet_Npv);
	hFracPuMediumJets_Npv->SetNameTitle("hFracPuJets_Npv","PU Medium jets fraction wrt N_{PV}");
	hFracPuMediumJets_Npv->SetXTitle("N_{PV}");
	hFracPuMediumJets_Npv->SetYTitle("PU Medium jets fraction");
	cNjets_Npv->Clear();
	hFracPuMediumJets_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/FracPuMediumJets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuTightJets as a function of Npv
//
//************************************************************************************************************

	h1_style(hNjet_Npv);
	h1_style(hNpuTightjet_Npv);
	
	cNjets_Npv->cd();
	cNjets_Npv->Clear();
	hNjet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/Njets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	cNjets_Npv->Clear();
	hNpuTightjet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/NpuTightjets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
	TH1F *hFracPuTightJets_Npv = new TH1F("hFracPuTightJets_Npv","hFracPuTightJets_Npv",250,0,2500);
	h1_style(hFracPuTightJets_Npv);
	hFracPuTightJets_Npv = (TH1F*)hNpuTightjet_Npv->Clone();
	hFracPuTightJets_Npv->Divide(hNjet_Npv);
	hFracPuTightJets_Npv->SetNameTitle("hFracPuJets_Npv","PU Tight jets fraction wrt N_{PV}");
	hFracPuTightJets_Npv->SetXTitle("N_{PV}");
	hFracPuTightJets_Npv->SetYTitle("PU Tight jets fraction");
	cNjets_Npv->Clear();
	hFracPuTightJets_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/FracPuTightJets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      Njet and NpuAllJets as a function of Npv
//
//************************************************************************************************************

	h1_style(hNjet_Npv);
	h1_style(hNpuAlljet_Npv);
	
	cNjets_Npv->cd();
	cNjets_Npv->Clear();
	hNjet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/Njets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	cNjets_Npv->Clear();
	hNpuAlljet_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/NpuAlljets_Npv" + typeName + extension;
	cNjets_Npv->SaveAs(saveName.c_str());
	
	TH1F *hFracPuAllJets_Npv = new TH1F("hFracPuAllJets_Npv","hFracPuAllJets_Npv",250,0,2500);
	h1_style(hFracPuAllJets_Npv);
	hFracPuAllJets_Npv = (TH1F*)hNpuAlljet_Npv->Clone();
	hFracPuAllJets_Npv->Divide(hNjet_Npv);
	hFracPuAllJets_Npv->SetNameTitle("hFracPuJets_Npv","PU All jets fraction wrt N_{PV}");
	hFracPuAllJets_Npv->SetXTitle("N_{PV}");
	hFracPuAllJets_Npv->SetYTitle("PU All jets fraction");
	cNjets_Npv->Clear();
	hFracPuAllJets_Npv->Draw("hist");
	cNjets_Npv->SetLogy();
	saveName = "images/Njets/FracPuAllJets_Npv" + typeName + extension;
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
//                                      MJB as a function of recoileta
//
//************************************************************************************************************	

	float aMJB_RecoilEta_Mean[numberEtaBins];
	float aMJB_RecoilEta_MeanError[numberEtaBins];
	float aRecoilEtaBins_Mean[numberEtaBins];
	float aRecoilEtaBins_MeanError[numberEtaBins];
	
	for(int i=0; i<numberEtaBins; i++) {
		aMJB_RecoilEta_Mean[i] = vMJB_RecoilEta[i]->GetMean();
		aMJB_RecoilEta_MeanError[i] = vMJB_RecoilEta[i]->GetMeanError();
		aRecoilEtaBins_Mean[i] = ( myEtaBinning.getBinValueInf(i)+myEtaBinning.getBinValueSup(i) )/2.;
		aRecoilEtaBins_MeanError[i]=0.;
	}
	
	TCanvas *cMJB_RecoilEta = new TCanvas("cMJB_RecoilEta","cMJB_RecoilEta");
	cMJB_RecoilEta->cd();
	
	TGraphErrors *gMJB_RecoilEta = new TGraphErrors(numberEtaBins,aRecoilEtaBins_Mean, aMJB_RecoilEta_Mean, aRecoilEtaBins_MeanError, aMJB_RecoilEta_MeanError);
	gMJB_RecoilEta->SetName("MJB");
	gMJB_RecoilEta->SetTitle("MJB as a function of |#eta^{Recoil}|");
	gMJB_RecoilEta->GetXaxis()->SetTitle("|#eta^{Recoil}|");
	gMJB_RecoilEta->GetYaxis()->SetTitle("MJB");
	gMJB_RecoilEta->SetMarkerStyle(20);
	gMJB_RecoilEta->SetMarkerColor(plotColor);
	gMJB_RecoilEta->SetLineColor(plotColor);
	//gMJB_RecoilEta->SetMarkerSize(0.5);	
	cMJB_RecoilEta->cd();
	//gMJB_RecoilEta->SetLogx(1);
	//gMJB_RecoilEta->GetYaxis()->SetRangeUser(0.7,1.1);
	gMJB_RecoilEta->Draw("ape");
	TGraph_style (gMJB_RecoilEta);
	saveName = "images/MJB/cMJB_RecoilEta" + typeName + extension;
	cMJB_RecoilEta->SaveAs(saveName.c_str());
	
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
	
//************************************************************************************************************
//
//                                      NjetsRecoil as a function of recoilpt
//
//************************************************************************************************************	

	float aNjetsRecoil_Mean[numberPtBins];
	float aNjetsRecoil_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aNjetsRecoil_Mean[i] = vNjetsRecoil_RecoilPt[i]->GetMean();
		aNjetsRecoil_MeanError[i] = vNjetsRecoil_RecoilPt[i]->GetMeanError();
	}
	
	TCanvas *cNjetsRecoil_RecoilPt = new TCanvas("cNjetsRecoil_RecoilPt","cNjetsRecoil_RecoilPt");
	cNjetsRecoil_RecoilPt->cd();
	
	TGraphErrors *gNjetsRecoil_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aNjetsRecoil_Mean, aRecoilPtBins_MeanError, aNjetsRecoil_MeanError);
	gNjetsRecoil_RecoilPt->SetName("NjetsRecoil");
	gNjetsRecoil_RecoilPt->SetTitle("Number of jets in the recoil as a function of p_{T}^{Recoil}");
	gNjetsRecoil_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gNjetsRecoil_RecoilPt->GetYaxis()->SetTitle("N_{Recoil jets}");
	gNjetsRecoil_RecoilPt->SetMarkerStyle(20);
	gNjetsRecoil_RecoilPt->SetMarkerColor(plotColor);
	gNjetsRecoil_RecoilPt->SetLineColor(plotColor);
	//gNjetsRecoil_RecoilPt->SetMarkerSize(0.5);	
	cNjetsRecoil_RecoilPt->cd();
	//gNjetsRecoil_RecoilPt->SetLogx(1);
	//gNjetsRecoil_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gNjetsRecoil_RecoilPt->Draw("ape");
	TGraph_style (gNjetsRecoil_RecoilPt);
	saveName = "images/NjetsRecoil/cNjetsRecoil_RecoilPt" + typeName + extension;
	cNjetsRecoil_RecoilPt->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      NjetsRecoil_068E as a function of recoilpt
//
//************************************************************************************************************	

	float aNjetsRecoil_068E_Mean[numberPtBins];
	float aNjetsRecoil_068E_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aNjetsRecoil_068E_Mean[i] = vNjetsRecoil_068E_RecoilPt[i]->GetMean();
		aNjetsRecoil_068E_MeanError[i] = vNjetsRecoil_068E_RecoilPt[i]->GetMeanError();
	}
	
	TCanvas *cNjetsRecoil_068E_RecoilPt = new TCanvas("cNjetsRecoil_068E_RecoilPt","cNjetsRecoil_068E_RecoilPt");
	cNjetsRecoil_068E_RecoilPt->cd();
	
	TGraphErrors *gNjetsRecoil_068E_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aNjetsRecoil_068E_Mean, aRecoilPtBins_MeanError, aNjetsRecoil_068E_MeanError);
	gNjetsRecoil_068E_RecoilPt->SetName("NjetsRecoil_068E");
	gNjetsRecoil_068E_RecoilPt->SetTitle("Number of necessary recoil jets to carry 68% of p_{T}^{Recoil} as a function of p_{T}^{Recoil}");
	gNjetsRecoil_068E_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gNjetsRecoil_068E_RecoilPt->GetYaxis()->SetTitle("N_{Recoil jets}");
	gNjetsRecoil_068E_RecoilPt->SetMarkerStyle(20);
	gNjetsRecoil_068E_RecoilPt->SetMarkerColor(plotColor);
	gNjetsRecoil_068E_RecoilPt->SetLineColor(plotColor);
	//gNjetsRecoil_068E_RecoilPt->SetMarkerSize(0.5);	
	cNjetsRecoil_068E_RecoilPt->cd();
	//gNjetsRecoil_068E_RecoilPt->SetLogx(1);
	//gNjetsRecoil_068E_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gNjetsRecoil_068E_RecoilPt->Draw("ape");
	TGraph_style (gNjetsRecoil_068E_RecoilPt);
	saveName = "images/NjetsRecoil/cNjetsRecoil_068E_RecoilPt" + typeName + extension;
	cNjetsRecoil_068E_RecoilPt->SaveAs(saveName.c_str());
	
//************************************************************************************************************
//
//                                      NjetsRecoil_095E as a function of recoilpt
//
//************************************************************************************************************	

	float aNjetsRecoil_095E_Mean[numberPtBins];
	float aNjetsRecoil_095E_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aNjetsRecoil_095E_Mean[i] = vNjetsRecoil_095E_RecoilPt[i]->GetMean();
		aNjetsRecoil_095E_MeanError[i] = vNjetsRecoil_095E_RecoilPt[i]->GetMeanError();
	}
	
	TCanvas *cNjetsRecoil_095E_RecoilPt = new TCanvas("cNjetsRecoil_095E_RecoilPt","cNjetsRecoil_095E_RecoilPt");
	cNjetsRecoil_095E_RecoilPt->cd();
	
	TGraphErrors *gNjetsRecoil_095E_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aNjetsRecoil_095E_Mean, aRecoilPtBins_MeanError, aNjetsRecoil_095E_MeanError);
	gNjetsRecoil_095E_RecoilPt->SetName("NjetsRecoil_095E");
	gNjetsRecoil_095E_RecoilPt->SetTitle("Number of necessary recoil jets to carry 95% of p_{T}^{Recoil} as a function of p_{T}^{Recoil}");
	gNjetsRecoil_095E_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gNjetsRecoil_095E_RecoilPt->GetYaxis()->SetTitle("N_{Recoil jets}");
	gNjetsRecoil_095E_RecoilPt->SetMarkerStyle(20);
	gNjetsRecoil_095E_RecoilPt->SetMarkerColor(plotColor);
	gNjetsRecoil_095E_RecoilPt->SetLineColor(plotColor);
	//gNjetsRecoil_095E_RecoilPt->SetMarkerSize(0.5);	
	cNjetsRecoil_095E_RecoilPt->cd();
	//gNjetsRecoil_095E_RecoilPt->SetLogx(1);
	//gNjetsRecoil_095E_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gNjetsRecoil_095E_RecoilPt->Draw("ape");
	TGraph_style (gNjetsRecoil_095E_RecoilPt);
	saveName = "images/NjetsRecoil/cNjetsRecoil_095E_RecoilPt" + typeName + extension;
	cNjetsRecoil_095E_RecoilPt->SaveAs(saveName.c_str());
	
	
	TGraphErrors *gRtrue_leadingJet_RecoilPt = NULL;
	TGraphErrors *gRtrue_allJets_JetPt = NULL;
	TGraphErrors *gRrecoil_RecoilPt = NULL;
	
	if(isMC) {
//************************************************************************************************************
//
//                                      Rtrue,leading jet as a function of recoilpt
//
//************************************************************************************************************	

	
		float aRtrue_leadingJet_Mean[numberPtBins];
		float aRtrue_leadingJet_MeanError[numberPtBins];
	
		for(int i=0; i<numberPtBins; i++) {
			aRtrue_leadingJet_Mean[i] = vRtrue_leadingJet_RecoilPt[i]->GetMean();
			aRtrue_leadingJet_MeanError[i] = vRtrue_leadingJet_RecoilPt[i]->GetMeanError();
		}
	
		TCanvas *cRtrue_leadingJet_RecoilPt = new TCanvas("cRtrue_leadingJet_RecoilPt","cRtrue_leadingJet_RecoilPt");
		cRtrue_leadingJet_RecoilPt->cd();
	
		gRtrue_leadingJet_RecoilPt = new TGraphErrors(numberPtBins,aRecoilPtBins_Mean, aRtrue_leadingJet_Mean, aRecoilPtBins_MeanError, aRtrue_leadingJet_MeanError);
		gRtrue_leadingJet_RecoilPt->SetName("Rtrue");
		gRtrue_leadingJet_RecoilPt->SetTitle("R_{true} as a function of p_{T}^{Recoil}");
		gRtrue_leadingJet_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
		gRtrue_leadingJet_RecoilPt->GetYaxis()->SetTitle("R_{true}");
		gRtrue_leadingJet_RecoilPt->SetMarkerStyle(20);
		gRtrue_leadingJet_RecoilPt->SetMarkerColor(plotColor);
		gRtrue_leadingJet_RecoilPt->SetLineColor(plotColor);
		//gRtrue_leadingJet_RecoilPt->SetMarkerSize(0.5);	
		cRtrue_leadingJet_RecoilPt->cd();
		//gRtrue_leadingJet_RecoilPt->SetLogx(1);
		//gRtrue_leadingJet_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
		gRtrue_leadingJet_RecoilPt->Draw("ape");
		TGraph_style (gRtrue_leadingJet_RecoilPt);
		saveName = "images/Rtrue/cRtrue_leadingJet_RecoilPt" + typeName + extension;
		cRtrue_leadingJet_RecoilPt->SaveAs(saveName.c_str());

//************************************************************************************************************
//
//                                      Rtrue,all jets as a function of jetpt
//
//************************************************************************************************************	

	
		float aRtrue_allJets_Mean[myLowPtBinning.getSize()];
		float aRtrue_allJets_MeanError[myLowPtBinning.getSize()];		
		float aJetPtBins_Mean[myLowPtBinning.getSize()];
		float aJetPtBins_MeanError[myLowPtBinning.getSize()];

		for(int i=0; i<myLowPtBinning.getSize(); i++) {
			aRtrue_allJets_Mean[i] = vRtrue_allJets_JetPt[i]->GetMean();
			aRtrue_allJets_MeanError[i] = vRtrue_allJets_JetPt[i]->GetMeanError();
			aJetPtBins_Mean[i] = ( myLowPtBinning.getBinValueInf(i)+myLowPtBinning.getBinValueSup(i) )/2.;
			aJetPtBins_MeanError[i]=0.;
		}
	
		TCanvas *cRtrue_allJets_JetPt = new TCanvas("cRtrue_allJets_JetPt","cRtrue_allJets_JetPt");
		cRtrue_allJets_JetPt->cd();
	
		gRtrue_allJets_JetPt = new TGraphErrors(myLowPtBinning.getSize(),aJetPtBins_Mean, aRtrue_allJets_Mean, aJetPtBins_MeanError, aRtrue_allJets_MeanError);
		gRtrue_allJets_JetPt->SetName("Rtrue");
		gRtrue_allJets_JetPt->SetTitle("R_{true} as a function of p_{T,reco}^{jet}");
		gRtrue_allJets_JetPt->GetXaxis()->SetTitle("p_{T,reco}^{jet} (GeV)");
		gRtrue_allJets_JetPt->GetYaxis()->SetTitle("R_{true}");
		gRtrue_allJets_JetPt->SetMarkerStyle(20);
		gRtrue_allJets_JetPt->SetMarkerColor(plotColor);
		gRtrue_allJets_JetPt->SetLineColor(plotColor);
		//gRtrue_allJets_JetPt->SetMarkerSize(0.5);	
		cRtrue_allJets_JetPt->cd();
		//gRtrue_allJets_JetPt->SetLogx(1);
		//gRtrue_allJets_JetPt->GetYaxis()->SetRangeUser(0.7,1.1);
		gRtrue_allJets_JetPt->Draw("ape");
		TGraph_style (gRtrue_allJets_JetPt);
		saveName = "images/Rtrue/cRtrue_allJets_JetPt" + typeName + extension;
		cRtrue_allJets_JetPt->SaveAs(saveName.c_str());	

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
	TDirectory *ptbinDir = mjbDir->mkdir("recoilPtBin","recoilPtBin");
	ptbinDir->cd();
	gMJB_RecoilPt->Write("gMJB_RecoilPt");
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Write();
	}
	
	TDirectory *etabinDir = mjbDir->mkdir("recoilEtaBin","recoilEtaBin");
	etabinDir->cd();
	gMJB_RecoilEta->Write("gMJB_RecoilEta");
	for(int j=0; j<myEtaBinning.getSize(); j++) {
		vMJB_RecoilEta[j]->Write();
	}

	TDirectory *npvbinDir = mjbDir->mkdir("npvBin","npvBin");
	npvbinDir->cd();	
	gMJB_Npv->Write("gMJB_Npv");	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Write();
	}
	
	TDirectory *mpfDir = out->mkdir("MPF","MPF");
	mpfDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RecoilPt[j]->Write();
	}
	gMPF_RecoilPt->Write("gMPF_RecoilPt");	

  TDirectory *leadingJetDir = out->mkdir("leadingJet","leadingJet");
	leadingJetDir->cd();
	TDirectory *ptbin_jet1Dir = leadingJetDir->mkdir("recoilPtBin","recoilPtBin");
	ptbin_jet1Dir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vLeadingJetPt_RecoilPt[j]->Write();
	}
  TDirectory *leadingjetrawptbin_jet1Dir = leadingJetDir->mkdir("1stJetPtHLTBin", "1stJetPtHLTBin");
  leadingjetrawptbin_jet1Dir->cd();
  for(int j=0; j<myHLTPtBinning.getSize(); j++) {
    vLeadingJetPt_LeadingJetPtHLT[j]->Write();
  }

	if(isMC) {
		TDirectory *trueDir = out->mkdir("Rtrue","Rtrue");
		trueDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_leadingJet_RecoilPt[j]->Write();
			vRrecoil_RecoilPt[j]->Write();
		}
		gRtrue_leadingJet_RecoilPt->Write("gRtrue_leadingJet_RecoilPt");
		
		for(int j=0; j<myLowPtBinning.getSize(); j++) {
			vRtrue_allJets_JetPt[j]->Write();
		}
		gRtrue_allJets_JetPt->Write("gRtrue_allJets_JetPt");
	
		TDirectory *ptratioDir = out->mkdir("PtRatio","PtRatio");
		ptratioDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Write();
		}
		gPtRatio_GenPt->Write("gPtRatio_GenPt");
	}	

	TDirectory *recoilJetsDir = out->mkdir("recoilJets","recoilJets");
	recoilJetsDir->cd();	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vNjetsRecoil_RecoilPt[j]->Write();
		vNjetsRecoil_068E_RecoilPt[j]->Write();
		vNjetsRecoil_095E_RecoilPt[j]->Write();
	}


	TDirectory *recoilDir = out->mkdir("recoil","recoil");
	recoilDir->cd();	
	hRecoilEta->Write();
	hRecoilWidth->Write();
	TDirectory *ptbin_recoilDir = recoilDir->mkdir("recoilPtBin","recoilPtBin");
	ptbin_recoilDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vRecoilPt_RecoilPt[j]->Write();
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
  hJetsPt_beforeSel->Write();
  hJetsEta_beforeSel->Write();
  hJetsPhi_beforeSel->Write();
  hHT_beforeSel->Write();
  hNjets_ptSup25_etaInf5_beforeSel->Write();
	
	TDirectory *afterSelDir = variablesDir->mkdir("afterSel","afterSel");
	afterSelDir->cd();
	hFracPuLooseJets_JetPt->Write();
	hFracPuLooseJets_Npv->Write();
	hFracPuMediumJets_JetPt->Write();
	hFracPuMediumJets_Npv->Write();
	hFracPuTightJets_JetPt->Write();
	hFracPuTightJets_Npv->Write();
	hFracPuAllJets_JetPt->Write();
	hFracPuAllJets_Npv->Write();
	hNrmPuJets_JetPt->Write();
	hFracRmPuJets_JetPt->Write("hFracRmPuJets_JetPt");
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
	hNpuLoosejet_JetPt->Write();
	hNjet_Npv->Write();
	hNpuLoosejet_Npv->Write();
	hHT_afterSel->Write();
	hFracJetsPt->Write();
    hJetsPt_afterSel->Write();
    hJetsEta_afterSel->Write();
    hJetsPhi_afterSel->Write();
    hDeltaPhi_METRecoil_afterSel->Write();
    hDeltaPhi_METJet1_afterSel->Write();
	
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
	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vNjetsRecoil_RecoilPt[j]->Delete();
		vNjetsRecoil_068E_RecoilPt[j]->Delete();
		vNjetsRecoil_095E_RecoilPt[j]->Delete();
	}	
	
	if(isMC) {
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_leadingJet_RecoilPt[j]->Delete();
			vRrecoil_RecoilPt[j]->Delete();
		}
		for(int j=0; j<myLowPtBinning.getSize(); j++) {
			vRtrue_allJets_JetPt[j]->Delete();
		}
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Delete();
		}
	}
	hFracPuLooseJets_JetPt->Delete();
	hFracPuLooseJets_Npv->Delete();
	hFracPuMediumJets_JetPt->Delete();
	hFracPuMediumJets_Npv->Delete();
	hFracPuTightJets_JetPt->Delete();
	hFracPuTightJets_Npv->Delete();
	hFracPuAllJets_JetPt->Delete();
	hFracPuAllJets_Npv->Delete();
	hMJB_inclusive->Delete();
	hNjet_JetPt->Delete();
	hNpuLoosejet_JetPt->Delete();
	hNjet_Npv->Delete();
	hNpuLoosejet_Npv->Delete();
	hHT_afterSel->Delete();
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
	hRecoilEta->Delete();
	hRecoilWidth->Delete();
	hNrmPuJets_JetPt->Delete();
	hFracRmPuJets_JetPt->Delete();
	
	return 0;
}









