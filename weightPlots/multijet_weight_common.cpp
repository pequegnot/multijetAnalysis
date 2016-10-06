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
#include "../common/HLTPtBinning.h"
#include "../common/logPtPrimeBinning.h"
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

float computeDeltaPhi(float phi1, float phi2) {
	float deltaPhi = TMath::Abs((phi1) - (phi2));
	if(deltaPhi>TMath::Pi()){
		deltaPhi = 2*TMath::Pi()-deltaPhi;
	}
	return deltaPhi;
}

int main (int argc, char** argv) 
{
    //open input file

    //TFile *f=TFile::Open("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/extracted.root"); 

    HLTPtBinning myHLTPtBinning;
    std::vector<float> vHLTPrescaleFactor;
    int numberHLTBins = myHLTPtBinning.getHLTNumber(); 

    bool isMC = false;	
    bool useRecoilPtBin = true;
    bool useRecoilPtHLTBin = false;

    float weight = 1.;
    float lumiWeight = 1.;
    //string typeName = "_data";
    int plotColor = getDataColor();
    string extension = ".pdf";
    int Nevents = 1;
    float Xsection = 1.;
    std::vector<std::string> inputFiles;
    string outputName;
    std::string PUReweighting;

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

        std::vector<TCLAP::ValueArg<float>* > HLTPrescaleFactorArg;
        for (int i = 0; i < numberHLTBins; i++) {
            std::string HLTPath = myHLTPtBinning.getHLTName(i);
            std::string help = HLTPath + " prescaleFactor: put -1 if you don't want to test this trigger";
            HLTPrescaleFactorArg.push_back(new TCLAP::ValueArg<float>("", HLTPath, help, false, -1., "float", cmd));
        }

        TCLAP::ValueArg<string> PUReweightingArg("", "PU", "Data PU distribution used for PU reweighting: nominal? up? down?", false, "nominal", "string", cmd);


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
        useRecoilPtBin = recoilPtBinArg.getValue();
        useRecoilPtHLTBin = recoilPtHLTBinArg.getValue();
        for (int i = 0; i < numberHLTBins; i++) {
            vHLTPrescaleFactor.push_back(HLTPrescaleFactorArg[i]->getValue());
        }
        outputName = outputFileArg.getValue();
        Nevents = NeventsArg.getValue();
        Xsection = XsectionArg.getValue();
        PUReweighting = PUReweightingArg.getValue();

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
    logPtPrimeBinning myLogPtPrimeBinning; 
    npvBinning myNpvBinning;
    etaBinning myEtaBinning;
    ptBinning myLowPtBinning(true);
    myHLTPtBinning.fillHLTPtBins(useRecoilPtHLTBin);

    //PUReweighter myPUReweighter;
    //PUReweighter myPUReweighter_HLT_PFJet140("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet140.root");
    //PUReweighter myPUReweighter_HLT_PFJet200("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet200.root");
    //PUReweighter myPUReweighter_HLT_PFJet260("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet260.root");
    //PUReweighter myPUReweighter_HLT_PFJet320("/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/PatTopProduction/MyDataPileupHistogram_merged_Run2012ABCD_HLT_PFJet320.root");

    // Run1
    /*  std::vector<PUReweighter*> myPUReweighter;*/
    //for(int j=0; j<myHLTPtBinning.getSize(); j++) {
    //std::string HLTPath = myHLTPtBinning.getHLTName(j); 
    //std::string fileName;
    //if (PUReweighting == "nominal") {
    //fileName = "PUDistribution/DataPileupHistogram_merged_Run2012ABCD_" + HLTPath + ".root";
    //}
    //else if (PUReweighting == "up") {
    //fileName = "PUDistribution/Systematics/PUup/DataPileupHistogram_merged_Run2012ABCD_" + HLTPath + ".root";
    //}
    //else if (PUReweighting == "down") {
    //fileName = "PUDistribution/Systematics/PUdown/DataPileupHistogram_merged_Run2012ABCD_" + HLTPath + ".root";
    //}
    //myPUReweighter.push_back(new PUReweighter(fileName));
    /*}  */

    // Run2
    PUReweighter myPUReweighter("/gridgroup/cms/pequegnot/CMSSW/CMSSW_7_6_3_patch2/src/Extractors/MultijetExtractorAnalysis/test/MyDataPileupHistogram_JetHT_merged_miniAOD_woPU_pt10_eta30_notRmPUJets_Fall15_25nsV1_13Feb16.root");



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

    //MJB per pt bin
    vector<TH1F*> vMJB_RefObjPtBin = buildPtVectorH1(myPtBinning,"MJB",nbinsx,xlow,xup) ;
    vector<TH1F*> vMJB_gen_RefObjPtBin = buildPtVectorH1(myPtBinning,"MJB_gen",nbinsx,xlow,xup) ;

    //ptLeadingJet per pt bin
    vector<TH1F*> vLeadingJetPt_RefObjPtBin;

    //RecoilPt per pt bin
    vector<TH1F*> vRecoilPt_RefObjPtBin;

    if(useRecoilPtBin) {
        vLeadingJetPt_RefObjPtBin = buildPtVectorH1(myPtBinning,"LeadingJetPt",190, 100, 2000) ;	
        vRecoilPt_RefObjPtBin = buildBinnedDistriVectorH1(myPtBinning,"RecoilPt",5) ;  
    } 
    else {
        vRecoilPt_RefObjPtBin = buildPtVectorH1(myPtBinning,"RecoilPt",190, 100, 2000) ;	
        vLeadingJetPt_RefObjPtBin = buildBinnedDistriVectorH1(myPtBinning,"LeadingJetPt",5) ;    
    }

    //MJB per eta bin
    vector<TH1F*> vMJB_RefObjEtaBin = buildEtaVectorH1(myEtaBinning,"MJB",nbinsx,xlow,xup) ;

    //Rmpf per pt bin
    vector<TH1F*> vMPF_RefObjPtBin = buildPtVectorH1(myPtBinning,"MPF",nbinsx,xlow,xup) ;
    vector<TH1F*> vMPF_gen_RefObjPtBin = buildPtVectorH1(myPtBinning,"MPF_gen",nbinsx,xlow,xup) ;
    vector<TH1F*> vMPF_corr_RefObjPtBin = buildPtVectorH1(myPtBinning,"MPF_corr",nbinsx,xlow,xup) ;

    //NjetsRecoil per recoilpt
    vector<TH1F*> vNjetsRecoil_RecoilPt = buildPtVectorH1(myPtBinning,"NjetsRecoil",35,0,35) ;
    vector<TH1F*> vNjetsRecoil_068E_RecoilPt = buildPtVectorH1(myPtBinning,"NjetsRecoil_068E",35,0,35) ;
    vector<TH1F*> vNjetsRecoil_095E_RecoilPt = buildPtVectorH1(myPtBinning,"NjetsRecoil_095E",35,0,35) ;
    vector<TH1F*> vMeanLogPt_RecoilPt = buildPtVectorH1(myPtBinning,"MeanLogPt",30,3,6) ;
    vector<TH1F*> vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt = buildPtVectorH1(myPtBinning,"Log_ptrecoil_exp_sum_Fi_log_fi",30,3,6) ;
    vector<TH1F*> vExp_sum_Fi_log_fi_RecoilPt = buildPtVectorH1(myPtBinning,"Exp_sum_Fi_log_fi",30,0,1) ;
    //MJB per logPtPrime bin
    // PtPrime = ptRecoil * exp_sum_Fi_log_fi
    vector<TH1F*> vMJB_LogPtPrimeBin = buildPtVectorH1(myLogPtPrimeBinning,"MJB",nbinsx,xlow,xup) ;
    // InvMJB per logPtPrime bin
    // PtPrime = ptRecoil * exp_sum_Fi_log_fi
    vector<TH1F*> vInvMJB_LogPtPrimeBin = buildPtVectorH1(myLogPtPrimeBinning,"InvMJB",20,0.01,2.) ;


    //HLT ref object pt per HLT pt bin
    vector<TH1F*> vHLTRefObjPt_HLTRefObjPtBin = buildBinnedDistriVectorH1(myHLTPtBinning,"HLTRefObjPt", 5);

    // leadingjetpt distribution for each HLT path trigged
    vector<TH1F*> vLeadingJetPt_perHLTTrigger = buildPtVectorH1(myHLTPtBinning,"LeadingJetPt_perHLTTrigger",190, 100, 2000) ;


    //ptLeadingJet per HLT pt bin
    //vector<TH1F*> vLeadingJetPt_HLTRefObjPtBin = buildBinnedDistriVectorH1(myHLTPtBinning,"LeadingJetPt", 5);;
    vector<TH1F*> vLeadingJetPt_HLTRefObjPtBin = buildPtVectorH1(myHLTPtBinning,"LeadingJetPt_HLTPtBin",190, 100, 2000);

    // Nvtx per HLT recoil bins
    vector<TH1F*> vNvtx_HLTRefObjPtBin = buildPtVectorH1(myHLTPtBinning,"Nvtx_HLTPtBin",70, 0, 70);

    //RecoilPt per HLT pt bin
    vector<TH1F*> vRecoilPt_HLTRefObjPtBin = buildBinnedDistriVectorH1(myHLTPtBinning,"RecoilPt_HLTPtBin", 5);;

    for(int j=0; j<myPtBinning.getSize(); j++) {
        vMJB_RefObjPtBin[j]->Sumw2(); 
        vMJB_gen_RefObjPtBin[j]->Sumw2();
        vMPF_RefObjPtBin[j]->Sumw2();
        vMPF_gen_RefObjPtBin[j]->Sumw2();
        vMPF_corr_RefObjPtBin[j]->Sumw2();
        vLeadingJetPt_RefObjPtBin[j]->Sumw2();
        vRecoilPt_RefObjPtBin[j]->Sumw2();

    }

    for (int i=0; i<myLogPtPrimeBinning.getSize(); i++) {
        vMJB_LogPtPrimeBin[i]->Sumw2();
        vInvMJB_LogPtPrimeBin[i]->Sumw2();
    }

    for(int j=0; j<myHLTPtBinning.getSize(); j++) {
        vHLTRefObjPt_HLTRefObjPtBin[j]->Sumw2();
        vLeadingJetPt_HLTRefObjPtBin[j]->Sumw2();
        vNvtx_HLTRefObjPtBin[j]->Sumw2();
        vLeadingJetPt_perHLTTrigger[j]->Sumw2();
        vRecoilPt_HLTRefObjPtBin[j]->Sumw2();
    }

    for(int j=0; j<myEtaBinning.getSize(); j++) {
        vMJB_RefObjEtaBin[j]->Sumw2();
    }

    //if(isMC) {
    //recoilpt/firstjetgenpt ratio per firstjetgenpt
    vector<TH1F*> vPtRatio_GenPt = buildPtVectorH1(myPtBinning,"PtRatio",nbinsx,xlow,xup) ;
    //Rtrue per pt bin
    vector<TH1F*> vRtrue_leadingJet_RefObjPtBin = buildPtVectorH1(myPtBinning,"Rtrue_leadingJet",nbinsx,xlow,xup) ;

    vector<TH1F*> vRtrue_allJets_JetPt = buildPtVectorH1(myLowPtBinning,"Rtrue_allJets",nbinsx,xlow,xup) ;

    //Rrecoil per pt bin
    vector<TH1F*> vRrecoil_RefObjPtBin = buildPtVectorH1(myPtBinning,"Rrecoil",nbinsx,xlow,xup) ;

    for(int j=0; j<myPtBinning.getSize(); j++) {
        vPtRatio_GenPt[j]->Sumw2();
        vRtrue_leadingJet_RefObjPtBin[j]->Sumw2();
        vRtrue_allJets_JetPt[j]->Sumw2();
        vRrecoil_RefObjPtBin[j]->Sumw2();
        vNjetsRecoil_RecoilPt[j]->Sumw2();
        vNjetsRecoil_068E_RecoilPt[j]->Sumw2();
        vNjetsRecoil_095E_RecoilPt[j]->Sumw2();
        vMeanLogPt_RecoilPt[j]->Sumw2();
        vExp_sum_Fi_log_fi_RecoilPt[j]->Sumw2();
        vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt[j]->Sumw2();
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

    TH1::SetDefaultSumw2(true);

    TH1F* hWeight=new TH1F("hWeight","hWeight",1000,0,1000);
    hWeight->SetXTitle("Event weight");
    hWeight->Sumw2();

    TH1F* hGenWeight=new TH1F("hGenWeight","hGenWeight",100,0,1);
    hGenWeight->SetXTitle("Generator event weight");
    hGenWeight->Sumw2();

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

    TH1F* hNjet_JetPt=new TH1F("hNjet_JetPt","hNjet_JetPt",150,0,3000);
    hNjet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
    hNjet_JetPt->SetYTitle("N_{jets}");
    hNjet_JetPt->Sumw2();

    TH1F* hNpuLoosejetTmp_JetPt=new TH1F("hNpuLoosejetTmp_JetPt","hNpuLoosejetTmp_JetPt",150,0,3000);
    hNpuLoosejetTmp_JetPt->Sumw2();

    TH1F* hNpuMediumjetTmp_JetPt=new TH1F("hNpuMediumjetTmp_JetPt","hNpuMediumjetTmp_JetPt",150,0,3000);
    hNpuMediumjetTmp_JetPt->Sumw2();

    TH1F* hNpuTightjetTmp_JetPt=new TH1F("hNpuTightjetTmp_JetPt","hNpuTightjetTmp_JetPt",150,0,3000);
    hNpuTightjetTmp_JetPt->Sumw2();

    TH1F* hNpuAlljetTmp_JetPt=new TH1F("hNpuAlljetTmp_JetPt","hNpuAlljetTmp_JetPt",150,0,3000);
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
    TH1F* hNrmPuJets_JetPt=new TH1F("hNrmPuJets_JetPt","hNrmPuJets_JetPt",150,0,3000);
    hNrmPuJets_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
    hNrmPuJets_JetPt->SetYTitle("N_{PU jets}");
    hNrmPuJets_JetPt->Sumw2();


    TH1F* hNpuLoosejet_JetPt=new TH1F("hNpuLoosejet_JetPt","hNpuLoosejet_JetPt",150,0,3000);
    hNpuLoosejet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
    hNpuLoosejet_JetPt->SetYTitle("N_{PU loose jets}");
    hNpuLoosejet_JetPt->Sumw2();

    TH1F* hNpuMediumjet_JetPt=new TH1F("hNpuMediumjet_JetPt","hNpuMediumjet_JetPt",150,0,3000);
    hNpuMediumjet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
    hNpuMediumjet_JetPt->SetYTitle("N_{PU medium jets}");
    hNpuMediumjet_JetPt->Sumw2();

    TH1F* hNpuTightjet_JetPt=new TH1F("hNpuTightjet_JetPt","hNpuTightjet_JetPt",150,0,3000);
    hNpuTightjet_JetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
    hNpuTightjet_JetPt->SetYTitle("N_{PU tight jets}");
    hNpuTightjet_JetPt->Sumw2();

    TH1F* hTrueNotPuTightJetPt=new TH1F("hTrueNotPuTightJetPt","hTrueNotPuTightJetPt",150,0,3000);
    hTrueNotPuTightJetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
    hTrueNotPuTightJetPt->Sumw2();

    TH1F* hOtherJetPt=new TH1F("hOtherJetPt","hOtherJetPt",150,0,3000);
    hOtherJetPt->SetXTitle("p_{t}^{jet} [GeV/c]");
    hOtherJetPt->Sumw2();

    TH1F* hNpuAlljet_JetPt=new TH1F("hNpuAlljet_JetPt","hNpuAlljet_JetPt",150,0,3000);
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

    TH1F* hHT_afterSel=new TH1F("hHT_afterSel","hHT_afterSel",300,0,3000);
    hHT_afterSel->SetXTitle("HT [GeV/c]");
    hHT_afterSel->Sumw2();

    TH1F* hHT_beforeSel=new TH1F("hHT_beforeSel","hHT_beforeSel",300,0,3000);
    hHT_beforeSel->SetXTitle("HT [GeV/c]");
    hHT_beforeSel->Sumw2();

    TH1F* hFracJetsPt=new TH1F("hFracJetsPt","hFracJetsPt",20,0,1);
    hFracJetsPt->SetXTitle("p_{t}^{PU jets}/p_{t}^{totjets}");
    hFracJetsPt->Sumw2();

    TH1F* hMet_beforeSel=new TH1F("hMet_beforeSel","hMet_beforeSel",150,0,700);
    hMet_beforeSel->SetXTitle("MET [GeV/c]");
    hMet_beforeSel->Sumw2();

    TH1F* hMet_afterSel=new TH1F("hMet_afterSel","hMet_afterSel",150,0,700);
    hMet_afterSel->SetXTitle("MET [GeV/c]");
    hMet_afterSel->Sumw2();

    TH1F* hJetsPt_beforeSel=new TH1F("hJetsPt_beforeSel","hJetsPt_beforeSel",150,0,3000);
    hJetsPt_beforeSel->SetXTitle("p_{t}^{jet} [GeV/c]");
    hJetsPt_beforeSel->Sumw2();

    TH1F* hJetsEta_beforeSel=new TH1F("hJetsEta_beforeSel","hJetsEta_beforeSel",80,-4.,4.);
    hJetsEta_beforeSel->SetXTitle("#eta^{jet} [rad]");
    hJetsEta_beforeSel->Sumw2();

    TH1F* hJetsPhi_beforeSel=new TH1F("hJetsPhi_beforeSel","hJetsPhi_beforeSel",70,0,7.);
    hJetsPhi_beforeSel->SetXTitle("#phi^{jet} [rad]");
    hJetsPhi_beforeSel->Sumw2();

    TH1F* hJetsPt_afterSel=new TH1F("hJetsPt_afterSel","hJetsPt_afterSel",150,0,3000);
    hJetsPt_afterSel->SetXTitle("p_{t}^{jet} [GeV/c]");
    hJetsPt_afterSel->Sumw2();

    TH1F* hJetsEta_afterSel=new TH1F("hJetsEta_afterSel","hJetsEta_afterSel",80,-4.,4.);
    hJetsEta_afterSel->SetXTitle("#eta^{jet} [rad]");
    hJetsEta_afterSel->Sumw2();

    TH1F* hJetsPhi_afterSel=new TH1F("hJetsPhi_afterSel","hJetsPhi_afterSel",80,-4.,4.);
    hJetsPhi_afterSel->SetXTitle("#phi^{jet} [rad]");
    hJetsPhi_afterSel->Sumw2();

    TH1F* hLeadingJetEta_beforeSel=new TH1F("hLeadingJetEta_beforeSel","hLeadingJetEta_beforeSel",80,-4.,4.);
    hLeadingJetEta_beforeSel->SetXTitle("#eta^{leading jet} [rad]");
    hLeadingJetEta_beforeSel->Sumw2();

    TH1F* hRecoilEta_afterSel=new TH1F("hRecoilEta_afterSel","hRecoilEta_afterSel",80,-4.,4.);
    hRecoilEta_afterSel->SetXTitle("#eta^{leading jet} [rad]");
    hRecoilEta_afterSel->Sumw2();

    TH1F* hRecoilEta_beforeSel=new TH1F("hRecoilEta_beforeSel","hRecoilEta_beforeSel",80,-4.,4.);
    hRecoilEta_beforeSel->SetXTitle("#eta^{leading jet} [rad]");
    hRecoilEta_beforeSel->Sumw2();

    TH1F* hLeadingJetEta_afterSel=new TH1F("hLeadingJetEta_afterSel","hLeadingJetEta_afterSel",80,-4.,4.);
    hLeadingJetEta_afterSel->SetXTitle("#eta^{leading jet} [rad]");
    hLeadingJetEta_afterSel->Sumw2();

    TH1F* hDeltaPhi_METRecoil_afterSel=new TH1F("hDeltaPhi_METRecoil_afterSel","hDeltaPhi_METRecoil_afterSel",40,0,4);
    hDeltaPhi_METRecoil_afterSel->SetXTitle("|#Delta#phi (MET,Recoil)|");
    hDeltaPhi_METRecoil_afterSel->Sumw2();

    TH1F* hDeltaPhi_METJet1_afterSel=new TH1F("hDeltaPhi_METJet1_afterSel","hDeltaPhi_METJet1_afterSel",40,0,4);
    hDeltaPhi_METJet1_afterSel->SetXTitle("|#Delta#phi (MET,1^{st}jet)|");
    hDeltaPhi_METJet1_afterSel->Sumw2();

    TH1F* hLeadingJetPt_beforeSel=new TH1F("hLeadingJetPt_beforeSel","hLeadingJetPt_beforeSel",150,0,3000);
    hLeadingJetPt_beforeSel->SetXTitle("p_{t}^{leading jet} [GeV/c]");
    hLeadingJetPt_beforeSel->Sumw2();

    TH1F* hLeadingJetPt_afterSel=new TH1F("hLeadingJetPt_afterSel","hLeadingJetPt_afterSel",150,0,3000);
    hLeadingJetPt_afterSel->SetXTitle("p_{t}^{leading jet} [GeV/c]");
    hLeadingJetPt_afterSel->Sumw2();

    TH1F* hLeadingJetMass_afterSel=new TH1F("hLeadingJetMass_afterSel","hLeadingJetMass_afterSel",150,0,3000);
    hLeadingJetMass_afterSel->SetXTitle("M^{leading jet} [GeV/c]");
    hLeadingJetMass_afterSel->Sumw2();

    TH1F* hLeadingAndSecondJetsMass_afterSel=new TH1F("hLeadingAndSecondJetsMass_afterSel","hLeadingAndSecondJetsMass_afterSel",150,0,3000);
    hLeadingAndSecondJetsMass_afterSel->SetXTitle("M^{1^{st}+2^{nd} jets} [GeV/c]");
    hLeadingAndSecondJetsMass_afterSel->Sumw2();

    TH1F* hSecondJetPt_beforeSel=new TH1F("hSecondJetPt_beforeSel","hSecondJetPt_beforeSel",150,0,3000);
    hSecondJetPt_beforeSel->SetXTitle("p_{t}^{second jet} [GeV/c]");
    hSecondJetPt_beforeSel->Sumw2();

    TH1F* hSecondJetPt_afterSel=new TH1F("hSecondJetPt_afterSel","hSecondJetPt_afterSel",150,0,3000);
    hSecondJetPt_afterSel->SetXTitle("p_{t}^{second jet} [GeV/c]");
    hSecondJetPt_afterSel->Sumw2();


    TH1F* hRecoilPt_beforeSel=new TH1F("hRecoilPt_beforeSel","hRecoilPt_beforeSel",150,0,3000);
    hRecoilPt_beforeSel->SetXTitle("p_{t}^{Recoil} [GeV/c]");
    hRecoilPt_beforeSel->Sumw2();

    TH1F* hRecoilPt_afterSel=new TH1F("hRecoilPt_afterSel","hRecoilPt_afterSel",150,0,3000);
    hRecoilPt_afterSel->SetXTitle("p_{t}^{Recoil} [GeV/c]");
    hRecoilPt_afterSel->Sumw2();

    TH2F* h2LeadingJet_Recoil_pt_beforeSel = new TH2F("h2LeadingJet_Recoil_pt_beforeSel", "h2LeadingJet_Recoil_pt_beforeSel", 150,0,3000, 150,0,3000);
    h2LeadingJet_Recoil_pt_beforeSel->SetXTitle("p_{t}^{leading jet} [GeV/c]");
    h2LeadingJet_Recoil_pt_beforeSel->SetYTitle("p_{t}^{Recoil} [GeV/c]");
    h2LeadingJet_Recoil_pt_beforeSel->Sumw2();

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


    TH1F* hBeta_beforeSel=new TH1F("hBeta_beforeSel","hBeta_beforeSel",60,-3.15,3.15);
    hBeta_beforeSel->SetXTitle("#beta (rad)");
    hBeta_beforeSel->Sumw2();

    TH1F* hBeta_afterSel=new TH1F("hBeta_afterSel","hBeta_afterSel",60,-3.15,3.15);
    hBeta_afterSel->SetXTitle("#beta (rad)");
    hBeta_afterSel->Sumw2();


    TH1F* hA_beforeSel=new TH1F("hA_beforeSel","hA_beforeSel",20,0,1);
    hA_beforeSel->SetXTitle("A = p_{t}^{jet 2}/p_{t}^{Recoil}");
    hA_beforeSel->Sumw2();

    TH1F* hA_afterSel=new TH1F("hA_afterSel","hA_afterSel",20,0,1);
    hA_afterSel->SetXTitle("A = p_{t}^{jet 2}/p_{t}^{Recoil}");
    hA_afterSel->Sumw2();


    TH1F* hRecoilWidth=new TH1F("hRecoilWidth","hRecoilWidth",50,0,10);
    hRecoilWidth->SetXTitle("Recoil width (#Delta #eta = |#eta^{Recoil jet}_{max} - #eta^{Recoil jet}_{min}|)");
    hRecoilWidth->Sumw2();

    TH1F* hNjets_ptSup30_etaInf5_beforeSel = new TH1F("hNjets_ptSup30_etaInf5_beforeSel", "hNjets_ptSup30_etaInf5_beforeSel", 25, 0, 25);
    hNjets_ptSup30_etaInf5_beforeSel->SetXTitle("N_{jets} with p_{t} > 30 GeV and |#eta| < 5.0");
    hNjets_ptSup30_etaInf5_beforeSel->Sumw2();

    TH1F* hNjets_ptSup10_beforeSel = new TH1F("hNjets_ptSup10_beforeSel", "hNjets_ptSup10_beforeSel", 25, 0, 25);
    hNjets_ptSup10_beforeSel->SetXTitle("N_{jets} with p_{t} > 10 GeV");
    hNjets_ptSup10_beforeSel->Sumw2();

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

    TClonesArray* genrecoil_4vector = NULL;
    t_multijet->SetBranchAddress("genrecoil_4vector",&genrecoil_4vector);

    TClonesArray* met_4vector = new TClonesArray("TLorentzVector");
    t_multijet->SetBranchAddress("met_4vector",&met_4vector);

    TClonesArray* genmet_4vector = NULL;
    t_multijet->SetBranchAddress("genmet_4vector",&genmet_4vector);

    TClonesArray* met_corr_4vector = new TClonesArray("TLorentzVector");
    //t_multijet->SetBranchAddress("met_puSubstract_4vector",&met_corr_4vector);

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
    //t_multijet->SetBranchAddress("beta",&beta);

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

    float generator_weight;
    t_event->SetBranchAddress("generator_weight",&generator_weight);

    unsigned int run;
    t_event->SetBranchAddress("run",&run);

    unsigned int lumi;
    t_event->SetBranchAddress("lumi",&lumi);

    /*    std::cout << "Fill generator weight..." << std::endl;*/
    //for(unsigned int ievt=0; ievt<(int)t_event->GetEntries(); ievt++) {
    //t_event->GetEntry(ievt);
    //hGenWeight->Fill(generator_weight, generator_weight);
    //}
    //TFile *outgen = new TFile("genWeight.root", "recreate");
    //outgen->cd();
    //hGenWeight->Write();
    //outgen->Close();
    //std::cout << "Fill generator weight: done..." << std::endl;

    //jet_PF
    TClonesArray* jet_PF_4vector = new TClonesArray("TLorentzVector");
    t_jet_PF->SetBranchAddress("jet_4vector",&jet_PF_4vector);	

    int jet_PF_puJetFullId[100];
    t_jet_PF->SetBranchAddress("jet_puJetFullId",&jet_PF_puJetFullId);

    //HLT
    std::vector<std::string>* HLT_vector = NULL;
    t_HLT->SetBranchAddress("HLT_vector",&HLT_vector);


    //Usefull variables
    int binRefObjPt;
    int binRefObjEta;

    int binHLTRefObjPt;

    float HLTRefObjPt;

    int binLogPtPrime;
    int binRecoilPt;//bin en recoilpt
    int binHLTRecoilPt;//bin en recoilpt
    int binLeadingJetRawPt;//bin en leading jet raw pt
    int binHLTLeadingJetRawPt;//bin en leading jet raw pt
    int binLeadingJetPt;
    int binHLTLeadingJetPt;
    int binRecoilEta;//bin en recoileta		
    int binLeadingJetEta;//bin en leading jet eta		
    int binGenPt;//bin en firstjetgenpt
    int binGenRecoilPt;//bin en firstjetgenpt
    int binJetPt;//bin en pt des jets		
    float recoilpt;
    float recoilpx;
    float recoilpy;
    float recoileta;
    float recoilpt_gen;
    float recoilpx_gen;
    float recoilpy_gen;
    float recoileta_gen;
    float metpx;
    float metpy;
    float metpt;
    float metphi;
    float metpx_corr;
    float metpy_corr;
    float metpt_corr;
    float metphi_corr;
    float metpx_gen;
    float metpy_gen;
    float metpt_gen;
    float metphi_gen;
    float secondjetpt;	
    float leadingjetgenpt;
    float leadingjetrawpt;
    float leadingjetpt;
    float leadingjetmass;
    float leadingandsecondjetsmass;
    float leadingjeteta;
    float MJB_gen = -1.;
    float Rmpf = -1.;
    float Rmpf_gen = -1.;
    float Rmpf_corr = -1.;
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
    float Njets_ptSup30_etaInf5_beforeSel;
    float Njets_ptSup10_beforeSel;
    float leadingjetphi;
    float recoilphi;
    Double_t DeltaPhi_METRecoil;
    Double_t DeltaPhi_METJet1;
    double meanLogPt;
    double exp_sum_Fi_log_fi;
    double ptPrime; // PtPrime = ptRecoil * exp_sum_Fi_log_fi
    double Fi, fi, deltaPhiJetRecoil;


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

        met_corr_4vector = met_4vector;

        if(ievt%10000 == 0) {
            cout<<"Event # "<<ievt<<", file ended at "<<(ievt*100.)/(nEvents*1.)<<" %"<<endl;
        } 
        weight = 1.;
        //cout << "weight: " << weight << endl;

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

        if(isSelected == 1) {

            //*****************************************************************************************************
            //
            //                                      dynamic variables computation
            //
            //*****************************************************************************************************

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
            recoilphi = recoil->Phi();

            TLorentzVector* leadingjet = (TLorentzVector*) leadingjet_4vector->At(0);
            leadingjetpt = leadingjet->Pt();
            leadingjetphi = leadingjet->Phi();
            leadingjeteta = leadingjet->Eta();
            leadingjetmass = leadingjet->M();

            TLorentzVector* leadingjetraw = (TLorentzVector*) leadingjetraw_4vector->At(0);
            leadingjetrawpt = leadingjetraw->Pt();

            //std::cout << "leadingjetpt: " << leadingjetpt << std::endl;
            //std::cout << "recoilpt: " << recoilpt << std::endl;

            binRecoilPt = myPtBinning.getPtBin(recoilpt);
            binHLTRecoilPt = myHLTPtBinning.getPtBin(recoilpt);
            binHLTLeadingJetRawPt = myHLTPtBinning.getPtBin(leadingjetrawpt);
            binLeadingJetRawPt = myPtBinning.getPtBin(leadingjetrawpt);
            binHLTLeadingJetPt = myHLTPtBinning.getPtBin(leadingjetpt);
            binLeadingJetPt = myPtBinning.getPtBin(leadingjetpt);

            //std::cout << "binRecoilPt: " << binRecoilPt << std::endl;
            //std::cout << "binHLTRecoilPt: " << binHLTRecoilPt << std::endl;
            //std::cout << "binHLTLeadingJetPt: " << binHLTLeadingJetPt << std::endl;
            //std::cout << "binLeadingJetPt: " << binLeadingJetPt << std::endl;

            recoilpx = recoil->Px();
            recoilpy = recoil->Py();
            recoileta = recoil->Eta();
            binRecoilEta = myEtaBinning.getEtaBin(fabs(recoileta));

            binLeadingJetEta = myEtaBinning.getEtaBin(fabs(leadingjeteta));

            if(useRecoilPtBin) {
                binRefObjPt = binRecoilPt;
                binRefObjEta = binRecoilEta;
            }
            else {
                binRefObjPt = binLeadingJetPt;
                binRefObjEta = binLeadingJetEta;          
            }

            if(useRecoilPtHLTBin) {
                binHLTRefObjPt = binHLTRecoilPt;
                HLTRefObjPt = recoilpt;  
            }
            else {
                binHLTRefObjPt = binHLTLeadingJetPt;
                HLTRefObjPt = leadingjetpt;
            }

            //std::cout << "binRefObjPt: " << binRefObjPt << std::endl;
            //std::cout << "binHLTRefObjPt: " << binHLTRefObjPt << std::endl;

            if(jets_recoil_4vector->GetEntriesFast() == 0) continue;	
            TLorentzVector* secondjet = (TLorentzVector*) jets_recoil_4vector->At(0);// first jet of the recoil system, so index is 0
            secondjetpt = secondjet->Pt();
            TLorentzVector firstandsecondjets = *secondjet + *leadingjet;
            leadingandsecondjetsmass = firstandsecondjets.M();

            TLorentzVector* met = (TLorentzVector*) met_4vector->At(0);
            metpx = met->Px();
            metpy = met->Py();
            metpt = met->Pt();
            metphi = met->Phi();

            TLorentzVector* met_corr = (TLorentzVector*) met_corr_4vector->At(0);
            metpx_corr = met_corr->Px();
            metpy_corr = met_corr->Py();
            metpt_corr = met_corr->Pt();
            metphi_corr = met_corr->Phi();

            TLorentzVector* leadingjetgen = NULL;
            TLorentzVector* met_gen = NULL;
            TLorentzVector* recoil_gen = NULL; 
            if(isMC) {
                leadingjetgen = (TLorentzVector*) leadingjetgen_4vector->At(0);
                leadingjetgenpt = leadingjetgen->Pt();

                binGenPt = myPtBinning.getPtBin(leadingjetgenpt);

                Rtrue = leadingjetpt/leadingjetgenpt;

                if (genmet_4vector && genmet_4vector->GetEntriesFast() != 0) {
                    met_gen = (TLorentzVector*) genmet_4vector->At(0);
                    metpx_gen = met_gen->Px();
                    metpy_gen = met_gen->Py();
                    metpt_gen = met_gen->Pt();
                    metphi_gen = met_gen->Phi();
                }

                if (genrecoil_4vector && genrecoil_4vector->GetEntriesFast() != 0) {
                    recoil_gen = (TLorentzVector*) genrecoil_4vector->At(0);

                    recoilpt_gen = recoil_gen->Pt();
                    recoilpx_gen = recoil_gen->Px();
                    recoilpy_gen = recoil_gen->Py();
                    recoileta_gen = recoil_gen->Eta();           
                    Rmpf_gen = 1 + (recoilpx_gen*metpx_gen + recoilpy_gen*metpy_gen)/(pow(recoilpt_gen,2));		
                    MJB_gen = leadingjetgenpt/recoilpt_gen;
                }
                binGenRecoilPt = myPtBinning.getPtBin(recoilpt_gen);
            }

            Rmpf = 1 + (recoilpx*metpx + recoilpy*metpy)/(pow(recoilpt,2));		
            Rmpf_corr = 1 + (recoilpx*metpx_corr + recoilpy*metpy_corr)/(pow(recoilpt,2));		

            recoilrecopt = 0.;
            recoilgenpt = 0.;

            nRecoilJets = jets_recoil_4vector->GetSize();

            h2LeadingJet_Recoil_pt_beforeSel->Fill(leadingjetpt, recoilpt);


            //*****************************************************************************************************
            //
            //                                      PU reweighting
            //
            //*****************************************************************************************************

            dropEvent=false;

            if(isMC) {
                hNTrueInteractionsBeforePUReweighting->Fill(nTrueInteractions);
                PUWeight = 1; 
                /*            dropEvent = true;*/
                //for (int j = 0; j<myHLTPtBinning.getSize(); j++) {
                ////if ( HLTRefObjPt >= myHLTPtBinning.getBinValueInf(j) && HLTRefObjPt < myHLTPtBinning.getBinValueSup(j) ) {
                //if ( binHLTRefObjPt == j ) {
                ////std::cout << "HLTPath for PUReweighting: " << myHLTPtBinning.getHLTName(j) << std::endl;
                //PUWeight = myPUReweighter[j]->weight(nTrueInteractions); 
                //dropEvent = false;
                //}
                /*}*/

                PUWeight = myPUReweighter.weight(nTrueInteractions);
                hNTrueInteractionsAfterPUReweighting->Fill(nTrueInteractions,PUWeight);
                weight = lumiWeight*PUWeight;

                // generator weight
                weight *= generator_weight;
            }

            //*****************************************************************************************************
            //
            //                                      HLT prescale reweighting
            //
            //*****************************************************************************************************

            if (!isMC) {
                for (int j = 0; j<myHLTPtBinning.getSize(); j++) {
                    for (int i = 0; i < HLT_vector->size(); i++) {
                        if (TString(HLT_vector->at(i)).Contains(myHLTPtBinning.getHLTName(j))) {
                            vLeadingJetPt_perHLTTrigger[j]->Fill(leadingjetpt, weight);
                        }
                    }
                }


                dropEvent = true;
/*                cout<<"leadingjetpt: "<<leadingjetpt<<endl;*/
                //cout<<"recoilpt: "<<recoilpt<<endl;
                //cout<<"binHLTRefObjPt: "<<binHLTRefObjPt<<endl;
                /*cout<<" HLT_vector->size(): "<< HLT_vector->size()<<endl;*/

                for (int j = 0; j<myHLTPtBinning.getSize(); j++) {
                    if (binHLTRefObjPt == j && vHLTPrescaleFactor[j] != -1) {

                        bool passTrigger = false;
                        bool passHigherPtTrigger = false;
                        for (int i = 0; i < HLT_vector->size(); i++) {
                          if (TString(HLT_vector->at(i)).Contains(myHLTPtBinning.getHLTName(j))) {
/*                            cout<<"leadingjetpt: "<<leadingjetpt<<endl;*/
                            //cout<<"recoilpt: "<<recoilpt<<endl;
                            //cout<<"binHLTRefObjPt: "<<binHLTRefObjPt<<endl;
                            //cout<<" HLT_vector->size(): "<< HLT_vector->size()<<endl;
                            /*std::cout << "HLTPath for HLTPrescale: " << myHLTPtBinning.getHLTName(j) << std::endl;*/
                            passTrigger = true;
                                break;
                            }
                        }	

                        if (passTrigger) {
                            weight = vHLTPrescaleFactor[j];
                            //std::cout << "prescale weight: " << vHLTPrescaleFactor[j] << std::endl;
                            vHLTRefObjPt_HLTRefObjPtBin[j]->Fill(HLTRefObjPt, weight);
                            dropEvent = false;
                        }
                        break;
                    }
                }
            }
            //cout<<"dropEvent: "<<dropEvent<<endl;
            //if(dropEvent) continue;
            //cout<<"event weight after HLT prescale: " << weight << endl;

            hWeight->Fill(weight,1);

            //*****************************************************************************************************
            //
            //                                     HLT selection 
            //
            //*****************************************************************************************************

            //HLT selection
            if(dropEvent == false) {

                //************************************************************************************************************
                //
                //                                      deltaPhi calculation
                //
                //************************************************************************************************************

                //DeltaPhi between MET and the recoil
                DeltaPhi_METRecoil = TMath::Abs((metphi) - (recoilphi));
                if(DeltaPhi_METRecoil>TMath::Pi()){
                    DeltaPhi_METRecoil = 2*TMath::Pi()-DeltaPhi_METRecoil;
                }

                //DeltaPhi between MET and the leading jet 
                DeltaPhi_METJet1 = TMath::Abs((metphi) - (leadingjetphi));
                if(DeltaPhi_METJet1>TMath::Pi()){
                    DeltaPhi_METJet1 = 2*TMath::Pi()-DeltaPhi_METJet1;
                }

                //*****************************************************************************************************
                //
                //                  beta calculation 
                //
                //*****************************************************************************************************
	
                beta = -1;
                int indexClosestJet = 0;
                float phiClosestJet = ((TLorentzVector*)jets_recoil_4vector->At(0))->Phi();
                beta = computeDeltaPhi(leadingjetphi, phiClosestJet);
                for (int i = 1; i < nRecoilJets; i++) {
                  TLorentzVector *jetP = ((TLorentzVector*)jets_recoil_4vector->At(i));
                  float phiJet_tmp = jetP->Phi();
                  float deltaPhi_tmp = computeDeltaPhi(leadingjetphi, phiJet_tmp);
                  if(deltaPhi_tmp<beta) {
                    indexClosestJet = i;
                    beta = deltaPhi_tmp;
                  }			
                }

                //*****************************************************************************************************
                //
                //                                      filling histogramms
                //
                //*****************************************************************************************************


                hMet_beforeSel->Fill(metpt, weight);
                hLeadingJetPt_beforeSel->Fill(leadingjetpt, weight);
                hSecondJetPt_beforeSel->Fill(secondjetpt, weight);
                hRecoilPt_beforeSel->Fill(recoilpt, weight);
                for(int i=0; i<jets_recoil_4vector->GetSize(); i++) {
                    hRecoilJetsPt_beforeSel->Fill(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt(), weight);
                }
                for(int i=0; i<(jets_recoil_4vector->GetEntriesFast()+1); i++) {
                    if(i == 0) {
                        hJetsPt_beforeSel->Fill(leadingjetpt, weight);
                        hJetsEta_beforeSel->Fill(leadingjet->Eta(), weight);
                        hLeadingJetEta_beforeSel->Fill(leadingjet->Eta(), weight);
                        hJetsPhi_beforeSel->Fill(leadingjet->Phi(), weight);
                    }
                    else {
                        TLorentzVector* jetP4 = (TLorentzVector*)jets_recoil_4vector->At(i-1); 
                        hJetsPt_beforeSel->Fill(jetP4->Pt(), weight);
                        hJetsEta_beforeSel->Fill(jetP4->Eta(), weight);
                        hRecoilEta_beforeSel->Fill(jetP4->Eta(), weight);
                        hJetsPhi_beforeSel->Fill(jetP4->Phi(), weight);
                    }
                }
                hNpv_beforeSel->Fill(n_vertices, weight);
                hAlpha_beforeSel->Fill(alpha, weight);
                hBeta_beforeSel->Fill(beta, weight);
                hA_beforeSel->Fill(A, weight);
                hHT_beforeSel->Fill(HT, weight);

                if(binHLTRefObjPt >= 0) {
                    vLeadingJetPt_HLTRefObjPtBin[binHLTRefObjPt]->Fill(leadingjetpt, weight);
                    vRecoilPt_HLTRefObjPtBin[binHLTRefObjPt]->Fill(recoilpt, weight);
                }

                Njets_ptSup30_etaInf5_beforeSel = 0;
                Njets_ptSup10_beforeSel = 0;
                for(int i = 0; i < goodJetsIndex->size(); i++) {
                    TLorentzVector* goodjet_PF = (TLorentzVector*) jet_PF_4vector->At( goodJetsIndex->at(i) );
                    if(goodjet_PF->Pt() < 30. && fabs(goodjet_PF->Eta() < 5.0)) {Njets_ptSup30_etaInf5_beforeSel ++;}
                    if(goodjet_PF->Pt() < 10.) {Njets_ptSup10_beforeSel ++;}
                }
                hNjets_ptSup30_etaInf5_beforeSel->Fill(Njets_ptSup30_etaInf5_beforeSel, weight);
                hNjets_ptSup10_beforeSel->Fill(Njets_ptSup10_beforeSel, weight);
                for(int i=0; i<jet_PF_4vector->GetEntriesFast(); i++) {
                    jet_PF_pt = ((TLorentzVector*) jet_PF_4vector->At(i))->Pt();
                    hNjet_Npv->Fill(n_vertices, weight);
                    hNjet_JetPt->Fill(jet_PF_pt, weight);
                    if(jet_PF_puJetFullId[i] < 7) {
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

                    if(jet_PF_puJetFullId[i] == 7) {
                        hTrueNotPuTightJetPt->Fill(jet_PF_pt, weight);
                    }
                    else {
                        hOtherJetPt->Fill(jet_PF_pt, weight);
                    }
                }

                if(binRefObjPt < 0) continue;

                //angular selection
                //if(alpha < 0.3 && beta > 1.0) {
                //*****************************************************************************************************
                //
                //                  New beta cut: beta>1 for jets with e.g. (pT>30 GeV and pT>0.05*pTrecoil) 
                //
                //*****************************************************************************************************
                if(alpha < 0.3 && ( ((TLorentzVector*)jets_recoil_4vector->At(indexClosestJet))->Pt() > 0.05*recoilpt && beta > 1.0 ) ) {

                    //second jet selection
                    if(A < 0.6) {

                        //first jet selection
                        if(fabs(leadingjet->Eta()) < 1.3) {


                            hRecoilEta->Fill(fabs(recoileta), weight);
                            hDeltaPhi_METRecoil_afterSel->Fill(DeltaPhi_METRecoil, weight);
                            hDeltaPhi_METJet1_afterSel->Fill(DeltaPhi_METJet1, weight);
                            hMJB_inclusive->Fill(MJB, weight);
                            hHT_afterSel->Fill(HT,weight);
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

                            meanLogPt = 0.;
                            for (int i=0; i<jets_recoil_4vector->GetSize(); i++) {
                                meanLogPt = meanLogPt + TMath::Log(((TLorentzVector*)jets_recoil_4vector->At(i))->Pt());
                            }
                            meanLogPt = meanLogPt/((float) jets_recoil_4vector->GetSize());
                            vMeanLogPt_RecoilPt[binRecoilPt]->Fill(meanLogPt, weight);

                            exp_sum_Fi_log_fi = 0.;                      
                            for (int i=0; i<jets_recoil_4vector->GetSize(); i++) {
                                fi = ((TLorentzVector*)jets_recoil_4vector->At(i))->Pt()/recoilpt;
                                deltaPhiJetRecoil = TMath::Abs(((TLorentzVector*)jets_recoil_4vector->At(i))->Phi() - recoilphi);
                                if(deltaPhiJetRecoil>TMath::Pi()){
                                    deltaPhiJetRecoil = 2*TMath::Pi()-deltaPhiJetRecoil;
                                }
                                Fi = fi * TMath::Cos(deltaPhiJetRecoil);
                                exp_sum_Fi_log_fi = exp_sum_Fi_log_fi + Fi * TMath::Log(fi);
                            }
                            exp_sum_Fi_log_fi = TMath::Exp(exp_sum_Fi_log_fi);
                            vExp_sum_Fi_log_fi_RecoilPt[binRecoilPt]->Fill(exp_sum_Fi_log_fi, weight);
                            vLog_ptrecoil_exp_sum_Fi_log_fi_RecoilPt[binRecoilPt]->Fill(TMath::Log(recoilpt*exp_sum_Fi_log_fi), weight);

                            ptPrime = recoilpt * exp_sum_Fi_log_fi;
                            binLogPtPrime = myLogPtPrimeBinning.getPtBin(TMath::Log(ptPrime));
                            if (binLogPtPrime != -1) {
                                vMJB_LogPtPrimeBin[binLogPtPrime]->Fill(MJB, weight);
                                vInvMJB_LogPtPrimeBin[binLogPtPrime]->Fill(1./MJB, weight);
                            }

                            if(binHLTRefObjPt >= 0) {
                                vNvtx_HLTRefObjPtBin[binHLTRefObjPt]->Fill(n_vertices, weight);
                            }

                            hMet_afterSel->Fill(metpt, weight);
                            hLeadingJetPt_afterSel->Fill(leadingjetpt, weight);
                            hLeadingJetMass_afterSel->Fill(leadingjetmass, weight);
                            hLeadingAndSecondJetsMass_afterSel->Fill(leadingandsecondjetsmass, weight);
                            hSecondJetPt_afterSel->Fill(secondjetpt, weight);
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
                            vMJB_RefObjPtBin[binRefObjPt]->Fill(MJB, weight);
                            vRecoilPt_RefObjPtBin[binRefObjPt]->Fill(recoilpt, weight);
                            vLeadingJetPt_RefObjPtBin[binRefObjPt]->Fill(leadingjetpt, weight);
                            vMPF_RefObjPtBin[binRefObjPt]->Fill(Rmpf, weight);
                            vMPF_corr_RefObjPtBin[binRefObjPt]->Fill(Rmpf_corr, weight);
                            vMJB_RefObjEtaBin[binRefObjEta]->Fill(MJB, weight);

                            for(int i=0; i<(jets_recoil_4vector->GetEntriesFast()+1); i++) {
                                //cout<<"jet_puJetId["<<i<<"] : "<<jet_puJetId[i]<<endl;
                                //hNjet_Npv->Fill(n_vertices, weight);
                                if(i == 0) {
                                    jetsPt = jetsPt + leadingjetpt;
                                    hJetsPt_afterSel->Fill(leadingjetpt, weight);
                                    hJetsEta_afterSel->Fill(leadingjet->Eta(), weight);
                                    hLeadingJetEta_afterSel->Fill(leadingjet->Eta(), weight);
                                    hJetsPhi_afterSel->Fill(leadingjet->Phi(), weight);
                                }
                                else {
                                    TLorentzVector* jetP4 = (TLorentzVector*)jets_recoil_4vector->At(i-1); 
                                    jetsPt = jetsPt + (jetP4)->Pt();
                                    hJetsPt_afterSel->Fill(jetP4->Pt(), weight);
                                    hJetsEta_afterSel->Fill(jetP4->Eta(), weight);
                                    hRecoilEta_afterSel->Fill(jetP4->Eta(), weight);
                                    hJetsPhi_afterSel->Fill(jetP4->Phi(), weight);
                                }
                            }
                            hFracJetsPt->Fill(puJetsPt/jetsPt,weight);

                            if(isMC) {
                                if(isMC && (binGenPt < 0 || binGenRecoilPt < 0)) continue;
                                vMPF_gen_RefObjPtBin[binGenRecoilPt]->Fill(Rmpf_gen, weight);
                                vMJB_gen_RefObjPtBin[binGenRecoilPt]->Fill(MJB_gen, weight);
                                if(*leadingjetgen != TLorentzVector(0.,0.,0.,0.)) {//check if a gen jet matches the reco jet
                                    vPtRatio_GenPt[binGenPt]->Fill(recoilpt/leadingjetgenpt, weight);							
                                    vRtrue_leadingJet_RefObjPtBin[binRefObjPt]->Fill(Rtrue, weight);
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
                                vRrecoil_RefObjPtBin[binRefObjPt]->Fill(Rrecoil, weight);									
                            }


                        }      
                    }
                }
            }
        }
    }
	
	hNpuLoosejet_Npv->Add(hNjet_Npv, hNpuLoosejetTmp_Npv, 1, -1);
	hNpuMediumjet_Npv->Add(hNjet_Npv, hNpuMediumjetTmp_Npv, 1, -1);
	hNpuTightjet_Npv->Add(hNjet_Npv, hNpuTightjetTmp_Npv, 1, -1);
	hNpuAlljet_Npv->Add(hNjet_Npv, hNpuAlljetTmp_Npv, 1, -1);
	
	hNpuLoosejet_JetPt->Add(hNjet_JetPt, hNpuLoosejetTmp_JetPt, 1, -1);
	hNpuMediumjet_JetPt->Add(hNjet_JetPt, hNpuMediumjetTmp_JetPt, 1, -1);
	hNpuTightjet_JetPt->Add(hNjet_JetPt, hNpuTightjetTmp_JetPt, 1, -1);
	hNpuAlljet_JetPt->Add(hNjet_JetPt, hNpuAlljetTmp_JetPt, 1, -1);
	
 

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
    TDirectory *ptbinDir = mjbDir->mkdir("PtBin","PtBin");
    ptbinDir->cd();
    for(int j=0; j<myPtBinning.getSize(); j++) {
        vMJB_RefObjPtBin[j]->Write();
    }

    TDirectory *logptprimebinDir = mjbDir->mkdir("LogPtPrimeBin","LogPtPrimeBin");
    logptprimebinDir->cd();
    for(int j=0; j<myLogPtPrimeBinning.getSize(); j++) {
        vMJB_LogPtPrimeBin[j]->Write();
        vInvMJB_LogPtPrimeBin[j]->Write();
    }

    TDirectory *etabinDir = mjbDir->mkdir("EtaBin","EtaBin");
    etabinDir->cd();
    for(int j=0; j<myEtaBinning.getSize(); j++) {
        vMJB_RefObjEtaBin[j]->Write();
    }

    TDirectory *npvbinDir = mjbDir->mkdir("npvBin","npvBin");
    npvbinDir->cd();	
    for(int j=0; j<myNpvBinning.getSize(); j++) {
        vMJB_Npv[j]->Write();
    }

    TDirectory *mpfDir = out->mkdir("MPF","MPF");
    mpfDir->cd();
    TDirectory *ptbinmpfDir = mpfDir->mkdir("PtBin","PtBin");
    ptbinmpfDir->cd();
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
    TDirectory *leadingjet_hltptbin_Dir = leadingJetDir->mkdir("HLTPtBin","HLTPtBin");
    leadingjet_hltptbin_Dir->cd();
    for (int i = 0; i < numberHLTBins; i++) {
        vLeadingJetPt_HLTRefObjPtBin[i]->Write();
        vLeadingJetPt_perHLTTrigger[i]->Write();
    }

    TDirectory *nvtxDir = out->mkdir("Nvtx","Nvtx");
    nvtxDir->cd();
    TDirectory *nvtx_hltptbin_Dir = nvtxDir->mkdir("HLTPtBin","HLTPtBin");
    nvtx_hltptbin_Dir->cd();
    for (int i = 0; i < numberHLTBins; i++) {
        vNvtx_HLTRefObjPtBin[i]->Write();
    }


    if(isMC) {
        TDirectory *mjb_genDir = out->mkdir("MJB_gen","MJB_gen");
        mjb_genDir->cd();
        TDirectory *ptbin_genDir = mjb_genDir->mkdir("PtBin","PtBin");
        ptbin_genDir->cd();
        for(int j=0; j<myPtBinning.getSize(); j++) {
            vMJB_gen_RefObjPtBin[j]->Write();
        }

        TDirectory *mpf_genDir = out->mkdir("MPF_gen","MPF_gen");
        mpf_genDir->cd();
        TDirectory *ptbinmpf_genDir = mpf_genDir->mkdir("PtBin","PtBin");
        ptbinmpf_genDir->cd();
        for(int j=0; j<myPtBinning.getSize(); j++) {
            vMPF_gen_RefObjPtBin[j]->Write();
        }

        TDirectory *trueDir = out->mkdir("Rtrue","Rtrue");
        trueDir->cd();
        for(int j=0; j<myPtBinning.getSize(); j++) {
            vRtrue_leadingJet_RefObjPtBin[j]->Write();
            vRrecoil_RefObjPtBin[j]->Write();
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
        for (int i = 0; i < numberHLTBins; i++) {
            vHLTRefObjPt_HLTRefObjPtBin[i]->Write();
        }
    }	

    TDirectory *recoilJetsDir = out->mkdir("recoilJets","recoilJets");
    recoilJetsDir->cd();	
    for(int j=0; j<myPtBinning.getSize(); j++) {
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
    TDirectory *recoil_hltptbin_Dir = recoilDir->mkdir("HLTPtBin","HLTPtBin");
    recoil_hltptbin_Dir->cd();
    for (int i = 0; i < numberHLTBins; i++) {
        vRecoilPt_HLTRefObjPtBin[i]->Write();
    }


    TDirectory *variablesDir = out->mkdir("variables","variables");
    variablesDir->cd();

    TDirectory *beforeSelDir = variablesDir->mkdir("beforeSel","beforeSel");
    beforeSelDir->cd();
    h2LeadingJet_Recoil_pt_beforeSel->Write();
    hWeight->Write();
    hHT_beforeSel->Write();
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
    hNjets_ptSup30_etaInf5_beforeSel->Write();
    hNjets_ptSup10_beforeSel->Write();

    TDirectory *afterSelDir = variablesDir->mkdir("afterSel","afterSel");
    afterSelDir->cd();
    hNjetsRecoil->Write();
    hNjetsTotal->Write();
    hNjetsGood->Write();
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
    hTrueNotPuTightJetPt->Write();
    hOtherJetPt->Write();
    hHT_afterSel->Write();
    hFracJetsPt->Write();
    hJetsPt_afterSel->Write();
    hJetsEta_afterSel->Write();
    hLeadingJetEta_afterSel->Write();
    hRecoilEta_afterSel->Write();
    hJetsPhi_afterSel->Write();
    hDeltaPhi_METRecoil_afterSel->Write();
    hDeltaPhi_METJet1_afterSel->Write();


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
        vMJB_RefObjPtBin[j]->Delete();
        vNjetsRecoil_RecoilPt[j]->Delete();
        vNjetsRecoil_068E_RecoilPt[j]->Delete();
        vNjetsRecoil_095E_RecoilPt[j]->Delete();
        vMeanLogPt_RecoilPt[j]->Delete();
        vExp_sum_Fi_log_fi_RecoilPt[j]->Delete();
    }

    for(int j=0; j<myEtaBinning.getSize(); j++) {
        vMJB_RefObjEtaBin[j]->Delete();
    }

    for(int j=0; j<myNpvBinning.getSize(); j++) {
        vMJB_Npv[j]->Delete();
    }

    for(int j=0; j<myPtBinning.getSize(); j++) {
        vMPF_RefObjPtBin[j]->Delete();
        vMPF_corr_RefObjPtBin[j]->Delete();
    }	

    if(isMC) {
        for(int j=0; j<myPtBinning.getSize(); j++) {
            vRtrue_leadingJet_RefObjPtBin[j]->Delete();
            vRrecoil_RefObjPtBin[j]->Delete();
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
	
	//return 0;
}









