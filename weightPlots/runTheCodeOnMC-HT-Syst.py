#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # QCD HT

        ['MULTIJET_QCD_HT-100To250_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_%s.root', 'list/Systematics/MULTIJET_QCD_HT-100To250_%s_woPU_pt30_eta50_puJetIdT_type1fix.list', 50097518, 10360000],
        ['MULTIJET_QCD_HT-250To500_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_%s.root', 'list/Systematics/MULTIJET_QCD_HT-250To500_%s_woPU_pt30_eta50_puJetIdT_type1fix.list', 27062078, 276000],
        ['MULTIJET_MC_QCD_HT-500To1000_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_%s.root', 'list/Systematics/MULTIJET_QCD_HT-500To1000_%s_woPU_pt30_eta50_puJetIdT_type1fix.list', 30249292, 8426],
        ['MULTIJET_MC_QCD_HT-1000ToInf_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_%s.root', 'list/Systematics/MULTIJET_QCD_HT-1000ToInf_%s_woPU_pt30_eta50_puJetIdT_type1fix.list', 13808863,204],

        ]

systs = ["JECup", "JECdown", "JERup", "JERdown"]

def launch(input, output, nevents, xsection):
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--mc", "--Nevents", str(nevents), "--Xsection", str(xsection), "--recoilPtBin", "--recoilPtHLTBin"]

    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

# Build output tree structure

path = "output_rootfile/%s/MC/Systematics" % (d)
try:
    os.makedirs(path)
except:
    pass

print("Extracting dataset ...")
for input in inputs:
    path = "output_rootfile/%s/MC/Systematics" % (d)
    for syst in systs:
        tmpfile.write(launch(input[1] % syst, os.path.join(path, input[0] % syst), input[2], input[3]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "16"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging ...")

for syst in systs:
    args = ["hadd","-f", "output_rootfile/%s/MC/Systematics/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_%s.root" % (d, syst)]
    path = "output_rootfile/%s/MC/Systematics" % (d)
    for output in inputs:
        file = os.path.join(path, output[0] % syst)
        args.append(file)
    subprocess.call(args)
    #print args
