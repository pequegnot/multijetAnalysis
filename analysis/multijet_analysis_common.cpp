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
#include "../common/logPtPrimeBinning.h"
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
	string plotName = "";
	std::vector<std::string> inputFiles;
	string outputName;
	string inputName;
	string inputName_mc;
	string inputName_pt10;
	bool rmPU = false;		
    bool useRecoilPtBin = true;
    bool useRecoilPtHLTBin = false;
    bool useRecoilPtBinFrom10File = false;


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
    TCLAP::ValueArg<std::string> inputFileMCArg("", "input-file-mc", "The corresponding mc input file", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> inputFilePt10Arg("", "input-file-pt10", "The corresponding input file with recoil built with jets of pt>10 GeV", false, "", "string", cmd);
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

    TCLAP::SwitchArg recoilPtBinFrom10FileArg("", "recoilPtBinFrom10File", "For MPF, do you want to use recoil pt binning from file with recoil build with jets of pt>10 GeV?", false);
    cmd.add(recoilPtBinFrom10FileArg);

    TCLAP::SwitchArg recoilPtBinArg("", "recoilPtBin", "Do you want to bin in recoil pt?", false);
    TCLAP::SwitchArg firstJetPtBinArg("", "firstJetPtBin", "Do you want to bin in 1st jet pt?", false);
    cmd.xorAdd(recoilPtBinArg, firstJetPtBinArg);

    TCLAP::SwitchArg recoilPtHLTBinArg("", "recoilPtHLTBin", "Do you want to bin in recoil pt for HLT trigger?", false);
    TCLAP::SwitchArg firstJetPtHLTBinArg("", "firstJetPtHLTBin", "Do you want to bin in 1st jet pt for HLT trigger?", false);
    cmd.xorAdd(recoilPtHLTBinArg, firstJetPtHLTBinArg);
    
    // Parse the argv array.
    cmd.parse(argc, argv);
    
    // Get the value parsed by each arg.
    isMC = mcArg.getValue();
    outputName = outputFileArg.getValue();
    inputName = inputFileArg.getValue();
    inputName_mc = inputFileMCArg.getValue();
    inputName_pt10 = inputFilePt10Arg.getValue();
    extension = plotNameArg.getValue() + extensionArg.getValue();
    plotName = plotNameArg.getValue();
    rmPU = rmPUArg.getValue();
    useRecoilPtBinFrom10File = recoilPtBinFrom10FileArg.getValue();
    useRecoilPtBin = recoilPtBinArg.getValue();
    useRecoilPtHLTBin = recoilPtHLTBinArg.getValue();
    
    if(rmPU) {
	extension = "_woPUJets" + plotNameArg.getValue() + extensionArg.getValue();    
	plotName = "_woPUJets" + plotNameArg.getValue();    
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

  bool foundCorrespondingMCFile = false;
  TFile *f_mc=NULL;
    
  if (!isMC) {
    if(!inputName_mc.empty()) {
      foundCorrespondingMCFile = true;
      std::cout << "Found MC file corresponding to data file for low statistics bins..." << std::endl;
      f_mc=TFile::Open(inputName_mc.c_str());
    } else {
      std::cout << "WARNING! No MC file corresponding to data file for low statistics bins!" << std::endl;
    }
  }
    
  TFile *f=TFile::Open(inputName.c_str()); 
  TH1::SetDefaultSumw2(true);
  	

//*********************************************************************************************************
//
//                                      histograms definition
//
//*********************************************************************************************************

  ptBinning myPtBinning;
  logPtPrimeBinning myLogPtPrimeBinning;
  HLTPtBinning myHLTPtBinning;
  npvBinning myNpvBinning;
  ptBinning myLowPtBinning(true);
  etaBinning myEtaBinning;
  myHLTPtBinning.fillHLTPtBins(useRecoilPtHLTBin);  

  int numberPtBins = myPtBinning.getSize();
  int numberLogPtPrimeBins = myLogPtPrimeBinning.getSize();
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
	vector<TH1F*> vMJB_RefObjPtBin;
	vMJB_RefObjPtBin.resize(numberPtBins);

	//MPF per recoilpt
	vector<TH1F*> vMPF_RefObjPtBin;
	vMPF_RefObjPtBin.resize(numberPtBins);

	//MJB per recoilpt
	vector<TH1F*> vMJB_mc_RefObjPtBin;
	vMJB_mc_RefObjPtBin.resize(numberPtBins);

	//MPF per recoilpt
	vector<TH1F*> vMPF_mc_RefObjPtBin;
	vMPF_mc_RefObjPtBin.resize(numberPtBins);

	vector<TH1F*> vMJB_gen_RefObjPtBin;
	vMJB_gen_RefObjPtBin.resize(numberPtBins);

	//MPF per recoilpt
	vector<TH1F*> vMPF_gen_RefObjPtBin;
	vMPF_gen_RefObjPtBin.resize(numberPtBins);

	vector<TH1F*> vMPF_corr_RefObjPtBin;
	vMPF_corr_RefObjPtBin.resize(numberPtBins);

	vector<TH1F*> vMPF_corr_mc_RefObjPtBin;
	vMPF_corr_mc_RefObjPtBin.resize(numberPtBins);

	//leadingJetPt per recoilpt
	vector<TH1F*> vLeadingJetPt_RefObjPtBin;
	vLeadingJetPt_RefObjPtBin.resize(numberPtBins);

  //leadingJetPt per recoilpt
	vector<TH1F*> vLeadingJetPt_HLTRefObjPtBin;
	vLeadingJetPt_HLTRefObjPtBin.resize(numberHLTPtBins);

 	//RecoilPt per recoilpt
	vector<TH1F*> vRecoilPt_RefObjPtBin;
	vRecoilPt_RefObjPtBin.resize(numberPtBins);

  //MJB per recoileta
	vector<TH1F*> vMJB_RefObjEtaBin;
  vMJB_RefObjEtaBin.resize(numberEtaBins);

  //NjetsRecoil per recoilpt
  vector<TH1F*> vNjetsRecoil_RecoilPt;
  vNjetsRecoil_RecoilPt.resize(numberPtBins);
  vector<TH1F*> vNjetsRecoil_068E_RecoilPt;
  vNjetsRecoil_068E_RecoilPt.resize(numberPtBins);
  vector<TH1F*> vNjetsRecoil_095E_RecoilPt;
  vNjetsRecoil_095E_RecoilPt.resize(numberPtBins);
  vector<TH1F*> vMeanLogPt_RecoilPt;
  vMeanLogPt_RecoilPt.resize(numberPtBins);
  vector<TH1F*> vExp_sum_Fi_log_fi_RecoilPt;
  vExp_sum_Fi_log_fi_RecoilPt.resize(numberPtBins);
  vector<TH1F*> vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt;
  vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt.resize(numberPtBins);
  vector<TH1F*> vMJB_LogPtPrimeBin;
  vMJB_LogPtPrimeBin.resize(numberLogPtPrimeBins);
  vector<TH1F*> vInvMJB_LogPtPrimeBin;
  vInvMJB_LogPtPrimeBin.resize(numberLogPtPrimeBins);

  for (int j=0; j<numberLogPtPrimeBins; j++) {
		ptBinName = myLogPtPrimeBinning.getName(j);
    vectorName = "MJB/LogPtPrimeBin/MJB_" + ptBinName;
    vMJB_LogPtPrimeBin[j] = (TH1F*)f->Get(vectorName.c_str());
    vectorName = "MJB/LogPtPrimeBin/InvMJB_" + ptBinName;
    vInvMJB_LogPtPrimeBin[j] = (TH1F*)f->Get(vectorName.c_str());
  }

	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		ptBinName = myPtBinning.getName(j);
    vectorName = "MJB/PtBin/MJB_" + ptBinName;
    vMJB_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
    if (!isMC && foundCorrespondingMCFile) {
      vMJB_mc_RefObjPtBin[j] = (TH1F*)f_mc->Get(vectorName.c_str());
    }
    vectorName = "MPF/PtBin/MPF_" + ptBinName;
    vMPF_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
    if (!isMC && foundCorrespondingMCFile) {
      vMPF_mc_RefObjPtBin[j] = (TH1F*)f_mc->Get(vectorName.c_str());
    }
    vectorName = "MJB_gen/PtBin/MJB_gen_" + ptBinName;
    vMJB_gen_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
    vectorName = "MPF_gen/PtBin/MPF_gen_" + ptBinName;
    vMPF_gen_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
    vectorName = "MPF/PtBin/MPF_corr_" + ptBinName;
    vMPF_corr_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
    if (!isMC && foundCorrespondingMCFile) {
      vMPF_corr_mc_RefObjPtBin[j] = (TH1F*)f_mc->Get(vectorName.c_str());
    }
    vectorName = "leadingJet/PtBin/LeadingJetPt_" + ptBinName;
    vLeadingJetPt_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
    vectorName = "recoil/PtBin/RecoilPt_" + ptBinName;
    //vectorName = "recoil/recoilPtBin/RecoilPt_" + ptBinName;
    vRecoilPt_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
	}

  for(int j=0; j<myHLTPtBinning.getSize(); j++) {
    ptBinName = myHLTPtBinning.getName(j);
    vectorName = "leadingJet/HLTPtBin/LeadingJetPt_perHLTTrigger_" + ptBinName;
    vLeadingJetPt_HLTRefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
	}

	for(int j=0; j<myEtaBinning.getSize(); j++) {
		etaBinName = myEtaBinning.getName(j);
		vectorName = "MJB/EtaBin/MJB_" + etaBinName;
		vMJB_RefObjEtaBin[j] = (TH1F*)f->Get(vectorName.c_str());
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
		vector<TH1F*> vRtrue_leadingJet_RefObjPt;
		vRtrue_leadingJet_RefObjPt.resize(numberPtBins);
		
		vector<TH1F*> vRtrue_allJets_JetPt;
		vRtrue_allJets_JetPt.resize(myLowPtBinning.getSize());
	
		//Rrecoil per reference object pt
		vector<TH1F*> vRrecoil_RefObjPtBin;
		vRrecoil_RefObjPtBin.resize(numberPtBins);
		
		for(int j=0; j<myPtBinning.getSize(); j++) {
			ptBinName = myPtBinning.getName(j);
			vectorName = "Rtrue/Rtrue_leadingJet_" + ptBinName;
			vRtrue_leadingJet_RefObjPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "PtRatio/PtRatio_" + ptBinName;			
			vPtRatio_GenPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "Rtrue/Rrecoil_" + ptBinName;
			vRrecoil_RefObjPtBin[j] = (TH1F*)f->Get(vectorName.c_str());
			
			vectorName = "recoilJets/NjetsRecoil_" + ptBinName;
			vNjetsRecoil_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
			vectorName = "recoilJets/NjetsRecoil_068E_" + ptBinName;
      vNjetsRecoil_068E_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
      vectorName = "recoilJets/NjetsRecoil_095E_" + ptBinName;
      vNjetsRecoil_095E_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
      vectorName = "recoilJets/MeanLogPt_" + ptBinName;
      vMeanLogPt_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
      vectorName = "recoilJets/Exp_sum_Fi_log_fi_" + ptBinName;
      vExp_sum_Fi_log_fi_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
      vectorName = "recoilJets/Log_ptrecoil_exp_sum_Fi_log_fi_" + ptBinName;
      vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt[j] = (TH1F*)f->Get(vectorName.c_str());
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
    TH1F* hTrueNotPuTightJetPt = (TH1F*)f->Get("variables/afterSel/hTrueNotPuTightJetPt");
    TH1F* hOtherJetPt = (TH1F*)f->Get("variables/afterSel/hOtherJetPt");
	TH1F* hFracJetsPt = (TH1F*)f->Get("variables/afterSel/hFracJetsPt");
    TH1F* hHT_beforeSel = (TH1F*)f->Get("variables/beforeSel/hHT_beforeSel");
	TH1F* hHT_afterSel = (TH1F*)f->Get("variables/afterSel/hHT_afterSel");
	TH1F* hNjetsRecoil = (TH1F*)f->Get("variables/afterSel/hNjetsRecoil");
	TH1F* hNjetsTotal =  (TH1F*)f->Get("variables/afterSel/hNjetsTotal");	
    TH1F* hJetsPt_afterSel = (TH1F*)f->Get("variables/afterSel/hJetsPt_afterSel");
    TH1F* hJetsEta_afterSel = (TH1F*)f->Get("variables/afterSel/hJetsEta_afterSel");
    TH1F* hLeadingJetEta_afterSel = (TH1F*)f->Get("variables/afterSel/hLeadingJetEta_afterSel");
    TH1F* hRecoilEta_afterSel = (TH1F*)f->Get("variables/afterSel/hRecoilEta_afterSel");
    TH1F* hJetsPhi_afterSel = (TH1F*)f->Get("variables/afterSel/hJetsPhi_afterSel");

	TH1F* hMet_beforeSel = (TH1F*)f->Get("variables/beforeSel/hMet_beforeSel");
	TH1F* hLeadingJetPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hLeadingJetPt_beforeSel");	
	TH1F* hSecondJetPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hSecondJetPt_beforeSel");	
	TH1F* hRecoilPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hRecoilPt_beforeSel");	
	TH1F* hNpv_beforeSel = (TH1F*)f->Get("variables/beforeSel/hNpv_beforeSel");	
	TH1F* hAlpha_beforeSel = (TH1F*)f->Get("variables/beforeSel/hAlpha_beforeSel");
	TH1F* hBeta_beforeSel = (TH1F*)f->Get("variables/beforeSel/hBeta_beforeSel");	
	TH1F* hA_beforeSel = (TH1F*)f->Get("variables/beforeSel/hA_beforeSel");	
	TH1F* hRecoilJetsPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hRecoilJetsPt_beforeSel");	
    TH1F* hJetsPt_beforeSel = (TH1F*)f->Get("variables/beforeSel/hJetsPt_beforeSel");
    TH1F* hJetsEta_beforeSel = (TH1F*)f->Get("variables/beforeSel/hJetsEta_beforeSel");
    TH1F* hLeadingJetEta_beforeSel = (TH1F*)f->Get("variables/beforeSel/hLeadingJetEta_beforeSel");
    TH1F* hRecoilEta_beforeSel = (TH1F*)f->Get("variables/beforeSel/hRecoilEta_beforeSel");
    TH1F* hJetsPhi_beforeSel = (TH1F*)f->Get("variables/beforeSel/hJetsPhi_beforeSel");
    TH1F* hNjets_ptSup30_etaInf5_beforeSel = (TH1F*)f->Get("variables/beforeSel/hNjets_ptSup30_etaInf5_beforeSel");
		
	TH1F* hMet_afterSel = (TH1F*)f->Get("variables/afterSel/hMet_afterSel");	
	TH1F* hLeadingJetPt_afterSel = (TH1F*)f->Get("variables/afterSel/hLeadingJetPt_afterSel");		
	TH1F* hLeadingJetMass_afterSel = (TH1F*)f->Get("variables/afterSel/hLeadingJetMass_afterSel");		
	TH1F* hLeadingAndSecondJetsMass_afterSel = (TH1F*)f->Get("variables/afterSel/hLeadingAndSecondJetsMass_afterSel");		
	TH1F* hSecondJetPt_afterSel = (TH1F*)f->Get("variables/afterSel/hSecondJetPt_afterSel");		
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
//                                      NtrueNotPuTightJet and NotherJet as a function of ptjet
//
//************************************************************************************************************

    h1_style(hTrueNotPuTightJetPt);
    h1_style(hOtherJetPt);

    TCanvas *cTrueJet = new TCanvas();
    cTrueJet->cd();
    hTrueNotPuTightJetPt->SetFillColor(2);
    hOtherJetPt->SetFillColor(5);
    hTrueNotPuTightJetPt->SetLineColor(1);
    hOtherJetPt->SetLineColor(1);
    THStack* hsJetsPt = new THStack("hsJetsPt", "hsJetsPt");
    hsJetsPt->Add(hTrueNotPuTightJetPt);
    hsJetsPt->Add(hOtherJetPt);
    hsJetsPt->Draw("hist");
    hsJetsPt->GetXaxis()->SetTitle("Jet p_{t} [GeV]");
    hsJetsPt->GetXaxis()->SetRangeUser(0., 700.);
    TLegend *myLegend = new TLegend(0.74,0.70,0.88,0.88);
    myLegend->SetBorderSize(0);
	myLegend->AddEntry(hTrueNotPuTightJetPt,"True (not PU Tight) jets","f");
	myLegend->AddEntry(hOtherJetPt,"Others","f");
	myLegend->SetFillColor(0);
	myLegend->Draw("SAME");
    cTrueJet->SetLogy();
    saveName = "images/Njets/NTrueJet_jetPt" + typeName + extension;
    cTrueJet->SaveAs(saveName.c_str());

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
//                                      MJB as a function of reference object pt
//
//************************************************************************************************************

	float aMJB_RefObjPt_Mean[numberPtBins];
	float aMJB_RefObjPt_MeanError[numberPtBins];
	float aMJB_RefObjPt_RMS[numberPtBins];
	float aMJB_RefObjPt_RMSError[numberPtBins];
	float aRefObjPtBins_Mean[numberPtBins];
	float aRefObjPtBins_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aMJB_RefObjPt_Mean[i] = vMJB_RefObjPtBin[i]->GetMean();
		aMJB_RefObjPt_MeanError[i] = vMJB_RefObjPtBin[i]->GetMeanError();
    if (!isMC && foundCorrespondingMCFile) {
      if ( vMJB_RefObjPtBin[i]->GetEntries() < 10) {
        aMJB_RefObjPt_MeanError[i] = vMJB_mc_RefObjPtBin[i]->GetRMS()/sqrt(vMJB_RefObjPtBin[i]->GetEntries());
      }
    }
		aMJB_RefObjPt_RMS[i] = vMJB_RefObjPtBin[i]->GetRMS();
		aMJB_RefObjPt_RMSError[i] = vMJB_RefObjPtBin[i]->GetRMSError();
    if (vRecoilPt_RefObjPtBin[i]->GetEntries() != 0) {
      aRefObjPtBins_Mean[i] = vRecoilPt_RefObjPtBin[i]->GetMean();
      aRefObjPtBins_MeanError[i] = vRecoilPt_RefObjPtBin[i]->GetMeanError();
    } else {
      aRefObjPtBins_Mean[i] = ( myPtBinning.getBinValueInf(i)+myPtBinning.getBinValueSup(i) )/2.;
      aRefObjPtBins_MeanError[i]=0.;
    }
    std::cout << "aRefObjPtBins_Mean[" << i << "] = " << aRefObjPtBins_Mean[i] << ";" << std::endl;
    std::cout << "aRefObjPtBins_MeanError[" << i << "] = " << aRefObjPtBins_MeanError[i] << ";" << std::endl;
	}
	
	TCanvas *cMJB_RefObjPt = new TCanvas("cMJB_RefObjPt","cMJB_RefObjPt");
	cMJB_RefObjPt->cd();
	
	TGraphErrors *gMJB_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aMJB_RefObjPt_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_MeanError);
	gMJB_RefObjPt->SetName("MJB");
    if(useRecoilPtBin) {
	  gMJB_RefObjPt->SetTitle("MJB as a function of p_{T}^{Recoil}");
	  gMJB_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMJB_RefObjPt->SetTitle("MJB as a function of p_{T}^{Leading Jet}");
	  gMJB_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");    
    }
	gMJB_RefObjPt->GetYaxis()->SetTitle("MJB");
	gMJB_RefObjPt->SetMarkerStyle(20);
	gMJB_RefObjPt->SetMarkerColor(plotColor);
	gMJB_RefObjPt->SetLineColor(plotColor);
	//gMJB_RefObjPt->SetMarkerSize(0.5);	
	cMJB_RefObjPt->cd();
	//gMJB_RefObjPt->SetLogx(1);
	//gMJB_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMJB_RefObjPt->Draw("ape");
	TGraph_style (gMJB_RefObjPt);
	saveName = "images/MJB/cMJB_RefObjPt" + typeName + extension;
	cMJB_RefObjPt->SaveAs(saveName.c_str());

	TCanvas *cMJB_RMS_RefObjPt = new TCanvas("cMJB_RMS_RefObjPt","cMJB_RMS_RefObjPt");
	cMJB_RMS_RefObjPt->cd();

	TGraphErrors *gMJB_RMS_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aMJB_RefObjPt_RMS, aRefObjPtBins_MeanError, aMJB_RefObjPt_RMSError);
	gMJB_RMS_RefObjPt->SetName("MJB RMS");
    if(useRecoilPtBin) {
	  gMJB_RMS_RefObjPt->SetTitle("MJB RMS as a function of p_{T}^{Recoil}");
	  gMJB_RMS_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMJB_RMS_RefObjPt->SetTitle("MJB RMS as a function of p_{T}^{Leading Jet}");
	  gMJB_RMS_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");
    }
	gMJB_RMS_RefObjPt->GetYaxis()->SetTitle("MJB RMS");
	gMJB_RMS_RefObjPt->SetMarkerStyle(20);
	gMJB_RMS_RefObjPt->SetMarkerColor(plotColor);
	gMJB_RMS_RefObjPt->SetLineColor(plotColor);
	//gMJB_RMS_RefObjPt->SetMarkerSize(0.5);
	cMJB_RMS_RefObjPt->cd();
	//gMJB_RMS_RefObjPt->SetLogx(1);
	//gMJB_RMS_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMJB_RMS_RefObjPt->Draw("ape");
	TGraph_style (gMJB_RMS_RefObjPt);
	saveName = "images/MJB/cMJB_RMS_RefObjPt" + typeName + extension;
	cMJB_RMS_RefObjPt->SaveAs(saveName.c_str());

