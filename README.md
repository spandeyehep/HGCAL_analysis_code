# HGCAL_analysis_code

cmsrel CMSSW_9_3_0 
cd CMSSW_9_3_0/src
cmsenv
git clone -b pionOctTB_showerFinder git@github.com:spandeyehep/HGCAL_analysis_code.git
cd ShowerFinder/
make

#in order to Run the code, use following command :
Syntax is as follows

./analyzeHGCOctTB <path_of_inputFile_list> <outFileName> data <configuration> <energy>

e.g.
./analyzeHGCOctTB ./file_list/v12/pion200_config1_one_file.txt output.root data alpha 200


Note:
<configuration> arguments for the code:
use, 
"alpha" or "config1" for configuration-1
"bravo" or "config2" for configuration-2  
"charlie" or "config3" for configuration-3


Short Description of the codes:

HGCNtupleVariables.h  : Contains variables from the ntuples for rechits and DWC track(taken from MakeClass) & AHCAL
AnalyzeHGCMuons.h     : For declaring and booking histograms and various initialization
HGCNtupleVariables.cc : Contains helper functions.
AnalyzeHGCOctTB.cc    : Contains event loop. (Main Analysis Code)

