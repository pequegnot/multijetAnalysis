#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/07Jul14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting.root ../analysis/output_rootfile/30Jun14/MC/MULTIJET_MC_QCD-Flat-15to3000_analysis_woPU_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix.root --lumi --shape --lin --rmPU --recoilPtBin --recoilPtHLTBin --plotName \"_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting_QCD-Pt-Flat\" --extension \".pdf\""
os.system(cmd);

