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
#include <tclap/CmdLine.h>
#include <boost/shared_ptr.hpp>

#include "tdrstyle_mod14.C"

#include "../common/common.h"

#include "../common/ptBinning.h"
#include "../common/HLTPtBinning.h"
#include "../common/logPtPrimeBinning.h"
#include "../common/npvBinning.h"
#include "../common/etaBinning.h"


using namespace std;

Int_t getMJBColorTrans() {
  Int_t MJBColorTrans = TColor::GetColor("#69D2E7");
  //Int_t MJBColorTrans = TColor::GetColor(MJBColorStr);
  gROOT->GetColor(MJBColorTrans)->SetAlpha(0.2);
  return MJBColorTrans;
}

Int_t getMPFColorTrans() {
  Int_t MPFColorTrans = TColor::GetColor("#F38630");
  //Int_t MPFColorTrans = TColor::GetColor(MPFColorStr);
  gROOT->GetColor(MPFColorTrans)->SetAlpha(0.2);
  return MPFColorTrans;
}

TGraphErrors* getDataMcResponseRatio(TGraphErrors* gData, TGraphErrors* gMc, int aNumberOfPtBins, string XTitle) {
  Double_t x[aNumberOfPtBins];
  Double_t ex[aNumberOfPtBins];
  for(int j=0;j<aNumberOfPtBins; j++) {
    ex[j] = 0.;
  }
  Double_t xratio[aNumberOfPtBins];
  Double_t ydata[aNumberOfPtBins];
  Double_t ymc[aNumberOfPtBins];
  Double_t yr[aNumberOfPtBins];
  Double_t eydata[aNumberOfPtBins];
  Double_t eymc[aNumberOfPtBins];
  Double_t eyr[aNumberOfPtBins];

  int nBins = 0;
  for(int i=0; i<aNumberOfPtBins; i++) {
    gMc->GetPoint(i,x[i],ymc[i]);
    gData->GetPoint(i,x[i],ydata[i]);
    eymc[i] = gMc->GetErrorY(i);
    eydata[i] = gData->GetErrorY(i);
    if (ymc[i] == 0 || ydata[i] == 0)
      continue;
    xratio[nBins] = x[i];
    yr[nBins] = ydata[i]/ymc[i];
    std::cout << yr[nBins] << std::endl;
    eyr[nBins] = sqrt(pow(eydata[i]/ymc[i],2)+pow(eymc[i]*ydata[i]/(pow(ymc[i],2)),2));
    nBins++;
  }

  TGraphErrors *gDataMcResponseratio = new TGraphErrors(nBins,xratio,yr,ex,eyr);
  gDataMcResponseratio->SetMarkerStyle(20);
  gDataMcResponseratio->SetMarkerColor(1);
  gDataMcResponseratio->SetLineColor(1);
  gDataMcResponseratio->SetMarkerSize(1.0);
  //gDataMcResponseratio->SetMaximum(1.08);
  //gDataMcResponseratio->SetMinimum(0.90);
  gDataMcResponseratio->GetXaxis()->SetTitle(XTitle.c_str());

  return gDataMcResponseratio;
}