//************************************************************************************************************
//
//                                      MJB as a function of reference object pt resize
//
//************************************************************************************************************


  //Double_t aMJB_RefObjPt_data_Mean[numberPtBins-3];
  //Double_t aMJB_RefObjPt_data_MeanError[numberPtBins-3];
  //Double_t aRefObjPtBins_Mean_r[numberPtBins-3];
  //Double_t aRefObjPtBins_MeanError_r[numberPtBins-3];

  //for(int i=0; i<numberPtBins-3; i++) {
    //gMJB_RefObjPt->GetPoint(i,aRefObjPtBins_Mean_r[i],aMJB_RefObjPt_data_Mean[i]);
    //aMJB_RefObjPt_data_MeanError[i] = gMJB_RefObjPt->GetErrorY(i);
    //aRefObjPtBins_MeanError_r[i] = gMJB_RefObjPt->GetErrorX(i);
    //std::cout << "aRefObjPtBins_Mean_r[" << i<< "]: " << aRefObjPtBins_Mean_r[i] << std::endl;
    //std::cout << "aMJB_RefObjPt_data_Mean[" << i<< "]: " << aMJB_RefObjPt_data_Mean[i] << std::endl;
  //}

  //std::cout << "numberPtBins-3: " << numberPtBins-3 << std::endl;


	//TCanvas *cMJB_RefObjPt_resize = new TCanvas("cMJB_RefObjPt_resize","cMJB_RefObjPtresize");
	//cMJB_RefObjPt_resize->cd();

  //TGraphErrors* gMJB_RefObjPt_data_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean_r, aMJB_RefObjPt_data_Mean, aRefObjPtBins_MeanError_r, aMJB_RefObjPt_data_MeanError);
    //if(useRecoilPtBin) {
		//gMJB_RefObjPt_data_resize->SetTitle("MJB as a function of p_{T}^{Recoil}");
		//gMJB_RefObjPt_data_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    //}
    //else {
		//gMJB_RefObjPt_data_resize->SetTitle("MJB as a function of p_{T}^{Leading Jet}");
		//gMJB_RefObjPt_data_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");
    //}
	//gMJB_RefObjPt_data_resize->GetYaxis()->SetTitle("MJB");
	//gMJB_RefObjPt_data_resize->SetMarkerStyle(20);
	//gMJB_RefObjPt_data_resize->SetMarkerColor(plotColor);
	//gMJB_RefObjPt_data_resize->SetLineColor(plotColor);
	////gMJB_RefObjPt->SetMarkerSize(0.5);
	//cMJB_RefObjPt_resize->cd();
	////gMJB_RefObjPt->SetLogx(1);
	////gMJB_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	//gMJB_RefObjPt_data_resize->Draw("ape");
	//TGraph_style(gMJB_RefObjPt_data_resize);
	//saveName = "images/MJB/cMJB_RefObjPt_resize" + typeName + extension;
	/*cMJB_RefObjPt_resize->SaveAs(saveName.c_str());*/

