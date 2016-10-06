#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./compareCexp ../analysis/output_rootfile/25Jul14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting.root ../analysis/output_rootfile/25Jul14/MC/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root --lumi --shape --lin --rmPU --recoilPtBin --recoilPtHLTBin --plotName \"_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting_QCD-HT\""
os.system(cmd);

