#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")
#d = "21Dec15"


inputs = [
        # QCD HT

#        ['MULTIJET_MC_QCD_HT100to200_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT100to200_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', 82095800, 211000000],
        #['MULTIJET_MC_QCD_HT200to300_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT200to300_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', 18784379, 1717000],
        #['MULTIJET_MC_QCD_HT300to500_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT300to500_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', , ],
        ['MULTIJET_MC_QCD_HT500to700_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT500to700_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', 19665695, 31630],
        ['MULTIJET_MC_QCD_HT700to1000_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT700to1000_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', 15547962, 6802],
        ['MULTIJET_MC_QCD_HT1000to1500_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT1000to1500_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', 5049267, 1206],
        ['MULTIJET_MC_QCD_HT1500to2000_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT1500to2000_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', 3939077, 120.4],
        ['MULTIJET_MC_QCD_HT2000toInf_woPU_pt10_eta25_notRmPUJets_recoilPtHLTBin.root', 'list/MULTIJET_MC_QCD_HT2000toInf_woPU_pt10_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', 1981228, 25.25]

        ]

def launch(input, output, nevents, xsection): # cross section in pb
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--mc", "--Nevents", str(nevents), "--Xsection", str(xsection), "--recoilPtBin", "--recoilPtHLTBin"]

    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

# Build output tree structure

#d = d + "_newMJBBinning"

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

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "20"]
subprocess.call(args)
#print args

# All is done, merge

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt10_eta25_recoilPtHLTBin_type1fix.root" % (d)]
path = "output_rootfile/%s/MC" % (d)
for output in inputs:
  args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args


