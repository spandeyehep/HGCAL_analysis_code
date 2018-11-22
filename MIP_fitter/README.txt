This script takes output of MIP_analysis code as input.
Update the file path in the mipFitterTree.C

Loop(..) function inside mipFitterTree.C takes six arguments:
const char *part      :  "EE" or "FH"
unsigned int layer    :  absolute layer number (from 1 to 40)
unsigned int position :  Position in the layer (0 for EE, 1 to 7 for FH)
unsigned int skiroc   :  chip ID (0 to 3)
unsigned int channel  :  channel ID (0 to 62)
unsigned int module   :  Module number of the Hexaboard

For example if you want to look at channel 36 of chip 2 in EE-layer 1 (config-2):
Loop("EE",1,0,2,36,78)


	cmsrel CMSSW_9_3_0
	cd CMSSW_9_3_0/src
	cmsenv
	git clone -b MIP_code_OctTB git@github.com:spandeyehep/HGCAL_analysis_code.git
	cd HGCAL_analysis_code/MIP_fitter/
	root -l
	.L mipFitterTree.C+
	Loop("EE",1,0,2,36,78)
