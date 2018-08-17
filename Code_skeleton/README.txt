How to run the script:

How to run the code:

Download v10 muon files from following path:
/afs/cern.ch/work/s/spandey/public/hgcal/samples/v10/muon_v10.root

cmsrel CMSSW_8_0_1
cd CMSSW_8_0_1/src
cmsenv
git clone git@github.com:spandeyehep/HGCAL_analysis_code.git

#update the muon_runlist.txt file

cd HGCAL_analysis_code/Code_skeleton
make
./analyzeHGCMuons muon_runlist.txt temp.root data