//************************************************************************************************************
//
//                                      MPF as a function of ref obj pt
//
//************************************************************************************************************	

	float aMPF_RefObjPt_Mean[numberPtBins];
	float aMPF_RefObjPt_MeanError[numberPtBins];
	float aMPF_RefObjPt_RMS[numberPtBins];
	float aMPF_RefObjPt_RMSError[numberPtBins];

  // take <MPF> and <MJB> from the pt>30 GeV pTrecoil bins, but taking <pTrecoil> from pT>10 GeV bins for MPF

	float aRefObjPtBins_pt10_Mean[numberPtBins];
	float aRefObjPtBins_pt10_MeanError[numberPtBins];

  if(!useRecoilPtBinFrom10File) {
    for(int i=0; i<numberPtBins; i++) {
      aRefObjPtBins_pt10_Mean[i] = aRefObjPtBins_Mean[i];
      aRefObjPtBins_pt10_MeanError[i] = aRefObjPtBins_MeanError[i];
    }  
  } else {
    bool foundCorrespondingFilePt10 = false;

    if(!inputName_pt10.empty()) {
      foundCorrespondingFilePt10 = true;
      std::cout << "Found file corresponding to recoil build with jets of pt>10 GeV for MPF..." << std::endl;
    } else {
      std::cout << "WARNING! No file corresponding to recoil build with jets of pt>10 GeV for MPF!" << std::endl;
    }

    TFile *f_pt10=TFile::Open(inputName_pt10.c_str());
    //RecoilPt per recoilpt
    vector<TH1F*> vRecoilPt_RefObjPtBin_pt10;
    vRecoilPt_RefObjPtBin_pt10.resize(numberPtBins);
    for(int i=0; i<numberPtBins; i++) {
      ptBinName = myPtBinning.getName(i);
      vectorName = "recoil/PtBin/RecoilPt_" + ptBinName;
      vRecoilPt_RefObjPtBin_pt10[i] = (TH1F*)f_pt10->Get(vectorName.c_str());
      if (vRecoilPt_RefObjPtBin_pt10[i]->GetEntries() != 0) {
        aRefObjPtBins_pt10_Mean[i] = vRecoilPt_RefObjPtBin_pt10[i]->GetMean();
        aRefObjPtBins_pt10_MeanError[i] = vRecoilPt_RefObjPtBin_pt10[i]->GetMeanError();
      } else {
        aRefObjPtBins_pt10_Mean[i] = ( myPtBinning.getBinValueInf(i)+myPtBinning.getBinValueSup(i) )/2.;
        aRefObjPtBins_pt10_MeanError[i]=0.;
      }
      std::cout << "aRefObjPtBins_pt10_Mean[" << i << "] = " << aRefObjPtBins_pt10_Mean[i] << ";" << std::endl;
      std::cout << "aRefObjPtBins_pt10_MeanError[" << i << "] = " << aRefObjPtBins_pt10_MeanError[i] << ";" << std::endl;
    }
    f_pt10->Close();
    delete f_pt10;
  }



  // MC, recoil with pt>10 GeV
