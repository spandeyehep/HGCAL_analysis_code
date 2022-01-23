# HGCAL_analysis_code

## Noise fitting and value extraction <br/>

CMSSW version: CMSSW_9_3_0_pre3 (do `cmsenv`).<br/>
Input files are avaiable on IISER cluster: `/home/work/spandey/public/HGCAL/CERN_TB/octTB/CMSSW_9_3_0/src/MIP_analysis/SNR/muon_rereco_v12_719` <br/>


### Downlaod scripts <br/>
`git clone -b  SNR_study git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>

`cd Noise_fitter`<br/>
`root -l`<br/>
`.L noiseFitter.C+`<br/>
for EE cells: `for_all()` <br/>
for FH cells: `for_all_FH()` <br/>
