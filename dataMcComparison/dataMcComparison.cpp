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
#include <TFitResult.h>
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

#define TITLE_FONTSIZE 26
#define LABEL_FONTSIZE 18
#define LEFT_MARGIN 0.17
#define RIGHT_MARGIN 0.03
#define TOP_MARGIN 0.05
#define BOTTOM_MARGIN 0.13

TStyle* createStyle() {
  TStyle *style = new TStyle("style", "style");

  // For the canvas:
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(kWhite);
  style->SetCanvasDefH(800); //Height of canvas
  style->SetCanvasDefW(800); //Width of canvas
  style->SetCanvasDefX(0);   //POsition on screen
  style->SetCanvasDefY(0);

  // For the Pad:
  style->SetPadBorderMode(0);
  style->SetPadColor(kWhite);
  style->SetPadGridX(false);
  style->SetPadGridY(false);
  style->SetGridColor(0);
  style->SetGridStyle(3);
  style->SetGridWidth(1);

  // For the frame:
  style->SetFrameBorderMode(0);
  style->SetFrameBorderSize(1);
  style->SetFrameFillColor(0);
  style->SetFrameFillStyle(0);
  style->SetFrameLineColor(1);
  style->SetFrameLineStyle(1);
  style->SetFrameLineWidth(1);

  // For the histo:
  style->SetHistLineStyle(1);
  style->SetHistLineWidth(2);
  style->SetEndErrorSize(2);
  style->SetMarkerStyle(20);

  //For the fit/function:
  style->SetFitFormat("5.4g");
  style->SetFuncColor(2);
  style->SetFuncStyle(1);
  style->SetFuncWidth(1);

  // For the statistics box:
  style->SetOptFile(0);
  style->SetStatColor(kWhite);
  //style->SetStatFont(43);
  //style->SetStatFontSize(0.025);
  style->SetStatTextColor(1);
  style->SetStatFormat("6.4g");
  style->SetStatBorderSize(1);
  //style->SetStatH(0.1);
  //style->SetStatW(0.15);

  //For the date:
  style->SetOptDate(0);

  // Margins:
  style->SetPadTopMargin(TOP_MARGIN);
  style->SetPadBottomMargin(BOTTOM_MARGIN);
  style->SetPadLeftMargin(LEFT_MARGIN);
  style->SetPadRightMargin(RIGHT_MARGIN);

  // For the Global title:
  style->SetOptTitle(0);
  style->SetTitleFont(63);
  style->SetTitleColor(1);
  style->SetTitleFillColor(10);
  style->SetTitleBorderSize(0);
  style->SetTitleAlign(33);
  style->SetTitleX(1);
  style->SetTitleFontSize(TITLE_FONTSIZE);

  // For the axis titles:

  style->SetTitleColor(1, "XYZ");
  style->SetTitleFont(43, "XYZ");
  style->SetTitleSize(TITLE_FONTSIZE, "XYZ");
  //style->SetTitleXOffset(3.5); //FIXME
  style->SetTitleYOffset(2.5); //FIXME
  style->SetTitleXOffset(1.5);
  //style->SetTitleYOffset(1.5);

  style->SetLabelColor(1, "XYZ");
  style->SetLabelFont(43, "XYZ");
  style->SetLabelOffset(0.01, "YZ");
  style->SetLabelOffset(0.015, "X");
  style->SetLabelSize(LABEL_FONTSIZE, "XYZ");

  style->SetAxisColor(1, "XYZ");
  style->SetStripDecimals(kTRUE);
  style->SetTickLength(0.03, "XYZ");
  style->SetNdivisions(510, "XYZ");
  style->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  style->SetPadTickY(1);

  style->SetOptLogx(0);
  style->SetOptLogy(0);
  style->SetOptLogz(0);

  style->SetHatchesSpacing(1.3);
  style->SetHatchesLineWidth(1);

  style->cd();

  return style;
}

void cms_style(bool isData = false){
  std::string status = "Simulation preliminary";
  if (isData) status = "Preliminary";
  TPaveText* pt_exp = new TPaveText(LEFT_MARGIN, 1 - 0.5 * TOP_MARGIN, 1 - RIGHT_MARGIN, 1, "brNDC");
  pt_exp->SetFillStyle(0);
  pt_exp->SetBorderSize(0);
  pt_exp->SetMargin(0);
  pt_exp->SetTextFont(62);
  //pt_exp->SetTextSize(0.75 * TOP_MARGIN);
  pt_exp->SetTextSize(1.0 * TOP_MARGIN);
  pt_exp->SetTextAlign(13);
  TString d = TString::Format("CMS #font[52]{#scale[0.76]{%s}}", status.c_str());
  pt_exp->AddText(d);
  pt_exp->Draw();

  TString lumi_s = "19.8 fb^{-1} (8 TeV)";
  TPaveText* pt_lumi = new TPaveText(LEFT_MARGIN, 1 - 0.5 * TOP_MARGIN, 1 - RIGHT_MARGIN, 1, "brNDC");
  pt_lumi->SetFillStyle(0);
  pt_lumi->SetBorderSize(0);
  pt_lumi->SetMargin(0);
  pt_lumi->SetTextFont(42);
  //pt_lumi->SetTextSize(0.6 * TOP_MARGIN);
  pt_lumi->SetTextSize(1.0 * TOP_MARGIN);
  pt_lumi->SetTextAlign(33);
  pt_lumi->AddText(lumi_s);
  pt_lumi->Draw();

}

template<class T>
void setDefaultStyle(T* object, float topBottomScaleFactor) {
  object->GetXaxis()->SetLabelFont(43);
  object->GetYaxis()->SetLabelFont(43);
  object->GetXaxis()->SetTitleFont(43);
  object->GetYaxis()->SetTitleFont(43);
  object->GetXaxis()->SetLabelSize(LABEL_FONTSIZE);
  object->GetYaxis()->SetLabelSize(LABEL_FONTSIZE);
  object->GetXaxis()->SetTitleSize(TITLE_FONTSIZE);
  object->GetYaxis()->SetTitleSize(TITLE_FONTSIZE);
  //object->GetYaxis()->SetTitle("Data / MC");
  object->GetYaxis()->SetNdivisions(510);
  object->GetYaxis()->SetTitleOffset(2.5);
  object->GetYaxis()->SetLabelOffset(0.01);
  object->GetYaxis()->SetTickLength(0.03);
  object->GetXaxis()->SetTitleOffset(1.5 * topBottomScaleFactor);
  object->GetXaxis()->SetLabelOffset(0.012 * topBottomScaleFactor);
  object->GetXaxis()->SetTickLength(0.03);
}
void setDefaultStyle(TObject* object, float topBottomScaleFactor);

void setDefaultStyle(TObject* object, float topBottomScaleFactor) {
  if (dynamic_cast<TH1*>(object))
    setDefaultStyle(dynamic_cast<TH1*>(object), topBottomScaleFactor);
  else if (dynamic_cast<THStack*>(object))
    setDefaultStyle(dynamic_cast<THStack*>(object)->GetHistogram(), topBottomScaleFactor);
  else if (dynamic_cast<TGraph*>(object))
    setDefaultStyle(dynamic_cast<THStack*>(object)->GetHistogram(), topBottomScaleFactor);
}

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


/*void drawDataMcComparison(const string& canvasName, TH1 *hMc, TH1 *hData, const string& Xtitle, const string& path, bool inLinScale = false, const string& Ytitle = "", string legendPos = "r", int withratio = 1){*/
  //TCanvas *cCanvas = new TCanvas(canvasName.c_str(),canvasName.c_str(), 800, 800);
  //cCanvas->cd();
  //if(withratio == 1) {
    //// Data / MC comparison

    //std::shared_ptr<TPad> pad_hi;
    //std::shared_ptr<TPad> pad_lo;
    //pad_hi = std::make_shared<TPad>("pad_hi", "", 0., 0.33333, 1, 1);
    //pad_hi->Draw();
    //pad_hi->SetTopMargin(TOP_MARGIN / .6666);
    //pad_hi->SetLeftMargin(LEFT_MARGIN);
    //pad_hi->SetBottomMargin(0.015);
    //pad_hi->SetRightMargin(RIGHT_MARGIN);
    //pad_lo = std::make_shared<TPad>("pad_lo", "", 0., 0., 1, 0.33333);
    //pad_lo->Draw();
    //pad_lo->SetLeftMargin(LEFT_MARGIN);
    //pad_lo->SetTopMargin(1.);
    //pad_lo->SetBottomMargin(BOTTOM_MARGIN / .3333);
    //pad_lo->SetRightMargin(RIGHT_MARGIN);
    //pad_lo->SetTickx(1);

    //[>  		TPad* pad_hi = new TPad("pad_hi", "", 0., 0.33, 0.99, 0.99);<]
    ////pad_hi->Draw();
    //////pad_hi->SetLogx();
    ////pad_hi->SetLeftMargin(0.12);
    ////pad_hi->SetBottomMargin(0.015);

    ////// Data / MC ratio
    ////TPad* pad_lo = new TPad("pad_lo", "", 0., 0., 0.99, 0.33);
    ////pad_lo->Draw();
    //////pad_lo->SetLogx();
    ////pad_lo->SetLeftMargin(0.12);
    ////pad_lo->SetTopMargin(1.);
    //[>pad_lo->SetBottomMargin(0.3);<]

    //pad_lo->cd();
    //TF1* ratioFit = new TF1("ratioFit", "[0]", hData->GetXaxis()->GetXmin(), hData->GetXaxis()->GetXmax());
    //ratioFit->SetParameter(0, 0.);
    //ratioFit->SetLineColor(46);
    //ratioFit->SetLineWidth(2);
    //TH1* h = (TH1F*)hData->Clone();
    //h->Divide(hMc);
    //h1_style_lo(h);
    //h->SetStats(1);
    //h->SetTitle("Data / MC");
    //h->SetXTitle(Xtitle.c_str());
    //if(Ytitle != "") {
      //h->SetYTitle("Data / MC");
    //}
    //h->SetMarkerSize(1.0);
    //h->SetMaximum(2);
    //h->SetMinimum(0);
    //setDefaultStyle(h, 1. / 0.3333);
    //h->GetYaxis()->SetTickLength(0.04);
    //h->GetYaxis()->SetNdivisions(505, true);
    //h->GetXaxis()->SetTickLength(0.07);

    ////for(int i=1;i<h->GetEntries();i++){
    ////h->SetBinError(i,sqrt(pow(hData->GetBinError(i)/hMc->GetBinContent(i),2)+pow(hMc->GetBinError(i)*hData->GetBinContent(i)/(pow(hMc->GetBinContent(i),2)),2)));
    ////}
    //h->Fit(ratioFit, "RQ");
    //h->GetYaxis()->SetRangeUser(-1,3);
    //double fitValue = ratioFit->GetParameter(0);
    //double fitError = ratioFit->GetParError(0);

    //TPaveText* fitlabel = new TPaveText(0.55, 0.77, 0.88, 0.83, "brNDC");
    //fitlabel->SetTextSize(0.08);
    //fitlabel->SetFillColor(0);
    //fitlabel->SetTextFont(42);
    //TString fitLabelText = TString::Format("Fit: %.3f #pm %.3f", fitValue, fitError);
    //fitlabel->AddText(fitLabelText);
    //fitlabel->Draw("same");

    ////gStyle->SetOptFit(111111);
    //h->Draw("PE1same");
    //gPad->RedrawAxis();

    //pad_hi->cd();
  //}
  //h1_style_hi(hMc);
  //h1_style_hi(hData);


  //gStyle->SetOptStat(0);
  //hMc->Draw("hist");
  //hMc->GetXaxis()->SetTitle(Xtitle.c_str());
  //hMc->GetYaxis()->SetTitle(Ytitle.c_str());
  //hData->Draw("pe1same");
  //hData->SetMarkerColor(getDataColor());
  //hData->SetLineColor(1);
  //hData->SetMarkerStyle(20);
  ////hData->SetMarkerSize(0.8);
  //hMc->SetFillColor(kBlack);
  //hMc->SetFillStyle(3004);
  //hMc->DrawCopy("e2same");
  //hMc->SetFillStyle(1001);
  //hMc->SetFillColor(getMcColor());
  //hMc->SetLineColor(1);
  //if(!inLinScale) {
    //hData->SetMinimum(0.01);
    //gPad-> SetLogy();
  //}
  ////TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  //TLegend* legend = new TLegend(0.55, 0.65, 0.92, 0.88);
  ////TLegend* legend = new TLegend(0.6, 0.6, 0.9, 0.9);
  //legend->SetFillColor(kWhite);
  //legend->SetFillStyle(0);
  ////legend->SetTextSize(0.038);
  //legend->SetLineWidth(0);
  //legend->SetTextFont(42);
  //legend->SetBorderSize(0);
  //legend->AddEntry(hMc,"MC","f");
  //legend->AddEntry(hData,"Data 2012","p");
  //legend->Draw("same");

  //[>  	Float_t cmsTextSize = 0.043;<]
  ////TPaveText* label_cms = get_labelCMS(1);
  ////label_cms->SetTextSize(cmsTextSize);
  //[>label_cms->Draw("same");<]

  //cms_style(true);

  //gPad->RedrawAxis();

  //cCanvas->SaveAs(path.c_str());
/*}*/
Int_t getColorTrans() {
  Int_t ColorTrans = TColor::GetColor("#151515");
  //Int_t ColorTrans = TColor::GetColor("#A4A4A4");
  //Int_t ColorTrans = TColor::GetColor("#585858");
  //Int_t ColorTrans = TColor::GetColor("#424242");
  //Int_t MPFColorTrans = TColor::GetColor(MPFColorStr);
  gROOT->GetColor(ColorTrans)->SetAlpha(0.5);
  return ColorTrans;
}
void drawDataMcComparison(const string& canvasName, TH1 *hMc, TH1 *hData, const string& Xtitle, const string& path, bool inLinScale = false, const string& Ytitle = "", string legendPos = "r", int withratio = 1){ // with Mikko tdrstyle
  TCanvas *c1 = NULL;
  TCanvas *c2 = NULL;

  gStyle->SetOptStat(0);
  hMc->GetXaxis()->SetTitle(Xtitle.c_str());
  hMc->GetYaxis()->SetTitle(Ytitle.c_str());
  hData->SetMarkerColor(getDataColor());
  hData->SetLineColor(1);
  hData->SetMarkerStyle(20);
  //hData->SetMarkerSize(0.8);

  //TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  TLegend* legend = new TLegend(0.7, 0.65, 0.99, 0.88);
  //TLegend* legend = new TLegend(0.6, 0.6, 0.9, 0.9);
  legend->SetFillColor(kWhite);
  legend->SetFillStyle(kNone);
  //legend->SetTextSize(0.001);
  legend->SetLineWidth(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.045);
  legend->SetBorderSize(0);
  legend->AddEntry(hMc,"MC","f");
  legend->AddEntry(hData,"Data","p");


  if(withratio != 1) {
    c1 = tdrCanvas("c1",hMc,2,0,kTRUE);
    tdrDraw(hMc,"hist", kNone,kNone, kSolid, 1, 1001, getMcColor());
    tdrDraw(hData,"P", kFullCircle, getDataColor());
    TH1F* hMc_clone = (TH1F*)hMc->Clone();
    //hMc_clone->SetFillColor(kBlack);
    //hMc_clone->SetFillStyle(3004);
    Int_t ColorTrans = getColorTrans();
    hMc_clone->SetFillColor(ColorTrans);
    hMc_clone->SetMarkerSize(0);
    hMc_clone->SetLineWidth(1);
    hMc_clone->DrawCopy("e2same");
    if(!inLinScale) {
      hData->SetMinimum(0.01);
      gPad-> SetLogy();
    }
    gPad->RedrawAxis();
    legend->Draw("same");
    c1->SaveAs(path.c_str());
  } else if (withratio == 1) {
    TF1* ratioFit = new TF1("ratioFit", "[0]", hData->GetXaxis()->GetXmin(), hData->GetXaxis()->GetXmax());
    ratioFit->SetParameter(0, 0.);
    ratioFit->SetLineColor(46);
    ratioFit->SetLineWidth(2);
    TH1* h = (TH1F*)hData->Clone();
    h->Divide(hMc);
    h->SetStats(1);
    h->SetTitle("Data / MC");
    h->SetXTitle(Xtitle.c_str());
    if(Ytitle != "") {
      h->SetYTitle("Data / MC");
    }
    h->SetMarkerSize(1.0);
/*    h->SetMinimum(0.96);//0.33);*/
    /*h->SetMaximum(1.06);//0.66);*/

    //for(int i=1;i<h->GetEntries();i++){
    //h->SetBinError(i,sqrt(pow(hData->GetBinError(i)/hMc->GetBinContent(i),2)+pow(hMc->GetBinError(i)*hData->GetBinContent(i)/(pow(hMc->GetBinContent(i),2)),2)));
    //}
    TFitResultPtr res = h->Fit(ratioFit, "RQS");
    h->GetYaxis()->SetRangeUser(-1,3);
    double fitValue = ratioFit->GetParameter(0);
    double fitError = ratioFit->GetParError(0);
    double chi2 = ratioFit->GetChisquare();
    //double chi2_test = 0.;
    int ndf =  0;
    if ((TFitResult*) res.Get()) {
      //chi2_test = ((TFitResult*) res.Get())->Chi2();
      ndf =  ((TFitResult*) res.Get())->Ndf();
    }

    TPaveText* fitlabel = new TPaveText(0.65, 0.815, 0.98, 0.875, "brNDC");
    fitlabel->SetTextSize(0.08);
    fitlabel->SetFillColor(kWhite);
    fitlabel->SetFillStyle(kNone);
    fitlabel->SetBorderSize(0);
    fitlabel->SetTextFont(42);
    fitlabel->SetTextSize(0.07);
    TString fitLabelText = TString::Format("Fit: %.3f #pm %.3f", fitValue, fitError);
    fitlabel->AddText(fitLabelText);
    //fitlabel->Draw("same");

    TPaveText* fitlabel_2 = new TPaveText(0.65, 0.745, 0.98, 0.805, "brNDC");
    fitlabel_2->SetTextSize(0.08);
    fitlabel_2->SetFillColor(kWhite);
    fitlabel_2->SetFillStyle(kNone);
    fitlabel_2->SetTextFont(42);
    fitlabel_2->SetBorderSize(0);
    fitlabel_2->SetTextSize(0.07);
    TString fitLabelText_2 = TString::Format("#chi^{2}/ndf: %.3f/%i", chi2, ndf);
    fitlabel_2->AddText(fitLabelText_2);


    gStyle->SetOptFit(0);

    c2 = tdrDiCanvas("c2",hMc,h,2,0);
    c2->cd(1);

    tdrDraw(hMc,"hist", kNone,kNone, kSolid, 1, 1001, getMcColor());
    tdrDraw(hData,"P", kFullCircle, getDataColor());
    TH1F* hMc_clone = (TH1F*)hMc->Clone();
    //hMc_clone->SetFillColor(kBlack);
    //hMc_clone->SetFillStyle(3004);
    Int_t ColorTrans = getColorTrans();
    hMc_clone->SetFillColor(ColorTrans);
    hMc_clone->SetMarkerSize(0);
    hMc_clone->SetLineWidth(1);
    hMc_clone->DrawCopy("e2same");
    if(!inLinScale) {
      hData->SetMinimum(0.01);
      gPad-> SetLogy();
    }
    gPad->RedrawAxis();
    legend->Draw("same");

    c2->cd(2);

    //tdrDraw(h,"P", kOpenCircle, getMcColor());
    tdrDraw(h,"P", kFullCircle, 1);
    fitlabel->Draw("same");
    fitlabel_2->Draw("same");

    gPad->RedrawAxis();
    c2->SaveAs(path.c_str());
  }

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
    //mgResponse->SetMaximum(1.05);
    mgResponse->SetMaximum(1.1);
    mgResponse->SetMinimum(0.9+0.0001);
  } else {
    mgResponse->SetMaximum(0.5);
    mgResponse->SetMinimum(0.3);    
  }
  mgResponse->Draw("P");
  mgResponse->GetXaxis()->SetNdivisions(808);
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
    linfitlabel->SetTextFont(42);
    TString linfitLabelText = TString::Format("Data slope: %.5f #pm %.5f", linfitValue, linfitError);
    linfitlabel->AddText(linfitLabelText);
    linfitlabel->Draw("same");

    gPad->RedrawAxis();	
  }

  //TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  TLegend* legend = new TLegend(0.65, 0.15, 0.95, 0.30);
  legend->SetFillColor(kWhite);
  legend->SetFillStyle(kNone);
  legend->SetTextFont(42);
  legend->SetBorderSize(0);
  //legend->SetTextSize(0.045);
  legend->AddEntry(gResponseMC,"MC","p");
  legend->AddEntry(gResponseData,"Data","p");
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
  gratio->GetXaxis()->SetNdivisions(808);

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

