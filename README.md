# HGCAL_analysis_code

CMSSW version: CMSSW_9_3_0_pre3 (do `cmsenv`).

### Downlaod scripts <br/>
`git clone -b  OctTB_MIP_analysis git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>


## MIP analysis code



### How to run the script: <br/>

`cd tracking_inclusion`<br/>
`make`<br/>
<br/>
`./analyzeHGCOctTB <file_list> outFileName.root data alpha`<br/>


`<file_list.txt>` : contains muon runs to be analyzed (see for example: `Data_muon_config1_v9.txt`)<br/>

Example: <br/>
`./analyzeHGCOctTB Data_muon_config1_v9.txt out.root data alpha`

### Descrition of scripts: <br/>
`AnalyzeHGCOctTB.cc` : Main analysis code <br/>
`AnalyzeHGCOctTB.h` : Initialize histos here <br/>
`HGCNtupleVariables.h`: Tree variable initialization <br/>
<br/>

## MIP fitting and value extraction <br/>

`cd MIP_fitter` <br/>
use output file from previous step as input by specifying the path in `mipFitterTree.C` <br/>
<br/>
> To run the fit on a single channel: <br/>
<br/>
`root -l`<br/>
`.L mipFitterTree.C+`<br/>
`Loop(0,0,0)`<br/>
<br/>
<br/>
### Loop(unsigned int layer,unsigned int skiroc,unsigned int channel)<br/>
layer : 0 to 27<br/>
skiroc : 0 to 3<br/>
channel : 0 to 62<br/>
<br/>
<br/>

> To run on one skiroc chip:<br/>

`root -l`<br/>
`.L mipFitterTree.C+`<br/>
`runAllChannel(0, 0)`<br/>
<br/>
<br/>
### runAllChannel(unsigned int layer,unsigned int skiroc) <br/>
layer : 0 to 27 <br/>
skiroc : 0 to 3<br/>
<br/>
<br/>
> To run on one layer:<br/>
`root -l`<br/>
`.L mipFitterTree.C+`<br/>
`runAllSkiROC(0)`<br/>
<br/>
<br/>
### runAllSkiROC(unsigned int layer)<br/>
layer : 0 to 27<br/>
<br/>
<br/>
> To run on ENTIRE layers:<br/>
`root -l`<br/>
`.L mipFitterTree.C+`<br/>
`runAllLayers(0,27)`<br/>
<br/>
### runAllLayers(unsigned int layer_start, unsigned int layer_end)<br/>
layer : 0 to 27<br/>
