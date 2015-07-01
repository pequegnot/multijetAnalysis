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
#include "../common/HLTPtBinning.h"
#include "../common/logPtPrimeBinning.h"
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
  //style_->SetLegendBorderSize(1);
  style_->SetLegendBorderSize(0);
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
    if(Ytitle != "") {
      h->SetYTitle("Data/MC");
    }
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
	hMc->GetYaxis()->SetTitle(Ytitle.c_str());
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
    //TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
    TLegend* legend = new TLegend(0.55, 0.65, 0.92, 0.88);
  	legend->SetFillColor(kWhite);
  	legend->SetFillStyle(0);
  	legend->SetTextSize(0.038);
    legend->SetLineWidth(0);
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

void drawComparisonResponse(const string& canvasName, TMultiGraph *mgResponse, TGraphErrors *gResponseMC, TGraphErrors *gResponseData, TGraph *gratio,const string& mcSample, const string& path, bool doFit = false, bool setRangeUser = true) {
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
    if (setRangeUser) {
    mgResponse->SetMaximum(0.5);
    mgResponse->SetMinimum(0.);
    }
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
    bool useSyst = false;
    string systFileName = "";
    bool useRecoilPtBin = true;
    bool useRecoilPtHLTBin = false;
	
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
    TCLAP::CmdLine cmd("Draw data/MC comparison plots", ' ', "1.0");
    
    // Define a value argument and add it to the command line.
    // A value arg defines a flag and a type of value that it expects,
    // such as "-n Bishop".
    TCLAP::UnlabeledValueArg<std::string> dataArg("data", "data input file name", true, "", "string");
    cmd.add( dataArg );
    TCLAP::UnlabeledValueArg<std::string> mcArg("mc", "mc input file name", true, "", "string");
    cmd.add( mcArg );
    
    TCLAP::ValueArg<std::string> plotNameArg("", "plotName", "plotName", false, "", "string", cmd);
    //TCLAP::ValueArg<std::string> extensionArg("", "extension", "extension", false, ".pdf", "string", cmd);
    
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
    TCLAP::SwitchArg useSystArg("", "useSyst", "Do you want to use systematics?", false);
    cmd.add(useSystArg);
    TCLAP::ValueArg<std::string> systFileArg("", "syst-file", "The file containing the systematics", false, "", "string", cmd);

    TCLAP::SwitchArg recoilPtBinArg("", "recoilPtBin", "Do you want to bin in recoil pt?", false);
    TCLAP::SwitchArg firstJetPtBinArg("", "firstJetPtBin", "Do you want to bin in 1st jet pt?", false);
    cmd.xorAdd(recoilPtBinArg, firstJetPtBinArg);

    TCLAP::SwitchArg recoilPtHLTBinArg("", "recoilPtHLTBin", "Do you want to bin in recoil pt for HLT trigger?", false);
    TCLAP::SwitchArg firstJetPtHLTBinArg("", "firstJetPtHLTBin", "Do you want to bin in 1st jet pt for HLT trigger?", false);
    cmd.xorAdd(recoilPtHLTBinArg, firstJetPtHLTBinArg);
    
    // Parse the argv array.
    cmd.parse(argc, argv);
    
    // Get the value parsed by each arg.
    inname_data = dataArg.getValue();
    inname_mc = mcArg.getValue();
    isLumiNorm = lumiArg.getValue();
    isShapeNorm = shapeArg.getValue();
    inLinScale = linArg.getValue();
    //extension = plotNameArg.getValue() + extensionArg.getValue();
    extension = plotNameArg.getValue();
    rmPU = rmPUArg.getValue();
    useSyst = useSystArg.getValue();
    systFileName = systFileArg.getValue();    
    useRecoilPtBin = recoilPtBinArg.getValue();
    useRecoilPtHLTBin = recoilPtHLTBinArg.getValue();
    
    if(rmPU) {
	//extension = "_woPUJets" + plotNameArg.getValue() + extensionArg.getValue();    
	extension = "_woPUJets" + plotNameArg.getValue();    
    }
  
    
  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    return 1;
  }


    myHLTPtBinning.fillHLTPtBins(useRecoilPtHLTBin);
  
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
	
