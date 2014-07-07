#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
    ['MULTIJET_MC_QCD-Pt-herwig-120ToInf_analysis_woPU_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix.root', '../weightPlots/output_rootfile/01Jul14/MC/MULTIJET_MC_QCD-Pt-herwig_merged_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root', "isMC", "_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix_QCD-Pt-herwig", ".pdf"],
    #['MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting.root', '../weightPlots/output_rootfile/29Apr14/data/MULTIJET_Data_merged_2012_woPU_pt10_eta50_puJetIdT_afterPrecaleReweighting.root', "isData", "_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting", ".pdf"],

    #['MULTIJET_Run2012A-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Jet_Run2012A-22Jan2013.root', "isData", "_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_runA", ".pdf"],
    #['MULTIJET_Run2012B-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Data_runB_merged_2012_21Apr14.root', "isData", "_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_runB", ".pdf"],
    #['MULTIJET_Run2012C-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Data_runC_merged_2012_21Apr14.root', "isData", "_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_runC", ".pdf"],
    #['MULTIJET_Run2012D-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Data_runD_merged_2012_21Apr14.root', "isData", "_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_runD", ".pdf"],


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

