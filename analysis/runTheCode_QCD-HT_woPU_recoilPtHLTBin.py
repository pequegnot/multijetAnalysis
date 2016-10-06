#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")
#d = "22Dec15"

inputs = [
     # HT
    ['MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_notRmPUJets_recoilPtHLTBin_type1fix.root', '../weightPlots/output_rootfile/21Dec15/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_recoilPtHLTBin_type1fix.root', "isMC", "_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_QCD-HT", ".pdf"],
    ['MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt10_eta50_notRmPUJets_recoilPtHLTBin_type1fix.root', '../weightPlots/output_rootfile/21Dec15/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt10_eta50_recoilPtHLTBin_type1fix.root', "isMC", "_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix_QCD-HT", ".pdf"],

    # Data
    ['MULTIJET_data_JetHT_Run2015D-merged_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15.root', '../weightPlots/output_rootfile/21Dec15/data/MULTIJET_Data_JetHT_runD_merged_2015_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15.root', "isData", "_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15", ".pdf", "../weightPlots/output_rootfile/21Dec15/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_recoilPtHLTBin_type1fix.root"],

    ['MULTIJET_data_JetHT_Run2015D-merged_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15.root', '../weightPlots/output_rootfile/21Dec15/data/MULTIJET_Data_JetHT_runD_merged_2015_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15.root', "isData", "_pt10_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15", ".pdf", "../weightPlots/output_rootfile/21Dec15/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt10_eta50_recoilPtHLTBin_type1fix.root"],

        ]

def launch(input, output, type, plotName, extension, mcFile = ""):
    if type == "isMC":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--mc", "--rmPU", "--plotName", plotName, "--extension", extension, "--recoilPtBin", "--recoilPtHLTBin"]
    elif type == "isData":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--data", "--rmPU", "--plotName", plotName, "--extension", extension, "--recoilPtBin", "--recoilPtHLTBin", "--input-file-mc", mcFile]

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
        tmpfile.write(launch(input[1], os.path.join(path, input[0]), input[2], input[3], input[4]) + "\n")
    elif input[2] == "isData":
        path = "output_rootfile/%s/data" % (d)
        tmpfile.write(launch(input[1], os.path.join(path, input[0]), input[2], input[3], input[4], input[5]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "20"]
#subprocess.call(args)
print args