/*void drawComparisonResponse(const string& canvasName, TMultiGraph *mgResponse, TGraphErrors *gResponseMC, TGraphErrors *gResponseData, TGraph *gratio,const string& mcSample, const string& path, bool doFit = false, bool setRangeUser = true) {*/
  //TCanvas *cCanvas = new TCanvas(canvasName.c_str(),canvasName.c_str(), 800, 800);
  //cCanvas->cd();
  //// Data / MC comparison

  //std::shared_ptr<TPad> pad_hi;
  //std::shared_ptr<TPad> pad_lo;
  //pad_hi = std::make_shared<TPad>("pad_hi", "", 0., 0.33333, 1, 1);
  //pad_hi->Draw();
  //pad_hi->SetTopMargin(TOP_MARGIN / .6666);
  //pad_hi->SetLeftMargin(LEFT_MARGIN);
  //pad_hi->SetBottomMargin(0.015);
  //pad_hi->SetRightMargin(RIGHT_MARGIN);
  //pad_lo = std::make_shared<TPad>("pad_lo", "", 0., 0., 1, 0.33333);
  //pad_lo->Draw();
  //pad_lo->SetLeftMargin(LEFT_MARGIN);
  //pad_lo->SetTopMargin(1.);
  //pad_lo->SetBottomMargin(BOTTOM_MARGIN / .3333);
  //pad_lo->SetRightMargin(RIGHT_MARGIN);
  //pad_lo->SetTickx(1);

  ////TPad* pad_hi = new TPad("pad_hi", "", 0., 0.33, 0.99, 0.99);
  ////pad_hi->Draw();
  //////pad_hi->SetLogx();
  ////pad_hi->SetLeftMargin(0.15);
  ////pad_hi->SetBottomMargin(0.015);

  ////// Data / MC ratio
  ////TPad* pad_lo = new TPad("pad_lo", "", 0., 0., 0.99, 0.33);
  ////pad_lo->Draw();
  //////pad_lo->SetLogx();
  ////pad_lo->SetLeftMargin(0.15);
  ////pad_lo->SetTopMargin(1.);
  ////pad_lo->SetBottomMargin(0.3);


  //pad_hi->cd();

  //gStyle->SetOptStat(0);
  //if (setRangeUser) {
    //mgResponse->SetMaximum(1.05);
    //mgResponse->SetMinimum(0.9);
  //} else {
    //mgResponse->SetMaximum(0.5);
    //mgResponse->SetMinimum(0.3);    
  //}
  //mgResponse->Draw("AP");
  //setDefaultStyle(mgResponse, 1. / 0.3333);
  ////mgResponse->GetXaxis()->SetLabelSize(0);
  ////mgResponse->GetYaxis()->SetTitleOffset(1.3);
  //cCanvas->SetLogx(1);

  //if(doFit) {
    //TF1* myLinFit = new TF1("myLinFit", "[0]*x + [1]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
    //myLinFit->SetParameter(0.5, 1.);
    //myLinFit->SetLineColor(1);
    //myLinFit->SetLineWidth(2);


    //cCanvas->Update();
    //gResponseData->Fit(myLinFit, "RQ");
    //double linfitValue = myLinFit->GetParameter(0);
    //double linfitError = myLinFit->GetParError(0);

    //TPaveText* linfitlabel = new TPaveText(0.55, 0.77, 0.88, 0.83, "brNDC");
    //linfitlabel->SetTextSize(0.035);
    //linfitlabel->SetFillColor(0);
    //linfitlabel->SetTextFont(42);
    //TString linfitLabelText = TString::Format("Data slope: %.5f #pm %.5f", linfitValue, linfitError);
    //linfitlabel->AddText(linfitLabelText);
    //linfitlabel->Draw("same");

    //gPad->RedrawAxis();	
  //}

  ////TLegend* legend = new TLegend(0.55, 0.15, 0.92, 0.38);
  //TLegend* legend = new TLegend(0.65, 0.15, 0.95, 0.30);
  //legend->SetFillColor(kWhite);
  //legend->SetFillStyle(0);
  //legend->SetTextFont(42);
  //legend->SetBorderSize(0);
  ////legend->SetTextSize(0.038);
  //legend->AddEntry(gResponseMC,"MC","p");
  //legend->AddEntry(gResponseData,"Data 2012","p");
  //legend->Draw("same");

  ////Float_t cmsTextSize = 0.043;
  ////TPaveText* label_cms = get_labelCMS(1);
  ////label_cms->SetTextSize(cmsTextSize);
  ////label_cms->Draw("same");
  //cms_style(true);

  //gPad->RedrawAxis();

  //pad_lo->cd();
  //TF1* ratioFit = new TF1("ratioFit", "[0]", mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());
  //ratioFit->SetParameter(0, 0.);
  //ratioFit->SetLineColor(46);
  //ratioFit->SetLineWidth(2);
  //gratio->Draw("APE1");
  //gratio->GetYaxis()->SetTitle("Data / MC");
  //// 	//gratio->GetXaxis()->SetLabelOffset(0.1);
  //// 	gratio->GetXaxis()->SetLabelFont(42);
  //// 	gratio->GetXaxis()->SetLabelSize(0.06);
  //// 	//gratio->GetYaxis()->SetLabelOffset(0.1);
  //// 	gratio->GetYaxis()->SetLabelFont(42);
  //// 	gratio->GetYaxis()->SetLabelSize(0.06);
  //// 	gratio->GetXaxis()->SetTitleOffset(-0.6);
  //// 	gratio->GetXaxis()->SetTitleFont(42);
  //// 	gratio->GetXaxis()->SetTitleSize(0.06);
  ////gratio->GetYaxis()->SetTitleOffset(1.3);
  //// 	gratio->GetYaxis()->SetTitleFont(42);
  //// 	gratio->GetYaxis()->SetTitleSize(0.06);

  //gratio->SetMarkerSize(1.0);
  //gratio->SetMaximum(1.05);
  //gratio->SetMinimum(0.95);
  //setDefaultStyle(gratio, 1. / 0.3333);
  //gratio->GetYaxis()->SetTickLength(0.04);
  //gratio->GetYaxis()->SetNdivisions(505, true);
  //gratio->GetXaxis()->SetTickLength(0.07);

  //gratio->GetXaxis()->SetLimits(mgResponse->GetXaxis()->GetXmin(),mgResponse->GetXaxis()->GetXmax());

  //cCanvas->Update();
  //gratio->Fit(ratioFit, "RQ");
  ////gratio->GetYaxis()->SetRangeUser(-1,3);
  //double fitValue = ratioFit->GetParameter(0);
  //double fitError = ratioFit->GetParError(0);

  //TPaveText* fitlabel = new TPaveText(0.55, 0.77, 0.88, 0.83, "brNDC");
  //fitlabel->SetTextSize(0.08);
  //fitlabel->SetFillColor(0);
  //fitlabel->SetTextFont(42);
  //TString fitLabelText = TString::Format("Fit: %.4f #pm %.4f", fitValue, fitError);
  //fitlabel->AddText(fitLabelText);
  //fitlabel->Draw("same");

  //gPad->RedrawAxis();


  //cCanvas->SaveAs(path.c_str());
/*}*/

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

  TH1::SetDefaultSumw2(true);

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
    extension = plotNameArg.getValue() + extensionArg.getValue();
    rmPU = rmPUArg.getValue();
    useSyst = useSystArg.getValue();
    systFileName = systFileArg.getValue();    
    useRecoilPtBin = recoilPtBinArg.getValue();
    useRecoilPtHLTBin = recoilPtHLTBinArg.getValue();

    if(rmPU) {
      extension = "_woPUJets" + plotNameArg.getValue() + extensionArg.getValue();    
    }


  } catch (TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    return 1;
  }


  myHLTPtBinning.fillHLTPtBins(useRecoilPtHLTBin);

  TFile *f_data=TFile::Open(inname_data);
  TFile *f_mc=TFile::Open(inname_mc);

  string myHistoName;

  //applyStyle();
