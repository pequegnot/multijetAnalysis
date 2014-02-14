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
#include <TChain.h>
#include "TROOT.h"
#include <tclap/CmdLine.h>

#include "../common/common.h"

#include "../common/ptBinning.h"
#include "../common/npvBinning.h"
#include "../common/etaBinning.h"
#include "PUReweighting/PUReweighter.h"


using namespace std;

void loadChain(const std::vector<std::string>& inputFiles, const std::string& treeName, TChain*& output) {

  output = new TChain(treeName.c_str());

  for (const std::string& file: inputFiles) {
    output->Add(file.c_str());
  }

  output->SetCacheSize(30*1024*1024);
}

void loadInputFiles(const std::string& filename, std::vector<std::string>& files) {

  ifstream ifs(filename.c_str());
  std::string line;

  while (getline(ifs, line))
    files.push_back(line);

  ifs.close();
}

int main (int argc, char** argv) 
{
//open input file

	//TFile *f=TFile::Open("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/extracted.root"); 
	
	bool isMC = false;	
	
	float weight = 1.;
	float lumiWeight = 1.;
	//string typeName = "_data";
	int plotColor = getDataColor();
	string extension = ".pdf";
	int Nevents = 1;
	float Xsection = 1.;
	std::vector<std::string> inputFiles;
	string outputName;


//********************************************************************************************************* 	

  try {
    // Define the command line object, and insert a message
    // that describes the program. The "Convert extractor tuples to histograms" 
    // is printed last in the help text. The second argument is the 
    // delimiter (usually space) and the last one is the version number. 
    // The CmdLine object parses the argv array based on the Arg objects
    // that it contains. 
    TCLAP::CmdLine cmd("Convert extractor tuples to histograms", ' ', "1.0");
    
    // Define a value argument and add it to the command line.
    // A value arg defines a flag and a type of value that it expects,
    // such as "-n Bishop".
    TCLAP::ValueArg<std::string> inputListArg("", "input-list", "A text file containing a list of input files", true, "", "string");
    TCLAP::ValueArg<std::string> inputFileArg("i", "input-file", "The input file", true, "", "string");
    TCLAP::ValueArg<int> NeventsArg("", "Nevents", "Nevents", false, -1, "int", cmd);
    TCLAP::ValueArg<float> XsectionArg("", "Xsection", "Xsection", false, -1., "float", cmd);


    // Add the argument to the CmdLine object. The CmdLine object
    // uses this Arg to parse the command line.
    cmd.xorAdd(inputListArg, inputFileArg);

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

    
    // Parse the argv array.
    cmd.parse(argc, argv);
    
    // Get the value parsed by each arg.
    isMC = mcArg.getValue();
    outputName = outputFileArg.getValue();
    Nevents = NeventsArg.getValue();
    Xsection = XsectionArg.getValue();
    
    if (!isMC) {
      weight = 1.;
      //typeName = "_data";
      plotColor = getDataColor();    
    }
    else {
      if(Nevents == -1 || Xsection == -1) {
        cout<<"Enter the Xsection and the number of events !!!!"<<endl;
	return 1;
      }
      lumiWeight = Xsection/Nevents;
      //typeName = "_mc";
      plotColor = getMcColor();    
    }

    if (inputFileArg.isSet()) {
      inputFiles.push_back(inputFileArg.getValue());
    } else {
      loadInputFiles(inputListArg.getValue(), inputFiles);
    } 
    
  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    return 1;
  }
   
    
    

  	

//*********************************************************************************************************
//
//                                      histograms definition
//
//*********************************************************************************************************

	ptBinning myPtBinning;
	npvBinning myNpvBinning;
	etaBinning myEtaBinning;
	PUReweighter myPUReweighter;
  PUReweighter myPUReweighter_HLT_PFJet140("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet140.root");
  PUReweighter myPUReweighter_HLT_PFJet200("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet200.root");
  PUReweighter myPUReweighter_HLT_PFJet260("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet260.root");
  PUReweighter myPUReweighter_HLT_PFJet320("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet320.root");
	ptBinning myLowPtBinning(true);

	
//usefull variables
	Double_t xlow = getHistoXlow();
	Double_t xup = getHistoXup();
	//Double_t    binrange = 0.1;
	//Int_t    nbinsx = (xup - xlow)/binrange;
	Int_t    nbinsx = getHistoNbinsx();

	int nflavours = getFlavourNumber(); //uds, g, c, b, noMatched, all

//vectors for multijet study
	//recoilJetsPt spectrum for different Njetsrecoil values
	vector<TH1F*> vRecoilJetsPt_NrecoilJets =buildNrecoiljetsVectorH1("RecoilJetsPt",40,0.,800.);

	//MJB per Npv
	vector<TH1F*> vMJB_Npv = buildNpvVectorH1(myNpvBinning,"MJB",nbinsx,xlow,xup) ;

	//MJB per recoilpt
	vector<TH1F*> vMJB_RecoilPt = buildPtVectorH1(myPtBinning,"MJB",nbinsx,xlow,xup) ;

	//ptLeadingJet per recoilpt
	vector<TH1F*> vLeadingJetPt_RecoilPt = buildPtVectorH1(myPtBinning,"LeadingJetPt",35,200,650) ;
	
  //RecoilPt per recoilpt bin
	vector<TH1F*> vRecoilPt_RecoilPt = buildBinnedDistriVectorH1(myPtBinning,"RecoilPt",5) ;
	
  //MJB per recoileta
	vector<TH1F*> vMJB_RecoilEta = buildEtaVectorH1(myEtaBinning,"MJB",nbinsx,xlow,xup) ;
	
	//Rmpf per recoilpt
	vector<TH1F*> vMPF_RecoilPt = buildPtVectorH1(myPtBinning,"MPF",nbinsx,xlow,xup) ;
	
	//NjetsRecoil per recoilpt
	vector<TH1F*> vNjetsRecoil_RecoilPt = buildPtVectorH1(myPtBinning,"NjetsRecoil",35,0,35) ;
	vector<TH1F*> vNjetsRecoil_068E_RecoilPt = buildPtVectorH1(myPtBinning,"NjetsRecoil_068E",35,0,35) ;
	vector<TH1F*> vNjetsRecoil_095E_RecoilPt = buildPtVectorH1(myPtBinning,"NjetsRecoil_095E",35,0,35) ;

	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Sumw2();
		vMPF_RecoilPt[j]->Sumw2();
    vLeadingJetPt_RecoilPt[j]->Sumw2();
    vRecoilPt_RecoilPt[j]->Sumw2();

	}
	
	for(int j=0; j<myEtaBinning.getSize(); j++) {
		vMJB_RecoilEta[j]->Sumw2();
	}
	
	//if(isMC) {
		//recoilpt/firstjetgenpt ratio per firstjetgenpt
		vector<TH1F*> vPtRatio_GenPt = buildPtVectorH1(myPtBinning,"PtRatio",nbinsx,xlow,xup) ;
		//Rtrue per recoilpt
		vector<TH1F*> vRtrue_leadingJet_RecoilPt = buildPtVectorH1(myPtBinning,"Rtrue_leadingJet",nbinsx,xlow,xup) ;
		
		vector<TH1F*> vRtrue_allJets_JetPt = buildPtVectorH1(myLowPtBinning,"Rtrue_allJets",nbinsx,xlow,xup) ;
	
		//Rrecoil per recoilpt
		vector<TH1F*> vRrecoil_RecoilPt = buildPtVectorH1(myPtBinning,"Rrecoil",nbinsx,xlow,xup) ;
		
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Sumw2();
			vRtrue_leadingJet_RecoilPt[j]->Sumw2();
			vRtrue_allJets_JetPt[j]->Sumw2();
			vRrecoil_RecoilPt[j]->Sumw2();
			vNjetsRecoil_RecoilPt[j]->Sumw2();
			vNjetsRecoil_068E_RecoilPt[j]->Sumw2();
			vNjetsRecoil_095E_RecoilPt[j]->Sumw2();
		}
		for(int j=0; j<myLowPtBinning.getSize(); j++) {
			vRtrue_allJets_JetPt[j]->Sumw2();
		}		

	
	//}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Sumw2();
	}
	
	for(int j=0; j<16; j++) {
		vRecoilJetsPt_NrecoilJets[j]->Sumw2();
	}

