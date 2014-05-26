#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <TVector3.h>
#include <TStyle.h>
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
#include <TPaveText.h>
#include "TROOT.h"

#include "../common/common.h"

#include "../common/ptBinning.h"
#include "../common/HLTPtBinning.h"
#include "../common/npvBinning.h"
#include "../common/etaBinning.h"


using namespace std;

void applyStyle() {

  TStyle* style_ = new TStyle("drawBaseStyle", "");
  style_->SetCanvasColor(0);
  style_->SetPadColor(0);
  style_->SetFrameFillColor(0);
  style_->SetStatColor(0);
  style_->SetOptStat(0);
  style_->SetTitleFillColor(0);
  style_->SetCanvasBorderMode(0);
  style_->SetPadBorderMode(0);
  style_->SetFrameBorderMode(0);
  style_->SetPadBottomMargin(0.12);
  style_->SetPadLeftMargin(0.12);

  // For the canvas:
  style_->SetCanvasBorderMode(0);
  style_->SetCanvasColor(kWhite);
  style_->SetCanvasDefH(600); //Height of canvas
  style_->SetCanvasDefW(600); //Width of canvas
  style_->SetCanvasDefX(0); //POsition on screen
  style_->SetCanvasDefY(0);

  // For the Pad:
  style_->SetPadBorderMode(0);
  // style_->SetPadBorderSize(Width_t size = 1);
  style_->SetPadColor(kWhite);
  style_->SetPadGridX(false);
  style_->SetPadGridY(false);
  style_->SetGridColor(0);
  style_->SetGridStyle(3);
  style_->SetGridWidth(1);

  // For the frame:
  style_->SetFrameBorderMode(0);
  style_->SetFrameBorderSize(1);
  style_->SetFrameFillColor(0);
  style_->SetFrameFillStyle(0);
  style_->SetFrameLineColor(1);
  style_->SetFrameLineStyle(1);
  style_->SetFrameLineWidth(1);

  // Margins:
  style_->SetPadTopMargin(0.05);
  style_->SetPadBottomMargin(0.15);//0.13);
  style_->SetPadLeftMargin(0.15);//0.16);
  style_->SetPadRightMargin(0.05);//0.02);

  // For the Global title:

  style_->SetOptTitle(0);
  style_->SetTitleFont(42);
  style_->SetTitleColor(1);
  style_->SetTitleTextColor(1);
  style_->SetTitleFillColor(10);
  style_->SetTitleFontSize(0.05);

  // For the axis titles:

  style_->SetTitleColor(1, "XYZ");
  style_->SetTitleFont(42, "XYZ");
  style_->SetTitleSize(0.05, "XYZ");
  // style_->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // style_->SetTitleYSize(Float_t size = 0.02);
  style_->SetTitleXOffset(1.15);//0.9);
  style_->SetTitleYOffset(1.4); // => 1.15 if exponents
  // style_->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  style_->SetLabelColor(1, "XYZ");
  style_->SetLabelFont(42, "XYZ");
  style_->SetLabelOffset(0.007, "XYZ");
  style_->SetLabelSize(0.045, "XYZ");

  // For the axis:

  style_->SetAxisColor(1, "XYZ");
  style_->SetStripDecimals(kTRUE);
  style_->SetTickLength(0.03, "XYZ");
  style_->SetNdivisions(510, "XYZ");
  style_->SetPadTickX(1); // To get tick marks on the opposite side of the frame
  style_->SetPadTickY(1);

  // Legend
  style_->SetLegendBorderSize(1);
  style_->SetLegendFillColor(kWhite);
  style_->SetLegendFont(42);

  style_->cd();

}

  TPaveText* get_labelCMS(int legendQuadrant) {

    if (legendQuadrant != 0 && legendQuadrant != 1 && legendQuadrant != 2 && legendQuadrant != 3) {
      std::cout << "WARNING! Legend quadrant '" << legendQuadrant << "' not yet implemented for CMS label. Using 2." << std::endl;
      legendQuadrant = 2;
    }

    float x1, y1, x2, y2;
    if (legendQuadrant == 1) {
      x1 = 0.12;
      y1 = 0.95;
      x2 = 0.95;
      y2 = 1.;
    } else if (legendQuadrant == 2) {
      x1 = 0.25;
      y1 = 0.86;
      x2 = 0.42;
      y2 = 0.92;
    } else if (legendQuadrant == 3) {
      x1 = 0.25;
      y1 = 0.2;
      x2 = 0.42;
      y2 = 0.24;
    } else if (legendQuadrant == 0) {
      x1 = 0.17;
      y1 = 0.963;
      x2 = 0.65;
      y2 = 0.985;
    }


    TPaveText* cmslabel = new TPaveText(x1, y1, x2, y2, "brNDC");
    cmslabel->SetFillColor(kWhite);
    cmslabel->SetTextSize(0.038);
    cmslabel->SetTextFont(62);
    cmslabel->AddText(Form("CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.7 fb^{-1}"));
    return cmslabel;

  }


void drawComparisonResponse(const string& canvasName, TMultiGraph *mgResponse, TGraphErrors *gResponseMCnominal, TGraphErrors *gResponseMCup, TGraph *gResponseMCdown, const string& mcSampleNominal, const string& mcSampleUp, const string& mcSampleDown, const string& path) {
	TCanvas *cCanvas = new TCanvas(canvasName.c_str(),canvasName.c_str(), 600, 800);
	cCanvas->cd();

  gStyle->SetOptStat(0);
	mgResponse->SetMaximum(1.05);
	mgResponse->SetMinimum(0.9);
	mgResponse->Draw("AP");
	//mgResponse->GetXaxis()->SetLabelSize(0);
	mgResponse->GetYaxis()->SetTitleOffset(1.3);
	cCanvas->SetLogx(0);
	
  TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  legend->SetFillColor(kWhite);
  legend->SetFillStyle(0);
	legend->SetTextFont(42);
  legend->SetTextSize(0.038);
	legend->AddEntry(gResponseMCnominal,mcSampleNominal.c_str(),"p");
	legend->AddEntry(gResponseMCup,mcSampleUp.c_str(),"p");
	legend->AddEntry(gResponseMCdown,mcSampleDown.c_str(),"p");
	legend->Draw("same");
	gPad->RedrawAxis();
	cCanvas->Update();
	cCanvas->SaveAs(path.c_str());
}