/*  TStyle* m_style = createStyle();*/
  /*m_style->cd();*/

  setTDRStyle();

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
  vector<TH1F*> vMJB_RefObjPt_data_lumi;
  vMJB_RefObjPt_data_lumi.resize(numberPtBins);
  vector<TH1F*> vMJB_RefObjPt_mc_lumi;
  vMJB_RefObjPt_mc_lumi.resize(numberPtBins);
  vector<TH1F*> vMJB_RefObjPt_data_shape;
  vMJB_RefObjPt_data_shape.resize(numberPtBins);
  vector<TH1F*> vMJB_RefObjPt_mc_shape;
  vMJB_RefObjPt_mc_shape.resize(numberPtBins);

  //MPF per recoilpt
  vector<TH1F*> vMPF_RefObjPt_data_lumi;
  vMPF_RefObjPt_data_lumi.resize(numberPtBins);
  vector<TH1F*> vMPF_RefObjPt_mc_lumi;
  vMPF_RefObjPt_mc_lumi.resize(numberPtBins);
  vector<TH1F*> vMPF_RefObjPt_data_shape;
  vMPF_RefObjPt_data_shape.resize(numberPtBins);
  vector<TH1F*> vMPF_RefObjPt_mc_shape;
  vMPF_RefObjPt_mc_shape.resize(numberPtBins);

  //LeandingJetPt per recoilpt
  vector<TH1F*> vLeadingJetPt_RecoilPt_data_lumi;
  vLeadingJetPt_RecoilPt_data_lumi.resize(numberPtBins);
  vector<TH1F*> vLeadingJetPt_RecoilPt_mc_lumi;
  vLeadingJetPt_RecoilPt_mc_lumi.resize(numberPtBins);
  vector<TH1F*> vLeadingJetPt_RecoilPt_data_shape;
  vLeadingJetPt_RecoilPt_data_shape.resize(numberPtBins);
  vector<TH1F*> vLeadingJetPt_RecoilPt_mc_shape;
  vLeadingJetPt_RecoilPt_mc_shape.resize(numberPtBins);

  //LeandingJetPt per leadingjetrawpt
  vector<TH1F*> vLeadingJetPt_RefObjPtHLT_data_lumi;
  vLeadingJetPt_RefObjPtHLT_data_lumi.resize(numberHLTPtBins);
  vector<TH1F*> vLeadingJetPt_RefObjPtHLT_mc_lumi;
  vLeadingJetPt_RefObjPtHLT_mc_lumi.resize(numberHLTPtBins);
  vector<TH1F*> vLeadingJetPt_RefObjPtHLT_data_shape;
  vLeadingJetPt_RefObjPtHLT_data_shape.resize(numberHLTPtBins);
  vector<TH1F*> vLeadingJetPt_RefObjPtHLT_mc_shape;
  vLeadingJetPt_RefObjPtHLT_mc_shape.resize(numberHLTPtBins);

  //RecoilPt per recoilpt
  vector<TH1F*> vRecoilPt_RefObjPt_data_lumi;
  vRecoilPt_RefObjPt_data_lumi.resize(numberPtBins);
  vector<TH1F*> vRecoilPt_RefObjPt_mc_lumi;
  vRecoilPt_RefObjPt_mc_lumi.resize(numberPtBins);
  vector<TH1F*> vRecoilPt_RefObjPt_data_shape;
  vRecoilPt_RefObjPt_data_shape.resize(numberPtBins);
  vector<TH1F*> vRecoilPt_RefObjPt_mc_shape;
  vRecoilPt_RefObjPt_mc_shape.resize(numberPtBins);

  //MJB per recoileta
  vector<TH1F*> vMJB_RefObjEta_data_lumi;
  vMJB_RefObjEta_data_lumi.resize(numberEtaBins);
  vector<TH1F*> vMJB_RefObjEta_mc_lumi;
  vMJB_RefObjEta_mc_lumi.resize(numberEtaBins);
  vector<TH1F*> vMJB_RefObjEta_data_shape;
  vMJB_RefObjEta_data_shape.resize(numberEtaBins);
  vector<TH1F*> vMJB_RefObjEta_mc_shape;
  vMJB_RefObjEta_mc_shape.resize(numberEtaBins);


  for(int j=0; j<myPtBinning.getSize(); j++) {
    ptBinName = myPtBinning.getName(j);
    vectorName = "MJB/PtBin/MJB_" + ptBinName;
    vMJB_RefObjPt_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
    vMJB_RefObjPt_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
    vectorName = "MPF/PtBin/MPF_" + ptBinName;
    vMPF_RefObjPt_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
    vMPF_RefObjPt_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
    vectorName = "leadingJet/PtBin/LeadingJetPt_" + ptBinName;
    vLeadingJetPt_RecoilPt_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
    vLeadingJetPt_RecoilPt_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
    vectorName = "recoil/PtBin/RecoilPt_" + ptBinName;
    vRecoilPt_RefObjPt_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
    vRecoilPt_RefObjPt_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
  }

  for(int j=0; j<myHLTPtBinning.getSize(); j++) {
    ptBinName = myHLTPtBinning.getName(j);
    vectorName = "leadingJet/HLTPtBin/LeadingJetPt_perHLTTrigger_" + ptBinName;
    //vectorName = "leadingJet/HLTPtBin/LeadingJetPt_" + ptBinName;
    vLeadingJetPt_RefObjPtHLT_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
    vLeadingJetPt_RefObjPtHLT_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
  }

  for(int j=0; j<myNpvBinning.getSize(); j++) {
    npvBinName = myNpvBinning.getName(j);
    vectorName = "MJB/npvBin/MJB_" + npvBinName;
    vMJB_Npv_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
    vMJB_Npv_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());		
  }

  for(int j=0; j<myEtaBinning.getSize(); j++) {
    etaBinName = myEtaBinning.getName(j);
    vectorName = "MJB/EtaBin/MJB_" + etaBinName;
    vMJB_RefObjEta_data_lumi[j] = (TH1F*)f_data->Get(vectorName.c_str());
    vMJB_RefObjEta_mc_lumi[j] = (TH1F*)f_mc->Get(vectorName.c_str());	
  }


  //************************************************************************************************************
  //
  //                                      MJB as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gMJB_RefObjPt_data=(TGraphErrors*)f_data->Get("MJB/PtBin/gMJB_RefObjPt");
  TGraphErrors* gMJB_RefObjPt_mc=(TGraphErrors*)f_mc->Get("MJB/PtBin/gMJB_RefObjPt");

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
    mgMJB_RefObjPt->SetTitle("MJB as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];MJB");
  }
  else {
    mgMJB_RefObjPt->SetTitle("MJB as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];MJB");
  }

  TGraphErrors *gMJB_RefObjPt_ratio = NULL;
  if(useRecoilPtBin) {
    gMJB_RefObjPt_ratio = getDataMcResponseRatio(gMJB_RefObjPt_data,gMJB_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
    gMJB_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gMJB_RefObjPt_ratio = getDataMcResponseRatio(gMJB_RefObjPt_data,gMJB_RefObjPt_mc,numberPtBins, "p_{T}^{Leading Jet} [GeV]");
    gMJB_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");  
  }
  gMJB_RefObjPt_ratio->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
  gMJB_RefObjPt_ratio->SetName("Data/MC");
  gMJB_RefObjPt_ratio->SetTitle("Data/MC");
  gMJB_RefObjPt_ratio->SetMarkerSize(1.0);
  gMJB_RefObjPt_ratio->Fit("func","","",gMJB_RefObjPt_data->GetXaxis()->GetXmin(),gMJB_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/response/MJB_RefObjPt" + extension;	
  drawComparisonResponse("r1", mgMJB_RefObjPt, gMJB_RefObjPt_mc, gMJB_RefObjPt_data, gMJB_RefObjPt_ratio,"MC", myHistoName.c_str());

  //************************************************************************************************************
  //
  //                                      MJB as a function of ptrecoil without last ptbin (response equal to zero)
  //
  //************************************************************************************************************

  /* Double_t aMJB_RefObjPt_data_Mean[numberPtBins-1];*/
  //Double_t aMJB_RefObjPt_data_MeanError[numberPtBins-1];
  //Double_t aMJB_RefObjPt_mc_Mean[numberPtBins-1];
  //Double_t aMJB_RefObjPt_mc_MeanError[numberPtBins-1];
  //Double_t aRefObjPtBins_Mean[numberPtBins-1];
  //Double_t aRefObjPtBins_MeanError[numberPtBins-1];
  //Double_t aMJBRatio_Mean[numberPtBins-1];
  //Double_t aMJBRatio_MeanError[numberPtBins-1];

  //for(int i=0; i<numberPtBins-1; i++) {
  //gMJB_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i],aMJB_RefObjPt_data_Mean[i]);
  //gMJB_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i],aMJB_RefObjPt_mc_Mean[i]);
  //gMJB_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i],aMJBRatio_Mean[i]);
  //aMJB_RefObjPt_data_MeanError[i] = gMJB_RefObjPt_data->GetErrorY(i);
  //aMJB_RefObjPt_mc_MeanError[i] = gMJB_RefObjPt_mc->GetErrorY(i);
  //aRefObjPtBins_MeanError[i] = gMJB_RefObjPt_data->GetErrorX(i);
  //aMJBRatio_MeanError[i] = gMJB_RefObjPt_ratio->GetErrorY(i);
  //}

  //TGraphErrors* gMJB_RefObjPt_data_resize = new TGraphErrors(numberPtBins-1,aRefObjPtBins_Mean, aMJB_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_data_MeanError);
  //TGraphErrors* gMJB_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-1,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_mc_MeanError);	
  //TGraph* gMJB_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-1,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean);	

  //TGraphErrors* gMJB_RefObjPt_mc_clone_resize = (TGraphErrors*)gMJB_RefObjPt_mc_resize->Clone();

  //TGraph_data_style (gMJB_RefObjPt_data_resize);
  //TGraph_mc_style (gMJB_RefObjPt_mc_resize);
  //TGraph_mc_style (gMJB_RefObjPt_mc_resize_pointsOnly);
  //TGraph_mc_style (gMJB_RefObjPt_mc_clone_resize);

  //gMJB_RefObjPt_mc_clone_resize->SetFillColor(17);
  ////gMJB_RefObjPt_mc_resize->SetFillStyle(3002);
  //gMJB_RefObjPt_mc_resize->SetFillColor(17);

  //TMultiGraph *mgMJB_RefObjPt_resize = new TMultiGraph();
  //mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize,"e3");
  //mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize_pointsOnly,"p");
  //mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_data_resize,"pe");
  //if(useRecoilPtBin) {
  //mgMJB_RefObjPt_resize->SetTitle("MJB as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];MJB");
  //}
  //else {
  //mgMJB_RefObjPt_resize->SetTitle("MJB as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];MJB");
  //}

  //TGraphErrors *gMJB_RefObjPt_ratio_resize = NULL;
  //if(useRecoilPtBin) {
  //gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-1, "p_{T}^{Recoil} [GeV]");
  //gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  //}
  //else {
  //gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-1, "p_{T}^{Leading Jet} [GeV]");
  //gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  //}
  //gMJB_RefObjPt_ratio_resize->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
  //gMJB_RefObjPt_ratio_resize->SetName("Data/MC");
  //gMJB_RefObjPt_ratio_resize->SetTitle("Data/MC");
  //gMJB_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  //gMJB_RefObjPt_ratio_resize->Fit("func","","",gMJB_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMJB_RefObjPt_data_resize->GetXaxis()->GetXmax());

  //myHistoName = "images/response/MJB_RefObjPt_resize" + extension;	
  /*drawComparisonResponse("r1", mgMJB_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize,"MC", myHistoName.c_str());*/

  //*****************************************************************************************************************************

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
    mgMJB_RefObjPt_resize->SetTitle("MJB as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];MJB");
  }
  else {
    mgMJB_RefObjPt_resize->SetTitle("MJB as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];MJB");
  }

  TGraphErrors *gMJB_RefObjPt_ratio_resize = NULL;
  if(useRecoilPtBin) {
    gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
    gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Leading Jet} [GeV]");
    gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  }
  gMJB_RefObjPt_ratio_resize->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
  gMJB_RefObjPt_ratio_resize->SetName("Data/MC");
  gMJB_RefObjPt_ratio_resize->SetTitle("Data/MC");
  gMJB_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gMJB_RefObjPt_ratio_resize->Fit("func","","",gMJB_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMJB_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/response/MJB_RefObjPt_resize" + extension;	
  drawComparisonResponse("r1", mgMJB_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize,"MC", myHistoName.c_str());

  //***********************************************************************************************************
/*  Double_t aMJB_RefObjPt_data_Mean[numberPtBins-2];*/
  //Double_t aMJB_RefObjPt_data_MeanError[numberPtBins-2];
  //Double_t aMJB_RefObjPt_mc_Mean[numberPtBins-2];
  //Double_t aMJB_RefObjPt_mc_MeanError[numberPtBins-2];
  //Double_t aMJBRatio_Mean[numberPtBins-2];
  //Double_t aMJBRatio_MeanError[numberPtBins-2];
  //Double_t aRefObjPtBins_Mean[numberPtBins-2];
  //Double_t aRefObjPtBins_MeanError[numberPtBins-2];

  //for(int i=0; i<numberPtBins-2; i++) {
    //gMJB_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i],aMJB_RefObjPt_data_Mean[i]);
    //gMJB_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i],aMJB_RefObjPt_mc_Mean[i]);
    //gMJB_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i],aMJBRatio_Mean[i]);
    //aMJB_RefObjPt_data_MeanError[i] = gMJB_RefObjPt_data->GetErrorY(i);
    //aMJB_RefObjPt_mc_MeanError[i] = gMJB_RefObjPt_mc->GetErrorY(i);
    //aRefObjPtBins_MeanError[i] = gMJB_RefObjPt_data->GetErrorX(i);
    //aMJBRatio_MeanError[i] = gMJB_RefObjPt_ratio->GetErrorY(i);
  //}

  //TGraphErrors* gMJB_RefObjPt_data_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aMJB_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_data_MeanError);
  //TGraphErrors* gMJB_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aMJB_RefObjPt_mc_MeanError);	
  //TGraph* gMJB_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-2,aRefObjPtBins_Mean, aMJB_RefObjPt_mc_Mean);	

  //TGraphErrors* gMJB_RefObjPt_mc_clone_resize = (TGraphErrors*)gMJB_RefObjPt_mc_resize->Clone();

  //TGraph_data_style (gMJB_RefObjPt_data_resize);
  //TGraph_mc_style (gMJB_RefObjPt_mc_resize);
  //TGraph_mc_style (gMJB_RefObjPt_mc_resize_pointsOnly);
  //TGraph_mc_style (gMJB_RefObjPt_mc_clone_resize);

  //gMJB_RefObjPt_mc_clone_resize->SetFillColor(17);
  ////gMJB_RefObjPt_mc_resize->SetFillStyle(3002);
  //gMJB_RefObjPt_mc_resize->SetFillColor(17);

  //TMultiGraph *mgMJB_RefObjPt_resize = new TMultiGraph();
  //mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize,"e3");
  //mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_mc_resize_pointsOnly,"p");
  //mgMJB_RefObjPt_resize->Add(gMJB_RefObjPt_data_resize,"pe");
  //if(useRecoilPtBin) {
    //mgMJB_RefObjPt_resize->SetTitle("MJB as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];MJB");
  //}
  //else {
    //mgMJB_RefObjPt_resize->SetTitle("MJB as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];MJB");
  //}

  //TGraphErrors *gMJB_RefObjPt_ratio_resize = NULL;
  //if(useRecoilPtBin) {
    //gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Recoil} [GeV]");
    //gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  //}
  //else {
    //gMJB_RefObjPt_ratio_resize = getDataMcResponseRatio(gMJB_RefObjPt_data_resize,gMJB_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Leading Jet} [GeV]");
    //gMJB_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  //}
  //gMJB_RefObjPt_ratio_resize->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
  //gMJB_RefObjPt_ratio_resize->SetName("Data/MC");
  //gMJB_RefObjPt_ratio_resize->SetTitle("Data/MC");
  //gMJB_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  //gMJB_RefObjPt_ratio_resize->Fit("func","","",gMJB_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMJB_RefObjPt_data_resize->GetXaxis()->GetXmax());

  //myHistoName = "images/response/MJB_RefObjPt_resize" + extension;	
  /*drawComparisonResponse("r1", mgMJB_RefObjPt_resize, gMJB_RefObjPt_mc_resize, gMJB_RefObjPt_data_resize, gMJB_RefObjPt_ratio_resize,"MC", myHistoName.c_str());*/

  //************************************************************************************************************
  //
  //                                      MPF as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gMPF_RefObjPt_data=(TGraphErrors*)f_data->Get("MPF/PtBin/gMPF_RefObjPt");
  TGraphErrors* gMPF_RefObjPt_mc=(TGraphErrors*)f_mc->Get("MPF/PtBin/gMPF_RefObjPt");

  if (useSyst) {
    if (systFileName == "") {
      std::cout << "ERROR! You must specify the file to use for systematics! *********************" << std::endl;
    }
    else {
      TFile *f_syst=TFile::Open(systFileName.c_str());
      TGraphErrors* gMPF_SystTot=(TGraphErrors*)f_syst->Get("MPF/PtBin/gMPF_SystTot");
      for(int i=0; i<numberPtBins; i++) {
        // quadratic sum of statitics and systematics errors
        gMPF_RefObjPt_mc->SetPointError(i, 0., sqrt(gMPF_RefObjPt_mc->GetErrorY(i)*gMPF_RefObjPt_mc->GetErrorY(i) + gMPF_SystTot->GetErrorY(i)*gMPF_SystTot->GetErrorY(i)));
      }
    }
  }

  TGraphErrors* gMPF_RefObjPt_mc_clone = (TGraphErrors*)gMPF_RefObjPt_mc->Clone();

  TGraph_data_style (gMPF_RefObjPt_data);
  TGraph_mc_style (gMPF_RefObjPt_mc);
  TGraph_mc_style (gMPF_RefObjPt_mc_clone);

  gMPF_RefObjPt_mc_clone->SetFillColor(17);
  //gMPF_RefObjPt_mc_clone->SetFillStyle(3002);
  gMPF_RefObjPt_mc->SetLineColor(17);

  TMultiGraph *mgMPF_RefObjPt = new TMultiGraph();
  mgMPF_RefObjPt->Add(gMPF_RefObjPt_mc_clone,"e3");
  mgMPF_RefObjPt->Add(gMPF_RefObjPt_mc,"p");
  mgMPF_RefObjPt->Add(gMPF_RefObjPt_data,"pe");
  if(useRecoilPtBin) {
    mgMPF_RefObjPt->SetTitle("MPF as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];MPF");
  }
  else {
    mgMPF_RefObjPt->SetTitle("MPF as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];MPF");
  }

  TGraphErrors *gMPF_RefObjPt_ratio = NULL;
  if(useRecoilPtBin) {
    gMPF_RefObjPt_ratio = getDataMcResponseRatio(gMPF_RefObjPt_data,gMPF_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
    gMPF_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gMPF_RefObjPt_ratio = getDataMcResponseRatio(gMPF_RefObjPt_data,gMPF_RefObjPt_mc,numberPtBins, "p_{T}^{Leading Jet} [GeV]");
    gMPF_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");  
  }
  gMPF_RefObjPt_ratio->GetYaxis()->SetTitle("MPF^{data}/MPF^{MC}");
  gMPF_RefObjPt_ratio->SetName("Data/MC");
  gMPF_RefObjPt_ratio->SetTitle("Data/MC");
  gMPF_RefObjPt_ratio->SetMarkerSize(1.0);
  gMPF_RefObjPt_ratio->Fit("func","","",gMPF_RefObjPt_data->GetXaxis()->GetXmin(),gMPF_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/response/MPF_RefObjPt" + extension;	
  drawComparisonResponse("r1", mgMPF_RefObjPt, gMPF_RefObjPt_mc, gMPF_RefObjPt_data, gMPF_RefObjPt_ratio,"MC", myHistoName.c_str());

  //************************************************************************************************************
  //
  //                                      MPF as a function of ptrecoil without 2 ptbin
  //
  //************************************************************************************************************

/*  Double_t aMPF_RefObjPt_data_Mean[numberPtBins-2];*/
  //Double_t aMPF_RefObjPt_data_MeanError[numberPtBins-2];
  //Double_t aMPF_RefObjPt_mc_Mean[numberPtBins-2];
  //Double_t aMPF_RefObjPt_mc_MeanError[numberPtBins-2];
  //Double_t aMPFRatio_Mean[numberPtBins-2];
  //Double_t aMPFRatio_MeanError[numberPtBins-2];

  //for(int i=0; i<numberPtBins-2; i++) {
    //gMPF_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i],aMPF_RefObjPt_data_Mean[i]);
    //gMPF_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i],aMPF_RefObjPt_mc_Mean[i]);
    //gMPF_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i],aMPFRatio_Mean[i]);
    //aMPF_RefObjPt_data_MeanError[i] = gMPF_RefObjPt_data->GetErrorY(i);
    //aMPF_RefObjPt_mc_MeanError[i] = gMPF_RefObjPt_mc->GetErrorY(i);
    //aRefObjPtBins_MeanError[i] = gMPF_RefObjPt_data->GetErrorX(i);
    //aMPFRatio_MeanError[i] = gMPF_RefObjPt_ratio->GetErrorY(i);
  //}

  //TGraphErrors* gMPF_RefObjPt_data_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aMPF_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aMPF_RefObjPt_data_MeanError);
  //TGraphErrors* gMPF_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aMPF_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aMPF_RefObjPt_mc_MeanError);	
  //TGraph* gMPF_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-2,aRefObjPtBins_Mean, aMPF_RefObjPt_mc_Mean);	

  //TGraphErrors* gMPF_RefObjPt_mc_clone_resize = (TGraphErrors*)gMPF_RefObjPt_mc_resize->Clone();

  //TGraph_data_style (gMPF_RefObjPt_data_resize);
  //TGraph_mc_style (gMPF_RefObjPt_mc_resize);
  //TGraph_mc_style (gMPF_RefObjPt_mc_resize_pointsOnly);
  //TGraph_mc_style (gMPF_RefObjPt_mc_clone_resize);

  //gMPF_RefObjPt_mc_clone_resize->SetFillColor(17);
  ////gMPF_RefObjPt_mc_resize->SetFillStyle(3002);
  //gMPF_RefObjPt_mc_resize->SetFillColor(17);

  //TMultiGraph *mgMPF_RefObjPt_resize = new TMultiGraph();
  //mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_mc_resize,"e3");
  //mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_mc_resize_pointsOnly,"p");
  //mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_data_resize,"pe");
  //if(useRecoilPtBin) {
    //mgMPF_RefObjPt_resize->SetTitle("MPF as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];MPF");
  //}
  //else {
    //mgMPF_RefObjPt_resize->SetTitle("MPF as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];MPF");
  //}

  //TGraphErrors *gMPF_RefObjPt_ratio_resize = NULL;
  //if(useRecoilPtBin) {
    //gMPF_RefObjPt_ratio_resize = getDataMcResponseRatio(gMPF_RefObjPt_data_resize,gMPF_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Recoil} [GeV]");
    //gMPF_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  //}
  //else {
    //gMPF_RefObjPt_ratio_resize = getDataMcResponseRatio(gMPF_RefObjPt_data_resize,gMPF_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Leading Jet} [GeV]");
    //gMPF_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  //}
  //gMPF_RefObjPt_ratio_resize->GetYaxis()->SetTitle("MPF^{data}/MPF^{MC}");
  //gMPF_RefObjPt_ratio_resize->SetName("Data/MC");
  //gMPF_RefObjPt_ratio_resize->SetTitle("Data/MC");
  //gMPF_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  //gMPF_RefObjPt_ratio_resize->Fit("func","","",gMPF_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMPF_RefObjPt_data_resize->GetXaxis()->GetXmax());

  //myHistoName = "images/response/MPF_RefObjPt_resize" + extension;	
  /*drawComparisonResponse("r1", mgMPF_RefObjPt_resize, gMPF_RefObjPt_mc_resize, gMPF_RefObjPt_data_resize, gMPF_RefObjPt_ratio_resize,"MC", myHistoName.c_str());*/


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

  TGraph_data_style (gMPF_RefObjPt_data_resize);
  TGraph_mc_style (gMPF_RefObjPt_mc_resize);
  TGraph_mc_style (gMPF_RefObjPt_mc_resize_pointsOnly);
  TGraph_mc_style (gMPF_RefObjPt_mc_clone_resize);

  gMPF_RefObjPt_mc_clone_resize->SetFillColor(17);
  //gMPF_RefObjPt_mc_resize->SetFillStyle(3002);
  gMPF_RefObjPt_mc_resize->SetFillColor(17);

  TMultiGraph *mgMPF_RefObjPt_resize = new TMultiGraph();
  mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_mc_resize,"e3");
  mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_mc_resize_pointsOnly,"p");
  mgMPF_RefObjPt_resize->Add(gMPF_RefObjPt_data_resize,"pe");
  if(useRecoilPtBin) {
    mgMPF_RefObjPt_resize->SetTitle("MPF as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];MPF");
  }
  else {
    mgMPF_RefObjPt_resize->SetTitle("MPF as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];MPF");
  }

  TGraphErrors *gMPF_RefObjPt_ratio_resize = NULL;
  if(useRecoilPtBin) {
    gMPF_RefObjPt_ratio_resize = getDataMcResponseRatio(gMPF_RefObjPt_data_resize,gMPF_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
    gMPF_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gMPF_RefObjPt_ratio_resize = getDataMcResponseRatio(gMPF_RefObjPt_data_resize,gMPF_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Leading Jet} [GeV]");
    gMPF_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  }
  gMPF_RefObjPt_ratio_resize->GetYaxis()->SetTitle("MPF^{data}/MPF^{MC}");
  gMPF_RefObjPt_ratio_resize->SetName("Data/MC");
  gMPF_RefObjPt_ratio_resize->SetTitle("Data/MC");
  gMPF_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gMPF_RefObjPt_ratio_resize->Fit("func","","",gMPF_RefObjPt_data_resize->GetXaxis()->GetXmin(),gMPF_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/response/MPF_RefObjPt_resize" + extension;	
  drawComparisonResponse("r1", mgMPF_RefObjPt_resize, gMPF_RefObjPt_mc_resize, gMPF_RefObjPt_data_resize, gMPF_RefObjPt_ratio_resize,"MC", myHistoName.c_str());

  //************************************************************************************************************
  //
  //                                      cExp as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gcExp_RefObjPt_data=(TGraphErrors*)f_data->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");
  TGraphErrors* gcExp_RefObjPt_mc=(TGraphErrors*)f_mc->Get("recoilJets/gExp_sum_Fi_log_fi_RecoilPt");

  TGraphErrors* gcExp_RefObjPt_mc_clone = (TGraphErrors*)gcExp_RefObjPt_mc->Clone();

  TGraph_data_style (gcExp_RefObjPt_data);
  TGraph_mc_style (gcExp_RefObjPt_mc);
  TGraph_mc_style (gcExp_RefObjPt_mc_clone);

  gcExp_RefObjPt_mc_clone->SetFillColor(17);
  //gcExp_RefObjPt_mc_clone->SetFillStyle(3002);
  gcExp_RefObjPt_mc->SetLineColor(17);

  TMultiGraph *mgcExp_RefObjPt = new TMultiGraph();
  mgcExp_RefObjPt->Add(gcExp_RefObjPt_mc_clone,"e3");
  mgcExp_RefObjPt->Add(gcExp_RefObjPt_mc,"p");
  mgcExp_RefObjPt->Add(gcExp_RefObjPt_data,"pe");
  if(useRecoilPtBin) {
    mgcExp_RefObjPt->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];exp(#sum_{i}[F_{i} * log(f_{i})])");
  }
  else {
    mgcExp_RefObjPt->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];exp(#sum_{i}[F_{i} * log(f_{i})])");
  }

  TGraphErrors *gcExp_RefObjPt_ratio = NULL;
  if(useRecoilPtBin) {
    gcExp_RefObjPt_ratio = getDataMcResponseRatio(gcExp_RefObjPt_data,gcExp_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
    gcExp_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gcExp_RefObjPt_ratio = getDataMcResponseRatio(gcExp_RefObjPt_data,gcExp_RefObjPt_mc,numberPtBins, "p_{T}^{Leading Jet} [GeV]");
    gcExp_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");  
  }
  gcExp_RefObjPt_ratio->GetYaxis()->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio->SetName("Data / MC");
  gcExp_RefObjPt_ratio->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio->SetMarkerSize(1.0);
  gcExp_RefObjPt_ratio->Fit("func","","",gcExp_RefObjPt_data->GetXaxis()->GetXmin(),gcExp_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/cExp/cExp_sum_Fi_log_fi_RecoilPt_RefObjPt" + extension;	
  drawComparisonResponse("r1", mgcExp_RefObjPt, gcExp_RefObjPt_mc, gcExp_RefObjPt_data, gcExp_RefObjPt_ratio,"MC", myHistoName.c_str(), false, false);

  //************************************************************************************************************
  //
  //                                      cExp as a function of ptrecoil without 2 ptbin
  //
  //************************************************************************************************************

  Double_t acExp_RefObjPt_data_Mean[numberPtBins-3];
  Double_t acExp_RefObjPt_data_MeanError[numberPtBins-3];
  Double_t acExp_RefObjPt_mc_Mean[numberPtBins-3];
  Double_t acExp_RefObjPt_mc_MeanError[numberPtBins-3];
  Double_t acExpRatio_Mean[numberPtBins-3];
  Double_t acExpRatio_MeanError[numberPtBins-3];

  for(int i=1; i<numberPtBins-2; i++) {
    gcExp_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i-1],acExp_RefObjPt_data_Mean[i-1]);
    gcExp_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i-1],acExp_RefObjPt_mc_Mean[i-1]);
    gcExp_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i-1],acExpRatio_Mean[i-1]);
    acExp_RefObjPt_data_MeanError[i-1] = gcExp_RefObjPt_data->GetErrorY(i);
    acExp_RefObjPt_mc_MeanError[i-1] = gcExp_RefObjPt_mc->GetErrorY(i);
    aRefObjPtBins_MeanError[i-1] = gcExp_RefObjPt_data->GetErrorX(i);
    acExpRatio_MeanError[i-1] = gcExp_RefObjPt_ratio->GetErrorY(i);
  }

  TGraphErrors* gcExp_RefObjPt_data_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_data_Mean, aRefObjPtBins_MeanError, acExp_RefObjPt_data_MeanError);
  TGraphErrors* gcExp_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, acExp_RefObjPt_mc_MeanError);	
  TGraph* gcExp_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-3,aRefObjPtBins_Mean, acExp_RefObjPt_mc_Mean);	

  TGraphErrors* gcExp_RefObjPt_mc_clone_resize = (TGraphErrors*)gcExp_RefObjPt_mc_resize->Clone();

  TGraph_data_style (gcExp_RefObjPt_data_resize);
  TGraph_mc_style (gcExp_RefObjPt_mc_resize);
  TGraph_mc_style (gcExp_RefObjPt_mc_resize_pointsOnly);
  TGraph_mc_style (gcExp_RefObjPt_mc_clone_resize);

  gcExp_RefObjPt_mc_clone_resize->SetFillColor(17);
  //gcExp_RefObjPt_mc_resize->SetFillStyle(3002);
  gcExp_RefObjPt_mc_resize->SetFillColor(17);

  TMultiGraph *mgcExp_RefObjPt_resize = new TMultiGraph();
  mgcExp_RefObjPt_resize->Add(gcExp_RefObjPt_mc_resize,"e3");
  mgcExp_RefObjPt_resize->Add(gcExp_RefObjPt_mc_resize_pointsOnly,"p");
  mgcExp_RefObjPt_resize->Add(gcExp_RefObjPt_data_resize,"pe");
  if(useRecoilPtBin) {
    mgcExp_RefObjPt_resize->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];exp(#sum_{i}[F_{i} * log(f_{i})])");
  }
  else {
    mgcExp_RefObjPt_resize->SetTitle("exp(#sum_{i}[F_{i} * log(f_{i})]) as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];exp(#sum_{i}[F_{i} * log(f_{i})])");
  }

  TGraphErrors *gcExp_RefObjPt_ratio_resize = NULL;
  if(useRecoilPtBin) {
    gcExp_RefObjPt_ratio_resize = getDataMcResponseRatio(gcExp_RefObjPt_data_resize,gcExp_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
    gcExp_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gcExp_RefObjPt_ratio_resize = getDataMcResponseRatio(gcExp_RefObjPt_data_resize,gcExp_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Leading Jet} [GeV]");
    gcExp_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  }
  gcExp_RefObjPt_ratio_resize->GetYaxis()->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_resize->SetName("Data / MC");
  gcExp_RefObjPt_ratio_resize->SetTitle("Data / MC");
  gcExp_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gcExp_RefObjPt_ratio_resize->Fit("func","","",gcExp_RefObjPt_data_resize->GetXaxis()->GetXmin(),gcExp_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/cExp/cExp_sum_Fi_log_fi_RecoilPt_RefObjPt_resize" + extension;	
  drawComparisonResponse("r1", mgcExp_RefObjPt_resize, gcExp_RefObjPt_mc_resize, gcExp_RefObjPt_data_resize, gcExp_RefObjPt_ratio_resize,"MC", myHistoName.c_str(), false, false);

  //************************************************************************************************************
  //
  //                                      MJB as a function of reference object eta
  //
  //************************************************************************************************************

  TGraphErrors* gMJB_RefObjEta_data=(TGraphErrors*)f_data->Get("MJB/EtaBin/gMJB_RefObjEta");
  TGraphErrors* gMJB_RefObjEta_mc=(TGraphErrors*)f_mc->Get("MJB/EtaBin/gMJB_RefObjEta");

  TGraph_data_style (gMJB_RefObjEta_data);
  TGraph_mc_style (gMJB_RefObjEta_mc);

  TMultiGraph *mgMJB_RefObjEta = new TMultiGraph();
  mgMJB_RefObjEta->Add(gMJB_RefObjEta_mc,"pe");
  mgMJB_RefObjEta->Add(gMJB_RefObjEta_data,"pe");
  if(useRecoilPtBin) {
    mgMJB_RefObjEta->SetTitle("MJB as a function of |#eta^{Recoil}|;|#eta^{Recoil}|;MJB");
  }  
  else {
    mgMJB_RefObjEta->SetTitle("MJB as a function of |#eta^{Leading Jet}|;|#eta^{Leading Jet}|;MJB");
  }

  TGraphErrors *gMJB_RefObjEta_ratio = NULL;
  if(useRecoilPtBin) {
    gMJB_RefObjEta_ratio = getDataMcResponseRatio(gMJB_RefObjEta_data,gMJB_RefObjEta_mc,numberEtaBins, "p_{T}^{Recoil} [GeV]");
    gMJB_RefObjEta_ratio->GetXaxis()->SetTitle("|#eta^{Recoil}|");
  }
  else {
    gMJB_RefObjEta_ratio = getDataMcResponseRatio(gMJB_RefObjEta_data,gMJB_RefObjEta_mc,numberEtaBins, "p_{T}^{Leading Jet} [GeV]");
    gMJB_RefObjEta_ratio->GetXaxis()->SetTitle("|#eta^{Leading Jet}|");  
  }
  gMJB_RefObjEta_ratio->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
  gMJB_RefObjEta_ratio->SetName("Data/MC");
  gMJB_RefObjEta_ratio->SetTitle("Data/MC");
  gMJB_RefObjEta_ratio->SetMarkerSize(1.0);
  gMJB_RefObjEta_ratio->Fit("func","","",gMJB_RefObjEta_data->GetXaxis()->GetXmin(),gMJB_RefObjEta_data->GetXaxis()->GetXmax());

  myHistoName = "images/response/MJB_RefObjEta" + extension;	
  drawComparisonResponse("r1", mgMJB_RefObjEta, gMJB_RefObjEta_mc, gMJB_RefObjEta_data, gMJB_RefObjEta_ratio,"MC", myHistoName.c_str());

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
  //                                      NjetsRecoil as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gNjetsRecoil_RefObjPt_data=(TGraphErrors*)f_data->Get("recoilJets/gNjetsRecoil_RecoilPt");
  TGraphErrors* gNjetsRecoil_RefObjPt_mc=(TGraphErrors*)f_mc->Get("recoilJets/gNjetsRecoil_RecoilPt");

  TGraphErrors* gNjetsRecoil_RefObjPt_mc_clone = (TGraphErrors*)gNjetsRecoil_RefObjPt_mc->Clone();

  TGraph_data_style (gNjetsRecoil_RefObjPt_data);
  TGraph_mc_style (gNjetsRecoil_RefObjPt_mc);
  TGraph_mc_style (gNjetsRecoil_RefObjPt_mc_clone);

  gNjetsRecoil_RefObjPt_mc_clone->SetFillColor(17);
  //gNjetsRecoil_RefObjPt_mc_clone->SetFillStyle(3002);
  gNjetsRecoil_RefObjPt_mc->SetLineColor(17);

  TMultiGraph *mgNjetsRecoil_RefObjPt = new TMultiGraph();
  mgNjetsRecoil_RefObjPt->Add(gNjetsRecoil_RefObjPt_mc_clone,"e3");
  mgNjetsRecoil_RefObjPt->Add(gNjetsRecoil_RefObjPt_mc,"p");
  mgNjetsRecoil_RefObjPt->Add(gNjetsRecoil_RefObjPt_data,"pe");
  if(useRecoilPtBin) {
    mgNjetsRecoil_RefObjPt->SetTitle("N_{jets in Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}");
  }
  else {
    mgNjetsRecoil_RefObjPt->SetTitle("N_{jets in Recoil} as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];N_{jets in Recoil}");
  }

  TGraphErrors *gNjetsRecoil_RefObjPt_ratio = NULL;
  if(useRecoilPtBin) {
    gNjetsRecoil_RefObjPt_ratio = getDataMcResponseRatio(gNjetsRecoil_RefObjPt_data,gNjetsRecoil_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
    gNjetsRecoil_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gNjetsRecoil_RefObjPt_ratio = getDataMcResponseRatio(gNjetsRecoil_RefObjPt_data,gNjetsRecoil_RefObjPt_mc,numberPtBins, "p_{T}^{Leading Jet} [GeV]");
    gNjetsRecoil_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");  
  }
  gNjetsRecoil_RefObjPt_ratio->GetYaxis()->SetTitle("N_{jets in Recoil}^{data}/N_{jets in Recoil}^{MC}");
  gNjetsRecoil_RefObjPt_ratio->SetName("Data/MC");
  gNjetsRecoil_RefObjPt_ratio->SetTitle("Data/MC");
  gNjetsRecoil_RefObjPt_ratio->SetMarkerSize(1.0);
  gNjetsRecoil_RefObjPt_ratio->Fit("func","","",gNjetsRecoil_RefObjPt_data->GetXaxis()->GetXmin(),gNjetsRecoil_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/NjetsRecoil/NjetsRecoil_RefObjPt" + extension;	
  drawComparisonResponse("r1", mgNjetsRecoil_RefObjPt, gNjetsRecoil_RefObjPt_mc, gNjetsRecoil_RefObjPt_data, gNjetsRecoil_RefObjPt_ratio,"MC", myHistoName.c_str(), false, false);

  //************************************************************************************************************
  //
  //                                      NjetsRecoil as a function of ptrecoil without 2 ptbin
  //
  //************************************************************************************************************

  Double_t aNjetsRecoil_RefObjPt_data_Mean[numberPtBins-3];
  Double_t aNjetsRecoil_RefObjPt_data_MeanError[numberPtBins-3];
  Double_t aNjetsRecoil_RefObjPt_mc_Mean[numberPtBins-3];
  Double_t aNjetsRecoil_RefObjPt_mc_MeanError[numberPtBins-3];
  Double_t aNjetsRecoilRatio_Mean[numberPtBins-3];
  Double_t aNjetsRecoilRatio_MeanError[numberPtBins-3];

  for(int i=1; i<numberPtBins-2; i++) {
    gNjetsRecoil_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_RefObjPt_data_Mean[i-1]);
    gNjetsRecoil_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_RefObjPt_mc_Mean[i-1]);
    gNjetsRecoil_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoilRatio_Mean[i-1]);
    aNjetsRecoil_RefObjPt_data_MeanError[i-1] = gNjetsRecoil_RefObjPt_data->GetErrorY(i);
    aNjetsRecoil_RefObjPt_mc_MeanError[i-1] = gNjetsRecoil_RefObjPt_mc->GetErrorY(i);
    aNjetsRecoilRatio_MeanError[i-1] = gNjetsRecoil_RefObjPt_ratio->GetErrorY(i);
  }

  TGraphErrors* gNjetsRecoil_RefObjPt_data_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, aNjetsRecoil_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_RefObjPt_data_MeanError);
  TGraphErrors* gNjetsRecoil_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-3,aRefObjPtBins_Mean, aNjetsRecoil_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_RefObjPt_mc_MeanError);	
  TGraph* gNjetsRecoil_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-3,aRefObjPtBins_Mean, aNjetsRecoil_RefObjPt_mc_Mean);	

  TGraphErrors* gNjetsRecoil_RefObjPt_mc_clone_resize = (TGraphErrors*)gNjetsRecoil_RefObjPt_mc_resize->Clone();

  TGraph_data_style (gNjetsRecoil_RefObjPt_data_resize);
  TGraph_mc_style (gNjetsRecoil_RefObjPt_mc_resize);
  TGraph_mc_style (gNjetsRecoil_RefObjPt_mc_resize_pointsOnly);
  TGraph_mc_style (gNjetsRecoil_RefObjPt_mc_clone_resize);

  gNjetsRecoil_RefObjPt_mc_clone_resize->SetFillColor(17);
  //gNjetsRecoil_RefObjPt_mc_resize->SetFillStyle(3002);
  gNjetsRecoil_RefObjPt_mc_resize->SetFillColor(17);

  TMultiGraph *mgNjetsRecoil_RefObjPt_resize = new TMultiGraph();
  mgNjetsRecoil_RefObjPt_resize->Add(gNjetsRecoil_RefObjPt_mc_resize,"e3");
  mgNjetsRecoil_RefObjPt_resize->Add(gNjetsRecoil_RefObjPt_mc_resize_pointsOnly,"p");
  mgNjetsRecoil_RefObjPt_resize->Add(gNjetsRecoil_RefObjPt_data_resize,"pe");
  if(useRecoilPtBin) {
    mgNjetsRecoil_RefObjPt_resize->SetTitle("N_{jets in Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}");
  }
  else {
    mgNjetsRecoil_RefObjPt_resize->SetTitle("N_{jets in Recoil} as a function of p_{T}^{Leading Jet};p_{T}^{Leading Jet} [GeV];N_{jets in Recoil}");
  }

  TGraphErrors *gNjetsRecoil_RefObjPt_ratio_resize = NULL;
  if(useRecoilPtBin) {
    gNjetsRecoil_RefObjPt_ratio_resize = getDataMcResponseRatio(gNjetsRecoil_RefObjPt_data_resize,gNjetsRecoil_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Recoil} [GeV]");
    gNjetsRecoil_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gNjetsRecoil_RefObjPt_ratio_resize = getDataMcResponseRatio(gNjetsRecoil_RefObjPt_data_resize,gNjetsRecoil_RefObjPt_mc_resize,numberPtBins-3, "p_{T}^{Leading Jet} [GeV]");
    gNjetsRecoil_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  }
  gNjetsRecoil_RefObjPt_ratio_resize->GetYaxis()->SetTitle("N_{jets in Recoil}^{data}/N_{jets in Recoil}^{MC}");
  gNjetsRecoil_RefObjPt_ratio_resize->SetName("Data/MC");
  gNjetsRecoil_RefObjPt_ratio_resize->SetTitle("Data/MC");
  gNjetsRecoil_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gNjetsRecoil_RefObjPt_ratio_resize->Fit("func","","",gNjetsRecoil_RefObjPt_data_resize->GetXaxis()->GetXmin(),gNjetsRecoil_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/NjetsRecoil/NjetsRecoil_RefObjPt_resize" + extension;	
  drawComparisonResponse("r1", mgNjetsRecoil_RefObjPt_resize, gNjetsRecoil_RefObjPt_mc_resize, gNjetsRecoil_RefObjPt_data_resize, gNjetsRecoil_RefObjPt_ratio_resize,"MC", myHistoName.c_str(), false, false);


  //************************************************************************************************************
  //
  //                                      NjetsRecoil_068E as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gNjetsRecoil_068E_RefObjPt_data=(TGraphErrors*)f_data->Get("recoilJets/gNjetsRecoil_068E_RecoilPt");
  TGraphErrors* gNjetsRecoil_068E_RefObjPt_mc=(TGraphErrors*)f_mc->Get("recoilJets/gNjetsRecoil_068E_RecoilPt");

  TGraphErrors* gNjetsRecoil_068E_RefObjPt_mc_clone = (TGraphErrors*)gNjetsRecoil_068E_RefObjPt_mc->Clone();

  TGraph_data_style (gNjetsRecoil_068E_RefObjPt_data);
  TGraph_mc_style (gNjetsRecoil_068E_RefObjPt_mc);
  TGraph_mc_style (gNjetsRecoil_068E_RefObjPt_mc_clone);

  gNjetsRecoil_068E_RefObjPt_mc_clone->SetFillColor(17);
  //gNjetsRecoil_068E_RefObjPt_mc_clone->SetFillStyle(3002);
  gNjetsRecoil_068E_RefObjPt_mc->SetLineColor(17);

  TMultiGraph *mgNjetsRecoil_068E_RefObjPt = new TMultiGraph();
  mgNjetsRecoil_068E_RefObjPt->Add(gNjetsRecoil_068E_RefObjPt_mc_clone,"e3");
  mgNjetsRecoil_068E_RefObjPt->Add(gNjetsRecoil_068E_RefObjPt_mc,"p");
  mgNjetsRecoil_068E_RefObjPt->Add(gNjetsRecoil_068E_RefObjPt_data,"pe");
  if(useRecoilPtBin) {
    mgNjetsRecoil_068E_RefObjPt->SetTitle("Number of necessary recoil jets to carry 68% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 68% p_{T}^{Recoil}}");
  }
  else {
    mgNjetsRecoil_068E_RefObjPt->SetTitle("Number of necessary recoil jets to carry 68% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 68% p_{T}^{Recoil}}");
  }

  TGraphErrors *gNjetsRecoil_068E_RefObjPt_ratio = NULL;
  if(useRecoilPtBin) {
    gNjetsRecoil_068E_RefObjPt_ratio = getDataMcResponseRatio(gNjetsRecoil_068E_RefObjPt_data,gNjetsRecoil_068E_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
    gNjetsRecoil_068E_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gNjetsRecoil_068E_RefObjPt_ratio = getDataMcResponseRatio(gNjetsRecoil_068E_RefObjPt_data,gNjetsRecoil_068E_RefObjPt_mc,numberPtBins, "p_{T}^{Leading Jet} [GeV]");
    gNjetsRecoil_068E_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");  
  }
  gNjetsRecoil_068E_RefObjPt_ratio->GetYaxis()->SetTitle("N_{jets in Recoil, 68%}^{data}/N_{jets in Recoil,68%}^{MC}");
  gNjetsRecoil_068E_RefObjPt_ratio->SetName("Data/MC");
  gNjetsRecoil_068E_RefObjPt_ratio->SetTitle("Data/MC");
  gNjetsRecoil_068E_RefObjPt_ratio->SetMarkerSize(1.0);
  gNjetsRecoil_068E_RefObjPt_ratio->Fit("func","","",gNjetsRecoil_068E_RefObjPt_data->GetXaxis()->GetXmin(),gNjetsRecoil_068E_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/NjetsRecoil/NjetsRecoil_068E_RefObjPt" + extension;	
  drawComparisonResponse("r1", mgNjetsRecoil_068E_RefObjPt, gNjetsRecoil_068E_RefObjPt_mc, gNjetsRecoil_068E_RefObjPt_data, gNjetsRecoil_068E_RefObjPt_ratio,"MC", myHistoName.c_str(), false, false);

  //************************************************************************************************************
  //
  //                                      NjetsRecoil_068E as a function of ptrecoil without 2 ptbin
  //
  //************************************************************************************************************

  Double_t aNjetsRecoil_068E_RefObjPt_data_Mean[numberPtBins-2];
  Double_t aNjetsRecoil_068E_RefObjPt_data_MeanError[numberPtBins-2];
  Double_t aNjetsRecoil_068E_RefObjPt_mc_Mean[numberPtBins-2];
  Double_t aNjetsRecoil_068E_RefObjPt_mc_MeanError[numberPtBins-2];
  Double_t aNjetsRecoil_068ERatio_Mean[numberPtBins-2];
  Double_t aNjetsRecoil_068ERatio_MeanError[numberPtBins-2];

  for(int i=1; i<numberPtBins-1; i++) {
    gNjetsRecoil_068E_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_068E_RefObjPt_data_Mean[i-1]);
    gNjetsRecoil_068E_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_068E_RefObjPt_mc_Mean[i-1]);
    gNjetsRecoil_068E_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_068ERatio_Mean[i-1]);
    aNjetsRecoil_068E_RefObjPt_data_MeanError[i-1] = gNjetsRecoil_068E_RefObjPt_data->GetErrorY(i);
    aNjetsRecoil_068E_RefObjPt_mc_MeanError[i-1] = gNjetsRecoil_068E_RefObjPt_mc->GetErrorY(i);
    aNjetsRecoil_068ERatio_MeanError[i-1] = gNjetsRecoil_068E_RefObjPt_ratio->GetErrorY(i);
  }

  TGraphErrors* gNjetsRecoil_068E_RefObjPt_data_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aNjetsRecoil_068E_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_068E_RefObjPt_data_MeanError);
  TGraphErrors* gNjetsRecoil_068E_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aNjetsRecoil_068E_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_068E_RefObjPt_mc_MeanError);	
  TGraph* gNjetsRecoil_068E_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-2,aRefObjPtBins_Mean, aNjetsRecoil_068E_RefObjPt_mc_Mean);	

  TGraphErrors* gNjetsRecoil_068E_RefObjPt_mc_clone_resize = (TGraphErrors*)gNjetsRecoil_068E_RefObjPt_mc_resize->Clone();

  TGraph_data_style (gNjetsRecoil_068E_RefObjPt_data_resize);
  TGraph_mc_style (gNjetsRecoil_068E_RefObjPt_mc_resize);
  TGraph_mc_style (gNjetsRecoil_068E_RefObjPt_mc_resize_pointsOnly);
  TGraph_mc_style (gNjetsRecoil_068E_RefObjPt_mc_clone_resize);

  gNjetsRecoil_068E_RefObjPt_mc_clone_resize->SetFillColor(17);
  //gNjetsRecoil_068E_RefObjPt_mc_resize->SetFillStyle(3002);
  gNjetsRecoil_068E_RefObjPt_mc_resize->SetFillColor(17);

  TMultiGraph *mgNjetsRecoil_068E_RefObjPt_resize = new TMultiGraph();
  mgNjetsRecoil_068E_RefObjPt_resize->Add(gNjetsRecoil_068E_RefObjPt_mc_resize,"e3");
  mgNjetsRecoil_068E_RefObjPt_resize->Add(gNjetsRecoil_068E_RefObjPt_mc_resize_pointsOnly,"p");
  mgNjetsRecoil_068E_RefObjPt_resize->Add(gNjetsRecoil_068E_RefObjPt_data_resize,"pe");
  if(useRecoilPtBin) {
    mgNjetsRecoil_068E_RefObjPt_resize->SetTitle("Number of necessary recoil jets to carry 68% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 68% p_{T}^{Recoil}}");
  }
  else {
    mgNjetsRecoil_068E_RefObjPt_resize->SetTitle("Number of necessary recoil jets to carry 68% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 68% p_{T}^{Recoil}}");
  }

  mgNjetsRecoil_068E_RefObjPt_resize->SetMinimum(1.8);
  mgNjetsRecoil_068E_RefObjPt_resize->SetMaximum(2.2);

  TGraphErrors *gNjetsRecoil_068E_RefObjPt_ratio_resize = NULL;
  if(useRecoilPtBin) {
    gNjetsRecoil_068E_RefObjPt_ratio_resize = getDataMcResponseRatio(gNjetsRecoil_068E_RefObjPt_data_resize,gNjetsRecoil_068E_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Recoil} [GeV]");
    gNjetsRecoil_068E_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gNjetsRecoil_068E_RefObjPt_ratio_resize = getDataMcResponseRatio(gNjetsRecoil_068E_RefObjPt_data_resize,gNjetsRecoil_068E_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Leading Jet} [GeV]");
    gNjetsRecoil_068E_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  }
  gNjetsRecoil_068E_RefObjPt_ratio_resize->GetYaxis()->SetTitle("N_{jets in Recoil, 68%}^{data}/N_{jets in Recoil, 68%}^{MC}");
  gNjetsRecoil_068E_RefObjPt_ratio_resize->SetName("Data/MC");
  gNjetsRecoil_068E_RefObjPt_ratio_resize->SetTitle("Data/MC");
  gNjetsRecoil_068E_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gNjetsRecoil_068E_RefObjPt_ratio_resize->Fit("func","","",gNjetsRecoil_068E_RefObjPt_data_resize->GetXaxis()->GetXmin(),gNjetsRecoil_068E_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/NjetsRecoil/NjetsRecoil_068E_RefObjPt_resize" + extension;	
  drawComparisonResponse("r1", mgNjetsRecoil_068E_RefObjPt_resize, gNjetsRecoil_068E_RefObjPt_mc_resize, gNjetsRecoil_068E_RefObjPt_data_resize, gNjetsRecoil_068E_RefObjPt_ratio_resize,"MC", myHistoName.c_str(), false, false);



  //************************************************************************************************************
  //
  //                                      NjetsRecoil_095E as a function of reference object pt 
  //
  //************************************************************************************************************

  TGraphErrors* gNjetsRecoil_095E_RefObjPt_data=(TGraphErrors*)f_data->Get("recoilJets/gNjetsRecoil_095E_RecoilPt");
  TGraphErrors* gNjetsRecoil_095E_RefObjPt_mc=(TGraphErrors*)f_mc->Get("recoilJets/gNjetsRecoil_095E_RecoilPt");

  TGraphErrors* gNjetsRecoil_095E_RefObjPt_mc_clone = (TGraphErrors*)gNjetsRecoil_095E_RefObjPt_mc->Clone();

  TGraph_data_style (gNjetsRecoil_095E_RefObjPt_data);
  TGraph_mc_style (gNjetsRecoil_095E_RefObjPt_mc);
  TGraph_mc_style (gNjetsRecoil_095E_RefObjPt_mc_clone);

  gNjetsRecoil_095E_RefObjPt_mc_clone->SetFillColor(17);
  //gNjetsRecoil_095E_RefObjPt_mc_clone->SetFillStyle(3002);
  gNjetsRecoil_095E_RefObjPt_mc->SetLineColor(17);

  TMultiGraph *mgNjetsRecoil_095E_RefObjPt = new TMultiGraph();
  mgNjetsRecoil_095E_RefObjPt->Add(gNjetsRecoil_095E_RefObjPt_mc_clone,"e3");
  mgNjetsRecoil_095E_RefObjPt->Add(gNjetsRecoil_095E_RefObjPt_mc,"p");
  mgNjetsRecoil_095E_RefObjPt->Add(gNjetsRecoil_095E_RefObjPt_data,"pe");
  if(useRecoilPtBin) {
    mgNjetsRecoil_095E_RefObjPt->SetTitle("Number of necessary recoil jets to carry 95% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 95% p_{T}^{Recoil}}");
  }
  else {
    mgNjetsRecoil_095E_RefObjPt->SetTitle("Number of necessary recoil jets to carry 95% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 95% p_{T}^{Recoil}}");
  }

  TGraphErrors *gNjetsRecoil_095E_RefObjPt_ratio = NULL;
  if(useRecoilPtBin) {
    gNjetsRecoil_095E_RefObjPt_ratio = getDataMcResponseRatio(gNjetsRecoil_095E_RefObjPt_data,gNjetsRecoil_095E_RefObjPt_mc,numberPtBins, "p_{T}^{Recoil} [GeV]");
    gNjetsRecoil_095E_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gNjetsRecoil_095E_RefObjPt_ratio = getDataMcResponseRatio(gNjetsRecoil_095E_RefObjPt_data,gNjetsRecoil_095E_RefObjPt_mc,numberPtBins, "p_{T}^{Leading Jet} [GeV]");
    gNjetsRecoil_095E_RefObjPt_ratio->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");  
  }
  gNjetsRecoil_095E_RefObjPt_ratio->GetYaxis()->SetTitle("N_{jets in Recoil, 95%}^{data}/N_{jets in Recoil,95%}^{MC}");
  gNjetsRecoil_095E_RefObjPt_ratio->SetName("Data/MC");
  gNjetsRecoil_095E_RefObjPt_ratio->SetTitle("Data/MC");
  gNjetsRecoil_095E_RefObjPt_ratio->SetMarkerSize(1.0);
  gNjetsRecoil_095E_RefObjPt_ratio->Fit("func","","",gNjetsRecoil_095E_RefObjPt_data->GetXaxis()->GetXmin(),gNjetsRecoil_095E_RefObjPt_data->GetXaxis()->GetXmax());

  myHistoName = "images/NjetsRecoil/NjetsRecoil_095E_RefObjPt" + extension;	
  drawComparisonResponse("r1", mgNjetsRecoil_095E_RefObjPt, gNjetsRecoil_095E_RefObjPt_mc, gNjetsRecoil_095E_RefObjPt_data, gNjetsRecoil_095E_RefObjPt_ratio,"MC", myHistoName.c_str(), false, false);

  //************************************************************************************************************
  //
  //                                      NjetsRecoil_095E as a function of ptrecoil without 2 ptbin
  //
  //************************************************************************************************************

  Double_t aNjetsRecoil_095E_RefObjPt_data_Mean[numberPtBins-2];
  Double_t aNjetsRecoil_095E_RefObjPt_data_MeanError[numberPtBins-2];
  Double_t aNjetsRecoil_095E_RefObjPt_mc_Mean[numberPtBins-2];
  Double_t aNjetsRecoil_095E_RefObjPt_mc_MeanError[numberPtBins-2];
  Double_t aNjetsRecoil_095ERatio_Mean[numberPtBins-2];
  Double_t aNjetsRecoil_095ERatio_MeanError[numberPtBins-2];

  for(int i=1; i<numberPtBins-1; i++) {
    gNjetsRecoil_095E_RefObjPt_data->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_095E_RefObjPt_data_Mean[i-1]);
    gNjetsRecoil_095E_RefObjPt_mc->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_095E_RefObjPt_mc_Mean[i-1]);
    gNjetsRecoil_095E_RefObjPt_ratio->GetPoint(i,aRefObjPtBins_Mean[i-1],aNjetsRecoil_095ERatio_Mean[i-1]);
    aNjetsRecoil_095E_RefObjPt_data_MeanError[i-1] = gNjetsRecoil_095E_RefObjPt_data->GetErrorY(i);
    aNjetsRecoil_095E_RefObjPt_mc_MeanError[i-1] = gNjetsRecoil_095E_RefObjPt_mc->GetErrorY(i);
    aNjetsRecoil_095ERatio_MeanError[i-1] = gNjetsRecoil_095E_RefObjPt_ratio->GetErrorY(i);
  }

  TGraphErrors* gNjetsRecoil_095E_RefObjPt_data_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aNjetsRecoil_095E_RefObjPt_data_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_095E_RefObjPt_data_MeanError);
  TGraphErrors* gNjetsRecoil_095E_RefObjPt_mc_resize = new TGraphErrors(numberPtBins-2,aRefObjPtBins_Mean, aNjetsRecoil_095E_RefObjPt_mc_Mean, aRefObjPtBins_MeanError, aNjetsRecoil_095E_RefObjPt_mc_MeanError);	
  TGraph* gNjetsRecoil_095E_RefObjPt_mc_resize_pointsOnly = new TGraph(numberPtBins-2,aRefObjPtBins_Mean, aNjetsRecoil_095E_RefObjPt_mc_Mean);	

  TGraphErrors* gNjetsRecoil_095E_RefObjPt_mc_clone_resize = (TGraphErrors*)gNjetsRecoil_095E_RefObjPt_mc_resize->Clone();

  TGraph_data_style (gNjetsRecoil_095E_RefObjPt_data_resize);
  TGraph_mc_style (gNjetsRecoil_095E_RefObjPt_mc_resize);
  TGraph_mc_style (gNjetsRecoil_095E_RefObjPt_mc_resize_pointsOnly);
  TGraph_mc_style (gNjetsRecoil_095E_RefObjPt_mc_clone_resize);

  gNjetsRecoil_095E_RefObjPt_mc_clone_resize->SetFillColor(17);
  //gNjetsRecoil_095E_RefObjPt_mc_resize->SetFillStyle(3002);
  gNjetsRecoil_095E_RefObjPt_mc_resize->SetFillColor(17);

  TMultiGraph *mgNjetsRecoil_095E_RefObjPt_resize = new TMultiGraph();
  mgNjetsRecoil_095E_RefObjPt_resize->Add(gNjetsRecoil_095E_RefObjPt_mc_resize,"e3");
  mgNjetsRecoil_095E_RefObjPt_resize->Add(gNjetsRecoil_095E_RefObjPt_mc_resize_pointsOnly,"p");
  mgNjetsRecoil_095E_RefObjPt_resize->Add(gNjetsRecoil_095E_RefObjPt_data_resize,"pe");
  if(useRecoilPtBin) {
    mgNjetsRecoil_095E_RefObjPt_resize->SetTitle("Number of necessary recoil jets to carry 95% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 95% p_{T}^{Recoil}}");
  }
  else {
    mgNjetsRecoil_095E_RefObjPt_resize->SetTitle("Number of necessary recoil jets to carry 95% of p_{T}^{Recoil} as a function of p_{T}^{Recoil} as a function of p_{T}^{Recoil};p_{T}^{Recoil} [GeV];N_{jets in Recoil}^{carrying 95% p_{T}^{Recoil}}");
  }

  mgNjetsRecoil_095E_RefObjPt_resize->SetMinimum(1.5);
  mgNjetsRecoil_095E_RefObjPt_resize->SetMaximum(3.5);

  TGraphErrors *gNjetsRecoil_095E_RefObjPt_ratio_resize = NULL;
  if(useRecoilPtBin) {
    gNjetsRecoil_095E_RefObjPt_ratio_resize = getDataMcResponseRatio(gNjetsRecoil_095E_RefObjPt_data_resize,gNjetsRecoil_095E_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Recoil} [GeV]");
    gNjetsRecoil_095E_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Recoil} [GeV]");
  }
  else {
    gNjetsRecoil_095E_RefObjPt_ratio_resize = getDataMcResponseRatio(gNjetsRecoil_095E_RefObjPt_data_resize,gNjetsRecoil_095E_RefObjPt_mc_resize,numberPtBins-2, "p_{T}^{Leading Jet} [GeV]");
    gNjetsRecoil_095E_RefObjPt_ratio_resize->GetXaxis()->SetTitle("p_{T}^{Leading Jet} [GeV]");
  }
  gNjetsRecoil_095E_RefObjPt_ratio_resize->GetYaxis()->SetTitle("N_{jets in Recoil, 95%}^{data}/N_{jets in Recoil, 95%}^{MC}");
  gNjetsRecoil_095E_RefObjPt_ratio_resize->SetName("Data/MC");
  gNjetsRecoil_095E_RefObjPt_ratio_resize->SetTitle("Data/MC");
  gNjetsRecoil_095E_RefObjPt_ratio_resize->SetMarkerSize(1.0);
  gNjetsRecoil_095E_RefObjPt_ratio_resize->Fit("func","","",gNjetsRecoil_095E_RefObjPt_data_resize->GetXaxis()->GetXmin(),gNjetsRecoil_095E_RefObjPt_data_resize->GetXaxis()->GetXmax());

  myHistoName = "images/NjetsRecoil/NjetsRecoil_095E_RefObjPt_resize" + extension;	
  drawComparisonResponse("r1", mgNjetsRecoil_095E_RefObjPt_resize, gNjetsRecoil_095E_RefObjPt_mc_resize, gNjetsRecoil_095E_RefObjPt_data_resize, gNjetsRecoil_095E_RefObjPt_ratio_resize,"MC", myHistoName.c_str(), false, false);



  //************************************************************************************************************
  //
  //                                      MJB as a function of p_{T}^{Recoil}*exp(sum_i[F_i * log(f_i)]) 
  //
  //************************************************************************************************************

  TGraphErrors* gMJB_LogPtPrimeBin_data=(TGraphErrors*)f_data->Get("MJB/LogPtPrimeBin/gMJB_LogPtPrimeBin_RecoilPt");
  TGraphErrors* gMJB_LogPtPrimeBin_mc=(TGraphErrors*)f_mc->Get("MJB/LogPtPrimeBin/gMJB_LogPtPrimeBin_RecoilPt");

  TGraphErrors* gMJB_LogPtPrimeBin_mc_clone = (TGraphErrors*)gMJB_LogPtPrimeBin_mc->Clone();

  TGraph_data_style (gMJB_LogPtPrimeBin_data);
  TGraph_mc_style (gMJB_LogPtPrimeBin_mc);
  TGraph_mc_style (gMJB_LogPtPrimeBin_mc_clone);

  gMJB_LogPtPrimeBin_mc_clone->SetFillColor(17);
  //gMJB_LogPtPrimeBin_mc_clone->SetFillStyle(3002);
  gMJB_LogPtPrimeBin_mc->SetLineColor(17);

  TMultiGraph *mgMJB_LogPtPrimeBin = new TMultiGraph();
  mgMJB_LogPtPrimeBin->Add(gMJB_LogPtPrimeBin_mc_clone,"e3");
  mgMJB_LogPtPrimeBin->Add(gMJB_LogPtPrimeBin_mc,"p");
  mgMJB_LogPtPrimeBin->Add(gMJB_LogPtPrimeBin_data,"pe");

  mgMJB_LogPtPrimeBin->SetTitle("MJB as a function of p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}));p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}));MJB");

  mgMJB_LogPtPrimeBin->SetMinimum(0.85);
  mgMJB_LogPtPrimeBin->SetMaximum(1.15);


  TGraphErrors *gMJB_LogPtPrimeBin_ratio = NULL;

  gMJB_LogPtPrimeBin_ratio = getDataMcResponseRatio(gMJB_LogPtPrimeBin_data,gMJB_LogPtPrimeBin_mc,numberLogPtPrimeBins, "p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}))");
  gMJB_LogPtPrimeBin_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}))");  
  gMJB_LogPtPrimeBin_ratio->GetYaxis()->SetTitle("MJB^{data}/MJB^{MC}");
  gMJB_LogPtPrimeBin_ratio->SetName("Data/MC");
  gMJB_LogPtPrimeBin_ratio->SetTitle("Data/MC");
  gMJB_LogPtPrimeBin_ratio->SetMarkerSize(1.0);
  gMJB_LogPtPrimeBin_ratio->Fit("func","","",gMJB_LogPtPrimeBin_data->GetXaxis()->GetXmin(),gMJB_LogPtPrimeBin_data->GetXaxis()->GetXmax());

  myHistoName = "images/response/MJB_LogPtPrimeBin" + extension;	
  drawComparisonResponse("r1", mgMJB_LogPtPrimeBin, gMJB_LogPtPrimeBin_mc, gMJB_LogPtPrimeBin_data, gMJB_LogPtPrimeBin_ratio,"MC", myHistoName.c_str(), false, false);

  //************************************************************************************************************
  //
  //                                      InvMJB as a function of p_{T}^{Recoil}*exp(sum_i[F_i * log(f_i)]) 
  //
  //************************************************************************************************************

  TGraphErrors* gInvMJB_LogPtPrimeBin_data=(TGraphErrors*)f_data->Get("MJB/LogPtPrimeBin/gInvMJB_LogPtPrimeBin_RecoilPt");
  TGraphErrors* gInvMJB_LogPtPrimeBin_mc=(TGraphErrors*)f_mc->Get("MJB/LogPtPrimeBin/gInvMJB_LogPtPrimeBin_RecoilPt");

  TGraphErrors* gInvMJB_LogPtPrimeBin_mc_clone = (TGraphErrors*)gInvMJB_LogPtPrimeBin_mc->Clone();

  TGraph_data_style (gInvMJB_LogPtPrimeBin_data);
  TGraph_mc_style (gInvMJB_LogPtPrimeBin_mc);
  TGraph_mc_style (gInvMJB_LogPtPrimeBin_mc_clone);

  gInvMJB_LogPtPrimeBin_mc_clone->SetFillColor(17);
  //gInvMJB_LogPtPrimeBin_mc_clone->SetFillStyle(3002);
  gInvMJB_LogPtPrimeBin_mc->SetLineColor(17);

  TMultiGraph *mgInvMJB_LogPtPrimeBin = new TMultiGraph();
  mgInvMJB_LogPtPrimeBin->Add(gInvMJB_LogPtPrimeBin_mc_clone,"e3");
  mgInvMJB_LogPtPrimeBin->Add(gInvMJB_LogPtPrimeBin_mc,"p");
  mgInvMJB_LogPtPrimeBin->Add(gInvMJB_LogPtPrimeBin_data,"pe");

  mgInvMJB_LogPtPrimeBin->SetTitle("MJB^{-1} as a function of p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}));p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}));MJB^{-1}");

  mgInvMJB_LogPtPrimeBin->SetMinimum(0.85);
  mgInvMJB_LogPtPrimeBin->SetMaximum(1.15);


  TGraphErrors *gInvMJB_LogPtPrimeBin_ratio = NULL;

  gInvMJB_LogPtPrimeBin_ratio = getDataMcResponseRatio(gInvMJB_LogPtPrimeBin_data,gInvMJB_LogPtPrimeBin_mc,numberLogPtPrimeBins, "p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}))");
  gInvMJB_LogPtPrimeBin_ratio->GetXaxis()->SetTitle("p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}))");  
  gInvMJB_LogPtPrimeBin_ratio->GetYaxis()->SetTitle("MJB^{-1}_{data}/MJB^{-1}_{MC}");
  gInvMJB_LogPtPrimeBin_ratio->SetName("Data/MC");
  gInvMJB_LogPtPrimeBin_ratio->SetTitle("Data/MC");
  gInvMJB_LogPtPrimeBin_ratio->SetMarkerSize(1.0);
  gInvMJB_LogPtPrimeBin_ratio->Fit("func","","",gInvMJB_LogPtPrimeBin_data->GetXaxis()->GetXmin(),gInvMJB_LogPtPrimeBin_data->GetXaxis()->GetXmax());

  myHistoName = "images/response/InvMJB_LogPtPrimeBin" + extension;	
  drawComparisonResponse("r1", mgInvMJB_LogPtPrimeBin, gInvMJB_LogPtPrimeBin_mc, gInvMJB_LogPtPrimeBin_data, gInvMJB_LogPtPrimeBin_ratio,"MC", myHistoName.c_str(), false, false);



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

  TH1F* hLeadingJetMass_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hLeadingJetMass_afterSel");
  TH1F* hLeadingJetMass_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hLeadingJetMass_afterSel");

  TH1F* hLeadingAndSecondJetsMass_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hLeadingAndSecondJetsMass_afterSel");
  TH1F* hLeadingAndSecondJetsMass_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hLeadingAndSecondJetsMass_afterSel");

