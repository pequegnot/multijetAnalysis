#!/usr/bin/env python

import os, subprocess, datetime, tempfile, json

d = datetime.datetime.now().strftime("%d%b%y")
#d = "30Apr16" 

with open('prescaleWeight_nominal.json') as f:
#with open('prescaleWeight_nominal_run2.json') as f:
     weights = json.load(f)

#inputs = [
##        # RunA
        ##[
            ##['MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list',  weights["runA_Jet"], 'runA']
        ##],

        ### RunB
        ##[
            ##['MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runB_JetHT"], 'runB'],
            ##['MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list',  weights["runB_JetMon"], 'runB']
        ##],

        ### RunC
        ##[
            ##['MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runC_JetHT"], 'runC'],
            ##['MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runC_JetMon"], 'runC']            
        ##],

        ### RunD
        ##[
            ##['MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runD_JetHT"], 'runD'],
            ##['MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runD_JetMon"], 'runD']
        ##]
##        [
            ##['MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runD_JetHT"], 'runD']
            ##],
        ##[
            ##['MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt10_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt10_eta50_puJetIdT_type1fix.list', weights["runD_JetHT"], 'runD'],
            ##],
        ##[
            ##['MULTIJET_JetHT_7_4_X_RunD_25Jun15_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_7_4_X_RunD_25Jun15_woPU_pt30_eta50_puJetIdT.list', weights["runD_JetHT"], 'runD'],
            ##],


##            [
            ##['MULTIJET_data_JetHT_Run2015B-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015B-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingSummer15JEC_24Aug15.list', weights["runB_JetHT"], 'runB'],
            ##],
##            [
            ##['MULTIJET_data_JetHT_Run2015C-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015C-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_usingGT_16Sep15.list', weights["runC_JetHT"], 'runC'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015C-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015C-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingGT_16Sep15.list', weights["runC_JetHT"], 'runC'],
            ##],
            
##            [
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV3_28Sep15.list', weights["runD_JetHT"], 'runD'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV3_28Sep15.list', weights["runD_JetHT"], 'runD'],
            ##],

            ## closure
##            [
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting_closure.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV5_closure_06Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_closure.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV5_closure_06Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],

            ## Only L2Residual on data
##            [
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV5_onlyL2Res_06Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV5_onlyL2Res_06Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],

            ## Only L2Residual on data, more stat
##            [
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV5_L2ResOnlyOnData_moreStat_15Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV5_L2ResOnlyOnData_moreStat_15Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],

            ## Only L2Residual on data, more stat, 19Oct15
##            [
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV5_L2ResOnlyOnData_json19Oct15_26Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV5_L2ResOnlyOnData_json19Oct15_26Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],

            ## json 19Oct15, correct datasets
##        [
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_25ns_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json19Oct15.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_25ns_miniAOD_woPU_pt30_eta50_notRmPUJets_json19Oct15_27Oct15.list', weights["runD_JetHT"], 'runD'],
            ##['MULTIJET_data_JetHT_Run2015D-reMiniAOD_25ns_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json19Oct15.root', 'list/MULTIJET_data_JetHT_Run2015D-reMiniAOD_25ns_miniAOD_woPU_pt30_eta50_notRmPUJets_json19Oct15_27Oct15.list', weights["runD_JetHT"], 'runD']
        ##],

            ## json 13Nov15, correct datasets
        #[
            #['MULTIJET_data_JetHT_Run2015D-PromptReco_25ns_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_25ns_miniAOD_pt30_notRmPUJets_json13Nov15_rm2ndJetPtCut_18Dec15.list', weights["runD_JetHT_miniAOD"], 'runD'],
            #['MULTIJET_data_JetHT_Run2015D-reMiniAOD_25ns_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15.root', 'list/MULTIJET_data_JetHT_Run2015D-reMiniAOD_25ns_miniAOD_pt30_notRmPUJets_json13Nov15_rm2ndJetPtCut_18Dec15.list', weights["runD_JetHT_reMiniAOD"], 'runD']
        #],

            ## Only L2Residual on data, DCSOnly
##            [
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res_DCSOnly.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV5_L2ResOnlyOnData_DCSOnly_15Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_onlyL2Res_DCSOnly.root', 'list/MULTIJET_data_JetHT_Run2015D-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV5_L2ResOnlyOnData_DCSOnly_15Oct15.list', weights["runD_JetHT"], 'runD'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015B-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015B-PromptReco_miniAOD_woPU_pt30_eta50_notRmPUJets.list', weights["runB_JetHT"], 'runB'],
            ##],
            ##[
            ##['MULTIJET_data_JetHT_Run2015B-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets_beforePrescaleReweighting.root', 'list/MULTIJET_data_JetHT_Run2015B-PromptReco_miniAOD_woPU_pt10_eta50_notRmPUJets.list', weights["runB_JetHT"], 'runB'],
            ##],
##        [
            ##['MULTIJET_JetHT_7_4_X_RunD_25Jun15_woPU_pt10_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_7_4_X_RunD_25Jun15_woPU_pt10_eta50_puJetIdT.list', weights["runD_JetHT"], 'runD'],
            ##],


    #]


inputs = [
        # RunA
        [
            ['MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_Jet_Run2012A-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list',  weights["runA_Jet"], 'runA']
        ],

        # RunB
        [
            ['MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runB_JetHT"], 'runB'],
            ['MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012B-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list',  weights["runB_JetMon"], 'runB']
        ],

        # RunC
        [
            ['MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runC_JetHT"], 'runC'],
            ['MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012C-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runC_JetMon"], 'runC']            
        ],

        # RunD
        [
            ['MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetHT_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runD_JetHT"], 'runD'],
            ['MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_recoilPtHLTBin_puJetIdT_type1fix_beforePrescaleReweighting.root', 'list/MULTIJET_JetMon_Run2012D-22Jan2013_woPU_pt30_eta50_puJetIdT_type1fix.list', weights["runD_JetMon"], 'runD']
        ]
        
    ]

# For prescale factors in run 2, see https://docs.google.com/spreadsheets/d/1qZaFG4HYs_s5JfoFN60Iw1yX521OYJjyCyIFq8N5zAU/edit#gid=0

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

print("Merging run by run...")
for run in inputs:
    args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_%s_merged_2012_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root" % (d,run[0][3])]
    #args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_JetHT_%s_merged_2015_woPU_pt30_eta50_notRmPUJets_beforePrescaleReweighting_json13Nov15.root" % (d,run[0][3])]
    path = "output_rootfile/%s/data" % (d)
    for output in run:
      args.append(os.path.join(path,output[0]))
    subprocess.call(args)
    #print args

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/data/MULTIJET_Data_merged_2012_woPU_pt30_eta50_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root" % (d)]
path = "output_rootfile/%s/data" % (d)
for outputs in inputs:
    for output in outputs:
       args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args


