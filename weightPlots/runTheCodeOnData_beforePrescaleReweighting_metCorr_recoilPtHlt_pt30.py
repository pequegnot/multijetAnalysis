#!/usr/bin/env python

import os, subprocess, datetime, tempfile, json

d = datetime.datetime.now().strftime("%d%b%y")
#d = "19Dec15" 

#with open('prescaleWeight_nominal_run2.json') as f:
with open('prescaleWeight_nominal_run2_startingAtHT500.json') as f:
     weights = json.load(f)

inputs = [
        # RunC
        [
            ['MULTIJET_data_JetHT_Run2015C_miniAOD_woPU_pt30_eta25_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015C_miniAOD_woPU_pt30_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', weights["runC_JetHT"], 'runC']            
        ],

        # RunD
        [
            ['MULTIJET_data_JetHT_Run2015D_miniAOD_woPU_pt30_eta25_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015D_miniAOD_woPU_pt30_eta25_notRmPUJets_Fall15_25nsV1_15Feb16.list', weights["runD_JetHT"], 'runD']
        ],



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
subprocess.call(args)
#print args

# All is done, merge

#print("Merging run by run...") # only one file per run here
#for run in inputs:
    #args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_JetHT_%s_merged_2015_woPU_pt30_eta25_notRmPUJets_beforePrescaleReweighting.root" % (d,run[0][3])]
    #path = "output_rootfile/%s/data" % (d)
    #for output in run:
      #args.append(os.path.join(path,output[0]))
    #subprocess.call(args)
    ##print args

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_merged_2015_pt30_eta25_beforePrescaleReweighting.root" % (d)]
path = "output_rootfile/%s/data" % (d)
for outputs in inputs:
    for output in outputs:
       args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args



