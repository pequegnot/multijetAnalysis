#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/01Jul14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting.root ../analysis/output_rootfile/23Jun14/MC/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin.root --lumi --shape --lin --rmPU --plotName \"_pt10_eta50_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix_afterPrescaleReweighting_withSystErrors_QCD-HT\" --extension \".pdf\"  --recoilPtBin --recoilPtHLTBin --useSyst --syst-file \"../compareMC/output_rootfile/compareMC_woPU_pt10_eta50_puJetIdT_woPtRecoilCut_recoilPtHLTBin_type1fix.root \" "
os.system(cmd);