int main() 
{
	TString inname_data = "../analysis/output_rootfile/30Apr14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting.root";
	TString inname_mc_HT = "../analysis/output_rootfile/30Apr14/MC/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root";
	TString inname_mc_HT_JECup = "../analysis/output_rootfile/30Apr14/MC/Systematics/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_JECup.root";
	TString inname_mc_HT_JECdown = "../analysis/output_rootfile/30Apr14/MC/Systematics/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_JECdown.root";
	TString inname_mc_HT_JERup = "../analysis/output_rootfile/30Apr14/MC/Systematics/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_JERup.root";
	TString inname_mc_HT_JERdown = "../analysis/output_rootfile/30Apr14/MC/Systematics/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_JERdown.root";
	TString inname_mc_HT_PUup = "../analysis/output_rootfile/30Apr14/MC/Systematics/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_PUup.root";
	TString inname_mc_HT_PUdown = "../analysis/output_rootfile/30Apr14/MC/Systematics/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_PUdown.root";
	TString inname_mc_Pt_pythia = "../analysis/output_rootfile/30Apr14/MC/MULTIJET_MC_QCD-Pt-pythia-120To1400_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root";
	TString inname_mc_Pt_herwig = "../analysis/output_rootfile/30Apr14/MC/MULTIJET_MC_QCD-Pt-herwig-120ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root";
	bool isLumiNorm = false;
	bool isShapeNorm = false;
	bool inLinScale = false;
	bool rmPU = false;
	
	string extension = ".pdf";	
	
	//gStyle->SetOptFit(111111);
	
	ptBinning myPtBinning;
  HLTPtBinning myHLTPtBinning;
	npvBinning myNpvBinning;
	etaBinning myEtaBinning;
	
	Double_t xlow = getHistoXlow();
	Double_t xup = getHistoXup();
	//Double_t    binrange = 0.1;
	//Int_t    nbinsx = (xup - xlow)/binrange;
	Int_t    nbinsx = getHistoNbinsx();
  bool useRecoilPtBin = true;  
	
	int numberPtBins = myPtBinning.getSize();
  int numberHLTPtBins = myHLTPtBinning.getSize();
	int numberNpvBins = myNpvBinning.getSize();
	int numberEtaBins = myEtaBinning.getSize();
	
	TF1 *func = new TF1("func","[0]");
	func->SetParameter(0,1.);
	
  
	TFile *f_data=TFile::Open(inname_data);
	TFile *f_mc_HT=TFile::Open(inname_mc_HT);
	TFile *f_mc_HT_JECup=TFile::Open(inname_mc_HT_JECup);
	TFile *f_mc_HT_JECdown=TFile::Open(inname_mc_HT_JECdown);
	TFile *f_mc_HT_JERup=TFile::Open(inname_mc_HT_JERup);
	TFile *f_mc_HT_JERdown=TFile::Open(inname_mc_HT_JERdown);
	TFile *f_mc_HT_PUup=TFile::Open(inname_mc_HT_PUup);
	TFile *f_mc_HT_PUdown=TFile::Open(inname_mc_HT_PUdown);
	TFile *f_mc_Pt_pythia=TFile::Open(inname_mc_Pt_pythia);
	TFile *f_mc_Pt_herwig=TFile::Open(inname_mc_Pt_herwig);
	
	string myHistoName;
	
	applyStyle();

	
//************************************************************************************************************
//
//                                      MJB as a function of ptrecoil
//
//************************************************************************************************************

	TGraphErrors* gMJB_RefObjPt_data=(TGraphErrors*)f_data->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_HT=(TGraphErrors*)f_mc_HT->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_HT_JECup=(TGraphErrors*)f_mc_HT_JECup->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_HT_JECdown=(TGraphErrors*)f_mc_HT_JECdown->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_HT_JERup=(TGraphErrors*)f_mc_HT_JERup->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_HT_JERdown=(TGraphErrors*)f_mc_HT_JERdown->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_HT_PUup=(TGraphErrors*)f_mc_HT_PUup->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_HT_PUdown=(TGraphErrors*)f_mc_HT_PUdown->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_Pt_pythia=(TGraphErrors*)f_mc_Pt_pythia->Get("MJB/PtBin/gMJB_RefObjPt");
	TGraphErrors* gMJB_RefObjPt_mc_Pt_herwig=(TGraphErrors*)f_mc_Pt_herwig->Get("MJB/PtBin/gMJB_RefObjPt");
	
	TGraph_style (gMJB_RefObjPt_data);
	TGraph_style (gMJB_RefObjPt_mc_HT);
	TGraph_style (gMJB_RefObjPt_mc_HT_JECup);
	TGraph_style (gMJB_RefObjPt_mc_HT_JECdown);
	TGraph_style (gMJB_RefObjPt_mc_HT_JERup);
	TGraph_style (gMJB_RefObjPt_mc_HT_JERdown);
	TGraph_style (gMJB_RefObjPt_mc_HT_PUup);
	TGraph_style (gMJB_RefObjPt_mc_HT_PUdown);
	TGraph_style (gMJB_RefObjPt_mc_Pt_pythia);
	TGraph_style (gMJB_RefObjPt_mc_Pt_herwig);

	gMJB_RefObjPt_data->SetMarkerColor(1);
	gMJB_RefObjPt_mc_HT->SetMarkerColor(2);
	gMJB_RefObjPt_mc_HT_JECup->SetMarkerColor(4);
	gMJB_RefObjPt_mc_HT_JECdown->SetMarkerColor(7);
  gMJB_RefObjPt_mc_HT_JERup->SetMarkerColor(6);
	gMJB_RefObjPt_mc_HT_JERdown->SetMarkerColor(46);
	gMJB_RefObjPt_mc_HT_PUup->SetMarkerColor(38);
	gMJB_RefObjPt_mc_HT_PUdown->SetMarkerColor(9);
	gMJB_RefObjPt_mc_Pt_pythia->SetMarkerColor(42);
	gMJB_RefObjPt_mc_Pt_herwig->SetMarkerColor(28);
	
	TMultiGraph *mgMJB_RefObjPt_JEC = new TMultiGraph();
	mgMJB_RefObjPt_JEC->Add(gMJB_RefObjPt_mc_HT,"pe");
	mgMJB_RefObjPt_JEC->Add(gMJB_RefObjPt_mc_HT_JECup,"pe");
	mgMJB_RefObjPt_JEC->Add(gMJB_RefObjPt_mc_HT_JECdown,"pe");

  TMultiGraph *mgMJB_RefObjPt_JER = new TMultiGraph();
	mgMJB_RefObjPt_JER->Add(gMJB_RefObjPt_mc_HT,"pe");
	mgMJB_RefObjPt_JER->Add(gMJB_RefObjPt_mc_HT_JERup,"pe");
	mgMJB_RefObjPt_JER->Add(gMJB_RefObjPt_mc_HT_JERdown,"pe");

  TMultiGraph *mgMJB_RefObjPt_PU = new TMultiGraph();
	mgMJB_RefObjPt_PU->Add(gMJB_RefObjPt_mc_HT,"pe");
	mgMJB_RefObjPt_PU->Add(gMJB_RefObjPt_mc_HT_PUup,"pe");
	mgMJB_RefObjPt_PU->Add(gMJB_RefObjPt_mc_HT_PUdown,"pe");

	TMultiGraph *mgMJB_RefObjPt_generator = new TMultiGraph();
	mgMJB_RefObjPt_generator->Add(gMJB_RefObjPt_mc_HT,"pe");
	mgMJB_RefObjPt_generator->Add(gMJB_RefObjPt_mc_Pt_pythia,"pe");
	mgMJB_RefObjPt_generator->Add(gMJB_RefObjPt_mc_Pt_herwig,"pe");
	
  if(useRecoilPtBin) {
	  mgMJB_RefObjPt_JEC->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
	  mgMJB_RefObjPt_JER->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
	  mgMJB_RefObjPt_generator->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
	  mgMJB_RefObjPt_PU->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
  }
  else {
    mgMJB_RefObjPt_JEC->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
	  mgMJB_RefObjPt_JER->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
	  mgMJB_RefObjPt_generator->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
	  mgMJB_RefObjPt_PU->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
  }
	
	drawComparisonResponse("r1", mgMJB_RefObjPt_JEC, gMJB_RefObjPt_mc_HT, gMJB_RefObjPt_mc_HT_JECup, gMJB_RefObjPt_mc_HT_JECdown, "nominal", "JECup", "JECdown", "images/MJB_RefObjPt_JEC.pdf");
	drawComparisonResponse("r2", mgMJB_RefObjPt_JER, gMJB_RefObjPt_mc_HT, gMJB_RefObjPt_mc_HT_JERup, gMJB_RefObjPt_mc_HT_JERdown, "nominal", "JERup", "JERdown", "images/MJB_RefObjPt_JER.pdf");
	drawComparisonResponse("r3", mgMJB_RefObjPt_PU, gMJB_RefObjPt_mc_HT, gMJB_RefObjPt_mc_HT_PUup, gMJB_RefObjPt_mc_HT_PUdown, "nominal", "PUup", "PUdown", "images/MJB_RefObjPt_PU.pdf");
	drawComparisonResponse("r4", mgMJB_RefObjPt_generator, gMJB_RefObjPt_mc_HT, gMJB_RefObjPt_mc_Pt_herwig, gMJB_RefObjPt_mc_Pt_pythia, "HT", "Herwig", "Pythia", "images/MJB_RefObjPt_generator.pdf");

//************************************************************************************************************
//
//                                      MJB as a function of ptrecoil without 1 ptbin
//
//************************************************************************************************************

  Double_t aMJB_RefObjPt_data_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_data_MeanError[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_MeanError[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_JECup_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_JECup_MeanError[numberPtBins-2];	
  Double_t aMJB_RefObjPt_mc_HT_JECdown_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_JECdown_MeanError[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_JERup_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_JERup_MeanError[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_JERdown_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_JERdown_MeanError[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_PUup_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_PUup_MeanError[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_PUdown_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_HT_PUdown_MeanError[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_Pt_pythia_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_Pt_pythia_MeanError[numberPtBins-2];
 	Double_t aMJB_RefObjPt_mc_Pt_herwig_Mean[numberPtBins-2];
	Double_t aMJB_RefObjPt_mc_Pt_herwig_MeanError[numberPtBins-2];
	Double_t aRefObjPtBins_resize_Mean[numberPtBins-2];
	Double_t aRefObjPtBins_resize_MeanError[numberPtBins-2];



	
	for(int i=1; i<numberPtBins-1; i++) {
    gMJB_RefObjPt_data->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_data_Mean[i-1]);
    aMJB_RefObjPt_data_MeanError[i-1] = gMJB_RefObjPt_data->GetErrorY(i);
    gMJB_RefObjPt_mc_HT->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_HT_Mean[i-1]);
    aMJB_RefObjPt_mc_HT_MeanError[i-1] = gMJB_RefObjPt_mc_HT->GetErrorY(i);
    gMJB_RefObjPt_mc_Pt_pythia->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_Pt_pythia_Mean[i-1]);
    aMJB_RefObjPt_mc_Pt_pythia_MeanError[i-1] = gMJB_RefObjPt_mc_Pt_pythia->GetErrorY(i);
    gMJB_RefObjPt_mc_Pt_herwig->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_Pt_herwig_Mean[i-1]);
    aMJB_RefObjPt_mc_Pt_herwig_MeanError[i-1] = gMJB_RefObjPt_mc_Pt_herwig->GetErrorY(i);
    gMJB_RefObjPt_mc_HT_JECup->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_HT_JECup_Mean[i-1]);
    aMJB_RefObjPt_mc_HT_JECup_MeanError[i-1] = gMJB_RefObjPt_mc_HT_JECup->GetErrorY(i);
    gMJB_RefObjPt_mc_HT_PUdown->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_HT_PUdown_Mean[i-1]);
    aMJB_RefObjPt_mc_HT_PUdown_MeanError[i-1] = gMJB_RefObjPt_mc_HT_PUdown->GetErrorY(i);
    gMJB_RefObjPt_mc_HT_PUup->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_HT_PUup_Mean[i-1]);
    aMJB_RefObjPt_mc_HT_PUup_MeanError[i-1] = gMJB_RefObjPt_mc_HT_PUup->GetErrorY(i);
    gMJB_RefObjPt_mc_HT_JERdown->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_HT_JERdown_Mean[i-1]);
    aMJB_RefObjPt_mc_HT_JERdown_MeanError[i-1] = gMJB_RefObjPt_mc_HT_JERdown->GetErrorY(i);
    gMJB_RefObjPt_mc_HT_JERup->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_HT_JERup_Mean[i-1]);
    aMJB_RefObjPt_mc_HT_JERup_MeanError[i-1] = gMJB_RefObjPt_mc_HT_JERup->GetErrorY(i);
    gMJB_RefObjPt_mc_HT_JECdown->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMJB_RefObjPt_mc_HT_JECdown_Mean[i-1]);
    aMJB_RefObjPt_mc_HT_JECdown_MeanError[i-1] = gMJB_RefObjPt_mc_HT_JECdown->GetErrorY(i);
    aRefObjPtBins_resize_MeanError[i-1] = gMJB_RefObjPt_data->GetErrorX(i);
	}

	TGraphErrors* gMJB_RefObjPt_resize_data=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_data_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_data_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_HT=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_HT_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_HT_MeanError);	
  TGraphErrors* gMJB_RefObjPt_resize_mc_HT_JECup=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_HT_JECup_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_HT_JECup_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_HT_JECdown=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_HT_JECdown_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_HT_JECdown_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_HT_JERup=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_HT_JERup_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_HT_JERup_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_HT_JERdown=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_HT_JERdown_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_HT_JERdown_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_HT_PUup=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_HT_PUup_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_HT_PUup_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_HT_PUdown=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_HT_PUdown_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_HT_PUdown_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_Pt_pythia=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_Pt_pythia_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_Pt_pythia_MeanError);
	TGraphErrors* gMJB_RefObjPt_resize_mc_Pt_herwig=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMJB_RefObjPt_mc_Pt_herwig_Mean, aRefObjPtBins_resize_MeanError, aMJB_RefObjPt_mc_Pt_herwig_MeanError);

	TGraph_style (gMJB_RefObjPt_resize_data);
	TGraph_style (gMJB_RefObjPt_resize_mc_HT);
	TGraph_style (gMJB_RefObjPt_resize_mc_HT_JECup);
	TGraph_style (gMJB_RefObjPt_resize_mc_HT_JECdown);
	TGraph_style (gMJB_RefObjPt_resize_mc_HT_JERup);
	TGraph_style (gMJB_RefObjPt_resize_mc_HT_JERdown);
	TGraph_style (gMJB_RefObjPt_resize_mc_HT_PUup);
	TGraph_style (gMJB_RefObjPt_resize_mc_HT_PUdown);
	TGraph_style (gMJB_RefObjPt_resize_mc_Pt_pythia);
	TGraph_style (gMJB_RefObjPt_resize_mc_Pt_herwig);

	gMJB_RefObjPt_resize_data->SetMarkerColor(1);
	gMJB_RefObjPt_resize_mc_HT->SetMarkerColor(2);
	gMJB_RefObjPt_resize_mc_HT_JECup->SetMarkerColor(4);
	gMJB_RefObjPt_resize_mc_HT_JECdown->SetMarkerColor(7);
  gMJB_RefObjPt_resize_mc_HT_JERup->SetMarkerColor(6);
	gMJB_RefObjPt_resize_mc_HT_JERdown->SetMarkerColor(46);
	gMJB_RefObjPt_resize_mc_HT_PUup->SetMarkerColor(38);
	gMJB_RefObjPt_resize_mc_HT_PUdown->SetMarkerColor(9);
	gMJB_RefObjPt_resize_mc_Pt_pythia->SetMarkerColor(42);
	gMJB_RefObjPt_resize_mc_Pt_herwig->SetMarkerColor(28);

	gMJB_RefObjPt_resize_data->SetLineColor(1);
	gMJB_RefObjPt_resize_mc_HT->SetLineColor(2);
	gMJB_RefObjPt_resize_mc_HT_JECup->SetLineColor(4);
	gMJB_RefObjPt_resize_mc_HT_JECdown->SetLineColor(7);
  gMJB_RefObjPt_resize_mc_HT_JERup->SetLineColor(6);
	gMJB_RefObjPt_resize_mc_HT_JERdown->SetLineColor(46);
	gMJB_RefObjPt_resize_mc_HT_PUup->SetLineColor(38);
	gMJB_RefObjPt_resize_mc_HT_PUdown->SetLineColor(9);
	gMJB_RefObjPt_resize_mc_Pt_pythia->SetLineColor(42);
	gMJB_RefObjPt_resize_mc_Pt_herwig->SetLineColor(28);
	
	TMultiGraph *mgMJB_RefObjPt_resize_JEC = new TMultiGraph();
	mgMJB_RefObjPt_resize_JEC->Add(gMJB_RefObjPt_resize_mc_HT,"pe");
	mgMJB_RefObjPt_resize_JEC->Add(gMJB_RefObjPt_resize_mc_HT_JECup,"pe");
	mgMJB_RefObjPt_resize_JEC->Add(gMJB_RefObjPt_resize_mc_HT_JECdown,"pe");

  TMultiGraph *mgMJB_RefObjPt_resize_JER = new TMultiGraph();
	mgMJB_RefObjPt_resize_JER->Add(gMJB_RefObjPt_resize_mc_HT,"pe");
	mgMJB_RefObjPt_resize_JER->Add(gMJB_RefObjPt_resize_mc_HT_JERup,"pe");
	mgMJB_RefObjPt_resize_JER->Add(gMJB_RefObjPt_resize_mc_HT_JERdown,"pe");

  TMultiGraph *mgMJB_RefObjPt_resize_PU = new TMultiGraph();
	mgMJB_RefObjPt_resize_PU->Add(gMJB_RefObjPt_resize_mc_HT,"pe");
	mgMJB_RefObjPt_resize_PU->Add(gMJB_RefObjPt_resize_mc_HT_PUup,"pe");
	mgMJB_RefObjPt_resize_PU->Add(gMJB_RefObjPt_resize_mc_HT_PUdown,"pe");

	TMultiGraph *mgMJB_RefObjPt_resize_generator = new TMultiGraph();
	mgMJB_RefObjPt_resize_generator->Add(gMJB_RefObjPt_resize_mc_HT,"pe");
	mgMJB_RefObjPt_resize_generator->Add(gMJB_RefObjPt_resize_mc_Pt_pythia,"pe");
	mgMJB_RefObjPt_resize_generator->Add(gMJB_RefObjPt_resize_mc_Pt_herwig,"pe");

  if(useRecoilPtBin) {
	  mgMJB_RefObjPt_resize_JEC->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
	  mgMJB_RefObjPt_resize_JER->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
	  mgMJB_RefObjPt_resize_generator->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
	  mgMJB_RefObjPt_resize_PU->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
  }
  else {
    mgMJB_RefObjPt_resize_JEC->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
	  mgMJB_RefObjPt_resize_JER->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
	  mgMJB_RefObjPt_resize_generator->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
	  mgMJB_RefObjPt_resize_PU->SetTitle("MJB as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MJB");
  }
	
	
	drawComparisonResponse("r1b", mgMJB_RefObjPt_resize_JEC, gMJB_RefObjPt_resize_mc_HT, gMJB_RefObjPt_resize_mc_HT_JECup, gMJB_RefObjPt_resize_mc_HT_JECdown, "nominal", "JECup", "JECdown", "images/MJB_RefObjPt_resize_JEC.pdf");
	drawComparisonResponse("r2b", mgMJB_RefObjPt_resize_JER, gMJB_RefObjPt_resize_mc_HT, gMJB_RefObjPt_resize_mc_HT_JERup, gMJB_RefObjPt_resize_mc_HT_JERdown, "nominal", "JERup", "JERdown", "images/MJB_RefObjPt_resize_JER.pdf");
	drawComparisonResponse("r3b", mgMJB_RefObjPt_resize_PU, gMJB_RefObjPt_resize_mc_HT, gMJB_RefObjPt_resize_mc_HT_PUup, gMJB_RefObjPt_resize_mc_HT_PUdown, "nominal", "PUup", "PUdown", "images/MJB_RefObjPt_resize_PU.pdf");
	drawComparisonResponse("r4b", mgMJB_RefObjPt_resize_generator, gMJB_RefObjPt_resize_mc_HT, gMJB_RefObjPt_resize_mc_Pt_herwig, gMJB_RefObjPt_resize_mc_Pt_pythia, "HT", "Herwig", "Pythia", "images/MJB_RefObjPt_resize_generator.pdf");