/*aRefObjPtBins_pt10_Mean[0] = 252.013;*/
//aRefObjPtBins_pt10_MeanError[0] = 0.0978411;
//aRefObjPtBins_pt10_Mean[1] = 321.073;
//aRefObjPtBins_pt10_MeanError[1] = 0.13993;
//aRefObjPtBins_pt10_Mean[2] = 408.214;
//aRefObjPtBins_pt10_MeanError[2] = 0.186919;
//aRefObjPtBins_pt10_Mean[3] = 503.637;
//aRefObjPtBins_pt10_MeanError[3] = 0.255315;
//aRefObjPtBins_pt10_Mean[4] = 576.323;
//aRefObjPtBins_pt10_MeanError[4] = 0.313399;
//aRefObjPtBins_pt10_Mean[5] = 648.337;
//aRefObjPtBins_pt10_MeanError[5] = 0.519465;
//aRefObjPtBins_pt10_Mean[6] = 742.164;
//aRefObjPtBins_pt10_MeanError[6] = 0.745105;
//aRefObjPtBins_pt10_Mean[7] = 843.55;
//aRefObjPtBins_pt10_MeanError[7] = 0.491264;
//aRefObjPtBins_pt10_Mean[8] = 943.576;
//aRefObjPtBins_pt10_MeanError[8] = 0.768502;
//aRefObjPtBins_pt10_Mean[9] = 1077.15;
//aRefObjPtBins_pt10_MeanError[9] = 1.49597;
//aRefObjPtBins_pt10_Mean[10] = 1305.16;
//aRefObjPtBins_pt10_MeanError[10] = 2.76811;
//aRefObjPtBins_pt10_Mean[11] = 1641.01;
//aRefObjPtBins_pt10_MeanError[11] = 10.4198;
//aRefObjPtBins_pt10_Mean[12] = 2157.91;
//aRefObjPtBins_pt10_MeanError[12] = 15.1143;
//aRefObjPtBins_pt10_Mean[13] = 2648.1;
/*aRefObjPtBins_pt10_MeanError[13] = 48.7645;*/


  // data, recoil with pt>10 GeV
/*aRefObjPtBins_pt10_Mean[0] = 237.89;*/
//aRefObjPtBins_pt10_MeanError[0] = 0.471197;
//aRefObjPtBins_pt10_Mean[1] = 320.101;
//aRefObjPtBins_pt10_MeanError[1] = 0.408879;
//aRefObjPtBins_pt10_Mean[2] = 407.892;
//aRefObjPtBins_pt10_MeanError[2] = 0.304381;
//aRefObjPtBins_pt10_Mean[3] = 502.62;
//aRefObjPtBins_pt10_MeanError[3] = 0.366145;
//aRefObjPtBins_pt10_Mean[4] = 576.874;
//aRefObjPtBins_pt10_MeanError[4] = 0.337929;
//aRefObjPtBins_pt10_Mean[5] = 648.224;
//aRefObjPtBins_pt10_MeanError[5] = 0.391936;
//aRefObjPtBins_pt10_Mean[6] = 743.382;
//aRefObjPtBins_pt10_MeanError[6] = 0.64765;
//aRefObjPtBins_pt10_Mean[7] = 844.611;
//aRefObjPtBins_pt10_MeanError[7] = 1.02579;
//aRefObjPtBins_pt10_Mean[8] = 944.43;
//aRefObjPtBins_pt10_MeanError[8] = 1.60885;
//aRefObjPtBins_pt10_Mean[9] = 1070.74;
//aRefObjPtBins_pt10_MeanError[9] = 3.30558;
//aRefObjPtBins_pt10_Mean[10] = 1301.13;
//aRefObjPtBins_pt10_MeanError[10] = 8.03067;
//aRefObjPtBins_pt10_Mean[11] = 1691.35;
//aRefObjPtBins_pt10_MeanError[11] = 30.9797;
//aRefObjPtBins_pt10_Mean[12] = 2250;
//aRefObjPtBins_pt10_MeanError[12] = 0;
//aRefObjPtBins_pt10_Mean[13] = 2750;
/*aRefObjPtBins_pt10_MeanError[13] = 0;*/

	
	for(int i=0; i<numberPtBins; i++) {
		aMPF_RefObjPt_Mean[i] = vMPF_RefObjPtBin[i]->GetMean();
		aMPF_RefObjPt_MeanError[i] = vMPF_RefObjPtBin[i]->GetMeanError();
    if (!isMC && foundCorrespondingMCFile) {
      if ( vMPF_RefObjPtBin[i]->GetEntries() < 10) {
        aMPF_RefObjPt_MeanError[i] = vMPF_mc_RefObjPtBin[i]->GetRMS()/sqrt(vMPF_RefObjPtBin[i]->GetEntries());
      }
    }
		aMPF_RefObjPt_RMS[i] = vMPF_RefObjPtBin[i]->GetRMS();
		aMPF_RefObjPt_RMSError[i] = vMPF_RefObjPtBin[i]->GetRMSError();
	}
	
	TCanvas *cMPF_RefObjPt = new TCanvas("cMPF_RefObjPt","cMPF_RefObjPt");
	cMPF_RefObjPt->cd();
	
	TGraphErrors *gMPF_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_pt10_Mean, aMPF_RefObjPt_Mean, aRefObjPtBins_pt10_MeanError, aMPF_RefObjPt_MeanError);
	gMPF_RefObjPt->SetName("MPF");
    if(useRecoilPtBin) {
	  gMPF_RefObjPt->SetTitle("MPF as a function of p_{T}^{Recoil}");
	  gMPF_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMPF_RefObjPt->SetTitle("MPF as a function of p_{T}^{Leading Jet}");
	  gMPF_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");    
    }
	gMPF_RefObjPt->GetYaxis()->SetTitle("MPF");
	gMPF_RefObjPt->SetMarkerStyle(20);
	gMPF_RefObjPt->SetMarkerColor(plotColor);
	gMPF_RefObjPt->SetLineColor(plotColor);
	//gMPF_RefObjPt->SetMarkerSize(0.5);	
	cMPF_RefObjPt->cd();
	//gMPF_RefObjPt->SetLogx(1);
	//gMPF_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMPF_RefObjPt->Draw("ape");
	TGraph_style (gMPF_RefObjPt);
	saveName = "images/MPF/cMPF_RefObjPt" + typeName + extension;
	cMPF_RefObjPt->SaveAs(saveName.c_str());

  TCanvas *cMPF_RMS_RefObjPt = new TCanvas("cMPF_RMS_RefObjPt","cMPF_RMS_RefObjPt");
	cMPF_RMS_RefObjPt->cd();

	TGraphErrors *gMPF_RMS_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_pt10_Mean, aMPF_RefObjPt_RMS, aRefObjPtBins_pt10_MeanError, aMPF_RefObjPt_RMSError);
	gMPF_RMS_RefObjPt->SetName("MPF RMS");
    if(useRecoilPtBin) {
	  gMPF_RMS_RefObjPt->SetTitle("MPF RMS as a function of p_{T}^{Recoil}");
	  gMPF_RMS_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMPF_RMS_RefObjPt->SetTitle("MPF RMS as a function of p_{T}^{Leading Jet}");
	  gMPF_RMS_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");
    }
	gMPF_RMS_RefObjPt->GetYaxis()->SetTitle("MPF RMS");
	gMPF_RMS_RefObjPt->SetMarkerStyle(20);
	gMPF_RMS_RefObjPt->SetMarkerColor(plotColor);
	gMPF_RMS_RefObjPt->SetLineColor(plotColor);
	//gMPF_RMS_RefObjPt->SetMarkerSize(0.5);
	cMPF_RMS_RefObjPt->cd();
	//gMPF_RMS_RefObjPt->SetLogx(1);
	//gMPF_RMS_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMPF_RMS_RefObjPt->Draw("ape");
	TGraph_style (gMPF_RMS_RefObjPt);
	saveName = "images/MPF/cMPF_RMS_RefObjPt" + typeName + extension;
	cMPF_RMS_RefObjPt->SaveAs(saveName.c_str());


  //************************************************************************************************************
