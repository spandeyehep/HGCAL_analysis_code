# HGCAL_analysis_code

## Latest pion analysis code for OctTB2018


Since ntuple structure is different for data and sim therefore we have two folders data_analysis_code and sim_analysis_code. Both are same in all aspects except tree-initializing and synching code snippet.

### Downlaod scripts <br/>
`git clone -b pion_analysis_16Nov2020 git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>

### How to run the script for data: <br/>

`cd data_analysis_code`<br/>
`make`<br/>

`./analyzeHGCOctTB <file_list> outFileName.root data <configuration> <energy>`<br/>


`<file_list.txt>` : contains files to be analyzed (can be found in file_list folder)<br/>
`<configuration>` : alpha (always use alpha for config-1)<br/>
`<energy>` : beam_energy<br/>
Example: <br/>
`./analyzeHGCOctTB file_list/v16_v8/pion100_config1.txt outFileName.root data alpha 100`


### How to run the script for sim: <br/>
`cd sim_analysis_code`<br/>
`make`<br/>

`./analyzeHGCOctTB <file_list> outFileName.root <physcis_list> <configuration> <energy>`<br/>

Everthing is same expect for `<physcis_list>` <br/>
`<physcis_list>` : Use `FTFP_BERT_EMN` or `QGSP_FTFP_BERT_EMN` <br/>
Example: <br/>
`./analyzeHGCOctTB file_list/CMSSW11_0_withAHCAL_newBeamline/v44_VtxBeam_v3_correctFH10/FTFP_BERT_EMN/sim_pion_100.txt outFileName.root FTFP_BERT_EMN alpha 100`


### Descrition of scripts: <br/>
`AnalyzeHGCOctTB.cc` : Main analysis code <br/>
`AnalyzeHGCOctTB.h` : Initialize histos here <br/>
`HGCNtupleVariables.h`: Tree variable initialization <br/>
Others are helping classes
