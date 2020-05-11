#define AnalyzeHGCOctTB_cxx

#include <iostream>
#include <vector>
#include <cstring>
#include "AnalyzeHGCOctTB.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include <math.h>

using namespace std;



// chip 3022,44,3028




int main(int argc, char* argv[])
{

  if (argc < 6) {
    cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "configuration" 
	 <<" " << "energy" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *config          = argv[4];
  const char *energy = argv[5];
  AnalyzeHGCOctTB hgcOctTB(inputFileList, outFileName, data, config, energy);
  cout << "dataset " << data << " " << endl;
  cout << "configuration " << config << " " << endl;
  cout << "energy " << energy << " " << endl;

  hgcOctTB.EventLoop(data);
  return 0;
}

void AnalyzeHGCOctTB::EventLoop(const char *data) {


  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  // Long64_t nentries3 = fChain3->GetEntriesFast();
  Long64_t hgc_jentry = 0;

  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  Long64_t nbytes3 = 0, nb3 = 0;
  // Long64_t nbytes3 = 0, nb3 = 0;


  Long64_t region_1_classified_events = 0;
  Long64_t region_2_classified_events = 0;
  Long64_t region_3_classified_events = 0;
  Long64_t non_classified_events = 0;

  Long64_t hadronic_int = 0;
  Long64_t hard_hadronic_int = 0;
  Long64_t notFound_hadronic_int = 0;

  double within1Layer_cog = 0.0;
  double within2Layer_cog = 0.0;
  double total_hadInt_cog = 0.0;


  double within1Layer = 0.0;
  double within2Layer = 0.0;
  double total_hadInt = 0.0;

  double within1Layer_EE = 0.0;
  double within2Layer_EE = 0.0;
  double total_hadInt_EE = 0.0;

  double within1Layer_interface = 0.0;
  double within2Layer_interface = 0.0;
  double total_hadInt_interface = 0.0;

  double within1Layer_FH = 0.0;
  double within2Layer_FH = 0.0;
  double total_hadInt_FH = 0.0;



  int decade = 0;
  int ahc_zeroHit = 0;
  double ee_zeroHit = 0.0;
 
  
  float lambda[79];
  for(int i = 0; i < 79; i++) {
    lambda[i] = layer_positions[i+1].at(2);
  }

  

  //in mm
  float ahc_pos[39] = {27.45, 53.65, 79.85, 106.05, 132.25, 
		       158.45, 184.65,210.85,237.05, 263.25,
		       289.45, 315.65, 341.85, 368.05, 394.25,
		       420.45, 446.65, 472.85, 499.05, 525.25,
		       551.45, 577.65, 603.85, 630.05, 656.25, 
		       682.45, 708.65, 734.85, 761.05, 787.25,
		       813.45, 839.65, 865.85, 892.05, 918.25, 
		       944.45, 970.65, 996.85, 1075.45};
  //in cm
  float ahc_front = 169.9;


  bool DEBUG = false;
  int debug_count = 0;
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



  double E_beam = -1.0;




  if(DEBUG) cout<<"DEBUG: Configuration = "<<conf_<<endl;
  if(DEBUG) cout<<"DEBUG: TOTAL_ACTIVE_LAYER = "<<TOTAL_ACTIVE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: EE_LAYER = "<<EE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: FH_LAYER = "<<FH_LAYER<<endl;

  if(DEBUG) cout<<"DEBUG: Entering event Loop"<<endl;
  for(int i = 0; i< 28; i++){ cut_count[i] = 0;}
  Long64_t jentry = 0;;
  for (jentry=0; jentry<nentries;jentry++) {
  // for (jentry=0; jentry<nentries;jentry++,hgc_jentry++) {
    // cout<<"Hello"<<endl;
    //if(jentry == 10000 ) break;
    // Elayer.clear();
    // energy_ = -1.0;
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
    nb3 = fChain3->GetEntry(jentry); nbytes3 += nb3;

    
    // ===============Synch HGCAL & AHCAL with same entry == == == == == == == == == == ==

    // while(run != runNumber) {
    //   hgc_jentry++;
    //   nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
    //   nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    // }


    // if(jentry > 50) return;

    if(NRechits == 0) continue;
    // if(NRechits > 50 || NRechits < 20) continue;
    h_nTracks->Fill(ntracks);
    if(ntracks != 1) continue;
    E_beam = beamEnergy;
    
    h_particleID->Fill(pdgID);
    h_runNumber->Fill(run);
    h_beamEnergy->Fill(trueBeamEnergy);

    const int TOTAL_LAYERS = 79;
    Float_t track_x[TOTAL_LAYERS];
    Float_t track_y[TOTAL_LAYERS];

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

    for(int j = 0; j < 39; j++) {
      // double x = (rechit_z->at(j) - b_x)/m_x;
      // double y = (rechit_z->at(j) - b_y)/m_y;
      double z = ahc_front + (ahc_pos[j]/10.0) ;
      double x = (z * m_x) + b_x;
      double y = (z * m_y) + b_y;

      track_x[j+40] = x;
      track_y[j+40] = y;
      // cout<<"layer="<<rechit_layer->at(j)<<", z="<<rechit_z->at(j)
      // 	  <<" ;my (x,y)=("<<x<<","<<y<<") ;track(x,y)=("
      // 	  <<track_x[rechit_layer->at(j)-1]<<","
      // 	  <<track_y[rechit_layer->at(j)-1]<<")"<<endl;
    }


    Double_t rechitEnergySum = 0.0;
    // Double_t rechitEnergySum_mip_cut = 0.0;
    Double_t un_cali = 0.0;
    Long_t Nrechit_layer[40];
    // Long_t NRechits_EE[28];
    // Long_t NRechits_FH[12][7];

    Long_t NRechits_EE[28];
    Long_t NRechits_FH[12];
    Long_t NRechits_AH[39];
    Long_t Nrechit_EE_debug = 0;
    Double_t RechitsEn_EE[28];
    Double_t RechitsEn_FH[12];
    Double_t RechitsEn_AH[39];
    Double_t RechitEn_layer[40];
    // long Nrechit_layer[40];
    // long NRechits_EE[28];
    // long NRechits_FH[12][7];

    int module_part_ = -1;
    int module_layer_ = -1;
    int module_position_ = -1;
    // vector<double> dR[40];
    // double dr_min_index[40];
    // double dr_min[40];

    double energy_sum_dR2_[40];
    double cogX_[40];
    double cogY_[40];
    
    for(int ii=0;ii<40;ii++){
      if(ii<28) {
      	NRechits_EE[ii]=0;
      	RechitsEn_EE[ii] = 0.0;
      	// dR[ii].clear();
      	// dr_min_index[ii] = -1.0;
      	// dr_min[ii] = 1.e10;
      }
      else{
      	NRechits_FH[ii-28]=0;
      	RechitsEn_FH[ii-28] = 0.0;
      }
      Nrechit_layer[ii]=0;
      energy_sum_dR2_[ii] = 0.0;
      RechitEn_layer[ii] = 0.0;
      cogX_[ii] = 0.0;
      cogY_[ii] = 0.0;
      // dR[ii].clear();
      // dr_min_index[ii] = -1.0;
      // dr_min[ii] = 1.e10;
      if(ii < 39) {
      	RechitsEn_AH[ii] = 0.0;
      	NRechits_AH[ii] = 0;
      }

    }
    // for(int ii=0;ii<12;ii++){
    //   for(int jj=0;jj<7;jj++){
    //   NRechits_FH[ii][jj]=0;
    //   }
    // }


    // cout<<"BEFORE!!!"<<endl;

    // for(int ii=0;ii<12;ii++){
    //   for(int jj=0;jj<7;jj++){
    // 	NRechits_FH[ii][jj]++;
    // 	cout<<ii<<","<<jj<<" = "<<NRechits_FH[ii][jj]<<endl;;
    //   }
    // }

    /// FIll Tree
    if(DEBUG) cout<<"DEBUG: Start Analylizing RecHits!!"<<endl;
    if(DEBUG) cout<<"DEBUG: NRechits = "<<NRechits<<endl;


    bool DoMIPrescaling = false;

    std::vector<int> temp_moduleID;
    int Nrech_L1[4] = {0,0,0,0};
    for(int i = 0 ; i < NRechits; i++){
      temp_moduleID.clear();
      int temp_layer = rechit_layer->at(i);
      int temp_chip = rechit_chip->at(i);
      int temp_channel = rechit_channel->at(i);
      int en_chan = temp_chip*1000+temp_channel;
      
      double recx = rechit_x->at(i);
      double recy = rechit_y->at(i);
      int rechit_modID = (int)rechit_module->at(i);
      //std::pair<float, float> temp_mod_chip((int)rechit_module->at(i),(int)rechit_chip->at(i));
      std::pair<float, float> temp_mod_chip(rechit_modID,temp_chip);


      
      //channel masking
      if(en_chan == 3022 || en_chan == 3028 || en_chan == 44) continue;
      if(temp_layer==1 && temp_chip==0) continue;
      if(temp_layer==1) Nrech_L1[temp_chip]++;
      
      // noise cut
      
      float noise_chip = getNoise(temp_mod_chip);
      if(DEBUG && false) cout<<"Module, layer, chip , noise = "<<rechit_module->at(i)<<", "<<rechit_layer->at(i)<<", "<<rechit_chip->at(i)<<", "<<noise_chip<<endl;
      if(temp_layer <= 28 && rechit_amplitudeHigh->at(i) < 3*noise_chip) continue;
      else if(temp_layer > 28 && rechit_amplitudeHigh->at(i) < 4*noise_chip) continue;

      //if(rechit_amplitudeHigh->at(i) < 3*noise_chip) continue;

      
      float mip_ratio_chip = getMIPRatio(temp_mod_chip);
      bool Is200umCell = false;
      if(rechit_modID == 144 || rechit_modID == 145 || rechit_modID == 146 || rechit_modID == 147) Is200umCell = true;

      // // energy scaling using MIPs
      if(DoMIPrescaling) {
      	if(mip_ratio_chip > -1.0 && !Is200umCell) {
      	  rechit_energy->at(i) = rechit_energy->at(i)/mip_ratio_chip;
      	  if(rechit_modID == 144 || rechit_modID == 145 || rechit_modID == 146 || rechit_modID == 147) 
      	    cout<<"BUG found"<<endl;
      	}
      }

      //5% rescaling 
      // rechit_energy->at(i) = rechit_energy->at(i)*0.95;

      if(temp_layer <= 28) Nrechit_EE_debug++;
      
      double trackx = track_x[temp_layer-1];
      double tracky = track_y[temp_layer-1];
      if(temp_layer == 2)
	       dX_dY_layer_2->Fill(recx-trackx,recy-tracky);
      
      h_dX_dY_track_vs_rechit[temp_layer-1]->Fill(recx-trackx,recy-tracky);

      if(!strcmp(data,"data")) {
      	trackx = -1*trackx;
      	tracky = -1*tracky;
      	std::pair<float, float> dxy_al = dxy_alignment(temp_layer);
      	float dx_corr = dxy_al.first;
      	float dy_corr = dxy_al.second; 
      	recx -= dx_corr;
      	recy -= dy_corr;
      	
      	// noise cut
      	// std::pair<float, float> temp_mod_chip((int)rechit_module->at(i),(int)rechit_chip->at(i));
      	// float noise_chip = getNoise(temp_mod_chip);
      	// if(rechit_amplitudeHigh->at(i) < 3*noise_chip)
      	//   continue;
      	// if(temp_layer < 29 && rechit_amplitudeHigh->at(i) < (3*noise[temp_layer-1])) 
      	//   continue;
      	// if((temp_layer > 29 && rechit_amplitudeHigh->at(i) < (3*5.5)))
      	//   continue;
      	
      	// if(((rechit_module->at(i) == 146) || (rechit_module->at(i) == 147)) && rechit_amplitudeHigh->at(i) < (3*6.0))
      	//   continue;
      	
      }
      
      // else if(!strcmp(data,"sim")) {
      // 	if(rechit_amplitudeHigh->at(i) < 18) continue;
      // 	//if(rechit_module->at(i) == 42) continue;
      // }
      Nrechit_layer[temp_layer-1]++;
      RechitEn_layer[temp_layer-1]+=rechit_energy->at(i);
      
      temp_moduleID = getModuleLocation(rechit_module->at(i));
      
      cogX_[temp_layer-1] += (rechit_x->at(i)*rechit_energy->at(i));
      cogY_[temp_layer-1] += (rechit_y->at(i)*rechit_energy->at(i));



      if((temp_layer==38 && rechit_module->at(i)!=62) || (temp_layer==39 && rechit_module->at(i)!=54) || (temp_layer==40 && rechit_module->at(i)!=43)){
      	cout<< jentry+1 << " = " << rechit_module->at(i)<<endl;
      }


      if(!temp_moduleID.size() || temp_moduleID.size()<3) {
      	cout<<"ERROR: Could NOT locate MODULE location for module "<<rechit_module->at(i)<<endl;
      	cout<<"\t more info temp_moduleID.size() = "<<temp_moduleID.size()<<endl;
      	return;
      }
      module_part_ = temp_moduleID.at(0);
      module_layer_ = temp_moduleID.at(1);
      module_position_ = temp_moduleID.at(2);
      
      
      h_moduleID->Fill(module_part_);
      


      
      
      /////////////////// shower finding algo ////////
      
      double dR = deltaR(recx,recy,trackx,tracky);
      // cout<<rechit_layer->at(i)<<" => rec(X,Y) = ("<<recx<<","<<recy
      // 	  <<"), track(X,Y) = ("<<trackx<<","<<tracky
      // 	  <<")\t dR = "<<dR<<endl;
      if(rechit_layer->at(i) > 28)
      {
        if(dR < 2.0) 	energy_sum_dR2_[temp_layer-1]+=rechit_energy->at(i);
      }
      else 
      {
        if(dR < 2.0)  energy_sum_dR2_[temp_layer-1]+=rechit_energy->at(i);
      }
      
            
      ///////////////////////////////////////////////
      
      
      if(module_part_ == 0) {
      	// rechitEnergySum_EE+=rechit_energy->at(i);
      	RechitsEn_EE[module_layer_-1]+=rechit_energy->at(i);
      	NRechits_EE[module_layer_-1]++;
      }
      else if(module_part_ == 1) {
      	// rechitEnergySum_FH+=rechit_energy->at(i);
      	RechitsEn_FH[module_layer_-1]+=rechit_energy->at(i);
      	NRechits_FH[module_layer_-1]++;
	
      }
      else {
      	cout<<"ERROR: Unknown Module Part detected!!!!"<<endl;
      	return;
      	
      }
      rechitEnergySum+=rechit_energy->at(i);
      
    }
    if(Nrechit_EE_debug == 0)  {
      //cout<<jentry<<" : "<<energyLostEE<<" : "<<nsec<<" : "<<HadInt_z<<endl;
      continue;
    }

    ////////////////////////////////////////////
    //            AHCAL Part                  //
    ////////////////////////////////////////////
    
    Double_t rechitEnergySumAHCAL = sim_energyAH;
    // for(int i = 0 ; i < ahc_nHits; i++) {
    //   int temp_layer = ahc_hitK->at(i);
    //   RechitsEn_AH[temp_layer -1] += ahc_hitEnergy->at(i);
    //   rechitEnergySumAHCAL += ahc_hitEnergy->at(i);
    //   NRechits_AH[temp_layer -1]++;
    // }

    /////////////////////////////////////////////

    if(DEBUG) cout<<"DEBUG: For shower start"<<endl;
    if(DEBUG && false) {
      for (int ll = 0; ll < 40; ll++) {
      	cout<<ll+1<<"\t"<<energy_sum_dR2_[ll]<<endl;
      }
    }

   
    // if(jentry < 5) {
    //   cout<<"N E W   E V E N T:"<<jentry+1<<endl;
    //   for(int ii = 0; ii < 40; ii++) {
    // 	cout<<"Layer: "<<ii+1<<" E="<<energy_sum_dR2_[ii]<<endl;
    //   }
    // }
    // else
    //   break;
    



    
    /////////////////// shower finding algo ////////
    bool MIP = true;
    int shower_start_index = -1;
    float shower_lambda_ = -1.0;
    float shower_weight_ = 1.0;
    float energy_thres = 20.0;
    if(energy_sum_dR2_[0] > energy_thres) {
      // cout<<"Shower started in layer =1"<<endl;
      shower_start_index = 0;
      shower_lambda_ = lambda[0];
      shower_weight_ = lambda[0];
      MIP = false;
    }
    
    else if(energy_sum_dR2_[1] > energy_thres && energy_sum_dR2_[1] > 2*energy_sum_dR2_[0]) {
      // cout<<"Shower started in layer =2"<<endl;
      shower_start_index = 1;
      shower_lambda_ = lambda[1];
      shower_weight_ = lambda[1]-lambda[0];
      MIP = false;
    }
    
    else {
      
      for(int i = 2; i < 40; i++) {
      	if((energy_sum_dR2_[i] > energy_thres) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-1]) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-2])) {

      	  // if(i+1==27 || i+1==28) {
      	  //   cout<<"Shower started in layer ="<<i+1<<" ,lambda_ = "<<lambda[i]<<endl;
      	  // }
      	  shower_start_index = i;
      	  shower_lambda_ = lambda[i];
      	  shower_weight_ = lambda[i]-lambda[i-1];
      	  MIP = false;
      	  break;
      	}
      }
    }


    int closest_layer = getNextLayer(HadInt_z);
    if(closest_layer > 40) closest_layer = -1;



    // if(foundHadInt && closest_layer > 10)
    // // if(foundHadInt && (HadInt_z > 539.035))

    
    // if(foundHadInt && closest_layer > 10)
    //   cout<<jentry+1<<" Shower Start : HadInt_z : NextClosestLayer is \t "<<shower_start_index<<" : "<<HadInt_z<<" : "<<closest_layer<<endl;

    
    // cout<<"\t First hadronic interaction :  "<<HadInt_z<<endl;

    // if(shower_start_index > -1 && foundHadInt) {
    double nsc_total_KE = 0.0;
    vector<double> KE_list;
    double leadingHadKE = 0.0;
    double E_gen_kin = 0.0;
    KE_list.clear();
    double E_gen_kin_thres = 0.0;
    // if(E_beam == 20)  { E_gen_kin_thres = 10.0; }
    // if(E_beam == 100) E_gen_kin_thres = 60.0;
    

   

    if(!foundHadInt) notFound_hadronic_int++;
    if(foundHadInt) {
      hadronic_int++;
      for(int i = 0 ; i < nsec; i++) {
        nsc_total_KE += sec_kin->at(i);
        if(abs(sec_pdgID->at(i)) == 211 && sec_charge->at(i) == -1) KE_list.push_back(sec_kin->at(i));

      }
      leadingHadKE = getLeadingKE(KE_list);
      E_gen_kin = nsc_total_KE - leadingHadKE;
      if(leadingHadKE > 0) {
             if(E_gen_kin > 10 && false) {
                cout<<jentry<<" NextClosestLayer is \t "<<closest_layer<<endl;
                // cout<<jentry<<" Shower Start : HadInt_z : NextClosestLayer is \t "<<shower_start_index+1<<" : "<<HadInt_z<<" : "<<closest_layer<<endl;
                          }
             if(E_gen_kin < E_gen_kin_thres) continue;
             hard_hadronic_int++;
            
            if(jentry < 20 && false) {
              cout<<jentry<<" : "<<nsec<<" : "<<nsc_total_KE<< " : "<<leadingHadKE<<" : "<<E_gen_kin<<endl;
            }
            //h_2D_nsc_correlation->Fill(nsc_total_KE,nsec);
            h_2D_nsc_correlation->Fill(E_gen_kin,nsec);
            h_nsc->Fill(nsec);
            h_seckin->Fill(nsc_total_KE);
            h_had_leading_KE->Fill(leadingHadKE);
            h_gen_kin->Fill(E_gen_kin);
            h_2D_nsc_correlation_frac->Fill(E_gen_kin/E_beam,nsec);
            h_2D_hlead_sec_frac->Fill(E_gen_kin/E_beam,leadingHadKE/E_beam);

            if(closest_layer >= 3 && closest_layer <= 28) {
              h_EE_layer_i->Fill(energy_sum_dR2_[closest_layer-1]);
              h_EE_layer_i_1->Fill(energy_sum_dR2_[closest_layer-2]);
              h_EE_layer_i_2->Fill(energy_sum_dR2_[closest_layer-3]);

              h_EE_layer_i_1_ratio->Fill(energy_sum_dR2_[closest_layer-1]/energy_sum_dR2_[closest_layer-2]);
              h_EE_layer_i_2_ratio->Fill(energy_sum_dR2_[closest_layer-1]/energy_sum_dR2_[closest_layer-3]);
            }
            else if(closest_layer > 28 && closest_layer <= 31) {
              h_FH_interface_layer_i->Fill(energy_sum_dR2_[closest_layer-1]);
              h_FH_interface_layer_i_1->Fill(energy_sum_dR2_[closest_layer-2]);
              h_FH_interface_layer_i_2->Fill(energy_sum_dR2_[closest_layer-3]);
              h_FH_interface_layer_i_1_ratio->Fill(energy_sum_dR2_[closest_layer-1]/energy_sum_dR2_[closest_layer-2]);
              h_FH_interface_layer_i_2_ratio->Fill(energy_sum_dR2_[closest_layer-1]/energy_sum_dR2_[closest_layer-3]);
            }
            else if(closest_layer >= 32 && closest_layer <= 40) {
             h_FH_layer_i->Fill(energy_sum_dR2_[closest_layer-1]);
             h_FH_layer_i_1->Fill(energy_sum_dR2_[closest_layer-2]);
             h_FH_layer_i_2->Fill(energy_sum_dR2_[closest_layer-3]); 
             h_FH_layer_i_1_ratio->Fill(energy_sum_dR2_[closest_layer-1]/energy_sum_dR2_[closest_layer-2]);
             h_FH_layer_i_2_ratio->Fill(energy_sum_dR2_[closest_layer-1]/energy_sum_dR2_[closest_layer-3]);
            }

            if(closest_layer >= 3 && closest_layer <= 40 ) {
              if(abs(closest_layer - (shower_start_index +1)) <= 1) within1Layer++;
              if(abs(closest_layer - (shower_start_index +1)) <= 2) within2Layer++;

              total_hadInt++;

              if(closest_layer >= 3 && closest_layer <= 28) {
                if(abs(closest_layer - (shower_start_index +1)) <= 1) within1Layer_EE++;
                if(abs(closest_layer - (shower_start_index +1)) <= 2) within2Layer_EE++;
                // else {
                //   if(shower_start_index+1 >  closest_layer && false) {
                //   bool cond1 = (energy_sum_dR2_[closest_layer-1] > 20);
                //   bool cond2 = (energy_sum_dR2_[closest_layer-1] > 2*energy_sum_dR2_[closest_layer-2]);
                //   bool cond3 = (energy_sum_dR2_[closest_layer-1] > 2*energy_sum_dR2_[closest_layer-2]);
                //  cout<<jentry<<" : "<<closest_layer<<" : "<<(shower_start_index +1)<<" :: "<<cond1<<" : "<<cond2
                //     <<" : "<<cond3<<endl;
                //   cout<<"\tcond1 : cond2 : cond3 :: "<<energy_sum_dR2_[closest_layer-1]
                //     <<" : "<<(energy_sum_dR2_[shower_start_index]/energy_sum_dR2_[closest_layer-2])
                //     <<" : " <<(energy_sum_dR2_[shower_start_index]/energy_sum_dR2_[closest_layer-2])<<endl;
                //   }
                // }
                total_hadInt_EE++;
              }
              else if(closest_layer > 28 && closest_layer <= 31) {
                if(abs(closest_layer - (shower_start_index +1)) <= 1) within1Layer_interface++;
                if(abs(closest_layer - (shower_start_index +1)) <= 2) within2Layer_interface++;
                total_hadInt_interface++;
              }
              else if(closest_layer >= 32 && closest_layer <= 40) {
                if(abs(closest_layer - (shower_start_index +1)) <= 1) within1Layer_FH++;
                if(abs(closest_layer - (shower_start_index +1)) <= 2) within2Layer_FH++;
                else {
                  if(shower_start_index+1 <  closest_layer && false) {
                  bool cond1 = (energy_sum_dR2_[closest_layer-1] > 20);
                  bool cond2 = (energy_sum_dR2_[closest_layer-1] > 2*energy_sum_dR2_[closest_layer-2]);
                  bool cond3 = (energy_sum_dR2_[closest_layer-1] > 2*energy_sum_dR2_[closest_layer-2]);
                 cout<<jentry<<" : "<<closest_layer<<" : "<<(shower_start_index +1)<<" :: "<<cond1<<" : "<<cond2
                    <<" : "<<cond3<<endl;
                  cout<<"\tcond1 : cond2 : cond3 :: "<<energy_sum_dR2_[closest_layer-1]
                    <<" : "<<(energy_sum_dR2_[shower_start_index]/energy_sum_dR2_[closest_layer-2])
                    <<" : " <<(energy_sum_dR2_[shower_start_index]/energy_sum_dR2_[closest_layer-2])<<endl;
                  }
                }
                total_hadInt_FH++;
              }

            }
            
      }
    
      h_delta_layer->Fill(closest_layer - (shower_start_index +1));
      h_truth_reco_SS_corr->Fill(closest_layer,(shower_start_index +1));
      if(shower_start_index < 0) {
      	h_truth_reco_SS_corr_neg1->Fill(closest_layer,(shower_start_index));
      	h_delta_layer_neg1->Fill(closest_layer - (shower_start_index));
      }
      else {
      	h_truth_reco_SS_corr_neg1->Fill(closest_layer,(shower_start_index+1));
      	h_delta_layer_neg1->Fill(closest_layer - (shower_start_index +1));
      }
    }



    h_shower_start_check->Fill(shower_lambda_);
    if(closest_layer < 0.0) h_hadronic_Int->Fill(closest_layer);
    else h_hadronic_Int->Fill(lambda[closest_layer-1]);
	

    
    // if(MIP) {
    //   // cout<<"MIP like event"<<endl;
    //   //shower_start_index = -1;
    //   h_shower_start->Fill(-1.0);
    // }
    // else {
    //   h_shower_start->Fill(lambda[shower_start_index]);
    // }
    h_shower_start->Fill(shower_lambda_);
    h_shower_start_dN_dLambda->Fill(shower_lambda_,1/shower_weight_);

    ///////////////////////////////////////////////
    //                Collapsed EE               //
    ///////////////////////////////////////////////
    if(shower_start_index+1 <= 28 && (shower_start_index+1) > 0) {
      // cout<<""
      h_shower_start_full_collapsed_EE->Fill(lambda[0]);
      if((shower_start_index+1)%2 == 0) {
    	h_shower_start_part_collapsed_EE->Fill(lambda[shower_start_index-1]);
    	// cout<<shower_start_index<<" "<<(shower_start_index+1)%2<<" "<<shower_start_index-1<<" "<<lambda[(shower_start_index+1)/2]<<endl;
      }
      else {
    	h_shower_start_part_collapsed_EE->Fill(lambda[shower_start_index]);
      }
    }
    else {
      h_shower_start_full_collapsed_EE->Fill(shower_lambda_);
      h_shower_start_part_collapsed_EE->Fill(shower_lambda_);
    }

    if(DEBUG) cout<<"DEBUG: Shower start index = "<<shower_start_index<<endl;

    ///////////////////////////////////////////////
    // if(jentry > 50)
    //   return;

    

    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;
    Double_t test = 0.0;
    bool zero_rh[40];
    
    // cout<<"*** NEW EVENT="<<jentry+1<<",shower_start="<<shower_start_index<<" ***"<<endl;
    // cout<<"layer:track(x,y):cog(x,y)"<<endl;
    bool DoWeight = false;
    double beta_ = -1.0;
    switch((int)beamEnergy) {
    case 20: beta_ = 4.207; break;
    case 50: beta_ = 4.752; break;
    case 80: beta_ = 5.009; break;
    case 100: beta_ = 5.174; break;
    case 120: beta_ = 5.074; break;
    case 200: beta_ = 5.094; break;
    case 250: beta_ = 5.305; break;
    case 30: beta_ = 5.253; break;
    }
    if(!DoWeight) beta_ = 1.0;
      
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
      if(iL < EE_LAYER) {
      	Nrechit_EE+=NRechits_EE[iL];
      	rechitEnergySum_EE+=RechitsEn_EE[iL];

      	cogX_[iL] = cogX_[iL]/RechitsEn_EE[iL];
      	cogY_[iL] = cogY_[iL]/RechitsEn_EE[iL];

      	h_Rechits_En_inclusive[iL]->Fill(RechitsEn_EE[iL]);
      	h_longi_profile_inclusive->Fill(lambda[iL],RechitsEn_EE[iL]);
      	if(!MIP) {
      	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],RechitsEn_EE[iL]);
      	  h_longi_profile_region3->Fill(lambda[iL],RechitsEn_EE[iL]);
      	  h_Rechits_En_SS[shower_start_index][iL]->Fill(RechitsEn_EE[iL]);

      	}
      }
      else {
      	Nrechit_FH+=NRechits_FH[iL-EE_LAYER];
      	rechitEnergySum_FH+=RechitsEn_FH[iL-EE_LAYER];
      	cogX_[iL] = cogX_[iL]/RechitsEn_FH[iL-EE_LAYER];
      	cogY_[iL] = cogY_[iL]/RechitsEn_FH[iL-EE_LAYER];
      	h_Rechits_En_inclusive[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
      	h_longi_profile_inclusive->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
      	if(!MIP) {
      	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
      	  h_Rechits_En_SS[shower_start_index][iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);

      	}
      }
      
      h_trackX_trackY[iL]->Fill(track_x[iL],track_y[iL]);
      h_cogX_cogY[iL]->Fill(cogX_[iL],cogY_[iL]);
      h_cogX_trackX[iL]->Fill(cogX_[iL],track_x[iL]);
      h_cogY_trackY[iL]->Fill(cogY_[iL],track_y[iL]);
      h_dX_dY[iL]->Fill(cogX_[iL]-track_x[iL],cogY_[iL]-track_y[iL]);
      h_dX[iL]->Fill(cogX_[iL]-track_x[iL]);
      h_dY[iL]->Fill(cogY_[iL]-track_y[iL]);

      //cout<<iL+1<<":("<<track_x[iL]<<","<<track_y[iL]<<"):("<<cogX_[iL]<<","<<cogY_[iL]<<")"<<endl;
    }
    
   
    ////////////////////////////////////////////////////////////////////////////
    //////////////////// For shower start finder optimization    /////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    double SS_indR2[40];
    double SS_indR5[40];
    double SS_indR10[40];
    double SS_outdR2[40];

    int SS_inNrec_dR5[40];
    int SS_inNrec_dR10[40];
    //int SS_inNrec_dR5[40];
    for(int ii=0;ii<40;ii++){ SS_indR2[ii] = 0.0; SS_outdR2[ii] = 0.0; SS_indR5[ii] = 0.0; SS_indR10[ii] = 0.0; SS_inNrec_dR5[ii] = 0; SS_inNrec_dR10[ii] = 0; }

    for(int i = 0 ; i < NRechits; i++){
      temp_moduleID.clear();
      int temp_layer = rechit_layer->at(i);
      int temp_chip = rechit_chip->at(i);
      int temp_channel = rechit_channel->at(i);
      int en_chan = temp_chip*1000+temp_channel;
      
      double recx = rechit_x->at(i);
      double recy = rechit_y->at(i);
      int rechit_modID = (int)rechit_module->at(i);
      std::pair<float, float> temp_mod_chip(rechit_modID,temp_chip);
      //channel masking
      if(en_chan == 3022 || en_chan == 3028 || en_chan == 44) continue;
      if(temp_layer==1 && temp_chip==0) continue;
     
      // noise cut
      float noise_chip = getNoise(temp_mod_chip);
      if(temp_layer <= 28 && rechit_amplitudeHigh->at(i) < 3*noise_chip) continue;
      else if(temp_layer > 28 && rechit_amplitudeHigh->at(i) < 4*noise_chip) continue;

      if(!strcmp(data,"data")) {
        std::pair<float, float> dxy_al = dxy_alignment(temp_layer);
        float dx_corr = dxy_al.first;
        float dy_corr = dxy_al.second; 
        recx -= dx_corr;
        recy -= dy_corr;
      }
      /////////////////// shower finding algo ////////
      
      double dR = deltaR(recx,recy,cogX_[temp_layer-1],cogY_[temp_layer-1]);
      // if(jentry == 1 && temp_layer == 26) {
      //   cout<<temp_layer<<" => rec(X,Y) = ("<<recx<<","<<recy
      //      <<"), energy= "<<rechit_energy->at(i)<<" cog(X,Y) = ("<<cogX_[temp_layer-1]<<","<<cogY_[temp_layer-1]
      //      <<")\t dR = "<<dR<<endl;
      // }

      
      if(dR < 2.0) SS_indR2[temp_layer-1] += rechit_energy->at(i);
      else SS_outdR2[temp_layer-1] += rechit_energy->at(i);
      if(dR < 5.0) { SS_indR5[temp_layer-1] += rechit_energy->at(i); SS_inNrec_dR5[temp_layer-1]++;}
      if(dR < 10.0) { SS_indR10[temp_layer-1] += rechit_energy->at(i); SS_inNrec_dR10[temp_layer-1]++;}

      //if(dR < 5.0) SS_inNrec_dR5[temp_layer-1]++;
    }

   

    // if(E_gen_kin/E_beam > 0.5 && closest_layer > 2 && closest_layer < 25) {
    //   cout<<" **********   EVENT = "<<jentry<<" :: closest_layer = "<<closest_layer<<" *****************"<<endl;
    //   for(int ii=0; ii<28; ii++){ 
    //     //if(SS_indR2[ii] + SS_outdR2[ii] > 0.0)  cout<<"Layer : Ratio : nrechit_indR5 :: "<<ii+1<<" : "<<((SS_indR2[ii])/(SS_indR2[ii] + SS_outdR2[ii]))<<" : "<<SS_inNrec_dR5[ii]<<endl;
    //     if(SS_indR5[ii] > 0.0)  cout<<"Layer : Ratio : nrechit_indR5 :: "<<ii+1<<" : "<<((SS_indR2[ii])/(SS_indR5[ii]))<<" : "<<SS_inNrec_dR5[ii]<<endl;
    //     else cout<<"Layer : Ratio :: "<<ii+1<<" : -1"<<endl;
        
    //   }
    // }

    bool show = true;
    if(E_gen_kin/E_beam > 0.4 && closest_layer > 2 && closest_layer < 39 && debug_count < 200) {
      // if(show) cout<<" **********   EVENT = "<<jentry<<" :: closest_layer = "<<closest_layer<<" *****************"<<endl;
      if (show) { 
        cout<<" **********   EVENT = "<<jentry<<" :: closest_layer = "<<closest_layer<<" *****************"<<endl;
        cout<<endl<<"Layer : Nrechit(10cm) :  Energy(2cm) :  Energy(5cm) : Energy(10cm) : Ratio(2cm/5cm) : Ratio(2cm/10cm)"<<endl<<endl;;
      }
      for(int ii=0; ii < 40; ii++){ 
        if(show) {
          // if(SS_indR5[ii] > 0.0)  cout<<"Layer : Ratio : nrechit_indR5 :: "<<ii+1<<" : "<<((SS_indR2[ii])/(SS_indR5[ii]))<<" : "<<SS_inNrec_dR5[ii]<<endl;
          char* toShow = new char[2000];
          sprintf(toShow,"%d : %d : %0.2f : %0.2f : %0.2f : %0.2f : %0.2f",ii+1,SS_inNrec_dR10[ii],SS_indR2[ii],SS_indR5[ii],SS_indR10[ii],((SS_indR2[ii])/(SS_indR5[ii])),((SS_indR2[ii])/(SS_indR10[ii])));
          if(SS_indR10[ii] > 0.0 && show) { 
            cout<<toShow<<endl;
            // cout<< ii+1 <<" : "<< SS_inNrec_dR10[ii] << " : " 
            //     << SS_indR2[ii] << " : " 
            //     << SS_indR5[ii] << " : "
            //     << SS_indR10[ii] << " : "
            //     << ((SS_indR2[ii])/(SS_indR5[ii])) << " : "
            //     << ((SS_indR2[ii])/(SS_indR10[ii])) << endl;
                
          }
          else  {
            //cout<<"Layer : Ratio :: "<<ii+1<<" : -1"<<endl;
            cout<<ii+1<<" : -1"<<endl;
          }
        }
        
        if(SS_inNrec_dR5[ii] > 5 && ((SS_indR2[ii])/(SS_indR5[ii])) < 0.9)  {
          if(abs(closest_layer - ii+1) <= 1) within1Layer_cog++;
          if(abs(closest_layer - ii+1) <= 2) within2Layer_cog++;
          if(!show) break;
        }
        
      }
      debug_count++;
      total_hadInt_cog++;
    }

    ////////////////////////////////////////////////////////////////////////////


    if(Nrechit_EE == 0) {
      //cout<<" Nrechit_EE = "<<Nrechit_EE<<endl;
      // cout<<jentry<<" Shower Start : HadInt_z : NextClosestLayer is \t "<<shower_start_index+1<<" : "<<HadInt_z<<" : "<<closest_layer<<endl;
      // cout<<"\t"<<track_x[0]<<" : "<<track_y[0]<<" :: process = "<<process<<endl;
      //if(E_gen_kin < 50) cout<<"E_gen_kin = "<<E_gen_kin<<endl;
      h_hadInteraction_withzeroEEhits->Fill(HadInt_z/100.0);
      ee_zeroHit++;
    }
    else {
      h_hadInteraction_withNonzeroEEhits->Fill(HadInt_z/100.0);
    }

    //if(jentry > 5) return;
    if(DEBUG) cout<<"DEBUG: NRechits in EE = "<<Nrechit_EE<<"\t Rechit_energy in EE = "<<rechitEnergySum_EE<<endl;
    if(DEBUG) cout<<"DEBUG: NRechits in FH = "<<Nrechit_FH<<"\t Rechit_energy in FH = "<<rechitEnergySum_FH<<endl;
    // h_Nrechit_EE->Fill(Nrechit_EE);
    // h_Nrechit_FH->Fill(Nrechit_FH);
    // h_Nrechit_AH->Fill(ahc_nHits);
    // if(Nrechit_EE < 40 && Nrechit_FH < 50)
    //   continue;

    // if(ahc_nHits > 30 && ahc_nHits < 60) {
    //   h_rechit_energy_AHCAL_MIPwindow->Fill(rechitEnergySumAHCAL);
    //   h_rechit_energy_AHCAL_MIPwindow2D->Fill(rechitEnergySumAHCAL, ahc_nHits);
    // }

   
    double isMuonLike = false;
    bool isRegion1 = false;
    bool isRegion2 = false;
    bool isRegion3 = false;




    ///////////////////////////////////////////////////////////
    //// Original Event Classification                       //
    ///////////////////////////////////////////////////////////

    // if(Nrechit_EE < 80 && Nrechit_FH < 60) {  //MIP LIKE
    //   isRegion1 = true;
    //   isRegion2 = false;
    //   isRegion3 = false;

    // }
    // else if(Nrechit_EE < 80) { // H-Hadron
    //   //classify as H-hadron
    //   isRegion2 = true;
    //   isRegion1 = false;
    //   isRegion3 = false;
    // }
    // else {  // EH-Hadron
    //   isRegion3 = true;
    //   isRegion1 = false;
    //   isRegion2 = false;
	
    // }



    ///////////////////////////////////////////////////////////
    //// WITH ENERGY RATIOS  (IMPROVED Event Classification) //
    ///////////////////////////////////////////////////////////

    // if(Nrechit_EE < 80 && Nrechit_FH < 60 && shower_start_index == -1) {  //MIP LIKE
    //   isRegion1 = true;
    //   isRegion2 = false;
    //   isRegion3 = false;
    //   h_shower_start_reg1->Fill(shower_lambda_);
    //   region_1_classified_events++;
    // }
    // else if(Nrechit_EE < 80 && shower_start_index > 27) { //H hadrons
    //   isRegion1 = false;
    //   isRegion2 = true;
    //   isRegion3 = false;
    //   h_shower_start_reg2->Fill(shower_lambda_);
    //   region_2_classified_events++;
    // }
    // else if(Nrechit_EE > 80 && shower_start_index < 28  && shower_start_index > -1){  //EH hadron
    //   isRegion1 = false;
    //   isRegion2 = false;
    //   isRegion3 = true;
    //   h_shower_start_reg3->Fill(shower_lambda_);
    //   region_3_classified_events++;
    // }
    // else {
    //   h_shower_start_CHECK_FOR_UNCLASSIFIED->Fill(shower_lambda_);
    //   h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED->Fill(Nrechit_EE,Nrechit_FH);
    //   if(shower_start_index == -1)
    // 	h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R1->Fill(Nrechit_EE,Nrechit_FH);
    //   else if (shower_start_index < 28  && shower_start_index > -1)
    // 	h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R3->Fill(Nrechit_EE,Nrechit_FH);
    //   else
    // 	h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R2->Fill(Nrechit_EE,Nrechit_FH);
	     
    //   non_classified_events++;
    // }

    /////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////
    //// IMPROVED Event Classification only with shower start //
    ////////////////////////////////////////////////////////////

    if(shower_start_index == -1) {  //MIP LIKE
      isRegion1 = true;
      isRegion2 = false;
      isRegion3 = false;
      h_shower_start_reg1->Fill(shower_lambda_);
      region_1_classified_events++;
    }
    else if(shower_start_index > 27) { //H hadrons
      isRegion1 = false;
      isRegion2 = true;
      isRegion3 = false;
      h_shower_start_reg2->Fill(shower_lambda_);
      region_2_classified_events++;
    }
    else {  //EH hadron
      isRegion1 = false;
      isRegion2 = false;
      isRegion3 = true;
      h_shower_start_reg3->Fill(shower_lambda_);
      region_3_classified_events++;
    }


    // else {  //EH hadron
    //   isRegion1 = false;
    //   isRegion2 = false;
    //   isRegion3 = true;
    // }
    




    h_Nrechit_EE->Fill(Nrechit_EE);
    h_Nrechit_FH->Fill(Nrechit_FH);
    // h_Nrechit_AH->Fill(ahc_nHits);
    //rechitEnergySum_EE
    h_rechit_energy_raw_EE->Fill(rechitEnergySum_EE);    
    h_rechit_energy_raw_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);    

    h_rechit_energy_raw_low_EE->Fill(rechitEnergySum_EE);    
    h_rechit_energy_raw_low_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_low_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);    


    h_rechit_energy_raw_mid_EE->Fill(rechitEnergySum_EE);    
    h_rechit_energy_raw_mid_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_mid_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);    

    h_rechit_energy_raw_high_EE->Fill(rechitEnergySum_EE);    
    h_rechit_energy_raw_high_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_high_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);    




    h_Nrechit_EE_vs_FH->Fill(Nrechit_EE,Nrechit_FH);
    ///////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    // h_rechit_energy_FH_AHCAL_FB->Fill(rechitEnergySum_FH,rechitEnergySumAHCAL);
    // h_rechit_energy_EE_FH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);

    if(isRegion2) {
    // if(!isRegion3) {
      // if(ahc_nHits > 35 && ahc_nHits < 45 && rechitEnergySumAHCAL < 250) {
      // 	cout<<"Do NOT caluculate"<<endl;
      // 	continue;
      // }
      // h_Nrechit_EE->Fill(Nrechit_EE);
      // h_Nrechit_FH->Fill(Nrechit_FH);
      // h_Nrechit_AH->Fill(ahc_nHits);

      // h_Nrechit_EEFH_vs_AH->Fill(Nrechit_EE+Nrechit_FH,ahc_nHits);
      // double a = 0.0351296;
      // double b = 0.031534;
      // h_Nrechits_EE_FH_FB->Fill(Nrechit_EE,Nrechit_FH);
      h_rechit_energy_inEE_region2_mips->Fill(rechitEnergySum_EE);
      double a = -1.0;
      double b = -1.0;
      // double a = 0.0611934;
      // double b = 0.0183451;
      double alpha = -1.0;  // FH & AHCAL relative weight
      double gamma = -1.0;
      //double MIP_to_GeV_conversion_factor = 12.83;
      
      if(beamEnergy == 20) {
      	// a = 0.0611934; b = 0.0183451;
      	//a = 0.0630539 ;b = 0.0120932;
      	a = 0.0604318 ;b = 0.0307894;
      	//alpha = 0.45; gamma = 13.205;
      	alpha = 0.402; gamma = 12.826;
      }
      if(beamEnergy == 50) {
      	// a = 0.0608286 ;b = 0.0136046;
      	//a = 0.076075 ;b = 0.0116685;
      	a = 0.0679195 ;b = 0.0324864;
      	//alpha = 0.40; gamma = 12.856;
      	alpha = 0.404; gamma = 12.811;
      }
      if(beamEnergy == 80) {
      	// a = 0.0622612 ;b = 0.0152219;
      	//a = 0.0788491 ;b = 0.013591;
      	a = 0.0683456 ;b = 0.0320886;
      	//alpha = 0.40; gamma = 13.1375;
      	alpha = 0.405; gamma = 13.225;
      }
      if(beamEnergy == 100) {
      	//a = 0.0786438 ;b = 0.0151615;
      	a = 0.0677498 ;b = 0.031738;
      	//alpha = 0.40; gamma = 13.38;
      	alpha = 0.389; gamma = 13.323;
      }
      if(beamEnergy == 120) {
      	// a = 0.0600868 ;b = 0.018315;
      	//a = 0.0794755 ;b = 0.0174122;
      	a = 0.068558 ;b = 0.0314515;
      	//alpha = 0.40; gamma = 13.38;
      	alpha = 0.408; gamma = 13.556;
      }
      if(beamEnergy == 200) {
      	//a = 0.0802285 ;b = 0.0178579;
      	//a = 0.0681734 ;b = 0.031085;
      	a = 0.0678221 ;b = 0.0308716;
      	//alpha = 0.40; gamma = 13.635;
      	alpha = 0.422; gamma = 13.941;
      }
      if(beamEnergy == 250) {
      	// a = 0.0802285 ;b = 0.0178579;
      	// a = 0.0804709 ;b = 0.0182122;
      	a = 0.0678221 ;b = 0.0308716;
      	//alpha = 0.40; gamma = 13.756;
      	alpha = 0.432; gamma = 14.196;
      }
      if(beamEnergy == 300) {
      	//a = 0.0844364 ;b = 0.0148193;
      	a = 0.0703497 ;b = 0.0293021;
      	//alpha = 0.40; gamma = 13.7266;
      	alpha = 0.434; gamma = 14.193;
      }
      
      // double tot_ = (rechitEnergySum+rechitEnergySumAHCAL);

      //For 50 GeV
      // alpha = 0.40; gamma = 12.856;
      double tot_ = beamEnergy;
      // int AH_frac = ((a*rechitEnergySumAHCAL)/tot_)*100;
      // int FH_frac = ((b*rechitEnergySum_FH)/tot_)*100;
      // int AH_frac = ((a*rechitEnergySumAHCAL));
      // int FH_frac = ((b*rechitEnergySum_FH));

      //double sigma2_FB = (rechitEnergySumAHCAL+rechitEnergySum_FH);
      // double sigma2_FB = 1.0;

      h_shower_start_reg2->Fill(shower_lambda_);

      // h_rechit_energy_FB_FH_alone_raw->Fill(rechitEnergySum_FH);

      //double MIP_to_GeV_conversion_factor = 11.214;
      // double MIP_to_GeV_conversion_factor = 9.51;
      // double MIP_to_GeV_conversion_factor = 13.7;

      double MIP_to_GeV_conversion_factor = gamma;
      double FH_Gev_Scale = (rechitEnergySum_FH/MIP_to_GeV_conversion_factor);
      //double AH_Gev_Scale = (rechitEnergySumAHCAL/MIP_to_GeV_conversion_factor);
      double AH_Gev_Scale = rechitEnergySumAHCAL;
      //double full_energy = FH_Gev_Scale+(alpha*AH_Gev_Scale);
      double full_energy = FH_Gev_Scale+AH_Gev_Scale;

      

      // h_rechit_energy_FH_FB_weighted->Fill(FH_Gev_Scale);
      // h_rechit_energy_FH_AHCAL_FB_raw->Fill(rechitEnergySum_FH, rechitEnergySumAHCAL);
      // h_rechit_energy_FH_AHCAL_FB_weighted->Fill(FH_Gev_Scale,(alpha*AH_Gev_Scale));

      h_energy_region2_gev->Fill(full_energy);
      h_energy_region2_mips->Fill(rechitEnergySum_FH+alpha*rechitEnergySumAHCAL);
      h_energy_region2_gev_withoutAHCAL->Fill(FH_Gev_Scale);
      h_energy_all_gev->Fill(full_energy);
      h_energy_FH_vs_AHCAL_region2_gev->Fill(FH_Gev_Scale,AH_Gev_Scale);

      h_sim_energy_inGev_region2->Fill(energyLostFH+energyLostBH);
      h_sim_energy_all_gev->Fill(energyLostFH+energyLostBH);



      for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
  	    if(iL < EE_LAYER) {
  	      // Nrechit_EE+=NRechits_EE[iL];
  	      rechitEnergySum_EE+=RechitsEn_EE[iL];
  	      // cogX_[iL] = cogX_[iL]/RechitsEn_EE[iL];
  	      // cogY_[iL] = cogY_[iL]/RechitsEn_EE[iL];
  	    }
  	    else {
  	      Nrechit_FH+=NRechits_FH[iL-EE_LAYER];
  	      rechitEnergySum_FH+=RechitsEn_FH[iL-EE_LAYER];
  	      cogX_[iL] = cogX_[iL]/RechitsEn_FH[iL-EE_LAYER];
  	      cogY_[iL] = cogY_[iL]/RechitsEn_FH[iL-EE_LAYER];
  	      
  	    }

  	  }
	  }
    
    

    //if(isRegion3 && shower_start_index>0) {
    if(isRegion3) {
      // double MIP_to_GeV = 57.6;
      // double MIP_to_GeV = 60.56;
      // double alpha = 0.4;
      // // double beta = 4.4;
      // double beta = 4.9;

      double alpha = -1.0; // EE & FH + AHCAL relative weight
      double beta = -1.0; // FH & AHCAL relative weight
      double gamma = -1.0; // FH & AHCAL relative weight
      
      if(beamEnergy == 20) {
      	//beta = 4.8; alpha = 0.45; gamma = 54.15;
      	beta = 4.207; alpha = 0.402; gamma = 51.223;
      }
      if(beamEnergy == 50) {
      	//beta = 5.3; alpha = 0.40; gamma = 62.92;
      	beta = 4.752; alpha = 0.404; gamma = 59.03;
      }
      if(beamEnergy == 80) {
      	//beta = 5.0; alpha = 0.40; gamma = 63.737;
      	beta = 5.009; alpha = 0.405; gamma = 63.354;
      }
      if(beamEnergy == 100) {
      	//beta = 4.9; alpha = 0.40; gamma = 64.41;
      	beta = 5.174; alpha = 0.398; gamma = 65.967;
      }
      if(beamEnergy == 120) {
      	//beta = 5.0; alpha = 0.40; gamma = 65.812;
      	beta = 5.074; alpha = 0.408; gamma = 65.947;
      }
      if(beamEnergy == 200) {
      	//beta = 4.5; alpha = 0.40; gamma = 63.3;
      	beta = 5.094; alpha = 0.422; gamma = 68.321;
      }
      if(beamEnergy == 250) {
      	//beta = 5.6; alpha = 0.40; gamma = 73.76;
      	beta = 5.305; alpha = 0.432; gamma = 71.396;
      }
      if(beamEnergy == 300) {
      	//beta = 5.5; alpha = 0.40; gamma = 73.567;
      	beta = 5.253; alpha = 0.434; gamma = 71.443;
      }

      //For 50 GeV
      // beta = 5.3; alpha = 0.40; gamma = 62.92;
      double MIP_to_GeV = gamma;
      // double tot_E_MIPs = rechitEnergySum_EE + beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL));
      double EE_FH_MIPs = rechitEnergySum_EE + beta*(rechitEnergySum_FH);
      //double tot_E_gev = tot_E_MIPs/MIP_to_GeV;
      double EE_FH_gev = EE_FH_MIPs/MIP_to_GeV;
      double EE_gev = rechitEnergySum_EE/MIP_to_GeV;
      double FH_gev = (beta*rechitEnergySum_FH)/MIP_to_GeV;
      double AH_gev = rechitEnergySumAHCAL ;
      double tot_E_gev = EE_gev + FH_gev + AH_gev;
      
      // h_rechit_energy_raw_region3->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySumAHCAL);
      h_rechit_energy_raw_EE_alone_region3->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_AH_alone_region3->Fill(beta*rechitEnergySum_FH);
      
      // h_rechit_energy_part_weighted_region3->Fill(rechitEnergySum_EE+rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL));
      //For sim-data comparison
      h_rechit_energy_part_weighted_region3->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

      // h_rechit_energy_full_weighted_region3->Fill(rechitEnergySum_EE+beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL)));
      h_Nrechit_EE_vs_FH_region3->Fill(Nrechit_EE,Nrechit_FH);

      h_rechit_energy_full_weighted_region3->Fill(rechitEnergySum_EE+ beta*rechitEnergySum_FH);

      h_rechit_energy_full_weighted_inGev_region3->Fill(tot_E_gev);
      h_rechit_energy_full_weighted_inGev_region3_withoutAHCAL->Fill(EE_FH_gev);
      // h_rechit_energy_EE_FHAHCAL_EH_mips->Fill(rechitEnergySum_EE,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL))));
      //h_rechit_energy_EE_FHAHCAL_EH_gev->Fill(rechitEnergySum_EE/MIP_to_GeV,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL)))/MIP_to_GeV);
      h_rechit_energy_EE_FHAHCAL_EH_gev->Fill(EE_gev,(FH_gev+AH_gev));

      if(tot_E_gev < 100) {
      	h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG->Fill(EE_gev,(FH_gev+AH_gev));
      	h_shower_start_reg3_DEBUG->Fill(shower_lambda_);
      }

      h_energy_all_gev->Fill(tot_E_gev);
      h_sim_energy_inGev_region3->Fill(energyLostEE+energyLostFH+energyLostBH);
      h_sim_energy_all_gev->Fill(energyLostEE+energyLostFH+energyLostBH);


      h_shower_start_reg3->Fill(shower_lambda_);
      // if(!strcmp(data,"data")) {
      // 	float rel_weight = 0.0;
      // 	//float start = 4.0;
      // 	float start = 3.0;
      // 	float step = 0.1;
      // 	for(int ii = 0; ii < 50; ii++) {
      // 	  rel_weight = start+(step*ii);
      // 	  h_rechit_energy_EH_rel_weightScan[ii]->Fill(rechitEnergySum_EE+rel_weight*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL)));
      // 	}
      // }
      

    }

    if(shower_lambda_ > -1 || true) {
      //h_total_sim_energy->Fill(energyLostEE+energyLostFH+energyLostBH+energyLostBeam+energyLostOutside);
      h_total_sim_energy->Fill(energyLostEE+energyLostFH+energyLostBH+energyLostBeam);
    }
    if(DEBUG) cout<<"DEBUG: End of Event = "<<jentry+1<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;
    //if(DEBUG && jentry > 10) return;
    if(DEBUG) return;

    // if(jentry > 2) break;
    // if(shower_start_index < 0)
    //   cout<<jentry+1<<endl;
    // if(region_1_classified_events > 20) break;
  } // loop over entries

  cout<<"Got Out "<<jentry<<endl;
  Long64_t total_events = (region_1_classified_events+region_2_classified_events+region_3_classified_events+non_classified_events);
  cout<<"Events with zero hits in AHCAL = "<<ahc_zeroHit<<endl;
  cout<<"Events with zero hits in CE-E = "<<ee_zeroHit<<endl;
  cout<<"MIP like events = "<<((float)region_1_classified_events*100.0)/total_events<<"%"<<endl;
  cout<<"shower start in EE = "<<((float)region_3_classified_events*100.0)/total_events<<"%"<<endl;
  cout<<"shower start in FH = "<<((float)region_2_classified_events*100.0)/total_events<<"%"<<endl;
  cout<<"Non-classified events = "<<((float)non_classified_events*100.0)/total_events<<"%"<<endl;
  //cout<<"Sum = "<<(region_1_classified_events+region_2_classified_events+region_3_classified_events+non_classified_events)<<endl;           
  cout<<"Sum = "<<total_events<<endl;

  cout<<"EE effiecieny between  3 <= layer <= 28 witin +/- 1 : witin +/- 2 :: "<<(within1Layer_EE*100.0)/total_hadInt_EE
    <<"%% : "<<(within2Layer_EE*100.0)/total_hadInt_EE<<"%%"<<endl;
  cout<<"Inerface effiecieny between  29 <= layer <= 31 witin +/- 1 : witin +/- 2 :: "
    <<(within1Layer_interface*100.0)/total_hadInt_interface
    <<"%% : "<<(within2Layer_interface*100.0)/total_hadInt_interface<<"%%"<<endl;
  cout<<"FH effiecieny between  32 <= layer <= 40 witin +/- 1 : witin +/- 2 :: "<<(within1Layer_FH*100.0)/total_hadInt_FH
    <<"%% : "<<(within2Layer_FH*100.0)/total_hadInt_FH<<"%%"<<endl;

  cout<<"Overall effiecieny between  3 <= layer <= 40 witin +/- 1 : witin +/- 2 :: "<<(within1Layer*100.0)/total_hadInt
    <<"%% : "<<(within2Layer*100.0)/total_hadInt<<"%%"<<endl;

  

  cout<<"Total true events =  "<<hadronic_int<<endl;
  cout<<"Hadronic interaction not found =  "<<notFound_hadronic_int<<endl;
  cout<<"Hard interaction = "<<hard_hadronic_int<<endl;



  cout<<"***CHECK***"<<endl;
  cout<<"(within2Layer) EE : in : FH : Overall :: "<<within2Layer_EE<<" : "<<within2Layer_interface<< " : "<<within2Layer_FH<< " : "<<within2Layer<<endl;
  cout<<"(Total) EE : in : FH : Overall :: "<<total_hadInt_EE<<" : "<<total_hadInt_interface<< " : "<<total_hadInt_FH<< " : "<<total_hadInt<<endl;
  cout<<"***********"<<endl;


  cout<<"***COG***"<<endl;
  cout<<"Overall effiecieny between  2 < layer < 39 witin +/- 1 : witin +/- 2 :: "<<(within1Layer_cog*100.0)/total_hadInt_cog
      <<"%% : "<<(within2Layer_cog*100.0)/total_hadInt_cog<<"%%"<<endl;
  cout<<"***********"<<endl;
  // cout<<"Got Out "<<jentry<<endl;
  // cout<<"Events with zero hits in AHCAL = "<<ahc_zeroHit<<endl;
  // cout<<"MIP like events = "<<region_1_classified_events<<endl;
  // cout<<"shower start in EE = "<<region_3_classified_events<<endl;
  // cout<<"shower start in FH = "<<region_2_classified_events<<endl;
  // cout<<"Non-classified events = "<<non_classified_events<<endl;
  // cout<<"Sum = "<<(region_1_classified_events+region_2_classified_events+region_3_classified_events+non_classified_events)<<endl;

  if(E_beam < 0) {
    cout<<"E_beam negative!!!"<<endl;
    return;
  }

  


  /////////////////////////////////////////////////////
  //////                 FH Hadrons            ////////
  /////////////////////////////////////////////////////
  /*
  d_weightScan->cd();
  char* histname = new char[100];
  float rel_weight = 0.05;
  const int n = 20;
  float x[n];
  float y[n];

  for(int ii = 0; ii < 20; ii++) {
    rel_weight = 0.05*(ii+1);
    float mean = 0.0;
    float RMS = 0.0;
    float x_min = (mean-1.5*RMS);
    float x_max = (mean+1.0*RMS);
    mean  = h_rechit_energy_FB_rel_weightScan[ii]->GetMean();
    RMS  = h_rechit_energy_FB_rel_weightScan[ii]->GetRMS();
    TF1 *gaus = new TF1("gaus","gaus(0)",(mean - 1.5*RMS),(mean + RMS));
    // TF1 *gaus = new TF1("gaus","gaus(0)",x_min,x_max);
    gaus->SetParameters(1000.0, mean,  RMS);
    h_rechit_energy_FB_rel_weightScan[ii]->Fit(gaus,"R","",x_min,x_max);

    gaus = h_rechit_energy_FB_rel_weightScan[ii]->GetFunction("gaus");
    float res = gaus->GetParameter(2)/gaus->GetParameter(1);
    //cout<<"Fittig Gaus for "<<rel_weight<<" with range = ["<<(mean - RMS)<<","<<(mean + 2*RMS)<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;
    cout<<"Fittig Gaus for "<<rel_weight<<" with range = ["<<x_min<<","<<x_max<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;

    sprintf(histname,"rel weight = %f, resolution = %f",rel_weight, res);
    h_rechit_energy_FB_rel_weightScan[ii]->SetTitle(histname);
    x[ii] = rel_weight;
    y[ii] = res;
    // gaus->Delete();
  }
  
  h_reso_weights = new TGraph(n,x,y);
  h_reso_weights->SetTitle("Resolution vs weights");
  h_reso_weights->GetXaxis()->SetLabelSize(0.06);
  h_reso_weights->GetXaxis()->SetLabelSize(0.06);
  h_reso_weights->SetName("pion_weight_scan");
  h_reso_weights->SetMarkerColor( 4 );
  h_reso_weights->SetMarkerSize( 2 );
  h_reso_weights->SetMarkerStyle( 21 );
  h_reso_weights->Write();
  */

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////



  /////////////////////////////////////////////////////
  //////                 EH Hadrons            ////////
  /////////////////////////////////////////////////////
  /*
  d_weightScan_EH->cd();
  char* histname_EH = new char[100];
  float rel_weight_EH = 0.0;
  float start = 3.0;
  float step = 0.1;
  bool rebin = true;
  const int n_EH = 50;
  float x_EH[n_EH];
  float y_EH[n_EH];

  for(int ii = 0; ii < 50; ii++) {
    cout<<"****N E W   W E I G H T***"<<endl;
    //rel_weight_EH = step+(0.038*ii);
    rel_weight_EH = start+(step*ii);
    float mean = 0.0;
    float RMS = 0.0;
    if(rebin)
      h_rechit_energy_EH_rel_weightScan[ii]->Rebin(2);
    mean  = h_rechit_energy_EH_rel_weightScan[ii]->GetMean();
    RMS  = h_rechit_energy_EH_rel_weightScan[ii]->GetRMS();
    float x_min = (mean-1.5*RMS);
    float x_max = (mean+0.8*RMS);

    cout<<"Weight =  "<<rel_weight_EH<<" with range = ["<<x_min<<","<<x_max<<"]"<<endl;


    //TF1 *gaus = new TF1("gaus","gaus(0)",(mean - 1.5*RMS),(mean + RMS));
    TF1 *gaus = new TF1("gaus","gaus(0)",(mean - 1.5*RMS),(mean + 0.8*RMS));
    // TF1 *gaus = new TF1("gaus","gaus(0)",x_min,x_max);
    gaus->SetParameters(1000.0, mean,  RMS);
    h_rechit_energy_EH_rel_weightScan[ii]->Fit(gaus,"QR","",x_min,x_max);

    gaus = h_rechit_energy_EH_rel_weightScan[ii]->GetFunction("gaus");
    float res = gaus->GetParameter(2)/gaus->GetParameter(1);
    //cout<<"Fittig Gaus for "<<rel_weight<<" with range = ["<<(mean - RMS)<<","<<(mean + 2*RMS)<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;
    cout<<"Fittig Gaus for "<<rel_weight_EH<<" with range = ["<<x_min<<","<<x_max<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;

    sprintf(histname_EH,"rel weight = %f, resolution = %f",rel_weight_EH, res);
    h_rechit_energy_EH_rel_weightScan[ii]->SetTitle(histname_EH);
    x_EH[ii] = rel_weight_EH;
    y_EH[ii] = res;
    // gaus->Delete();
  }
  
  h_reso_weights_EH = new TGraph(n_EH,x_EH,y_EH);
  h_reso_weights_EH->SetTitle("Resolution vs weights");
  h_reso_weights_EH->GetXaxis()->SetLabelSize(0.06);
  h_reso_weights_EH->GetXaxis()->SetLabelSize(0.06);
  h_reso_weights_EH->SetName("pion_weight_scan_EH");
  h_reso_weights_EH->SetMarkerColor( 4 );
  h_reso_weights_EH->SetMarkerSize( 2 );
  h_reso_weights_EH->SetMarkerStyle( 21 );
  h_reso_weights_EH->Write();
  */
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  cout<<endl<<endl;

}