/*  TH1F* hSecondJetPt_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hSecondJetPt_beforeSel");*/
  //TH1F* hSecondJetPt_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hSecondJetPt_beforeSel");

  //TH1F* hSecondJetPt_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hSecondJetPt_afterSel");
  /*TH1F* hSecondJetPt_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hSecondJetPt_afterSel");*/

  TH1F* hJetsPt_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hJetsPt_beforeSel");
  TH1F* hJetsPt_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hJetsPt_beforeSel");

  TH1F* hJetsPt_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hJetsPt_afterSel");
  TH1F* hJetsPt_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hJetsPt_afterSel");

  TH1F* hJetsEta_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hJetsEta_beforeSel");
  TH1F* hJetsEta_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hJetsEta_beforeSel");

  TH1F* hJetsEta_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hJetsEta_afterSel");
  TH1F* hJetsEta_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hJetsEta_afterSel");

/*  TH1F* hLeadingJetEta_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hLeadingJetEta_beforeSel");*/
  //TH1F* hLeadingJetEta_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hLeadingJetEta_beforeSel");

  //TH1F* hLeadingJetEta_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hLeadingJetEta_afterSel");
  //TH1F* hLeadingJetEta_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hLeadingJetEta_afterSel");

  //TH1F* hRecoilEta_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hRecoilEta_beforeSel");
  //TH1F* hRecoilEta_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hRecoilEta_beforeSel");

  //TH1F* hRecoilEta_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hRecoilEta_afterSel");
  /*TH1F* hRecoilEta_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hRecoilEta_afterSel");*/

  TH1F* hJetsPhi_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hJetsPhi_beforeSel");
  TH1F* hJetsPhi_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hJetsPhi_beforeSel");

  TH1F* hJetsPhi_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hJetsPhi_afterSel");
  TH1F* hJetsPhi_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hJetsPhi_afterSel");

  TH1F* hNjets_ptSup30_etaInf5_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hNjets_ptSup30_etaInf5_beforeSel");
  TH1F* hNjets_ptSup30_etaInf5_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hNjets_ptSup30_etaInf5_beforeSel");

  TH1F* hHT_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hHT_afterSel");
  TH1F* hHT_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hHT_afterSel");	

  TH1F* hHT_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hHT_beforeSel");
  TH1F* hHT_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hHT_beforeSel");	

  TH1F* hRecoilPt_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hRecoilPt_beforeSel");
  TH1F* hRecoilPt_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hRecoilPt_beforeSel");

  TH1F* hRecoilPt_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hRecoilPt_afterSel");
  TH1F* hRecoilPt_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hRecoilPt_afterSel");

  TH1F* hNpv_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hNpv_beforeSel");
  TH1F* hNpv_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hNpv_beforeSel");

  TH1F* hNpv_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hNpv_afterSel");
  TH1F* hNpv_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hNpv_afterSel");

  TH1F* hNjetsRecoil_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hNjetsRecoil");
  TH1F* hNjetsRecoil_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hNjetsRecoil");

  TH1F* hAlpha_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hAlpha_beforeSel");
  TH1F* hAlpha_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hAlpha_beforeSel");

  TH1F* hAlpha_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hAlpha_afterSel");
  TH1F* hAlpha_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hAlpha_afterSel");

  TH1F* hBeta_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hBeta_beforeSel");
  hBeta_beforeSel_mc_lumi->GetXaxis()->SetRangeUser(0.,3.15);
  TH1F* hBeta_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hBeta_beforeSel");
  hBeta_beforeSel_data_lumi->GetXaxis()->SetRangeUser(0.,3.15);;

  TH1F* hBeta_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hBeta_afterSel");
  hBeta_afterSel_mc_lumi->GetXaxis()->SetRangeUser(0.,3.15);
  TH1F* hBeta_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hBeta_afterSel");
  hBeta_afterSel_data_lumi->GetXaxis()->SetRangeUser(0.,3.15);;

  TH1F* hA_beforeSel_mc_lumi=(TH1F*)f_mc->Get("variables/beforeSel/hA_beforeSel");
  TH1F* hA_beforeSel_data_lumi=(TH1F*)f_data->Get("variables/beforeSel/hA_beforeSel");

  TH1F* hA_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hA_afterSel");
  TH1F* hA_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hA_afterSel");

  TH1F* hDeltaPhi_METJet1_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hDeltaPhi_METJet1_afterSel");
  TH1F* hDeltaPhi_METJet1_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hDeltaPhi_METJet1_afterSel");

  TH1F* hDeltaPhi_METRecoil_afterSel_mc_lumi=(TH1F*)f_mc->Get("variables/afterSel/hDeltaPhi_METRecoil_afterSel");
  TH1F* hDeltaPhi_METRecoil_afterSel_data_lumi=(TH1F*)f_data->Get("variables/afterSel/hDeltaPhi_METRecoil_afterSel");

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
      h1_style(vMJB_RefObjPt_data_lumi[j]);
      h1_style(vMJB_RefObjPt_mc_lumi[j]);
      vMJB_RefObjPt_mc_lumi[j]->Scale(getLumi());
      binName = myPtBinning.getName(j);
      myName = "MJB_{data}/MJB_{MC} for " + binName;
      //myXName = "MJB for " + binName;
      std::stringstream PtBinningName ;
      PtBinningName.str("");
      PtBinningName << "MJB response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
      myXName = PtBinningName.str();
      mySaveName = "images/MJBperRefObjPt/MJB_" + binName + "_lumi_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_lumi[j], vMJB_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
      mySaveName =  "images/MJBperRefObjPt/MJB_" + binName + "_lumi_inLogScale_woRatio" + extension;
      drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_lumi[j], vMJB_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), false, "", "r", 0);
    }

    //************************************************************************************************************
    //
    //                                      MPF per ptrecoil 
    //
    //************************************************************************************************************	

    for(int j=0; j<myPtBinning.getSize(); j++) {
      h1_style(vMPF_RefObjPt_data_lumi[j]);
      h1_style(vMPF_RefObjPt_mc_lumi[j]);
      vMPF_RefObjPt_mc_lumi[j]->Scale(getLumi());
      binName = myPtBinning.getName(j);
      myName = "MPF_{data}/MPF_{MC} for " + binName;
      //myXName = "MPF for " + binName;
      std::stringstream PtBinningName ;
      PtBinningName.str("");
      PtBinningName << "MPF response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
      myXName = PtBinningName.str();
      mySaveName = "images/MPFperRefObjPt/MPF_" + binName + "_lumi_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vMPF_RefObjPt_mc_lumi[j], vMPF_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
      mySaveName =  "images/MPFperRefObjPt/MPF_" + binName + "_lumi_inLogScale_woRatio" + extension;
      drawDataMcComparison(myName.c_str(), vMPF_RefObjPt_mc_lumi[j], vMPF_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), false, "", "r", 0);
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
      myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
      myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
      mySaveName = "images/LeadingJetPtperRefObjPt/LeadingJetPt_" + binName + "_lumi_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_lumi[j], vLeadingJetPt_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
    }

    //************************************************************************************************************
    //
    //                                      LeadingJetPt per HLT pt bin 
    //
    //************************************************************************************************************	

    for(int j=0; j<myHLTPtBinning.getSize(); j++) {
      h1_style(vLeadingJetPt_RefObjPtHLT_data_lumi[j]);
      h1_style(vLeadingJetPt_RefObjPtHLT_mc_lumi[j]);
      vLeadingJetPt_RefObjPtHLT_mc_lumi[j]->Scale(getLumi());
      binName = myHLTPtBinning.getName(j);
      myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
      myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
      mySaveName = "images/LeadingJetPtperPtHLT/LeadingJetPt_" + binName + "_lumi_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vLeadingJetPt_RefObjPtHLT_mc_lumi[j], vLeadingJetPt_RefObjPtHLT_data_lumi[j], myXName.c_str(), mySaveName.c_str());
    }

    //************************************************************************************************************
    //
    //                                      RecoilPt per reference object pt 
    //
    //************************************************************************************************************	

    for(int j=0; j<myPtBinning.getSize(); j++) {
      h1_style(vRecoilPt_RefObjPt_data_lumi[j]);
      h1_style(vRecoilPt_RefObjPt_mc_lumi[j]);
      vRecoilPt_RefObjPt_mc_lumi[j]->Scale(getLumi());
      binName = myPtBinning.getName(j);
      myName = "pT^{Recoil}_{data}/pT^{Recoil}_{MC} for " + binName;
      myXName = "pT^{Recoil} for " + binName + " [GeV]";
      mySaveName = "images/RecoilPtperRefObjPt/RecoilPt_" + binName + "_lumi_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vRecoilPt_RefObjPt_mc_lumi[j], vRecoilPt_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
    }


    //************************************************************************************************************
    //
    //                                      MJB per reference object eta 
    //
    //************************************************************************************************************	

    for(int j=0; j<myEtaBinning.getSize(); j++) {
      h1_style(vMJB_RefObjEta_data_lumi[j]);
      h1_style(vMJB_RefObjEta_mc_lumi[j]);
      vMJB_RefObjEta_mc_lumi[j]->Scale(getLumi());
      binName = myEtaBinning.getName(j);
      myName = "MJB_{data}/MJB_{MC} for " + binName;
      myXName = "MJB for " + binName;
      mySaveName = "images/MJBperRefObjEta/MJB_" + binName + "_lumi_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vMJB_RefObjEta_mc_lumi[j], vMJB_RefObjEta_data_lumi[j], myXName.c_str(), mySaveName.c_str());
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
    drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_lumi, hMet_beforeSel_data_lumi, "MET [GeV]", myHistoName.c_str());



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
    drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_lumi, hMet_afterSel_data_lumi, "MET [GeV]", myHistoName.c_str());



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
    drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_lumi, hLeadingJetPt_beforeSel_data_lumi, "p_{T}^{leading jet} [GeV]", myHistoName.c_str());




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
    drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_lumi, hLeadingJetPt_afterSel_data_lumi,"p_{T}^{leading jet} [GeV]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      LeadingJetMass afterSel 
    //
    //************************************************************************************************************	


    //h1_style(hLeadingJetMass_afterSel_mc_lumi);
    //h1_style(hLeadingJetMass_afterSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hLeadingJetMass_afterSel_mc_lumi->Scale(getLumi());
    //myHistoName = "images/variables/LeadingJetMass_afterSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("LeadingJetMass_afterSel", hLeadingJetMass_afterSel_mc_lumi, hLeadingJetMass_afterSel_data_lumi,"M^{leading jet} [GeV]", myHistoName.c_str());*/


    //************************************************************************************************************
    //
    //                                      LeadingAndSecondJetsMass afterSel 
    //
    //************************************************************************************************************	


