#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/02Jun14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting.root ../analysis/output_rootfile/02Jun14/MC/MULTIJET_MC_QCD-HT-100ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root --lumi --shape --lin --rmPU --recoilPtBin --plotName \"_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_afterPrescaleReweighting_QCD-HT_newHLTSel\" --extension \".pdf\""
os.system(cmd);

