# HGCAL_analysis_code

cmsrel CMSSW_9_3_0 <br/>
cd CMSSW_9_3_0/src <br/>
cmsenv <br/>
git clone -b pionOctTB_showerFinder git@github.com:spandeyehep/HGCAL_analysis_code.git <br/>
cd ShowerFinder/ <br/>
make <br/>

#in order to Run the code, use following command : <br/>
Syntax is as follows <br/>

./analyzeHGCOctTB <path_of_inputFile_list> <outFileName> data <configuration> <energy> <br/>

e.g. <br/>
./analyzeHGCOctTB ./file_list/v12/pion200_config1_one_file.txt output.root data alpha 200 <br/>


Note: <br/>
<configuration> arguments for the code: <br/>
use,  <br/>
"alpha" or "config1" for configuration-1 <br/>
"bravo" or "config2" for configuration-2   <br/>
"charlie" or "config3" for configuration-3 <br/>


Short Description of the codes: <br/>

HGCNtupleVariables.h  : Contains variables from the ntuples for rechits and DWC track(taken from MakeClass) & AHCAL <br/>
AnalyzeHGCMuons.h     : For declaring and booking histograms and various initialization <br/>
HGCNtupleVariables.cc : Contains helper functions. <br/>
AnalyzeHGCOctTB.cc    : Contains event loop. (Main Analysis Code) <br/>

