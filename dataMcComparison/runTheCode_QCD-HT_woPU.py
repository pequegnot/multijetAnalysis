#!/usr/bin/env python

import os, subprocess, datetime, tempfile

#cmd = "./dataMcComparison ../analysis/output_rootfile/17Feb16/data/MULTIJET_data_2015-merged_pt10_eta30_notRmPUJets_beforePrescaleReweighting.root ../analysis/output_rootfile/17Feb16/MC/MULTIJET_MC_QCD-HT-500ToInf_analysis_woPU_pt10_eta30_notRmPUJets.root --lumi --shape --lin --recoilPtBin --recoilPtHLTBin --plotName \"_pt10_eta30_notRmPUJets_beforePrescaleReweighting_QCD-HT_13TeV_25ns\" --extension \".pdf\""
#cmd = "./dataMcComparison ../analysis/output_rootfile/17Feb16/data/MULTIJET_data_2015-merged_pt30_eta30_notRmPUJets_beforePrescaleReweighting.root ../analysis/output_rootfile/17Feb16/MC/MULTIJET_MC_QCD-HT-500ToInf_analysis_woPU_pt30_eta30_notRmPUJets.root --lumi --shape --lin --recoilPtBin --recoilPtHLTBin --plotName \"_pt30_eta30_notRmPUJets_beforePrescaleReweighting_QCD-HT_13TeV_25ns\" --extension \".pdf\""
#cmd = "./dataMcComparison ../analysis/output_rootfile/17Feb16/data/MULTIJET_data_2015-merged_pt30_eta30_notRmPUJets_beforePrescaleReweighting_recoilBinFrom10FileForMPF.root ../analysis/output_rootfile/17Feb16/MC/MULTIJET_MC_QCD-HT-500ToInf_analysis_woPU_pt30_eta30_notRmPUJets_recoilBinFrom10FileForMPF.root --lumi --shape --lin  --recoilPtBin --recoilPtHLTBin --plotName \"_pt30_eta30_notRmPUJets_beforePrescaleReweighting_recoilBinFrom10FileForMPF_QCD-HT_13TeV_25ns\" --extension \".pdf\""


cmd = "./dataMcComparison ../analysis/output_rootfile/24Feb16/data/MULTIJET_data_2015-merged_pt10_eta30_notRmPUJets_beforePrescaleReweighting_closure.root ../analysis/output_rootfile/24Feb16/MC/MULTIJET_MC_QCD-HT-500ToInf_analysis_woPU_pt10_eta30_notRmPUJets.root --lumi --shape --lin --recoilPtBin --recoilPtHLTBin --plotName \"_pt10_eta30_notRmPUJets_beforePrescaleReweighting_QCD-HT_13TeV_25ns_closure\" --extension \".pdf\""
#cmd = "./dataMcComparison ../analysis/output_rootfile/24Feb16/data/MULTIJET_data_2015-merged_pt30_eta30_notRmPUJets_beforePrescaleReweighting_closure.root ../analysis/output_rootfile/24Feb16/MC/MULTIJET_MC_QCD-HT-500ToInf_analysis_woPU_pt30_eta30_notRmPUJets.root --lumi --shape --lin --recoilPtBin --recoilPtHLTBin --plotName \"_pt30_eta30_notRmPUJets_beforePrescaleReweighting_QCD-HT_13TeV_25ns_closure\" --extension \".pdf\""
#cmd = "./dataMcComparison ../analysis/output_rootfile/24Feb16/data/MULTIJET_data_2015-merged_pt30_eta30_notRmPUJets_beforePrescaleReweighting_recoilBinFrom10FileForMPF_closure.root ../analysis/output_rootfile/24Feb16/MC/MULTIJET_MC_QCD-HT-500ToInf_analysis_woPU_pt30_eta30_notRmPUJets_recoilBinFrom10FileForMPF.root --lumi --shape --lin  --recoilPtBin --recoilPtHLTBin --plotName \"_pt30_eta30_notRmPUJets_beforePrescaleReweighting_recoilBinFrom10FileForMPF_QCD-HT_13TeV_25ns_closure\" --extension \".pdf\""
os.system(cmd);

