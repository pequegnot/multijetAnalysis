#!/usr/bin/env python

import os, subprocess, datetime, tempfile

#d = datetime.datetime.now().strftime("%d%b%y")
d = "25Jul14"

inputs = [
    ['MULTIJET_MC_QCD-Pt-pythia-120To1400_analysis_woPU_pt20_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix.root', '../weightPlots/output_rootfile/24Jul14/MC/MULTIJET_MC_QCD-Pt-pythia_merged_woPU_pt20_eta50_puJetIdT_recoilPtHLTBin_type1fix.root', "isMC", "_pt20_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix_QCD-Pt-pythia", ".pdf"],
    #['MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt20_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting.root', '../weightPlots/output_rootfile/29Apr14/data/MULTIJET_Data_merged_2012_woPU_pt20_eta50_puJetIdT_afterPrecaleReweighting.root', "isData", "_pt20_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting", ".pdf"],

        ]

def launch(input, output, type, plotName, extension):
    if type == "isMC":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--mc", "--rmPU", "--plotName", plotName, "--extension", extension, "--recoilPtBin", "--recoilPtHLTBin"]
    elif type == "isData":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--data", "--rmPU", "--plotName", plotName, "--extension", extension, "--recoilPtBin", "--recoilPtHLTBin"]

    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

# Build output tree structure

path = "output_rootfile/%s/data" % (d)
try:
    os.makedirs(path)
except:
    pass
    
path = "output_rootfile/%s/MC" % (d)
try:
    os.makedirs(path)
except:
    pass

print("Extracting dataset ...")
for input in inputs:
    if input[2] == "isMC":
        path = "output_rootfile/%s/MC" % (d)
    elif input[2] == "isData":
        path = "output_rootfile/%s/data" % (d)
    tmpfile.write(launch(input[1], os.path.join(path, input[0]), input[2], input[3], input[4]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "6"]
subprocess.call(args)
#print args

