# HGCAL_analysis_code

## Latest pion analysis code for OctTB2018


Same analysis code for both data and simulation.
Takes skimmed trees as input.

### Downlaod scripts <br/>
`git clone -b pion_analysis_skimmedTree_CodeSkeleton_04Jan2021 git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>

### How to run the script for data: <br/>

`cd code_skeleton`<br/>
`make`<br/>

`./analyzeHGCOctTB <file_list> outFileName.root data <energy>`<br/>


`<file_list>` : contains files to be analyzed (can be found in file_list folder)<br/>
`<energy>` : beam_energy<br/>
Example: <br/>
`./analyzeHGCOctTB file_list/data/v16_v8/pion100.txt outFileName.root data 100`


### How to run the script for sim: <br/>
`make`<br/>

`./analyzeHGCOctTB <file_list> outFileName.root <physcis_list> <energy>`<br/>

Everthing is same expect for `<physcis_list>` <br/>
`<physcis_list>` : Use `FTFP_BERT_EMN` or `QGSP_FTFP_BERT_EMN` <br/>
Example: <br/>
`./analyzeHGCOctTB file_list/sim/v44_VtxBeam_v3_correctFH10/FTFP_BERT_EMN/pion100.txt outFileName.root FTFP_BERT_EMN 100`


### Descrition of scripts: <br/>
`AnalyzeHGCOctTB.cc` : Main analysis code <br/>
`AnalyzeHGCOctTB.h` : Initialize histos here <br/>
`HGCNtupleVariables.h`: Tree variable initialization <br/>
Others are helping classes

### ROOT files can be copied from IISER cluster. Path is avilable in the file_list text files. <br/>
