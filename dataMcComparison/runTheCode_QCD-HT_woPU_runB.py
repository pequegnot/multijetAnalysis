#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/25Jul14/data/MULTIJET_Run2012B-22Jan2013_analysis_woPU_pt20_eta50_puJetIdT_recoilPtHLTBin_type1fix_beforePrescaleReweighting.root ../analysis/output_rootfile/25Jul14/MC/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt20_eta50_puJetIdT_recoilPtHLTBin_type1fix.root --lumi --shape --lin --rmPU --recoilPtBin --recoilPtHLTBin --plotName \"_pt20_eta50_puJetIdT_recoilPtHLTBin_type1fix_beforePrescaleReweighting_QCD-HT_runB\" --extension \".pdf\""
os.system(cmd);