//*******************************************************************************************************
//
//                                      histograms definition
//
//*******************************************************************************************************

	string saveName;
	
	TH1F* hRecoilEta=new TH1F("hRecoilEta","hRecoilEta",13,0,5.2);
	hRecoilEta->SetXTitle("|#eta_{Recoil}|");
	hRecoilEta->Sumw2();
	
	TH1F* hNTrueInteractionsBeforePUReweighting=new TH1F("hNTrueInteractionsBeforePUReweighting","hNTrueInteractionsBeforePUReweighting",70,0,70);
	hNTrueInteractionsBeforePUReweighting->SetXTitle("nTrueInteractions");
	hNTrueInteractionsBeforePUReweighting->Sumw2();
	
	TH1F* hNTrueInteractionsAfterPUReweighting=new TH1F("hNTrueInteractionsAfterPUReweighting","hNTrueInteractionsAfterPUReweighting",70,0,70);
	hNTrueInteractionsAfterPUReweighting->SetXTitle("nTrueInteractions");
	hNTrueInteractionsAfterPUReweighting->Sumw2();

	TH1F* hMJB_inclusive=new TH1F("hMJB_inclusive","hMJB_inclusive",40,0,2);
	hMJB_inclusive->SetXTitle("MJB");
	hMJB_inclusive->Sumw2();

	TH1F* hNjetsRecoil=new TH1F("hNjetsRecoil","hNjetsRecoil",25,0,25);
	hNjetsRecoil->SetXTitle("N_{recoil jets}");
	hNjetsRecoil->Sumw2();
	
	TH1F* hNjetsTotal=new TH1F("hNjetsTotal","hNjetsTotal",25,0,25);
	hNjetsTotal->SetXTitle("N_{total jets}");
	hNjetsTotal->Sumw2();
	
	TH1F* hNjetsGood=new TH1F("hNjetsGood","hNjetsGood",25,0,25);
	hNjetsGood->SetXTitle("N_{good jets}");
	hNjetsGood->Sumw2();

	TH1F* hNjet_JetPt=new TH1F("hNjet_JetPt","hNjet_JetPt",100,0,1000);
	hNjet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNjet_JetPt->SetYTitle("N_{jets}");
	hNjet_JetPt->Sumw2();
	
	TH1F* hNpuLoosejetTmp_JetPt=new TH1F("hNpuLoosejetTmp_JetPt","hNpuLoosejetTmp_JetPt",100,0,1000);
	hNpuLoosejetTmp_JetPt->Sumw2();
	
	TH1F* hNpuMediumjetTmp_JetPt=new TH1F("hNpuMediumjetTmp_JetPt","hNpuMediumjetTmp_JetPt",100,0,1000);
	hNpuMediumjetTmp_JetPt->Sumw2();
	
	TH1F* hNpuTightjetTmp_JetPt=new TH1F("hNpuTightjetTmp_JetPt","hNpuTightjetTmp_JetPt",100,0,1000);
	hNpuTightjetTmp_JetPt->Sumw2();
	
	TH1F* hNpuAlljetTmp_JetPt=new TH1F("hNpuAlljetTmp_JetPt","hNpuAlljetTmp_JetPt",100,0,1000);
	hNpuAlljetTmp_JetPt->Sumw2();
	
	TH1F* hNjet_Npv=new TH1F("hNjet_Npv","hNjet_Npv",70,0,70);
	hNjet_Npv->SetXTitle("N_{PV}");
	hNjet_Npv->SetYTitle("N_{jets}");
	hNjet_Npv->Sumw2();
	
	TH1F* hNpuLoosejetTmp_Npv=new TH1F("hNpuLoosejetTmp_Npv","hNpuLoosejetTmp_Npv",70,0,70);
	hNpuLoosejetTmp_Npv->Sumw2();
	
	TH1F* hNpuMediumjetTmp_Npv=new TH1F("hNpuMediumjetTmp_Npv","hNpuMediumjetTmp_Npv",70,0,70);
	hNpuMediumjetTmp_Npv->Sumw2();
	
	TH1F* hNpuTightjetTmp_Npv=new TH1F("hNpuTightjetTmp_Npv","hNpuTightjetTmp_Npv",70,0,70);
	hNpuTightjetTmp_Npv->Sumw2();
	
	TH1F* hNpuAlljetTmp_Npv=new TH1F("hNpuAlljetTmp_Npv","hNpuAlljetTmp_Npv",70,0,70);
	hNpuAlljetTmp_Npv->Sumw2();
	
	
