#!/usr/bin/env python

import os, subprocess, datetime, tempfile, json

d = datetime.datetime.now().strftime("%d%b%y")

with open('prescaleWeight_reweighted.json') as f:
     weights = json.load(f)

inputs = [
        # RunA
        [
            ['MULTIJET_Jet_Run2012A-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list',  weights["runA_Jet"], 'runA']
        ],

        # RunB
        [
            ['MULTIJET_JetHT_Run2012B-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', weights["runB_JetHT"], 'runB'],
            ['MULTIJET_JetMon_Run2012B-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list',  weights["runB_JetMon"], 'runB']
        ],

        # RunC
        [
            ['MULTIJET_JetHT_Run2012C-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', weights["runC_JetHT"], 'runC'],
            ['MULTIJET_JetMon_Run2012C-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', weights["runC_JetMon"], 'runC']            
        ],

        # RunD
        [
            ['MULTIJET_JetHT_Run2012D-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', weights["runD_JetHT"], 'runD'],
            ['MULTIJET_JetMon_Run2012D-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', weights["runD_JetMon"], 'runD']
        ]
        
    ]

def launch(input, output, HLTPrescaleFactor):
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--data", "--recoilPtBin", "--recoilPtHLTBin"]
    for HLTPath, weight in HLTPrescaleFactor.items():
      opt = "--" + HLTPath
      args.append(opt)
      args.append(str(weight))
   
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
    for list in input:
        path = "output_rootfile/%s/data" % (d)
        tmpfile.write(launch(list[1], os.path.join(path, list[0]), list[2]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "7"]
#subprocess.call(args)
print args

## All is done, merge

print("Merging run by run...")
for run in inputs:
  if len(run) > 1:
    args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_%s_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting.root" % (d,run[0][3])]
    path = "output_rootfile/%s/data" % (d)
    for output in run:
      args.append(os.path.join(path,output[0]))
    #subprocess.call(args)
    print args

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting.root" % (d)]
path = "output_rootfile/%s/data" % (d)
for outputs in inputs:
    for output in outputs:
       args.append(os.path.join(path,output[0]))

#subprocess.call(args)
print args



