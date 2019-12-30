# pion analysis code
cmrel CMSSW_9_3_0 <br/>
cd CMSSW_9_3_0/src <br/>
git clone -b data_pion_energy_scales_NewEvntCat_v3_v14_chi2 git@github.com:spandeyehep/HGCAL_analysis_code.git <br/>
cd pion_energy_scales_NewEvntCat_v3_v14_chi2 <br/>
make <br/>
**./analyzeHGCOctTB** <file_list> <output_file> **data** **alpha** <beam_energy> <br/>
For example: <br/>
**./analyzeHGCOctTB** **file_list/v14/pion20_config1.txt** **outPut.root** **data** **alpha** **20**
