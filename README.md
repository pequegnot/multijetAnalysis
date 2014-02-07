# Presentation

This is the multijet analysis following the Extractor step available here: https://github.com/IPNL-CMS/MultijetExtractorAnalysis.

## Get the code

```bash
myWorkingArea> git clone https://github.com/pequegnot/multijetAnalysis
myWorkingArea> cd multijetAnalysis
```

## Step 1: weight the MC plots

The aim of this step is to convert extractor tuples to histograms. It allows to reweight the MC to 1 /pb and to do the pileup reweighting on MC from the data pileup profile.


```bash
multijetAnalysis> cd weightPlots
```

The instructions to do the pileup reweighting on MC from the data are described in this twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJSONFileforData#Calculating_Your_Pileup_Distribu
[here](https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJSONFileforData#Calculating_Your_Pileup_Distribu).
In particular, the twiki explains how to calculate your pileup distribution on data from your Pattuples. You can also fine a usefull application
[here](https://github.com/IPNL-CMS/PatTopProduction/wiki/Usage). The data pileup profile you get with `pileupCalc.py` script is required in the `PUReweighter` class:
specify where you can find it in the `PUReweighting/PUReweighter.cpp` file.
```json
PUReweighter::PUReweighter(PUProfile profile/* = PUProfile::S10*/, Systematic syst/* = Systematic::NOMINAL*/):
  puHisto(NULL) {
  
    const std::string dataFileName = "MyDataPileupHistogram.root";

   [...]
}

```

The script used to weight the MC is called `multijet_weight_common.cpp` and it can be executed by the following python files:
- `runTheCodeOnMC_woPU.py` for MC
- `runTheCodeOnData_woPU.py` for data

*Remark*: You can also use `runTheCodeOnMC(Data).py` if the PU jets are not removed from the jets collection in your input rootfiles (output files of Extractors).

Edit the executable `runTheCodeOnMC(Data)_woPU.py` and tune it with your corresponding files and informations (see how the executable is constructed bellow). Then, type
these few commands:

```bash
weightPlots> source setup_lyoserv_env.sh
weightPlots> export LD_LIBRARY_PATH=$HOME/.local/lib:\$LD_LIBRARY_PATH
weightPlots> make
weightPlots> ./runTheCodeOnMC_woPU.py
weightPlots> ./runTheCodeOnData_woPU.py
```

### Run on MC

The executable `runTheCodeOnMC_woPU.py` launches your jobs automatically. It runs the `multijet_weight_common.cpp` script on the different MC datasets you list as inputs
in the code. The typical command to run to execute the code on MC is the following:
```bash
weightPlots>./multijet_weight_common --input-list myInputList -o myOutputFile --mc --Nevents myNumberOfProcessedEvents --Xsection myXsection
```

The options of `multijet_weight_common.cpp` script are:
```
   --input-list <string>
     (OR required)  A text file containing a list of input files
         -- OR --
   -i <string>,  --input-file <string>
     (OR required)  The input file


   --data
     (OR required)  Is this data?
         -- OR --
   --mc
     (OR required)  Is this mc?


   -o <string>,  --output-file <string>
     (required)  name of your output file

   --Xsection <float>
     the cross section used to generate your sample

   --Nevents <int>
     Number of processed events 

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```

First of all, you need to create an output list of the Extractuples by executing
the `createOutputListForData.py` and `createOutputListForMC.py` scripts:
```
./createOutputListForData.py -p /<path>/Extractors/MultijetExtractorAnalysis/test
./createOutputListForMC.py -p /<path>/Extractors/MultijetExtractorAnalysis/test
```

You can compute, from the MC Pattuples, the real number of processed events in MC (which can be different from the number of generated AOD events or the number of events at the end of Pat obtained by
the `crab -publish` command) using the following command:
```bash
PatTopProduction> curl -o crabNumberOfProcessedEvents.py https://raw.github.com/blinkseb/cms-utilities/master/crabNumberOfProcessedEvents.py
PatTopProduction> crabNumberOfProcessedEvents.py myWorkingDir
```

Finally, you can have the cross section of your sample using Prep: http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6&campid=Summer12_DR53X

The informations you have to adapt in `runTheCodeOnMC_woPU.py` are bellow:
```json
inputs = [
        ['myWeightPlotsOutputRootfile1.root', 'myInputList1.list', myNprocessedEvents1, myXsection1],
        ['myWeightPlotsOutputRootfile2.root', 'myInputList2.list', myNprocessedEvents2, myXsection2]
]
```

### Run on Data

The typical command to run to execute the code on MC is the following:
```bash
weightPlots>./multijet_weight_common --input-list myInputList -o myOutputFile --data 
```

Note that here, it is not mandatory to specify the cross section and the number of processed events. Indeed, remember that this first step is used particularly to
reweight the MC to a luminosity of 1 /pb.

The informations you have to adapt in `runTheCodeOnData_woPU.py` are bellow:
```json
inputs = [
        ['myWeightPlotsOutputRootfile1.root', 'myInputList1.list'],
        ['myWeightPlotsOutputRootfile2.root', 'myInputList2.list']
]
```


## Step 2: analysis

In this step, we extract responses and histograms from weight plots obtained in the previous step.

```bash
weightPlots> cd ..
multijetAnalysis> cd analysis
```

The executable `runTheCode_woPU.py` launches your jobs automatically. It runs the `multijet_analysis_common.cpp` script on the data and MC rootfiles you obtained in Step
1. 

The typical command to run to execute the code on MC is the following:
```bash
analysis>./multijet_analysis_common --i myInputFile -o myOutputFile --mc --rmPU --plotName "mySpecificName" --extension ".pdf"
```

The typical command to run to execute the code on data is the following:
```bash
analysis>./multijet_analysis_common --i myInputFile -o myOutputFile --data --rmPU --plotName "mySpecificName" --extension ".pdf"
```

The options of `multijet_analysis_common.cpp` script are:

```
   --data
     (OR required)  Is this data?
         -- OR --
   --mc
     (OR required)  Is this mc?


   --rmPU
     Do you want to remove PU jets?

   --extension <string>
     File extension 

   --plotName <string>
     You can add a more specific name to your saved plots

   -o <string>,  --output-file <string>
     (required)  output file

   -i <string>,  --input-file <string>
     (required)  The input file

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

```


The `runTheCode_woPU.py` types these commands for you. The informations you have to adapt in `runTheCode_woPU.py` are bellow:
```json
inputs = [
        ['MULTIJET_MC_analysis_output.root', '../weightPlots/output_rootfile/myMergedWeightPlotsOutputRootfile_MC.root', "isMC", "_mySpecificName", ".pdf"],
	['MULTIJET_Data_analysis_output.root', '../weightPlots/output_rootfile/myMergedWeightPlotsOutputRootfile_Data.root', "isData", "_mySpecificName", ".pdf"],
        ]
```

Finalyy, type these few commands:

```bash
analysis> source setup_lyoserv_env.sh
analysis> export LD_LIBRARY_PATH=$HOME/.local/lib:\$LD_LIBRARY_PATH
analysis> make
analysis> ./runTheCode_woPU.py

```



## Step 3: draw data MC comparison plots

In this step, we draw data/MC comparison plots.

```bash
analysis> cd ..
multijetAnalysis> cd dataMcComparison
```

The executable `runTheCode_woPU.py` runs the `dataMcComparison.cpp` script on the data and MC rootfiles you obtained in Step
2. 

The typical command to run to execute the code is the following:
```bash
dataMcComparison>./dataMcComparison ../analysis/output_rootfile/MULTIJET_Data_analysis_output.root ../analysis/output_rootfile/MULTIJET_MC_analysis_output.root --lumi
--shape --lin --rmPU --plotName "_mySpecificName" --extension ".pdf"
```

The options of `dataMcComparison.cpp` script are:

```
   --rmPU
     Do you want to remove PU jets?

   --lin
     Do you want linear scale?

   --shape
     Do you want shape normalization??

   --lumi
     Do you want lumi normalization?

   --extension <string>
     File extension

   --plotName <string>
     You can add a more specific name to your saved plots

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <string>
     (required)  data input file name

   <string>
     (required)  mc input file name
```

Finalyy, type these few commands:

```bash
dataMcComparison> make
dataMcComparison> ./runTheCode_woPU.py

```




