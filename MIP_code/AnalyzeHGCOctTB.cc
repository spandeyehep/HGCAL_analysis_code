#define AnalyzeHGCOctTB_cxx

#include <iostream>
#include <vector>
#include <cstring>
#include "AnalyzeHGCOctTB.h"

using namespace std;



// chip 3022,44,3028




int main(int argc, char* argv[])
{

  if (argc < 3) {
    cerr << "Please give 4 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "configuration" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *config          = argv[4];

  AnalyzeHGCOctTB hgcOctTB(inputFileList, outFileName, data, config);
  cout << "dataset " << data << " " << endl;
  cout << "congiguration " << config << " " << endl;

  hgcOctTB.EventLoop(data);
  return 0;
}

void AnalyzeHGCOctTB::EventLoop(const char *data) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  int decade = 0;

  float rechit_cut[28] = {0.02,0.039,0.019,0.02,0.037,0.076,0.10,
			  0.14,0.18,0.24,0.28,0.33,0.48,0.52,
			  0.56,0.6,0.64,0.70,0.74,0.78,0.82,
			  0.86,0.90,0.92,0.94,0.95,0.98,1.0};
  Long64_t cut_count[28];
  Long64_t nEvents = 0;
  Long64_t MIP_pions = 0;

  for(int i = 0; i< 28; i++){ cut_count[i] = 0;}
  Long64_t jentry;
  //for (Long64_t jentry=0; jentry<nentries;jentry++) {
  for (jentry=0; jentry<nentries;jentry++) {
    // cout<<"Hello"<<endl;
    // if(jentry != 10044 ) continue;
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;
    
    // ===============read this entry == == == == == == == == == == ==

    Long64_t ientry = LoadTree(jentry);
    // cout<<"ientry = "<<ientry<<endl;
    //if (ientry < 0) break;
    if (ientry < 0) { break; cout<<"Breaking"<<endl;}
        // cout<<"****"<<endl;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // cout<<jentry<<" : "<<NRechits<<endl;

    //h_nrechits->Fill(NRechits);
    
    // int nRechit_[28];
    // float rechit_E[28];
    // //double rechit_en_layer[28];
    // double bary_x[28];
    // double bary_y[28];
    // for(int i=0;i<28;i++){

    //   nRechit_[i]=0;
    //   rechit_E[i] = 0.0;
    //   bary_x[i] = 0.0;
    //   bary_y[i] = 0.0;
    // }

    if(NRechits == 0) continue;
    //nEvents++;



    


    Double_t rechitEnergySum = 0.0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;
    Double_t un_cali = 0.0;
    Long_t Nrechit_layer[40];
    Long_t NRechits_EE[28];
    Long_t NRechits_FH[12][7];
    // Long_t Nrechit_EE[28] = 0;
    // Long_t Nrechit_FH = 0;
    int module_part_ = 0;
    int module_layer_ = 0;
    int module_position_ = 0;
    for(int ii=0;ii<40;ii++){
      if(ii<28)
	NRechits_EE[ii]=0;
      Nrechit_layer[ii]=0;
    }
    for(int ii=0;ii<12;ii++){
      for(int jj=0;jj<7;jj++){
      NRechits_FH[ii][jj]=0;
      }
    }
    
    std::vector<int> temp_moduleID;
    for(int i = 0 ; i < NRechits; i++){
      temp_moduleID.clear();
      int temp_layer = rechit_layer->at(i);
      int temp_chip = rechit_chip->at(i);
      int temp_channel = rechit_channel->at(i);
      int en_chan = temp_chip*1000+temp_channel;
      if(en_chan == 3022 || en_chan == 3028 || en_chan == 44) continue;
      if(rechit_amplitudeHigh->at(i) < 20) continue;
      Nrechit_layer[temp_layer]++;

      temp_moduleID = getModuleLocation(rechit_module->at(i));
      if(!temp_moduleID.size() || temp_moduleID.size()<3) {
	cout<<"ERROR: Could NOT locate MODULE location for module "<<rechit_module->at(i)<<endl;
	cout<<"\t more info temp_moduleID.size() = "<<temp_moduleID.size()<<endl;
	return;
      }
      module_part_ = temp_moduleID.at(0);
      module_layer_ = temp_moduleID.at(1);
      module_position_ = temp_moduleID.at(2);
      int channel_bin = getBIN(temp_chip,temp_channel);
      if (channel_bin < 0) { cout<<"INVALID CHANNEL!!!"<<endl; return;}
      // cout<<temp_moduleID.size()<<endl;
      // cout<<temp_moduleID.at(0)<<"\t"<<temp_moduleID.at(1)<<"\t"<<temp_moduleID.at(2)<<endl;
      h_moduleID->Fill(module_part_);
      if(module_part_ == 0) {
      	rechitEnergySum_EE+=rechit_energy->at(i);
	NRechits_EE[module_layer_-1]++;
      	h_adcHG_EE[module_layer_-1][channel_bin]->Fill(rechit_amplitudeHigh->at(i));
      }
      if(module_part_ == 1) {
      	rechitEnergySum_FH+=rechit_energy->at(i);
	NRechits_FH[module_layer_-1][module_position_-1]++;
      	h_adcHG_FH[module_layer_-1][module_position_-1][channel_bin]->Fill(rechit_amplitudeHigh->at(i));
      }
      rechitEnergySum+=rechit_energy->at(i);
      
    }

    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    //cout<<"Config = "<<conf_<<endl;
    for(int iL = 0; iL < 40; iL++){
      if(iL < 28) {
	Nrechit_EE+=NRechits_EE[iL];
    	h_NRechits_EE[iL]->Fill(NRechits_EE[iL]);
      }
      else {
	for(int j = 0; j < 7; j++) {
	  if(!strcmp(conf_,"alpha") && (j+1) !=4 && ((iL-28)+1) > 9 ) continue;
	  Nrechit_FH+=NRechits_FH[iL-28][j];
	  h_NRechits_FH[iL-28][j]->Fill(NRechits_FH[iL-28][j]);
	}
      }
    }

    
    h_particleID->Fill(pdgID);
    h_beamEnergy->Fill(beamEnergy);
    h_runNumber->Fill(run);
    h_rechit_energy_all->Fill(rechitEnergySum);
    h_rechit_energy_EE_only->Fill(rechitEnergySum_EE);
    h_rechit_energy_FH_only->Fill(rechitEnergySum_FH);
    h_rechit_energy_EE_vs_FH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
    h_rechit_En_NRechits->Fill(NRechits,rechitEnergySum);
    h_Nrechit_EE_vs_FH->Fill(Nrechit_EE,Nrechit_FH);
    h_Nrechit_EE->Fill(Nrechit_EE);
    h_Nrechit_FH->Fill(Nrechit_FH);

  } // loop over entries

  cout<<"Got Out "<<jentry<<endl;
  // cout<<"% Events survived = "<<((float)MIP_pions/nEvents)*100 << endl;
  }

