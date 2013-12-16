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
	PUReweighter myPUReweighter;

	
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
	
	//Rmpf per recoilpt
	vector<TH1F*> vMPF_RecoilPt = buildPtVectorH1(myPtBinning,"MPF",nbinsx,xlow,xup) ;

	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Sumw2();
		vMPF_RecoilPt[j]->Sumw2();
	}
	
	//if(isMC) {
		//recoilpt/firstjetgenpt ratio per firstjetgenpt
		vector<TH1F*> vPtRatio_GenPt = buildPtVectorH1(myPtBinning,"PtRatio",nbinsx,xlow,xup) ;
		//Rtrue per recoilpt
		vector<TH1F*> vRtrue_RecoilPt = buildPtVectorH1(myPtBinning,"Rtrue",nbinsx,xlow,xup) ;
	
		//Rrecoil per recoilpt
		vector<TH1F*> vRrecoil_RecoilPt = buildPtVectorH1(myPtBinning,"Rrecoil",nbinsx,xlow,xup) ;
		
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Sumw2();
			vRtrue_RecoilPt[j]->Sumw2();
			vRrecoil_RecoilPt[j]->Sumw2();
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
	hNjetsTotal->SetXTitle("N_{recoil jets}");
	hNjetsTotal->Sumw2();

	TH1F* hNjet_JetPt=new TH1F("hNjet_JetPt","hNjet_JetPt",150,0,3000);
	hNjet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNjet_JetPt->SetYTitle("N_{jets}");
	hNjet_JetPt->Sumw2();
	
	TH1F* hNpujet_JetPt=new TH1F("hNpujet_JetPt","hNpujet_JetPt",150,0,3000);
	hNpujet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
	hNpujet_JetPt->SetYTitle("N_{PU jets}");
	hNpujet_JetPt->Sumw2();
	
	TH1F* hNjet_Npv=new TH1F("hNjet_Npv","hNjet_Npv",70,0,70);
	hNjet_Npv->SetXTitle("N_{PV}");
	hNjet_Npv->SetYTitle("N_{jets}");
	hNjet_Npv->Sumw2();
	
	TH1F* hNpujet_Npv=new TH1F("hNpujet_Npv","hNpujet_Npv",70,0,70);
	hNpujet_Npv->SetXTitle("N_{PV}");
	hNpujet_Npv->SetYTitle("N_{PU jets}");
	hNpujet_Npv->Sumw2();
	
	TH1F* hHT=new TH1F("hHT","hHT",250,0,2500);
	hHT->SetXTitle("HT [GeV/c]");
	hHT->Sumw2();
	
	TH1F* hFracJetsPt=new TH1F("hFracJetsPt","hFracJetsPt",20,0,1);
	hFracJetsPt->SetXTitle("p_{t}^{PU jets}/p_{t}^{totjets}");
	hFracJetsPt->Sumw2();
	
	TH1F* hMet_beforeSel=new TH1F("hMet_beforeSel","hMet_beforeSel",10,0,1000);
	hMet_beforeSel->SetXTitle("MET [GeV/c]");
	hMet_beforeSel->Sumw2();
	
	TH1F* hMet_afterSel=new TH1F("hMet_afterSel","hMet_afterSel",10,0,1000);
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

//*****************************************************************************************************
//
//                                      reading the root file 
//
//*****************************************************************************************************

