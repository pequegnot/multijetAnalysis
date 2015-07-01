#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./mergePlot --data-mjb ../analysis/output_rootfile/11Jan15/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting.root --mc-mjb ../analysis/output_rootfile/11Jan15/MC/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_recoilPtHLTBin_type1fix.root  --data-mpf ../analysis/output_rootfile/11Jan15/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix_afterPrescaleReweighting.root --mc-mpf ../analysis/output_rootfile/11Jan15/MC/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root --lin --plotName \"_puJetIdT_HLTsel_woPtRecoilCut_recoilPtHLTBin_type1fix_afterPrescaleReweighting_withSystErrors_QCD-HT\" --extension \".pdf\"  --useSyst --syst-file-mjb \"../compareMC/output_rootfile/compareMC_woPU_pt30_eta50_puJetIdT_woPtRecoilCut_recoilPtHLTBin_type1fix.root \" --syst-file-mpf \"../compareMC/output_rootfile/compareMC_woPU_pt10_eta50_puJetIdT_woPtRecoilCut_recoilPtHLTBin_type1fix.root \" "
os.system(cmd);

