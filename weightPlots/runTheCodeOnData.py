#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # RunA
        [
            ['MULTIJET_Jet_Run2012A-22Jan2013.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '2.13747611779145814e+02', '3.82073091664954134e+01', '9.61097999987588913e+00', '1', 'runA']
            #['MULTIJET_Jet_Run2012A-22Jan2013.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '(232.363 * (4.2863E4 / 4.1730E1) * (1.2598E2 / 1.4067E5))', '(46.129 * (4.2863E4 / 4.1730E1) * (4.4737E2 / 5.5479E5))', '(10.701 * (4.2863E4 / 4.1730E1) * (1.3979E3 / 1.5987E6))', '1', 'runA']
        ],

        # RunB
        [
            ['MULTIJET_JetHT_Run2012B-22Jan2013.root', 'list/MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '-1', '-1', '-1', '1', 'runB'],
            ['MULTIJET_JetMon_Run2012B-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '2.58902135637169238e+02', '5.99099629559211948e+01', '1.53526601878273130e+01', '-1', 'runB']
            #['MULTIJET_JetMon_Run2012B-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '271.730 * (2.0788E5 / 4.1730E1) * (1.2598E2 / 6.5867E5)', '69.120 * (2.0788E5 / 4.1730E1) * (4.4737E2 / 2.5712E6)', '16.634 * (2.0788E5 / 4.1730E1) * (1.3979E3 / 7.5449E6)', '-1', 'runB']
        ],

        # RunC
        [
            ['MULTIJET_JetHT_Run2012C-22Jan2013.root', 'list/MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '-1', '-1', '-1', '1', 'runC'],
            ['MULTIJET_JetMon_Run2012C-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '3.19487149007501216e+02', '5.97213928872980233e+01', '1.57037333357340572e+01', '-1', 'runC']
            #['MULTIJET_JetMon_Run2012C-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '404.194 * (2.0587E5 / 4.1730E1) * (1.2598E2 / 7.8629E5)', '80.840 * (2.0587E5 / 4.1730E1) * (4.4737E2 / 2.9875E6)', '20.143 * (2.0587E5 / 4.1730E1) * (1.3979E3 / 8.8459E6)', '-1', 'runC']            
        ],

        # RunD
        [
            ['MULTIJET_JetHT_Run2012D-22Jan2013.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '-1', '-1', '-1', '1', 'runD'],
            ['MULTIJET_JetMon_Run2012D-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '2.44358536823608944e+02', '4.50190143454263136e+01', '1.20060949461418502e+01', '-1', 'runD']
            #['MULTIJET_JetMon_Run2012D-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '409.434 * (2.1505E5 / 4.1730E1) * (1.2598E2 / 1.0878E6)', '81.885 * (2.1505E5 / 4.1730E1) * (4.4737E2 / 4.1934E6)', '20.471 * (2.1505E5 / 4.1730E1) * (1.3979E3 / 1.2283E7)', '-1', 'runD']
        ]

        #[
            #['MULTIJET_JetHT_Run2012D-22Jan2013.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt25_eta50_puJetIdMT_15Apr.list', '-1', '-1', '-1', '1', 'runD'],
            #['MULTIJET_JetMon_Run2012D-22Jan2013.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt25_eta50_puJetIdMT_15Apr.list', '409.434 * (2.1439 * pow(10, 3) / ( 4.1058 * pow(10, 1))) * (1.4188*pow(10,5))/(1.2423*pow(10, 7))', '81.885 * (2.1439 * pow(10, 5) / ( 4.1058 * pow(10, 1))) * 315.65/(3.0660*pow(10, 6))', '20.471 * (2.1439 * pow(10, 5) / ( 4.1058 * pow(10, 1))) * 98.117/(8.3836*pow(10, 5))', '-1', 'runD']
        #] 
        
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

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "8"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging run by run...")
for run in inputs:
  if len(run) > 1:
    args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_%s_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting_%s.root" % (d,run[0][6],d)]
    path = "output_rootfile/%s/data" % (d)
    for output in run:
      args.append(os.path.join(path,output[0]))
    subprocess.call(args)
    #print args

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting_%s.root" % (d,d)]
path = "output_rootfile/%s/data" % (d)
for outputs in inputs:
    for output in outputs:
       args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args

