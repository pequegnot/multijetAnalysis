#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/01Apr14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt25_eta50_puJetIdMT_HLTsel_woPtRecoilCut.root ../analysis/output_rootfile/01Apr14/MC/MULTIJET_MC_QCD-Pt-pythia-120To1400_analysis_woPU_pt25_eta50_puJetIdMT_HLTsel_woPtRecoilCut.root --lumi --shape --lin --rmPU --plotName \"_pt25_eta50_puJetIdMT_HLTsel_woPtRecoilCut_QCD-Pt-pythia\" --extension \".pdf\""
os.system(cmd);

