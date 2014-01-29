# Presentation

This is the multijet analysis following the Extractor step available here: https://github.com/IPNL-CMS/MultijetExtractorAnalysis.

## Get the code

```bash
myWorkingArea> git clone https://github.com/pequegnot/multijetAnalysis
myWorkingArea> cd multijetAnalysis
```

## Step 1: weight the MC plots

The aim of this step is to reweight the MC to 1 /pb and to do the pileup reweighting on MC from the data pileup profile.


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
_Remark_: You can also use `runTheCodeOnMC(Data).py` if the PU jets are not removed from the jets collection in your input rootfiles (output files of Extractors).

Edit the executable `runTheCodeOnMC(Data)_woPU.py` and tune it with your corresponding files and informations (see how the executable is constructed bellow). 

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

The options are:

--input-list, -i: the list of your input rootfiles (output files of Extractors)
--ouput-file, -o: name of your output file
--mc: you precise here that your input rootfiles are MC
--Nevents: Number of processed events 
--Xsection: the cross section used to generate your sample

First of all, you need to create an output list of the Extractuples by executing
the `createOutputListForData.py` and `createOutputListForMC.py` scripts:
```
./createOutputListForData.py -p /<path>/Extractors/MultijetExtractorAnalysis/test
./createOutputListForData.py -p /<path>/Extractors/MultijetExtractorAnalysis/test
```

You can compute, from the MC Pattuples, the real number of processed events in MC (which can be different from the number of generated AOD events or the number of events at the end of Pat obtained by
the `crab -publish` command) using the following command:
```bash
PatTopProduction> curl -o crabNumberOfProcessedEvents.py https://raw.github.com/blinkseb/cms-utilities/master/crabNumberOfProcessedEvents.py
PatTopProduction> crabNumberOfProcessedEvents.py myWorkingDir
```

Finally, you can have the cross section of your sample using Prep: http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6&campid=Summer12_DR53X


### Run on Data

The typical command to run to execute the code on MC is the following:
```bash
weightPlots>./multijet_weight_common --input-list myInputList -o myOutputFile --data 
```

Note that here, it is not mandatory to specify the cross section and the number of processed events. Indeed, remember that this first step is used particularly to
reweight the MC to a luminosity of 1 /pb.











