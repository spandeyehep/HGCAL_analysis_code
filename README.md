# Check secondaries analysis code

### Downlaod scripts <br/>
`git clone -b HadInt_analysisCode git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>

### How to run the script for data and simulation: <br/>

`cd HadInt/code/`<br/>
`make`<br/>

`./analyzeHGCOctTB <file_list> outFileName.root <dataset> alpha <energy>`<br/>


`<file_list.txt>` : contains files* to be analyzed <br/>
*files are available on TIFR cluster on this path: `/home/spandey/t3store3/HGCAL_sim_files/GenSim_v2/HadInt_moreInfo_1060pre4/`
`<dataset>` : `FTFP_BERT_EMN` or `QGSP_FTFP_BERT_EMN` <br/>
`<energy>` : beam_energy<br/>
Example: <br/>
`./analyzeHGCOctTB pion_100.txt out.root FTFP_BERT_EMN 120`

### Descrition of scripts: <br/>
`AnalyzeHGCOctTB.cc` : Main analysis code <br/>
`AnalyzeHGCOctTB.h` : Initialize histos here <br/>
`HGCNtupleVariables.h`: Tree variable initialization <br/>