//************************************************************************************************************
//
//                                      MJB as a function of reference object pt 
//
//************************************************************************************************************

	TGraphErrors* gMJB_RefObjPt_data=(TGraphErrors*)f_data->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");
	TGraphErrors* gMJB_RefObjPt_mc=(TGraphErrors*)f_mc->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");

  if (useSyst) {
    if (systFileName == "") {
      std::cout << "ERROR! You must specify the file to use for systematics! *********************" << std::endl;
    }
    else {
      TFile *f_syst=TFile::Open(systFileName.c_str());
      TGraphErrors* gMJB_SystTot=(TGraphErrors*)f_syst->Get("MJB/PtBin/gMJB_SystTot");
      for(int i=0; i<numberPtBins; i++) {
        // quadratic sum of statitics and systematics errors
        gMJB_RefObjPt_mc->SetPointError(i, 0., sqrt(gMJB_RefObjPt_mc->GetErrorY(i)*gMJB_RefObjPt_mc->GetErrorY(i) + gMJB_SystTot->GetErrorY(i)*gMJB_SystTot->GetErrorY(i)));
	    }
    }
  }

  TGraphErrors* gMJB_RefObjPt_mc_clone = (TGraphErrors*)gMJB_RefObjPt_mc->Clone();

	TGraph_data_style (gMJB_RefObjPt_data);
	TGraph_mc_style (gMJB_RefObjPt_mc);
	TGraph_mc_style (gMJB_RefObjPt_mc_clone);

	gMJB_RefObjPt_mc_clone->SetFillColor(17);
	//gMJB_RefObjPt_mc_clone->SetFillStyle(3002);
  gMJB_RefObjPt_mc->SetLineColor(17);
	
	TMultiGraph *mgMJB_RefObjPt = new TMultiGraph();
	mgMJB_RefObjPt->Add(gMJB_RefObjPt_mc_clone,"e3");
	mgMJB_RefObjPt->Add(gMJB_RefObjPt_mc,"p");
	mgMJB_RefObjPt->Add(gMJB_RefObjPt_data,"pe");
  if(useRecoilPtBin) {
	  mgMJB_RefObjPt->SetTitle("exp( #sum_{i}F_{i}log(f_{i}) ) as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];exp( #sum_{i}F_{i}log(f_{i}) )");
  }
  else {
	  mgMJB_RefObjPt->SetTitle("exp( #sum_{i}F_{i}log(f_{i}) ) as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];exp( #sum_{i}F_{i}log(f_{i}) )");
  }
	
	TGraphErrors *gMJB_RefObjPt_ratio = NULL;
  if(useRecoilPtBin) {
    gMJB_RefObjPt_ratio = getDataMcResponseRatio(gMJB_RefObjPt_data,gMJB_RefObjPt_mc,numberPtBins, "p_{t}^{Recoil} [GeV/c]");
	  gMJB_RefObjPt_ratio->GetXaxis()->SetTitle("p_{t}^{Recoil} [GeV/c]");
  }
  else {
    gMJB_RefObjPt_ratio = getDataMcResponseRatio(gMJB_RefObjPt_data,gMJB_RefObjPt_mc,numberPtBins, "p_{t}^{Leading Jet} [GeV/c]");
	  gMJB_RefObjPt_ratio->GetXaxis()->SetTitle("p_{t}^{Leading Jet} [GeV/c]");  
  }
	gMJB_RefObjPt_ratio->GetYaxis()->SetTitle("Data/MC");
	gMJB_RefObjPt_ratio->SetName("Data/MC");
	gMJB_RefObjPt_ratio->SetTitle("Data/MC");
	gMJB_RefObjPt_ratio->SetMarkerSize(1.0);
	gMJB_RefObjPt_ratio->Fit("func","","",gMJB_RefObjPt_data->GetXaxis()->GetXmin(),gMJB_RefObjPt_data->GetXaxis()->GetXmax());
	
	myHistoName = "images/Exp_sum_Fi_log_fi_RecoilPt" + extension + ".pdf";	
	drawComparisonResponse("r1", mgMJB_RefObjPt, gMJB_RefObjPt_mc, gMJB_RefObjPt_data, gMJB_RefObjPt_ratio,"MC", myHistoName.c_str());
	
	myHistoName = "images/Exp_sum_Fi_log_fi_RecoilPt" + extension + ".root";	
	drawComparisonResponse("r1", mgMJB_RefObjPt, gMJB_RefObjPt_mc, gMJB_RefObjPt_data, gMJB_RefObjPt_ratio,"MC", myHistoName.c_str());
	
	myHistoName = "images/Exp_sum_Fi_log_fi_RecoilPt" + extension + ".C";	
	drawComparisonResponse("r1", mgMJB_RefObjPt, gMJB_RefObjPt_mc, gMJB_RefObjPt_data, gMJB_RefObjPt_ratio,"MC", myHistoName.c_str());