//******************************************************************************************************************************************


  Double_t aY[numberPtBins];
  Double_t aMJBDeltaJEC[numberPtBins];
  Double_t aMJBDeltaJER[numberPtBins];
  Double_t aMJBDeltaPU[numberPtBins];
  Double_t aMJBDeltaSystTot[numberPtBins];
	Double_t aRefObjPtBins_Mean[numberPtBins];
	Double_t aRefObjPtBins_MeanError[numberPtBins];

  Double_t valueUp, valueDown;
  for (int i = 0; i < numberPtBins; i++) {
    aY[i] = 0.;
    aRefObjPtBins_MeanError[i] = 0.;

    gMJB_RefObjPt_mc_HT_JECup->GetPoint(i,aRefObjPtBins_Mean[i],valueUp);
    gMJB_RefObjPt_mc_HT_JECdown->GetPoint(i,aRefObjPtBins_Mean[i],valueDown);
    aMJBDeltaJEC[i] = fabs(valueUp - valueDown)/2.;

    std::cout << "Bin " << i << "..." << std::endl;
    std::cout << "JEC: " << std::endl;
    std::cout << "valueUp: " << valueUp << std::endl;
    std::cout << "valueDown: " << valueDown << std::endl << std::endl;

    gMJB_RefObjPt_mc_HT_JERup->GetPoint(i,aRefObjPtBins_Mean[i],valueUp);
    gMJB_RefObjPt_mc_HT_JERdown->GetPoint(i,aRefObjPtBins_Mean[i],valueDown);
    aMJBDeltaJER[i] = fabs(valueUp - valueDown)/2.;

    std::cout << "JER: " << std::endl;
    std::cout << "valueUp: " << valueUp << std::endl;
    std::cout << "valueDown: " << valueDown << std::endl << std::endl;

    gMJB_RefObjPt_mc_HT_PUup->GetPoint(i,aRefObjPtBins_Mean[i],valueUp);
    gMJB_RefObjPt_mc_HT_PUdown->GetPoint(i,aRefObjPtBins_Mean[i],valueDown);
    aMJBDeltaPU[i] = fabs(valueUp - valueDown)/2.;

    std::cout << "PU: " << std::endl;
    std::cout << "valueUp: " << valueUp << std::endl;
    std::cout << "valueDown: " << valueDown << std::endl << std::endl;

    aMJBDeltaSystTot[i] = sqrt(aMJBDeltaJEC[i]*aMJBDeltaJEC[i] + aMJBDeltaJER[i]*aMJBDeltaJER[i] + aMJBDeltaPU[i]*aMJBDeltaPU[i]);

    std::cout << "Checking systematics value for bin " << i << std::endl;
    std::cout << "aMJBDeltaJEC[" << i << "]: " << aMJBDeltaJEC[i] << std::endl;
    std::cout << "aMJBDeltaJER[" << i << "]: " << aMJBDeltaJER[i] << std::endl;
    std::cout << "aMJBDeltaPU[" << i << "]: " << aMJBDeltaPU[i] << std::endl;
    std::cout << "aMJBDeltaSystTot[" << i << "]: " << aMJBDeltaSystTot[i] << std::endl << std::endl;

  }

  TGraphErrors *gMJB_JEC = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMJBDeltaJEC);
  TGraphErrors *gMJB_JER = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMJBDeltaJER);
  TGraphErrors *gMJB_PU = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMJBDeltaPU);
  TGraphErrors *gMJB_SystTot = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMJBDeltaSystTot);


