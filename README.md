# HGCAL_analysis_code

## MIP analysis code for OctTB2018


CMSSW version: CMSSW_9_3_0_pre3 (do `cmsenv`).

### Downlaod scripts <br/>
`git clone -b  OctTB_MIP_analysis git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>

### How to run the script: <br/>

`cd tracking_inclusion`<br/>
`make`<br/>

`./analyzeHGCOctTB <file_list> outFileName.root data alpha`<br/>


`<file_list.txt>` : contains muon runs to be analyzed (see for example: `Data_muon_config1_v9.txt`)<br/>

Example: <br/>
`./analyzeHGCOctTB Data_muon_config1_v9.txt out.root data alpha`

### Descrition of scripts: <br/>
`AnalyzeHGCOctTB.cc` : Main analysis code <br/>
`AnalyzeHGCOctTB.h` : Initialize histos here <br/>
`HGCNtupleVariables.h`: Tree variable initialization <br/>