/*    h1_style(hLeadingAndSecondJetsMass_afterSel_mc_lumi);*/
    //h1_style(hLeadingAndSecondJetsMass_afterSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hLeadingAndSecondJetsMass_afterSel_mc_lumi->Scale(getLumi());
    //myHistoName = "images/variables/LeadingAndSecondJetsMass_afterSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("LeadingAndSecondJetsMass_afterSel", hLeadingAndSecondJetsMass_afterSel_mc_lumi, hLeadingAndSecondJetsMass_afterSel_data_lumi,"M^{j_{1}+j_{2}} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      SecondJetPt beforeSel 
    //
    //************************************************************************************************************	


    //h1_style(hSecondJetPt_beforeSel_mc_lumi);
    //h1_style(hSecondJetPt_beforeSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hSecondJetPt_beforeSel_mc_lumi->Scale(getLumi());

    //myHistoName = "images/variables/SecondJetPt_beforeSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("SecondJetPt_beforeSel", hSecondJetPt_beforeSel_mc_lumi, hSecondJetPt_beforeSel_data_lumi, "p_{T}^{2^{nd} jet} [GeV]", myHistoName.c_str());*/




    //************************************************************************************************************
    //
    //                                      SecondJetPt afterSel 
    //
    //************************************************************************************************************	