//************************************************************************************************************
//
//                                      MJB as a function of ptrecoil without last ptbin (response equal to zero)
//
//************************************************************************************************************

	Double_t aMJB_RefObjPt_data_Mean[numberPtBins-1];
	Double_t aMJB_RefObjPt_data_MeanError[numberPtBins-1];
	Double_t aMJB_RefObjPt_mc_Mean[numberPtBins-1];
	Double_t aMJB_RefObjPt_mc_MeanError[numberPtBins-1];
	Double_t aRefObjPtBins_Mean[numberPtBins-1];
	Double_t aRefObjPtBins_MeanError[numberPtBins-1];
	Double_t aMJBRatio_Mean[numberPtBins-1];
	Double_t aMJBRatio_MeanError[numberPtBins-1];
	
	for(int i=0; i<numberPtBins-1; i++) {
    gMJB_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i],aMJB_RefObjPt_data_Mean[i]);
    gMJB_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i],aMJB_RefObjPt_mc_Mean[i]);
    gMJB_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i],aMJBRatio_Mean[i]);
    aMJB_RefObjPt_data_MeanError[i] = gMJB_RefObjPt_data->GetErrorY(i);
    aMJB_RefObjPt_mc_MeanError[i] = gMJB_RefObjPt_mc->GetErrorY(i);
    aRefObjPtBins_MeanError[i] = gMJB_RefObjPt_data->GetErrorX(i);
    aMJBRatio_MeanError[i] = gMJB_RefObjPt_ratio->GetErrorY(i);
	}

	TGraphErrors* gMJB_RefObjPt_data_resize = new TGraphErrors(numberPtBins-1,aRefObjPtBins_Mean, aMJB_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_data_MeanError);
	TGraphErrors* gMJB_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-1,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_mc_MeanError);	
	TGraph* gMJB_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-1,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean);	

  TGraphErrors* gMJB_RefObjPt_mc_clone_resize = (TGraphErrors*)gMJB_RefObjPt_mc_resize->Clone();
	
	TGraph_data_style (gMJB_RefObjPt_data_resize);
	TGraph_mc_style (gMJB_RefObjPt_mc_resize);
	TGraph_mc_style (gMJB_RefObjPt_mc_resize_pointsOnly);
	TGraph_mc_style (gMJB_RefObjPt_mc_clone_resize);

	gMJB_RefObjPt_mc_clone_resize->SetFillColor(17);
	//gMJB_RefObjPt_mc_resize->SetFillStyle(3002);
  gMJB_RefObjPt_mc_resize->SetFillColor(17);
	
	TMultiGraph *mgMJB_RefObjPt_resize = new TMultiGraph();
  mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize,"e3");
	mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize_pointsOnly,"p");
	mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_data_resize,"pe");
  if(useRecoilPtBin) {
	  mgMJB_RefObjPt_resize->SetTitle("exp( #sum_{i}F_{i}log(f_{i}) ) as a function of p_{t}^{Recoil};p_{t}^{Recoil} [GeV/c];exp( #sum_{i}F_{i}log(f_{i}) )");
  }
  else {
    mgMJB_RefObjPt_resize->SetTitle("exp( #sum_{i}F_{i}log(f_{i}) ) as a function of p_{t}^{Leading Jet};p_{t}^{Leading Jet} [GeV/c];exp( #sum_{i}F_{i}log(f_{i}) )");
  }
	
	TGraphErrors *gMJB_RefObjPt_ratio_resize = NULL;
  if(useRecoilPtBin) {
    gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-1, "p_{t}^{Recoil} [GeV/c]");
	  gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{t}^{Recoil} [GeV/c]");
  }
  else {
    gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-1, "p_{t}^{Leading Jet} [GeV/c]");
	  gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{t}^{Leading Jet} [GeV/c]");
  }
	gMJB_RefObjPt_ratio_resize->GetYaxis()->SetTitle("Data/MC");
	gMJB_RefObjPt_ratio_resize->SetName("Data/MC");
	gMJB_RefObjPt_ratio_resize->SetTitle("Data/MC");
	gMJB_RefObjPt_ratio_resize->SetMarkerSize(1.0);
	gMJB_RefObjPt_ratio_resize->Fit("func","","",gMJB_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMJB_RefObjPt_data_resize->GetXaxis()->GetXmax());
	
	myHistoName = "images/Exp_sum_Fi_log_fi_RecoilPt_resize" + extension + ".pdf";	
	drawComparisonResponse("r1bis", mgMJB_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize,"MC", myHistoName.c_str());

	myHistoName = "images/Exp_sum_Fi_log_fi_RecoilPt_resize" + extension + ".root";	
	drawComparisonResponse("r2bis", mgMJB_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize,"MC", myHistoName.c_str());

  myHistoName = "images/Exp_sum_Fi_log_fi_RecoilPt_resize" + extension + ".C";	
	drawComparisonResponse("r3bis", mgMJB_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize,"MC", myHistoName.c_str());
}
