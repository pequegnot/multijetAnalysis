#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # QCD HT

        ['MULTIJET_QCD_Pt-15to3000_Flat_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root', 'list/MULTIJET_QCD_Pt-15to3000_Flat_woPU_pt10_eta50_puJetIdT_type1fix_30Jun14.list', 9989730, 2.99815997E10],

        ]


def launch(input, output, nevents, xsection):
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--mc", "--Nevents", str(nevents), "--Xsection", str(xsection), "--recoilPtBin", "--recoilPtHLTBin"]

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

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "4"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/MC/MULTIJET_MC_QCD-Flat_merged_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root" % (d)]
path = "output_rootfile/%s/MC" % (d)
for output in inputs:
  args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args

