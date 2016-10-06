#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [

    # Systematics HT
    ['../analysis/output_rootfile/11Jan15/MC/Systematics/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix_%s.root', "_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_type1fix_QCD-HT-%s", ".pdf"],

        ]

systs = ["JECup", "JECdown", "JERup", "JERdown", "PUup", "PUdown"]

def launch(input, plotName, extension):
    args = ["./dataMcComparison", "../analysis/output_rootfile/25Jul14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting.root", input, "--lumi", "--shape", "--lin", "--rmPU", "--recoilPtBin", "--recoilPtHLTBin", "--plotName", plotName, "--extension", extension]
    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

print("Drawing plots...")
for input in inputs:
    for syst in systs:
        tmpfile.write(launch(input[0] % syst, input[1] % syst, input[2]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "6"]
subprocess.call(args)
#print args

