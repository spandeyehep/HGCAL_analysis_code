How to run the code:

Download v10 muon files from following path:
/afs/cern.ch/work/s/spandey/public/hgcal/samples/v10/muon_v10.root


cmsrel CMSSW_8_0_1
cd CMSSW_8_0_1/src
cmsenv
git clone git@github.com:spandeyehep/HGCAL_analysis_code.git
cd HGCAL_analysis_code/MIP_fitter/

> update the file path in the mipFitterTree.C

> To run the fit on a single channel:

root -l
.L mipFitterTree.C+
Loop(0,0,0)


### Loop(unsigned int layer,unsigned int skiroc,unsigned int channel)
layer : 0 to 27
skiroc : 0 to 3
channel : 0 to 62


> To run on one skiroc chip:
root -l
.L mipFitterTree.C+
runAllChannel(0, 0)


### runAllChannel(unsigned int layer,unsigned int skiroc)
layer : 0 to 27
skiroc : 0 to 3


> To run on one layer:
root -l
.L mipFitterTree.C+
runAllSkiROC(0)


### runAllSkiROC(unsigned int layer)
layer : 0 to 27



> To run on ENTIRE layers:
root -l
.L mipFitterTree.C+
runAllLayers(0,27)

### runAllLayers(unsigned int layer_start, unsigned int layer_end)
layer : 0 to 27



