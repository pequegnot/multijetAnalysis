#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        ['MULTIJET_Run2012A-22Jan2013_woPU_pt25_eta50_HLTsel.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt25_eta50_28Jan.list'],
        ['MULTIJET_Run2012B-22Jan2013_woPU_pt25_eta50_HLTsel.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt25_eta50_28Jan.list'],
        ['MULTIJET_Run2012C-22Jan2013_woPU_pt25_eta50_HLTsel.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt25_eta50_28Jan.list'],
        ['MULTIJET_Run2012D-22Jan2013_woPU_pt25_eta50_HLTsel.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt25_eta50_28Jan.list'],

        ]

def launch(input, output):
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--data"]

    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

# Build output tree structure

path = "output_rootfile/%s/data" % (d)
try:
    os.makedirs(path)
except:
    pass

print("Extracting dataset ...")
for input in inputs:
    path = "output_rootfile/%s/data" % (d)
    tmpfile.write(launch(input[1], os.path.join(path, input[0])) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "6"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_merged_2012_woPU_pt25_eta50_HLTsel_%s.root" % (d,d)]
path = "output_rootfile/%s/data" % (d)
for output in inputs:
  args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args

