#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
    ['MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/02Jun14_nominal/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_puJetIdT.root', "isMC", "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_QCD-HT_newHLTSel", ".pdf"],
    ['MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting.root', '../weightPlots/output_rootfile/02Jun14_nominal/data/MULTIJET_Data_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting.root', "isData", "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting_newHLTSel", ".pdf"],
    
    #['MULTIJET_MC_QCD-HT-500ToInf_analysis_woPU_pt35_eta50_puJetIdMT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/22Apr14/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt35_eta50_puJetIdT_22Apr14.root', "isMC", "_pt35_eta50_puJetIdMT_HLTsel_woPtRecoilCut_QCD-HT", ".pdf"],
    #['MULTIJET_Run2012D-22Jan2013_analysis_woPU_pt35_eta50_puJetIdMT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/22Apr14/data/MULTIJET_Data_runD_merged_2012_22Apr14.root', "isData", "_pt35_eta50_puJetIdMT_HLTsel_woPtRecoilCut_runD", ".pdf"],


    #['MULTIJET_Run2012A-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Jet_Run2012A-22Jan2013.root', "isData", "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_runA", ".pdf"],
    #['MULTIJET_Run2012B-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Data_runB_merged_2012_21Apr14.root', "isData", "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_runB", ".pdf"],
    #['MULTIJET_Run2012C-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Data_runC_merged_2012_21Apr14.root', "isData", "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_runC", ".pdf"],
    #['MULTIJET_Run2012D-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root', '../weightPlots/output_rootfile/21Apr14/data/MULTIJET_Data_runD_merged_2012_21Apr14.root', "isData", "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_runD", ".pdf"],

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

