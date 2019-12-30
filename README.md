# pion analysis code
cmrel CMSSW_9_3_0 <br/>
cd CMSSW_9_3_0/src <br/>
git clone -b sim_pion_energy_scales_NewEvntCat_chi2 git@github.com:spandeyehep/HGCAL_analysis_code.git <br/>
cd sim_pion_energy_scales_NewEvntCat_chi2 <br/>
make <br/>
**./analyzeHGCOctTB** <file_list> <output_file> **sim** **alpha** <beam_energy> <br/>
For example: <br/>
**./analyzeHGCOctTB** **file_list/CMSSW11_0_withAHCAL_newBeamline/QGSP_BERT/sim_pion_20.txt** **outPut.root** **sim** **alpha** **100**