//************************************************************************************************************************
	TH1F* hNrmPuJets_JetPt=new TH1F("hNrmPuJets_JetPt","hNrmPuJets_JetPt",100,0,1000);
	hNrmPuJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNrmPuJets_JetPt->SetYTitle("N_{PU jets}");
	hNrmPuJets_JetPt->Sumw2();
	

	TH1F* hNpuLoosejet_JetPt=new TH1F("hNpuLoosejet_JetPt","hNpuLoosejet_JetPt",100,0,1000);
	hNpuLoosejet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNpuLoosejet_JetPt->SetYTitle("N_{PU loose jets}");
	hNpuLoosejet_JetPt->Sumw2();
	
	TH1F* hNpuMediumjet_JetPt=new TH1F("hNpuMediumjet_JetPt","hNpuMediumjet_JetPt",100,0,1000);
	hNpuMediumjet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNpuMediumjet_JetPt->SetYTitle("N_{PU medium jets}");
	hNpuMediumjet_JetPt->Sumw2();
	
	TH1F* hNpuTightjet_JetPt=new TH1F("hNpuTightjet_JetPt","hNpuTightjet_JetPt",100,0,1000);
	hNpuTightjet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNpuTightjet_JetPt->SetYTitle("N_{PU tight jets}");
	hNpuTightjet_JetPt->Sumw2();
	
	TH1F* hNpuAlljet_JetPt=new TH1F("hNpuAlljet_JetPt","hNpuAlljet_JetPt",100,0,1000);
	hNpuAlljet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNpuAlljet_JetPt->SetYTitle("N_{PU jets}");
	hNpuAlljet_JetPt->Sumw2();

	
	TH1F* hNpuLoosejet_Npv=new TH1F("hNpuLoosejet_Npv","hNpuLoosejet_Npv",70,0,70);
	hNpuLoosejet_Npv->SetXTitle("N_{PV}");
	hNpuLoosejet_Npv->SetYTitle("N_{PU loose jets}");
	hNpuLoosejet_Npv->Sumw2();
	
	TH1F* hNpuMediumjet_Npv=new TH1F("hNpuMediumjet_Npv","hNpuMediumjet_Npv",70,0,70);
	hNpuMediumjet_Npv->SetXTitle("N_{PV}");
	hNpuMediumjet_Npv->SetYTitle("N_{PU medium jets}");
	hNpuMediumjet_Npv->Sumw2();
	
	TH1F* hNpuTightjet_Npv=new TH1F("hNpuTightjet_Npv","hNpuTightjet_Npv",70,0,70);
	hNpuTightjet_Npv->SetXTitle("N_{PV}");
	hNpuTightjet_Npv->SetYTitle("N_{PU tight jets}");
	hNpuTightjet_Npv->Sumw2();
	
	TH1F* hNpuAlljet_Npv=new TH1F("hNpuAlljet_Npv","hNpuAlljet_Npv",70,0,70);
	hNpuAlljet_Npv->SetXTitle("N_{PV}");
	hNpuAlljet_Npv->SetYTitle("N_{PU jets}");
	hNpuAlljet_Npv->Sumw2();
//************************************************************************************************************************
	
	TH1F* hHT=new TH1F("hHT","hHT",250,0,2500);
	hHT->SetXTitle("HT [GeV/c]");
	hHT->Sumw2();
	
	TH1F* hFracJetsPt=new TH1F("hFracJetsPt","hFracJetsPt",20,0,1);
	hFracJetsPt->SetXTitle("p_{t}^{PU jets}/p_{t}^{totjets}");
	hFracJetsPt->Sumw2();
	
	TH1F* hMet_beforeSel=new TH1F("hMet_beforeSel","hMet_beforeSel",100,0,1000);
	hMet_beforeSel->SetXTitle("MET [GeV/c]");
	hMet_beforeSel->Sumw2();
	
	TH1F* hMet_afterSel=new TH1F("hMet_afterSel","hMet_afterSel",100,0,1000);
	hMet_afterSel->SetXTitle("MET [GeV/c]");
	hMet_afterSel->Sumw2();
	
	
	TH1F* hLeadingJetPt_beforeSel=new TH1F("hLeadingJetPt_beforeSel","hLeadingJetPt_beforeSel",150,0,3000);
	hLeadingJetPt_beforeSel->SetXTitle("p_{t}^{leading jet} [GeV/c]");
	hLeadingJetPt_beforeSel->Sumw2();
	
	TH1F* hLeadingJetPt_afterSel=new TH1F("hLeadingJetPt_afterSel","hLeadingJetPt_afterSel",150,0,3000);
	hLeadingJetPt_afterSel->SetXTitle("p_{t}^{leading jet} [GeV/c]");
	hLeadingJetPt_afterSel->Sumw2();
	
	
	TH1F* hRecoilPt_beforeSel=new TH1F("hRecoilPt_beforeSel","hRecoilPt_beforeSel",150,0,3000);
	hRecoilPt_beforeSel->SetXTitle("p_{t}^{Recoil} [GeV/c]");
	hRecoilPt_beforeSel->Sumw2();
	
	TH1F* hRecoilPt_afterSel=new TH1F("hRecoilPt_afterSel","hRecoilPt_afterSel",150,0,3000);
	hRecoilPt_afterSel->SetXTitle("p_{t}^{Recoil} [GeV/c]");
	hRecoilPt_afterSel->Sumw2();
	
	TH1F* hRecoilJetsPt_beforeSel=new TH1F("hRecoilJetsPt_beforeSel","hRecoilJetsPt_beforeSel",150,0,3000);
	hRecoilJetsPt_beforeSel->SetXTitle("p_{t}^{Recoil jets} [GeV/c]");
	hRecoilJetsPt_beforeSel->Sumw2();
	
	TH1F* hRecoilJetsPt_afterSel=new TH1F("hRecoilJetsPt_afterSel","hRecoilJetsPt_afterSel",150,0,3000);
	hRecoilJetsPt_afterSel->SetXTitle("p_{t}^{Recoil jets} [GeV/c]");
	hRecoilJetsPt_afterSel->Sumw2();	
	
	
	TH1F* hNpv_beforeSel=new TH1F("hNpv_beforeSel","hNpv_beforeSel",70,0,70);
	hNpv_beforeSel->SetXTitle("N_{PV}");
	hNpv_beforeSel->Sumw2();
	
	TH1F* hNpv_afterSel=new TH1F("hNpv_afterSel","hNpv_afterSel",70,0,70);
	hNpv_afterSel->SetXTitle("N_{PV}");
	hNpv_afterSel->Sumw2();	
	
	TH1F* hAlpha_beforeSel=new TH1F("hAlpha_beforeSel","hAlpha_beforeSel",40,0,0.4);
	hAlpha_beforeSel->SetXTitle("#alpha (rad)");
	hAlpha_beforeSel->Sumw2();
	
	TH1F* hAlpha_afterSel=new TH1F("hAlpha_afterSel","hAlpha_afterSel",40,0,0.4);
	hAlpha_afterSel->SetXTitle("#alpha (rad)");
	hAlpha_afterSel->Sumw2();
	
	
	TH1F* hBeta_beforeSel=new TH1F("hBeta_beforeSel","hBeta_beforeSel",30,0,3.15);
	hBeta_beforeSel->SetXTitle("#beta (rad)");
	hBeta_beforeSel->Sumw2();
	
	TH1F* hBeta_afterSel=new TH1F("hBeta_afterSel","hBeta_afterSel",30,0,3.15);
	hBeta_afterSel->SetXTitle("#beta (rad)");
	hBeta_afterSel->Sumw2();
	
	
	TH1F* hA_beforeSel=new TH1F("hA_beforeSel","hA_beforeSel",20,0,1);
	hA_beforeSel->SetXTitle("A = p_{t}^{jet 2}/p_{t}^{Recoil}");
	hA_beforeSel->Sumw2();
	
	TH1F* hA_afterSel=new TH1F("hA_afterSel","hA_afterSel",20,0,1);
	hA_afterSel->SetXTitle("A = p_{t}^{jet 2}/p_{t}^{Recoil}");
	hA_afterSel->Sumw2();
	
	
	TH1F* hRecoilWidth=new TH1F("hRecoilWidth","hRecoilWidth",20,0,1);
	hRecoilWidth->SetXTitle("Recoil width (#Delta #eta = |#eta^{Recoil jet}_{max} - #eta^{Recoil jet}_{min}|)");
	hRecoilWidth->Sumw2();
	
	TH1F* hLeadingJetPt_170to230=new TH1F("hLeadingJetPt_170to230","hLeadingJetPt_170to230",6,170,230);
	hLeadingJetPt_170to230->SetXTitle("p_{t}^{leading jet} [GeV/c]");
	hLeadingJetPt_170to230->Sumw2();

	TH1F* hLeadingJetPt_230to290=new TH1F("hLeadingJetPt_230to290","hLeadingJetPt_230to290",6,230,290);
	hLeadingJetPt_230to290->SetXTitle("p_{t}^{leading jet} [GeV/c]");
	hLeadingJetPt_230to290->Sumw2();

  TH1F* hLeadingJetPt_290to350=new TH1F("hLeadingJetPt_290to350","hLeadingJetPt_290to350",6,290,350);
	hLeadingJetPt_290to350->SetXTitle("p_{t}^{leading jet} [GeV/c]");
	hLeadingJetPt_290to350->Sumw2();
	
	TH1F* hLeadingJetPt_350toInf=new TH1F("hLeadingJetPt_350toInf","hLeadingJetPt_350toInf",265,350,3000);
	hLeadingJetPt_350toInf->SetXTitle("p_{t}^{leading jet} [GeV/c]");
	hLeadingJetPt_350toInf->Sumw2();

