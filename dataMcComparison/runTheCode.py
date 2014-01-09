#!/usr/bin/env python

import os, subprocess, datetime, tempfile

cmd = "./dataMcComparison ../analysis/output_rootfile/08Jan14/data/MULTIJET_Run2012ABCD-22Jan2013_analysis.root ../analysis/output_rootfile/08Jan14/MC/MULTIJET_MC_QCD_HT-500ToInf_analysis.root --lumi --shape --lin"
os.system(cmd);

