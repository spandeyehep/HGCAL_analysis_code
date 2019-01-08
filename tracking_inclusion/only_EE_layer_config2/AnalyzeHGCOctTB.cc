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

  bool DEBUG = false;
  Long64_t cut_count[28];
  Long64_t nEvents = 0;
  Long64_t MIP_pions = 0;
  int TOTAL_ACTIVE_LAYER = -1;
  int EE_LAYER = -1;
  int FH_LAYER = -1;
  if(!strcmp(conf_,"alpha") || !strcmp(conf_,"config1")) {
    TOTAL_ACTIVE_LAYER = 40;
    EE_LAYER = 28;
    FH_LAYER = 12;
  }
  else if(!strcmp(conf_,"bravo") || !strcmp(conf_,"config2")){
    TOTAL_ACTIVE_LAYER = 39;
    EE_LAYER = 28;
    FH_LAYER = 11;
  }
  else if(!strcmp(conf_,"charlie") || !strcmp(conf_,"config3")) {
    TOTAL_ACTIVE_LAYER = 20;
    EE_LAYER = 8;
    FH_LAYER = 12;
  }
  else {
    cout<<"ERROR: Unknown configuration!!!!"<<endl;
    return;
  }

  if(DEBUG) cout<<"DEBUG: Configuration = "<<conf_<<endl;
  if(DEBUG) cout<<"DEBUG: TOTAL_ACTIVE_LAYER = "<<TOTAL_ACTIVE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: EE_LAYER = "<<EE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: FH_LAYER = "<<FH_LAYER<<endl;

  if(DEBUG) cout<<"DEBUG: Entering event Loop"<<endl;
  for(int i = 0; i< 28; i++){ cut_count[i] = 0;}
  Long64_t jentry;
  //for (Long64_t jentry=0; jentry<nentries;jentry++) {
  for (jentry=0; jentry<nentries;jentry++) {
    // cout<<"Hello"<<endl;
    //if(jentry == 10000 ) break;
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;
    
    // ===============read this entry == == == == == == == == == == ==

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) { break; cout<<"Breaking"<<endl;}
        // cout<<"****"<<endl;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    nb2 = fChain2->GetEntry(jentry); nbytes2 += nb2;

    if(NRechits == 0) continue;
    // if(NRechits > 50 || NRechits < 20) continue;
    h_nTracks->Fill(ntracks);
    if(ntracks != 1) continue;
    Float_t track_x[28];
    Float_t track_y[28];

    track_x[0] = impactX_HGCal_layer_1;
    track_y[0] = impactY_HGCal_layer_1;
    track_x[1] = impactX_HGCal_layer_2;
    track_y[1] = impactY_HGCal_layer_2;
    track_x[2] = impactX_HGCal_layer_3;
    track_y[2] = impactY_HGCal_layer_3;
    track_x[3] = impactX_HGCal_layer_4;
    track_y[3] = impactY_HGCal_layer_4;
    track_x[4] = impactX_HGCal_layer_5;
    track_y[4] = impactY_HGCal_layer_5;
    track_x[5] = impactX_HGCal_layer_6;
    track_y[5] = impactY_HGCal_layer_6;
    track_x[6] = impactX_HGCal_layer_7;
    track_y[6] = impactY_HGCal_layer_7;
    track_x[7] = impactX_HGCal_layer_8;
    track_y[7] = impactY_HGCal_layer_8;
    track_x[8] = impactX_HGCal_layer_9;
    track_y[8] = impactY_HGCal_layer_9;
    track_x[9] = impactX_HGCal_layer_10;
    track_y[9] = impactY_HGCal_layer_10;
    track_x[10] = impactX_HGCal_layer_11;
    track_y[10] = impactY_HGCal_layer_11;
    track_x[11] = impactX_HGCal_layer_12;
    track_y[11] = impactY_HGCal_layer_12;
    track_x[12] = impactX_HGCal_layer_13;
    track_y[12] = impactY_HGCal_layer_13;
    track_x[13] = impactX_HGCal_layer_14;
    track_y[13] = impactY_HGCal_layer_14;
    track_x[14] = impactX_HGCal_layer_15;
    track_y[14] = impactY_HGCal_layer_15;
    track_x[15] = impactX_HGCal_layer_16;
    track_y[15] = impactY_HGCal_layer_16;
    track_x[16] = impactX_HGCal_layer_17;
    track_y[16] = impactY_HGCal_layer_17;
    track_x[17] = impactX_HGCal_layer_18;
    track_y[17] = impactY_HGCal_layer_18;
    track_x[18] = impactX_HGCal_layer_19;
    track_y[18] = impactY_HGCal_layer_19;
    track_x[19] = impactX_HGCal_layer_20;
    track_y[19] = impactY_HGCal_layer_20;
    track_x[20] = impactX_HGCal_layer_21;
    track_y[20] = impactY_HGCal_layer_21;
    track_x[21] = impactX_HGCal_layer_22;
    track_y[21] = impactY_HGCal_layer_22;
    track_x[22] = impactX_HGCal_layer_23;
    track_y[22] = impactY_HGCal_layer_23;
    track_x[23] = impactX_HGCal_layer_24;
    track_y[23] = impactY_HGCal_layer_24;
    track_x[24] = impactX_HGCal_layer_25;
    track_y[24] = impactY_HGCal_layer_25;
    track_x[25] = impactX_HGCal_layer_26;
    track_y[25] = impactY_HGCal_layer_26;
    track_x[26] = impactX_HGCal_layer_27;
    track_y[26] = impactY_HGCal_layer_27;
    track_x[27] = impactX_HGCal_layer_28;
    track_y[27] = impactY_HGCal_layer_28;
    track_x[28] = impactX_HGCal_layer_29;
    track_y[28] = impactY_HGCal_layer_29;
    track_x[29] = impactX_HGCal_layer_30;
    track_y[29] = impactY_HGCal_layer_30;
    track_x[30] = impactX_HGCal_layer_31;
    track_y[30] = impactY_HGCal_layer_31;
    track_x[31] = impactX_HGCal_layer_32;
    track_y[31] = impactY_HGCal_layer_32;
    track_x[32] = impactX_HGCal_layer_33;
    track_y[32] = impactY_HGCal_layer_33;
    track_x[33] = impactX_HGCal_layer_34;
    track_y[33] = impactY_HGCal_layer_34;
    track_x[34] = impactX_HGCal_layer_35;
    track_y[34] = impactY_HGCal_layer_35;
    track_x[35] = impactX_HGCal_layer_36;
    track_y[35] = impactY_HGCal_layer_36;
    track_x[36] = impactX_HGCal_layer_37;
    track_y[36] = impactY_HGCal_layer_37;
    track_x[37] = impactX_HGCal_layer_38;
    track_y[37] = impactY_HGCal_layer_38;
    track_x[38] = impactX_HGCal_layer_39;
    track_y[38] = impactY_HGCal_layer_39;
    track_x[39] = impactX_HGCal_layer_40;
    track_y[39] = impactY_HGCal_layer_40;

    
    Double_t rechitEnergySum = 0.0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;
    Double_t un_cali = 0.0;
    Long_t Nrechit_layer[40];
    Long_t NRechits_EE[28];
    Long_t NRechits_FH[12][7];
    int module_part_ = 0;
    int module_layer_ = 0;
    int module_position_ = 0;
    vector<double> dR[40];
    double dr_min_index[40];
    double dr_min[40];

    for(int ii=0;ii<40;ii++){
      if(ii<28) {
	NRechits_EE[ii]=0;
	dR[ii].clear();
	dr_min_index[ii] = -1.0;
	dr_min[ii] = 1.e10;
      }
      Nrechit_layer[ii]=0;
      // dR[ii].clear();
      // dr_min_index[ii] = -1.0;
      // dr_min[ii] = 1.e10;

    }
    for(int ii=0;ii<12;ii++){
      for(int jj=0;jj<7;jj++){
      NRechits_FH[ii][jj]=0;
      }
    }
    
    if(DEBUG) cout<<"DEBUG: Start Analylizing RecHits!!"<<endl;
    std::vector<int> temp_moduleID;
    for(int i = 0 ; i < NRechits; i++){
      temp_moduleID.clear();
      int temp_layer = rechit_layer->at(i);
      int temp_chip = rechit_chip->at(i);
      int temp_channel = rechit_channel->at(i);
      int en_chan = temp_chip*1000+temp_channel;
      if(en_chan == 3022 || en_chan == 3028 || en_chan == 44) continue;
      if(rechit_amplitudeHigh->at(i) < 20) continue;
      Nrechit_layer[temp_layer-1]++;

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


      if(module_part_ == 0) {
	// h_rechitX_vs_trackX_layer[temp_layer-1]->Fill(rechit_x->at(i),track_x[temp_layer-1]);
	// h_rechitY_vs_trackY_layer[temp_layer-1]->Fill(rechit_y->at(i),track_y[temp_layer-1]);
	h_rechitX_vs_rechitY_layer[temp_layer-1]->Fill(rechit_x->at(i),rechit_y->at(i));
	h_trackX_vs_trackY_layer[temp_layer-1]->Fill(-1*track_x[temp_layer-1],-1*track_y[temp_layer-1]);
	h_rechitX_vs_trackX_layer[temp_layer-1]->Fill(rechit_x->at(i),-1*track_x[temp_layer-1]);
	h_rechitY_vs_trackY_layer[temp_layer-1]->Fill(rechit_y->at(i),-1*track_y[temp_layer-1]);
	double recx = rechit_x->at(i);
	double recy = rechit_y->at(i);
	double trackx = track_x[temp_layer-1];
	double tracky = track_y[temp_layer-1];
	double temp = deltaR(recx,recy,-1*trackx,-1*tracky);
	double dX = recx - (-1*trackx);
	double dY = recy - (-1*tracky);
	//double temp = deltaR(rechit_x->at(i),rechit_y->at(i),track_x[temp_layer-1],track_y[temp_layer-1]);
	h_dX_dY_layer[temp_layer-1]->Fill(dX,dY);
	h_dR[temp_layer-1]->Fill(abs(temp));
	if (abs(temp) < dr_min[temp_layer-1]) {
	  dr_min[temp_layer-1] = abs(temp);
	  dr_min_index[temp_layer-1] = i;
	}
	
	// if(temp_layer-1 == 0) {
	//   cout<<"entrty,RecX,RecY,TracX,TracY,dR = "<<jentry<<","<<recx<<","<<recy<<","<<trackx<<","<<tracky<<","<<abs(temp)<<endl;;
	//   if(jentry == 10) return;
	  
	// }
	dR[temp_layer-1].push_back(abs(temp));
	// if(!dR[temp_layer-1].size()) {
	  
	// }

      }

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
    // cout<<"NEW EVENT "<<jentry<<endl;
    // cout<<"Rechits = "<<NRechits<<endl;
    // for(int i = 0 ; i < 28; i++ ){
    //   cout<<i<<" "<<dR[i].size()<<" "<<NRechits_EE[i]<<endl;
    // }
    // if(jentry > 50) return;
    
    if(DEBUG) cout<<"DEBUG: Values Assigned!!!"<<endl;

    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    //cout<<"Config = "<<conf_<<endl;

    bool zero_rh[28];
    
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
      if(iL < EE_LAYER) {
    	Nrechit_EE+=NRechits_EE[iL];
    	h_NRechits_EE[iL]->Fill(NRechits_EE[iL]);
	if(!NRechits_EE[iL]) zero_rh[iL]=true;
	else zero_rh[iL]=false;
      }
      else {
    	for(int j = 0; j < 7; j++) {
	  //    	  if(!strcmp(conf_,"alpha") && (j+1) !=4 && ((iL-28)+1) > 9 ) continue;
    	  Nrechit_FH+=NRechits_FH[iL-EE_LAYER][j];
    	  h_NRechits_FH[iL-EE_LAYER][j]->Fill(NRechits_FH[iL-EE_LAYER][j]);

    	}
      }
    }

    if(DEBUG) cout<<"DEBUG: Assigned NRechits for each layer"<<endl;

    if(!strcmp(conf_,"alpha")) h_configuration->Fill(10);
    else if(!strcmp(conf_,"bravo")) h_configuration->Fill(20);
    else if(!strcmp(conf_,"charlie")) h_configuration->Fill(30);
    else {
      cout<<"ERROR: Unknown configuration!!!!"<<endl;
      return;
    }

    //if(zero_rh) continue;
    
    // if(DEBUG) {
    //   if(zero_rh) {
    // 	cout<<"DEBUG: found zero rechit in one of the layer... skipping"<<endl;
    // 	continue;
    //   }
    // }
    for(int i=0;i<28;i++) {
      if(zero_rh[i]) continue;
      int temp_layer = i;
      int temp_chip = rechit_chip->at(dr_min_index[i]);
      int temp_channel = rechit_channel->at(dr_min_index[i]);
      int channel_bin = getBIN(temp_chip,temp_channel);
      if (channel_bin < 0) { cout<<"INVALID CHANNEL!!!"<<endl; return;}

      float adcHG = rechit_amplitudeHigh->at(dr_min_index[i]);
      h_adcHG_EE_track[i][channel_bin]->Fill(adcHG);

    }

    if(DEBUG) cout<<"DEBUG: Filling Histos!!!"<<endl;
    h_particleID->Fill(pdgID);
    h_beamEnergy->Fill(beamEnergy);
    h_runNumber->Fill(run);
    h_rechit_energy_all->Fill(rechitEnergySum);
    h_rechit_energy_EE_only->Fill(rechitEnergySum_EE);
    h_rechit_energy_FH_only->Fill(rechitEnergySum_FH);
    h_rechit_energy_EE_vs_FH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
    h_rechit_En_NRechits->Fill(NRechits,rechitEnergySum);
    // h_Nrechit_EE_vs_FH->Fill(Nrechit_EE,Nrechit_FH);
    // h_Nrechit_EE->Fill(Nrechit_EE);
    // h_Nrechit_FH->Fill(Nrechit_FH);

    if(DEBUG) cout<<"DEBUG: End of Event = "<<jentry+1<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;
  } // loop over entries

  cout<<"Got Out "<<jentry<<endl;
  // cout<<"% Events survived = "<<((float)MIP_pions/nEvents)*100 << endl;
  }

