#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # RunA
        [
            ['MULTIJET_Jet_Run2012A-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '2.10721839162138110e+02', '3.78209998246507837e+01', '9.70224208644777164e+00', '1', 'runA']
            #['MULTIJET_Jet_Run2012A-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '(232.363 * (3.7664E4 / 3.6134E1) * (1.0757E3 / 1.2364E6))', '(46.129 * (3.7664E4 / 3.6134E1) * (3.5277E2 / 4.4848E5))', '(10.701 * (3.7664E4 / 3.6134E1) * (1.0338E2 / 1.1885E5))', '1', 'runA']

            # wo PrecaleReweighting
            #['MULTIJET_Jet_Run2012A-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '232.363', '46.129', '10.701', '1', 'runA']
        ],

        # RunB
        [
            ['MULTIJET_JetHT_Run2012B-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '-1', '-1', '-1', '1', 'runB'],
            ['MULTIJET_JetMon_Run2012B-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '2.52535897218694799e+02', '5.99934292793522417e+01', '1.57449006357000290e+01', '-1', 'runB']
            #['MULTIJET_JetMon_Run2012B-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '271.730 * (1.8205E5 / 3.6134E1) * (1.0757E3 / 5.8315E6)', '69.120 * (1.8205E5 / 3.6134E1) * (3.5277E2 / 2.0477E6)', '16.634 * (1.8205E5 / 3.6134E1) * (1.0338E2 / 5.5026E5)', '-1', 'runB']

            # woPrescaleReweighting
            #['MULTIJET_JetMon_Run2012B-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '271.730', '69.120', '16.634', '-1', 'runB']
        ],

        # RunC
        [
            ['MULTIJET_JetHT_Run2012C-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '-1', '-1', '-1', '1', 'runC'],
            ['MULTIJET_JetMon_Run2012C-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '3.18261518091150890e+02', '5.99809805853585800e+01', '1.60202165337022890e+01', '-1', 'runC']
            #['MULTIJET_JetMon_Run2012C-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '404.194 * (1.7904E5 / 3.6134E1) * (1.0757E3 / 6.7691E6)', '80.840 * (1.7904E5 / 3.6134E1) * (3.5277E2 / 2.3558E6)', '20.143 * (1.7904E5 / 3.6134E1) * (1.0338E2 / 6.4406E5)', '-1', 'runC']

            # woPrescaleReweighting
            #['MULTIJET_JetMon_Run2012C-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '404.194', '80.840', '20.143', '-1', 'runC']            
        ],

        # RunD
        [
            ['MULTIJET_JetHT_Run2012D-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '-1', '-1', '-1', '1', 'runD'],
            ['MULTIJET_JetMon_Run2012D-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '2.43547613865758223e+02', '4.53539359489631977e+01', '1.22138448956136205e+01', '-1', 'runD']
            #['MULTIJET_JetMon_Run2012D-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '409.434 * (1.8643E5 / 3.6134E1) * (1.0757E3 / 9.3302E6)', '81.885 * (1.8643E5 / 3.6134E1) * (3.5277E2 / 3.2861E6)', '20.471 * (1.8643E5 / 3.6134E1) * (1.0338E2 / 8.9397E5)', '-1', 'runD']

            # woPrescaleReweighting
            #['MULTIJET_JetMon_Run2012D-22Jan2013_afterPrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_23Apr.list', '409.434', '81.885', '20.471', '-1', 'runD']
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

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "7"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging run by run...")
for run in inputs:
  #if len(run) > 1:
  args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_%s_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting.root" % (d,run[0][6])]
  path = "output_rootfile/%s/data" % (d)
  for output in run:
    args.append(os.path.join(path,output[0]))
  subprocess.call(args)
  #print args

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_merged_2012_woPU_pt30_eta50_puJetIdT_afterPrecaleReweighting.root" % (d)]
path = "output_rootfile/%s/data" % (d)
for outputs in inputs:
    for output in outputs:
       args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args

