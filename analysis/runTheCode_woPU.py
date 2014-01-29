#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        ['MULTIJET_MC_QCD_HT-500ToInf_analysis_woPU_pt25_eta28.root', '../weightPlots/output_rootfile/29Jan14/MC/MULTIJET_MC_QCD_HT_merged_woPU_pt25_eta28_29Jan14.root', "isMC", "_pt25_eta28", ".pdf"],
	['MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt25_eta28.root', '../weightPlots/output_rootfile/29Jan14/data/MULTIJET_Data_merged_2012_woPU_pt25_eta28_29Jan14.root', "isData", "_pt25_eta28", ".pdf"],

        ]

def launch(input, output, type, plotName, extension):
    if type == "isMC":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--mc", "--rmPU", "--plotName", plotName, "--extension", extension]
    elif type == "isData":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--data", "--rmPU", "--plotName", plotName, "--extension", extension]

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
