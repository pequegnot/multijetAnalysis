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

#include "../common/common.h"

#include "../common/ptBinning.h"
#include "../common/npvBinning.h"
#include "../common/etaBinning.h"


using namespace std;


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


void drawDataMcComparison(const string& canvasName, TH1 *hMc, TH1 *hData, const string& Xtitle, const string& path, bool inLinScale = false, const string& Ytitle = "", string legendPos = "r", int withratio = 1){
	TCanvas *cCanvas = new TCanvas(canvasName.c_str(),canvasName.c_str(), 600, 800);
	cCanvas->cd();
	if(withratio == 1) {
  		// Data / MC comparison
  		TPad* pad_hi = new TPad("pad_hi", "", 0., 0.33, 0.99, 0.99);
  		pad_hi->Draw();
  		//pad_hi->SetLogx();
  		pad_hi->SetLeftMargin(0.12);
  		pad_hi->SetBottomMargin(0.015);

  		// Data / MC ratio
		TPad* pad_lo = new TPad("pad_lo", "", 0., 0., 0.99, 0.33);
		pad_lo->Draw();
		//pad_lo->SetLogx();
		pad_lo->SetLeftMargin(0.12);
		pad_lo->SetTopMargin(1.);
		pad_lo->SetBottomMargin(0.3);
		
		pad_lo->cd();
   		TF1* ratioFit = new TF1("ratioFit", "[0]", hData->GetXaxis()->GetXmin(), hData->GetXaxis()->GetXmax());
    		ratioFit->SetParameter(0, 0.);
    		ratioFit->SetLineColor(46);
    		ratioFit->SetLineWidth(2);
		TH1* h = (TH1F*)hData->Clone();
		h->Divide(hMc);
		h1_style_lo(h);
		h->SetStats(1);
		h->SetTitle("Data/MC");
		h->SetXTitle(Xtitle.c_str());
		h->SetYTitle(Ytitle.c_str());
		h->SetMarkerSize(0.5);
		
		//for(int i=1;i<h->GetEntries();i++){
			//h->SetBinError(i,sqrt(pow(hData->GetBinError(i)/hMc->GetBinContent(i),2)+pow(hMc->GetBinError(i)*hData->GetBinContent(i)/(pow(hMc->GetBinContent(i),2)),2)));
		//}
		h->Fit(ratioFit, "RQ");
		h->GetYaxis()->SetRangeUser(-1,3);
		double fitValue = ratioFit->GetParameter(0);
    		double fitError = ratioFit->GetParError(0);
		
    		TPaveText* fitlabel = new TPaveText(0.55, 0.77, 0.88, 0.83, "brNDC");
    		fitlabel->SetTextSize(0.08);
    		fitlabel->SetFillColor(0);
		fitlabel->SetTextFont(42);
    		TString fitLabelText = TString::Format("Fit: %.3f #pm %.3f", fitValue, fitError);
    		fitlabel->AddText(fitLabelText);
    		fitlabel->Draw("same");
		
		//gStyle->SetOptFit(111111);
		h->Draw("PE1same");
    		gPad->RedrawAxis();
		
    		pad_hi->cd();
	}
	h1_style_hi(hMc);
	h1_style_hi(hData);


	gStyle->SetOptStat(0);
	hMc->Draw("hist");
	hMc->GetXaxis()->SetTitle(Xtitle.c_str());
	hData->Draw("pe1same");
	hData->SetMarkerColor(getDataColor());
	hData->SetLineColor(1);
	hData->SetMarkerStyle(20);
	//hData->SetMarkerSize(0.8);
	hMc->SetFillColor(kBlack);
	hMc->SetFillStyle(3001);
	hMc->DrawCopy("e2same");
	hMc->SetFillStyle(1001);
	hMc->SetFillColor(getMcColor());
	hMc->SetLineColor(1);
	if(!inLinScale) {
		hData->SetMinimum(0.01);
		gPad-> SetLogy();
	}
  	TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  	legend->SetFillColor(kWhite);
  	legend->SetFillStyle(0);
  	legend->SetTextSize(0.038);
	legend->SetTextFont(42);
	legend->AddEntry(hMc,"MC","f");
	legend->AddEntry(hData,"Data 2012","p");
	legend->Draw("same");
		
  	Float_t cmsTextSize = 0.043;
  	TPaveText* label_cms = get_labelCMS(1);
  	label_cms->SetTextSize(cmsTextSize);
	label_cms->Draw("same");
	gPad->RedrawAxis();

	cCanvas->SaveAs(path.c_str());
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
	gDataMcResponseratio->SetMarkerSize(0.5);
	//gDataMcResponseratio->SetMaximum(1.08);
	//gDataMcResponseratio->SetMinimum(0.90);
	gDataMcResponseratio->GetXaxis()->SetTitle(XTitle.c_str());
	
	return gDataMcResponseratio;
}

