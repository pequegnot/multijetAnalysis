#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # QCD Pt Pythia
        ['MULTIJET_MC_QCD_Pt-120to170_pythia_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-120to170_pythia_woPU_pt30_eta50_puJetIdT_23Apr14.list', 5935732, 156293.3],
        ['MULTIJET_MC_QCD_Pt-170to300_pythia_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-170to300_pythia_woPU_pt30_eta50_puJetIdT_23Apr14.list', 5814398, 34138.15],
        ['MULTIJET_MC_QCD_Pt-300to470_pythia_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-300to470_pythia_woPU_pt30_eta50_puJetIdT_23Apr14.list', 5978500, 1759.549],
        ['MULTIJET_MC_QCD_Pt-470to600_pythia_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-470to600_pythia_woPU_pt30_eta50_puJetIdT_23Apr14.list', 3844848, 113.8791],
        ['MULTIJET_MC_QCD_Pt-600to800_pythia_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-600to800_pythia_woPU_pt30_eta50_puJetIdT_23Apr14.list', 3996864, 26.9921],
        ['MULTIJET_MC_QCD_Pt-800to1000_pythia_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-800to1000_pythia_woPU_pt30_eta50_puJetIdT_23Apr14.list', 3998563, 3.550036],
        ['MULTIJET_MC_QCD_Pt-1000to1400_pythia_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-1000to1400_pythia_woPU_pt30_eta50_puJetIdT_24Apr14.list', 1964088, 0.737844],
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
args = ["hadd","-f", "output_rootfile/%s/MC/MULTIJET_MC_QCD-Pt-pythia_merged_woPU_pt30_eta50_puJetIdT_%s.root" % (d,d)]
path = "output_rootfile/%s/MC" % (d)
for output in inputs:
  args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args
