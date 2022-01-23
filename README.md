# HGCAL_analysis_code

## Noise fitting and value extraction <br/>

CMSSW version: CMSSW_9_3_0_pre3 (do `cmsenv`).

### Downlaod scripts <br/>
`git clone -b  SNR_study git@github.com:spandeyehep/HGCAL_analysis_code.git .` <br/>

`cd Noise_fitter`<br/>
`root -l`<br/>
`.L noiseFitter.C+`<br/>
for EE cells: `for_all()` <br/>
for FH cells: `for_all_FH()` <br/>
