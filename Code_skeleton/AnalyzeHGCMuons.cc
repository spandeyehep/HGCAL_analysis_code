#define ANALYZEHGCMuons_cxx

#include <iostream>
#include <vector>
#include <cstring>
#include "AnalyzeHGCMuons.h"

using namespace std;


// int getBIN(unsigned int skiroc,unsigned int channel) {
//   if (channel%2 != 0 || channel > 62 || skiroc > 3) return -1;
//   else return (skiroc*32 + (channel/2));
// }



int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please give 3 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  AnalyzeHGCMuons hgcmuons(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  hgcmuons.EventLoop(data);

  return 0;
}

void AnalyzeHGCMuons::EventLoop(const char *data) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  int decade = 0;

  int temp_channel = 0, temp_chip = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;
    
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    nb2 = fChain2->GetEntry(jentry);   nbytes2 += nb2;

    for(unsigned int i = 0; i < rechit_layer->size(); i++) {
      if ((rechit_layer->at(i)-1) == 0 || true) {
	temp_channel = rechit_channel->at(i);
	temp_chip = rechit_chip->at(i);
	h_ADChg[(rechit_layer->at(i)-1)][getBIN(temp_chip,temp_channel)]->Fill(rechit_amplitudeHigh->at(i));
      }
    }



    // std::cout << jentry << " Run " << run << "  Event " << event << " beamEnergy " << beamEnergy 
    // 	      << " tree2 ntracks " << ntracks << " impactX_HGCal_layer_1 " << impactX_HGCal_layer_1
    // 	      << std::endl;
    
  } // loop over entries
  
}