//
//                                      MPF_corr as a function of ref obj pt
//
//************************************************************************************************************	

	float aMPF_corr_RefObjPt_Mean[numberPtBins];
	float aMPF_corr_RefObjPt_MeanError[numberPtBins];
	float aMPF_corr_RefObjPt_RMS[numberPtBins];
	float aMPF_corr_RefObjPt_RMSError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aMPF_corr_RefObjPt_Mean[i] = vMPF_corr_RefObjPtBin[i]->GetMean();
		aMPF_corr_RefObjPt_MeanError[i] = vMPF_corr_RefObjPtBin[i]->GetMeanError();
    if (!isMC && foundCorrespondingMCFile) {
      if ( vMPF_corr_RefObjPtBin[i]->GetEntries() < 10) {
        aMPF_corr_RefObjPt_MeanError[i] = vMPF_corr_mc_RefObjPtBin[i]->GetRMS()/sqrt(vMPF_corr_RefObjPtBin[i]->GetEntries());
      }
    }
		aMPF_corr_RefObjPt_RMS[i] = vMPF_corr_RefObjPtBin[i]->GetRMS();
		aMPF_corr_RefObjPt_RMSError[i] = vMPF_corr_RefObjPtBin[i]->GetRMSError();
	}
	
	TCanvas *cMPF_corr_RefObjPt = new TCanvas("cMPF_corr_RefObjPt","cMPF_corr_RefObjPt");
	cMPF_corr_RefObjPt->cd();
	
	TGraphErrors *gMPF_corr_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_pt10_Mean, aMPF_corr_RefObjPt_Mean, aRefObjPtBins_pt10_MeanError, aMPF_corr_RefObjPt_MeanError);
	gMPF_corr_RefObjPt->SetName("MPF corrected with PU");
    if(useRecoilPtBin) {
	  gMPF_corr_RefObjPt->SetTitle("MPF corrected with PU as a function of p_{T}^{Recoil}");
	  gMPF_corr_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMPF_corr_RefObjPt->SetTitle("MPF corrected with PU as a function of p_{T}^{Leading Jet}");
	  gMPF_corr_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");    
    }
	gMPF_corr_RefObjPt->GetYaxis()->SetTitle("MPF corrected with PU");
	gMPF_corr_RefObjPt->SetMarkerStyle(20);
	gMPF_corr_RefObjPt->SetMarkerColor(plotColor);
	gMPF_corr_RefObjPt->SetLineColor(plotColor);
	//gMPF_corr_RefObjPt->SetMarkerSize(0.5);	
	cMPF_corr_RefObjPt->cd();
	//gMPF_corr_RefObjPt->SetLogx(1);
	//gMPF_corr_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMPF_corr_RefObjPt->Draw("ape");
	TGraph_style (gMPF_corr_RefObjPt);
	saveName = "images/MPF/cMPF_corr_RefObjPt" + typeName + extension;
	cMPF_corr_RefObjPt->SaveAs(saveName.c_str());

	TCanvas *cMPF_RMS_corr_RefObjPt = new TCanvas("cMPF_RMS_corr_RefObjPt","cMPF_RMS_corr_RefObjPt");
	cMPF_RMS_corr_RefObjPt->cd();

	TGraphErrors *gMPF_RMS_corr_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_pt10_Mean, aMPF_corr_RefObjPt_RMS, aRefObjPtBins_pt10_MeanError, aMPF_corr_RefObjPt_RMSError);
	gMPF_RMS_corr_RefObjPt->SetName("MPF RMS corrected with PU");
    if(useRecoilPtBin) {
	  gMPF_RMS_corr_RefObjPt->SetTitle("MPF RMS corrected with PU as a function of p_{T}^{Recoil}");
	  gMPF_RMS_corr_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMPF_RMS_corr_RefObjPt->SetTitle("MPF RMS corrected with PU as a function of p_{T}^{Leading Jet}");
	  gMPF_RMS_corr_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");
    }
	gMPF_RMS_corr_RefObjPt->GetYaxis()->SetTitle("MPF RMS corrected with PU");
	gMPF_RMS_corr_RefObjPt->SetMarkerStyle(20);
	gMPF_RMS_corr_RefObjPt->SetMarkerColor(plotColor);
	gMPF_RMS_corr_RefObjPt->SetLineColor(plotColor);
	//gMPF_RMS_corr_RefObjPt->SetMarkerSize(0.5);
	cMPF_RMS_corr_RefObjPt->cd();
	//gMPF_RMS_corr_RefObjPt->SetLogx(1);
	//gMPF_RMS_corr_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMPF_RMS_corr_RefObjPt->Draw("ape");
	TGraph_style (gMPF_RMS_corr_RefObjPt);
	saveName = "images/MPF/cMPF_RMS_corr_RefObjPt" + typeName + extension;
	cMPF_RMS_corr_RefObjPt->SaveAs(saveName.c_str());


  TGraphErrors *gMJB_gen_RefObjPt = NULL;
  TGraphErrors *gMPF_gen_RefObjPt = NULL;

  if (isMC){

//************************************************************************************************************
//
//                                      MPF_gen as a function of ref obj pt
//
//************************************************************************************************************	

	float aMPF_gen_RefObjPt_Mean[numberPtBins];
	float aMPF_gen_RefObjPt_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aMPF_gen_RefObjPt_Mean[i] = vMPF_gen_RefObjPtBin[i]->GetMean();
		aMPF_gen_RefObjPt_MeanError[i] = vMPF_gen_RefObjPtBin[i]->GetMeanError();
	}
	
	TCanvas *cMPF_gen_RefObjPt = new TCanvas("cMPF_gen_RefObjPt","cMPF_gen_RefObjPt");
	cMPF_gen_RefObjPt->cd();
	
	gMPF_gen_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_pt10_Mean, aMPF_gen_RefObjPt_Mean, aRefObjPtBins_pt10_MeanError, aMPF_gen_RefObjPt_MeanError);
	gMPF_gen_RefObjPt->SetName("MPF_gen");
    if(useRecoilPtBin) {
	  gMPF_gen_RefObjPt->SetTitle("MPF_gen as a function of p_{T}^{Recoil}");
	  gMPF_gen_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMPF_gen_RefObjPt->SetTitle("MPF_gen as a function of p_{T}^{Leading Jet}");
	  gMPF_gen_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");    
    }
	gMPF_gen_RefObjPt->GetYaxis()->SetTitle("MPF_gen");
	gMPF_gen_RefObjPt->SetMarkerStyle(20);
	gMPF_gen_RefObjPt->SetMarkerColor(plotColor);
	gMPF_gen_RefObjPt->SetLineColor(plotColor);
	//gMPF_gen_RefObjPt->SetMarkerSize(0.5);	
	cMPF_gen_RefObjPt->cd();
	//gMPF_gen_RefObjPt->SetLogx(1);
	//gMPF_gen_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMPF_gen_RefObjPt->Draw("ape");
	TGraph_style (gMPF_gen_RefObjPt);
	saveName = "images/MPF_gen/cMPF_gen_RefObjPt" + typeName + extension;
	cMPF_gen_RefObjPt->SaveAs(saveName.c_str());


//************************************************************************************************************
//
//                                      MJB_gen as a function of reference object pt
//
//************************************************************************************************************	

	float aMJB_gen_RefObjPt_Mean[numberPtBins];
	float aMJB_gen_RefObjPt_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aMJB_gen_RefObjPt_Mean[i] = vMJB_gen_RefObjPtBin[i]->GetMean();
		aMJB_gen_RefObjPt_MeanError[i] = vMJB_gen_RefObjPtBin[i]->GetMeanError();
	}
	
	TCanvas *cMJB_gen_RefObjPt = new TCanvas("cMJB_gen_RefObjPt","cMJB_gen_RefObjPt");
	cMJB_gen_RefObjPt->cd();
	
	gMJB_gen_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aMJB_gen_RefObjPt_Mean, aRefObjPtBins_MeanError, aMJB_gen_RefObjPt_MeanError);
	gMJB_gen_RefObjPt->SetName("MJB_gen");
    if(useRecoilPtBin) {
	  gMJB_gen_RefObjPt->SetTitle("MJB_gen as a function of p_{T}^{Recoil}");
	  gMJB_gen_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
    }
    else {
	  gMJB_gen_RefObjPt->SetTitle("MJB_gen as a function of p_{T}^{Leading Jet}");
	  gMJB_gen_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");    
    }
	gMJB_gen_RefObjPt->GetYaxis()->SetTitle("MJB_gen");
	gMJB_gen_RefObjPt->SetMarkerStyle(20);
	gMJB_gen_RefObjPt->SetMarkerColor(plotColor);
	gMJB_gen_RefObjPt->SetLineColor(plotColor);
	//gMJB_gen_RefObjPt->SetMarkerSize(0.5);	
	cMJB_gen_RefObjPt->cd();
	//gMJB_gen_RefObjPt->SetLogx(1);
	//gMJB_gen_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMJB_gen_RefObjPt->Draw("ape");
	TGraph_style (gMJB_gen_RefObjPt);
	saveName = "images/MJB_gen/cMJB_gen_RefObjPt" + typeName + extension;
	cMJB_gen_RefObjPt->SaveAs(saveName.c_str());
	




  } // end if isMC