//************************************************************************************************************
//
//                                      MPF as a function of ptrecoil
//
//************************************************************************************************************

	TGraphErrors* gMPF_RefObjPt_data=(TGraphErrors*)f_data->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_HT=(TGraphErrors*)f_mc_HT->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_HT_JECup=(TGraphErrors*)f_mc_HT_JECup->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_HT_JECdown=(TGraphErrors*)f_mc_HT_JECdown->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_HT_JERup=(TGraphErrors*)f_mc_HT_JERup->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_HT_JERdown=(TGraphErrors*)f_mc_HT_JERdown->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_HT_PUup=(TGraphErrors*)f_mc_HT_PUup->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_HT_PUdown=(TGraphErrors*)f_mc_HT_PUdown->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_Pt_pythia=(TGraphErrors*)f_mc_Pt_pythia->Get("MPF/PtBin/gMPF_RefObjPt");
	TGraphErrors* gMPF_RefObjPt_mc_Pt_herwig=(TGraphErrors*)f_mc_Pt_herwig->Get("MPF/PtBin/gMPF_RefObjPt");
	
	TGraph_style (gMPF_RefObjPt_data);
	TGraph_style (gMPF_RefObjPt_mc_HT);
	TGraph_style (gMPF_RefObjPt_mc_HT_JECup);
	TGraph_style (gMPF_RefObjPt_mc_HT_JECdown);
	TGraph_style (gMPF_RefObjPt_mc_HT_JERup);
	TGraph_style (gMPF_RefObjPt_mc_HT_JERdown);
	TGraph_style (gMPF_RefObjPt_mc_HT_PUup);
	TGraph_style (gMPF_RefObjPt_mc_HT_PUdown);
	TGraph_style (gMPF_RefObjPt_mc_Pt_pythia);
	TGraph_style (gMPF_RefObjPt_mc_Pt_herwig);

	gMPF_RefObjPt_data->SetMarkerColor(1);
	gMPF_RefObjPt_mc_HT->SetMarkerColor(2);
	gMPF_RefObjPt_mc_HT_JECup->SetMarkerColor(4);
	gMPF_RefObjPt_mc_HT_JECdown->SetMarkerColor(7);
  gMPF_RefObjPt_mc_HT_JERup->SetMarkerColor(6);
	gMPF_RefObjPt_mc_HT_JERdown->SetMarkerColor(46);
	gMPF_RefObjPt_mc_HT_PUup->SetMarkerColor(38);
	gMPF_RefObjPt_mc_HT_PUdown->SetMarkerColor(9);
	gMPF_RefObjPt_mc_Pt_pythia->SetMarkerColor(42);
	gMPF_RefObjPt_mc_Pt_herwig->SetMarkerColor(28);

	gMPF_RefObjPt_data->SetLineColor(1);
	gMPF_RefObjPt_mc_HT->SetLineColor(2);
	gMPF_RefObjPt_mc_HT_JECup->SetLineColor(4);
	gMPF_RefObjPt_mc_HT_JECdown->SetLineColor(7);
  gMPF_RefObjPt_mc_HT_JERup->SetLineColor(6);
	gMPF_RefObjPt_mc_HT_JERdown->SetLineColor(46);
	gMPF_RefObjPt_mc_HT_PUup->SetLineColor(38);
	gMPF_RefObjPt_mc_HT_PUdown->SetLineColor(9);
	gMPF_RefObjPt_mc_Pt_pythia->SetLineColor(42);
	gMPF_RefObjPt_mc_Pt_herwig->SetLineColor(28);
	
	TMultiGraph *mgMPF_RefObjPt_JEC = new TMultiGraph();
	mgMPF_RefObjPt_JEC->Add(gMPF_RefObjPt_mc_HT,"pe");
	mgMPF_RefObjPt_JEC->Add(gMPF_RefObjPt_mc_HT_JECup,"pe");
	mgMPF_RefObjPt_JEC->Add(gMPF_RefObjPt_mc_HT_JECdown,"pe");

  TMultiGraph *mgMPF_RefObjPt_JER = new TMultiGraph();
	mgMPF_RefObjPt_JER->Add(gMPF_RefObjPt_mc_HT,"pe");
	mgMPF_RefObjPt_JER->Add(gMPF_RefObjPt_mc_HT_JERup,"pe");
	mgMPF_RefObjPt_JER->Add(gMPF_RefObjPt_mc_HT_JERdown,"pe");

  TMultiGraph *mgMPF_RefObjPt_PU = new TMultiGraph();
	mgMPF_RefObjPt_PU->Add(gMPF_RefObjPt_mc_HT,"pe");
	mgMPF_RefObjPt_PU->Add(gMPF_RefObjPt_mc_HT_PUup,"pe");
	mgMPF_RefObjPt_PU->Add(gMPF_RefObjPt_mc_HT_PUdown,"pe");

	TMultiGraph *mgMPF_RefObjPt_generator = new TMultiGraph();
	mgMPF_RefObjPt_generator->Add(gMPF_RefObjPt_mc_HT,"pe");
	mgMPF_RefObjPt_generator->Add(gMPF_RefObjPt_mc_Pt_pythia,"pe");
	mgMPF_RefObjPt_generator->Add(gMPF_RefObjPt_mc_Pt_herwig,"pe");
	
  if(useRecoilPtBin) {
	  mgMPF_RefObjPt_JEC->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
	  mgMPF_RefObjPt_JER->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
	  mgMPF_RefObjPt_generator->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
	  mgMPF_RefObjPt_PU->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
  }
  else {
    mgMPF_RefObjPt_JEC->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
	  mgMPF_RefObjPt_JER->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
	  mgMPF_RefObjPt_generator->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
	  mgMPF_RefObjPt_PU->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
  }
	
	drawComparisonResponse("r5", mgMPF_RefObjPt_JEC, gMPF_RefObjPt_mc_HT, gMPF_RefObjPt_mc_HT_JECup, gMPF_RefObjPt_mc_HT_JECdown, "nominal", "JECup", "JECdown", "images/MPF_RefObjPt_JEC.pdf");
	drawComparisonResponse("r6", mgMPF_RefObjPt_JER, gMPF_RefObjPt_mc_HT, gMPF_RefObjPt_mc_HT_JERup, gMPF_RefObjPt_mc_HT_JERdown, "nominal", "JERup", "JERdown", "images/MPF_RefObjPt_JER.pdf");
	drawComparisonResponse("r7", mgMPF_RefObjPt_PU, gMPF_RefObjPt_mc_HT, gMPF_RefObjPt_mc_HT_PUup, gMPF_RefObjPt_mc_HT_PUdown, "nominal", "PUup", "PUdown", "images/MPF_RefObjPt_PU.pdf");
	drawComparisonResponse("r8", mgMPF_RefObjPt_generator, gMPF_RefObjPt_mc_HT, gMPF_RefObjPt_mc_Pt_herwig, gMPF_RefObjPt_mc_Pt_pythia, "HT", "Herwig", "Pythia", "images/MPF_RefObjPt_generator.pdf");


