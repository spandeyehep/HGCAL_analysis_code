# HGCAL_analysis_code

## Baseline code pion analysis code for OctTB2018 : ONLY FOR DATA


Any CMSSW version environment should be fine (do `cmsenv`).

### Downlaod scripts <br/>
`git clone -b pion_ana_baseline_code git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>

### How to run the script for data: <br/>

`cd code`<br/>
`make`<br/>

`./analyzeHGCOctTB <file_list> outFileName.root data <configuration> <energy>`<br/>


`<file_list.txt>` : contains files to be analyzed (can be found in file_list folder)<br/>
`<configuration>` : `alpha`/`bravo`/`charlie` (for config1/config2/config3)<br/>
`<energy>` : beam_energy<br/>
Example: <br/>
`./analyzeHGCOctTB file_list/v16_v8/pion100_config1.txt outFileName.root data alpha 100`

### Descrition of scripts: <br/>
`AnalyzeHGCOctTB.cc` : Main analysis code <br/>
`AnalyzeHGCOctTB.h` : Initialize histos here <br/>
`HGCNtupleVariables.h`: Tree variable initialization <br/>