//************************************************************************************************************
//
//                                      MJB as a function of ref obj eta
//
//************************************************************************************************************	

	float aMJB_RefObjEta_Mean[numberEtaBins];
	float aMJB_RefObjEta_MeanError[numberEtaBins];
	float aRefObjEtaBins_Mean[numberEtaBins];
	float aRefObjEtaBins_MeanError[numberEtaBins];
	
	for(int i=0; i<numberEtaBins; i++) {
		aMJB_RefObjEta_Mean[i] = vMJB_RefObjEtaBin[i]->GetMean();
		aMJB_RefObjEta_MeanError[i] = vMJB_RefObjEtaBin[i]->GetMeanError();
		aRefObjEtaBins_Mean[i] = ( myEtaBinning.getBinValueInf(i)+myEtaBinning.getBinValueSup(i) )/2.;
		aRefObjEtaBins_MeanError[i]=0.;
	}
	
	TCanvas *cMJB_RefObjEta = new TCanvas("cMJB_RefObjEta","cMJB_RefObjEta");
	cMJB_RefObjEta->cd();
	
	TGraphErrors *gMJB_RefObjEta = new TGraphErrors(numberEtaBins,aRefObjEtaBins_Mean, aMJB_RefObjEta_Mean, aRefObjEtaBins_MeanError, aMJB_RefObjEta_MeanError);
	gMJB_RefObjEta->SetName("MJB");
    if(useRecoilPtBin) {
	  gMJB_RefObjEta->SetTitle("MJB as a function of |#eta^{Recoil}|");
	  gMJB_RefObjEta->GetXaxis()->SetTitle("|#eta^{Recoil}|");
    }
    else {
 	  gMJB_RefObjEta->SetTitle("MJB as a function of |#eta^{Leading Jet}|");
	  gMJB_RefObjEta->GetXaxis()->SetTitle("|#eta^{Leading Jet}|");
    }
	gMJB_RefObjEta->GetYaxis()->SetTitle("MJB");
	gMJB_RefObjEta->SetMarkerStyle(20);
	gMJB_RefObjEta->SetMarkerColor(plotColor);
	gMJB_RefObjEta->SetLineColor(plotColor);
	//gMJB_RefObjEta->SetMarkerSize(0.5);	
	cMJB_RefObjEta->cd();
	//gMJB_RefObjEta->SetLogx(1);
	//gMJB_RefObjEta->GetYaxis()->SetRangeUser(0.7,1.1);
	gMJB_RefObjEta->Draw("ape");
	TGraph_style (gMJB_RefObjEta);
	saveName = "images/MJB/cMJB_RefObjEta" + typeName + extension;
	cMJB_RefObjEta->SaveAs(saveName.c_str());
	
	
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
	
	TGraphErrors *gNjetsRecoil_RecoilPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aNjetsRecoil_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_MeanError);
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
	
	TGraphErrors *gNjetsRecoil_068E_RecoilPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aNjetsRecoil_068E_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_068E_MeanError);
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
	
	TGraphErrors *gNjetsRecoil_095E_RecoilPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aNjetsRecoil_095E_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_095E_MeanError);
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


//************************************************************************************************************
//
//                                      MJB_LogPtPrimeBin as a function of recoilpt 
//
//************************************************************************************************************	

	float aMJB_LogPtPrimeBin_Mean[numberLogPtPrimeBins];
	float aMJB_LogPtPrimeBin_MeanError[numberLogPtPrimeBins];
	float aLogPtPrimeBins_Mean[numberLogPtPrimeBins];
	float aLogPtPrimeBins_MeanError[numberLogPtPrimeBins];

	
	for(int i=0; i<numberLogPtPrimeBins; i++) {
		aMJB_LogPtPrimeBin_Mean[i] = vMJB_LogPtPrimeBin[i]->GetMean();
		aMJB_LogPtPrimeBin_MeanError[i] = vMJB_LogPtPrimeBin[i]->GetMeanError();
		aLogPtPrimeBins_Mean[i] = ( myLogPtPrimeBinning.getBinValueInf(i)+myLogPtPrimeBinning.getBinValueSup(i) )/2.;
		aLogPtPrimeBins_MeanError[i]=0.;

	}
	
	TCanvas *cMJB_LogPtPrimeBin_RecoilPt = new TCanvas("cMJB_LogPtPrimeBin_RecoilPt","cMJB_LogPtPrimeBin_RecoilPt");
	cMJB_LogPtPrimeBin_RecoilPt->cd();
	
	TGraphErrors *gMJB_LogPtPrimeBin_RecoilPt = new TGraphErrors(numberLogPtPrimeBins,aLogPtPrimeBins_Mean, aMJB_LogPtPrimeBin_Mean, aLogPtPrimeBins_MeanError, aMJB_LogPtPrimeBin_MeanError);
	gMJB_LogPtPrimeBin_RecoilPt->SetName("MJB_LogPtPrimeBin");
	gMJB_LogPtPrimeBin_RecoilPt->SetTitle("MJB as a function of p_{T}^{Recoil}*exp(sum_i[F_i * log(f_i)])");
	gMJB_LogPtPrimeBin_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil}*exp(sum_i[F_i * log(f_i)])");
	gMJB_LogPtPrimeBin_RecoilPt->GetYaxis()->SetTitle("MJB");
	gMJB_LogPtPrimeBin_RecoilPt->SetMarkerStyle(20);
	gMJB_LogPtPrimeBin_RecoilPt->SetMarkerColor(plotColor);
	gMJB_LogPtPrimeBin_RecoilPt->SetLineColor(plotColor);
	//gMJB_LogPtPrimeBin_RecoilPt->SetMarkerSize(0.5);	
	cMJB_LogPtPrimeBin_RecoilPt->cd();
	//gMJB_LogPtPrimeBin_RecoilPt->SetLogx(1);
	//gMJB_LogPtPrimeBin_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMJB_LogPtPrimeBin_RecoilPt->Draw("ape");
	TGraph_style (gMJB_LogPtPrimeBin_RecoilPt);
	saveName = "images/MJB/cMJB_LogPtPrimeBin_RecoilPt" + typeName + extension;
	cMJB_LogPtPrimeBin_RecoilPt->SaveAs(saveName.c_str());
	saveName = "images/MJB/cMJB_LogPtPrimeBin_RecoilPt" + typeName + ".root";
	cMJB_LogPtPrimeBin_RecoilPt->SaveAs(saveName.c_str());


  
//************************************************************************************************************
//
//                                      InvMJB_LogPtPrimeBin as a function of recoilpt 
//
//************************************************************************************************************	

	float aInvMJB_LogPtPrimeBin_Mean[numberLogPtPrimeBins];
	float aInvMJB_LogPtPrimeBin_MeanError[numberLogPtPrimeBins];
	
	for(int i=0; i<numberLogPtPrimeBins; i++) {
		aInvMJB_LogPtPrimeBin_Mean[i] = vInvMJB_LogPtPrimeBin[i]->GetMean();
		aInvMJB_LogPtPrimeBin_MeanError[i] = vInvMJB_LogPtPrimeBin[i]->GetMeanError();
	}
	
	TCanvas *cInvMJB_LogPtPrimeBin_RecoilPt = new TCanvas("cInvMJB_LogPtPrimeBin_RecoilPt","cInvMJB_LogPtPrimeBin_RecoilPt");
	cInvMJB_LogPtPrimeBin_RecoilPt->cd();
	
	TGraphErrors *gInvMJB_LogPtPrimeBin_RecoilPt = new TGraphErrors(numberLogPtPrimeBins,aLogPtPrimeBins_Mean, aInvMJB_LogPtPrimeBin_Mean, aLogPtPrimeBins_MeanError, aInvMJB_LogPtPrimeBin_MeanError);
	gInvMJB_LogPtPrimeBin_RecoilPt->SetName("InvMJB_LogPtPrimeBin");
	gInvMJB_LogPtPrimeBin_RecoilPt->SetTitle("InvMJB as a function of p_{T}^{Recoil}*exp(sum_i[F_i * log(f_i)])");
	gInvMJB_LogPtPrimeBin_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil}*exp(sum_i[F_i * log(f_i)])");
	gInvMJB_LogPtPrimeBin_RecoilPt->GetYaxis()->SetTitle("InvMJB");
	gInvMJB_LogPtPrimeBin_RecoilPt->SetMarkerStyle(20);
	gInvMJB_LogPtPrimeBin_RecoilPt->SetMarkerColor(plotColor);
	gInvMJB_LogPtPrimeBin_RecoilPt->SetLineColor(plotColor);
	//gInvMJB_LogPtPrimeBin_RecoilPt->SetMarkerSize(0.5);	
	cInvMJB_LogPtPrimeBin_RecoilPt->cd();
	//gInvMJB_LogPtPrimeBin_RecoilPt->SetLogx(1);
	//gInvMJB_LogPtPrimeBin_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gInvMJB_LogPtPrimeBin_RecoilPt->Draw("ape");
	TGraph_style (gInvMJB_LogPtPrimeBin_RecoilPt);
	saveName = "images/MJB/cInvMJB_LogPtPrimeBin_RecoilPt" + typeName + extension;
	cInvMJB_LogPtPrimeBin_RecoilPt->SaveAs(saveName.c_str());
	saveName = "images/MJB/cInvMJB_LogPtPrimeBin_RecoilPt" + typeName + ".root";
	cInvMJB_LogPtPrimeBin_RecoilPt->SaveAs(saveName.c_str());