void drawComparisonResponse(const string& canvasName, TMultiGraph *mgResponse, TGraphErrors *gResponseMC, TGraphErrors *gResponseData, TGraph *gratio,const string& mcSample, const string& path, bool doFit = false, bool setRangeUser = true) { // Mikko's tdrstyle_mod14
  TCanvas *c2 = tdrDiCanvas(canvasName.c_str(),mgResponse,gratio,2,0);
  c2->cd(1);

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  if (setRangeUser) {
    mgResponse->SetMaximum(1.05);
    mgResponse->SetMinimum(0.9);
  } else {
    mgResponse->SetMaximum(0.5);
    mgResponse->SetMinimum(0.3);    
  }
  mgResponse->Draw("P");
  gPad->RedrawAxis();
  c2->SetLogx(1);
  if(doFit) {
    TF1* myLinFit = new TF1("myLinFit", "[0]*x + [1]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
    myLinFit->SetParameter(0.5, 1.);
    myLinFit->SetLineColor(1);
    myLinFit->SetLineWidth(2);


    c2->Update();
    gResponseData->Fit(myLinFit, "RQ");
    double linfitValue = myLinFit->GetParameter(0);
    double linfitError = myLinFit->GetParError(0);

    TPaveText* linfitlabel = new TPaveText(0.55, 0.77, 0.88, 0.83, "brNDC");
    linfitlabel->SetTextSize(0.035);
    linfitlabel->SetFillColor(0);
    linfitlabel->SetTextFont(43);
    TString linfitLabelText = TString::Format("Data slope: %.5f #pm %.5f", linfitValue, linfitError);
    linfitlabel->AddText(linfitLabelText);
    linfitlabel->Draw("same");

    gPad->RedrawAxis();	
  }

  //TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  TLegend* legend = new TLegend(0.65, 0.15, 0.95, 0.30);
  legend->SetFillColor(kWhite);
  legend->SetFillStyle(kNone);
  legend->SetTextFont(43);
  legend->SetBorderSize(0);
  //legend->SetTextSize(0.045);
  legend->AddEntry(gResponseMC,"MC","p");
  legend->AddEntry(gResponseData,"Data 2012","p");
  legend->Draw("same");

  legend->Draw("same");

  c2->cd(2);

  TF1* ratioFit = new TF1("ratioFit", "[0]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
  ratioFit->SetParameter(0, 0.);
  ratioFit->SetLineColor(46);
  ratioFit->SetLineWidth(2);
  gratio->Draw("APE1");
  gratio->GetYaxis()->SetTitle("Data / MC");
  // 	//gratio->GetXaxis()->SetLabelOffset(0.1);
  // 	gratio->GetXaxis()->SetLabelFont(42);
  // 	gratio->GetXaxis()->SetLabelSize(0.06);
  // 	//gratio->GetYaxis()->SetLabelOffset(0.1);
  // 	gratio->GetYaxis()->SetLabelFont(42);
  // 	gratio->GetYaxis()->SetLabelSize(0.06);
  // 	gratio->GetXaxis()->SetTitleOffset(-0.6);
  // 	gratio->GetXaxis()->SetTitleFont(42);
  // 	gratio->GetXaxis()->SetTitleSize(0.06);
  //gratio->GetYaxis()->SetTitleOffset(1.3);
  // 	gratio->GetYaxis()->SetTitleFont(42);
  // 	gratio->GetYaxis()->SetTitleSize(0.06);

  gratio->SetMarkerSize(1.0);
  gratio->SetMaximum(1.05);
  gratio->SetMinimum(0.95);
  gratio->GetXaxis()->SetLimits(mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());

  c2->Update();
  gratio->Fit(ratioFit, "RQ");
  //gratio->GetYaxis()->SetRangeUser(-1,3);
  double fitValue = ratioFit->GetParameter(0);
  double fitError = ratioFit->GetParError(0);

  TPaveText* fitlabel = new TPaveText(0.55, 0.77, 0.88, 0.83, "brNDC");
  fitlabel->SetTextSize(0.08);
  fitlabel->SetFillColor(0);
  fitlabel->SetTextFont(42);
  TString fitLabelText = TString::Format("Fit: %.4f #pm %.4f", fitValue, fitError);
  fitlabel->AddText(fitLabelText);
  fitlabel->Draw("same");

  gPad->RedrawAxis();
  c2->SaveAs(path.c_str());
}

void mergePlotResponse(const string& canvasName, TMultiGraph *mgResponse, TGraph *gResponseMC_MJB, TGraph *gResponseData_MJB, TGraph *gratio_MJB, TGraph *gResponseMC_MPF, TGraph *gResponseData_MPF, TGraph *gratio_MPF, double y_min, double y_max, const string& label_MJB, const string& label_MPF, const string& path) { // Mikko's tdrstyle_mod14
  TCanvas *c3 = tdrDiCanvas(canvasName.c_str(),mgResponse,gratio_MJB,2,0);
  c3->cd(1);

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  mgResponse->SetMaximum(y_max);
  mgResponse->SetMinimum(y_min);
  mgResponse->Draw("P");
  gPad->RedrawAxis();
  //c3->SetLogx(1);

  //TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  TLegend* legend = new TLegend(0.63, 0.15, 0.93, 0.30);
  legend->SetFillColor(kWhite);
  legend->SetFillStyle(kNone);
  legend->SetTextFont(43);
  legend->SetBorderSize(0);
  //legend->SetTextSize(0.045);
  string toLeg = label_MJB + ", MC";
  legend->AddEntry(gResponseMC_MJB, toLeg.c_str(),"p");
  toLeg = label_MJB + ", data";
  legend->AddEntry(gResponseData_MJB,toLeg.c_str(),"p");
  toLeg = label_MPF + ", MC";
  legend->AddEntry(gResponseMC_MPF,toLeg.c_str(),"p");
  toLeg = label_MPF + ", data";
  legend->AddEntry(gResponseData_MPF,toLeg.c_str(),"p");
  legend->Draw("same");


  c3->cd(2);
  TF1* ratioFit_MJB = new TF1("ratioFit_MJB", "[0]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
  ratioFit_MJB->SetParameter(0, 0.);
  ratioFit_MJB->SetLineColor(gratio_MJB->GetLineColor());
  ratioFit_MJB->SetLineWidth(0);

  gratio_MJB->Draw("APE1");
  gratio_MJB->GetYaxis()->SetTitle("Data / MC");
  gratio_MJB->SetMarkerSize(1.0);
  gratio_MJB->SetMaximum(1.05);
  gratio_MJB->SetMinimum(0.95);
  gratio_MJB->GetXaxis()->SetLimits(mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());

  gratio_MPF->Draw("PE1same");

  c3->Update();
  gratio_MJB->Fit(ratioFit_MJB, "RQ");
  //gratio_MJB->GetYaxis()->SetRangeUser(-1,3);
  double fitValue_MJB = ratioFit_MJB->GetParameter(0);
  double fitError_MJB = ratioFit_MJB->GetParError(0);

  TF1* ratioFit_MPF = new TF1("ratioFit_MPF", "[0]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
  ratioFit_MPF->SetParameter(0, 0.);
  ratioFit_MPF->SetLineColor(gratio_MPF->GetLineColor());
  ratioFit_MPF->SetLineWidth(0);
  gratio_MPF->Fit(ratioFit_MPF, "RQ");
  //gratio_MPF->GetYaxis()->SetRangeUser(-1,3);
  double fitValue_MPF = ratioFit_MPF->GetParameter(0);
  double fitError_MPF = ratioFit_MPF->GetParError(0);

  TPaveText* fitlabel = new TPaveText(0.57, 0.82, 0.85, 0.84, "brNDC");
  fitlabel->SetTextSize(0.08);
  fitlabel->SetFillColor(0);
  fitlabel->SetTextFont(42);
  fitlabel->SetTextColor(gratio_MJB->GetLineColor());
  TString fitLabelTextMJB = TString::Format("Fit %s: %.4f #pm %.4f", label_MJB.c_str(), fitValue_MJB, fitError_MJB);
  fitlabel->AddText(fitLabelTextMJB);
  fitlabel->Draw("same");

  TPaveText* fitlabel_MPF = new TPaveText(0.57, 0.72, 0.85, 0.74, "brNDC");
  fitlabel_MPF->SetTextSize(0.08);
  fitlabel_MPF->SetFillColor(0);
  fitlabel_MPF->SetTextFont(42);
  fitlabel_MPF->SetTextColor(gratio_MPF->GetLineColor());
  TString fitLabelTextMPF = TString::Format("Fit %s: %.4f #pm %.4f", label_MPF.c_str(), fitValue_MPF, fitError_MPF);
  fitlabel_MPF->AddText(fitLabelTextMPF);
  fitlabel_MPF->Draw("same");

  gPad->RedrawAxis();
  c3->SaveAs(path.c_str());
}

void TGraph_style (TGraph* g, Int_t markerSyle, Int_t markerColor) {
	g->SetMarkerStyle(markerSyle);
	g->SetMarkerColor(markerColor);
	g->SetLineColor(markerColor);
	g->SetMarkerSize(1.0);	
}

int main (int argc, char** argv) 
{
  TString inname_data_mjb;
  TString inname_mc_mjb;
  TString inname_data_mpf;
  TString inname_mc_mpf;
  bool inLinScale = false;
  bool useSyst = false;
  string systFileNameMJB = "";
  string systFileNameMPF = "";

  string extension = ".pdf";	  	

  ptBinning myPtBinning;
  logPtPrimeBinning myLogPtPrimeBinning;
  HLTPtBinning myHLTPtBinning;
  npvBinning myNpvBinning;
  etaBinning myEtaBinning;

  //gStyle->SetOptFit(111111);

  Double_t xlow = getHistoXlow();
  Double_t xup = getHistoXup();
  //Double_t    binrange = 0.1;
  //Int_t    nbinsx = (xup - xlow)/binrange;
  Int_t    nbinsx = getHistoNbinsx();

  int numberPtBins = myPtBinning.getSize();
  int numberLogPtPrimeBins = myLogPtPrimeBinning.getSize();
  int numberHLTPtBins = myHLTPtBinning.getSize();
  int numberNpvBins = myNpvBinning.getSize();
  int numberEtaBins = myEtaBinning.getSize();

  TF1 *func = new TF1("func","[0]");
  func->SetParameter(0,1.);

  //********************************************************************************************************* 	

  try {
    // Define the command line object, and insert a message
    // that describes the program. The "Convert extractor tuples to histograms" 
    // is printed last in the help text. The second argument is the 
    // delimiter (usually space) and the last one is the version number. 
    // The CmdLine object parses the argv array based on the Arg objects
    // that it contains. 
    TCLAP::CmdLine cmd("Draw data/MC comparison merged plots", ' ', "1.0");

    // Define a value argument and add it to the command line.
    // A value arg defines a flag and a type of value that it expects,
    // such as "-n Bishop".
    TCLAP::ValueArg<std::string> dataMJBArg("", "data-mjb", "data input file name for MJB", true, "", "string");
    cmd.add( dataMJBArg );
    TCLAP::ValueArg<std::string> mcMJBArg("", "mc-mjb", "mc input file name for MJB", true, "", "string");
    cmd.add( mcMJBArg );
    TCLAP::ValueArg<std::string> dataMPFArg("", "data-mpf", "data input file name for MPF", true, "", "string");
    cmd.add( dataMPFArg );
    TCLAP::ValueArg<std::string> mcMPFArg("", "mc-mpf", "mc input file name for MPF", true, "", "string");
    cmd.add( mcMPFArg );

    TCLAP::ValueArg<std::string> plotNameArg("", "plotName", "plotName", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> extensionArg("", "extension", "extension", false, ".pdf", "string", cmd);

    // Define a switch and add it to the command line.
    // A switch arg is a boolean argument and only defines a flag that
    // indicates true or false.  In this example the SwitchArg adds itself
    // to the CmdLine object as part of the constructor.  This eliminates
    // the need to call the cmd.add() method.  All args have support in
    // their constructors to add themselves directly to the CmdLine object.
    // It doesn't matter which idiom you choose, they accomplish the same thing.

    TCLAP::SwitchArg linArg("", "lin", "Do you want linear scale?", false);
    cmd.add( linArg );
    TCLAP::SwitchArg useSystArg("", "useSyst", "Do you want to use systematics?", false);
    cmd.add(useSystArg);
    TCLAP::ValueArg<std::string> systFileMJBArg("", "syst-file-mjb", "The file containing the systematics-mjb", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> systFileMPFArg("", "syst-file-mpf", "The file containing the systematics-mjb", false, "", "string", cmd);

    // Parse the argv array.
    cmd.parse(argc, argv);

    // Get the value parsed by each arg.
    inname_data_mjb = dataMJBArg.getValue();
    inname_mc_mjb = mcMJBArg.getValue();
    inname_data_mpf = dataMPFArg.getValue();
    inname_mc_mpf = mcMPFArg.getValue();
    inLinScale = linArg.getValue();
    extension = "_woPUJets" + plotNameArg.getValue() + extensionArg.getValue();
    useSyst = useSystArg.getValue();
    systFileNameMJB = systFileMJBArg.getValue();    
    systFileNameMPF = systFileMPFArg.getValue();    


  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    return 1;
  }

  bool useRecoilPtHLTBin = true;
  myHLTPtBinning.fillHLTPtBins(useRecoilPtHLTBin);

  TFile *f_data_mjb=TFile::Open(inname_data_mjb);
  TFile *f_mc_mjb=TFile::Open(inname_mc_mjb);
  TFile *f_data_mpf=TFile::Open(inname_data_mpf);
  TFile *f_mc_mpf=TFile::Open(inname_mc_mpf);

  string myHistoName;

  setTDRStyle();

  TString MJBColorStr_data = "#13747D";
  //TString MJBColorStr_data = "#13498E";
  TString MPFColorStr_data = "#CC333F";

  Int_t MJBColor_data = TColor::GetColor(MJBColorStr_data);
  Int_t MPFColor_data = TColor::GetColor(MPFColorStr_data);

  //TString MJBColorStr_MC = "#0ABFBC";
  TString MJBColorStr_MC = "#00A0B0";
  TString MPFColorStr_MC = "#EB6841";

  Int_t MJBColor_MC = TColor::GetColor(MJBColorStr_MC);
  Int_t MPFColor_MC = TColor::GetColor(MPFColorStr_MC);

  Int_t MJBColorTrans = getMJBColorTrans();

  Int_t MPFColorTrans = getMPFColorTrans();

  Int_t MJBDataStyle = 24;
  Int_t MJBMCStyle = 25;
  Int_t MPFDataStyle = 20;
  Int_t MPFMCStyle = 21;



  //************************************************************************************************************
  //
  //                                      MJB as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gMJB_RefObjPt_data=(TGraphErrors*)f_data_mjb->Get("MJB/PtBin/gMJB_RefObjPt");
  TGraphErrors* gMJB_RefObjPt_mc=(TGraphErrors*)f_mc_mjb->Get("MJB/PtBin/gMJB_RefObjPt");

  if (useSyst) {
    if (systFileNameMJB == "") {
      std::cout << "ERROR! You must specify the file to use for systematics! *********************" << std::endl;
    }
    else {
      TFile *f_syst=TFile::Open(systFileNameMJB.c_str());
      TGraphErrors* gMJB_SystTot=(TGraphErrors*)f_syst->Get("MJB/PtBin/gMJB_SystTot");
      for(int i=0; i<numberPtBins; i++) {
        // quadratic sum of statitics and systematics errors
        gMJB_RefObjPt_mc->SetPointError(i, 0., sqrt(gMJB_RefObjPt_mc->GetErrorY(i)*gMJB_RefObjPt_mc->GetErrorY(i) + gMJB_SystTot->GetErrorY(i)*gMJB_SystTot->GetErrorY(i)));
      }
    }
  }

  TGraphErrors* gMJB_RefObjPt_mc_clone = (TGraphErrors*)gMJB_RefObjPt_mc->Clone();

  TGraph_style(gMJB_RefObjPt_data, MJBDataStyle, MJBColor_data);
  TGraph_style(gMJB_RefObjPt_mc, MJBMCStyle, MJBColor_MC);
  //TGraph_style(gMJB_RefObjPt_mc_clone, MJBMCStyle, MJBColor_MC);

  gMJB_RefObjPt_mc_clone->SetFillColor(MJBColorTrans);
  //gMJB_RefObjPt_mc_clone->SetFillStyle(3002);

  //gMJB_RefObjPt_mc->SetLineColor(MJBColorTrans);

  TMultiGraph *mgMJB_RefObjPt = new TMultiGraph();
  mgMJB_RefObjPt->Add(gMJB_RefObjPt_mc_clone,"e3");
  mgMJB_RefObjPt->Add(gMJB_RefObjPt_mc,"p");
  mgMJB_RefObjPt->Add(gMJB_RefObjPt_data,"pe");
  mgMJB_RefObjPt->SetTitle("MJB as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];Response");


  TGraphErrors *gMJB_RefObjPt_ratio = NULL;
  gMJB_RefObjPt_ratio = getDataMcResponseRatio(gMJB_RefObjPt_data,gMJB_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gMJB_RefObjPt_ratio, MJBDataStyle, MJBColor_MC);
  gMJB_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gMJB_RefObjPt_ratio->GetYaxis()->SetTitle("Data / MC");
  gMJB_RefObjPt_ratio->SetName("Data/MC");
  gMJB_RefObjPt_ratio->SetTitle("Data/MC");
  gMJB_RefObjPt_ratio->SetMarkerSize(1.0);
  gMJB_RefObjPt_ratio->Fit("func","","",gMJB_RefObjPt_data->GetXaxis()->GetXmin(),gMJB_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/response/MJB_RefObjPt" + extension;	
  //drawComparisonResponse("r1", mgMJB_RefObjPt, gMJB_RefObjPt_mc, gMJB_RefObjPt_data, gMJB_RefObjPt_ratio,"MC", myHistoName.c_str());

  //************************************************************************************************************
  //
  //                                      MJB as a function of ptrecoil without last ptbin (response equal to zero)
  //
  //************************************************************************************************************

  Double_t aMJB_RefObjPt_data_Mean[numberPtBins-3];
  Double_t aMJB_RefObjPt_data_MeanError[numberPtBins-3];
  Double_t aMJB_RefObjPt_mc_Mean[numberPtBins-3];
  Double_t aMJB_RefObjPt_mc_MeanError[numberPtBins-3];
  Double_t aMJBRatio_Mean[numberPtBins-3];
  Double_t aMJBRatio_MeanError[numberPtBins-3];
  Double_t aRefObjPtBins_Mean[numberPtBins-3];
  Double_t aRefObjPtBins_MeanError[numberPtBins-3];

  for(int i=1; i<numberPtBins-2; i++) {
    gMJB_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i-1],aMJB_RefObjPt_data_Mean[i-1]);
    gMJB_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i-1],aMJB_RefObjPt_mc_Mean[i-1]);
    gMJB_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i-1],aMJBRatio_Mean[i-1]);
    aMJB_RefObjPt_data_MeanError[i-1] = gMJB_RefObjPt_data->GetErrorY(i);
    aMJB_RefObjPt_mc_MeanError[i-1] = gMJB_RefObjPt_mc->GetErrorY(i);
    aRefObjPtBins_MeanError[i-1] = gMJB_RefObjPt_data->GetErrorX(i);
    aMJBRatio_MeanError[i-1] = gMJB_RefObjPt_ratio->GetErrorY(i);
  }

  TGraphErrors* gMJB_RefObjPt_data_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, aMJB_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_data_MeanError);
  TGraphErrors* gMJB_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_mc_MeanError);	
  TGraph* gMJB_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-3,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean);	

  TGraphErrors* gMJB_RefObjPt_mc_clone_resize = (TGraphErrors*)gMJB_RefObjPt_mc_resize->Clone();

  TGraph_style(gMJB_RefObjPt_data_resize, MJBDataStyle, MJBColor_data);
  TGraph_style(gMJB_RefObjPt_mc_resize, MJBMCStyle, MJBColor_MC);
  TGraph_style(gMJB_RefObjPt_mc_resize_pointsOnly, MJBMCStyle, MJBColor_MC);
  //TGraph_style(gMJB_RefObjPt_mc_clone_resize, MJBMCStyle, MJBColor_MC);

  gMJB_RefObjPt_mc_clone_resize->SetFillColor(MJBColorTrans);
  //gMJB_RefObjPt_mc_clone_resize->SetFillStyle(3002);
  //gMJB_RefObjPt_mc_resize->SetLineColor(MJBColorTrans);

  TMultiGraph *mgMJB_RefObjPt_resize = new TMultiGraph();
  mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_clone_resize,"e3");
  mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize_pointsOnly,"p");
  mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_data_resize,"pe");
  mgMJB_RefObjPt_resize->SetTitle("MJB as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];Response");

  TGraphErrors *gMJB_RefObjPt_ratio_resize = NULL;
  gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gMJB_RefObjPt_ratio_resize, MJBDataStyle, MJBColor_MC);
  gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gMJB_RefObjPt_ratio_resize->GetYaxis()->SetTitle("Data / MC");
  gMJB_RefObjPt_ratio_resize->SetName("Data/MC");
  gMJB_RefObjPt_ratio_resize->SetTitle("Data/MC");
  gMJB_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gMJB_RefObjPt_ratio_resize->Fit("func","","",gMJB_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMJB_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/response/MJB_RefObjPt_resize" + extension;	
  //drawComparisonResponse("r1", mgMJB_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize,"MC", myHistoName.c_str());


  //************************************************************************************************************
  //
  //                                      MPF as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gMPF_RefObjPt_data=(TGraphErrors*)f_data_mpf->Get("MPF/PtBin/gMPF_RefObjPt");
  TGraphErrors* gMPF_RefObjPt_mc=(TGraphErrors*)f_mc_mpf->Get("MPF/PtBin/gMPF_RefObjPt");

  if (useSyst) {
    if (systFileNameMPF == "") {
      std::cout << "ERROR! You must specify the file to use for systematics! *********************" << std::endl;
    }
    else {
      TFile *f_syst=TFile::Open(systFileNameMPF.c_str());
      TGraphErrors* gMPF_SystTot=(TGraphErrors*)f_syst->Get("MPF/PtBin/gMPF_SystTot");
      for(int i=0; i<numberPtBins; i++) {
        // quadratic sum of statitics and systematics errors
        gMPF_RefObjPt_mc->SetPointError(i, 0., sqrt(gMPF_RefObjPt_mc->GetErrorY(i)*gMPF_RefObjPt_mc->GetErrorY(i) + gMPF_SystTot->GetErrorY(i)*gMPF_SystTot->GetErrorY(i)));
      }
    }
  }

  TGraphErrors* gMPF_RefObjPt_mc_clone = (TGraphErrors*)gMPF_RefObjPt_mc->Clone();

  TGraph_style(gMPF_RefObjPt_data, MPFDataStyle, MPFColor_data);
  TGraph_style(gMPF_RefObjPt_mc, MPFMCStyle, MPFColor_MC);
  //TGraph_style(gMPF_RefObjPt_mc_clone, MPFMCStyle, MPFColor_MC);

  gMPF_RefObjPt_mc_clone->SetFillColor(MPFColorTrans);
  //gMPF_RefObjPt_mc_clone->SetFillStyle(3002);
  //gMPF_RefObjPt_mc->SetLineColor(MPFColorTrans);

  TMultiGraph *mgMPF_RefObjPt = new TMultiGraph();
  mgMPF_RefObjPt->Add(gMPF_RefObjPt_mc_clone,"e3");
  mgMPF_RefObjPt->Add(gMPF_RefObjPt_mc,"p");
  mgMPF_RefObjPt->Add(gMPF_RefObjPt_data,"pe");
  mgMPF_RefObjPt->SetTitle("MPF as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];Response");
  TGraphErrors *gMPF_RefObjPt_ratio = NULL;
  gMPF_RefObjPt_ratio = getDataMcResponseRatio(gMPF_RefObjPt_data,gMPF_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gMPF_RefObjPt_ratio, MPFDataStyle, MPFColor_MC);
  gMPF_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gMPF_RefObjPt_ratio->GetYaxis()->SetTitle("Data / MC");
  gMPF_RefObjPt_ratio->SetName("Data/MC");
  gMPF_RefObjPt_ratio->SetTitle("Data/MC");
  gMPF_RefObjPt_ratio->SetMarkerSize(1.0);
  gMPF_RefObjPt_ratio->Fit("func","","",gMPF_RefObjPt_data->GetXaxis()->GetXmin(),gMPF_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/response/MPF_RefObjPt" + extension;	
  //drawComparisonResponse("r1", mgMPF_RefObjPt, gMPF_RefObjPt_mc, gMPF_RefObjPt_data, gMPF_RefObjPt_ratio,"MC", myHistoName.c_str());

  //************************************************************************************************************
  //
  //                                      MPF as a function of ptrecoil without 2 ptbin
  //
  //************************************************************************************************************

  Double_t aMPF_RefObjPt_data_Mean[numberPtBins-3];
  Double_t aMPF_RefObjPt_data_MeanError[numberPtBins-3];
  Double_t aMPF_RefObjPt_mc_Mean[numberPtBins-3];
  Double_t aMPF_RefObjPt_mc_MeanError[numberPtBins-3];
  Double_t aMPFRatio_Mean[numberPtBins-3];
  Double_t aMPFRatio_MeanError[numberPtBins-3];

  for(int i=1; i<numberPtBins-2; i++) {
    gMPF_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i-1],aMPF_RefObjPt_data_Mean[i-1]);
    gMPF_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i-1],aMPF_RefObjPt_mc_Mean[i-1]);
    gMPF_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i-1],aMPFRatio_Mean[i-1]);
    aMPF_RefObjPt_data_MeanError[i-1] = gMPF_RefObjPt_data->GetErrorY(i);
    aMPF_RefObjPt_mc_MeanError[i-1] = gMPF_RefObjPt_mc->GetErrorY(i);
    aRefObjPtBins_MeanError[i-1] = gMPF_RefObjPt_data->GetErrorX(i);
    aMPFRatio_MeanError[i-1] = gMPF_RefObjPt_ratio->GetErrorY(i);
  }

  TGraphErrors* gMPF_RefObjPt_data_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, aMPF_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aMPF_RefObjPt_data_MeanError);
  TGraphErrors* gMPF_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, aMPF_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aMPF_RefObjPt_mc_MeanError);	
  TGraph* gMPF_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-3,aRefObjPtBins_Mean, aMPF_RefObjPt_mc_Mean);	

  TGraphErrors* gMPF_RefObjPt_mc_clone_resize = (TGraphErrors*)gMPF_RefObjPt_mc_resize->Clone();

  TGraph_style(gMPF_RefObjPt_data_resize, MPFDataStyle, MPFColor_data);
  TGraph_style(gMPF_RefObjPt_mc_resize, MPFMCStyle, MPFColor_MC);
  TGraph_style(gMPF_RefObjPt_mc_resize_pointsOnly, MPFMCStyle, MPFColor_MC);
  //TGraph_style(gMPF_RefObjPt_mc_clone_resize, MPFMCStyle, MPFColor_MC);

  gMPF_RefObjPt_mc_clone_resize->SetFillColor(MPFColorTrans);
  //gMPF_RefObjPt_mc_clone_resize->SetFillStyle(3002);
  //gMPF_RefObjPt_mc_resize->SetLineColor(MPFColorTrans);

  TMultiGraph *mgMPF_RefObjPt_resize = new TMultiGraph();
  mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_mc_clone_resize,"e3");
  mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_mc_resize_pointsOnly,"p");
  mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_data_resize,"pe");
  mgMPF_RefObjPt_resize->SetTitle("MPF as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];Response");

  TGraphErrors *gMPF_RefObjPt_ratio_resize = NULL;
  gMPF_RefObjPt_ratio_resize = getDataMcResponseRatio(gMPF_RefObjPt_data_resize,gMPF_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gMPF_RefObjPt_ratio_resize, MPFDataStyle, MPFColor_MC);
  gMPF_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gMPF_RefObjPt_ratio_resize->GetYaxis()->SetTitle("Data / MC");
  gMPF_RefObjPt_ratio_resize->SetName("Data/MC");
  gMPF_RefObjPt_ratio_resize->SetTitle("Data/MC");
  gMPF_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gMPF_RefObjPt_ratio_resize->Fit("func","","",gMPF_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMPF_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/response/MPF_RefObjPt_resize" + extension;	
  //drawComparisonResponse("r1", mgMPF_RefObjPt_resize, gMPF_RefObjPt_mc_resize, gMPF_RefObjPt_data_resize, gMPF_RefObjPt_ratio_resize,"MC", myHistoName.c_str());

   myHistoName = "images/response/MPF_RefObjPt_resize_test" + extension;	
  //drawComparisonResponse("r2", mgMPF_RefObjPt_resize, gMPF_RefObjPt_mc_resize, gMPF_RefObjPt_data_resize, gMPF_RefObjPt_ratio_resize,"MC", myHistoName.c_str());


  //************************************************************************************************************
  //
  //                                      Response as a function of ptrecoil, resize
  //
  //************************************************************************************************************

  myHistoName = "images/response/Response_RefObjPt_resize" + extension;

  TMultiGraph *mgAll_RefObjPt_resize = new TMultiGraph();
  mgAll_RefObjPt_resize->Add(gMJB_RefObjPt_mc_clone_resize,"e3");
  mgAll_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize_pointsOnly,"p");
  mgAll_RefObjPt_resize->Add(gMJB_RefObjPt_data_resize,"pe");
  mgAll_RefObjPt_resize->Add(gMPF_RefObjPt_mc_clone_resize,"e3");
  mgAll_RefObjPt_resize->Add(gMPF_RefObjPt_mc_resize_pointsOnly,"p");
  mgAll_RefObjPt_resize->Add(gMPF_RefObjPt_data_resize,"pe");

  mgAll_RefObjPt_resize->SetTitle("Response as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];Response");


   mergePlotResponse("r5", mgAll_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize, gMPF_RefObjPt_mc_resize, gMPF_RefObjPt_data_resize, gMPF_RefObjPt_ratio_resize, 0.92+0.0001, 1.02, "MJB", "MPF", myHistoName.c_str());

 

  //***********************************************************************************************************
  //
  //                                      cExp as a function of reference object pt for MJB 
  //
  //***********************************************************************************************************

  TGraphErrors* gcExp_RefObjPt_data_mjb=(TGraphErrors*)f_data_mjb->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");
  TGraphErrors* gcExp_RefObjPt_mc_mjb=(TGraphErrors*)f_mc_mjb->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");

  TGraphErrors* gcExp_RefObjPt_mc_mjb_clone = (TGraphErrors*)gcExp_RefObjPt_mc_mjb->Clone();

  TGraph_style(gcExp_RefObjPt_data_mjb, MJBDataStyle, MJBColor_data);
  TGraph_style(gcExp_RefObjPt_mc_mjb, MJBMCStyle, MJBColor_MC);
  //TGraph_style(gcExp_RefObjPt_mc_mjb_clone, MJBMCStyle, MJBColor_MC);

  gcExp_RefObjPt_mc_mjb_clone->SetFillColor(MJBColorTrans);
  //gcExp_RefObjPt_mc_mjb_clone->SetFillStyle(3002);
  //gcExp_RefObjPt_mc_mjb->SetLineColor(MJBColorTrans);

  TMultiGraph *mgcExp_RefObjPt_mjb = new TMultiGraph();
  mgcExp_RefObjPt_mjb->Add(gcExp_RefObjPt_mc_mjb_clone,"e3");
  mgcExp_RefObjPt_mjb->Add(gcExp_RefObjPt_mc_mjb,"p");
  mgcExp_RefObjPt_mjb->Add(gcExp_RefObjPt_data_mjb,"pe");
  mgcExp_RefObjPt_mjb->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];C_{recoil}");


  TGraphErrors *gcExp_RefObjPt_ratio_mjb = NULL;
  gcExp_RefObjPt_ratio_mjb = getDataMcResponseRatio(gcExp_RefObjPt_data_mjb,gcExp_RefObjPt_mc_mjb,numberPtBins, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gcExp_RefObjPt_ratio_mjb, MJBDataStyle, MJBColor_MC);
  gcExp_RefObjPt_ratio_mjb->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gcExp_RefObjPt_ratio_mjb->GetYaxis()->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mjb->SetName("Data / MC");
  gcExp_RefObjPt_ratio_mjb->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mjb->SetMarkerSize(1.0);
  gcExp_RefObjPt_ratio_mjb->Fit("func","","",gcExp_RefObjPt_data_mjb->GetXaxis()->GetXmin(),gcExp_RefObjPt_data_mjb->GetXaxis()->GetXmax());

  myHistoName = "images/cExp/cExp_sum_Fi_log_fi_RecoilPt_RefObjPt_mjb" + extension;	
  //drawComparisonResponse("r1", mgcExp_RefObjPt_mjb, gcExp_RefObjPt_mc_mjb, gcExp_RefObjPt_data_mjb, gcExp_RefObjPt_ratio_mjb,"MC", myHistoName.c_str(), false, false);

  //***********************************************************************************************************
  //
  //                                      cExp as a function of ptrecoil without 2 ptbin for MJB
  //
  //***********************************************************************************************************

  Double_t acExp_RefObjPt_data_mjb_Mean[numberPtBins-3];
  Double_t acExp_RefObjPt_data_mjb_MeanError[numberPtBins-3];
  Double_t acExp_RefObjPt_mc_mjb_Mean[numberPtBins-3];
  Double_t acExp_RefObjPt_mc_mjb_MeanError[numberPtBins-3];
  Double_t acExpRatio_Mean[numberPtBins-3];
  Double_t acExpRatio_MeanError[numberPtBins-3];

  for(int i=1; i<numberPtBins-2; i++) {
    gcExp_RefObjPt_data_mjb->GetPoint(i,aRefObjPtBins_Mean[i-1],acExp_RefObjPt_data_mjb_Mean[i-1]);
    gcExp_RefObjPt_mc_mjb->GetPoint(i,aRefObjPtBins_Mean[i-1],acExp_RefObjPt_mc_mjb_Mean[i-1]);
    gcExp_RefObjPt_ratio_mjb->GetPoint(i,aRefObjPtBins_Mean[i-1],acExpRatio_Mean[i-1]);
    acExp_RefObjPt_data_mjb_MeanError[i-1] = gcExp_RefObjPt_data_mjb->GetErrorY(i);
    acExp_RefObjPt_mc_mjb_MeanError[i-1] = gcExp_RefObjPt_mc_mjb->GetErrorY(i);
    aRefObjPtBins_MeanError[i-1] = gcExp_RefObjPt_data_mjb->GetErrorX(i);
    acExpRatio_MeanError[i-1] = gcExp_RefObjPt_ratio_mjb->GetErrorY(i);
  }

  TGraphErrors* gcExp_RefObjPt_data_mjb_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_data_mjb_Mean, aRefObjPtBins_MeanError, acExp_RefObjPt_data_mjb_MeanError);
  TGraphErrors* gcExp_RefObjPt_mc_mjb_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_mc_mjb_Mean, aRefObjPtBins_MeanError, acExp_RefObjPt_mc_mjb_MeanError);	
  TGraph* gcExp_RefObjPt_mc_mjb_resize_pointsOnly = new TGraph(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_mc_mjb_Mean);	

  TGraphErrors* gcExp_RefObjPt_mc_mjb_clone_resize = (TGraphErrors*)gcExp_RefObjPt_mc_mjb_resize->Clone();

  TGraph_style(gcExp_RefObjPt_data_mjb_resize, MJBDataStyle, MJBColor_data);
  TGraph_style(gcExp_RefObjPt_mc_mjb_resize, MJBMCStyle, MJBColor_MC);
  TGraph_style(gcExp_RefObjPt_mc_mjb_resize_pointsOnly, MJBMCStyle, MJBColor_MC);
  //TGraph_style(gcExp_RefObjPt_mc_mjb_clone_resize, MJBMCStyle, MJBColor_MC);

  gcExp_RefObjPt_mc_mjb_clone_resize->SetFillColor(MJBColorTrans);
  //gcExp_RefObjPt_mc_mjb_clone_resize->SetFillStyle(3002);
  //gcExp_RefObjPt_mc_mjb_resize_pointsOnly->SetLineColor(MJBColorTrans);

  TMultiGraph *mgcExp_RefObjPt_mjb_resize = new TMultiGraph();
  mgcExp_RefObjPt_mjb_resize->Add(gcExp_RefObjPt_mc_mjb_clone_resize,"e3");
  mgcExp_RefObjPt_mjb_resize->Add(gcExp_RefObjPt_mc_mjb_resize_pointsOnly,"p");
  mgcExp_RefObjPt_mjb_resize->Add(gcExp_RefObjPt_data_mjb_resize,"pe");
  mgcExp_RefObjPt_mjb_resize->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];C_{recoil}");

  TGraphErrors *gcExp_RefObjPt_ratio_mjb_resize = NULL;
  gcExp_RefObjPt_ratio_mjb_resize = getDataMcResponseRatio(gcExp_RefObjPt_data_mjb_resize,gcExp_RefObjPt_mc_mjb_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gcExp_RefObjPt_ratio_mjb_resize, MJBDataStyle, MJBColor_MC);
  gcExp_RefObjPt_ratio_mjb_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gcExp_RefObjPt_ratio_mjb_resize->GetYaxis()->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mjb_resize->SetName("Data / MC");
  gcExp_RefObjPt_ratio_mjb_resize->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mjb_resize->SetMarkerSize(1.0);
  gcExp_RefObjPt_ratio_mjb_resize->Fit("func","","",gcExp_RefObjPt_data_mjb_resize->GetXaxis()->GetXmin(),gcExp_RefObjPt_data_mjb_resize->GetXaxis()->GetXmax());

  myHistoName = "images/cExp/cExp_sum_Fi_log_fi_RecoilPt_RefObjPt_mjb_resize" + extension;	
  //drawComparisonResponse("r1", mgcExp_RefObjPt_mjb_resize, gcExp_RefObjPt_mc_mjb_resize, gcExp_RefObjPt_data_mjb_resize, gcExp_RefObjPt_ratio_mjb_resize,"MC", myHistoName.c_str(), false, false);


  //***********************************************************************************************************
  //
  //                                      cExp as a function of reference object pt for MPF 
  //
  //***********************************************************************************************************

  TGraphErrors* gcExp_RefObjPt_data_mpf=(TGraphErrors*)f_data_mpf->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");
  TGraphErrors* gcExp_RefObjPt_mc_mpf=(TGraphErrors*)f_mc_mpf->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");

  TGraphErrors* gcExp_RefObjPt_mc_mpf_clone = (TGraphErrors*)gcExp_RefObjPt_mc_mpf->Clone();

  TGraph_style(gcExp_RefObjPt_data_mpf, MPFDataStyle, MPFColor_data);
  TGraph_style(gcExp_RefObjPt_mc_mpf, MPFMCStyle, MPFColor_MC);
  //TGraph_style(gcExp_RefObjPt_mc_mpf_clone, MPFMCStyle, MPFColor_MC);

  gcExp_RefObjPt_mc_mpf_clone->SetFillColor(MPFColorTrans);
  //gcExp_RefObjPt_mc_mpf_clone->SetFillStyle(3002);
  //gcExp_RefObjPt_mc_mpf->SetLineColor(MPFColorTrans);

  TMultiGraph *mgcExp_RefObjPt_mpf = new TMultiGraph();
  mgcExp_RefObjPt_mpf->Add(gcExp_RefObjPt_mc_mpf_clone,"e3");
  mgcExp_RefObjPt_mpf->Add(gcExp_RefObjPt_mc_mpf,"p");
  mgcExp_RefObjPt_mpf->Add(gcExp_RefObjPt_data_mpf,"pe");
  mgcExp_RefObjPt_mpf->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];C_{recoil}");


  TGraphErrors *gcExp_RefObjPt_ratio_mpf = NULL;
  gcExp_RefObjPt_ratio_mpf = getDataMcResponseRatio(gcExp_RefObjPt_data_mpf,gcExp_RefObjPt_mc_mpf,numberPtBins, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gcExp_RefObjPt_ratio_mpf, MPFDataStyle, MPFColor_MC);
  gcExp_RefObjPt_ratio_mpf->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gcExp_RefObjPt_ratio_mpf->GetYaxis()->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mpf->SetName("Data / MC");
  gcExp_RefObjPt_ratio_mpf->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mpf->SetMarkerSize(1.0);
  gcExp_RefObjPt_ratio_mpf->Fit("func","","",gcExp_RefObjPt_data_mpf->GetXaxis()->GetXmin(),gcExp_RefObjPt_data_mpf->GetXaxis()->GetXmax());

  myHistoName = "images/cExp/cExp_sum_Fi_log_fi_RecoilPt_RefObjPt_mpf" + extension;	
  //drawComparisonResponse("r1", mgcExp_RefObjPt_mpf, gcExp_RefObjPt_mc_mpf, gcExp_RefObjPt_data_mpf, gcExp_RefObjPt_ratio_mpf,"MC", myHistoName.c_str(), false, false);

  //***********************************************************************************************************
  //
  //                                      cExp as a function of ptrecoil without 2 ptbin for MPF
  //
  //***********************************************************************************************************

  Double_t acExp_RefObjPt_data_mpf_Mean[numberPtBins-3];
  Double_t acExp_RefObjPt_data_mpf_MeanError[numberPtBins-3];
  Double_t acExp_RefObjPt_mc_mpf_Mean[numberPtBins-3];
  Double_t acExp_RefObjPt_mc_mpf_MeanError[numberPtBins-3];

  for(int i=1; i<numberPtBins-2; i++) {
    gcExp_RefObjPt_data_mpf->GetPoint(i,aRefObjPtBins_Mean[i-1],acExp_RefObjPt_data_mpf_Mean[i-1]);
    gcExp_RefObjPt_mc_mpf->GetPoint(i,aRefObjPtBins_Mean[i-1],acExp_RefObjPt_mc_mpf_Mean[i-1]);
    gcExp_RefObjPt_ratio_mpf->GetPoint(i,aRefObjPtBins_Mean[i-1],acExpRatio_Mean[i-1]);
    acExp_RefObjPt_data_mpf_MeanError[i-1] = gcExp_RefObjPt_data_mpf->GetErrorY(i);
    acExp_RefObjPt_mc_mpf_MeanError[i-1] = gcExp_RefObjPt_mc_mpf->GetErrorY(i);
    aRefObjPtBins_MeanError[i-1] = gcExp_RefObjPt_data_mpf->GetErrorX(i);
    acExpRatio_MeanError[i-1] = gcExp_RefObjPt_ratio_mpf->GetErrorY(i);
  }

  TGraphErrors* gcExp_RefObjPt_data_mpf_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_data_mpf_Mean, aRefObjPtBins_MeanError, acExp_RefObjPt_data_mpf_MeanError);
  TGraphErrors* gcExp_RefObjPt_mc_mpf_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_mc_mpf_Mean, aRefObjPtBins_MeanError, acExp_RefObjPt_mc_mpf_MeanError);	
  TGraph* gcExp_RefObjPt_mc_mpf_resize_pointsOnly = new TGraph(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_mc_mpf_Mean);	

  TGraphErrors* gcExp_RefObjPt_mc_mpf_clone_resize = (TGraphErrors*)gcExp_RefObjPt_mc_mpf_resize->Clone();

  TGraph_style(gcExp_RefObjPt_data_mpf_resize, MPFDataStyle, MPFColor_data);
  TGraph_style(gcExp_RefObjPt_mc_mpf_resize, MPFMCStyle, MPFColor_MC);
  TGraph_style(gcExp_RefObjPt_mc_mpf_resize_pointsOnly, MPFMCStyle, MPFColor_MC);
  //TGraph_style(gcExp_RefObjPt_mc_mpf_clone_resize, MPFMCStyle, MPFColor_MC);

  gcExp_RefObjPt_mc_mpf_clone_resize->SetFillColor(MPFColorTrans);
  //gcExp_RefObjPt_mc_mpf_clone_resize->SetFillStyle(3002);
  //gcExp_RefObjPt_mc_mpf_resize_pointsOnly->SetLineColor(MPFColorTrans);

  TMultiGraph *mgcExp_RefObjPt_mpf_resize = new TMultiGraph();
  mgcExp_RefObjPt_mpf_resize->Add(gcExp_RefObjPt_mc_mpf_clone_resize,"e3");
  mgcExp_RefObjPt_mpf_resize->Add(gcExp_RefObjPt_mc_mpf_resize_pointsOnly,"p");
  mgcExp_RefObjPt_mpf_resize->Add(gcExp_RefObjPt_data_mpf_resize,"pe");
  mgcExp_RefObjPt_mpf_resize->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];C_{recoil}");

  TGraphErrors *gcExp_RefObjPt_ratio_mpf_resize = NULL;
  gcExp_RefObjPt_ratio_mpf_resize = getDataMcResponseRatio(gcExp_RefObjPt_data_mpf_resize,gcExp_RefObjPt_mc_mpf_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
  TGraph_style(gcExp_RefObjPt_ratio_mpf_resize, MPFDataStyle, MPFColor_MC);
  gcExp_RefObjPt_ratio_mpf_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  gcExp_RefObjPt_ratio_mpf_resize->GetYaxis()->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mpf_resize->SetName("Data / MC");
  gcExp_RefObjPt_ratio_mpf_resize->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_mpf_resize->SetMarkerSize(1.0);
  gcExp_RefObjPt_ratio_mpf_resize->Fit("func","","",gcExp_RefObjPt_data_mpf_resize->GetXaxis()->GetXmin(),gcExp_RefObjPt_data_mpf_resize->GetXaxis()->GetXmax());

  myHistoName = "images/cExp/cExp_sum_Fi_log_fi_RecoilPt_RefObjPt_mpf_resize" + extension;	
  //drawComparisonResponse("r1", mgcExp_RefObjPt_mpf_resize, gcExp_RefObjPt_mc_mpf_resize, gcExp_RefObjPt_data_mpf_resize, gcExp_RefObjPt_ratio_mpf_resize,"MC", myHistoName.c_str(), false, false);


  //************************************************************************************************************
  //
  //                                      cResoil as a function of ptrecoil, resize
  //
  //************************************************************************************************************

  myHistoName = "images/cExp/Crecoil_RefObjPt_resize" + extension;

  TMultiGraph *mgAll_crecoil_RefObjPt_resize = new TMultiGraph();
  mgAll_crecoil_RefObjPt_resize->Add(gcExp_RefObjPt_mc_mjb_clone_resize,"e3");
  mgAll_crecoil_RefObjPt_resize->Add(gcExp_RefObjPt_mc_mjb_resize_pointsOnly,"p");
  mgAll_crecoil_RefObjPt_resize->Add(gcExp_RefObjPt_data_mjb_resize,"pe");
  mgAll_crecoil_RefObjPt_resize->Add(gcExp_RefObjPt_mc_mpf_clone_resize,"e3");
  mgAll_crecoil_RefObjPt_resize->Add(gcExp_RefObjPt_mc_mpf_resize_pointsOnly,"p");
  mgAll_crecoil_RefObjPt_resize->Add(gcExp_RefObjPt_data_mpf_resize,"pe");

  mgAll_crecoil_RefObjPt_resize->SetTitle("Crecoil as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];C_{recoil}");


   mergePlotResponse("r5", mgAll_crecoil_RefObjPt_resize, gcExp_RefObjPt_mc_mjb_resize_pointsOnly, gcExp_RefObjPt_data_mjb_resize, gcExp_RefObjPt_ratio_mjb_resize, gcExp_RefObjPt_mc_mpf_resize_pointsOnly, gcExp_RefObjPt_data_mpf_resize, gcExp_RefObjPt_ratio_mpf_resize, 0.34+0.0001, 0.46, "p_{T} > 30 GeV", "p_{T} > 10 GeV", myHistoName.c_str());



  return 0;
}
