#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/16Jan14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis_woPU.root ../analysis/output_rootfile/16Jan14/MC/MULTIJET_MC_QCD_HT-500ToInf_analysis_woPU.root --lumi --shape --lin --rmPU"
os.system(cmd);

