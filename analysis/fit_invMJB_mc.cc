//Breit-Wigner function
Double_t mybw(Double_t* x, Double_t* par)
{
  Double_t arg1 = 14.0/22.0; // 2 over pi
  Double_t arg2 = par[1]*par[1]*par[2]*par[2]; //Gamma=par[1]  M=par[2]
  Double_t arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]));
  Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]));
  return par[0]*arg1*arg2/(arg3 + arg4);
}

#include "tdrstyle.C"

void fit_invMJB_mc() {

  setTDRStyle();

  TCanvas *c = new TCanvas("c", "c", 800, 800);

  TGraphErrors * invmjb_response = (TGraphErrors*) _file0->Get("MJB/LogPtPrimeBin/gInvMJB_LogPtPrimeBin_RecoilPt");
  invmjb_response->SetTitle("");
  invmjb_response->GetYaxis()->SetRangeUser(0.85, 1.15);
  invmjb_response->GetXaxis()->SetTitle("p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}))");
  invmjb_response->GetYaxis()->SetTitle("MJB^{-1}");
  invmjb_response->SetLineColor(TColor::GetColor("#542437"));
  invmjb_response->SetLineColor(TColor::GetColor("#8A9B0F"));
  invmjb_response->SetLineColor(TColor::GetColor("#C02942"));
  invmjb_response->SetMarkerColor(invmjb_response->GetLineColor());
  invmjb_response->SetMarkerStyle(20);
  invmjb_response->SetMarkerSize(1.5);
  //invmjb_response->SetFillColor(invmjb->GetLineColor());
  //invmjb_response->SetFillStyle(3004);
  invmjb_response->GetXaxis()->SetTitleOffset(1.5);
  invmjb_response->GetYaxis()->SetTitleOffset(1.7);

  TF1 *line = new TF1("line", "pol1", 0, 2000);
  invmjb_response->Fit(line, "QN");

  line->SetLineColor(TColor::GetColor("#8A9B0F"));
  line->SetLineWidth(2);

  invmjb_response->Draw("AP");
  line->Draw("same");
  invmjb_response->Draw("P same");

  TLatex* latex = new TLatex();
  latex->SetTextFont(42);
  latex->SetTextSize(0.033);
  latex->DrawLatexNDC(0.55, 0.84, TString::Format("p_{0} = %.3f #pm %.3f", line->GetParameter(0), line->GetParError(0)));
  latex->DrawLatexNDC(0.55, 0.8, TString::Format("p_{1} = %.3f #pm %.2e", line->GetParameter(1), line->GetParError(1)));

  gPad->Modified();
  gPad->Update();

  c->Print("images/MJB/invmjb_response_fit_pt20_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix_QCD-HT.pdf");
}