//************************************************************************************************************
//
//                                      MPF as a function of ptrecoil without 1 ptbin
//
//************************************************************************************************************

  Double_t aMPF_RefObjPt_data_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_data_MeanError[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_MeanError[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_JECup_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_JECup_MeanError[numberPtBins-2];	
  Double_t aMPF_RefObjPt_mc_HT_JECdown_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_JECdown_MeanError[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_JERup_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_JERup_MeanError[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_JERdown_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_JERdown_MeanError[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_PUup_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_PUup_MeanError[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_PUdown_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_HT_PUdown_MeanError[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_Pt_pythia_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_Pt_pythia_MeanError[numberPtBins-2];
 	Double_t aMPF_RefObjPt_mc_Pt_herwig_Mean[numberPtBins-2];
	Double_t aMPF_RefObjPt_mc_Pt_herwig_MeanError[numberPtBins-2];
	//Double_t aRefObjPtBins_resize_Mean[numberPtBins-2];
	//Double_t aRefObjPtBins_resize_MeanError[numberPtBins-2];



	
	for(int i=1; i<numberPtBins-1; i++) {
    gMPF_RefObjPt_data->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_data_Mean[i-1]);
    aMPF_RefObjPt_data_MeanError[i-1] = gMPF_RefObjPt_data->GetErrorY(i);
    gMPF_RefObjPt_mc_HT->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_HT_Mean[i-1]);
    aMPF_RefObjPt_mc_HT_MeanError[i-1] = gMPF_RefObjPt_mc_HT->GetErrorY(i);
    gMPF_RefObjPt_mc_Pt_pythia->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_Pt_pythia_Mean[i-1]);
    aMPF_RefObjPt_mc_Pt_pythia_MeanError[i-1] = gMPF_RefObjPt_mc_Pt_pythia->GetErrorY(i);
    gMPF_RefObjPt_mc_Pt_herwig->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_Pt_herwig_Mean[i-1]);
    aMPF_RefObjPt_mc_Pt_herwig_MeanError[i-1] = gMPF_RefObjPt_mc_Pt_herwig->GetErrorY(i);
    gMPF_RefObjPt_mc_HT_JECup->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_HT_JECup_Mean[i-1]);
    aMPF_RefObjPt_mc_HT_JECup_MeanError[i-1] = gMPF_RefObjPt_mc_HT_JECup->GetErrorY(i);
    gMPF_RefObjPt_mc_HT_PUdown->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_HT_PUdown_Mean[i-1]);
    aMPF_RefObjPt_mc_HT_PUdown_MeanError[i-1] = gMPF_RefObjPt_mc_HT_PUdown->GetErrorY(i);
    gMPF_RefObjPt_mc_HT_PUup->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_HT_PUup_Mean[i-1]);
    aMPF_RefObjPt_mc_HT_PUup_MeanError[i-1] = gMPF_RefObjPt_mc_HT_PUup->GetErrorY(i);
    gMPF_RefObjPt_mc_HT_JERdown->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_HT_JERdown_Mean[i-1]);
    aMPF_RefObjPt_mc_HT_JERdown_MeanError[i-1] = gMPF_RefObjPt_mc_HT_JERdown->GetErrorY(i);
    gMPF_RefObjPt_mc_HT_JERup->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_HT_JERup_Mean[i-1]);
    aMPF_RefObjPt_mc_HT_JERup_MeanError[i-1] = gMPF_RefObjPt_mc_HT_JERup->GetErrorY(i);
    gMPF_RefObjPt_mc_HT_JECdown->GetPoint(i,aRefObjPtBins_resize_Mean[i-1],aMPF_RefObjPt_mc_HT_JECdown_Mean[i-1]);
    aMPF_RefObjPt_mc_HT_JECdown_MeanError[i-1] = gMPF_RefObjPt_mc_HT_JECdown->GetErrorY(i);
    aRefObjPtBins_resize_MeanError[i-1] = gMPF_RefObjPt_data->GetErrorX(i);
	}

	TGraphErrors* gMPF_RefObjPt_resize_data=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_data_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_data_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_HT=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_HT_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_HT_MeanError);	
  TGraphErrors* gMPF_RefObjPt_resize_mc_HT_JECup=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_HT_JECup_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_HT_JECup_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_HT_JECdown=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_HT_JECdown_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_HT_JECdown_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_HT_JERup=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_HT_JERup_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_HT_JERup_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_HT_JERdown=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_HT_JERdown_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_HT_JERdown_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_HT_PUup=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_HT_PUup_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_HT_PUup_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_HT_PUdown=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_HT_PUdown_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_HT_PUdown_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_Pt_pythia=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_Pt_pythia_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_Pt_pythia_MeanError);
	TGraphErrors* gMPF_RefObjPt_resize_mc_Pt_herwig=new TGraphErrors(numberPtBins-2,aRefObjPtBins_resize_Mean, aMPF_RefObjPt_mc_Pt_herwig_Mean, aRefObjPtBins_resize_MeanError, aMPF_RefObjPt_mc_Pt_herwig_MeanError);

	TGraph_style (gMPF_RefObjPt_resize_data);
	TGraph_style (gMPF_RefObjPt_resize_mc_HT);
	TGraph_style (gMPF_RefObjPt_resize_mc_HT_JECup);
	TGraph_style (gMPF_RefObjPt_resize_mc_HT_JECdown);
	TGraph_style (gMPF_RefObjPt_resize_mc_HT_JERup);
	TGraph_style (gMPF_RefObjPt_resize_mc_HT_JERdown);
	TGraph_style (gMPF_RefObjPt_resize_mc_HT_PUup);
	TGraph_style (gMPF_RefObjPt_resize_mc_HT_PUdown);
	TGraph_style (gMPF_RefObjPt_resize_mc_Pt_pythia);
	TGraph_style (gMPF_RefObjPt_resize_mc_Pt_herwig);

	gMPF_RefObjPt_resize_data->SetMarkerColor(1);
	gMPF_RefObjPt_resize_mc_HT->SetMarkerColor(2);
	gMPF_RefObjPt_resize_mc_HT_JECup->SetMarkerColor(4);
	gMPF_RefObjPt_resize_mc_HT_JECdown->SetMarkerColor(7);
  gMPF_RefObjPt_resize_mc_HT_JERup->SetMarkerColor(6);
	gMPF_RefObjPt_resize_mc_HT_JERdown->SetMarkerColor(46);
	gMPF_RefObjPt_resize_mc_HT_PUup->SetMarkerColor(38);
	gMPF_RefObjPt_resize_mc_HT_PUdown->SetMarkerColor(9);
	gMPF_RefObjPt_resize_mc_Pt_pythia->SetMarkerColor(42);
	gMPF_RefObjPt_resize_mc_Pt_herwig->SetMarkerColor(28);

	gMPF_RefObjPt_resize_data->SetLineColor(1);
	gMPF_RefObjPt_resize_mc_HT->SetLineColor(2);
	gMPF_RefObjPt_resize_mc_HT_JECup->SetLineColor(4);
	gMPF_RefObjPt_resize_mc_HT_JECdown->SetLineColor(7);
  gMPF_RefObjPt_resize_mc_HT_JERup->SetLineColor(6);
	gMPF_RefObjPt_resize_mc_HT_JERdown->SetLineColor(46);
	gMPF_RefObjPt_resize_mc_HT_PUup->SetLineColor(38);
	gMPF_RefObjPt_resize_mc_HT_PUdown->SetLineColor(9);
	gMPF_RefObjPt_resize_mc_Pt_pythia->SetLineColor(42);
	gMPF_RefObjPt_resize_mc_Pt_herwig->SetLineColor(28);
	
	TMultiGraph *mgMPF_RefObjPt_resize_JEC = new TMultiGraph();
	mgMPF_RefObjPt_resize_JEC->Add(gMPF_RefObjPt_resize_mc_HT,"pe");
	mgMPF_RefObjPt_resize_JEC->Add(gMPF_RefObjPt_resize_mc_HT_JECup,"pe");
	mgMPF_RefObjPt_resize_JEC->Add(gMPF_RefObjPt_resize_mc_HT_JECdown,"pe");

  TMultiGraph *mgMPF_RefObjPt_resize_JER = new TMultiGraph();
	mgMPF_RefObjPt_resize_JER->Add(gMPF_RefObjPt_resize_mc_HT,"pe");
	mgMPF_RefObjPt_resize_JER->Add(gMPF_RefObjPt_resize_mc_HT_JERup,"pe");
	mgMPF_RefObjPt_resize_JER->Add(gMPF_RefObjPt_resize_mc_HT_JERdown,"pe");

  TMultiGraph *mgMPF_RefObjPt_resize_PU = new TMultiGraph();
	mgMPF_RefObjPt_resize_PU->Add(gMPF_RefObjPt_resize_mc_HT,"pe");
	mgMPF_RefObjPt_resize_PU->Add(gMPF_RefObjPt_resize_mc_HT_PUup,"pe");
	mgMPF_RefObjPt_resize_PU->Add(gMPF_RefObjPt_resize_mc_HT_PUdown,"pe");

	TMultiGraph *mgMPF_RefObjPt_resize_generator = new TMultiGraph();
	mgMPF_RefObjPt_resize_generator->Add(gMPF_RefObjPt_resize_mc_HT,"pe");
	mgMPF_RefObjPt_resize_generator->Add(gMPF_RefObjPt_resize_mc_Pt_pythia,"pe");
	mgMPF_RefObjPt_resize_generator->Add(gMPF_RefObjPt_resize_mc_Pt_herwig,"pe");

  if(useRecoilPtBin) {
	  mgMPF_RefObjPt_resize_JEC->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
	  mgMPF_RefObjPt_resize_JER->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
	  mgMPF_RefObjPt_resize_generator->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
	  mgMPF_RefObjPt_resize_PU->SetTitle("MPF as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MPF");
  }
  else {
    mgMPF_RefObjPt_resize_JEC->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
	  mgMPF_RefObjPt_resize_JER->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
	  mgMPF_RefObjPt_resize_generator->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
	  mgMPF_RefObjPt_resize_PU->SetTitle("MPF as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];MPF");
  }
	
	drawComparisonResponse("r5b", mgMPF_RefObjPt_resize_JEC, gMPF_RefObjPt_resize_mc_HT, gMPF_RefObjPt_resize_mc_HT_JECup, gMPF_RefObjPt_resize_mc_HT_JECdown, "nominal", "JECup", "JECdown", "images/MPF_RefObjPt_resize_JEC.pdf");
	drawComparisonResponse("r6b", mgMPF_RefObjPt_resize_JER, gMPF_RefObjPt_resize_mc_HT, gMPF_RefObjPt_resize_mc_HT_JERup, gMPF_RefObjPt_resize_mc_HT_JERdown, "nominal", "JERup", "JERdown", "images/MPF_RefObjPt_resize_JER.pdf");
	drawComparisonResponse("r7b", mgMPF_RefObjPt_resize_PU, gMPF_RefObjPt_resize_mc_HT, gMPF_RefObjPt_resize_mc_HT_PUup, gMPF_RefObjPt_resize_mc_HT_PUdown, "nominal", "PUup", "PUdown", "images/MPF_RefObjPt_resize_PU.pdf");
	drawComparisonResponse("r8b", mgMPF_RefObjPt_resize_generator, gMPF_RefObjPt_resize_mc_HT, gMPF_RefObjPt_resize_mc_Pt_herwig, gMPF_RefObjPt_resize_mc_Pt_pythia, "HT", "Herwig", "Pythia", "images/MPF_RefObjPt_resize_generator.pdf");