//************************************************************************************************************
//
//                                      Exp_sum_Fi_log_fi as a function of recoilpt 
//
//************************************************************************************************************	

	float aExp_sum_Fi_log_fi_Mean[numberPtBins];
	float aExp_sum_Fi_log_fi_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aExp_sum_Fi_log_fi_Mean[i] = vExp_sum_Fi_log_fi_RecoilPt[i]->GetMean();
		aExp_sum_Fi_log_fi_MeanError[i] = vExp_sum_Fi_log_fi_RecoilPt[i]->GetMeanError();
	}
	
	TCanvas *cExp_sum_Fi_log_fi_RecoilPt = new TCanvas("cExp_sum_Fi_log_fi_RecoilPt","cExp_sum_Fi_log_fi_RecoilPt");
	cExp_sum_Fi_log_fi_RecoilPt->cd();
	
	TGraphErrors *gExp_sum_Fi_log_fi_RecoilPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aExp_sum_Fi_log_fi_Mean, aRefObjPtBins_MeanError, aExp_sum_Fi_log_fi_MeanError);
	gExp_sum_Fi_log_fi_RecoilPt->SetName("Exp_sum_Fi_log_fi");
	gExp_sum_Fi_log_fi_RecoilPt->SetTitle("exp(sum_i[F_i * log(f_i)]) as a function of p_{T}^{Recoil}");
	gExp_sum_Fi_log_fi_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gExp_sum_Fi_log_fi_RecoilPt->GetYaxis()->SetTitle("exp(sum_i[F_i * log(f_i)])");
	gExp_sum_Fi_log_fi_RecoilPt->SetMarkerStyle(20);
	gExp_sum_Fi_log_fi_RecoilPt->SetMarkerColor(plotColor);
	gExp_sum_Fi_log_fi_RecoilPt->SetLineColor(plotColor);
	//gExp_sum_Fi_log_fi_RecoilPt->SetMarkerSize(0.5);	
	cExp_sum_Fi_log_fi_RecoilPt->cd();
	//gExp_sum_Fi_log_fi_RecoilPt->SetLogx(1);
	//gExp_sum_Fi_log_fi_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gExp_sum_Fi_log_fi_RecoilPt->Draw("ape");
	TGraph_style (gExp_sum_Fi_log_fi_RecoilPt);
	saveName = "images/NjetsRecoil/cExp_sum_Fi_log_fi_RecoilPt" + typeName + extension;
	cExp_sum_Fi_log_fi_RecoilPt->SaveAs(saveName.c_str());
	saveName = "images/NjetsRecoil/cExp_sum_Fi_log_fi_RecoilPt" + typeName + plotName + ".root";
	cExp_sum_Fi_log_fi_RecoilPt->SaveAs(saveName.c_str());


//************************************************************************************************************
//
//                                      Log_ptrecoil_exp_sum_Fi_log_fi as a function of recoilpt 
//
//************************************************************************************************************	

	float aLog_ptrecoil_exp_sum_Fi_log_fi_Mean[numberPtBins];
	float aLog_ptrecoil_exp_sum_Fi_log_fi_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aLog_ptrecoil_exp_sum_Fi_log_fi_Mean[i] = vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt[i]->GetMean();
		aLog_ptrecoil_exp_sum_Fi_log_fi_MeanError[i] = vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt[i]->GetMeanError();
	}
	
	TCanvas *cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt = new TCanvas("cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt","cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt");
	cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->cd();
	
	TGraphErrors *gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aLog_ptrecoil_exp_sum_Fi_log_fi_Mean, aRefObjPtBins_MeanError, aLog_ptrecoil_exp_sum_Fi_log_fi_MeanError);
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SetName("Log_ptrecoil_exp_sum_Fi_log_fi");
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SetTitle("log( ptrecoil * exp(sum_i[F_i * log(f_i)]) ) as a function of p_{T}^{Recoil}");
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->GetYaxis()->SetTitle("log( ptrecoil * exp(sum_i[F_i * log(f_i)]) )");
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SetMarkerStyle(20);
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SetMarkerColor(plotColor);
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SetLineColor(plotColor);
	//gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SetMarkerSize(0.5);	
	cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->cd();
	//gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SetLogx(1);
	//gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->Draw("ape");
	TGraph_style (gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt);
	saveName = "images/NjetsRecoil/cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt" + typeName + extension;
	cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SaveAs(saveName.c_str());
    saveName = "images/NjetsRecoil/cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt" + typeName + ".root";
	cLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->SaveAs(saveName.c_str());


