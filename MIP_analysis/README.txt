Short Description of the codes:

HGCNtupleVariables.h  : Contains variables from the ntuples for rechits and DWC track(taken from MakeClass)
AnalyzeHGCMuons.h     : For declaring and booking histograms and various initialization
HGCNtupleVariables.cc : Contains helper functions.
AnalyzeHGCOctTB.cc    : Contains event loop.

../configs/*          : Contains module mapping for three configurations.

How to run the code:

Define the paths of the your input ntuples in the "InputFileList.txt" file.


> git clone -b MIP_code_OctTB git@github.com:spandeyehep/HGCAL_analysis_code.git
> cd HGCAL_analysis_code/Code_skeleton
> make
> ./analyzeHGCMuons <fileList.txt> <outputFile.root> data <configuration>

For example: if you have defined the paths of the file in InputFileList.txt and the corresponding runs are from configuration-1, then you should use:
> ./analyzeHGCMuons ./InputFileList.txt outPut.root data bravo

Note:
configuration arguments for the code:
use, 
"alpha" or "config1" for configuration-1
"bravo" or "config2" for configuration-2  
"charlie" or "config3" for configuration-3