/*    h1_style(hSecondJetPt_afterSel_mc_lumi);*/
    //h1_style(hSecondJetPt_afterSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hSecondJetPt_afterSel_mc_lumi->Scale(getLumi());
    //myHistoName = "images/variables/SecondJetPt_afterSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("SecondJetPt_afterSel", hSecondJetPt_afterSel_mc_lumi, hSecondJetPt_afterSel_data_lumi,"p_{T}^{2^{nd} jet} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      LeadingJetPt afterSel zoom 
    //
    //************************************************************************************************************	

    TH1* hLeadingJetPt_afterSel_mc_lumi_zoom = (TH1F*)hLeadingJetPt_afterSel_mc_lumi->Clone();
    TH1* hLeadingJetPt_afterSel_data_lumi_zoom = (TH1F*)hLeadingJetPt_afterSel_data_lumi->Clone();


    h1_style(hLeadingJetPt_afterSel_mc_lumi_zoom);
    h1_style(hLeadingJetPt_afterSel_data_lumi_zoom);

    hLeadingJetPt_afterSel_mc_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);
    hLeadingJetPt_afterSel_data_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);

    myHistoName = "images/variables/LeadingJetPt_afterSel_zoom_lumi_inLogScale" + extension;
    drawDataMcComparison("LeadingJetPt_afterSel_zoom", hLeadingJetPt_afterSel_mc_lumi_zoom, hLeadingJetPt_afterSel_data_lumi_zoom,"p_{T}^{leading jet} [GeV]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      LeadingJetMass afterSel zoom 
    //
    //************************************************************************************************************	

    //TH1* hLeadingJetMass_afterSel_mc_lumi_zoom = (TH1F*)hLeadingJetMass_afterSel_mc_lumi->Clone();
    //TH1* hLeadingJetMass_afterSel_data_lumi_zoom = (TH1F*)hLeadingJetMass_afterSel_data_lumi->Clone();


    //h1_style(hLeadingJetMass_afterSel_mc_lumi_zoom);
    //h1_style(hLeadingJetMass_afterSel_data_lumi_zoom);

    //hLeadingJetMass_afterSel_mc_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);
    //hLeadingJetMass_afterSel_data_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);

    //myHistoName = "images/variables/LeadingJetMass_afterSel_zoom_lumi_inLogScale" + extension;
    /*drawDataMcComparison("LeadingJetMass_afterSel_zoom", hLeadingJetMass_afterSel_mc_lumi_zoom, hLeadingJetMass_afterSel_data_lumi_zoom,"M^{leading jet} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      LeadingAndSecondJetsMass afterSel zoom 
    //
    //************************************************************************************************************	

 /*   TH1* hLeadingAndSecondJetsMass_afterSel_mc_lumi_zoom = (TH1F*)hLeadingAndSecondJetsMass_afterSel_mc_lumi->Clone();*/
    //TH1* hLeadingAndSecondJetsMass_afterSel_data_lumi_zoom = (TH1F*)hLeadingAndSecondJetsMass_afterSel_data_lumi->Clone();


    //h1_style(hLeadingAndSecondJetsMass_afterSel_mc_lumi_zoom);
    //h1_style(hLeadingAndSecondJetsMass_afterSel_data_lumi_zoom);

    //hLeadingAndSecondJetsMass_afterSel_mc_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);
    //hLeadingAndSecondJetsMass_afterSel_data_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);

    //myHistoName = "images/variables/LeadingAndSecondJetsMass_afterSel_zoom_lumi_inLogScale" + extension;
    /*drawDataMcComparison("LeadingAndSecondJetsMass_afterSel_zoom", hLeadingAndSecondJetsMass_afterSel_mc_lumi_zoom, hLeadingAndSecondJetsMass_afterSel_data_lumi_zoom,"M^{j_{1}+j_{2}} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      JetsPt beforeSel 
    //
    //************************************************************************************************************	


    h1_style(hJetsPt_beforeSel_mc_lumi);
    h1_style(hJetsPt_beforeSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hJetsPt_beforeSel_mc_lumi->Scale(getLumi());

    myHistoName = "images/variables/JetsPt_beforeSel_lumi_inLogScale" + extension;
    drawDataMcComparison("JetsPt_beforeSel", hJetsPt_beforeSel_mc_lumi, hJetsPt_beforeSel_data_lumi, "p_{T}^{jet} [GeV]", myHistoName.c_str());



    //************************************************************************************************************
    //
    //                                      JetsPt afterSel 
    //
    //************************************************************************************************************	


    h1_style(hJetsPt_afterSel_mc_lumi);
    h1_style(hJetsPt_afterSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hJetsPt_afterSel_mc_lumi->Scale(getLumi());
    myHistoName = "images/variables/JetsPt_afterSel_lumi_inLogScale" + extension;
    drawDataMcComparison("JetsPt_afterSel", hJetsPt_afterSel_mc_lumi, hJetsPt_afterSel_data_lumi,"p_{T}^{jet} [GeV]", myHistoName.c_str());


    //************************************************************************************************************
    //
    //                                      JetsEta beforeSel 
    //
    //************************************************************************************************************	


    h1_style(hJetsEta_beforeSel_mc_lumi);
    h1_style(hJetsEta_beforeSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hJetsEta_beforeSel_mc_lumi->Scale(getLumi());

    myHistoName = "images/variables/JetsEta_beforeSel_lumi_inLogScale" + extension;
    drawDataMcComparison("JetsEta_beforeSel", hJetsEta_beforeSel_mc_lumi, hJetsEta_beforeSel_data_lumi, "#eta^{jet} [rad]", myHistoName.c_str());




    //************************************************************************************************************
    //
    //                                      JetsEta afterSel 
    //
    //************************************************************************************************************	


    h1_style(hJetsEta_afterSel_mc_lumi);
    h1_style(hJetsEta_afterSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hJetsEta_afterSel_mc_lumi->Scale(getLumi());
    myHistoName = "images/variables/JetsEta_afterSel_lumi_inLogScale" + extension;
    drawDataMcComparison("JetsEta_afterSel", hJetsEta_afterSel_mc_lumi, hJetsEta_afterSel_data_lumi,"#eta^{jet} [rad]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      LeadingJetEta beforeSel 
    //
    //************************************************************************************************************	


/*    h1_style(hLeadingJetEta_beforeSel_mc_lumi);*/
    //h1_style(hLeadingJetEta_beforeSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hLeadingJetEta_beforeSel_mc_lumi->Scale(getLumi());

    //myHistoName = "images/variables/LeadingJetEta_beforeSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("LeadingJetEta_beforeSel", hLeadingJetEta_beforeSel_mc_lumi, hLeadingJetEta_beforeSel_data_lumi, "#eta^{leading jet} [rad]", myHistoName.c_str());*/




    //************************************************************************************************************
    //
    //                                      LeadingJetEta afterSel 
    //
    //************************************************************************************************************	


    //h1_style(hLeadingJetEta_afterSel_mc_lumi);
    //h1_style(hLeadingJetEta_afterSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hLeadingJetEta_afterSel_mc_lumi->Scale(getLumi());
    //myHistoName = "images/variables/LeadingJetEta_afterSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("LeadingJetEta_afterSel", hLeadingJetEta_afterSel_mc_lumi, hLeadingJetEta_afterSel_data_lumi,"#eta^{leading jet} [rad]", myHistoName.c_str());*/


    //************************************************************************************************************
    //
    //                                      RecoilEta beforeSel 
    //
    //************************************************************************************************************	


    //h1_style(hRecoilEta_beforeSel_mc_lumi);
    //h1_style(hRecoilEta_beforeSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hRecoilEta_beforeSel_mc_lumi->Scale(getLumi());

    //myHistoName = "images/variables/RecoilEta_beforeSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("RecoilEta_beforeSel", hRecoilEta_beforeSel_mc_lumi, hRecoilEta_beforeSel_data_lumi, "#eta^{recoil} [rad]", myHistoName.c_str());*/




    //************************************************************************************************************
    //
    //                                      RecoilEta afterSel 
    //
    //************************************************************************************************************	


/*    h1_style(hRecoilEta_afterSel_mc_lumi);*/
    //h1_style(hRecoilEta_afterSel_data_lumi);

    ////rescale the Monte Carlo histogramm with luminosity
    //hRecoilEta_afterSel_mc_lumi->Scale(getLumi());
    //myHistoName = "images/variables/RecoilEta_afterSel_lumi_inLogScale" + extension;
    /*drawDataMcComparison("RecoilEta_afterSel", hRecoilEta_afterSel_mc_lumi, hRecoilEta_afterSel_data_lumi,"#eta^{recoil} [rad]", myHistoName.c_str());*/


    //************************************************************************************************************
    //
    //                                      JetsPhi beforeSel 
    //
    //************************************************************************************************************	


    h1_style(hJetsPhi_beforeSel_mc_lumi);
    h1_style(hJetsPhi_beforeSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hJetsPhi_beforeSel_mc_lumi->Scale(getLumi());

    myHistoName = "images/variables/JetsPhi_beforeSel_lumi_inLogScale" + extension;
    drawDataMcComparison("JetsPhi_beforeSel", hJetsPhi_beforeSel_mc_lumi, hJetsPhi_beforeSel_data_lumi, "#phi^{jet} [rad]", myHistoName.c_str());




    //************************************************************************************************************
    //
    //                                      JetsPhi afterSel 
    //
    //************************************************************************************************************	


    h1_style(hJetsPhi_afterSel_mc_lumi);
    h1_style(hJetsPhi_afterSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hJetsPhi_afterSel_mc_lumi->Scale(getLumi());
    myHistoName = "images/variables/JetsPhi_afterSel_lumi_inLogScale" + extension;
    drawDataMcComparison("JetsPhi_afterSel", hJetsPhi_afterSel_mc_lumi, hJetsPhi_afterSel_data_lumi,"#phi^{jet} [rad]", myHistoName.c_str());


    //************************************************************************************************************
    //
    //                                      Njets_ptSup30_etaInf5 beforeSel 
    //
    //************************************************************************************************************	


    h1_style(hNjets_ptSup30_etaInf5_beforeSel_mc_lumi);
    h1_style(hNjets_ptSup30_etaInf5_beforeSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hNjets_ptSup30_etaInf5_beforeSel_mc_lumi->Scale(getLumi());

    myHistoName = "images/variables/Njets_ptSup30_etaInf5_beforeSel_lumi_inLogScale" + extension;
    drawDataMcComparison("Njets_ptSup30_etaInf5_beforeSel", hNjets_ptSup30_etaInf5_beforeSel_mc_lumi, hNjets_ptSup30_etaInf5_beforeSel_data_lumi, "N_{jets} with p_{T} > 25 GeV and |#eta| < 5.0", myHistoName.c_str());




    //************************************************************************************************************
    //
    //                                      HT afterSel 
    //
    //************************************************************************************************************	


    h1_style(hHT_afterSel_mc_lumi);
    h1_style(hHT_afterSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hHT_afterSel_mc_lumi->Scale(getLumi());
    myHistoName = "images/variables/HT_afterSel_lumi_inLogScale" + extension;
    drawDataMcComparison("HT_afterSel", hHT_afterSel_mc_lumi, hHT_afterSel_data_lumi,"HT [GeV]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      HT beforeSel 
    //
    //************************************************************************************************************	


    h1_style(hHT_beforeSel_mc_lumi);
    h1_style(hHT_beforeSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hHT_beforeSel_mc_lumi->Scale(getLumi());
    myHistoName = "images/variables/HT_beforeSel_lumi_inLogScale" + extension;
    drawDataMcComparison("HT_beforeSel", hHT_beforeSel_mc_lumi, hHT_beforeSel_data_lumi,"HT [GeV]", myHistoName.c_str());


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
    drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_lumi, hRecoilPt_beforeSel_data_lumi, "p_{T}^{Recoil} [GeV]", myHistoName.c_str());



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
    drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_lumi, hRecoilPt_afterSel_data_lumi,"p_{T}^{Recoil} [GeV]", myHistoName.c_str());


    //************************************************************************************************************
    //
    //                                      RecoilPt afterSel zoom 
    //
    //************************************************************************************************************	

    TH1* hRecoilPt_afterSel_mc_lumi_zoom = (TH1F*)hRecoilPt_afterSel_mc_lumi->Clone();
    TH1* hRecoilPt_afterSel_data_lumi_zoom = (TH1F*)hRecoilPt_afterSel_data_lumi->Clone();


    h1_style(hRecoilPt_afterSel_mc_lumi_zoom);
    h1_style(hRecoilPt_afterSel_data_lumi_zoom);

    hRecoilPt_afterSel_mc_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);
    hRecoilPt_afterSel_data_lumi_zoom->GetXaxis()->SetRangeUser(0., 600.);

    myHistoName = "images/variables/RecoilPt_afterSel_zoom_lumi_inLogScale" + extension;
    drawDataMcComparison("RecoilPt_afterSel_zoom", hRecoilPt_afterSel_mc_lumi_zoom, hRecoilPt_afterSel_data_lumi_zoom,"p_{T}^{leading jet} [GeV]", myHistoName.c_str());


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
    //                                      Njets in recoil afterSel 
    //
    //************************************************************************************************************	


    h1_style(hNjetsRecoil_mc_lumi);
    h1_style(hNjetsRecoil_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hNjetsRecoil_mc_lumi->Scale(getLumi());
    myHistoName = "images/variables/NjetsRecoil_lumi_inLogScale" + extension;
    drawDataMcComparison("NjetsRecoil", hNjetsRecoil_mc_lumi, hNjetsRecoil_data_lumi, "N_{PV}", myHistoName.c_str());


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
    drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_lumi, hFracRmPuJets_JetPt_data_lumi, "p_{T}^{jet} [GeV]", myHistoName.c_str(), false, "N_{removed PU jets}/N_{total jets}");	

    //************************************************************************************************************
    //
    //                                      FracRmPuJets_JetPt zoomed
    //
    //************************************************************************************************************	

    TH1* hFracRmPuJets_JetPt_mc_lumi_zoomed = (TH1F*)hFracRmPuJets_JetPt_mc_lumi->Clone();
    TH1* hFracRmPuJets_JetPt_data_lumi_zoomed = (TH1F*)hFracRmPuJets_JetPt_data_lumi->Clone();


    h1_style(hFracRmPuJets_JetPt_mc_lumi_zoomed);
    h1_style(hFracRmPuJets_JetPt_data_lumi_zoomed);

    hFracRmPuJets_JetPt_mc_lumi_zoomed->GetXaxis()->SetRangeUser(0., 1500.);
    hFracRmPuJets_JetPt_data_lumi_zoomed->GetXaxis()->SetRangeUser(0., 1500.);

    myHistoName = "images/variables/FracRmPuJets_JetPt_zoomed_lumi_inLogScale" + extension;
    drawDataMcComparison("FracRmPuJets_JetPt_zoomed", hFracRmPuJets_JetPt_mc_lumi_zoomed, hFracRmPuJets_JetPt_data_lumi_zoomed, "p_{T}^{jet} [GeV]", myHistoName.c_str(), false, "N_{removed PU jets}/N_{total jets}");		

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
    drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_lumi, hA_beforeSel_data_lumi, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str());


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
    drawDataMcComparison("A_afterSel", hA_afterSel_mc_lumi, hA_afterSel_data_lumi, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      DeltaPhi_METRecoil afterSel 
    //
    //************************************************************************************************************	


    h1_style(hDeltaPhi_METRecoil_afterSel_mc_lumi);
    h1_style(hDeltaPhi_METRecoil_afterSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hDeltaPhi_METRecoil_afterSel_mc_lumi->Scale(getLumi());

    myHistoName = "images/variables/DeltaPhi_METRecoil_afterSel_lumi_inLogScale" + extension;
    drawDataMcComparison("DeltaPhi_METRecoil_afterSel", hDeltaPhi_METRecoil_afterSel_mc_lumi, hDeltaPhi_METRecoil_afterSel_data_lumi, "|#Delta#phi (MET,Recoil)|", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      DeltaPhi_METJet1 afterSel 
    //
    //************************************************************************************************************	


    h1_style(hDeltaPhi_METJet1_afterSel_mc_lumi);
    h1_style(hDeltaPhi_METJet1_afterSel_data_lumi);

    //rescale the Monte Carlo histogramm with luminosity
    hDeltaPhi_METJet1_afterSel_mc_lumi->Scale(getLumi());

    myHistoName = "images/variables/DeltaPhi_METJet1_afterSel_lumi_inLogScale" + extension;
    drawDataMcComparison("DeltaPhi_METJet1_afterSel", hDeltaPhi_METJet1_afterSel_mc_lumi, hDeltaPhi_METJet1_afterSel_data_lumi, "|#Delta#phi (MET,1^{st}jet)|", myHistoName.c_str());

    //*******************************************************************************************************

    if(inLinScale) {
      myHistoName = "images/variables/MJB_inclusive_lumi_inLinScale" + extension;
      drawDataMcComparison("MJB_inclusive", hMJB_inclusive_mc_lumi, hMJB_inclusive_data_lumi, "MJB", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_lumi, hMet_beforeSel_data_lumi, "MET [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_lumi, hMet_afterSel_data_lumi, "MET [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_lumi, hLeadingJetPt_beforeSel_data_lumi, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_lumi, hLeadingJetPt_afterSel_data_lumi, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);

      //myHistoName = "images/variables/LeadingJetMass_afterSel_lumi_inLinScale" + extension;
      //drawDataMcComparison("LeadingJetMass_afterSel", hLeadingJetMass_afterSel_mc_lumi, hLeadingJetMass_afterSel_data_lumi, "M^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);

      //myHistoName = "images/variables/LeadingAndSecondJetsMass_afterSel_lumi_inLinScale" + extension;
      /*drawDataMcComparison("LeadingAndSecondJetsMass_afterSel", hLeadingAndSecondJetsMass_afterSel_mc_lumi, hLeadingAndSecondJetsMass_afterSel_data_lumi, "M^{j_{1}+j_{2}} [GeV]", myHistoName.c_str(), inLinScale);*/

/*      myHistoName = "images/variables/SecondJetPt_beforeSel_lumi_inLinScale" + extension;*/
      //drawDataMcComparison("SecondJetPt_beforeSel", hSecondJetPt_beforeSel_mc_lumi, hSecondJetPt_beforeSel_data_lumi, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);
      //myHistoName = "images/variables/SecondJetPt_afterSel_lumi_inLinScale" + extension;
      /*drawDataMcComparison("SecondJetPt_afterSel", hSecondJetPt_afterSel_mc_lumi, hSecondJetPt_afterSel_data_lumi, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);*/

      myHistoName = "images/variables/JetsPt_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("JetsPt_beforeSel", hJetsPt_beforeSel_mc_lumi, hJetsPt_beforeSel_data_lumi, "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/JetsPt_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("JetsPt_afterSel", hJetsPt_afterSel_mc_lumi, hJetsPt_afterSel_data_lumi, "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale);

      myHistoName = "images/variables/JetsEta_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("JetsEta_beforeSel", hJetsEta_beforeSel_mc_lumi, hJetsEta_beforeSel_data_lumi, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/JetsEta_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("JetsEta_afterSel", hJetsEta_afterSel_mc_lumi, hJetsEta_afterSel_data_lumi, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);

/*      myHistoName = "images/variables/LeadingJetEta_beforeSel_lumi_inLinScale" + extension;*/
      //drawDataMcComparison("LeadingJetEta_beforeSel", hLeadingJetEta_beforeSel_mc_lumi, hLeadingJetEta_beforeSel_data_lumi, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);
      //myHistoName = "images/variables/LeadingJetEta_afterSel_lumi_inLinScale" + extension;
      //drawDataMcComparison("LeadingJetEta_afterSel", hLeadingJetEta_afterSel_mc_lumi, hLeadingJetEta_afterSel_data_lumi, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);

      //myHistoName = "images/variables/RecoilEta_beforeSel_lumi_inLinScale" + extension;
      //drawDataMcComparison("RecoilEta_beforeSel", hRecoilEta_beforeSel_mc_lumi, hRecoilEta_beforeSel_data_lumi, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);
      //myHistoName = "images/variables/RecoilEta_afterSel_lumi_inLinScale" + extension;
      /*drawDataMcComparison("RecoilEta_afterSel", hRecoilEta_afterSel_mc_lumi, hRecoilEta_afterSel_data_lumi, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);*/

      myHistoName = "images/variables/JetsPhi_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("JetsPhi_beforeSel", hJetsPhi_beforeSel_mc_lumi, hJetsPhi_beforeSel_data_lumi, "#phi^{jet} [rad]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/JetsPhi_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("JetsPhi_afterSel", hJetsPhi_afterSel_mc_lumi, hJetsPhi_afterSel_data_lumi, "#phi^{jet} [rad]", myHistoName.c_str(), inLinScale);

      myHistoName = "images/variables/Njets_ptSup30_etaInf5_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Njets_ptSup30_etaInf5_beforeSel", hNjets_ptSup30_etaInf5_beforeSel_mc_lumi, hNjets_ptSup30_etaInf5_beforeSel_data_lumi, "N_{jets} with p_{T} > 25 GeV and |#eta| < 5.0", myHistoName.c_str(), inLinScale);

      myHistoName = "images/variables/HT_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("HT_afterSel", hHT_afterSel_mc_lumi, hHT_afterSel_data_lumi, "HT [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/HT_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("HT_beforeSel", hHT_beforeSel_mc_lumi, hHT_beforeSel_data_lumi, "HT [GeV]", myHistoName.c_str(), inLinScale);

      myHistoName = "images/variables/RecoilPt_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_lumi, hRecoilPt_beforeSel_data_lumi, "p_{T}^{Recoil} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/RecoilPt_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_lumi, hRecoilPt_afterSel_data_lumi, "p_{T}^{Recoil} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_beforeSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Npv_beforeSel", hNpv_beforeSel_mc_lumi, hNpv_beforeSel_data_lumi,  "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("Npv_afterSel", hNpv_afterSel_mc_lumi, hNpv_afterSel_data_lumi,  "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/NjetsRecoil_lumi_inLinScale" + extension;
      drawDataMcComparison("NjetsRecoil", hNjetsRecoil_mc_lumi, hNjetsRecoil_data_lumi,  "N_{jets in recoil}", myHistoName.c_str(), inLinScale);

      myHistoName = "images/variables/FracRmPuJets_JetPt_lumi_inLinScale" + extension;
      drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_lumi, hFracRmPuJets_JetPt_data_lumi,  "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale, "N_{removed PU jets}/N_{total jets}");
      myHistoName = "images/variables/FracRmPuJets_JetPt_zoomed_lumi_inLinScale" + extension;
      drawDataMcComparison("FracRmPuJets_JetPt_zoomed", hFracRmPuJets_JetPt_mc_lumi_zoomed, hFracRmPuJets_JetPt_data_lumi_zoomed,  "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale, "N_{removed PU jets}/N_{total jets}");
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
      drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_lumi, hA_beforeSel_data_lumi, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/A_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("A_afterSel", hA_afterSel_mc_lumi, hA_afterSel_data_lumi, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/DeltaPhi_METRecoil_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("DeltaPhi_METRecoil_afterSel", hDeltaPhi_METRecoil_afterSel_mc_lumi, hDeltaPhi_METRecoil_afterSel_data_lumi, "#Delta#phi (MET,Recoil)", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/DeltaPhi_METJet1_afterSel_lumi_inLinScale" + extension;
      drawDataMcComparison("DeltaPhi_METJet1_afterSel", hDeltaPhi_METJet1_afterSel_mc_lumi, hDeltaPhi_METJet1_afterSel_data_lumi, "#Delta#phi (MET,1^{st}jet)", myHistoName.c_str(), inLinScale);
      for(int j=0; j<myPtBinning.getSize(); j++) {
        binName = myPtBinning.getName(j);
        myName = "MJB_{data}/MJB_{MC} for " + binName;
        //myXName = "MJB for " + binName;
        std::stringstream PtBinningName ;
        PtBinningName.str("");
        PtBinningName << "MJB response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
        myXName = PtBinningName.str();
        mySaveName = "images/MJBperRefObjPt/MJB_" + binName + "_lumi_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_lumi[j], vMJB_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
        mySaveName =  "images/MJBperRefObjPt/MJB_" + binName + "_lumi_inLinScale_woRatio" + extension;
        drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_lumi[j], vMJB_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale, "", "r", 0);

        myName = "MPF_{data}/MPF_{MC} for " + binName;
        //myXName = "MPF for " + binName;
        PtBinningName.str("");
        PtBinningName << "MPF response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
        myXName = PtBinningName.str();
        mySaveName = "images/MPFperRefObjPt/MPF_" + binName + "_lumi_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vMPF_RefObjPt_mc_lumi[j], vMPF_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
        mySaveName =  "images/MPFperRefObjPt/MPF_" + binName + "_lumi_inLinScale_woRatio" + extension;
        drawDataMcComparison(myName.c_str(), vMPF_RefObjPt_mc_lumi[j], vMPF_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale, "", "r", 0);

        myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
        myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
        mySaveName = "images/LeadingJetPtperRefObjPt/LeadingJetPt_" + binName + "_lumi_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_lumi[j], vLeadingJetPt_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);

        myName = "pT^{Recoil}_{data}/pT^{Recoil}_{MC} for " + binName;
        myXName = "pT^{Recoil} for " + binName + " [GeV]";
        mySaveName = "images/RecoilPtperRefObjPt/RecoilPt_" + binName + "_lumi_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vRecoilPt_RefObjPt_mc_lumi[j], vRecoilPt_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }

      for(int j=0; j<myHLTPtBinning.getSize(); j++) {
        binName = myHLTPtBinning.getName(j);
        myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
        myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
        mySaveName = "images/LeadingJetPtperPtHLT/LeadingJetPt_" + binName + "_lumi_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vLeadingJetPt_RefObjPtHLT_mc_lumi[j], vLeadingJetPt_RefObjPtHLT_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }

      for(int j=0; j<myEtaBinning.getSize(); j++) {
        binName = myEtaBinning.getName(j);
        myName = "MJB_{data}/MJB_{MC} for " + binName;
        myXName = "MJB for " + binName;
        mySaveName = "images/MJBperRefObjEta/MJB_" + binName + "_lumi_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vMJB_RefObjEta_mc_lumi[j], vMJB_RefObjEta_data_lumi[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
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
    float Nentries_MJB_RefObjPt_mc;
    float Nentries_MPF_RefObjPt_mc;
    float Nentries_MJB_RefObjPt_Data;
    float Nentries_MPF_RefObjPt_Data;
    float Nentries_LeadingJetPt_RecoilPt_mc;
    float Nentries_LeadingJetPt_RecoilPt_Data;
    float Nentries_LeadingJetPt_LeadingJetPtHLT_mc;
    float Nentries_LeadingJetPt_LeadingJetPtHLT_Data;
    float Nentries_RecoilPt_RecoilPt_mc;
    float Nentries_RecoilPt_RecoilPt_Data;
    float Nentries_MJB_RefObjEta_mc;
    float Nentries_MJB_RefObjEta_Data;
    float Nentries_MJB_Npv_mc;
    float Nentries_MJB_Npv_Data;

    //************************************************************************************************************
    //
    //                                      MJB per ptrecoil 
    //
    //************************************************************************************************************	

    for(int j=0; j<myPtBinning.getSize(); j++) {
      vMJB_RefObjPt_mc_shape[j]=(TH1F*)vMJB_RefObjPt_mc_lumi[j]->Clone();
      vMJB_RefObjPt_data_shape[j]=(TH1F*)vMJB_RefObjPt_data_lumi[j]->Clone();
      h1_style(vMJB_RefObjPt_data_shape[j]);
      h1_style(vMJB_RefObjPt_mc_shape[j]);

      Nentries_MJB_RefObjPt_mc = vMJB_RefObjPt_mc_shape[j]->Integral();
      Nentries_MJB_RefObjPt_Data = vMJB_RefObjPt_data_shape[j]->Integral();
      vMJB_RefObjPt_mc_shape[j]->Scale(Nentries_MJB_RefObjPt_Data/Nentries_MJB_RefObjPt_mc);

      binName = myPtBinning.getName(j);
      myName = "MJB_{data}/MJB_{MC} for " + binName;
      //myXName = "MJB for " + binName;
      std::stringstream PtBinningName ;
      PtBinningName.str("");
      PtBinningName << "MJB response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
      myXName = PtBinningName.str();
      mySaveName = "images/MJBperRefObjPt/MJB_" + binName + "_shape_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_shape[j], vMJB_RefObjPt_data_shape[j], myXName.c_str(), mySaveName.c_str());
      mySaveName =  "images/MJBperRefObjPt/MJB_" + binName + "_shape_inLogScale_woRatio" + extension;
      drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_shape[j], vMJB_RefObjPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), false, "", "r", 0);
    }

    //************************************************************************************************************
    //
    //                                      MPF per ptrecoil 
    //
    //************************************************************************************************************	

    for(int j=0; j<myPtBinning.getSize(); j++) {
      vMPF_RefObjPt_mc_shape[j]=(TH1F*)vMPF_RefObjPt_mc_lumi[j]->Clone();
      vMPF_RefObjPt_data_shape[j]=(TH1F*)vMPF_RefObjPt_data_lumi[j]->Clone();
      h1_style(vMPF_RefObjPt_data_shape[j]);
      h1_style(vMPF_RefObjPt_mc_shape[j]);

      Nentries_MPF_RefObjPt_mc = vMPF_RefObjPt_mc_shape[j]->Integral();
      Nentries_MPF_RefObjPt_Data = vMPF_RefObjPt_data_shape[j]->Integral();
      vMPF_RefObjPt_mc_shape[j]->Scale(Nentries_MPF_RefObjPt_Data/Nentries_MPF_RefObjPt_mc);

      binName = myPtBinning.getName(j);
      myName = "MPF_{data}/MPF_{MC} for " + binName;
      //myXName = "MPF for " + binName;
      std::stringstream PtBinningName ;
      PtBinningName.str("");
      PtBinningName << "MPF response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
      myXName = PtBinningName.str();
      mySaveName = "images/MPFperRefObjPt/MPF_" + binName + "_shape_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vMPF_RefObjPt_mc_lumi[j], vMPF_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
      mySaveName =  "images/MPFperRefObjPt/MPF_" + binName + "_shape_inLogScale_woRatio" + extension;
      drawDataMcComparison(myName.c_str(), vMPF_RefObjPt_mc_lumi[j], vMPF_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str(), false, "", "r", 0);
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
      myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
      mySaveName = "images/LeadingJetPtperRefObjPt/LeadingJetPt_" + binName + "_shape_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_lumi[j], vLeadingJetPt_RecoilPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
    }

    //************************************************************************************************************
    //
    //                                      LeadingJetPt per leadingjetrawpt 
    //
    //************************************************************************************************************	

    for(int j=0; j<myHLTPtBinning.getSize(); j++) {
      vLeadingJetPt_RefObjPtHLT_mc_shape[j]=(TH1F*)vLeadingJetPt_RefObjPtHLT_mc_lumi[j]->Clone();
      vLeadingJetPt_RefObjPtHLT_data_shape[j]=(TH1F*)vLeadingJetPt_RefObjPtHLT_data_lumi[j]->Clone();
      h1_style(vLeadingJetPt_RefObjPtHLT_data_shape[j]);
      h1_style(vLeadingJetPt_RefObjPtHLT_mc_shape[j]);

      Nentries_LeadingJetPt_LeadingJetPtHLT_mc = vLeadingJetPt_RefObjPtHLT_mc_shape[j]->Integral();
      Nentries_LeadingJetPt_LeadingJetPtHLT_Data = vLeadingJetPt_RefObjPtHLT_data_shape[j]->Integral();
      vLeadingJetPt_RefObjPtHLT_mc_shape[j]->Scale(Nentries_LeadingJetPt_LeadingJetPtHLT_Data/Nentries_LeadingJetPt_LeadingJetPtHLT_mc);

      binName = myHLTPtBinning.getName(j);
      myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
      myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
      mySaveName = "images/LeadingJetPtperPtHLT/LeadingJetPt_" + binName + "_shape_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vLeadingJetPt_RefObjPtHLT_mc_lumi[j], vLeadingJetPt_RefObjPtHLT_data_lumi[j], myXName.c_str(), mySaveName.c_str());
    }


    //************************************************************************************************************
    //
    //                                      RecoilPt per ptrecoil 
    //
    //************************************************************************************************************	

    for(int j=0; j<myPtBinning.getSize(); j++) {
      vRecoilPt_RefObjPt_mc_shape[j]=(TH1F*)vRecoilPt_RefObjPt_mc_lumi[j]->Clone();
      vRecoilPt_RefObjPt_data_shape[j]=(TH1F*)vRecoilPt_RefObjPt_data_lumi[j]->Clone();
      h1_style(vRecoilPt_RefObjPt_data_shape[j]);
      h1_style(vRecoilPt_RefObjPt_mc_shape[j]);

      Nentries_RecoilPt_RecoilPt_mc = vRecoilPt_RefObjPt_mc_shape[j]->Integral();
      Nentries_RecoilPt_RecoilPt_Data = vRecoilPt_RefObjPt_data_shape[j]->Integral();
      vRecoilPt_RefObjPt_mc_shape[j]->Scale(Nentries_RecoilPt_RecoilPt_Data/Nentries_RecoilPt_RecoilPt_mc);

      binName = myPtBinning.getName(j);
      myName = "pT^{Recoil}_{data}/pT^{Recoil}_{MC} for " + binName;
      myXName = "pT^{Recoil} for " + binName + " [GeV]";
      mySaveName = "images/RecoilPtperRefObjPt/RecoilPt_" + binName + "_shape_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vRecoilPt_RefObjPt_mc_lumi[j], vRecoilPt_RefObjPt_data_lumi[j], myXName.c_str(), mySaveName.c_str());
    }


    //************************************************************************************************************
    //
    //                                      MJB per etarecoil 
    //
    //************************************************************************************************************	

    for(int j=0; j<myEtaBinning.getSize(); j++) {
      vMJB_RefObjEta_mc_shape[j]=(TH1F*)vMJB_RefObjEta_mc_lumi[j]->Clone();
      vMJB_RefObjEta_data_shape[j]=(TH1F*)vMJB_RefObjEta_data_lumi[j]->Clone();
      h1_style(vMJB_RefObjEta_data_shape[j]);
      h1_style(vMJB_RefObjEta_mc_shape[j]);

      Nentries_MJB_RefObjEta_mc = vMJB_RefObjEta_mc_shape[j]->Integral();
      Nentries_MJB_RefObjEta_Data = vMJB_RefObjEta_data_shape[j]->Integral();
      vMJB_RefObjEta_mc_shape[j]->Scale(Nentries_MJB_RefObjEta_Data/Nentries_MJB_RefObjEta_mc);

      binName = myEtaBinning.getName(j);
      myName = "MJB_{data}/MJB_{MC} for " + binName;
      myXName = "MJB for " + binName;
      mySaveName = "images/MJBperRefObjEta/MJB_" + binName + "_shape_inLogScale" + extension;
      drawDataMcComparison(myName.c_str(), vMJB_RefObjEta_mc_lumi[j], vMJB_RefObjEta_data_lumi[j], myXName.c_str(), mySaveName.c_str());
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
    drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_shape, hMet_beforeSel_data_shape, "MET [GeV]", myHistoName.c_str());

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
    drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_shape, hMet_afterSel_data_shape, "MET [GeV]", myHistoName.c_str());

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
    drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_shape, hLeadingJetPt_beforeSel_data_shape, "p_{T}^{leading jet} [GeV]", myHistoName.c_str());

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
    drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_shape, hLeadingJetPt_afterSel_data_shape, "p_{T}^{leading jet} [GeV]", myHistoName.c_str());


    //************************************************************************************************************
    //
    //                                      LeadingJetMass afterSel 
    //
    //************************************************************************************************************	

    //TH1F* hLeadingJetMass_afterSel_mc_shape=(TH1F*)hLeadingJetMass_afterSel_mc_lumi->Clone();
    //TH1F* hLeadingJetMass_afterSel_data_shape=(TH1F*)hLeadingJetMass_afterSel_data_lumi->Clone();

    //h1_style(hLeadingJetMass_afterSel_mc_shape);
    //h1_style(hLeadingJetMass_afterSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_LeadingJetMass_afterSel_mc = hLeadingJetMass_afterSel_mc_shape->Integral();
    //float Nentries_LeadingJetMass_afterSel_Data = hLeadingJetMass_afterSel_data_shape->Integral();	
    //hLeadingJetMass_afterSel_mc_shape->Scale(Nentries_LeadingJetMass_afterSel_Data/Nentries_LeadingJetMass_afterSel_mc);

    //myHistoName = "images/variables/LeadingJetMass_afterSel_shape_inLogScale" + extension;
    /*drawDataMcComparison("LeadingJetMass_afterSel", hLeadingJetMass_afterSel_mc_shape, hLeadingJetMass_afterSel_data_shape, "M^{leading jet} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      LeadingAndSecondJetsMass afterSel 
    //
    //************************************************************************************************************	

    //TH1F* hLeadingAndSecondJetsMass_afterSel_mc_shape=(TH1F*)hLeadingAndSecondJetsMass_afterSel_mc_lumi->Clone();
    //TH1F* hLeadingAndSecondJetsMass_afterSel_data_shape=(TH1F*)hLeadingAndSecondJetsMass_afterSel_data_lumi->Clone();

    //h1_style(hLeadingAndSecondJetsMass_afterSel_mc_shape);
    //h1_style(hLeadingAndSecondJetsMass_afterSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_LeadingAndSecondJetsMass_afterSel_mc = hLeadingAndSecondJetsMass_afterSel_mc_shape->Integral();
    //float Nentries_LeadingAndSecondJetsMass_afterSel_Data = hLeadingAndSecondJetsMass_afterSel_data_shape->Integral();	
    //hLeadingAndSecondJetsMass_afterSel_mc_shape->Scale(Nentries_LeadingAndSecondJetsMass_afterSel_Data/Nentries_LeadingAndSecondJetsMass_afterSel_mc);

    //myHistoName = "images/variables/LeadingAndSecondJetsMass_afterSel_shape_inLogScale" + extension;
    /*drawDataMcComparison("LeadingAndSecondJetsMass_afterSel", hLeadingAndSecondJetsMass_afterSel_mc_shape, hLeadingAndSecondJetsMass_afterSel_data_shape, "M^{j_{1}+j_{2}} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      SecondJetPt beforeSel 
    //
    //************************************************************************************************************	

/*    TH1F* hSecondJetPt_beforeSel_mc_shape=(TH1F*)hSecondJetPt_beforeSel_mc_lumi->Clone();*/
    //TH1F* hSecondJetPt_beforeSel_data_shape=(TH1F*)hSecondJetPt_beforeSel_data_lumi->Clone();

    //h1_style(hSecondJetPt_beforeSel_mc_shape);
    //h1_style(hSecondJetPt_beforeSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_SecondJetPt_beforeSel_mc = hSecondJetPt_beforeSel_mc_shape->Integral();
    //float Nentries_SecondJetPt_beforeSel_Data = hSecondJetPt_beforeSel_data_shape->Integral();	
    //hSecondJetPt_beforeSel_mc_shape->Scale(Nentries_SecondJetPt_beforeSel_Data/Nentries_SecondJetPt_beforeSel_mc);

    //myHistoName = "images/variables/SecondJetPt_beforeSel_shape_inLogScale" + extension;
    /*drawDataMcComparison("SecondJetPt_beforeSel", hSecondJetPt_beforeSel_mc_shape, hSecondJetPt_beforeSel_data_shape, "p_{T}^{2^{nd} jet} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      SecondJetPt afterSel 
    //
    //************************************************************************************************************	

    //TH1F* hSecondJetPt_afterSel_mc_shape=(TH1F*)hSecondJetPt_afterSel_mc_lumi->Clone();
    //TH1F* hSecondJetPt_afterSel_data_shape=(TH1F*)hSecondJetPt_afterSel_data_lumi->Clone();

    //h1_style(hSecondJetPt_afterSel_mc_shape);
    //h1_style(hSecondJetPt_afterSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_SecondJetPt_afterSel_mc = hSecondJetPt_afterSel_mc_shape->Integral();
    //float Nentries_SecondJetPt_afterSel_Data = hSecondJetPt_afterSel_data_shape->Integral();	
    //hSecondJetPt_afterSel_mc_shape->Scale(Nentries_SecondJetPt_afterSel_Data/Nentries_SecondJetPt_afterSel_mc);

    //myHistoName = "images/variables/SecondJetPt_afterSel_shape_inLogScale" + extension;
    /*drawDataMcComparison("SecondJetPt_afterSel", hSecondJetPt_afterSel_mc_shape, hSecondJetPt_afterSel_data_shape, "p_{T}^{2^{nd} jet} [GeV]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      JetsPt beforeSel 
    //
    //************************************************************************************************************	

    TH1F* hJetsPt_beforeSel_mc_shape=(TH1F*)hJetsPt_beforeSel_mc_lumi->Clone();
    TH1F* hJetsPt_beforeSel_data_shape=(TH1F*)hJetsPt_beforeSel_data_lumi->Clone();

    h1_style(hJetsPt_beforeSel_mc_shape);
    h1_style(hJetsPt_beforeSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_JetsPt_beforeSel_mc = hJetsPt_beforeSel_mc_shape->Integral();
    float Nentries_JetsPt_beforeSel_Data = hJetsPt_beforeSel_data_shape->Integral();	
    hJetsPt_beforeSel_mc_shape->Scale(Nentries_JetsPt_beforeSel_Data/Nentries_JetsPt_beforeSel_mc);

    myHistoName = "images/variables/JetsPt_beforeSel_shape_inLogScale" + extension;
    drawDataMcComparison("JetsPt_beforeSel", hJetsPt_beforeSel_mc_shape, hJetsPt_beforeSel_data_shape, "p_{T}^{jet} [GeV]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      JetsPt afterSel 
    //
    //************************************************************************************************************	

    TH1F* hJetsPt_afterSel_mc_shape=(TH1F*)hJetsPt_afterSel_mc_lumi->Clone();
    TH1F* hJetsPt_afterSel_data_shape=(TH1F*)hJetsPt_afterSel_data_lumi->Clone();

    h1_style(hJetsPt_afterSel_mc_shape);
    h1_style(hJetsPt_afterSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_JetsPt_afterSel_mc = hJetsPt_afterSel_mc_shape->Integral();
    float Nentries_JetsPt_afterSel_Data = hJetsPt_afterSel_data_shape->Integral();	
    hJetsPt_afterSel_mc_shape->Scale(Nentries_JetsPt_afterSel_Data/Nentries_JetsPt_afterSel_mc);

    myHistoName = "images/variables/JetsPt_afterSel_shape_inLogScale" + extension;
    drawDataMcComparison("JetsPt_afterSel", hJetsPt_afterSel_mc_shape, hJetsPt_afterSel_data_shape, "p_{T}^{jet} [GeV]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      JetsEta beforeSel 
    //
    //************************************************************************************************************	

    TH1F* hJetsEta_beforeSel_mc_shape=(TH1F*)hJetsEta_beforeSel_mc_lumi->Clone();
    TH1F* hJetsEta_beforeSel_data_shape=(TH1F*)hJetsEta_beforeSel_data_lumi->Clone();

    h1_style(hJetsEta_beforeSel_mc_shape);
    h1_style(hJetsEta_beforeSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_JetsEta_beforeSel_mc = hJetsEta_beforeSel_mc_shape->Integral();
    float Nentries_JetsEta_beforeSel_Data = hJetsEta_beforeSel_data_shape->Integral();	
    hJetsEta_beforeSel_mc_shape->Scale(Nentries_JetsEta_beforeSel_Data/Nentries_JetsEta_beforeSel_mc);

    myHistoName = "images/variables/JetsEta_beforeSel_shape_inLogScale" + extension;
    drawDataMcComparison("JetsEta_beforeSel", hJetsEta_beforeSel_mc_shape, hJetsEta_beforeSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      JetsEta afterSel 
    //
    //************************************************************************************************************	

    TH1F* hJetsEta_afterSel_mc_shape=(TH1F*)hJetsEta_afterSel_mc_lumi->Clone();
    TH1F* hJetsEta_afterSel_data_shape=(TH1F*)hJetsEta_afterSel_data_lumi->Clone();

    h1_style(hJetsEta_afterSel_mc_shape);
    h1_style(hJetsEta_afterSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_JetsEta_afterSel_mc = hJetsEta_afterSel_mc_shape->Integral();
    float Nentries_JetsEta_afterSel_Data = hJetsEta_afterSel_data_shape->Integral();	
    hJetsEta_afterSel_mc_shape->Scale(Nentries_JetsEta_afterSel_Data/Nentries_JetsEta_afterSel_mc);

    myHistoName = "images/variables/JetsEta_afterSel_shape_inLogScale" + extension;
    drawDataMcComparison("JetsEta_afterSel", hJetsEta_afterSel_mc_shape, hJetsEta_afterSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str()); 

    //************************************************************************************************************
    //
    //                                      LeadingJetEta beforeSel 
    //
    //************************************************************************************************************	

/*    TH1F* hLeadingJetEta_beforeSel_mc_shape=(TH1F*)hLeadingJetEta_beforeSel_mc_lumi->Clone();*/
    //TH1F* hLeadingJetEta_beforeSel_data_shape=(TH1F*)hLeadingJetEta_beforeSel_data_lumi->Clone();

    //h1_style(hLeadingJetEta_beforeSel_mc_shape);
    //h1_style(hLeadingJetEta_beforeSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_LeadingJetEta_beforeSel_mc = hLeadingJetEta_beforeSel_mc_shape->Integral();
    //float Nentries_LeadingJetEta_beforeSel_Data = hLeadingJetEta_beforeSel_data_shape->Integral();	
    //hLeadingJetEta_beforeSel_mc_shape->Scale(Nentries_LeadingJetEta_beforeSel_Data/Nentries_LeadingJetEta_beforeSel_mc);

    //myHistoName = "images/variables/LeadingJetEta_beforeSel_shape_inLogScale" + extension;
    //drawDataMcComparison("LeadingJetEta_beforeSel", hLeadingJetEta_beforeSel_mc_shape, hLeadingJetEta_beforeSel_data_shape, "#eta^{leading jet} [rad]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      LeadingJetEta afterSel 
    //
    //************************************************************************************************************	

    //TH1F* hLeadingJetEta_afterSel_mc_shape=(TH1F*)hLeadingJetEta_afterSel_mc_lumi->Clone();
    //TH1F* hLeadingJetEta_afterSel_data_shape=(TH1F*)hLeadingJetEta_afterSel_data_lumi->Clone();

    //h1_style(hLeadingJetEta_afterSel_mc_shape);
    //h1_style(hLeadingJetEta_afterSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_LeadingJetEta_afterSel_mc = hLeadingJetEta_afterSel_mc_shape->Integral();
    //float Nentries_LeadingJetEta_afterSel_Data = hLeadingJetEta_afterSel_data_shape->Integral();	
    //hLeadingJetEta_afterSel_mc_shape->Scale(Nentries_LeadingJetEta_afterSel_Data/Nentries_LeadingJetEta_afterSel_mc);

    //myHistoName = "images/variables/LeadingJetEta_afterSel_shape_inLogScale" + extension;
    /*drawDataMcComparison("LeadingJetEta_afterSel", hLeadingJetEta_afterSel_mc_shape, hLeadingJetEta_afterSel_data_shape, "#eta^{leading jet} [rad]", myHistoName.c_str()); */


    //************************************************************************************************************
    //
    //                                      RecoilEta beforeSel 
    //
    //************************************************************************************************************	

 /*   TH1F* hRecoilEta_beforeSel_mc_shape=(TH1F*)hRecoilEta_beforeSel_mc_lumi->Clone();*/
    //TH1F* hRecoilEta_beforeSel_data_shape=(TH1F*)hRecoilEta_beforeSel_data_lumi->Clone();

    //h1_style(hRecoilEta_beforeSel_mc_shape);
    //h1_style(hRecoilEta_beforeSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_RecoilEta_beforeSel_mc = hRecoilEta_beforeSel_mc_shape->Integral();
    //float Nentries_RecoilEta_beforeSel_Data = hRecoilEta_beforeSel_data_shape->Integral();	
    //hRecoilEta_beforeSel_mc_shape->Scale(Nentries_RecoilEta_beforeSel_Data/Nentries_RecoilEta_beforeSel_mc);

    //myHistoName = "images/variables/RecoilEta_beforeSel_shape_inLogScale" + extension;
    /*drawDataMcComparison("RecoilEta_beforeSel", hRecoilEta_beforeSel_mc_shape, hRecoilEta_beforeSel_data_shape, "#eta^{recoil} [rad]", myHistoName.c_str());*/

    //************************************************************************************************************
    //
    //                                      RecoilEta afterSel 
    //
    //************************************************************************************************************	

/*    TH1F* hRecoilEta_afterSel_mc_shape=(TH1F*)hRecoilEta_afterSel_mc_lumi->Clone();*/
    //TH1F* hRecoilEta_afterSel_data_shape=(TH1F*)hRecoilEta_afterSel_data_lumi->Clone();

    //h1_style(hRecoilEta_afterSel_mc_shape);
    //h1_style(hRecoilEta_afterSel_data_shape);

    ////rescale the Monte Carlo histogramm with number of entries
    //float Nentries_RecoilEta_afterSel_mc = hRecoilEta_afterSel_mc_shape->Integral();
    //float Nentries_RecoilEta_afterSel_Data = hRecoilEta_afterSel_data_shape->Integral();	
    //hRecoilEta_afterSel_mc_shape->Scale(Nentries_RecoilEta_afterSel_Data/Nentries_RecoilEta_afterSel_mc);

    //myHistoName = "images/variables/RecoilEta_afterSel_shape_inLogScale" + extension;
    /*drawDataMcComparison("RecoilEta_afterSel", hRecoilEta_afterSel_mc_shape, hRecoilEta_afterSel_data_shape, "#eta^{recoil} [rad]", myHistoName.c_str()); */

    //************************************************************************************************************
    //
    //                                      JetsPhi beforeSel 
    //
    //************************************************************************************************************	

    TH1F* hJetsPhi_beforeSel_mc_shape=(TH1F*)hJetsPhi_beforeSel_mc_lumi->Clone();
    TH1F* hJetsPhi_beforeSel_data_shape=(TH1F*)hJetsPhi_beforeSel_data_lumi->Clone();

    h1_style(hJetsPhi_beforeSel_mc_shape);
    h1_style(hJetsPhi_beforeSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_JetsPhi_beforeSel_mc = hJetsPhi_beforeSel_mc_shape->Integral();
    float Nentries_JetsPhi_beforeSel_Data = hJetsPhi_beforeSel_data_shape->Integral();	
    hJetsPhi_beforeSel_mc_shape->Scale(Nentries_JetsPhi_beforeSel_Data/Nentries_JetsPhi_beforeSel_mc);

    myHistoName = "images/variables/JetsPhi_beforeSel_shape_inLogScale" + extension;
    drawDataMcComparison("JetsPhi_beforeSel", hJetsPhi_beforeSel_mc_shape, hJetsPhi_beforeSel_data_shape, "#phi^{jet} [rad]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      JetsPhi afterSel 
    //
    //************************************************************************************************************	

    TH1F* hJetsPhi_afterSel_mc_shape=(TH1F*)hJetsPhi_afterSel_mc_lumi->Clone();
    TH1F* hJetsPhi_afterSel_data_shape=(TH1F*)hJetsPhi_afterSel_data_lumi->Clone();

    h1_style(hJetsPhi_afterSel_mc_shape);
    h1_style(hJetsPhi_afterSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_JetsPhi_afterSel_mc = hJetsPhi_afterSel_mc_shape->Integral();
    float Nentries_JetsPhi_afterSel_Data = hJetsPhi_afterSel_data_shape->Integral();	
    hJetsPhi_afterSel_mc_shape->Scale(Nentries_JetsPhi_afterSel_Data/Nentries_JetsPhi_afterSel_mc);

    myHistoName = "images/variables/JetsPhi_afterSel_shape_inLogScale" + extension;
    drawDataMcComparison("JetsPhi_afterSel", hJetsPhi_afterSel_mc_shape, hJetsPhi_afterSel_data_shape, "#phi^{jet} [rad]", myHistoName.c_str()); 
    //************************************************************************************************************
    //
    //                                      Njets_ptSup30_etaInf5 beforeSel 
    //
    //************************************************************************************************************	

    TH1F* hNjets_ptSup30_etaInf5_beforeSel_mc_shape=(TH1F*)hNjets_ptSup30_etaInf5_beforeSel_mc_lumi->Clone();
    TH1F* hNjets_ptSup30_etaInf5_beforeSel_data_shape=(TH1F*)hNjets_ptSup30_etaInf5_beforeSel_data_lumi->Clone();

    h1_style(hNjets_ptSup30_etaInf5_beforeSel_mc_shape);
    h1_style(hNjets_ptSup30_etaInf5_beforeSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_Njets_ptSup30_etaInf5_beforeSel_mc = hNjets_ptSup30_etaInf5_beforeSel_mc_shape->Integral();
    float Nentries_Njets_ptSup30_etaInf5_beforeSel_Data = hNjets_ptSup30_etaInf5_beforeSel_data_shape->Integral();	
    hNjets_ptSup30_etaInf5_beforeSel_mc_shape->Scale(Nentries_Njets_ptSup30_etaInf5_beforeSel_Data/Nentries_Njets_ptSup30_etaInf5_beforeSel_mc);

    myHistoName = "images/variables/Njets_ptSup30_etaInf5_beforeSel_shape_inLogScale" + extension;
    drawDataMcComparison("Njets_ptSup30_etaInf5_beforeSel", hNjets_ptSup30_etaInf5_beforeSel_mc_shape, hNjets_ptSup30_etaInf5_beforeSel_data_shape, "N_{jets} with p_{T} > 25 GeV and |#eta| < 5.0", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      HT afterSel 
    //
    //************************************************************************************************************	

    TH1F* hHT_afterSel_mc_shape=(TH1F*)hHT_afterSel_mc_lumi->Clone();
    TH1F* hHT_afterSel_data_shape=(TH1F*)hHT_afterSel_data_lumi->Clone();

    h1_style(hHT_afterSel_mc_shape);
    h1_style(hHT_afterSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_HT_afterSel_mc = hHT_afterSel_mc_shape->Integral();
    float Nentries_HT_afterSel_Data = hHT_afterSel_data_shape->Integral();	
    hHT_afterSel_mc_shape->Scale(Nentries_HT_afterSel_Data/Nentries_HT_afterSel_mc);

    myHistoName = "images/variables/HT_afterSel_shape_inLogScale" + extension;
    drawDataMcComparison("HT_afterSel", hHT_afterSel_mc_shape, hHT_afterSel_data_shape, "HT [GeV]", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      HT beforeSel 
    //
    //************************************************************************************************************	

    TH1F* hHT_beforeSel_mc_shape=(TH1F*)hHT_beforeSel_mc_lumi->Clone();
    TH1F* hHT_beforeSel_data_shape=(TH1F*)hHT_beforeSel_data_lumi->Clone();

    h1_style(hHT_beforeSel_mc_shape);
    h1_style(hHT_beforeSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_HT_beforeSel_mc = hHT_beforeSel_mc_shape->Integral();
    float Nentries_HT_beforeSel_Data = hHT_beforeSel_data_shape->Integral();	
    hHT_beforeSel_mc_shape->Scale(Nentries_HT_beforeSel_Data/Nentries_HT_beforeSel_mc);

    myHistoName = "images/variables/HT_beforeSel_shape_inLogScale" + extension;
    drawDataMcComparison("HT_beforeSel", hHT_beforeSel_mc_shape, hHT_beforeSel_data_shape, "HT [GeV]", myHistoName.c_str());


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
    drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_shape, hRecoilPt_beforeSel_data_shape, "p_{T}^{Recoil} [GeV]", myHistoName.c_str());

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
    drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_shape, hRecoilPt_afterSel_data_shape, "p_{T}^{Recoil} [GeV]", myHistoName.c_str());

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
    //                                      Njets in recoil afterSel 
    //
    //************************************************************************************************************	

    TH1F* hNjetsRecoil_mc_shape=(TH1F*)hNjetsRecoil_mc_lumi->Clone();
    TH1F* hNjetsRecoil_data_shape=(TH1F*)hNjetsRecoil_data_lumi->Clone();

    h1_style(hNjetsRecoil_mc_shape);
    h1_style(hNjetsRecoil_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_NjetsRecoil_mc = hNjetsRecoil_mc_shape->Integral();
    float Nentries_NjetsRecoil_Data = hNjetsRecoil_data_shape->Integral();	
    hNjetsRecoil_mc_shape->Scale(Nentries_NjetsRecoil_Data/Nentries_NjetsRecoil_mc);

    myHistoName = "images/variables/NjetsRecoil_shape_inLogScale" + extension;
    drawDataMcComparison("NjetsRecoil", hNjetsRecoil_mc_shape, hNjetsRecoil_data_shape, "N_{PV}", myHistoName.c_str());

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
    drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_shape, hFracRmPuJets_JetPt_data_shape, "p_{T}^{jet} [GeV]", myHistoName.c_str(), false, "N_{removed PU jets}/N_{total jets}");

    //************************************************************************************************************
    //
    //                                      FracRmPuJets_JetPt_zoomed
    //
    //************************************************************************************************************	

    TH1F* hFracRmPuJets_JetPt_mc_shape_zoomed=(TH1F*)hFracRmPuJets_JetPt_mc_shape->Clone();
    TH1F* hFracRmPuJets_JetPt_data_shape_zoomed=(TH1F*)hFracRmPuJets_JetPt_data_shape->Clone();

    h1_style(hFracRmPuJets_JetPt_mc_shape_zoomed);
    h1_style(hFracRmPuJets_JetPt_data_shape_zoomed);

    hFracRmPuJets_JetPt_mc_shape_zoomed->GetXaxis()->SetRangeUser(0., 1500.);
    hFracRmPuJets_JetPt_data_shape_zoomed->GetXaxis()->SetRangeUser(0., 1500.);

    myHistoName = "images/variables/FracRmPuJets_JetPt_zoomed_shape_inLogScale" + extension;
    drawDataMcComparison("FracRmPuJets_JetPt_zoomed", hFracRmPuJets_JetPt_mc_shape_zoomed, hFracRmPuJets_JetPt_data_shape_zoomed, "p_{T}^{jet} [GeV]", myHistoName.c_str(), false, "N_{removed PU jets}/N_{total jets}");	

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
    drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_shape, hA_beforeSel_data_shape, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str());

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
    drawDataMcComparison("A_afterSel", hA_afterSel_mc_shape, hA_afterSel_data_shape, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      DeltaPhi_METRecoil afterSel 
    //
    //************************************************************************************************************	

    TH1F* hDeltaPhi_METRecoil_afterSel_mc_shape=(TH1F*)hDeltaPhi_METRecoil_afterSel_mc_lumi->Clone();
    TH1F* hDeltaPhi_METRecoil_afterSel_data_shape=(TH1F*)hDeltaPhi_METRecoil_afterSel_data_lumi->Clone();

    h1_style(hDeltaPhi_METRecoil_afterSel_mc_shape);
    h1_style(hDeltaPhi_METRecoil_afterSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_DeltaPhi_METRecoil_afterSel_mc = hDeltaPhi_METRecoil_afterSel_mc_shape->Integral();
    float Nentries_DeltaPhi_METRecoil_afterSel_Data = hDeltaPhi_METRecoil_afterSel_data_shape->Integral();	
    hDeltaPhi_METRecoil_afterSel_mc_shape->Scale(Nentries_DeltaPhi_METRecoil_afterSel_Data/Nentries_DeltaPhi_METRecoil_afterSel_mc);

    myHistoName = "images/variables/DeltaPhi_METRecoil_afterSel_shape_inLogScale" + extension;
    drawDataMcComparison("DeltaPhi_METRecoil_afterSel", hDeltaPhi_METRecoil_afterSel_mc_shape, hDeltaPhi_METRecoil_afterSel_data_shape, "|#Delta#phi (MET,Recoil)|", myHistoName.c_str());

    //************************************************************************************************************
    //
    //                                      DeltaPhi_METJet1 afterSel 
    //
    //************************************************************************************************************	

    TH1F* hDeltaPhi_METJet1_afterSel_mc_shape=(TH1F*)hDeltaPhi_METJet1_afterSel_mc_lumi->Clone();
    TH1F* hDeltaPhi_METJet1_afterSel_data_shape=(TH1F*)hDeltaPhi_METJet1_afterSel_data_lumi->Clone();

    h1_style(hDeltaPhi_METJet1_afterSel_mc_shape);
    h1_style(hDeltaPhi_METJet1_afterSel_data_shape);

    //rescale the Monte Carlo histogramm with number of entries
    float Nentries_DeltaPhi_METJet1_afterSel_mc = hDeltaPhi_METJet1_afterSel_mc_shape->Integral();
    float Nentries_DeltaPhi_METJet1_afterSel_Data = hDeltaPhi_METJet1_afterSel_data_shape->Integral();	
    hDeltaPhi_METJet1_afterSel_mc_shape->Scale(Nentries_DeltaPhi_METJet1_afterSel_Data/Nentries_DeltaPhi_METJet1_afterSel_mc);

    myHistoName = "images/variables/DeltaPhi_METJet1_afterSel_shape_inLogScale" + extension;
    drawDataMcComparison("DeltaPhi_METJet1_afterSel", hDeltaPhi_METJet1_afterSel_mc_shape, hDeltaPhi_METJet1_afterSel_data_shape, "|#Delta#phi (MET,1^{st}jet)|", myHistoName.c_str());

    //*******************************************************************************************************

    if(inLinScale) {
      myHistoName = "images/variables/MJB_inclusive_shape_inLinScale" + extension;
      drawDataMcComparison("MJB_inclusive", hMJB_inclusive_mc_shape, hMJB_inclusive_data_shape, "MJB", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("Met_beforeSel", hMet_beforeSel_mc_shape, hMet_beforeSel_data_shape, "MET [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Met_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("Met_afterSel", hMet_afterSel_mc_shape, hMet_afterSel_data_shape, "MET [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_beforeSel", hLeadingJetPt_beforeSel_mc_shape, hLeadingJetPt_beforeSel_data_shape, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/LeadingJetPt_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("LeadingJetPt_afterSel", hLeadingJetPt_afterSel_mc_shape, hLeadingJetPt_afterSel_data_shape, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);

      //myHistoName = "images/variables/LeadingJetMass_afterSel_shape_inLinScale" + extension;
      //drawDataMcComparison("LeadingJetMass_afterSel", hLeadingJetMass_afterSel_mc_shape, hLeadingJetMass_afterSel_data_shape, "M^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);

      //myHistoName = "images/variables/LeadingAndSecondJetsMass_afterSel_shape_inLinScale" + extension;
      /*drawDataMcComparison("LeadingAndSecondJetsMass_afterSel", hLeadingAndSecondJetsMass_afterSel_mc_shape, hLeadingAndSecondJetsMass_afterSel_data_shape, "M^{j_{1}+j_{2}} [GeV]", myHistoName.c_str(), inLinScale);*/

      //myHistoName = "images/variables/SecondJetPt_beforeSel_shape_inLinScale" + extension;
      //drawDataMcComparison("SecondJetPt_beforeSel", hSecondJetPt_beforeSel_mc_shape, hSecondJetPt_beforeSel_data_shape, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);
      //myHistoName = "images/variables/SecondJetPt_afterSel_shape_inLinScale" + extension;
      /*drawDataMcComparison("SecondJetPt_afterSel", hSecondJetPt_afterSel_mc_shape, hSecondJetPt_afterSel_data_shape, "p_{T}^{leading jet} [GeV]", myHistoName.c_str(), inLinScale);*/

      myHistoName = "images/variables/JetsPt_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("JetsPt_beforeSel", hJetsPt_beforeSel_mc_shape, hJetsPt_beforeSel_data_shape, "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/JetsPt_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("JetsPt_afterSel", hJetsPt_afterSel_mc_shape, hJetsPt_afterSel_data_shape, "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale);

      myHistoName = "images/variables/JetsEta_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("JetsEta_beforeSel", hJetsEta_beforeSel_mc_shape, hJetsEta_beforeSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/JetsEta_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("JetsEta_afterSel", hJetsEta_afterSel_mc_shape, hJetsEta_afterSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);     

/*      myHistoName = "images/variables/LeadingJetEta_beforeSel_shape_inLinScale" + extension;*/
      //drawDataMcComparison("LeadingJetEta_beforeSel", hLeadingJetEta_beforeSel_mc_shape, hLeadingJetEta_beforeSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);
      //myHistoName = "images/variables/LeadingJetEta_afterSel_shape_inLinScale" + extension;
      //drawDataMcComparison("LeadingJetEta_afterSel", hLeadingJetEta_afterSel_mc_shape, hLeadingJetEta_afterSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale); 

      //myHistoName = "images/variables/RecoilEta_beforeSel_shape_inLinScale" + extension;
      //drawDataMcComparison("RecoilEta_beforeSel", hRecoilEta_beforeSel_mc_shape, hRecoilEta_beforeSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale);
      //myHistoName = "images/variables/RecoilEta_afterSel_shape_inLinScale" + extension;
      /*drawDataMcComparison("RecoilEta_afterSel", hRecoilEta_afterSel_mc_shape, hRecoilEta_afterSel_data_shape, "#eta^{jet} [rad]", myHistoName.c_str(), inLinScale); */

      myHistoName = "images/variables/JetsPhi_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("JetsPhi_beforeSel", hJetsPhi_beforeSel_mc_shape, hJetsPhi_beforeSel_data_shape, "#phi^{jet} [rad]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/JetsPhi_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("JetsPhi_afterSel", hJetsPhi_afterSel_mc_shape, hJetsPhi_afterSel_data_shape, "#phi^{jet} [rad]", myHistoName.c_str(), inLinScale); 

      myHistoName = "images/variables/Njets_ptSup30_etaInf5_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("Njets_ptSup30_etaInf5_beforeSel", hNjets_ptSup30_etaInf5_beforeSel_mc_shape, hNjets_ptSup30_etaInf5_beforeSel_data_shape, "N_{jets} with p_{T} < 30 GeV and |#eta| < 5.0", myHistoName.c_str(), inLinScale);

      myHistoName = "images/variables/HT_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("HT_afterSel", hHT_afterSel_mc_shape, hHT_afterSel_data_shape, "HT [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/HT_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("HT_beforeSel", hHT_beforeSel_mc_shape, hHT_beforeSel_data_shape, "HT [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/RecoilPt_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_beforeSel", hRecoilPt_beforeSel_mc_shape, hRecoilPt_beforeSel_data_shape, "p_{T}^{Recoil} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/RecoilPt_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("RecoilPt_afterSel", hRecoilPt_afterSel_mc_shape, hRecoilPt_afterSel_data_shape, "p_{T}^{Recoil} [GeV]", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_beforeSel_shape_inLinScale" + extension;
      drawDataMcComparison("Npv_beforeSel", hNpv_beforeSel_mc_shape, hNpv_beforeSel_data_shape, "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/Npv_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("Npv_afterSel", hNpv_afterSel_mc_shape, hNpv_afterSel_data_shape, "N_{PV}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/NjetsRecoil_shape_inLinScale" + extension;
      drawDataMcComparison("NjetsRecoil", hNjetsRecoil_mc_shape, hNjetsRecoil_data_shape, "N_{jets in recoil}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/FracRmPuJets_JetPt_shape_inLinScale" + extension;
      drawDataMcComparison("FracRmPuJets_JetPt", hFracRmPuJets_JetPt_mc_shape, hFracRmPuJets_JetPt_data_shape, "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale, "N_{removed PU jets}/N_{total jets}");
      myHistoName = "images/variables/FracRmPuJets_JetPt_zoomed_shape_inLinScale" + extension;
      drawDataMcComparison("FracRmPuJets_JetPt_zoomed", hFracRmPuJets_JetPt_mc_shape_zoomed, hFracRmPuJets_JetPt_data_shape_zoomed, "p_{T}^{jet} [GeV]", myHistoName.c_str(), inLinScale, "N_{removed PU jets}/N_{total jets}");
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
      drawDataMcComparison("A_beforeSel", hA_beforeSel_mc_shape, hA_beforeSel_data_shape, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/A_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("A_afterSel", hA_afterSel_mc_shape, hA_afterSel_data_shape, "A = p_{T}^{jet 2}/p_{T}^{Recoil}", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/DeltaPhi_METRecoil_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("DeltaPhi_METRecoil_afterSel", hDeltaPhi_METRecoil_afterSel_mc_shape, hDeltaPhi_METRecoil_afterSel_data_shape, "|#Delta#phi (MET,Recoil)|", myHistoName.c_str(), inLinScale);
      myHistoName = "images/variables/DeltaPhi_METJet1_afterSel_shape_inLinScale" + extension;
      drawDataMcComparison("DeltaPhi_METJet1_afterSel", hDeltaPhi_METJet1_afterSel_mc_shape, hDeltaPhi_METJet1_afterSel_data_shape, "|#Delta#phi (MET,Jet1)|", myHistoName.c_str(), inLinScale);
      for(int j=0; j<myPtBinning.getSize(); j++) {
        binName = myPtBinning.getName(j);
        myName = "MJB_{data}/MJB_{MC} for " + binName;
        //myXName = "MJB for " + binName;
        std::stringstream PtBinningName ;
        PtBinningName.str("");
        PtBinningName << "MJB response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
        myXName = PtBinningName.str();
        mySaveName = "images/MJBperRefObjPt/MJB_" + binName + "_shape_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_shape[j], vMJB_RefObjPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
        mySaveName =  "images/MJBperRefObjPt/MJB_" + binName + "_shape_inLinScale_woRatio" + extension;
        drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_shape[j], vMJB_RefObjPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale, "", "r", 0);


        myName = "MPF_{data}/MPF_{MC} for " + binName;
        //myXName = "MPF for " + binName;
        PtBinningName.str("");
        PtBinningName << "MPF response (" << (int) myPtBinning.getBinValueInf(j) << " #leq p_{T,recoil} < " << (int) myPtBinning.getBinValueSup(j) << " GeV)";
        myXName = PtBinningName.str();
        mySaveName = "images/MPFperRefObjPt/MPF_" + binName + "_shape_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vMPF_RefObjPt_mc_shape[j], vMPF_RefObjPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
        mySaveName =  "images/MPFperRefObjPt/MPF_" + binName + "_shape_inLinScale_woRatio" + extension;
        drawDataMcComparison(myName.c_str(), vMJB_RefObjPt_mc_shape[j], vMJB_RefObjPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale, "", "r", 0);


        myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
        myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
        mySaveName = "images/LeadingJetPtperRefObjPt/LeadingJetPtperPt_" + binName + "_shape_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vLeadingJetPt_RecoilPt_mc_shape[j], vLeadingJetPt_RecoilPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);

        myName = "pT^{Recoil}_{data}/pT^{Recoil}_{MC} for " + binName;
        myXName = "pT^{Recoil} for " + binName + " [GeV]";
        mySaveName = "images/RecoilPtperRefObjPt/RecoilPtperPt_" + binName + "_shape_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vRecoilPt_RefObjPt_mc_shape[j], vRecoilPt_RefObjPt_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }
      for(int j=0; j<myHLTPtBinning.getSize(); j++) {
        binName = myHLTPtBinning.getName(j);
        myName = "pT^{Leading Jet}_{data}/pT^{Leading Jet}_{MC} for " + binName;
        myXName = "pT^{Leading Jet} for " + binName + " [GeV]";
        mySaveName = "images/LeadingJetPtperPtHLT/LeadingJetPt_" + binName + "_shape_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vLeadingJetPt_RefObjPtHLT_mc_shape[j], vLeadingJetPt_RefObjPtHLT_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
      }

      for(int j=0; j<myEtaBinning.getSize(); j++) {
        binName = myEtaBinning.getName(j);
        myName = "MJB_{data}/MJB_{MC} for " + binName;
        myXName = "MJB for " + binName;
        mySaveName = "images/MJBperRefObjEta/MJB_" + binName + "_shape_inLinScale" + extension;
        drawDataMcComparison(myName.c_str(), vMJB_RefObjEta_mc_shape[j], vMJB_RefObjEta_data_shape[j], myXName.c_str(), mySaveName.c_str(), inLinScale);
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