//*****************************************************************************************************
//
//                                      reading the root file 
//
//*****************************************************************************************************

//retrieve the trees
	TChain* t_multijet = NULL;
	TChain* t_vertices = NULL;
	TChain* t_event = NULL;
	TChain* t_jet_PF = NULL;
  TChain* t_HLT = NULL;
	loadChain(inputFiles, "Multijet", t_multijet);
	loadChain(inputFiles, "Vertices", t_vertices);
	loadChain(inputFiles, "event", t_event);
	loadChain(inputFiles, "jet_PF", t_jet_PF);
	loadChain(inputFiles, "HLT", t_HLT);



//retrieve the variables
	//Multijet
	float MJB;
	t_multijet->SetBranchAddress("MJB",&MJB);
	
	TClonesArray* recoil_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("recoil_4vector",&recoil_4vector);
	
	TClonesArray* met_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("met_4vector",&met_4vector);
	
	TClonesArray* leadingjet_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("leadingjet_4vector",&leadingjet_4vector);
	
	TClonesArray* leadingjetgen_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("leadingjetgen_4vector",&leadingjetgen_4vector);

	TClonesArray* leadingjetraw_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("leadingjetraw_4vector",&leadingjetraw_4vector);
	
	TClonesArray* jets_recoil_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("jets_recoil_4vector",&jets_recoil_4vector);
	
	TClonesArray* jetsgen_recoil_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("jetsgen_recoil_4vector",&jetsgen_recoil_4vector);
	
	std::vector<int>* goodJetsIndex = NULL;
  t_multijet->SetBranchAddress("goodJetsIndex",&goodJetsIndex);
	
	float A;
	t_multijet->SetBranchAddress("A",&A);
	
	float alpha;
	t_multijet->SetBranchAddress("alpha",&alpha);
	
	float beta;
	t_multijet->SetBranchAddress("beta",&beta);
	
	float HT;
	t_multijet->SetBranchAddress("HT",&HT);
	
	int isSelected;
	t_multijet->SetBranchAddress("isSel",&isSelected);
	
	int n_muons;
	t_multijet->SetBranchAddress("n_muons",&n_muons);
	
	int n_photons;
	t_multijet->SetBranchAddress("n_photons",&n_photons);
	
	int n_jets_recoil;
	t_multijet->SetBranchAddress("n_jets_recoil",&n_jets_recoil);
	
	int n_goodJets;
	t_multijet->SetBranchAddress("n_goodJets",&n_goodJets);
	
	int n_totJets;
	t_multijet->SetBranchAddress("n_totJets",&n_totJets);

	int jet_puJetId[100];
	t_multijet->SetBranchAddress("jet_puJetId",&jet_puJetId);
	
	int muon_isLooseMuon[100];
	t_multijet->SetBranchAddress("muon_isLooseMuon",&muon_isLooseMuon);
	
	int muon_isSoftMuon[100];
	t_multijet->SetBranchAddress("muon_isSoftMuon",&muon_isSoftMuon);
	
	int muon_isTightMuon[100];
	t_multijet->SetBranchAddress("muon_isTightMuon",&muon_isTightMuon);
	
	int muon_isHighPtMuon[100];
	t_multijet->SetBranchAddress("muon_isHighPtMuon",&muon_isHighPtMuon);
	
	int photon_isLoosePhoton[100];
	t_multijet->SetBranchAddress("photon_isLoosePhoton",&photon_isLoosePhoton);

	int photon_isMediumPhoton[100];
	t_multijet->SetBranchAddress("photon_isMediumPhoton",&photon_isMediumPhoton);
	
	int photon_isTightPhoton[100];
	t_multijet->SetBranchAddress("photon_isTightPhoton",&photon_isTightPhoton);	
	
	
	//Vertices
	unsigned int n_vertices;
	t_vertices->SetBranchAddress("n_vertices",&n_vertices);
	
	//Event
	float nTrueInteractions;
	t_event->SetBranchAddress("nTrueInteractions",&nTrueInteractions);
	
	//jet_PF
	TClonesArray* jet_PF_4vector = new TClonesArray("TLorentzVector");
	t_jet_PF->SetBranchAddress("jet_4vector",&jet_PF_4vector);	
	
	int jet_PF_puJetFullId[100];
	t_jet_PF->SetBranchAddress("jet_puJetFullId",&jet_PF_puJetFullId);

	//HLT
	std::vector<std::string>* HLT_vector = NULL;
  t_HLT->SetBranchAddress("HLT_vector",&HLT_vector);
	
	
	//Usefull variables
	int binRecoilPt;//bin en recoilpt
	int binRecoilEta;//bin en recoileta		
	int binGenPt;//bin en firstjetgenpt
	int binJetPt;//bin en pt des jets		
	float recoilpt;
	float recoilpx;
	float recoilpy;
	float recoileta;
	float metpx;
	float metpy;
	float metpt;
	float secondjetpt;	
	float leadingjetgenpt;
  float leadingjetrawpt;
	float leadingjetpt;
	float Rmpf = -1.;
	float Rtrue = -1.;
	float Rrecoil = -1.;
	float recoilrecopt = -1;
	float recoilgenpt = -1;
	bool dropEvent=false;
	float jetsPt;
	float puJetsPt;
	double PUWeight = 1;
	float recoilEtaMin;
	float recoilEtaMax;
	float recoilEtaMinTmp;
	float recoilEtaMaxTmp;
	float recoilDeltaEta;
	float jet_PF_pt;
	
	
	int ntot = 0;
	int nphotonsel = 0;
	int nmuonsel = 0;
	
	int nRecoilJets;
	int nRecoilJets_068E;
	int nRecoilJets_095E;
	float ptRecoil_tmp;
	
	int numberPtBins = myPtBinning.getSize();	
	int numberEtaBins = myEtaBinning.getSize();
	
	//count events in the tree
	unsigned int nEvents = (int)t_multijet->GetEntries();

	
	//loop over them
	for(unsigned int ievt=0; ievt<nEvents; ievt++) {
		//cout<<"event # "<<ievt<<endl;
		t_multijet->GetEntry(ievt);
		t_vertices->GetEntry(ievt);
		t_event->GetEntry(ievt);
		t_jet_PF->GetEntry(ievt);
		t_HLT->GetEntry(ievt);
    
		if(ievt%10000 == 0) {
			cout<<"Event # "<<ievt<<", file ended at "<<(ievt*100.)/(nEvents*1.)<<" %"<<endl;
		} 
		
// 		if(ievt%100000 == 0) {
// 			cout<<"Check n_jets_recoil: "<<endl;
// 			cout<<"n_jets_recoil: "<<n_jets_recoil<<endl;
// 			cout<<"jets_recoil_4vector->GetSize(): "<<jets_recoil_4vector->GetSize()<<endl;
// 		} 	
// 		cout<<"goodJetsIndex->size(): "<<goodJetsIndex->size()<<endl;
// 		for(int i=0; i<goodJetsIndex->size(); i++) {			
// 			cout<<"goodJetsIndex->at("<< i <<") :"<< goodJetsIndex->at(i)<<endl;
// 		}
	
//*********************************************************************************************************

				
		jetsPt = 0.;
		puJetsPt = 0.;
		nRecoilJets_068E = 0;
		nRecoilJets_095E = 0;
		ptRecoil_tmp = 0.;
		
		int nentries_recoil_4vector = recoil_4vector->GetEntriesFast();
		if(nentries_recoil_4vector == 0) 
		  continue;
		  
		TLorentzVector* recoil = (TLorentzVector*) recoil_4vector->At(0);
		recoilpt = recoil->Pt();
		
		TLorentzVector* leadingjet = (TLorentzVector*) leadingjet_4vector->At(0);
		leadingjetpt = leadingjet->Pt();

		TLorentzVector* leadingjetraw = (TLorentzVector*) leadingjetraw_4vector->At(0);
		leadingjetrawpt = leadingjetraw->Pt();

		binRecoilPt = myPtBinning.getPtBin(recoilpt);
		
		TLorentzVector* leadingjetgen = NULL;
		if(isMC) {
			leadingjetgen = (TLorentzVector*) leadingjetgen_4vector->At(0);
			leadingjetgenpt = leadingjetgen->Pt();
		
			binGenPt = myPtBinning.getPtBin(leadingjetgenpt);
			
			Rtrue = leadingjetpt/leadingjetgenpt;
		}
		
		recoilpx = recoil->Px();
		recoilpy = recoil->Py();
		recoileta = recoil->Eta();
		binRecoilEta = myEtaBinning.getEtaBin(fabs(recoileta));
		
	  if(jets_recoil_4vector->GetEntriesFast() == 0) continue;	
		TLorentzVector* secondjet = (TLorentzVector*) jets_recoil_4vector->At(0);// first jet of the recoil system, so index is 0
		secondjetpt = secondjet->Pt();
		
		TLorentzVector* met = (TLorentzVector*) met_4vector->At(0);
		metpx = met->Px();
		metpy = met->Py();
		metpt = met->Pt();
		
		Rmpf = 1 + (recoilpx*metpx + recoilpy*metpy)/(pow(recoilpt,2));		
		
		recoilrecopt = 0.;
		recoilgenpt = 0.;
		
		nRecoilJets = jets_recoil_4vector->GetSize();


//*****************************************************************************************************
//
//                                      reweighting
//
//*****************************************************************************************************

		dropEvent=false;

		if(isMC) {
			hNTrueInteractionsBeforePUReweighting->Fill(nTrueInteractions);
      PUWeight = 1; 
      dropEvent = true;
			if(leadingjetrawpt >= 170. && leadingjetrawpt < 230.) {
				PUWeight = myPUReweighter_HLT_PFJet140.weight(nTrueInteractions);
        dropEvent = false;
			}
			else if(leadingjetrawpt >= 230. && leadingjetrawpt < 290.) {
				PUWeight = myPUReweighter_HLT_PFJet200.weight(nTrueInteractions);			
        dropEvent = false;
			}
			else if(leadingjetrawpt >= 290. && leadingjetrawpt < 350.) {
				PUWeight = myPUReweighter_HLT_PFJet260.weight(nTrueInteractions);			
        dropEvent = false;
			}
			else if(leadingjetrawpt >= 350.) {
				PUWeight = myPUReweighter_HLT_PFJet320.weight(nTrueInteractions);		
        dropEvent = false;
			}
			//PUWeight = myPUReweighter.weight(nTrueInteractions);
			hNTrueInteractionsAfterPUReweighting->Fill(nTrueInteractions,PUWeight);
			weight = lumiWeight*PUWeight;
		}
    //cout<<"isMC: "<<isMC<<endl;
    //cout<<"!isMC: "<<!isMC<<endl;
		if(!isMC) {
			dropEvent = true;
      //cout<<"leadingjetpt: "<<leadingjetpt<<endl;
      //cout<<" HLT_vector->size(): "<< HLT_vector->size()<<endl;
			if(leadingjetrawpt >= 170. && leadingjetrawpt < 230.) {
				for(int i = 0; i < HLT_vector->size(); i++) {
          //cout<<"HLT_vector->at("<<i<<")"<< HLT_vector->at(i) <<endl;
          //cout<<"leadingjetpt"<<leadingjetpt<<endl;
					if(TString(HLT_vector->at(i)).Contains("HLT_PFJet140")) {
						dropEvent = false;
						weight = HLT_PFJet140_prescaleFactor();
						hLeadingJetPt_170to230->Fill(leadingjetpt, weight);
            break;
					}
				}								
			}
			else if(leadingjetrawpt >= 230. && leadingjetrawpt < 290.) {
				for(int i = 0; i < HLT_vector->size(); i++) {
					if(TString(HLT_vector->at(i)).Contains("HLT_PFJet200")) {
						dropEvent = false;
						weight = HLT_PFJet200_prescaleFactor();
						hLeadingJetPt_230to290->Fill(leadingjetpt, weight);
            break;
					}	
				}								
			}
			else if(leadingjetrawpt >= 290. && leadingjetrawpt < 350.) {
				for(int i = 0; i < HLT_vector->size(); i++) {
					if(TString(HLT_vector->at(i)).Contains("HLT_PFJet260")) {
						dropEvent = false;
						weight = HLT_PFJet260_prescaleFactor();
						hLeadingJetPt_290to350->Fill(leadingjetpt, weight);
            break;
					}	
				}								
			}
			else if(leadingjetrawpt >= 350.) {
				for(int i = 0; i < HLT_vector->size(); i++) {
          //cout<<"HLT_vector->at("<<i<<")"<< HLT_vector->at(i) <<endl;
          //cout<<"leadingjetpt"<<leadingjetpt<<endl;
					if(TString(HLT_vector->at(i)).Contains("HLT_PFJet320")) {
						dropEvent = false;
						weight = HLT_PFJet320_prescaleFactor();
						hLeadingJetPt_350toInf->Fill(leadingjetpt, weight);
            break;
					}	
				}								
			}
		}
    //cout<<"dropEvent: "<<dropEvent<<endl;
    //if(dropEvent) continue;

//*****************************************************************************************************
//
//                                      filling histogramms
//
//*****************************************************************************************************
		int n_muons_loose = 0;
		if(n_muons != 0) {			
			for(int i=0; i<n_muons; i++) {
				if(muon_isLooseMuon[i] == 1) {
					n_muons_loose ++;
				}
			}
		}
		
		int n_photons_loose = 0;
		if(n_photons != 0) {			
			for(int i=0; i<n_photons; i++) {
				if(photon_isLoosePhoton[i] == 1) {
					n_photons_loose ++;
				}
			}
		}
		
		hMet_beforeSel->Fill(metpt, weight);
		hLeadingJetPt_beforeSel->Fill(leadingjetpt, weight);
		hRecoilPt_beforeSel->Fill(recoilpt, weight);
		for(int i=0; i<jets_recoil_4vector->GetSize(); i++) {
			hRecoilJetsPt_beforeSel->Fill(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt(), weight);
		}
		hNpv_beforeSel->Fill(n_vertices, weight);
		hAlpha_beforeSel->Fill(alpha, weight);
		hBeta_beforeSel->Fill(beta, weight);
		hA_beforeSel->Fill(A, weight);
		
		for(int i=0; i<jet_PF_4vector->GetEntriesFast(); i++) {
			jet_PF_pt = ((TLorentzVector*) jet_PF_4vector->At(i))->Pt();
			hNjet_Npv->Fill(n_vertices, weight);
			hNjet_JetPt->Fill(jet_PF_pt, weight);
			if(jet_PF_puJetFullId[i] < 6) {
				hNrmPuJets_JetPt->Fill(jet_PF_pt, weight);
			}
			if(jet_PF_puJetFullId[i] == 4) {
				hNpuLoosejetTmp_Npv->Fill(n_vertices, weight);
				hNpuLoosejetTmp_JetPt->Fill(jet_PF_pt, weight);
			}
			else if(jet_PF_puJetFullId[i] == 6) {
				hNpuMediumjetTmp_Npv->Fill(n_vertices, weight);
				hNpuMediumjetTmp_JetPt->Fill(jet_PF_pt, weight);
			}
			else if(jet_PF_puJetFullId[i] == 7) {
				hNpuTightjetTmp_Npv->Fill(n_vertices, weight);
				hNpuTightjetTmp_JetPt->Fill(jet_PF_pt, weight);
			}
			if(jet_PF_puJetFullId[i] >= 4) {
				hNpuAlljetTmp_Npv->Fill(n_vertices, weight);
				hNpuAlljetTmp_JetPt->Fill(jet_PF_pt, weight);
			}
		}
    
    if(binRecoilPt < 0) continue;
    if(isMC && binGenPt < 0) continue;
		
		if(isSelected == 1) {
			if(n_muons == 0 || n_muons_loose == 0) {
				if(n_photons == 0 || n_photons_loose == 0) {
					//if(leadingjetpt>350.) {
					//if(recoilpt>250.) {
						//if(A<0.8) {
							//if(secondjetpt < 1450.) {							
								if(dropEvent == false) {
									hRecoilEta->Fill(fabs(recoileta), weight);
									hMJB_inclusive->Fill(MJB, weight);
									hHT->Fill(HT,weight);
									hNjetsRecoil->Fill(jets_recoil_4vector->GetSize(), weight);
									hNjetsTotal->Fill(n_totJets, weight);
									hNjetsGood->Fill(n_goodJets, weight);
									vNjetsRecoil_RecoilPt[binRecoilPt]->Fill(jets_recoil_4vector->GetSize(), weight);
									ptRecoil_tmp = 0;
									for(int i=0; i<jets_recoil_4vector->GetSize(); i++) {
										nRecoilJets_068E ++;
										ptRecoil_tmp = ptRecoil_tmp + ((TLorentzVector*)jets_recoil_4vector->At(i))->Pt();
										if(ptRecoil_tmp >= 0.68 * recoilpt) {
											vNjetsRecoil_068E_RecoilPt[binRecoilPt]->Fill(nRecoilJets_068E, weight);
											break;
										}	
									}
									ptRecoil_tmp = 0;
									for(int i=0; i<jets_recoil_4vector->GetSize(); i++) {
										nRecoilJets_095E ++;
										ptRecoil_tmp = ptRecoil_tmp + ((TLorentzVector*)jets_recoil_4vector->At(i))->Pt();
										if(ptRecoil_tmp >= 0.95 * recoilpt) {
											vNjetsRecoil_095E_RecoilPt[binRecoilPt]->Fill(nRecoilJets_095E, weight);
											break;
										}	
									}
						
									hMet_afterSel->Fill(metpt, weight);
									hLeadingJetPt_afterSel->Fill(leadingjetpt, weight);
									hRecoilPt_afterSel->Fill(recoilpt, weight);
								
									recoilEtaMin = ((TLorentzVector*)jets_recoil_4vector->At(0))->Eta();
									recoilEtaMax = ((TLorentzVector*)jets_recoil_4vector->At(0))->Eta();
									for(int i=0; i<jets_recoil_4vector->GetSize(); i++) {
										recoilEtaMinTmp = ((TLorentzVector*)jets_recoil_4vector->At(i))->Eta();
										recoilEtaMaxTmp = ((TLorentzVector*)jets_recoil_4vector->At(i))->Eta();
										if(recoilEtaMinTmp < recoilEtaMin) {
											recoilEtaMin = recoilEtaMinTmp;
										}
									
										if (recoilEtaMaxTmp > recoilEtaMax) {
											recoilEtaMax = recoilEtaMaxTmp;
										}
										hRecoilJetsPt_afterSel->Fill(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt(), weight);
										if(nRecoilJets<16) {
											vRecoilJetsPt_NrecoilJets[nRecoilJets]->Fill(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt(), weight);
										}
									}
									recoilDeltaEta = fabs(recoilEtaMax - recoilEtaMin);
									hRecoilWidth->Fill(recoilDeltaEta, weight);
								
									hNpv_afterSel->Fill(n_vertices, weight);
									hAlpha_afterSel->Fill(alpha, weight);
									hBeta_afterSel->Fill(beta, weight);
									hA_afterSel->Fill(A, weight);
							
								vMJB_Npv[n_vertices]->Fill(MJB, weight);
								vMJB_RecoilPt[binRecoilPt]->Fill(MJB, weight);
                vRecoilPt_RecoilPt[binRecoilPt]->Fill(recoilpt, weight);
                vLeadingJetPt_RecoilPt[binRecoilPt]->Fill(leadingjetpt, weight);
								vMPF_RecoilPt[binRecoilPt]->Fill(Rmpf, weight);
								vMJB_RecoilEta[binRecoilEta]->Fill(MJB, weight);
							
								for(int i=0; i<(jets_recoil_4vector->GetEntriesFast()+1); i++) {
									//cout<<"jet_puJetId["<<i<<"] : "<<jet_puJetId[i]<<endl;
									//hNjet_Npv->Fill(n_vertices, weight);
									if(i == 0) {
										jetsPt = jetsPt + leadingjetpt;
										//hNjet_JetPt->Fill(leadingjetpt, weight);
									}
									else {
										jetsPt = jetsPt + ((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt();
										//hNjet_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);
									}
									
									/*if(jet_puJetId[goodJetsIndex->at(i)] >= 4) {
										hNpuAlljetTmp_Npv->Fill(n_vertices, weight);
										if(i == 0) {
											puJetsPt = puJetsPt + leadingjetpt;
											hNpuAlljetTmp_JetPt->Fill(leadingjetpt, weight);
										}
										else {
											puJetsPt = puJetsPt + ((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt();
											hNpuAlljetTmp_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);
										}
									}
									if(jet_puJetId[goodJetsIndex->at(i)] < 6) {
										if(i == 0) {
											hNrmPuJets_JetPt->Fill(leadingjetpt, weight);
										}
										else {
											hNrmPuJets_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);
										}
									}
									if(jet_puJetId[goodJetsIndex->at(i)] == 4) {
										hNpuLoosejetTmp_Npv->Fill(n_vertices, weight);
										if(i == 0) {
											hNpuLoosejetTmp_JetPt->Fill(leadingjetpt, weight);                    
										}
										else {
											hNpuLoosejetTmp_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);                    
										}

									}
									else if(jet_puJetId[goodJetsIndex->at(i)] == 6) {
										hNpuMediumjetTmp_Npv->Fill(n_vertices, weight);
										if(i == 0) {
											hNpuMediumjetTmp_JetPt->Fill(leadingjetpt, weight);
										}
                   	else {
												hNpuMediumjetTmp_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);
									 	}

									}
									else if(jet_puJetId[goodJetsIndex->at(i)] == 7) {
										hNpuTightjetTmp_Npv->Fill(n_vertices, weight);
										if(i == 0) {
											hNpuTightjetTmp_JetPt->Fill(leadingjetpt, weight);
										}
											else {
												hNpuTightjetTmp_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);
											}
									}*/
								}
								hFracJetsPt->Fill(puJetsPt/jetsPt,weight);
							
								if(isMC) {
									if(*leadingjetgen != TLorentzVector(0.,0.,0.,0.)) {//check if a gen jet matches the reco jet
										vPtRatio_GenPt[binGenPt]->Fill(recoilpt/leadingjetgenpt, weight);							
										vRtrue_leadingJet_RecoilPt[binRecoilPt]->Fill(Rtrue, weight);
										binJetPt = myLowPtBinning.getPtBin(leadingjetpt);
										if(binJetPt == -1) continue;
										vRtrue_allJets_JetPt[binJetPt]->Fill(Rtrue, weight);
									}
									for (int i = 0; i < n_jets_recoil; i++) {
										if(*((TLorentzVector*)jetsgen_recoil_4vector->At(i)) != TLorentzVector(0.,0.,0.,0.)) {									
											binJetPt = myLowPtBinning.getPtBin(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt());
											if(binJetPt == -1) continue;
											Rtrue = ((TLorentzVector*)jets_recoil_4vector->At(i))->Pt()/((TLorentzVector*)jetsgen_recoil_4vector->At(i))->Pt();
											vRtrue_allJets_JetPt[binJetPt]->Fill(Rtrue, weight);
										}
									}								
									for (int i = 0; i < n_jets_recoil; i++) {
										if(*((TLorentzVector*)jetsgen_recoil_4vector->At(i)) == TLorentzVector(0.,0.,0.,0.)) break;
										recoilrecopt = recoilrecopt + ((TLorentzVector*)jets_recoil_4vector->At(i))->Pt();
										recoilgenpt = recoilgenpt + ((TLorentzVector*)jetsgen_recoil_4vector->At(i))->Pt();
									}
									Rrecoil = recoilrecopt/recoilgenpt;
									vRrecoil_RecoilPt[binRecoilPt]->Fill(Rrecoil, weight);									
								}

                 //}
						   //}      
             //}
            }
					//}
				}
			}		
		}
	}
	
	hNpuLoosejetTmp_Npv->Add(hNjet_Npv, hNpuLoosejetTmp_Npv, 1, -1);
	hNpuMediumjetTmp_Npv->Add(hNjet_Npv, hNpuMediumjetTmp_Npv, 1, -1);
	hNpuTightjetTmp_Npv->Add(hNjet_Npv, hNpuTightjetTmp_Npv, 1, -1);
	hNpuAlljetTmp_Npv->Add(hNjet_Npv, hNpuAlljetTmp_Npv, 1, -1);
	
	hNpuLoosejetTmp_JetPt->Add(hNjet_JetPt, hNpuLoosejetTmp_JetPt, 1, -1);
	hNpuMediumjetTmp_JetPt->Add(hNjet_JetPt, hNpuMediumjetTmp_JetPt, 1, -1);
	hNpuTightjetTmp_JetPt->Add(hNjet_JetPt, hNpuTightjetTmp_JetPt, 1, -1);
	hNpuAlljetTmp_JetPt->Add(hNjet_JetPt, hNpuAlljetTmp_JetPt, 1, -1);
	
 

//*********************************************************************************************************
//
//                                      Output file
//
//*********************************************************************************************************

	//create the output file and write into it
	//string outputName = "output_rootfile/outputMultijetWeight" + typeName + ".root";
	TFile *out = new TFile(outputName.c_str(), "recreate");
	
	out->cd();	

	TDirectory *mjbDir = out->mkdir("MJB","MJB");
	mjbDir->cd();
	TDirectory *ptbinDir = mjbDir->mkdir("recoilPtBin","recoilPtBin");
	ptbinDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Write();
	}
	
	TDirectory *etabinDir = mjbDir->mkdir("recoilEtaBin","recoilEtaBin");
	etabinDir->cd();
	for(int j=0; j<myEtaBinning.getSize(); j++) {
		vMJB_RecoilEta[j]->Write();
	}

	TDirectory *npvbinDir = mjbDir->mkdir("npvBin","npvBin");
	npvbinDir->cd();	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Write();
	}
	
	TDirectory *mpfDir = out->mkdir("MPF","MPF");
	mpfDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RecoilPt[j]->Write();
	}

  TDirectory *leadingJetDir = out->mkdir("leadingJet","leadingJet");
	leadingJetDir->cd();
	TDirectory *ptbin_jet1Dir = leadingJetDir->mkdir("recoilPtBin","recoilPtBin");
	ptbin_jet1Dir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vLeadingJetPt_RecoilPt[j]->Write();
	}
	
	if(isMC) {
		TDirectory *trueDir = out->mkdir("Rtrue","Rtrue");
		trueDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_leadingJet_RecoilPt[j]->Write();
			vRrecoil_RecoilPt[j]->Write();
		}
		for(int j=0; j<myLowPtBinning.getSize(); j++) {
			vRtrue_allJets_JetPt[j]->Write();
		}
	
		TDirectory *ptratioDir = out->mkdir("PtRatio","PtRatio");
		ptratioDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Write();
		}
	}

	if(!isMC) {
		TDirectory *checkHLTDir = out->mkdir("checkHLT","checkHLT");
		checkHLTDir->cd();
    hLeadingJetPt_170to230->Write();
    hLeadingJetPt_230to290->Write();
		hLeadingJetPt_290to350->Write();
		hLeadingJetPt_350toInf->Write();

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
	
	TDirectory *afterSelDir = variablesDir->mkdir("afterSel","afterSel");
	afterSelDir->cd();
	hNjetsRecoil->Write();
	hNjetsTotal->Write();
	hNjetsGood->Write();
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
	hNrmPuJets_JetPt->Write();
	hNpuLoosejet_JetPt->Write();
	hNpuMediumjet_JetPt->Write();
	hNpuTightjet_JetPt->Write();
	hNpuAlljet_JetPt->Write();
	hNjet_Npv->Write();
	hNpuLoosejet_Npv->Write();
	hNpuMediumjet_Npv->Write();
	hNpuTightjet_Npv->Write();
	hNpuAlljet_Npv->Write();
	hHT->Write();
	hFracJetsPt->Write();
	
	TDirectory *PUWeightDir = out->mkdir("PUWeight","PUWeight");
	PUWeightDir->cd();
	hNTrueInteractionsBeforePUReweighting->Write();
	hNTrueInteractionsAfterPUReweighting->Write();
	
	TDirectory *recoilJetsPtDir = out->mkdir("recoilJetsPt","recoilJetsPt");
	recoilJetsPtDir->cd();
	for(int i=0; i<16; i++) {	
		vRecoilJetsPt_NrecoilJets[i]->Write();
	}
	

	out->Close();
	

	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Delete();
		vNjetsRecoil_RecoilPt[j]->Delete();
		vNjetsRecoil_068E_RecoilPt[j]->Delete();
		vNjetsRecoil_095E_RecoilPt[j]->Delete();
	}

	for(int j=0; j<myEtaBinning.getSize(); j++) {
		vMJB_RecoilEta[j]->Delete();
	}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Delete();
	}

	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RecoilPt[j]->Delete();
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
	hMJB_inclusive->Delete();
	hNjet_JetPt->Delete();
	hNrmPuJets_JetPt->Delete();
	hNpuLoosejet_JetPt->Delete();
	hNpuMediumjet_JetPt->Delete();
	hNpuTightjet_JetPt->Delete();
	hNpuAlljet_JetPt->Delete();
	hNjet_Npv->Delete();
	hNpuLoosejet_Npv->Delete();
	hNpuMediumjet_Npv->Delete();
	hNpuTightjet_Npv->Delete();
	hNpuAlljet_Npv->Delete();
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
	hNjetsGood->Delete();
	hRecoilJetsPt_beforeSel->Delete();
	hRecoilJetsPt_afterSel->Delete();
	
	hNTrueInteractionsBeforePUReweighting->Delete();
	hNTrueInteractionsAfterPUReweighting->Delete();
	
	hRecoilEta->Delete();
	hRecoilWidth->Delete();
	
	t_multijet->Delete();
	t_vertices->Delete();
	t_event->Delete();
	
	return 0;
}









