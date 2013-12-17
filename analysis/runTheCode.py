#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        ['MULTIJET_MC_QCD_HT-500ToInf_analysis.root', '../weightPlots/output_rootfile/17Dec13/MC/MULTIJET_MC_QCD_HT_merged_17Dec13.root', "isMC"],
	['MULTIJET_Run2012ABCD-22Jan2013_analysis.root', '../weightPlots/output_rootfile/17Dec13/data/MULTIJET_Data_merged_2012_17Dec13.root', "isData"],

        ]

def launch(input, output, type):
    if type == "isMC":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--mc"]
    elif type == "isData":
        args = ["./multijet_analysis_common", "-i", input, "-o", output, "--data"]

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
    tmpfile.write(launch(input[1], os.path.join(path, input[0]), input[2]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "6"]
subprocess.call(args)
#print args

