#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # RunA
        [
        ['MULTIJET_Jet_Run2012A-22Jan2013.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt25_eta50_puJetIdMT_08Apr.list', '232.363', '46.129', '10.701', '1', 'runA']
        ],

        # RunB
        [
            ['MULTIJET_JetHT_Run2012B-22Jan2013.root', 'list/MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt25_eta50_puJetIdMT_08Apr.list', '-1', '-1', '-1', '1', 'runB'],
            ['MULTIJET_JetMon_Run2012B-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt25_eta50_puJetIdMT_08Apr.list', '271.730', '69.120', '16.634', '-1', 'runB']
        ],

        # RunC
        [
            ['MULTIJET_JetHT_Run2012C-22Jan2013.root', 'list/MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt25_eta50_puJetIdMT_08Apr.list', '-1', '-1', '-1', '1', 'runC'],
            ['MULTIJET_JetMon_Run2012C-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt25_eta50_puJetIdMT_08Apr.list', '404.194', '80.840', '20.143', '-1', 'runC']
            
        ],

        # RunD
        [
            ['MULTIJET_JetHT_Run2012D-22Jan2013.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt25_eta50_puJetIdMT_08Apr.list', '-1', '-1', '-1', '1', 'runD'],
            ['MULTIJET_JetMon_Run2012D-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt25_eta50_puJetIdMT_08Apr.list', '409.434', '81.885', '20.471', '-1', 'runD']
        ]
        
    ]

def launch(input, output, HLT_PFJet140_prescaleFactor, HLT_PFJet200_prescaleFactor, HLT_PFJet260_prescaleFactor, HLT_PFJet320_prescaleFactor):
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--data", "--HLT_PFJet140", HLT_PFJet140_prescaleFactor, "--HLT_PFJet200", HLT_PFJet200_prescaleFactor, "--HLT_PFJet260", HLT_PFJet260_prescaleFactor, "--HLT_PFJet320", HLT_PFJet320_prescaleFactor]

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
        tmpfile.write(launch(list[1], os.path.join(path, list[0]), list[2], list[3], list[4], list[5]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "6"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging run by run...")
for run in inputs:
  if len(run) > 1:
    args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_%s_merged_2012_%s.root" % (d,run[0][6],d)]
    path = "output_rootfile/%s/data" % (d)
    for output in run:
      args.append(os.path.join(path,output[0]))
    subprocess.call(args)
    #print args

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_merged_2012_%s.root" % (d,d)]
path = "output_rootfile/%s/data" % (d)
for outputs in inputs:
    for output in outputs:
       args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args