void drawComparisonResponse(const string& canvasName, TMultiGraph *mgResponse, TGraphErrors *gResponseMC, TGraphErrors *gResponseData, TGraph *gratio,const string& mcSample, const string& path, bool doFit = false) {
	TCanvas *cCanvas = new TCanvas(canvasName.c_str(),canvasName.c_str(), 600, 800);
	cCanvas->cd();
  	// Data / MC comparison
  	TPad* pad_hi = new TPad("pad_hi", "", 0., 0.33, 0.99, 0.99);
  	pad_hi->Draw();
  	//pad_hi->SetLogx();
  	pad_hi->SetLeftMargin(0.15);
  	pad_hi->SetBottomMargin(0.015);

  	// Data / MC ratio
	TPad* pad_lo = new TPad("pad_lo", "", 0., 0., 0.99, 0.33);
	pad_lo->Draw();
	//pad_lo->SetLogx();
	pad_lo->SetLeftMargin(0.15);
	pad_lo->SetTopMargin(1.);
	pad_lo->SetBottomMargin(0.3);
	
		
    	pad_hi->cd();

	gStyle->SetOptStat(0);
	mgResponse->SetMaximum(1.05);
	mgResponse->SetMinimum(0.9);
	mgResponse->Draw("AP");
	mgResponse->GetXaxis()->SetLabelSize(0);
	mgResponse->GetYaxis()->SetTitleOffset(1.3);
	cCanvas->SetLogx(1);
	
	if(doFit) {
   	  TF1* myLinFit = new TF1("myLinFit", "[0]*x + [1]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
      	  myLinFit->SetParameter(0.5, 1.);
    	  myLinFit->SetLineColor(1);
    	  myLinFit->SetLineWidth(2);

	
	  cCanvas->Update();
	  gResponseData->Fit(myLinFit, "RQ");
	  double linfitValue = myLinFit->GetParameter(0);
    	  double linfitError = myLinFit->GetParError(0);
		
    	  TPaveText* linfitlabel = new TPaveText(0.55, 0.77, 0.88, 0.83, "brNDC");
    	  linfitlabel->SetTextSize(0.035);
    	  linfitlabel->SetFillColor(0);
	  linfitlabel->SetTextFont(42);
    	  TString linfitLabelText = TString::Format("Data slope: %.5f #pm %.5f", linfitValue, linfitError);
    	  linfitlabel->AddText(linfitLabelText);
    	  linfitlabel->Draw("same");

    	  gPad->RedrawAxis();	
	}
	
  	TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  	legend->SetFillColor(kWhite);
  	legend->SetFillStyle(0);
	legend->SetTextFont(42);
  	legend->SetTextSize(0.038);
	legend->AddEntry(gResponseMC,"MC","p");
	legend->AddEntry(gResponseData,"Data 2012","p");
	legend->Draw("same");
		
  	Float_t cmsTextSize = 0.043;
  	TPaveText* label_cms = get_labelCMS(1);
  	label_cms->SetTextSize(cmsTextSize);
	label_cms->Draw("same");
	gPad->RedrawAxis();
		
	pad_lo->cd();
   	TF1* ratioFit = new TF1("ratioFit", "[0]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
    	ratioFit->SetParameter(0, 0.);
    	ratioFit->SetLineColor(46);
    	ratioFit->SetLineWidth(2);
	gratio->Draw("APE1");
	gratio->GetYaxis()->SetTitle("Data/MC");
// 	//gratio->GetXaxis()->SetLabelOffset(0.1);
// 	gratio->GetXaxis()->SetLabelFont(42);
// 	gratio->GetXaxis()->SetLabelSize(0.06);
// 	//gratio->GetYaxis()->SetLabelOffset(0.1);
// 	gratio->GetYaxis()->SetLabelFont(42);
// 	gratio->GetYaxis()->SetLabelSize(0.06);
// 	gratio->GetXaxis()->SetTitleOffset(-0.6);
// 	gratio->GetXaxis()->SetTitleFont(42);
// 	gratio->GetXaxis()->SetTitleSize(0.06);
 	gratio->GetYaxis()->SetTitleOffset(1.3);
// 	gratio->GetYaxis()->SetTitleFont(42);
// 	gratio->GetYaxis()->SetTitleSize(0.06);
	
	gratio->GetXaxis()->SetLimits(mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
	
	cCanvas->Update();
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

	
	cCanvas->SaveAs(path.c_str());
}

int main (int argc, char** argv) 
{
	TString inname_data;
	TString inname_mc;
	bool isLumiNorm = false;
	bool isShapeNorm = false;
	bool inLinScale = false;
	bool rmPU = false;
	
	string extension = ".pdf";	
	
	//gStyle->SetOptFit(111111);
	
	ptBinning myPtBinning;
	npvBinning myNpvBinning;
	etaBinning myEtaBinning;
	
	Double_t xlow = getHistoXlow();
	Double_t xup = getHistoXup();
	//Double_t    binrange = 0.1;
	//Int_t    nbinsx = (xup - xlow)/binrange;
	Int_t    nbinsx = getHistoNbinsx();
	
	int numberPtBins = myPtBinning.getSize();
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
    TCLAP::CmdLine cmd("Draw data/MC comparison plots", ' ', "1.0");
    
    // Define a value argument and add it to the command line.
    // A value arg defines a flag and a type of value that it expects,
    // such as "-n Bishop".
    TCLAP::UnlabeledValueArg<std::string> dataArg("data", "data input file name", true, "", "string");
    cmd.add( dataArg );
    TCLAP::UnlabeledValueArg<std::string> mcArg("mc", "mc input file name", true, "", "string");
    cmd.add( mcArg );
    
    TCLAP::ValueArg<std::string> plotNameArg("", "plotName", "plotName", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> extensionArg("", "extension", "extension", false, ".pdf", "string", cmd);
    
    // Define a switch and add it to the command line.
    // A switch arg is a boolean argument and only defines a flag that
    // indicates true or false.  In this example the SwitchArg adds itself
    // to the CmdLine object as part of the constructor.  This eliminates
    // the need to call the cmd.add() method.  All args have support in
    // their constructors to add themselves directly to the CmdLine object.
    // It doesn't matter which idiom you choose, they accomplish the same thing.

    TCLAP::SwitchArg lumiArg("", "lumi", "Do you want lumi normalization?", false);
    cmd.add( lumiArg );
    TCLAP::SwitchArg shapeArg("", "shape", "Do you want shape normalization??", false);
    cmd.add( shapeArg );
    TCLAP::SwitchArg linArg("", "lin", "Do you want linear scale?", false);
    cmd.add( linArg );
    TCLAP::SwitchArg rmPUArg("", "rmPU", "Do you want to remove PU jets?", false);
    cmd.add(rmPUArg);

    
    // Parse the argv array.
    cmd.parse(argc, argv);
    
    // Get the value parsed by each arg.
    inname_data = dataArg.getValue();
    inname_mc = mcArg.getValue();
    isLumiNorm = lumiArg.getValue();
    isShapeNorm = shapeArg.getValue();
    inLinScale = linArg.getValue();
    extension = plotNameArg.getValue() + extensionArg.getValue();
    rmPU = rmPUArg.getValue();
    
    if(rmPU) {
	extension = "_woPUJets" + plotNameArg.getValue() + extensionArg.getValue();    
    }
  
    
  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    return 1;
  }
  
	TFile *f_data=TFile::Open(inname_data);
	TFile *f_mc=TFile::Open(inname_mc);
	
	string myHistoName;
	
	applyStyle();

//************************************************************************************************************
//
//                                      MJB per ptrecoil bin and per Npv bin
//
//************************************************************************************************************	
	
	string vectorName;
	string ptBinName;
	string npvBinName;
	string etaBinName;
	
//vectors for multijet study
	
	//MJB per Npv
	vector<TH1F*> vMJB_Npv_data_lumi;
	vMJB_Npv_data_lumi.resize(numberNpvBins);
	vector<TH1F*> vMJB_Npv_mc_lumi;
	vMJB_Npv_mc_lumi.resize(numberNpvBins);
	vector<TH1F*> vMJB_Npv_data_shape;
	vMJB_Npv_data_shape.resize(numberNpvBins);
	vector<TH1F*> vMJB_Npv_mc_shape;
	vMJB_Npv_mc_shape.resize(numberNpvBins);

	//MJB per recoilpt
	vector<TH1F*> vMJB_RecoilPt_data_lumi;
	vMJB_RecoilPt_data_lumi.resize(numberPtBins);
	vector<TH1F*> vMJB_RecoilPt_mc_lumi;
	vMJB_RecoilPt_mc_lumi.resize(numberPtBins);
	vector<TH1F*> vMJB_RecoilPt_data_shape;
	vMJB_RecoilPt_data_shape.resize(numberPtBins);
	vector<TH1F*> vMJB_RecoilPt_mc_shape;
	vMJB_RecoilPt_mc_shape.resize(numberPtBins);

  //LeandingJetPt per recoilpt
	vector<TH1F*> vLeadingJetPt_RecoilPt_data_lumi;
	vLeadingJetPt_RecoilPt_data_lumi.resize(numberPtBins);
	vector<TH1F*> vLeadingJetPt_RecoilPt_mc_lumi;
	vLeadingJetPt_RecoilPt_mc_lumi.resize(numberPtBins);
	vector<TH1F*> vLeadingJetPt_RecoilPt_data_shape;
	vLeadingJetPt_RecoilPt_data_shape.resize(numberPtBins);
	vector<TH1F*> vLeadingJetPt_RecoilPt_mc_shape;
	vLeadingJetPt_RecoilPt_mc_shape.resize(numberPtBins);
	
	//MJB per recoileta
	vector<TH1F*> vMJB_RecoilEta_data_lumi;
	vMJB_RecoilEta_data_lumi.resize(numberEtaBins);
	vector<TH1F*> vMJB_RecoilEta_mc_lumi;
	vMJB_RecoilEta_mc_lumi.resize(numberEtaBins);
	vector<TH1F*> vMJB_RecoilEta_data_shape;
	vMJB_RecoilEta_data_shape.resize(numberEtaBins);
	vector<TH1F*> vMJB_RecoilEta_mc_shape;
	vMJB_RecoilEta_mc_shape.resize(numberEtaBins);

	
	for(int j=0; j<myPtBinning.getSize(); j++) {
		ptBinName = myPtBinning.getName(j);
		vectorName = "MJB/recoilPtBin/MJB_" + ptBinName;
		vMJB_RecoilPt_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
		vMJB_RecoilPt_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
    vectorName = "leadingJet/recoilPtBin/LeadingJetPt_" + ptBinName;
		vLeadingJetPt_RecoilPt_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
		vLeadingJetPt_RecoilPt_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
	}
	
	for(int j=0; j<myNpvBinning.getSize(); j++) {
		npvBinName = myNpvBinning.getName(j);
		vectorName = "MJB/npvBin/MJB_" + npvBinName;
		vMJB_Npv_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
		vMJB_Npv_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());		
	}
	
	for(int j=0; j<myEtaBinning.getSize(); j++) {
		etaBinName = myEtaBinning.getName(j);
		vectorName = "MJB/recoilEtaBin/MJB_" + etaBinName;
		vMJB_RecoilEta_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
		vMJB_RecoilEta_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
	}
	
//************************************************************************************************************
//
//                                      MJB as a function of ptrecoil
//
//************************************************************************************************************

	TGraphErrors* gMJB_RecoilPt_data=(TGraphErrors*)f_data->Get("MJB/recoilPtBin/gMJB_RecoilPt");
	TGraphErrors* gMJB_RecoilPt_mc=(TGraphErrors*)f_mc->Get("MJB/recoilPtBin/gMJB_RecoilPt");
	
	TGraph_data_style (gMJB_RecoilPt_data);
	TGraph_mc_style (gMJB_RecoilPt_mc);
	
	TMultiGraph *mgMJB_RecoilPt = new TMultiGraph();
	mgMJB_RecoilPt->Add(gMJB_RecoilPt_mc,"pe");
	mgMJB_RecoilPt->Add(gMJB_RecoilPt_data,"pe");
	mgMJB_RecoilPt->SetTitle("MJB as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];MJB");
	
	TGraphErrors *gMJB_RecoilPt_ratio = getDataMcResponseRatio(gMJB_RecoilPt_data,gMJB_RecoilPt_mc,numberPtBins, "p_{t}^{Recoil} [GeV/c]");
	gMJB_RecoilPt_ratio->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
	gMJB_RecoilPt_ratio->GetXaxis()->SetTitle("p_{t}^{Recoil} [GeV/c]");
	gMJB_RecoilPt_ratio->SetName("Data/MC");
	gMJB_RecoilPt_ratio->SetTitle("Data/MC");
	gMJB_RecoilPt_ratio->SetMarkerSize(1.0);
	gMJB_RecoilPt_ratio->Fit("func","","",gMJB_RecoilPt_data->GetXaxis()->GetXmin(),gMJB_RecoilPt_data->GetXaxis()->GetXmax());
	
	myHistoName = "images/response/MJB_RecoilPt" + extension;	
	drawComparisonResponse("r1", mgMJB_RecoilPt, gMJB_RecoilPt_mc, gMJB_RecoilPt_data, gMJB_RecoilPt_ratio,"MC", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      MJB as a function of etarecoil
//
//************************************************************************************************************

	TGraphErrors* gMJB_RecoilEta_data=(TGraphErrors*)f_data->Get("MJB/recoilEtaBin/gMJB_RecoilEta");
	TGraphErrors* gMJB_RecoilEta_mc=(TGraphErrors*)f_mc->Get("MJB/recoilEtaBin/gMJB_RecoilEta");
	
	TGraph_data_style (gMJB_RecoilEta_data);
	TGraph_mc_style (gMJB_RecoilEta_mc);
	
	TMultiGraph *mgMJB_RecoilEta = new TMultiGraph();
	mgMJB_RecoilEta->Add(gMJB_RecoilEta_mc,"pe");
	mgMJB_RecoilEta->Add(gMJB_RecoilEta_data,"pe");
	mgMJB_RecoilEta->SetTitle("MJB as a function of |#eta^{Recoil}|;|#eta^{Recoil}|;MJB");
	
	TGraphErrors *gMJB_RecoilEta_ratio = getDataMcResponseRatio(gMJB_RecoilEta_data,gMJB_RecoilEta_mc,numberEtaBins, "p_{t}^{Recoil} [GeV/c]");
	gMJB_RecoilEta_ratio->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
	gMJB_RecoilEta_ratio->GetXaxis()->SetTitle("|#eta^{Recoil}|");
	gMJB_RecoilEta_ratio->SetName("Data/MC");
	gMJB_RecoilEta_ratio->SetTitle("Data/MC");
	gMJB_RecoilEta_ratio->SetMarkerSize(1.0);
	gMJB_RecoilEta_ratio->Fit("func","","",gMJB_RecoilEta_data->GetXaxis()->GetXmin(),gMJB_RecoilEta_data->GetXaxis()->GetXmax());
	
	myHistoName = "images/response/MJB_RecoilEta" + extension;	
	drawComparisonResponse("r1", mgMJB_RecoilEta, gMJB_RecoilEta_mc, gMJB_RecoilEta_data, gMJB_RecoilEta_ratio,"MC", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      MJB as a function of Npv
//
//************************************************************************************************************

	TGraphErrors* gMJB_Npv_data=(TGraphErrors*)f_data->Get("MJB/npvBin/gMJB_Npv");
	TGraphErrors* gMJB_Npv_mc=(TGraphErrors*)f_mc->Get("MJB/npvBin/gMJB_Npv");
	
	TGraph_data_style (gMJB_Npv_data);
	TGraph_mc_style (gMJB_Npv_mc);
	
	TMultiGraph *mgMJB_Npv = new TMultiGraph();
	mgMJB_Npv->Add(gMJB_Npv_mc,"pe");
	mgMJB_Npv->Add(gMJB_Npv_data,"pe");
	mgMJB_Npv->SetTitle("MJB as a function of N_{PV};N_{PV};MJB");
	
	TGraphErrors *gMJB_Npv_ratio = getDataMcResponseRatio(gMJB_Npv_data,gMJB_Npv_mc,numberNpvBins,"N_{PV}");
	gMJB_Npv_ratio->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
	gMJB_Npv_ratio->GetXaxis()->SetTitle("N_{PV}");
	gMJB_Npv_ratio->SetNameTitle("Data/MC_Npv","Data/MC");
	gMJB_Npv_ratio->SetMarkerSize(1.0);
	gMJB_Npv_ratio->Fit("func","","",gMJB_Npv_data->GetXaxis()->GetXmin(),gMJB_Npv_data->GetXaxis()->GetXmax());
	
	myHistoName = "images/response/MJB_Npv" + extension;	
	drawComparisonResponse("r2", mgMJB_Npv, gMJB_Npv_mc, gMJB_Npv_data, gMJB_Npv_ratio,"MC", myHistoName.c_str(), true);

//************************************************************************************************************
//
//                                      get the histograms
//
//************************************************************************************************************

	TH1F* hMJB_inclusive_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hMJB_inclusive");
	TH1F* hMJB_inclusive_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hMJB_inclusive");	
	
	TH1F* hMet_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hMet_beforeSel");
	TH1F* hMet_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hMet_beforeSel");
	
	TH1F* hMet_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hMet_afterSel");
	TH1F* hMet_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hMet_afterSel");
	
	TH1F* hLeadingJetPt_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hLeadingJetPt_beforeSel");
	TH1F* hLeadingJetPt_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hLeadingJetPt_beforeSel");
	
	TH1F* hLeadingJetPt_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hLeadingJetPt_afterSel");
	TH1F* hLeadingJetPt_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hLeadingJetPt_afterSel");
	
	TH1F* hRecoilPt_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hRecoilPt_beforeSel");
	TH1F* hRecoilPt_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hRecoilPt_beforeSel");
	
	TH1F* hRecoilPt_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hRecoilPt_afterSel");
	TH1F* hRecoilPt_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hRecoilPt_afterSel");
	
	TH1F* hNpv_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hNpv_beforeSel");
	TH1F* hNpv_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hNpv_beforeSel");
	
	TH1F* hNpv_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hNpv_afterSel");
	TH1F* hNpv_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hNpv_afterSel");
	
	TH1F* hAlpha_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hAlpha_beforeSel");
	TH1F* hAlpha_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hAlpha_beforeSel");
	
	TH1F* hAlpha_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hAlpha_afterSel");
	TH1F* hAlpha_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hAlpha_afterSel");
	
	TH1F* hBeta_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hBeta_beforeSel");
	TH1F* hBeta_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hBeta_beforeSel");
	
	TH1F* hBeta_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hBeta_afterSel");
	TH1F* hBeta_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hBeta_afterSel");
	
	TH1F* hA_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hA_beforeSel");
	TH1F* hA_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hA_beforeSel");
	
	TH1F* hA_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hA_afterSel");
	TH1F* hA_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hA_afterSel");
	
	TH1F* hFracRmPuJets_JetPt_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hFracRmPuJets_JetPt");
	TH1F* hFracRmPuJets_JetPt_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hFracRmPuJets_JetPt");
	
	TH1F* hRecoilEta_mc_lumi=(TH1F*)f_mc->Get("recoil/hRecoilEta");
	TH1F* hRecoilEta_data_lumi=(TH1F*)f_data->Get("recoil//hRecoilEta");
	
	TH1F* hRecoilWidth_mc_lumi=(TH1F*)f_mc->Get("recoil/hRecoilWidth");
	TH1F* hRecoilWidth_data_lumi=(TH1F*)f_data->Get("recoil//hRecoilWidth");

	string binName;
	string myName;
	string myXName;
	string mySaveName;

//************************************************************************************************************
//
//                                      Lumi
//
//************************************************************************************************************

  if(isLumiNorm) {
  
//************************************************************************************************************
//
//                                      MJB per ptrecoil 
//
//************************************************************************************************************	

	for(int j=0; j<myPtBinning.getSize(); j++) {
		h1_style(vMJB_RecoilPt_data_lumi[j]);
		h1_style(vMJB_RecoilPt_mc_lumi[j]);
		vMJB_RecoilPt_mc_lumi[j]->Scale(getLumi());
		binName = myPtBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperPtRecoil/MJB_" + binName + "_lumi_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilPt_mc_lumi[j], vMJB_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}

//************************************************************************************************************
//
//                                      LeadingJetPt per ptrecoil 
//
//************************************************************************************************************	

	for(int j=0; j<myPtBinning.getSize(); j++) {
		h1_style(vLeadingJetPt_RecoilPt_data_lumi[j]);
		h1_style(vLeadingJetPt_RecoilPt_mc_lumi[j]);
		vLeadingJetPt_RecoilPt_mc_lumi[j]->Scale(getLumi());
		binName = myPtBinning.getName(j);
		myName = "LeadingJetPt_{data}/LeadingJetPt_{MC} for " + binName;
		myXName = "LeadingJetPt for " + binName + " [GeV/c]";
		mySaveName = "images/LeadingJetPtperPtRecoil/LeadingJetPt_" + binName + "_lumi_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_lumi[j], vLeadingJetPt_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}
	
//************************************************************************************************************
//
//                                      MJB per etarecoil 
//
//************************************************************************************************************	

	for(int j=0; j<myEtaBinning.getSize(); j++) {
		h1_style(vMJB_RecoilEta_data_lumi[j]);
		h1_style(vMJB_RecoilEta_mc_lumi[j]);
		vMJB_RecoilEta_mc_lumi[j]->Scale(getLumi());
		binName = myEtaBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperEtaRecoil/MJB_" + binName + "_lumi_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilEta_mc_lumi[j], vMJB_RecoilEta_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}
	
//************************************************************************************************************
//
//                                      MJB per Npv
//
//************************************************************************************************************	

	for(int j=0; j<myNpvBinning.getSize(); j++) {
		h1_style(vMJB_Npv_data_lumi[j]);
		h1_style(vMJB_Npv_mc_lumi[j]);
		vMJB_Npv_mc_lumi[j]->Scale(getLumi());
		binName = myNpvBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperNpv/MJB_" + binName + "_lumi_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_Npv_mc_lumi[j], vMJB_Npv_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}

//************************************************************************************************************
//
//                                      MJB inclusive 
//
//************************************************************************************************************	
			

	h1_style(hMJB_inclusive_mc_lumi);
	h1_style(hMJB_inclusive_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hMJB_inclusive_mc_lumi->Scale(getLumi());
	
	myHistoName = "images/variables/MJB_inclusive_lumi_inLogScale" + extension;	
	drawDataMcComparison("MJB_inclusive", hMJB_inclusive_mc_lumi, hMJB_inclusive_data_lumi, "MJB", myHistoName.c_str());
		
	
	
//************************************************************************************************************
//
//                                      MET beforeSel 
//
//************************************************************************************************************	
			
	
	h1_style(hMet_beforeSel_mc_lumi);
	h1_style(hMet_beforeSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hMet_beforeSel_mc_lumi->Scale(getLumi());

	myHistoName = "images/variables/Met_beforeSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_lumi, hMet_beforeSel_data_lumi, "MET [GeV/c]", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      MET afterSel 
//
//************************************************************************************************************	
			
	
	h1_style(hMet_afterSel_mc_lumi);
	h1_style(hMet_afterSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hMet_afterSel_mc_lumi->Scale(getLumi());

	myHistoName = "images/variables/Met_afterSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_lumi, hMet_afterSel_data_lumi, "MET [GeV/c]", myHistoName.c_str());
		
	
	
//************************************************************************************************************
//
//                                      LeadingJetPt beforeSel 
//
//************************************************************************************************************	
			
	
	h1_style(hLeadingJetPt_beforeSel_mc_lumi);
	h1_style(hLeadingJetPt_beforeSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hLeadingJetPt_beforeSel_mc_lumi->Scale(getLumi());
	
	myHistoName = "images/variables/LeadingJetPt_beforeSel_lumi_inLogScale" + extension;
	drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_lumi, hLeadingJetPt_beforeSel_data_lumi, "p_{t}^{leading jet} [GeV/c]", myHistoName.c_str());
		
	

	
//************************************************************************************************************
//
//                                      LeadingJetPt afterSel 
//
//************************************************************************************************************	
			
	
	h1_style(hLeadingJetPt_afterSel_mc_lumi);
	h1_style(hLeadingJetPt_afterSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hLeadingJetPt_afterSel_mc_lumi->Scale(getLumi());
	myHistoName = "images/variables/LeadingJetPt_afterSel_lumi_inLogScale" + extension;
	drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_lumi, hLeadingJetPt_afterSel_data_lumi,"p_{t}^{leading jet} [GeV/c]", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      RecoilPt beforeSel 
//
//************************************************************************************************************	
			
	
	h1_style(hRecoilPt_beforeSel_mc_lumi);
	h1_style(hRecoilPt_beforeSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hRecoilPt_beforeSel_mc_lumi->Scale(getLumi());
	
	myHistoName = "images/variables/RecoilPt_beforeSel_lumi_inLogScale" + extension;
	drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_lumi, hRecoilPt_beforeSel_data_lumi, "p_{t}^{Recoil} [GeV/c]", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      RecoilPt afterSel 
//
//************************************************************************************************************	
			
	
	h1_style(hRecoilPt_afterSel_mc_lumi);
	h1_style(hRecoilPt_afterSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hRecoilPt_afterSel_mc_lumi->Scale(getLumi());
	myHistoName = "images/variables/RecoilPt_afterSel_lumi_inLogScale" + extension;
	drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_lumi, hRecoilPt_afterSel_data_lumi,"p_{t}^{Recoil} [GeV/c]", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      Npv beforeSel 
//
//************************************************************************************************************	
			
	
	h1_style(hNpv_beforeSel_mc_lumi);
	h1_style(hNpv_beforeSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hNpv_beforeSel_mc_lumi->Scale(getLumi());
	
	myHistoName = "images/variables/Npv_beforeSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Npv_beforeSel", hNpv_beforeSel_mc_lumi, hNpv_beforeSel_data_lumi, "N_{PV}", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      Npv afterSel 
//
//************************************************************************************************************	
			
	
	h1_style(hNpv_afterSel_mc_lumi);
	h1_style(hNpv_afterSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hNpv_afterSel_mc_lumi->Scale(getLumi());
	myHistoName = "images/variables/Npv_afterSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Npv_afterSel", hNpv_afterSel_mc_lumi, hNpv_afterSel_data_lumi, "N_{PV}", myHistoName.c_str());
		

//************************************************************************************************************
//
//                                      FracRmPuJets_JetPt
//
//************************************************************************************************************	
			
	
	h1_style(hFracRmPuJets_JetPt_mc_lumi);
	h1_style(hFracRmPuJets_JetPt_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hFracRmPuJets_JetPt_mc_lumi->Scale(getLumi());
	myHistoName = "images/variables/FracRmPuJets_JetPt_lumi_inLogScale" + extension;
	drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_lumi, hFracRmPuJets_JetPt_data_lumi, "p_{t}^{jet} [GeV/c]", myHistoName.c_str(), false, "N_{removed PU jets}/N_{total jets}");	

	
//************************************************************************************************************
//
//                                      Alpha beforeSel 
//
//************************************************************************************************************	
			
	
	h1_style(hAlpha_beforeSel_mc_lumi);
	h1_style(hAlpha_beforeSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hAlpha_beforeSel_mc_lumi->Scale(getLumi());
	
	myHistoName = "images/variables/Alpha_beforeSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Alpha_beforeSel", hAlpha_beforeSel_mc_lumi, hAlpha_beforeSel_data_lumi, "#alpha", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      Alpha afterSel 
//
//************************************************************************************************************	
			
	
	h1_style(hAlpha_afterSel_mc_lumi);
	h1_style(hAlpha_afterSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hAlpha_afterSel_mc_lumi->Scale(getLumi());

	myHistoName = "images/variables/Alpha_afterSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Alpha_afterSel", hAlpha_afterSel_mc_lumi, hAlpha_afterSel_data_lumi, "#alpha", myHistoName.c_str());
		

//************************************************************************************************************
//
//                                      etaRecoil
//
//************************************************************************************************************	
			
	
	h1_style(hRecoilEta_mc_lumi);
	h1_style(hRecoilEta_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hRecoilEta_mc_lumi->Scale(getLumi());

	myHistoName = "images/recoil/RecoilEta_lumi_inLogScale" + extension;
	drawDataMcComparison("RecoilEta", hRecoilEta_mc_lumi, hRecoilEta_data_lumi, "|#eta^{Recoil}|", myHistoName.c_str());	
	
	
//************************************************************************************************************
//
//                                      deltaEtaRecoil
//
//************************************************************************************************************	
			
	
	h1_style(hRecoilWidth_mc_lumi);
	h1_style(hRecoilWidth_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hRecoilWidth_mc_lumi->Scale(getLumi());

	myHistoName = "images/recoil/RecoilDeltaEta_lumi_inLogScale" + extension;
	drawDataMcComparison("RecoilDeltaEta", hRecoilWidth_mc_lumi, hRecoilWidth_data_lumi, "Recoil width (#Delta #eta = |#eta^{Recoil jet}_{max} - #eta^{Recoil jet}_{min}|)", myHistoName.c_str());	


//************************************************************************************************************
//
//                                      Beta beforeSel 
//
//************************************************************************************************************	
			
	
	h1_style(hBeta_beforeSel_mc_lumi);
	h1_style(hBeta_beforeSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hBeta_beforeSel_mc_lumi->Scale(getLumi());
	
	myHistoName = "images/variables/Beta_beforeSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Beta_beforeSel", hBeta_beforeSel_mc_lumi, hBeta_beforeSel_data_lumi, "#beta", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      Beta afterSel 
//
//************************************************************************************************************	
			
	
	h1_style(hBeta_afterSel_mc_lumi);
	h1_style(hBeta_afterSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hBeta_afterSel_mc_lumi->Scale(getLumi());
	
	myHistoName = "images/variables/Beta_afterSel_lumi_inLogScale" + extension;
	drawDataMcComparison("Beta_afterSel", hBeta_afterSel_mc_lumi, hBeta_afterSel_data_lumi, "#beta", myHistoName.c_str());
		
	

//************************************************************************************************************
//
//                                      A beforeSel 
//
//************************************************************************************************************	
			
	
	h1_style(hA_beforeSel_mc_lumi);
	h1_style(hA_beforeSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hA_beforeSel_mc_lumi->Scale(getLumi());

	myHistoName = "images/variables/A_beforeSel_lumi_inLogScale" + extension;
	drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_lumi, hA_beforeSel_data_lumi, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str());
		

//************************************************************************************************************
//
//                                      A afterSel 
//
//************************************************************************************************************	
	
	
	h1_style(hA_afterSel_mc_lumi);
	h1_style(hA_afterSel_data_lumi);
		
	//rescale the Monte Carlo histogramm with luminosity
	hA_afterSel_mc_lumi->Scale(getLumi());

	myHistoName = "images/variables/A_afterSel_lumi_inLogScale" + extension;
	drawDataMcComparison("A_afterSel", hA_afterSel_mc_lumi, hA_afterSel_data_lumi, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str());
		

//*******************************************************************************************************

    if(inLinScale) {
      myHistoName = "images/variables/MJB_inclusive_lumi_inLinScale" + extension;
      drawDataMcComparison("MJB_inclusive", hMJB_inclusive_mc_lumi, hMJB_inclusive_data_lumi, "MJB", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_lumi, hMet_beforeSel_data_lumi, "MET [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_lumi, hMet_afterSel_data_lumi, "MET [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_lumi, hLeadingJetPt_beforeSel_data_lumi, "p_{t}^{leading jet} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_lumi, hLeadingJetPt_afterSel_data_lumi, "p_{t}^{leading jet} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/RecoilPt_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_lumi, hRecoilPt_beforeSel_data_lumi, "p_{t}^{Recoil} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/RecoilPt_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_lumi, hRecoilPt_afterSel_data_lumi, "p_{t}^{Recoil} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Npv_beforeSel", hNpv_beforeSel_mc_lumi, hNpv_beforeSel_data_lumi,  "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Npv_afterSel", hNpv_afterSel_mc_lumi, hNpv_afterSel_data_lumi,  "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/FracRmPuJets_JetPt_lumi_inLinScale" + extension;
      drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_lumi, hFracRmPuJets_JetPt_data_lumi,  "p_{t}^{jet} [GeV/c]", myHistoName.c_str(), inLinScale, "N_{removed PU jets}/N_{total jets}");
      myHistoName = "images/variables/Alpha_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Alpha_beforeSel", hAlpha_beforeSel_mc_lumi, hAlpha_beforeSel_data_lumi,"#alpha", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Alpha_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Alpha_afterSel", hAlpha_afterSel_mc_lumi, hAlpha_afterSel_data_lumi, "#alpha", myHistoName.c_str(), inLinScale);
      myHistoName = "images/recoil/RecoilEta_lumi_inLinScale" + extension;
      drawDataMcComparison("RecoilEta", hRecoilEta_mc_lumi, hRecoilEta_data_lumi, "|#eta^{Recoil}|", myHistoName.c_str(), inLinScale);
      myHistoName = "images/recoil/RecoilDeltaEta_lumi_inLinScale" + extension;
      drawDataMcComparison("RecoilDeltaEta", hRecoilWidth_mc_lumi, hRecoilWidth_data_lumi, "Recoil width (#Delta #eta = |#eta^{Recoil jet}_{max} - #eta^{Recoil jet}_{min}|)", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Beta_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Beta_beforeSel", hBeta_beforeSel_mc_lumi, hBeta_beforeSel_data_lumi, "#beta", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Beta_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Beta_afterSel", hBeta_afterSel_mc_lumi, hBeta_afterSel_data_lumi, "#beta", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/A_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_lumi, hA_beforeSel_data_lumi, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/A_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("A_afterSel", hA_afterSel_mc_lumi, hA_afterSel_data_lumi, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str(), inLinScale);
      for(int j=0; j<myPtBinning.getSize(); j++) {
		binName = myPtBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperPtRecoil/MJB_" + binName + "_lumi_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilPt_mc_lumi[j], vMJB_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
    
    myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
		myXName = "pT^{Leading Jet} for " + binName + " [GeV/c]";
		mySaveName = "images/LeadingJetPtperPtRecoil/LeadingJetPt_" + binName + "_lumi_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_lumi[j], vLeadingJetPt_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }
      for(int j=0; j<myEtaBinning.getSize(); j++) {
		binName = myEtaBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperEtaRecoil/MJB_" + binName + "_lumi_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilEta_mc_lumi[j], vMJB_RecoilEta_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }
      for(int j=0; j<myNpvBinning.getSize(); j++) {
		binName = myNpvBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperNpv/MJB_" + binName + "_lumi_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_Npv_mc_lumi[j], vMJB_Npv_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }
    }

  }


//************************************************************************************************************
//
//                                      Shape
//
//************************************************************************************************************

  if(isShapeNorm) {
  float Nentries_MJB_RecoilPt_mc;
  float Nentries_MJB_RecoilPt_Data;
  float Nentries_LeadingJetPt_RecoilPt_mc;
  float Nentries_LeadingJetPt_RecoilPt_Data;
  float Nentries_MJB_RecoilEta_mc;
  float Nentries_MJB_RecoilEta_Data;
  float Nentries_MJB_Npv_mc;
  float Nentries_MJB_Npv_Data;
  
//************************************************************************************************************
//
//                                      MJB per ptrecoil 
//
//************************************************************************************************************	

	for(int j=0; j<myPtBinning.getSize(); j++) {
		vMJB_RecoilPt_mc_shape[j]=(TH1F*)vMJB_RecoilPt_mc_lumi[j]->Clone();
		vMJB_RecoilPt_data_shape[j]=(TH1F*)vMJB_RecoilPt_data_lumi[j]->Clone();
		h1_style(vMJB_RecoilPt_data_shape[j]);
		h1_style(vMJB_RecoilPt_mc_shape[j]);

		Nentries_MJB_RecoilPt_mc = vMJB_RecoilPt_mc_shape[j]->Integral();
		Nentries_MJB_RecoilPt_Data = vMJB_RecoilPt_data_shape[j]->Integral();
		vMJB_RecoilPt_mc_shape[j]->Scale(Nentries_MJB_RecoilPt_Data/Nentries_MJB_RecoilPt_mc);

		binName = myPtBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperPtRecoil/MJB_" + binName + "_shape_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilPt_mc_lumi[j], vMJB_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}

//************************************************************************************************************
//
//                                      LeadingJetPt per ptrecoil 
//
//************************************************************************************************************	

	for(int j=0; j<myPtBinning.getSize(); j++) {
		vLeadingJetPt_RecoilPt_mc_shape[j]=(TH1F*)vLeadingJetPt_RecoilPt_mc_lumi[j]->Clone();
		vLeadingJetPt_RecoilPt_data_shape[j]=(TH1F*)vLeadingJetPt_RecoilPt_data_lumi[j]->Clone();
		h1_style(vLeadingJetPt_RecoilPt_data_shape[j]);
		h1_style(vLeadingJetPt_RecoilPt_mc_shape[j]);

		Nentries_LeadingJetPt_RecoilPt_mc = vLeadingJetPt_RecoilPt_mc_shape[j]->Integral();
		Nentries_LeadingJetPt_RecoilPt_Data = vLeadingJetPt_RecoilPt_data_shape[j]->Integral();
		vLeadingJetPt_RecoilPt_mc_shape[j]->Scale(Nentries_LeadingJetPt_RecoilPt_Data/Nentries_LeadingJetPt_RecoilPt_mc);

		binName = myPtBinning.getName(j);
		myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
		myXName = "pT^{Leading Jet} for " + binName + " [GeV/c]";
		mySaveName = "images/LeadingJetPtperPtRecoil/LeadingJetPt_" + binName + "_shape_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_lumi[j], vLeadingJetPt_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}
	
//************************************************************************************************************
//
//                                      MJB per etarecoil 
//
//************************************************************************************************************	

	for(int j=0; j<myEtaBinning.getSize(); j++) {
		vMJB_RecoilEta_mc_shape[j]=(TH1F*)vMJB_RecoilEta_mc_lumi[j]->Clone();
		vMJB_RecoilEta_data_shape[j]=(TH1F*)vMJB_RecoilEta_data_lumi[j]->Clone();
		h1_style(vMJB_RecoilEta_data_shape[j]);
		h1_style(vMJB_RecoilEta_mc_shape[j]);

		Nentries_MJB_RecoilEta_mc = vMJB_RecoilEta_mc_shape[j]->Integral();
		Nentries_MJB_RecoilEta_Data = vMJB_RecoilEta_data_shape[j]->Integral();
		vMJB_RecoilEta_mc_shape[j]->Scale(Nentries_MJB_RecoilEta_Data/Nentries_MJB_RecoilEta_mc);

		binName = myEtaBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperEtaRecoil/MJB_" + binName + "_shape_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilEta_mc_lumi[j], vMJB_RecoilEta_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}
	
//************************************************************************************************************
//
//                                      MJB per Npv
//
//************************************************************************************************************	

	for(int j=0; j<myNpvBinning.getSize(); j++) {
		vMJB_Npv_mc_shape[j]=(TH1F*)vMJB_Npv_mc_lumi[j]->Clone();
		vMJB_Npv_data_shape[j]=(TH1F*)vMJB_Npv_data_lumi[j]->Clone();
		h1_style(vMJB_Npv_data_shape[j]);
		h1_style(vMJB_Npv_mc_shape[j]);

		Nentries_MJB_Npv_mc = vMJB_Npv_mc_shape[j]->Integral();
		Nentries_MJB_Npv_Data = vMJB_Npv_data_shape[j]->Integral();
		vMJB_Npv_mc_shape[j]->Scale(Nentries_MJB_Npv_Data/Nentries_MJB_Npv_mc);

		binName = myNpvBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperNpv/MJB_" + binName + "_shape_inLogScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_Npv_mc_lumi[j], vMJB_Npv_data_lumi[j], myXName.c_str(), mySaveName.c_str());
	}
	
//************************************************************************************************************
//
//                                      MJB inclusive 
//
//************************************************************************************************************	
	TH1F* hMJB_inclusive_mc_shape=(TH1F*)hMJB_inclusive_mc_lumi->Clone();
	TH1F* hMJB_inclusive_data_shape=(TH1F*)hMJB_inclusive_data_lumi->Clone();

	h1_style(hMJB_inclusive_mc_shape);
	h1_style(hMJB_inclusive_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_MJB_inclusive_mc = hMJB_inclusive_mc_shape->Integral();
	float Nentries_MJB_inclusive_Data = hMJB_inclusive_data_shape->Integral();	
	hMJB_inclusive_mc_shape->Scale(Nentries_MJB_inclusive_Data/Nentries_MJB_inclusive_mc);

	myHistoName = "images/variables/MJB_inclusive_shape_inLogScale" + extension;
	drawDataMcComparison("MJB_inclusive", hMJB_inclusive_mc_shape, hMJB_inclusive_data_shape, "MJB", myHistoName.c_str());
	
	
//************************************************************************************************************
//
//                                      MET beforeSel 
//
//************************************************************************************************************	
	
	TH1F* hMet_beforeSel_mc_shape=(TH1F*)hMet_beforeSel_mc_lumi->Clone();
	TH1F* hMet_beforeSel_data_shape=(TH1F*)hMet_beforeSel_data_lumi->Clone();

	h1_style(hMet_beforeSel_mc_shape);
	h1_style(hMet_beforeSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Met_beforeSel_mc = hMet_beforeSel_mc_shape->Integral();
	float Nentries_Met_beforeSel_Data = hMet_beforeSel_data_shape->Integral();	
	hMet_beforeSel_mc_shape->Scale(Nentries_Met_beforeSel_Data/Nentries_Met_beforeSel_mc);

	myHistoName = "images/variables/Met_beforeSel_shape_inLogScale" + extension;
	drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_shape, hMet_beforeSel_data_shape, "MET [GeV/c]", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      MET afterSel 
//
//************************************************************************************************************	
	
	TH1F* hMet_afterSel_mc_shape=(TH1F*)hMet_afterSel_mc_lumi->Clone();
	TH1F* hMet_afterSel_data_shape=(TH1F*)hMet_afterSel_data_lumi->Clone();

	h1_style(hMet_afterSel_mc_shape);
	h1_style(hMet_afterSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Met_afterSel_mc = hMet_afterSel_mc_shape->Integral();
	float Nentries_Met_afterSel_Data = hMet_afterSel_data_shape->Integral();	
	hMet_afterSel_mc_shape->Scale(Nentries_Met_afterSel_Data/Nentries_Met_afterSel_mc);

	myHistoName = "images/variables/Met_afterSel_shape_inLogScale" + extension;
	drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_shape, hMet_afterSel_data_shape, "MET [GeV/c]", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      LeadingJetPt beforeSel 
//
//************************************************************************************************************	
	
	TH1F* hLeadingJetPt_beforeSel_mc_shape=(TH1F*)hLeadingJetPt_beforeSel_mc_lumi->Clone();
	TH1F* hLeadingJetPt_beforeSel_data_shape=(TH1F*)hLeadingJetPt_beforeSel_data_lumi->Clone();

	h1_style(hLeadingJetPt_beforeSel_mc_shape);
	h1_style(hLeadingJetPt_beforeSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_LeadingJetPt_beforeSel_mc = hLeadingJetPt_beforeSel_mc_shape->Integral();
	float Nentries_LeadingJetPt_beforeSel_Data = hLeadingJetPt_beforeSel_data_shape->Integral();	
	hLeadingJetPt_beforeSel_mc_shape->Scale(Nentries_LeadingJetPt_beforeSel_Data/Nentries_LeadingJetPt_beforeSel_mc);
	
	myHistoName = "images/variables/LeadingJetPt_beforeSel_shape_inLogScale" + extension;
	drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_shape, hLeadingJetPt_beforeSel_data_shape, "p_{t}^{leading jet} [GeV/c]", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      LeadingJetPt afterSel 
//
//************************************************************************************************************	
	
	TH1F* hLeadingJetPt_afterSel_mc_shape=(TH1F*)hLeadingJetPt_afterSel_mc_lumi->Clone();
	TH1F* hLeadingJetPt_afterSel_data_shape=(TH1F*)hLeadingJetPt_afterSel_data_lumi->Clone();

	h1_style(hLeadingJetPt_afterSel_mc_shape);
	h1_style(hLeadingJetPt_afterSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_LeadingJetPt_afterSel_mc = hLeadingJetPt_afterSel_mc_shape->Integral();
	float Nentries_LeadingJetPt_afterSel_Data = hLeadingJetPt_afterSel_data_shape->Integral();	
	hLeadingJetPt_afterSel_mc_shape->Scale(Nentries_LeadingJetPt_afterSel_Data/Nentries_LeadingJetPt_afterSel_mc);
	
	myHistoName = "images/variables/LeadingJetPt_afterSel_shape_inLogScale" + extension;
	drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_shape, hLeadingJetPt_afterSel_data_shape, "p_{t}^{leading jet} [GeV/c]", myHistoName.c_str());


//************************************************************************************************************
//
//                                      RecoilPt beforeSel 
//
//************************************************************************************************************	
	
	TH1F* hRecoilPt_beforeSel_mc_shape=(TH1F*)hRecoilPt_beforeSel_mc_lumi->Clone();
	TH1F* hRecoilPt_beforeSel_data_shape=(TH1F*)hRecoilPt_beforeSel_data_lumi->Clone();

	h1_style(hRecoilPt_beforeSel_mc_shape);
	h1_style(hRecoilPt_beforeSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_RecoilPt_beforeSel_mc = hRecoilPt_beforeSel_mc_shape->Integral();
	float Nentries_RecoilPt_beforeSel_Data = hRecoilPt_beforeSel_data_shape->Integral();	
	hRecoilPt_beforeSel_mc_shape->Scale(Nentries_RecoilPt_beforeSel_Data/Nentries_RecoilPt_beforeSel_mc);
	
	myHistoName = "images/variables/RecoilPt_beforeSel_shape_inLogScale" + extension;
	drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_shape, hRecoilPt_beforeSel_data_shape, "p_{t}^{Recoil} [GeV/c]", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      RecoilPt afterSel 
//
//************************************************************************************************************	
	
	TH1F* hRecoilPt_afterSel_mc_shape=(TH1F*)hRecoilPt_afterSel_mc_lumi->Clone();
	TH1F* hRecoilPt_afterSel_data_shape=(TH1F*)hRecoilPt_afterSel_data_lumi->Clone();

	h1_style(hRecoilPt_afterSel_mc_shape);
	h1_style(hRecoilPt_afterSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_RecoilPt_afterSel_mc = hRecoilPt_afterSel_mc_shape->Integral();
	float Nentries_RecoilPt_afterSel_Data = hRecoilPt_afterSel_data_shape->Integral();	
	hRecoilPt_afterSel_mc_shape->Scale(Nentries_RecoilPt_afterSel_Data/Nentries_RecoilPt_afterSel_mc);
	
	myHistoName = "images/variables/RecoilPt_afterSel_shape_inLogScale" + extension;
	drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_shape, hRecoilPt_afterSel_data_shape, "p_{t}^{Recoil} [GeV/c]", myHistoName.c_str());

//************************************************************************************************************
//
//                                      Npv beforeSel 
//
//************************************************************************************************************	
	
	TH1F* hNpv_beforeSel_mc_shape=(TH1F*)hNpv_beforeSel_mc_lumi->Clone();
	TH1F* hNpv_beforeSel_data_shape=(TH1F*)hNpv_beforeSel_data_lumi->Clone();

	h1_style(hNpv_beforeSel_mc_shape);
	h1_style(hNpv_beforeSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Npv_beforeSel_mc = hNpv_beforeSel_mc_shape->Integral();
	float Nentries_Npv_beforeSel_Data = hNpv_beforeSel_data_shape->Integral();	
	hNpv_beforeSel_mc_shape->Scale(Nentries_Npv_beforeSel_Data/Nentries_Npv_beforeSel_mc);
	
	myHistoName = "images/variables/Npv_beforeSel_shape_inLogScale" + extension;
	drawDataMcComparison("Npv_beforeSel", hNpv_beforeSel_mc_shape, hNpv_beforeSel_data_shape, "N_{PV}", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      Npv afterSel 
//
//************************************************************************************************************	
	
	TH1F* hNpv_afterSel_mc_shape=(TH1F*)hNpv_afterSel_mc_lumi->Clone();
	TH1F* hNpv_afterSel_data_shape=(TH1F*)hNpv_afterSel_data_lumi->Clone();

	h1_style(hNpv_afterSel_mc_shape);
	h1_style(hNpv_afterSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Npv_afterSel_mc = hNpv_afterSel_mc_shape->Integral();
	float Nentries_Npv_afterSel_Data = hNpv_afterSel_data_shape->Integral();	
	hNpv_afterSel_mc_shape->Scale(Nentries_Npv_afterSel_Data/Nentries_Npv_afterSel_mc);

	myHistoName = "images/variables/Npv_afterSel_shape_inLogScale" + extension;
	drawDataMcComparison("Npv_afterSel", hNpv_afterSel_mc_shape, hNpv_afterSel_data_shape, "N_{PV}", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      FracRmPuJets_JetPt
//
//************************************************************************************************************	
	
	TH1F* hFracRmPuJets_JetPt_mc_shape=(TH1F*)hFracRmPuJets_JetPt_mc_lumi->Clone();
	TH1F* hFracRmPuJets_JetPt_data_shape=(TH1F*)hFracRmPuJets_JetPt_data_lumi->Clone();

	h1_style(hFracRmPuJets_JetPt_mc_shape);
	h1_style(hFracRmPuJets_JetPt_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_FracRmPuJets_JetPt_mc = hFracRmPuJets_JetPt_mc_shape->Integral();
	float Nentries_FracRmPuJets_JetPt_Data = hFracRmPuJets_JetPt_data_shape->Integral();	
	hFracRmPuJets_JetPt_mc_shape->Scale(Nentries_FracRmPuJets_JetPt_Data/Nentries_FracRmPuJets_JetPt_mc);

	myHistoName = "images/variables/FracRmPuJets_JetPt_shape_inLogScale" + extension;
	drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_shape, hFracRmPuJets_JetPt_data_shape, "p_{t}^{jet} [GeV/c]", myHistoName.c_str(), false, "N_{removed PU jets}/N_{total jets}");
	
	
//************************************************************************************************************
//
//                                      Alpha beforeSel 
//
//************************************************************************************************************	
	TH1F* hAlpha_beforeSel_mc_shape=(TH1F*)hAlpha_beforeSel_mc_lumi->Clone();
	TH1F* hAlpha_beforeSel_data_shape=(TH1F*)hAlpha_beforeSel_data_lumi->Clone();

	h1_style(hAlpha_beforeSel_mc_shape);
	h1_style(hAlpha_beforeSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Alpha_beforeSel_mc = hAlpha_beforeSel_mc_shape->Integral();
	float Nentries_Alpha_beforeSel_Data = hAlpha_beforeSel_data_shape->Integral();	
	hAlpha_beforeSel_mc_shape->Scale(Nentries_Alpha_beforeSel_Data/Nentries_Alpha_beforeSel_mc);

	myHistoName = "images/variables/Alpha_beforeSel_shape_inLogScale" + extension;
	drawDataMcComparison("Alpha_beforeSel", hAlpha_beforeSel_mc_shape, hAlpha_beforeSel_data_shape, "#alpha", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      Alpha afterSel 
//
//************************************************************************************************************	
	
	TH1F* hAlpha_afterSel_mc_shape=(TH1F*)hAlpha_afterSel_mc_lumi->Clone();
	TH1F* hAlpha_afterSel_data_shape=(TH1F*)hAlpha_afterSel_data_lumi->Clone();

	h1_style(hAlpha_afterSel_mc_shape);
	h1_style(hAlpha_afterSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Alpha_afterSel_mc = hAlpha_afterSel_mc_shape->Integral();
	float Nentries_Alpha_afterSel_Data = hAlpha_afterSel_data_shape->Integral();	
	hAlpha_afterSel_mc_shape->Scale(Nentries_Alpha_afterSel_Data/Nentries_Alpha_afterSel_mc);

	myHistoName = "images/variables/Alpha_afterSel_shape_inLogScale" + extension;
	drawDataMcComparison("Alpha_afterSel", hAlpha_afterSel_mc_shape, hAlpha_afterSel_data_shape,"#alpha", myHistoName.c_str());

//************************************************************************************************************
//
//                                      recoilEta
//
//************************************************************************************************************	
	
	TH1F* hRecoilEta_mc_shape=(TH1F*)hRecoilEta_mc_lumi->Clone();
	TH1F* hRecoilEta_data_shape=(TH1F*)hRecoilEta_data_lumi->Clone();

	h1_style(hRecoilEta_mc_shape);
	h1_style(hRecoilEta_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_RecoilEta_mc = hRecoilEta_mc_shape->Integral();
	float Nentries_RecoilEta_Data = hRecoilEta_data_shape->Integral();	
	hRecoilEta_mc_shape->Scale(Nentries_RecoilEta_Data/Nentries_RecoilEta_mc);

	myHistoName = "images/recoil/RecoilEta_shape_inLogScale" + extension;
	drawDataMcComparison("RecoilEta", hRecoilEta_mc_shape, hRecoilEta_data_shape,"|#eta^{Recoil}|", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      recoilDeltaEta
//
//************************************************************************************************************	
	
	TH1F* hRecoilWidth_mc_shape=(TH1F*)hRecoilWidth_mc_lumi->Clone();
	TH1F* hRecoilWidth_data_shape=(TH1F*)hRecoilWidth_data_lumi->Clone();

	h1_style(hRecoilWidth_mc_shape);
	h1_style(hRecoilWidth_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_RecoilDeltaEta_mc = hRecoilWidth_mc_shape->Integral();
	float Nentries_RecoilDeltaEta_Data = hRecoilWidth_data_shape->Integral();	
	hRecoilWidth_mc_shape->Scale(Nentries_RecoilEta_Data/Nentries_RecoilEta_mc);

	myHistoName = "images/recoil/RecoilDeltaEta_shape_inLogScale" + extension;
	drawDataMcComparison("RecoilDeltaEta", hRecoilWidth_mc_shape, hRecoilWidth_data_shape,"Recoil width (#Delta #eta = |#eta^{Recoil jet}_{max} - #eta^{Recoil jet}_{min}|)", myHistoName.c_str());

//************************************************************************************************************
//
//                                      Beta beforeSel 
//
//************************************************************************************************************	
	
	TH1F* hBeta_beforeSel_mc_shape=(TH1F*)hBeta_beforeSel_mc_lumi->Clone();
	TH1F* hBeta_beforeSel_data_shape=(TH1F*)hBeta_beforeSel_data_lumi->Clone();

	h1_style(hBeta_beforeSel_mc_shape);
	h1_style(hBeta_beforeSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Beta_beforeSel_mc = hBeta_beforeSel_mc_shape->Integral();
	float Nentries_Beta_beforeSel_Data = hBeta_beforeSel_data_shape->Integral();	
	hBeta_beforeSel_mc_shape->Scale(Nentries_Beta_beforeSel_Data/Nentries_Beta_beforeSel_mc);

	myHistoName = "images/variables/Beta_beforeSel_shape_inLogScale" + extension;
	drawDataMcComparison("Beta_beforeSel", hBeta_beforeSel_mc_shape, hBeta_beforeSel_data_shape, "#beta", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      Beta afterSel 
//
//************************************************************************************************************	
	
	TH1F* hBeta_afterSel_mc_shape=(TH1F*)hBeta_afterSel_mc_lumi->Clone();
	TH1F* hBeta_afterSel_data_shape=(TH1F*)hBeta_afterSel_data_lumi->Clone();

	h1_style(hBeta_afterSel_mc_shape);
	h1_style(hBeta_afterSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_Beta_afterSel_mc = hBeta_afterSel_mc_shape->Integral();
	float Nentries_Beta_afterSel_Data = hBeta_afterSel_data_shape->Integral();	
	hBeta_afterSel_mc_shape->Scale(Nentries_Beta_afterSel_Data/Nentries_Beta_afterSel_mc);

	myHistoName = "images/variables/Beta_afterSel_shape_inLogScale" + extension;
	drawDataMcComparison("Beta_afterSel", hBeta_afterSel_mc_shape, hBeta_afterSel_data_shape,"#beta", myHistoName.c_str());


//************************************************************************************************************
//
//                                      A beforeSel 
//
//************************************************************************************************************	
			
	TH1F* hA_beforeSel_mc_shape=(TH1F*)hA_beforeSel_mc_lumi->Clone();
	TH1F* hA_beforeSel_data_shape=(TH1F*)hA_beforeSel_data_lumi->Clone();

	h1_style(hA_beforeSel_mc_shape);
	h1_style(hA_beforeSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_A_beforeSel_mc = hA_beforeSel_mc_shape->Integral();
	float Nentries_A_beforeSel_Data = hA_beforeSel_data_shape->Integral();	
	hA_beforeSel_mc_shape->Scale(Nentries_A_beforeSel_Data/Nentries_A_beforeSel_mc);

	myHistoName = "images/variables/A_beforeSel_shape_inLogScale" + extension;
	drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_shape, hA_beforeSel_data_shape, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str());
	
//************************************************************************************************************
//
//                                      A afterSel 
//
//************************************************************************************************************	
	
	TH1F* hA_afterSel_mc_shape=(TH1F*)hA_afterSel_mc_lumi->Clone();
	TH1F* hA_afterSel_data_shape=(TH1F*)hA_afterSel_data_lumi->Clone();

	h1_style(hA_afterSel_mc_shape);
	h1_style(hA_afterSel_data_shape);
		
	//rescale the Monte Carlo histogramm with number of entries
	float Nentries_A_afterSel_mc = hA_afterSel_mc_shape->Integral();
	float Nentries_A_afterSel_Data = hA_afterSel_data_shape->Integral();	
	hA_afterSel_mc_shape->Scale(Nentries_A_afterSel_Data/Nentries_A_afterSel_mc);

	myHistoName = "images/variables/A_afterSel_shape_inLogScale" + extension;
	drawDataMcComparison("A_afterSel", hA_afterSel_mc_shape, hA_afterSel_data_shape, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str());

//*******************************************************************************************************

    if(inLinScale) {
      myHistoName = "images/variables/MJB_inclusive_shape_inLinScale" + extension;
      drawDataMcComparison("MJB_inclusive", hMJB_inclusive_mc_shape, hMJB_inclusive_data_shape, "MJB", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_shape, hMet_beforeSel_data_shape, "MET [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_shape, hMet_afterSel_data_shape, "MET [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_shape, hLeadingJetPt_beforeSel_data_shape, "p_{t}^{leading jet} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_shape, hLeadingJetPt_afterSel_data_shape, "p_{t}^{leading jet} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/RecoilPt_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_shape, hRecoilPt_beforeSel_data_shape, "p_{t}^{Recoil} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/RecoilPt_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_shape, hRecoilPt_afterSel_data_shape, "p_{t}^{Recoil} [GeV/c]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("Npv_beforeSel", hNpv_beforeSel_mc_shape, hNpv_beforeSel_data_shape, "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("Npv_afterSel", hNpv_afterSel_mc_shape, hNpv_afterSel_data_shape, "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/FracRmPuJets_JetPt_shape_inLinScale" + extension;
      drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_shape, hFracRmPuJets_JetPt_data_shape, "p_{t}^{jet} [GeV/c]", myHistoName.c_str(), inLinScale, "N_{removed PU jets}/N_{total jets}");
      myHistoName = "images/variables/Alpha_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("Alpha_beforeSel", hAlpha_beforeSel_mc_shape, hAlpha_beforeSel_data_shape, "#alpha", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Alpha_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("Alpha_afterSel", hAlpha_afterSel_mc_shape, hAlpha_afterSel_data_shape,"#alpha", myHistoName.c_str(), inLinScale);
      myHistoName = "images/recoil/RecoilEta_shape_inLinScale" + extension;
      drawDataMcComparison("RecoilEta", hRecoilEta_mc_shape, hRecoilEta_data_shape,"|#eta^{Recoil}|", myHistoName.c_str(), inLinScale);
      myHistoName = "images/recoil/RecoilDeltaEta_shape_inLinScale" + extension;
      drawDataMcComparison("RecoilDeltaEta", hRecoilWidth_mc_shape, hRecoilWidth_data_shape,"Recoil width (#Delta #eta = |#eta^{Recoil jet}_{max} - #eta^{Recoil jet}_{min}|)", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Beta_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("Beta_beforeSel", hBeta_beforeSel_mc_shape, hBeta_beforeSel_data_shape, "#beta", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Beta_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("Beta_afterSel", hBeta_afterSel_mc_shape, hBeta_afterSel_data_shape, "#beta", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/A_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_shape, hA_beforeSel_data_shape, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/A_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("A_afterSel", hA_afterSel_mc_shape, hA_afterSel_data_shape, "A = p_{t}^{jet 2}/p_{t}^{Recoil}", myHistoName.c_str(), inLinScale);
      for(int j=0; j<myPtBinning.getSize(); j++) {
		binName = myPtBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperPtRecoil/MJB_" + binName + "_shape_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilPt_mc_shape[j], vMJB_RecoilPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);

    myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
		myXName = "pT^{Leading Jet} for " + binName + " [GeV/c]";
		mySaveName = "images/LeadingJetPtperPtRecoil/LeadingJetPtperPt_" + binName + "_shape_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_shape[j], vLeadingJetPt_RecoilPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }
      for(int j=0; j<myEtaBinning.getSize(); j++) {
		binName = myEtaBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperEtaRecoil/MJB_" + binName + "_shape_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_RecoilEta_mc_shape[j], vMJB_RecoilEta_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }
      for(int j=0; j<myNpvBinning.getSize(); j++) {
		binName = myNpvBinning.getName(j);
		myName = "MJB_{data}/MJB_{MC} for " + binName;
		myXName = "MJB for " + binName;
		mySaveName = "images/MJBperNpv/MJB_" + binName + "_shape_inLinScale" + extension;
		drawDataMcComparison(myName.c_str(), vMJB_Npv_mc_shape[j], vMJB_Npv_data_shape[j],myXName.c_str(), mySaveName.c_str(), inLinScale);
      }    
    }  
  
  }	
	
	return 0;

}
