#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/10Feb14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU_pt25_eta50_HLTsel.root ../analysis/output_rootfile/10Feb14/MC/MULTIJET_MC_QCD_HT-500ToInf_analysis_woPU_pt25_eta50_HLTsel.root --lumi --shape --lin --rmPU --plotName \"_pt25_eta50_HLTsel\" --extension \".pdf\""
os.system(cmd);