//******************************************************************************************************************************************

  Double_t aMPFDeltaJEC[numberPtBins];
  Double_t aMPFDeltaJER[numberPtBins];
  Double_t aMPFDeltaPU[numberPtBins];
  Double_t aMPFDeltaSystTot[numberPtBins];

  for (int i = 0; i < numberPtBins; i++) {
    aY[i] = 0.;
    aRefObjPtBins_MeanError[i] = 0.;

    gMPF_RefObjPt_mc_HT_JECup->GetPoint(i,aRefObjPtBins_Mean[i],valueUp);
    gMPF_RefObjPt_mc_HT_JECdown->GetPoint(i,aRefObjPtBins_Mean[i],valueDown);
    aMPFDeltaJEC[i] = fabs(valueUp - valueDown)/2.;

    std::cout << "Bin " << i << "..." << std::endl;
    std::cout << "JEC: " << std::endl;
    std::cout << "valueUp: " << valueUp << std::endl;
    std::cout << "valueDown: " << valueDown << std::endl << std::endl;

    gMPF_RefObjPt_mc_HT_JERup->GetPoint(i,aRefObjPtBins_Mean[i],valueUp);
    gMPF_RefObjPt_mc_HT_JERdown->GetPoint(i,aRefObjPtBins_Mean[i],valueDown);
    aMPFDeltaJER[i] = fabs(valueUp - valueDown)/2.;

    std::cout << "JER: " << std::endl;
    std::cout << "valueUp: " << valueUp << std::endl;
    std::cout << "valueDown: " << valueDown << std::endl << std::endl;

    gMPF_RefObjPt_mc_HT_PUup->GetPoint(i,aRefObjPtBins_Mean[i],valueUp);
    gMPF_RefObjPt_mc_HT_PUdown->GetPoint(i,aRefObjPtBins_Mean[i],valueDown);
    aMPFDeltaPU[i] = fabs(valueUp - valueDown)/2.;

    std::cout << "PU: " << std::endl;
    std::cout << "valueUp: " << valueUp << std::endl;
    std::cout << "valueDown: " << valueDown << std::endl << std::endl;

    aMPFDeltaSystTot[i] = sqrt(aMPFDeltaJEC[i]*aMPFDeltaJEC[i] + aMPFDeltaJER[i]*aMPFDeltaJER[i] + aMPFDeltaPU[i]*aMPFDeltaPU[i]);

    std::cout << "Checking systematics value for bin " << i << std::endl;
    std::cout << "aMPFDeltaJEC[" << i << "]: " << aMPFDeltaJEC[i] << std::endl;
    std::cout << "aMPFDeltaJER[" << i << "]: " << aMPFDeltaJER[i] << std::endl;
    std::cout << "aMPFDeltaPU[" << i << "]: " << aMPFDeltaPU[i] << std::endl;
    std::cout << "aMPFDeltaSystTot[" << i << "]: " << aMPFDeltaSystTot[i] << std::endl << std::endl;

  }

  TGraphErrors *gMPF_JEC = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMPFDeltaJEC);
  TGraphErrors *gMPF_JER = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMPFDeltaJER);
  TGraphErrors *gMPF_PU = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMPFDeltaPU);
  TGraphErrors *gMPF_SystTot = new TGraphErrors(numberPtBins, aRefObjPtBins_Mean, aY, aRefObjPtBins_MeanError, aMPFDeltaSystTot);

//*********************************************************************************************************
//
//                                      Output file
//
//*********************************************************************************************************

	//create the output file and write into it
	TFile *out = new TFile("output_rootfile/compareMC.root", "recreate");
	
	out->cd();	

	TDirectory *mjbDir = out->mkdir("MJB","MJB");
	mjbDir->cd();
	TDirectory *ptbinDir = mjbDir->mkdir("PtBin","PtBin");
	ptbinDir->cd();
  gMJB_JEC->Write("gMJB_JEC");
  gMJB_JER->Write("gMJB_JER");
  gMJB_PU->Write("gMJB_PU");
  gMJB_SystTot->Write("gMJB_SystTot");
	
	TDirectory *mpfDir = out->mkdir("MPF","MPF");
	mpfDir->cd();
	TDirectory *ptbinmpfDir = mpfDir->mkdir("PtBin","PtBin");
	ptbinmpfDir->cd();
  gMPF_JEC->Write("gMPF_JEC");
  gMPF_JER->Write("gMPF_JER");
  gMPF_PU->Write("gMPF_PU");
  gMPF_SystTot->Write("gMPF_SystTot");

  out->Close();


  return 0;

}
