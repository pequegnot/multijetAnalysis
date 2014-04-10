#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

inputs = [
        # QCD Pt Herwig
        ['MULTIJET_MC_QCD_Pt-120to170_herwig_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_Pt-120to170_herwig_woPU_pt25_eta50_puJetIdMT_05Mar14.list', 1493670, 126490],
        ['MULTIJET_MC_QCD_Pt-170to300_herwig_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_Pt-170to300_herwig_woPU_pt25_eta50_puJetIdMT_05Mar14.list', 1497220, 27935],
        ['MULTIJET_MC_QCD_Pt-300to470_herwig_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_Pt-300to470_herwig_woPU_pt25_eta50_puJetIdMT_05Mar14.list', 1987929, 1461],
        ['MULTIJET_MC_QCD_Pt-470to600_herwig_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_Pt-470to600_herwig_woPU_pt25_eta50_puJetIdMT_05Mar14.list', 1945737, 95.25],
        ['MULTIJET_MC_QCD_Pt-600to800_herwig_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_Pt-600to800_herwig_woPU_pt25_eta50_puJetIdMT_05Mar14.list', 983991, 22.73],
        ['MULTIJET_MC_QCD_Pt-800to1000_herwig_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_Pt-800to1000_herwig_woPU_pt25_eta50_puJetIdMT_05Mar14.list', 962795, 2.997],
        ['MULTIJET_MC_QCD_Pt-1000toInf_herwig_woPU_pt25_eta50_puJetIdMT.root', '/gridgroup/cms/pequegnot/CMSSW/CMSSW_5_3_9_patch2/src/Extractors/MultijetExtractorAnalysis/test/MULTIJET_QCD_Pt-1000toInf_herwig_woPU_pt25_eta50_puJetIdMT_05Mar14.list', 498073, 0.665],
        ]

def launch(input, output, nevents, xsection):
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--mc", "--Nevents", str(nevents), "--Xsection", str(xsection)]

    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

# Build output tree structure

path = "output_rootfile/%s/MC" % (d)
try:
    os.makedirs(path)
except:
    pass

print("Extracting dataset ...")
for input in inputs:
    path = "output_rootfile/%s/MC" % (d)
    tmpfile.write(launch(input[1], os.path.join(path, input[0]), input[2], input[3]) + "\n")

tmpfile.flush()

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "2"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/MC/MULTIJET_MC_QCD-Pt-herwig_merged_woPU_pt25_eta50_puJetIdMT_%s.root" % (d,d)]
path = "output_rootfile/%s/MC" % (d)
for output in inputs:
  args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args