//************************************************************************************************************
//
//                                      MeanLogPt as a function of recoilpt
//
//************************************************************************************************************	

	float aMeanLogPt_Mean[numberPtBins];
	float aMeanLogPt_MeanError[numberPtBins];
	
	for(int i=0; i<numberPtBins; i++) {
		aMeanLogPt_Mean[i] = vMeanLogPt_RecoilPt[i]->GetMean();
		aMeanLogPt_MeanError[i] = vMeanLogPt_RecoilPt[i]->GetMeanError();
	}
	
	TCanvas *cMeanLogPt_RecoilPt = new TCanvas("cMeanLogPt_RecoilPt","cMeanLogPt_RecoilPt");
	cMeanLogPt_RecoilPt->cd();
	
	TGraphErrors *gMeanLogPt_RecoilPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aMeanLogPt_Mean, aRefObjPtBins_MeanError, aMeanLogPt_MeanError);
	gMeanLogPt_RecoilPt->SetName("MeanLogPt");
	gMeanLogPt_RecoilPt->SetTitle("<log(p_{T}^{jets in recoil})> as a function of p_{T}^{Recoil}");
	gMeanLogPt_RecoilPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
	gMeanLogPt_RecoilPt->GetYaxis()->SetTitle("<log(p_{T}^{jets in recoil})>");
	gMeanLogPt_RecoilPt->SetMarkerStyle(20);
	gMeanLogPt_RecoilPt->SetMarkerColor(plotColor);
	gMeanLogPt_RecoilPt->SetLineColor(plotColor);
	//gMeanLogPt_RecoilPt->SetMarkerSize(0.5);	
	cMeanLogPt_RecoilPt->cd();
	//gMeanLogPt_RecoilPt->SetLogx(1);
	//gMeanLogPt_RecoilPt->GetYaxis()->SetRangeUser(0.7,1.1);
	gMeanLogPt_RecoilPt->Draw("ape");
	TGraph_style (gMeanLogPt_RecoilPt);
	saveName = "images/NjetsRecoil/cMeanLogPt_RecoilPt" + typeName + extension;
	cMeanLogPt_RecoilPt->SaveAs(saveName.c_str());
	saveName = "images/NjetsRecoil/cMeanLogPt_RecoilPt" + typeName + ".root";
	cMeanLogPt_RecoilPt->SaveAs(saveName.c_str());

	
	TGraphErrors *gRtrue_leadingJet_RefObjPt = NULL;
	TGraphErrors *gRtrue_allJets_JetPt = NULL;
	TGraphErrors *gRrecoil_RefObjPt = NULL;
	
	if(isMC) {
//************************************************************************************************************
//
//                                      Rtrue,leading jet as a function of ref obj pt
//
//************************************************************************************************************	

	
		float aRtrue_leadingJet_Mean[numberPtBins];
		float aRtrue_leadingJet_MeanError[numberPtBins];
	
		for(int i=0; i<numberPtBins; i++) {
			aRtrue_leadingJet_Mean[i] = vRtrue_leadingJet_RefObjPt[i]->GetMean();
			aRtrue_leadingJet_MeanError[i] = vRtrue_leadingJet_RefObjPt[i]->GetMeanError();
		}
	
		TCanvas *cRtrue_leadingJet_RefObjPt = new TCanvas("cRtrue_leadingJet_RefObjPt","cRtrue_leadingJet_RefObjPt");
		cRtrue_leadingJet_RefObjPt->cd();
	
		gRtrue_leadingJet_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aRtrue_leadingJet_Mean, aRefObjPtBins_MeanError, aRtrue_leadingJet_MeanError);
		gRtrue_leadingJet_RefObjPt->SetName("Rtrue");
        if (useRecoilPtBin) {
		  gRtrue_leadingJet_RefObjPt->SetTitle("R_{true} as a function of p_{T}^{Recoil}");
		  gRtrue_leadingJet_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
        }
        else {
		  gRtrue_leadingJet_RefObjPt->SetTitle("R_{true} as a function of p_{T}^{Leading Jet}");
		  gRtrue_leadingJet_RefObjPt->GetXaxis()->SetTitle("p_{T}^{LeadingJet} (GeV)");      
        }
		gRtrue_leadingJet_RefObjPt->GetYaxis()->SetTitle("R_{true}");
		gRtrue_leadingJet_RefObjPt->SetMarkerStyle(20);
		gRtrue_leadingJet_RefObjPt->SetMarkerColor(plotColor);
		gRtrue_leadingJet_RefObjPt->SetLineColor(plotColor);
		//gRtrue_leadingJet_RefObjPt->SetMarkerSize(0.5);	
		cRtrue_leadingJet_RefObjPt->cd();
		//gRtrue_leadingJet_RefObjPt->SetLogx(1);
		//gRtrue_leadingJet_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
		gRtrue_leadingJet_RefObjPt->Draw("ape");
		TGraph_style (gRtrue_leadingJet_RefObjPt);
		saveName = "images/Rtrue/cRtrue_leadingJet_RefObjPt" + typeName + extension;
		cRtrue_leadingJet_RefObjPt->SaveAs(saveName.c_str());

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
//                                      Rrecoil as a function of reference object pt
//
//************************************************************************************************************	

		float aRrecoil_Mean[numberPtBins];
		float aRrecoil_MeanError[numberPtBins];
	
		for(int i=0; i<numberPtBins; i++) {
			aRrecoil_Mean[i] = vRrecoil_RefObjPtBin[i]->GetMean();
			aRrecoil_MeanError[i] = vRrecoil_RefObjPtBin[i]->GetMeanError();
		}
	
		TCanvas *cRrecoil_RefObjPt = new TCanvas("cRrecoil_RefObjPt","cRrecoil_RefObjPt");
		cRrecoil_RefObjPt->cd();
	
		gRrecoil_RefObjPt = new TGraphErrors(numberPtBins,aRefObjPtBins_Mean, aRrecoil_Mean, aRefObjPtBins_MeanError, aRrecoil_MeanError);
		gRrecoil_RefObjPt->SetName("Rrecoil");
        if(useRecoilPtBin) {
		  gRrecoil_RefObjPt->SetTitle("R_{recoil} as a function of p_{T}^{Recoil}");
		  gRrecoil_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Recoil} (GeV)");
        }
        else {
		  gRrecoil_RefObjPt->SetTitle("R_{recoil} as a function of p_{T}^{Leading Jet}");
		  gRrecoil_RefObjPt->GetXaxis()->SetTitle("p_{T}^{Leading Jet} (GeV)");        
        }
		gRrecoil_RefObjPt->GetYaxis()->SetTitle("R_{recoil}");
		gRrecoil_RefObjPt->SetMarkerStyle(20);
		gRrecoil_RefObjPt->SetMarkerColor(plotColor);
		gRrecoil_RefObjPt->SetLineColor(plotColor);
		//gRrecoil_RefObjPt->SetMarkerSize(0.5);	
		cRrecoil_RefObjPt->cd();
		//gRrecoil_RefObjPt->SetLogx(1);
		//gRrecoil_RefObjPt->GetYaxis()->SetRangeUser(0.7,1.1);
		gRrecoil_RefObjPt->Draw("ape");
		TGraph_style (gRrecoil_RefObjPt);
		saveName = "images/Rrecoil/cRrecoil_RefObjPt" + typeName + extension;
		cRrecoil_RefObjPt->SaveAs(saveName.c_str());	
	
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
	TDirectory *ptbinDir = mjbDir->mkdir("PtBin","PtBin");
	ptbinDir->cd();
	gMJB_RefObjPt->Write("gMJB_RefObjPt");
	gMJB_RMS_RefObjPt->Write("gMJB_RMS_RefObjPt");
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RefObjPtBin[j]->Write();
	}

	TDirectory *logptprimebinDir = mjbDir->mkdir("LogPtPrimeBin","LogPtPrimeBin");
	logptprimebinDir->cd();
	for(int j=0; j<myLogPtPrimeBinning.getSize(); j++) {
    gMJB_LogPtPrimeBin_RecoilPt->Write("gMJB_LogPtPrimeBin_RecoilPt");
    gInvMJB_LogPtPrimeBin_RecoilPt->Write("gInvMJB_LogPtPrimeBin_RecoilPt");
		vMJB_LogPtPrimeBin[j]->Write();
    vInvMJB_LogPtPrimeBin[j]->Write();
	}
	
	TDirectory *etabinDir = mjbDir->mkdir("EtaBin","EtaBin");
	etabinDir->cd();
	gMJB_RefObjEta->Write("gMJB_RefObjEta");
	for(int j=0; j<myEtaBinning.getSize(); j++) {
		vMJB_RefObjEtaBin[j]->Write();
	}

	TDirectory *npvbinDir = mjbDir->mkdir("npvBin","npvBin");
	npvbinDir->cd();	
	gMJB_Npv->Write("gMJB_Npv");	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Write();
	}
	
	TDirectory *mpfDir = out->mkdir("MPF","MPF");
	mpfDir->cd();
	TDirectory *ptbinmpfDir = mpfDir->mkdir("PtBin","PtBin");
	ptbinmpfDir->cd();
	gMPF_RefObjPt->Write("gMPF_RefObjPt");
	gMPF_RMS_RefObjPt->Write("gMPF_RMS_RefObjPt");
	gMPF_corr_RefObjPt->Write("gMPF_corr_RefObjPt");
	gMPF_RMS_corr_RefObjPt->Write("gMPF_RMS_corr_RefObjPt");
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RefObjPtBin[j]->Write();
		vMPF_corr_RefObjPtBin[j]->Write();
	}

  TDirectory *leadingJetDir = out->mkdir("leadingJet","leadingJet");
	leadingJetDir->cd();
	TDirectory *ptbin_jet1Dir = leadingJetDir->mkdir("PtBin","PtBin");
	ptbin_jet1Dir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vLeadingJetPt_RefObjPtBin[j]->Write();
	}
  TDirectory *leadingjetrawptbin_jet1Dir = leadingJetDir->mkdir("HLTPtBin", "HLTPtBin");
  leadingjetrawptbin_jet1Dir->cd();
  for(int j=0; j<myHLTPtBinning.getSize(); j++) {
    vLeadingJetPt_HLTRefObjPtBin[j]->Write();
  }

	if(isMC) {
        TDirectory *mjb_genDir = out->mkdir("MJB_gen","MJB_gen");
        mjb_genDir->cd();
        TDirectory *ptbin_genDir = mjb_genDir->mkdir("PtBin","PtBin");
        ptbin_genDir->cd();
        gMJB_gen_RefObjPt->Write("gMJB_gen_RefObjPt");
        for(int j=0; j<myPtBinning.getSize(); j++) {
            vMJB_gen_RefObjPtBin[j]->Write();
        }

        TDirectory *mpf_genDir = out->mkdir("MPF_gen","MPF_gen");
        mpf_genDir->cd();
        TDirectory *ptbinmpf_genDir = mpf_genDir->mkdir("PtBin","PtBin");
        ptbinmpf_genDir->cd();
        gMPF_gen_RefObjPt->Write("gMPF_gen_RefObjPt");
        for(int j=0; j<myPtBinning.getSize(); j++) {
            vMPF_gen_RefObjPtBin[j]->Write();
        }


		TDirectory *trueDir = out->mkdir("Rtrue","Rtrue");
		trueDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_leadingJet_RefObjPt[j]->Write();
			vRrecoil_RefObjPtBin[j]->Write();
		}
		gRtrue_leadingJet_RefObjPt->Write("gRtrue_leadingJet_RefObjPt");
		
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
        gNjetsRecoil_RecoilPt->Write("gNjetsRecoil_RecoilPt");
        gNjetsRecoil_068E_RecoilPt->Write("gNjetsRecoil_068E_RecoilPt");
        gNjetsRecoil_095E_RecoilPt->Write("gNjetsRecoil_095E_RecoilPt");
        gMeanLogPt_RecoilPt->Write("gMeanLogPt_RecoilPt");
        gExp_sum_Fi_log_fi_RecoilPt->Write("gExp_sum_Fi_log_fi_RecoilPt");
        gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt->Write("gLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt");
        vNjetsRecoil_RecoilPt[j]->Write();
        vNjetsRecoil_068E_RecoilPt[j]->Write();
        vNjetsRecoil_095E_RecoilPt[j]->Write();
        vMeanLogPt_RecoilPt[j]->Write();
        vExp_sum_Fi_log_fi_RecoilPt[j]->Write();
        vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt[j]->Write();
    }


	TDirectory *recoilDir = out->mkdir("recoil","recoil");
	recoilDir->cd();	
	hRecoilEta->Write();
	hRecoilWidth->Write();
	TDirectory *ptbin_recoilDir = recoilDir->mkdir("PtBin","PtBin");
	ptbin_recoilDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vRecoilPt_RefObjPtBin[j]->Write();
	}
	
	TDirectory *variablesDir = out->mkdir("variables","variables");
	variablesDir->cd();
	
	TDirectory *beforeSelDir = variablesDir->mkdir("beforeSel","beforeSel");
	beforeSelDir->cd();
	hMet_beforeSel->Write();
	hLeadingJetPt_beforeSel->Write();
	hSecondJetPt_beforeSel->Write();
	hRecoilPt_beforeSel->Write();
	hRecoilJetsPt_beforeSel->Write();
	hNpv_beforeSel->Write();
	hAlpha_beforeSel->Write();
	hBeta_beforeSel->Write();
	hA_beforeSel->Write();
  hJetsPt_beforeSel->Write();
  hJetsEta_beforeSel->Write();
  hLeadingJetEta_beforeSel->Write();
  hRecoilEta_beforeSel->Write();
  hJetsPhi_beforeSel->Write();
  hHT_beforeSel->Write();
  hNjets_ptSup30_etaInf5_beforeSel->Write();
	
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
	hLeadingJetMass_afterSel->Write();
	hLeadingAndSecondJetsMass_afterSel->Write();
	hSecondJetPt_afterSel->Write();
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
    hLeadingJetEta_afterSel->Write();
    hRecoilEta_afterSel->Write();
    hJetsPhi_afterSel->Write();
    hDeltaPhi_METRecoil_afterSel->Write();
    hDeltaPhi_METJet1_afterSel->Write();
	
	out->Close();
	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RefObjPtBin[j]->Delete();
	}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Delete();
	}

	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RefObjPtBin[j]->Delete();
		vMPF_corr_RefObjPtBin[j]->Delete();
	}
	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vNjetsRecoil_RecoilPt[j]->Delete();
		vNjetsRecoil_068E_RecoilPt[j]->Delete();
		vNjetsRecoil_095E_RecoilPt[j]->Delete();
	}	
	
	if(isMC) {
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_leadingJet_RefObjPt[j]->Delete();
			vRrecoil_RefObjPtBin[j]->Delete();
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
	hSecondJetPt_beforeSel->Delete();
	hRecoilPt_beforeSel->Delete();
	hNpv_beforeSel->Delete();
	hAlpha_beforeSel->Delete();
	hBeta_beforeSel->Delete();
	hA_beforeSel->Delete();	
	hMet_afterSel->Delete();
	hLeadingJetPt_afterSel->Delete();
	delete hLeadingJetMass_afterSel;
	delete hLeadingAndSecondJetsMass_afterSel;
	hSecondJetPt_afterSel->Delete();
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









