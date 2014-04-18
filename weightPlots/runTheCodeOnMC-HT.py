#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # QCD HT
        ['MULTIJET_QCD_HT-100To250_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_HT-100To250_woPU_pt25_eta50_puJetIdMT_26Mar14.list', 50097518, 10360000],
        ['MULTIJET_QCD_HT-250To500_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_HT-250To500_woPU_pt25_eta50_puJetIdMT_26Mar14.list', 27062078, 276000],
        ['MULTIJET_MC_QCD_HT-500To1000_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_HT-500To1000_woPU_pt25_eta50_puJetIdMT_18Feb14.list', 30249292, 8426],
        ['MULTIJET_MC_QCD_HT-1000ToInf_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_HT-1000ToInf_woPU_pt25_eta50_puJetIdMT_18Feb14.list', 13808863,204],
        ]

def launch(input, output, nevents, xsection):
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--mc", "--Nevents", str(nevents), "--Xsection", str(xsection)]

    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

# Build output tree structure

path = "output_rootfile/%s/MC" % (d)
try:
    os.makedirs(path)
except:
    pass

print("Extracting dataset ...")
for input in inputs:
    path = "output_rootfile/%s/MC" % (d)
    tmpfile.write(launch(input[1], os.path.join(path, input[0]), input[2], input[3]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "8"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt25_eta50_puJetIdMT_%s.root" % (d,d)]
path = "output_rootfile/%s/MC" % (d)
for output in inputs:
  args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args
