#!/usr/bin/env python

import os, subprocess, datetime, tempfile, json, shutil
import HLTPtBinning

today = datetime.datetime.now().strftime("%d%b%y")

from optparse import OptionParser
from ROOT import TFile, TH1

parser = OptionParser()
parser.add_option("", "--runDependent", action="store_true", dest="run", default=True, help="Recompute prescale weight run by run")
parser.add_option("", "--useRecoilForHLT", action="store_true", dest="useRecoilForHLT", default=False, help="Use recoil pt for HLT binning")
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False, help="Print informations about weights computation")
parser.add_option("-d", "--date", action="store", dest="date", default=today, help="Date of the directory where your input files are stored")
(option, args) = parser.parse_args()

d = option.date

dest = "prescaleWeight_reweighted_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix.json"
shutil.copyfile("prescaleWeight_nominal.json", dest)

with open(dest, 'r') as f:
    weights = json.load(f)

runs = {
  "runA": "runA_Jet",
  "runB": "runB_JetMon",
  "runC": "runC_JetMon",
  "runD": "runD_JetMon",
        }

inputs_data = []

if option.run:
    for aRun, aDataset in runs.items():
        input = "output_rootfile/%s/data/MULTIJET_Data_%s_merged_2012_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root" % (d,aRun)
        #input = "output_rootfile/21Jun14/data/MULTIJET_Data_%s_merged_2012_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root" % (aRun)
        #input = "output_rootfile/%s/data_woPrescaleReweighting/MULTIJET_Data_%s_merged_2012.root" % (d,aRun)
        inputs_data.append(input)
else:
    input = "output_rootfile/%s/data/MULTIJET_Data_merged_2012_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root" % (d)
    inputs_data.append(input)

inputs_mc = ["output_rootfile/%s/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix.root" % (d)]
#inputs_mc = ["output_rootfile/08Jul14/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix.root"]

#inputs_data = ["output_rootfile/28Apr14_finalSel/data/MULTIJET_Data_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting.root"]
#inputs_mc = ["output_rootfile/28Apr14_finalSel/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_puJetIdT.root"]


myHLTPtBinning = HLTPtBinning.HLTPtBinning()
myHLTPtBinning.fillHLTPtBins(option.useRecoilForHLT)
numberHLTPtBins = myHLTPtBinning.getSize()
f_mc = TFile.Open(inputs_mc[0])
hmc = f_mc.Get("variables/afterSel/hLeadingJetPt_afterSel")
if option.useRecoilForHLT:
  hmc = f_mc.Get("variables/afterSel/hRecoilPt_afterSel")

for input_data in inputs_data:
  if option.verbose:
    print input_data
  f_data = TFile.Open(input_data)
  hdata = f_data.Get("variables/afterSel/hLeadingJetPt_afterSel")
  if option.useRecoilForHLT:
    hdata = f_data.Get("variables/afterSel/hRecoilPt_afterSel")
  hdata.Draw()

  valInf = myHLTPtBinning.getBinValueInf(numberHLTPtBins-1)
  valSup = myHLTPtBinning.getBinValueSup(numberHLTPtBins-1)
  Ndata_ref = hdata.Integral(hdata.FindBin(valInf), hdata.FindBin(valSup))
  Nmc_ref = hmc.Integral(hmc.FindBin(valInf), hmc.FindBin(valSup))

  if option.verbose:
    print "Ndata_ref: "
    print Ndata_ref
    print "Nmc_ref: "
    print Nmc_ref

  for i in range(0, numberHLTPtBins-1):
      valInf = myHLTPtBinning.getBinValueInf(i)
      valSup = myHLTPtBinning.getBinValueSup(i)
      HLTPath = myHLTPtBinning.getHLTName(i)
      Ndata = hdata.Integral(hdata.FindBin(valInf), hdata.FindBin(valSup)-1)
      Nmc = hmc.Integral(hmc.FindBin(valInf), hmc.FindBin(valSup)-1)
      weight = (Ndata_ref / Nmc_ref) * (Nmc / Ndata)

      if option.verbose:
        print "HLTPath: "
        print HLTPath
        print "HLT bin # " 
        print i
        print "valInf: " 
        print valInf
        print "valSup: " 
        print valSup
        print "Ndata: "
        print Ndata
        print "Nmc: " 
        print Nmc
        print "Weight: "
        print weight
        print "" 
      if option.run:
          for aRun, aDataset in runs.items():
              if aRun in input_data:
                  weights[aDataset][HLTPath] = weights[aDataset][HLTPath] * weight
                  if option.verbose:
                    print aRun
                    print aDataset
                    print "weights[aDataset][HLTPath]" 
                    print weights[aDataset][HLTPath]
      else:
          for aRun, aDataset in runs.items():
              weights[aDataset][HLTPath] = weights[aDataset][HLTPath] * weight

if option.verbose:
  print weights

with open(dest, 'w+') as f:
    json.dump(weights, f, indent=4)


