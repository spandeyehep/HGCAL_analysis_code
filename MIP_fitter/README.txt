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
runOneCell(0,0,0)

### runOneCell(unsigned int layer,unsigned int skiroc,unsigned int channel)
layer : 0 to 27
skiroc : 0 to 3
channel : 0 to 62


> To run on one skiroc chip:
root -l
.L mipFitterTree.C+
runOneChip(0, 0)


### runOneChip(unsigned int layer,unsigned int skiroc)
layer : 0 to 27
skiroc : 0 to 3


> To run on one layer:
root -l
.L mipFitterTree.C+
runAllChip(0)


### runAllChip(unsigned int layer)
layer : 0 to 27



> To run on ENTIRE layers:
>IMPORTANT: in order to suppress drawing of plots, run ROOT in batch mode by using "root -l -b" command
root -l
.L mipFitterTree.C+
runAllLayers(0,27)

### runAllLayers(unsigned int layer_start, unsigned int layer_end)
layer : 0 to 27



