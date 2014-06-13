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

void fit() {

  setTDRStyle();

  TCanvas *c = new TCanvas("c", "c", 800, 800);

  TGraphErrors * mjb_response = (TGraphErrors*) _file0->Get("MJB/LogPtPrimeBin/gMJB_LogPtPrimeBin_RecoilPt");
  mjb_response->SetTitle("");
  mjb_response->GetYaxis()->SetRangeUser(0.85, 1.15);
  mjb_response->GetXaxis()->SetTitle("p_{T}^{Recoil}.exp(#sum_{i}F_{i}.log(f_{i}))");
  mjb_response->GetYaxis()->SetTitle("MJB");
  mjb_response->SetLineColor(TColor::GetColor("#542437"));
  mjb_response->SetLineColor(TColor::GetColor("#8A9B0F"));
  mjb_response->SetLineColor(TColor::GetColor("#C02942"));
  mjb_response->SetMarkerColor(mjb_response->GetLineColor());
  mjb_response->SetMarkerStyle(20);
  mjb_response->SetMarkerSize(1.5);
  //mjb_response->SetFillColor(mjb->GetLineColor());
  //mjb_response->SetFillStyle(3004);
  mjb_response->GetXaxis()->SetTitleOffset(1.5);
  mjb_response->GetYaxis()->SetTitleOffset(1.7);

  TF1 *line = new TF1("line", "pol1", 0, 2000);
  mjb_response->Fit(line, "QN");

  line->SetLineColor(TColor::GetColor("#8A9B0F"));
  line->SetLineWidth(2);

  mjb_response->Draw("AP");
  line->Draw("same");
  mjb_response->Draw("P same");

  TLatex* latex = new TLatex();
  latex->SetTextFont(42);
  latex->SetTextSize(0.033);
  latex->DrawLatexNDC(0.55, 0.84, TString::Format("p_{0} = %.3f #pm %.3f", line->GetParameter(0), line->GetParError(0)));
  latex->DrawLatexNDC(0.55, 0.8, TString::Format("p_{1} = %.3f #pm %.2e", line->GetParameter(1), line->GetParError(1)));

  gPad->Modified();
  gPad->Update();

  c->Print("images/MJB/mjb_response_fit.pdf");
}