//retrieve the trees
	TChain* t_multijet = NULL;
	TChain* t_vertices = NULL;
	TChain* t_event = NULL;
	loadChain(inputFiles, "Multijet", t_multijet);
	loadChain(inputFiles, "Vertices", t_vertices);
	loadChain(inputFiles, "event", t_event);



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
	
	TClonesArray* jets_recoil_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("jets_recoil_4vector",&jets_recoil_4vector);
	
	TClonesArray* jetsgen_recoil_4vector = new TClonesArray("TLorentzVector");
	t_multijet->SetBranchAddress("jetsgen_recoil_4vector",&jetsgen_recoil_4vector);
	
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
	
	int n_jets;
	t_multijet->SetBranchAddress("n_jets",&n_jets);

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
	
	
	//Usefull variables
	int binRecoilPt;//bin en recoilpt	
	int binGenPt;//bin en firstjetgenpt	
	float recoilpt;
	float recoilpx;
	float recoilpy;
	float metpx;
	float metpy;
	float metpt;
	float secondjetpt;	
	float leadingjetgenpt;
	float leadingjetpt;
	float Rmpf = -1.;
	float Rtrue = -1.;
	float Rrecoil = -1.;
	float recoilrecopt = -1;
	float recoilgenpt = -1;
	bool dropEvent=false;
	float jetsPt;
	float puJetsPt;
	double PUWeight;
	
	int ntot = 0;
	int nphotonsel = 0;
	int nmuonsel = 0;
	
	int nRecoilJets;
	
	int numberPtBins = myPtBinning.getSize();
	
	//count events in the tree
	unsigned int nEvents = (int)t_multijet->GetEntries();

	
	//loop over them
	for(unsigned int ievt=0; ievt<nEvents; ievt++) {
		//cout<<"event # "<<ievt<<endl;
		t_multijet->GetEntry(ievt);
		t_vertices->GetEntry(ievt);
		t_event->GetEntry(ievt);
		if(ievt%10000 == 0) {
			cout<<"Event # "<<ievt<<", file ended at "<<(ievt*100.)/(nEvents*1.)<<" %"<<endl;
		} 

	
//*********************************************************************************************************

		if(isMC) {
			hNTrueInteractionsBeforePUReweighting->Fill(nTrueInteractions);
			PUWeight = myPUReweighter.weight(nTrueInteractions);
			hNTrueInteractionsAfterPUReweighting->Fill(nTrueInteractions,PUWeight);
			weight = lumiWeight*PUWeight;
		}
		
		dropEvent=false;
		
		jetsPt = 0.;
		puJetsPt = 0.;
		
		int nentries_recoil_4vector = recoil_4vector->GetEntriesFast();
		if(nentries_recoil_4vector == 0) 
		  continue;
		  
		TLorentzVector* recoil = (TLorentzVector*) recoil_4vector->At(0);
		recoilpt = recoil->Pt();
		
		TLorentzVector* leadingjet = (TLorentzVector*) leadingjet_4vector->At(0);
		leadingjetpt = leadingjet->Pt();

		binRecoilPt = myPtBinning.getPtBin(recoilpt);
		if(binRecoilPt == -1) continue;
		
		TLorentzVector* leadingjetgen = NULL;
		if(isMC) {
			leadingjetgen = (TLorentzVector*) leadingjetgen_4vector->At(0);
			leadingjetgenpt = leadingjetgen->Pt();
		
			binGenPt = myPtBinning.getPtBin(leadingjetgenpt);
			if(binGenPt == -1) continue;
			
			Rtrue = leadingjetpt/leadingjetgenpt;
		}
		
		recoilpx = recoil->Px();
		recoilpy = recoil->Py();
		
		
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
		
		if(isSelected == 1) {
			if(n_muons == 0 || n_muons_loose == 0) {
				if(n_photons == 0 || n_photons_loose == 0) {
					if(leadingjetpt>350.) {
						//if(A<0.8) {
							//if(secondjetpt < 1450.) {
								
								hMJB_inclusive->Fill(MJB, weight);
								hHT->Fill(HT,weight);
								hNjetsRecoil->Fill(jets_recoil_4vector->GetSize(), weight);
								hNjetsTotal->Fill(n_jets, weight);
						
								hMet_afterSel->Fill(metpt, weight);
								hLeadingJetPt_afterSel->Fill(leadingjetpt, weight);
								hRecoilPt_afterSel->Fill(recoilpt, weight);
								for(int i=0; i<jets_recoil_4vector->GetSize(); i++) {
									hRecoilJetsPt_afterSel->Fill(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt(), weight);
									if(nRecoilJets<16) {
										vRecoilJetsPt_NrecoilJets[nRecoilJets]->Fill(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt(), weight);
									}
								}
								hNpv_afterSel->Fill(n_vertices, weight);
								hAlpha_afterSel->Fill(alpha, weight);
								hBeta_afterSel->Fill(beta, weight);
								hA_afterSel->Fill(A, weight);
							
								vMJB_Npv[n_vertices]->Fill(MJB, weight);
								vMJB_RecoilPt[binRecoilPt]->Fill(MJB, weight);
								vMPF_RecoilPt[binRecoilPt]->Fill(Rmpf, weight);
							
								for(int i=0; i<n_jets; i++) {
									//cout<<"jet_puJetId["<<i<<"] : "<<jet_puJetId[i]<<endl;
									hNjet_Npv->Fill(n_vertices, weight);
									if(i == 0) {
										jetsPt = jetsPt + leadingjetpt;
										hNjet_JetPt->Fill(leadingjetpt, weight);
									}
									else {
										jetsPt = jetsPt + ((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt();
										hNjet_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);
									}
									if(jet_puJetId[i] == 1) {
										hNpujet_Npv->Fill(n_vertices, weight);
										if(i == 0) {
											puJetsPt = puJetsPt + leadingjetpt;
											hNpujet_JetPt->Fill(leadingjetpt, weight);
										}
										else {
											puJetsPt = puJetsPt + ((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt();
											hNpujet_JetPt->Fill(((TLorentzVector*)jets_recoil_4vector->At(i-1))->Pt(), weight);
										}
									}
								}
								hFracJetsPt->Fill(puJetsPt/jetsPt,weight);
							
								if(isMC) {
									if(*leadingjetgen != TLorentzVector(0.,0.,0.,0.)) {//check if a gen jet matches the reco jet
										vPtRatio_GenPt[binGenPt]->Fill(recoilpt/leadingjetgenpt, weight);							
										vRtrue_RecoilPt[binRecoilPt]->Fill(Rtrue, weight);								
										for (int i = 0; i < n_jets_recoil; i++) {
											recoilrecopt = recoilrecopt + ((TLorentzVector*)jets_recoil_4vector->At(i))->Pt();
											recoilgenpt = recoilgenpt + ((TLorentzVector*)jetsgen_recoil_4vector->At(i))->Pt();
										}
										Rrecoil = recoilrecopt/recoilgenpt;
										vRrecoil_RecoilPt[binRecoilPt]->Fill(Rrecoil, weight);
									}
								}
							//}
						//}
					}
				}
			}		
		}
	}
 

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
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt[j]->Write();
	}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv[j]->Write();
	}
	
	TDirectory *mpfDir = out->mkdir("MPF","MPF");
	mpfDir->cd();
	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMPF_RecoilPt[j]->Write();
	}	
	
	if(isMC) {
		TDirectory *trueDir = out->mkdir("Rtrue","Rtrue");
		trueDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vRtrue_RecoilPt[j]->Write();
			vRrecoil_RecoilPt[j]->Write();
		}
	
		TDirectory *ptratioDir = out->mkdir("PtRatio","PtRatio");
		ptratioDir->cd();
		for(int j=0; j<myPtBinning.getSize(); j++) {
			vPtRatio_GenPt[j]->Write();
		}
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
	
	hNTrueInteractionsBeforePUReweighting->Delete();
	hNTrueInteractionsAfterPUReweighting->Delete();
	
	t_multijet->Delete();
	t_vertices->Delete();
	t_event->Delete();
	
	return 0;
}









