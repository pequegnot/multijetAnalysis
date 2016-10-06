#!/usr/bin/env python

import os, subprocess, datetime, tempfile

d = datetime.datetime.now().strftime("%d%b%y")

#inputs = [
        ## QCD HT

##        ['MULTIJET_QCD_HT-100To250_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root', 'list/MULTIJET_QCD_HT-100To250_woPU_pt10_eta50_puJetIdT_type1fix.list', 50097518, 10360000],
        ##['MULTIJET_QCD_HT-250To500_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root', 'list/MULTIJET_QCD_HT-250To500_woPU_pt10_eta50_puJetIdT_type1fix.list', 27062078, 276000],
        ##['MULTIJET_MC_QCD_HT-500To1000_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root', 'list/MULTIJET_QCD_HT-500To1000_woPU_pt10_eta50_puJetIdT_type1fix.list', 30249292, 8426],
        ##['MULTIJET_MC_QCD_HT-1000ToInf_woPU_pt10_eta50_puJetIdT_recoilPtHLTBin_type1fix.root', 'list/MULTIJET_QCD_HT-1000ToInf_woPU_pt10_eta50_puJetIdT_type1fix.list', 13808863,204],

        ##['MULTIJET_MCQCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6_woPU_pt30_eta50_puJetIdT.root', 'list/MULTIJET_QCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6_woPU_pt30_eta50_puJetIdT.list', 9697512, 89731000000],
        ##['MULTIJET_MCQCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6_woPU_pt30_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6_woPU_pt30_eta50_notRmPUJets.list', 9697512, 89731000000],

        ## Nevents is integral of generator_weight distribution
##        std::cout << "Fill generator weight..." << std::endl;
        ##for(unsigned int ievt=0; ievt<(int)t_event->GetEntries(); ievt++) {
            ##t_event->GetEntry(ievt);
            ##hGenWeight->Fill(generator_weight, generator_weight);
            ##}
        ##TFile *outgen = new TFile("genWeight.root", "recreate");
        ##outgen->cd();
        ##hGenWeight->Write();
        ##outgen->Close();
        ##std::cout << "Fill generator weight: done..." << std::endl;

        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt30_eta50_notRmPUJets_JEC24Aug15.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt30_eta50_notRmPUJets_JEC24Aug15.list', 197692, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets.list', 201576, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt10_eta50_notRmPUJets_JEC24Aug15.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt10_eta50_notRmPUJets_JEC24Aug15.list', 197692, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.list', 201576, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets_16Sept15.list', 201576, 2022100000],

        ## missing 2 rootfiles on T3 WARNING !!!!!! MODIFY NUMBER OF EVENTS !!!!!!
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets_16Sept15.list', 201576, 2022079079],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets_16Sept15.list', 9858314, 2022100000],

##        ['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV3_28Sept15.list', 9609110, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV3_28Sept15.list', 9858314, 2022100000],

        #['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets_usingJEC25nsV5_06Oct15.list', 9858314, 2022100000],
        #['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets_usingJEC25nsV5_06Oct15.list', 9858314, 2022100000],

        ## Nevents given in DAS
##        ['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt30_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt30_eta50_notRmPUJets.list', 9665007, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt30_eta50_notRmPUJets.list', 9858314, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt10_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx50_miniAOD_woPU_pt10_eta50_notRmPUJets.list', 9665007, 2022100000],
        ##['MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.root', 'list/MULTIJET_QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8_bx25_miniAOD_woPU_pt10_eta50_notRmPUJets.list', 9858314, 2022100000],

        #]


inputs = [
        # QCD HT

        ['MULTIJET_QCD_HT-100To250_woPU_pt30_eta50_recoilPtHLTBin.root', 'list/MULTIJET_QCD_HT-100To250_woPU_pt30_eta50_puJetIdT_type1fix.list', 50097518, 10360000],
        ['MULTIJET_QCD_HT-250To500_woPU_pt30_eta50_recoilPtHLTBin.root', 'list/MULTIJET_QCD_HT-250To500_woPU_pt30_eta50_puJetIdT_type1fix.list', 27062078, 276000],
        ['MULTIJET_MC_QCD_HT-500To1000_woPU_pt30_eta50_recoilPtHLTBin.root', 'list/MULTIJET_QCD_HT-500To1000_woPU_pt30_eta50_puJetIdT_type1fix.list', 30249292, 8426],
        ['MULTIJET_MC_QCD_HT-1000ToInf_woPU_pt30_eta50_recoilPtHLTBin.root', 'list/MULTIJET_QCD_HT-1000ToInf_woPU_pt30_eta50_puJetIdT_type1fix.list', 13808863,204],

        ]

def launch(input, output, nevents, xsection): # cross section in pb
    args = ["./multijet_weight_common", "--input-list", input, "-o", output, "--mc", "--Nevents", str(nevents), "--Xsection", str(xsection), "--recoilPtBin", "--recoilPtHLTBin"]

    return " ".join(args)

tmpfile = tempfile.NamedTemporaryFile(dir = '/scratch/', delete = False)

# Build output tree structure

#d = d + "_newMJBBinning"

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

args = ["parallel", "-u", "-a", tmpfile.name, "-j", "20"]
subprocess.call(args)
#print args

## All is done, merge

print("Merging ...")
args = ["hadd","-f", "output_rootfile/%s/MC/MULTIJET_MC_QCD-HT_merged_woPU_pt30_eta50_recoilPtHLTBin_type1fix.root" % (d)]
path = "output_rootfile/%s/MC" % (d)
for output in inputs:
  args.append(os.path.join(path,output[0]))

subprocess.call(args)
#print args
