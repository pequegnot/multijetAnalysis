#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/28Apr14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root ../analysis/output_rootfile/28Apr14/MC/MULTIJET_MC_QCD-Pt-herwig-120ToInf_analysis_woPU_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut.root --lumi --shape --lin --rmPU --plotName \"_pt30_eta50_puJetIdT_HLTsel_woPtRecoilCut_QCD-Pt-herwig\" --extension \".pdf\""
os.system(cmd);

