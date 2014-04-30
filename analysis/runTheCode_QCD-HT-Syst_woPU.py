#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [

    # Systematics HT
    ['MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_%s.root', '../weightPlots/output_rootfile/29Apr14/MC/Systematics/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_puJetIdT_%s.root', "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_QCD-HT-%s", ".pdf"],

        ]

systs = ["JECup", "JECdown", "JERup", "JERdown", "PUup", "PUdown"]

def launch(input, output, plotName, extension):
    args = ["./multijet_analysis_common", "-i", input, "-o", output, "--mc", "--rmPU", "--plotName", plotName, "--extension", extension]
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
        tmpfile.write(launch(input[1] % syst, os.path.join(path, input[0] % syst), input[2] % syst, input[3]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "6"]
subprocess.call(args)
#print args

