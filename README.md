# HGCAL_analysis_code

Short Description of the codes:

HGCNtupleVariables.h  : Contains variables from the ntuples for rechits and DWC track(taken from MakeClass)

AnalyzeHGCMuons.h     : For declaring and booking histograms and various initialization

HGCNtupleVariables.cc : Contains helper functions.

AnalyzeHGCOctTB.cc    : Contains event loop.



../configs/*          : Contains module mapping for three configurations.

How to run the code:

file paths are defined in pion_analysis/file_list/v8/*.txt files


> git clone -b OctTB_pion_analysis git@github.com:spandeyehep/HGCAL_analysis_code.git

> cd HGCAL_analysis_code/pion_analysis

> make

> ./analyzeHGCOctTB <fileList.txt> <outputFile.root> data <TB_configuration>

For example: if you want to run the analysis code on 20GeV pion file from configuration-1 then use following command

> ./analyzeHGCOctTB ./file_list/v8/Data_pion_679_689_config1_20GeV_v8.txt outPut.root data alpha

Note:
configuration arguments for the code:
use, 

"alpha" or "config1" for configuration-1

"bravo" or "config2" for configuration-2  

"charlie" or "config3" for configuration-3
