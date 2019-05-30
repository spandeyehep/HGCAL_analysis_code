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
  Long64_t nentries3 = fChain3->GetEntriesFast();
  Long64_t hgc_jentry = 0;

  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  Long64_t nbytes3 = 0, nb3 = 0;

  int decade = 0;
  int ahc_zeroHit = 0;
  float rechit_cut[28] = {0.02,0.039,0.019,0.02,0.037,0.076,0.10,
			  0.14,0.18,0.24,0.28,0.33,0.48,0.52,
			  0.56,0.6,0.64,0.70,0.74,0.78,0.82,
			  0.86,0.90,0.92,0.94,0.95,0.98,1.0};


  float noise[28] = {5.352416,5.101376000000001,5.111968000000002,5.553151999999998,5.810864000000001,5.266255999999999,4.994791999999999,5.191439999999998,5.444968,5.691231999999999,5.493031999999999,5.161951999999998,5.293104000000002,5.672935999999997,5.3420159999999965,5.140415999999996,5.506759999999999,5.452791999999999,5.353551999999999,4.9301759999999994,5.447656,5.3445599999999995,4.9980639999999985,5.742087999999999,7.619096000000002,6.003623999999999,6.718855999999998,6.006079999999995};

  
  // float lambda[40] = {0.0807, 0.0857, 0.0807, 0.1714, 0.1664, 0.2571,
  // 		      0.2521, 0.3428, 0.3378, 0.4285, 0.4235, 0.5142,
  // 		      0.5092, 0.5999, 0.5949, 0.6856, 0.6806, 0.7713,
  // 		      0.7663, 0.857, 0.852, 0.9427, 0.9377, 1.0284,
  // 		      1.0234, 1.1141, 1.10912, 1.1998, 1.475, 1.75,
  // 		      2.025, 2.3, 2.575, 2.85, 3.125, 3.4, 3.675, 
  // 		      3.95, 4.225, 4.5};
  

  float lambda[40] = {0.0807, 0.0857, 0.1664, 0.1714, 0.2521, 0.2571,
		      0.3378, 0.3428, 0.4235, 0.4285, 0.5092, 0.5142,
		      0.5949, 0.5999, 0.6806, 0.6856, 0.7663, 0.7713,
		      0.852, 0.857, 0.9377, 0.9427, 1.0234, 1.0284,
		      1.1091, 1.1141, 1.1948, 1.1998, 1.475, 1.75,
		      2.025, 2.3, 2.575, 2.85, 3.125, 3.4, 3.675,
		      3.95, 4.225, 4.5};


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
  Long64_t cut_count[28];
  Long64_t nEvents = 0;
  Long64_t MIP_pions = 0;
  int TOTAL_ACTIVE_LAYER = -1;
  int EE_LAYER = -1;
  int FH_LAYER = -1;
  double E_beam = -1.0;

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
  for (jentry=0; jentry<nentries;jentry++,hgc_jentry++) {
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
    nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
    nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    nb3 = fChain3->GetEntry(jentry); nbytes3 += nb3;

    
    // ===============Synch HGCAL & AHCAL with same entry == == == == == == == == == == ==
    while(run != runNumber) {
      hgc_jentry++;
      nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
      nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    }


    if(NRechits == 0) continue;
    h_nTracks->Fill(ntracks);
    if(ntracks != 1) continue;
    E_beam = beamEnergy;
    
    h_particleID->Fill(pdgID);
    h_beamEnergy->Fill(beamEnergy);
    h_runNumber->Fill(run);

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
      double z = ahc_front + (ahc_pos[j]/10.0) ;
      double x = (z * m_x) + b_x;
      double y = (z * m_y) + b_y;

      track_x[j+40] = x;
      track_y[j+40] = y;
    }


    Double_t rechitEnergySum = 0.0;
    Double_t un_cali = 0.0;
    Long_t Nrechit_layer[40];

    Long_t NRechits_EE[28];
    Long_t NRechits_FH[12];
    Long_t NRechits_AH[39];

    Double_t RechitsEn_EE[28];
    Double_t RechitsEn_FH[12];
    Double_t RechitsEn_AH[39];
    Double_t RechitEn_layer[40];

    int module_part_ = -1;
    int module_layer_ = -1;
    int module_position_ = -1;

    double energy_sum_dR2_[40];
    
    for(int ii=0;ii<40;ii++){
      if(ii<28) {
	NRechits_EE[ii]=0;
	RechitsEn_EE[ii] = 0.0;
      }
      else{
	NRechits_FH[ii-28]=0;
	RechitsEn_FH[ii-28] = 0.0;
      }
      Nrechit_layer[ii]=0;
      energy_sum_dR2_[ii] = 0.0;
      RechitEn_layer[ii] = 0.0;
      if(ii < 39) {
	RechitsEn_AH[ii] = 0.0;
	NRechits_AH[ii] = 0;
      }

    }

    /// FIll Tree
    if(DEBUG) cout<<"DEBUG: Start Analylizing RecHits!!"<<endl;
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
      
      //channel masking
      if(en_chan == 3022 || en_chan == 3028 || en_chan == 44) continue;
      if(temp_layer==1 && temp_chip==0) continue;
      if(temp_layer==1) {
	Nrech_L1[temp_chip]++;
      }
      

      double trackx = track_x[temp_layer-1];
      double tracky = track_y[temp_layer-1];
      
      if(!strcmp(data,"data")) {
	trackx = -1*trackx;
	tracky = -1*tracky;
	std::pair<float, float> dxy_al = dxy_alignment(temp_layer);
	float dx_corr = dxy_al.first;
	float dy_corr = dxy_al.second; 
	recx -= dx_corr;
	recy -= dy_corr;
	
	
	// noise cut
	if(temp_layer < 29 && rechit_amplitudeHigh->at(i) < (3*noise[temp_layer-1])) 
	  continue;
	if((temp_layer > 29 && rechit_amplitudeHigh->at(i) < (3*5.5)))
	  continue;
	
	if(((rechit_module->at(i) == 146) || (rechit_module->at(i) == 147)) && rechit_amplitudeHigh->at(i) < (3*6.0))
	  continue;
      }
      else if(!strcmp(data,"sim")) {
	if(rechit_amplitudeHigh->at(i) < 18) continue;
	if(rechit_module->at(i) == 42) continue;
      }
      
      Nrechit_layer[temp_layer-1]++;
      RechitEn_layer[temp_layer-1]+=rechit_energy->at(i);
      
      temp_moduleID = getModuleLocation(rechit_module->at(i));

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
      if(dR < 2.0) {
	energy_sum_dR2_[temp_layer-1]+=rechit_energy->at(i);
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


    ////////////////////////////////////////////
    //            AHCAL Part                  //
    ////////////////////////////////////////////
    
    Double_t rechitEnergySumAHCAL = 0.0;
    for(int i = 0 ; i < ahc_nHits; i++) {
      int temp_layer = ahc_hitK->at(i) -1;
      RechitsEn_AH[temp_layer -1] += ahc_hitEnergy->at(i);
      rechitEnergySumAHCAL += ahc_hitEnergy->at(i);
      NRechits_AH[temp_layer -1]++;
    }

    
   

    
    /////////////////// shower finding algo ////////
    bool MIP = true;
    int shower_start_index = -1;
    float shower_lambda_ = -1.0;
    if(energy_sum_dR2_[0] > 20) {
      // cout<<"Shower started in layer =1"<<endl;
      shower_start_index = 0;
      shower_lambda_ = lambda[0];
      MIP = false;
    }
    
    else if(energy_sum_dR2_[1] > 20 && energy_sum_dR2_[1] > 2*energy_sum_dR2_[0]) {
      // cout<<"Shower started in layer =2"<<endl;
      shower_start_index = 1;
      shower_lambda_ = lambda[1];
      MIP = false;
    }
    
    else {
      
      for(int i = 2; i < 40; i++) {
	if((energy_sum_dR2_[i] > 20) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-1])) {

	  // if(i+1==27 || i+1==28) {
	  //   cout<<"Shower started in layer ="<<i+1<<" ,lambda_ = "<<lambda[i]<<endl;
	  // }
	  shower_start_index = i;
	  shower_lambda_ = lambda[i];
	  MIP = false;
	  break;
	}
      }
    }
    h_shower_start->Fill(shower_lambda_);

    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;
    Double_t test = 0.0;
    bool zero_rh[40];
    
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
      if(iL < EE_LAYER) {
	Nrechit_EE+=NRechits_EE[iL];
	rechitEnergySum_EE+=RechitsEn_EE[iL];
      }
      else {
	Nrechit_FH+=NRechits_FH[iL-EE_LAYER];
	rechitEnergySum_FH+=RechitsEn_FH[iL-EE_LAYER];
	
      }
    }
    
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

    if(Nrechit_EE < 80 && Nrechit_FH < 60 && shower_start_index == -1) {  //MIP LIKE
      isRegion1 = true;
      isRegion2 = false;
      isRegion3 = false;
      
    }
    else if(Nrechit_EE < 80 && shower_start_index > 28) { //H hadrons
      isRegion1 = false;
      isRegion2 = true;
      isRegion3 = false;
    }
    else {  //EH hadron
      isRegion1 = false;
      isRegion2 = false;
      isRegion3 = true;
    }
    

    if(isRegion1) h_shower_start_reg1->Fill(shower_lambda_);
    if(isRegion2) h_shower_start_reg2->Fill(shower_lambda_);
    if(isRegion3) h_shower_start_reg3->Fill(shower_lambda_);
    

    ///////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////




    if(DEBUG) cout<<"DEBUG: End of Event = "<<jentry+1<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;
    if(DEBUG) return;

    // if(jentry > 5000) break;
  } // loop over entries

  
  cout<<"Got Out "<<jentry<<endl;
  if(E_beam < 0) {
    cout<<"E_beam negative!!!"<<endl;
    return;
  }


  cout<<endl<<endl;
  
}

