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

  Long64_t region_1_classified_events = 0;
  Long64_t region_2_classified_events = 0;
  Long64_t region_3_classified_events = 0;
  Long64_t non_classified_events = 0;
  

  int decade = 0;
  int ahc_zeroHit = 0;
  float rechit_cut[28] = {0.02,0.039,0.019,0.02,0.037,0.076,0.10,
			  0.14,0.18,0.24,0.28,0.33,0.48,0.52,
			  0.56,0.6,0.64,0.70,0.74,0.78,0.82,
			  0.86,0.90,0.92,0.94,0.95,0.98,1.0};


  // float noise[28] = {5.352416,5.101376000000001,5.111968000000002,5.553151999999998,5.810864000000001,5.266255999999999,4.994791999999999,5.191439999999998,5.444968,5.691231999999999,5.493031999999999,5.161951999999998,5.293104000000002,5.672935999999997,5.3420159999999965,5.140415999999996,5.506759999999999,5.452791999999999,5.353551999999999,4.9301759999999994,5.447656,5.3445599999999995,4.9980639999999985,5.742087999999999,7.619096000000002,6.003623999999999,6.718855999999998,6.006079999999995};

  
  // float lambda[40] = {0.0807, 0.0857, 0.0807, 0.1714, 0.1664, 0.2571,
  // 		      0.2521, 0.3428, 0.3378, 0.4285, 0.4235, 0.5142,
  // 		      0.5092, 0.5999, 0.5949, 0.6856, 0.6806, 0.7713,
  // 		      0.7663, 0.857, 0.852, 0.9427, 0.9377, 1.0284,
  // 		      1.0234, 1.1141, 1.10912, 1.1998, 1.475, 1.75,
  // 		      2.025, 2.3, 2.575, 2.85, 3.125, 3.4, 3.675, 
  // 		      3.95, 4.225, 4.5};
  
  ////////////////////////////////////////////////////////////////
  ///////////// Lambda From Crude Caluculations //////////////////
  ////////////////////////////////////////////////////////////////

  // float lambda[40] = {0.0807, 0.0857, 0.1664, 0.1714, 0.2521, 0.2571,
  // 		      0.3378, 0.3428, 0.4235, 0.4285, 0.5092, 0.5142,
  // 		      0.5949, 0.5999, 0.6806, 0.6856, 0.7663, 0.7713,
  // 		      0.852, 0.857, 0.9377, 0.9427, 1.0234, 1.0284,
  // 		      1.1091, 1.1141, 1.1948, 1.1998, 1.475, 1.75,
  // 		      2.025, 2.3, 2.575, 2.85, 3.125, 3.4, 3.675,
  // 		      3.95, 4.225, 4.5};

  ////////////////////////////////////////////////////////////////
  /////////////     Lambda From dEdX            //////////////////
  ////////////////////////////////////////////////////////////////

  // float lambda[40] =  {0.038, 0.1, 0.135, 0.197, 0.231, 
  // 		       0.293, 0.328, 0.39, 0.424, 0.486, 
  // 		       0.521, 0.583, 0.617, 0.679, 0.714, 
  // 		       0.776, 0.81, 0.872, 0.907, 0.969,
  // 		       1.003, 1.081, 1.116, 1.193, 1.228, 
  // 		       1.301, 1.335, 1.409, 1.71, 2.011,
  // 		       2.312, 2.614, 2.915, 3.208, 3.319, 
  // 		       3.62, 3.921, 4.226, 4.522, 4.816};

  ////////////////////////////////////////////////////////////////
  /////////////     Lambda From Shilpi            ////////////////
  ////////////////////////////////////////////////////////////////

  // float lambda[40]={0.11197,
  //                   0.145689,
  //                   0.207199,
  //                   0.240918,
  //                   0.302427,
  //                   0.336146,
  //                   0.397655,
  //                   0.431374,
  //                   0.492884,
  //                   0.526603,
  //                   0.588112,
  //                   0.621831,
  //                   0.68334,
  //                   0.717059,
  //                   0.778569,
  //                   0.812288,
  //                   0.873797,
  //                   0.907516,
  //                   0.969025,
  //                   1.00274,
  //                   1.07992,
  //                   1.11364,
  //                   1.19081,
  //                   1.22453,
  //                   1.29668,
  //                   1.3304,
  //                   1.40402,
  //                   1.64898,
  //                   1.99845,
  //                   2.2931,
  //                   2.58776,
  //                   2.88242,
  //                   3.16908,
  //                   3.57123,
  //                   3.86589,
  //                   4.16055,
  //                   4.45838,
  //                   4.74838,
  //                   5.0352,
  //                   5.13603};

  float lambda[79];
  for(int i = 0; i < 79; i++) {
    lambda[i] = layer_positions[i+1].at(2);
    //std::cout<<i+1<<"\t"<<lambda[i]<<endl;
  }
  float weights_[40] = {0.820384,
			1,
			0.548196,
			1.00002,
			0.548196,
			1,
			0.548196,
			1,
			0.548196,
			1.00002,
			0.548196,
			1,
			0.548196,
			1,
			0.548196,
			1.00002,
			0.548196,
			1,
			0.548196,
			1,
			0.548131,
			1.25478,
			0.548211,
			1.25461,
			0.548213,
			1.173,
			0.548213,
			1.1969,
			4.8736,
			4.79052,
			4.79036,
			4.79052,
			4.79052,
			4.66046,
			6.53807,
			4.79052,
			4.79052,
			4.84206,
			4.71476,
			4.66306};


  

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



/*
 ROOT::Math::SMatrix<double,2, 2, ROOT::Math::MatRepStd<double,2> > coeffs_EH;
  ROOT::Math::SVector<double, 2> consts_EH;
  ROOT::Math::SVector<double, 2> values_EH;
  bool isInverted_EH;

  ROOT::Math::SMatrix<double,2, 2, ROOT::Math::MatRepStd<double,2> > coeffs_H;
  ROOT::Math::SVector<double, 2> consts_H;
  ROOT::Math::SVector<double, 2> values_H;
  bool isInverted_H;
  double H_num = 0.0;
  double H_den = 0.0;*/

  ROOT::Math::SMatrix<double,2, 2, ROOT::Math::MatRepStd<double,2> > coeffs_FB;
  ROOT::Math::SVector<double, 2> consts_FB;
  ROOT::Math::SVector<double, 2> values_FB;
  bool isInverted_FB = false;
/*
  coeffs_EH(0, 0) = 0;
  coeffs_EH(0, 1) = 0;
  coeffs_EH(1, 0) = 0;
  coeffs_EH(1, 1) = 0;
  consts_EH(0) = 0;
  consts_EH(1) = 0;

  coeffs_H(0, 0) = 0;
  coeffs_H(0, 1) = 0;
  coeffs_H(1, 0) = 0;
  coeffs_H(1, 1) = 0;
  consts_H(0) = 0;
  consts_H(1) = 0;*/

  coeffs_FB(0, 0) = 0;
  coeffs_FB(0, 1) = 0;
  coeffs_FB(1, 0) = 0;
  coeffs_FB(1, 1) = 0;
  consts_FB(0) = 0;
  consts_FB(1) = 0;

  double E_beam = -1.0;




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


    // if(jentry > 50) return;

    if(NRechits == 0) continue;
    // if(NRechits > 50 || NRechits < 20) continue;
    h_nTracks->Fill(ntracks);
    if(ntracks != 1) continue;
    E_beam = beamEnergy;
    
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
      cogX_[ii] = -1.0;
      cogY_[ii] = -1.0;
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
      //noise cut
      std::pair<float, float> temp_mod_chip((int)rechit_module->at(i),(int)rechit_chip->at(i));
      float noise_chip = getNoise(temp_mod_chip);
      // if(DEBUG) cout<<"Module, layer, chip , noise = "<<rechit_module->at(i)<<", "<<rechit_layer->at(i)<<", "<<rechit_chip->at(i)<<", "<<noise_chip<<endl;
      if(rechit_amplitudeHigh->at(i) < 3*noise_chip)
      	continue;
      
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
	
	
	// // noise cut
	// if(temp_layer < 29 && rechit_amplitudeHigh->at(i) < (3*noise[temp_layer-1])) 
	//   continue;
	// if((temp_layer > 29 && rechit_amplitudeHigh->at(i) < (3*5.5)))
	//   continue;
	
	// if(((rechit_module->at(i) == 146) || (rechit_module->at(i) == 147)) && rechit_amplitudeHigh->at(i) < (3*6.0))
	//   continue;
      }
      // else if(!strcmp(data,"sim")) {
      // 	if(rechit_amplitudeHigh->at(i) < 18) continue;
      // 	if(rechit_module->at(i) == 42) continue;
      // }
      
      Nrechit_layer[temp_layer-1]++;
      RechitEn_layer[temp_layer-1]+=rechit_energy->at(i);

      cogX_[temp_layer-1] += (rechit_x->at(i)*rechit_energy->at(i));
      cogY_[temp_layer-1] += (rechit_y->at(i)*rechit_energy->at(i));
      
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
      int temp_layer = ahc_hitK->at(i);
      RechitsEn_AH[temp_layer -1] += ahc_hitEnergy->at(i);
      rechitEnergySumAHCAL += ahc_hitEnergy->at(i);
      NRechits_AH[temp_layer -1]++;
    }

    
   
    // if(jentry < 5) {
    //   cout<<"N E W   E V E N T:"<<jentry+1<<endl;
    //   for(int ii = 0; ii < (int)Elayer.size(); ii++) {
    // 	cout<<"Layer: "<<ii+1<<" E="<<Elayer.at(ii)<<endl;
    //   }
    // }
    // else
    //   break;

    
    /////////////////// shower finding algo ////////
    bool MIP = true;
    int shower_start_index = -1;
    float shower_lambda_ = -1.0;
    float shower_weight_ = 1.0;
    if(energy_sum_dR2_[0] > 20) {
      // cout<<"Shower started in layer =1"<<endl;
      shower_start_index = 0;
      shower_lambda_ = lambda[0];
      shower_weight_ = lambda[0];
      MIP = false;
    }
    
    else if(energy_sum_dR2_[1] > 20 && energy_sum_dR2_[1] > 2*energy_sum_dR2_[0]) {
      // cout<<"Shower started in layer =2"<<endl;
      shower_start_index = 1;
      shower_lambda_ = lambda[1];
      shower_weight_ = lambda[1]-lambda[0];
      MIP = false;
    }
    
    else {
      
      for(int i = 2; i < 40; i++) {
	//if((energy_sum_dR2_[i] > 20) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-1])) {
	if((energy_sum_dR2_[i] > 20) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-1]) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-2])) {

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
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
      if(iL < EE_LAYER) {
	Nrechit_EE+=NRechits_EE[iL];
	rechitEnergySum_EE+=RechitsEn_EE[iL];
	cogX_[iL] = cogX_[iL]/RechitsEn_EE[iL];
	cogY_[iL] = cogY_[iL]/RechitsEn_EE[iL];
	// if(!MIP)
        //   h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],RechitsEn_EE[iL]);

      }
      else {
	Nrechit_FH+=NRechits_FH[iL-EE_LAYER];
	rechitEnergySum_FH+=RechitsEn_FH[iL-EE_LAYER];
	cogX_[iL] = cogX_[iL]/RechitsEn_FH[iL-EE_LAYER];
	cogY_[iL] = cogY_[iL]/RechitsEn_FH[iL-EE_LAYER];
	// if(!MIP)
        //   h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);

      }

      // cout<<iL+1<<":("<<track_x[iL]<<","<<track_y[iL]<<"):("<<cogX_[iL]<<","<<cogY_[iL]<<")"<<endl; 
    }





    ///////////////////////////////////////////////////////////
    //// Longitudinal shower profile                         //
    ///////////////////////////////////////////////////////////

    // cout<<"*** NEW EVENT="<<jentry+1<<",shower_start="<<shower_start_index<<" ***"<<endl;
    // cout<<"layer:track(x,y):cog(x,y)"<<endl;
    bool DoWeight = true;
    double beta_ = -1.0;
    switch((int)beamEnergy) {
    case 20: beta_ = 4.207; break;
    case 50: beta_ = 4.752; break;
    case 80: beta_ = 5.009; break;
    case 100: beta_ = 5.174; break;
    case 120: beta_ = 5.074; break;
    case 200: beta_ = 5.094; break;
    case 250: beta_ = 5.305; break;
    case 300: beta_ = 5.253; break;
    }
    if(!DoWeight) beta_ = 1.0;

    
    int summed_up_last_layer[5] = {4, 10, 16, 22, 28};
    float relative_weighted_energy = rechitEnergySum_EE + beta_*rechitEnergySum_FH;
    float absolute_energy = rechitEnergySum_EE + rechitEnergySum_FH;
    if(jentry < 1)
      cout<<"Layer \t lambda \t delta_lambda \t weight "<<endl;


    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
      float del_lamb_weight = weights_[iL];
      if(jentry < 1) {
      	cout<<iL+1<<" \t "<<lambda[iL]<<" \t "<<"xx"<<" \t "<<del_lamb_weight<<endl;
      }
      float to_fill = 0.0;
      float frac_ = 0.0;
      float abs_frac_ = 0.0;
      if(iL < EE_LAYER) {
	h_longi_profile_inclusive->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	h_longi_profile_inclusive_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
	frac_ = RechitsEn_EE[iL]/relative_weighted_energy;
	abs_frac_ = RechitsEn_EE[iL]/absolute_energy;
	if(!MIP) {

	  
	  //h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],frac_);
	  if(shower_start_index+1 < 29) {
	    h_longi_profile_region3->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	    h_longi_profile_region3_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
	    switch(iL+1) {
	    case 4:  for(int jj=0;jj<4;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_region3_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_region3_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_region3_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_region3_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 10:  for(int jj=4;jj<10;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_region3_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_region3_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_region3_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_region3_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 16:  for(int jj=10;jj<16;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_region3_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_region3_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_region3_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_region3_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 22:  for(int jj=17;jj<22;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_region3_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_region3_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_region3_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);
		      
	      h_longi_profile_region3_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 28:  for(int jj=23;jj<28;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_region3_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_region3_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_region3_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_region3_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    default:
	      break;
	    }

	    h_longi_profile_region3_fractionalE->Fill(lambda[iL],frac_);
	    h_longi_profile_region3_abs_fractionalE->Fill(lambda[iL],abs_frac_);
	    //if(shower_start_index+1 == 15) h_longi_profile_region3_summed_up_15EE->Fill(lambda[iL],to_fill);

	  }
	  else {
	    h_longi_profile_region2->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	    h_longi_profile_region2_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
	    h_longi_profile_region2_fractionalE->Fill(lambda[iL],frac_);
	    h_longi_profile_region2_abs_fractionalE->Fill(lambda[iL],abs_frac_);
	  }

	}
      }
      else {
	frac_ = (beta_*RechitsEn_FH[iL-EE_LAYER])/relative_weighted_energy;
	abs_frac_ = RechitsEn_FH[iL-EE_LAYER]/absolute_energy;
	//h_longi_profile_inclusive->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	h_longi_profile_inclusive->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	//h_longi_profile_inclusive_noWeight->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
	h_longi_profile_inclusive_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
	if(!MIP) {
	  //h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER],del_lamb_weight);
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],frac_);
	  if(shower_start_index+1 < 29) {
	    h_longi_profile_region3->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	    h_longi_profile_region3_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
	    h_longi_profile_region3_summed_up_EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
	    h_longi_profile_region3_fractionalE->Fill(lambda[iL],frac_);
	    h_longi_profile_region3_abs_fractionalE->Fill(lambda[iL],abs_frac_);
	    if(shower_start_index+1 == 15)
	      h_longi_profile_region3_summed_up_15EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
	    if(shower_start_index+1 == 22) h_longi_profile_region3_summed_up_22EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);

	    h_longi_profile_region3_summed_up_[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);

	  }
	  else {
	    h_longi_profile_region2->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	    h_longi_profile_region2_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
	    h_longi_profile_region2_fractionalE->Fill(lambda[iL],frac_);
	    h_longi_profile_region2_abs_fractionalE->Fill(lambda[iL],abs_frac_);
	    h_longi_profile_region3_summed_up_[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);


	  }
	  
	}
      }
      
      
    }



    ///////////////////////////////////////////////////////////


    // if(jentry > 5) return;
    
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
    // else if(Nrechit_EE < 80 && shower_start_index > 28) { //H hadrons
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
    //   non_classified_events++;
    // }

    // else {  //EH hadron
    //   isRegion1 = false;
    //   isRegion2 = false;
    //   isRegion3 = true;
    // }
    



    ///////////////////////////////////////////////////////////
    //// Event Classification only with shower start     /// //
    ///////////////////////////////////////////////////////////


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



    h_Nrechit_EE->Fill(Nrechit_EE);
    h_Nrechit_FH->Fill(Nrechit_FH);
    h_Nrechit_AH->Fill(ahc_nHits);
    
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
      h_Nrechits_EE_FH_FB->Fill(Nrechit_EE,Nrechit_FH);
      h_rechit_energy_inEE_region2_mips->Fill(rechitEnergySum_EE);
      double a = -1.0;
      double b = -1.0;
      // double a = 0.0611934;
      // double b = 0.0183451;
      double alpha = -1.0;  // FH & AHCAL relative weight
      double beta = -1.0;
      double gamma = -1.0;
      //double MIP_to_GeV_conversion_factor = 12.83;
      
      if(beamEnergy == 20) {
	// a = 0.0611934; b = 0.0183451;
	//a = 0.0630539 ;b = 0.0120932;
	a = 0.0604318 ;b = 0.0307894;
	//alpha = 0.45; gamma = 13.205;
	//alpha = 0.402; gamma = 13.205;
	alpha = 0.402; gamma = 12.826; beta = 4.207;
      }
      if(beamEnergy == 50) {
	// a = 0.0608286 ;b = 0.0136046;
	//a = 0.076075 ;b = 0.0116685;
	a = 0.0679195 ;b = 0.0324864;
	//alpha = 0.40; gamma = 12.856;
	//alpha = 0.404; gamma = 12.856;
	alpha = 0.404; gamma = 12.811; beta = 4.752;
      }
      if(beamEnergy == 80) {
	// a = 0.0622612 ;b = 0.0152219;
	//a = 0.0788491 ;b = 0.013591;
	a = 0.0683456 ;b = 0.0320886;
	//alpha = 0.40; gamma = 13.1375;
	//alpha = 0.405; gamma = 13.1375;
	alpha = 0.405; gamma = 13.225;  beta = 5.009;
      }
      if(beamEnergy == 100) {
	//a = 0.0786438 ;b = 0.0151615;
	a = 0.0677498 ;b = 0.031738;
	//alpha = 0.40; gamma = 13.38;
	//alpha = 0.389; gamma = 13.38;
	alpha = 0.389; gamma = 13.323; beta = 5.174;
      }
      if(beamEnergy == 120) {
	// a = 0.0600868 ;b = 0.018315;
	//a = 0.0794755 ;b = 0.0174122;
	a = 0.068558 ;b = 0.0314515;
	//alpha = 0.40; gamma = 13.38;
	//alpha = 0.408; gamma = 13.38;
	alpha = 0.408; gamma = 13.556; beta = 5.074;
      }
      if(beamEnergy == 200) {
	//a = 0.0802285 ;b = 0.0178579;
	//a = 0.0681734 ;b = 0.031085;
	a = 0.0678221 ;b = 0.0308716;
	//alpha = 0.40; gamma = 13.635;
	//alpha = 0.422; gamma = 13.635;
	alpha = 0.422; gamma = 13.941; beta = 5.094;
      }
      if(beamEnergy == 250) {
	// a = 0.0802285 ;b = 0.0178579;
	// a = 0.0804709 ;b = 0.0182122;
	a = 0.0678221 ;b = 0.0308716;
	//alpha = 0.40; gamma = 13.756;
	//alpha = 0.432; gamma = 13.756;
	alpha = 0.432; gamma = 14.196; beta = 5.305;
      }
      if(beamEnergy == 300) {
	//a = 0.0844364 ;b = 0.0148193;
	a = 0.0703497 ;b = 0.0293021;
	//alpha = 0.40; gamma = 13.7266;
	//alpha = 0.434; gamma = 13.7266;
	alpha = 0.434; gamma = 14.193; beta = 5.253;
      }
      //For 50 GeV
      // alpha = 0.40; gamma = 12.856;
      // double tot_ = (rechitEnergySum+rechitEnergySumAHCAL);
      double tot_ = beamEnergy;
      // int AH_frac = ((a*rechitEnergySumAHCAL)/tot_)*100;
      // int FH_frac = ((b*rechitEnergySum_FH)/tot_)*100;
      int AH_frac = ((a*rechitEnergySumAHCAL));
      int FH_frac = ((b*rechitEnergySum_FH));

      //double sigma2_FB = (rechitEnergySumAHCAL+rechitEnergySum_FH);
      double sigma2_FB = 1.0;
      // coeffs_FB(0,0) += (rechitEnergySum_FH*rechitEnergySum_FH)/sigma2_FB;
      // coeffs_FB(0,1) += (rechitEnergySumAHCAL*rechitEnergySum_FH)/sigma2_FB;
      // coeffs_FB(1,0) += (rechitEnergySum_FH*rechitEnergySumAHCAL)/sigma2_FB;
      // coeffs_FB(1,1) += (rechitEnergySumAHCAL*rechitEnergySumAHCAL)/sigma2_FB;
      // consts_FB(0)   += E_beam*rechitEnergySum_FH/sigma2_FB;
      // consts_FB(1)   += E_beam*rechitEnergySumAHCAL/sigma2_FB;


      //h_shower_start_reg2->Fill(shower_lambda_);
      // h_rechit_energy_FB_raw->Fill(rechitEnergySumAHCAL+rechitEnergySum_FH);
      h_rechit_energy_FB_FH_alone_raw->Fill(rechitEnergySum_FH);

      //double MIP_to_GeV_conversion_factor = 11.214;
      // double MIP_to_GeV_conversion_factor = 9.51;
      // double MIP_to_GeV_conversion_factor = 13.7;

      double MIP_to_GeV_conversion_factor = gamma;
      double FH_Gev_Scale = (rechitEnergySum_FH/MIP_to_GeV_conversion_factor);
      double AH_Gev_Scale = (rechitEnergySumAHCAL/MIP_to_GeV_conversion_factor);
      double full_energy = FH_Gev_Scale+(alpha*AH_Gev_Scale);
      h_rechit_energy_FH_FB_weighted->Fill(FH_Gev_Scale);

      h_rechit_energy_FH_AHCAL_FB_raw->Fill(rechitEnergySum_FH, rechitEnergySumAHCAL);
      h_rechit_energy_FH_AHCAL_FB_weighted->Fill(FH_Gev_Scale,(alpha*AH_Gev_Scale));
      h_energy_region2_gev->Fill(full_energy);
      h_energy_region2_mips->Fill(rechitEnergySum_FH+alpha*rechitEnergySumAHCAL);
      h_energy_all_gev->Fill(full_energy);
      if(shower_start_index+1 >= 29 && shower_start_index+1 <= 30) {
	h_compart5->Fill(full_energy);
      }
      else if (shower_start_index+1 > 30) {
	h_compart6->Fill(full_energy);
      }
      //h_rechit_energy_FB_weighted->Fill((0.0351296*rechitEnergySum_FH)+(0.031534*rechitEnergySumAHCAL));
      // h_rechit_energy_FH_FB->Fill(rechitEnergySum_FH);
      // h_rechit_energy_AHCAL_FB->Fill(rechitEnergySumAHCAL);
      // h_Nrechit_vs_energy_AH->Fill(ahc_nHits, rechitEnergySumAHCAL);
      //h_rechit_energy_FH_AHCAL_FB->Fill(rechitEnergySum_FH,rechitEnergySumAHCAL);
      // h_rechit_energy_EEFH_AHCAL_FB->Fill((rechitEnergySum_FH+rechitEnergySum_EE),rechitEnergySumAHCAL);

      // h_rechit_energy_FH_FB_gev->Fill(a*rechitEnergySum_FH);
      // h_rechit_energy_AHCAL_FB_gev->Fill(b*rechitEnergySumAHCAL);
      // h_rechit_energy_FH_AHCAL_FB_weighted->Fill((a*rechitEnergySum_FH),(b*rechitEnergySumAHCAL));
      // h_rechit_energy_FB_weighted->Fill((a*rechitEnergySum_FH)+(b*rechitEnergySumAHCAL));
      // h_rechit_energy_FB_rel_weighted->Fill(rechitEnergySum_FH+((b/a)*rechitEnergySumAHCAL));

      // cout<<"(total,FH,AH,sumshower) = "<< (int)tot_ <<", "<<FH_frac <<", "<<AH_frac<<", "
      // 	  <<(FH_frac+AH_frac)<<", "<<shower_start_index-27<<endl;

      h_rechit_energy_full_withEE_weighted_region2->Fill(rechitEnergySum_EE+ 4.9*(rechitEnergySum_FH+0.40*rechitEnergySumAHCAL));
      h_rechit_energy_full_withoutEE_weighted_region2->Fill((rechitEnergySum_FH+0.40*rechitEnergySumAHCAL));
      h_rechit_energy_raw->Fill(rechitEnergySum_FH+rechitEnergySumAHCAL);
      h_rechit_energy_weighted->Fill(rechitEnergySum_FH+(0.4*rechitEnergySumAHCAL));

      float rel_weight = 0.02;
      float step_ = rel_weight;
      for(int ii = 0; ii < 50; ii++) {
      	rel_weight = step_*(ii+1);
      	//h_rechit_energy_FB_rel_weightScan[ii]->Fill(FH_Gev_Scale + (rel_weight*AH_Gev_Scale));
	h_rechit_energy_FB_rel_weightScan[ii]->Fill(rechitEnergySum_FH + (rel_weight*rechitEnergySumAHCAL));
      }
      

      /////////////////////////////////////////////////////
      /////  Shower for FH shower start  /////
      ////////////////////////////////////////////////////



      float EE_mip = rechitEnergySum_EE;
      float FH_mip = beta*rechitEnergySum_FH;
      float AH_mip = alpha*beta*rechitEnergySumAHCAL;
      float total_mip = EE_mip+FH_mip+AH_mip;
      float total_abs = rechitEnergySum_EE + rechitEnergySum_FH + rechitEnergySumAHCAL;

      float EE_frac_ = 0.0;
      float FH_frac_ = 0.0;
      float AH_frac_ = 0.0;



      for(int ii = 0; ii < 79; ii++) {
	if(ii < 28) {    // For EE
	  EE_frac_ = RechitsEn_EE[ii]/total_mip;
	  h_longi_profile_region2_withAH->Fill(lambda[ii],RechitsEn_EE[ii]);
	  h_longi_profile_region2_withAH_energy_fraction->Fill(lambda[ii],EE_frac_);
	  EE_frac_ = RechitsEn_EE[ii]/total_abs;
	  h_longi_profile_region2_withAH_abs_energy_fraction->Fill(lambda[ii],EE_frac_);
	    
	}
	else if(ii >= 28 && ii < 40) {    // For FH
	  FH_frac_ = (beta*RechitsEn_FH[ii-28])/total_mip;
	  h_longi_profile_region2_withAH->Fill(lambda[ii],RechitsEn_FH[ii-28]);
	  h_longi_profile_region2_withAH_energy_fraction->Fill(lambda[ii],FH_frac_);
	  FH_frac_ = RechitsEn_FH[ii-28]/total_abs;
	  h_longi_profile_region2_withAH_abs_energy_fraction->Fill(lambda[ii],FH_frac_);

	}
	else {  // For AHCAL
	  AH_frac_ = (alpha*beta*RechitsEn_AH[ii-40])/total_mip;
	  h_longi_profile_region2_withAH->Fill(lambda[ii],RechitsEn_AH[ii-40]);
	  h_longi_profile_region2_withAH_energy_fraction->Fill(lambda[ii],AH_frac_);
	  AH_frac_ = RechitsEn_AH[ii-40]/total_abs;
	  h_longi_profile_region2_withAH_abs_energy_fraction->Fill(lambda[ii],AH_frac_);

	}
      }



    }
    
    

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
	//beta = 4.8; alpha = 0.402; gamma = 54.15;
	//beta = 4.207; alpha = 0.402; gamma = 54.15;
	beta = 4.207; alpha = 0.402; gamma = 51.223;
      }
      if(beamEnergy == 50) {
	//beta = 5.3; alpha = 0.40; gamma = 62.92;
	//beta = 5.3; alpha = 0.404; gamma = 62.92;
	//beta = 4.752; alpha = 0.404; gamma = 62.92;
	beta = 4.752; alpha = 0.404; gamma = 59.03;
      }
      if(beamEnergy == 80) {
	//beta = 5.0; alpha = 0.40; gamma = 63.737;
	//beta = 5.0; alpha = 0.405; gamma = 63.737;
	//beta = 5.009; alpha = 0.405; gamma = 63.737;
	beta = 5.009; alpha = 0.405; gamma = 63.354;
      }
      if(beamEnergy == 100) {
	//beta = 4.9; alpha = 0.40; gamma = 64.41;
	//beta = 4.9; alpha = 0.398; gamma = 64.41;
	//beta = 5.174; alpha = 0.398; gamma = 64.41;
	beta = 5.174; alpha = 0.398; gamma = 65.967;
      }
      if(beamEnergy == 120) {
	//beta = 5.0; alpha = 0.40; gamma = 65.812;
	//beta = 5.0; alpha = 0.408; gamma = 65.812;
	//beta = 5.074; alpha = 0.408; gamma = 65.812;
	beta = 5.074; alpha = 0.408; gamma = 65.947;
      }
      if(beamEnergy == 200) {
	//beta = 4.5; alpha = 0.40; gamma = 63.3;
	//beta = 4.5; alpha = 0.422; gamma = 63.3;
	//beta = 5.094; alpha = 0.422; gamma = 63.3;
	beta = 5.094; alpha = 0.422; gamma = 68.321;
      }
      if(beamEnergy == 250) {
	//beta = 5.6; alpha = 0.40; gamma = 73.76;
	//beta = 5.6; alpha = 0.432; gamma = 73.76;
	//beta = 5.305; alpha = 0.432; gamma = 73.76;
	beta = 5.305; alpha = 0.432; gamma = 71.396;
      }
      if(beamEnergy == 300) {
	//beta = 5.5; alpha = 0.40; gamma = 73.567;
	//beta = 5.5; alpha = 0.434; gamma = 73.567;
	//beta = 5.253; alpha = 0.434; gamma = 73.567;
	beta = 5.253; alpha = 0.434; gamma = 71.443;
      }
      //For 50 GeV
      // beta = 5.3; alpha = 0.40; gamma = 62.92;
      double MIP_to_GeV = gamma;
      double tot_E_MIPs = rechitEnergySum_EE + beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL));
      double EE_FH_MIPs = rechitEnergySum_EE + beta*(rechitEnergySum_FH);
      double tot_E_gev = tot_E_MIPs/MIP_to_GeV;
      double EE_FH_gev = EE_FH_MIPs/MIP_to_GeV;
      //cout<<"Gamma = "<<gamma<<"\t total E GeV= "<<tot_E_gev<<endl;
      h_rechit_energy_raw_region3->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySumAHCAL);
      h_rechit_energy_raw_EE_alone_region3->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_AH_alone_region3->Fill(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL));
      //h_rechit_energy_part_weighted_region3->Fill(rechitEnergySum_EE+rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL));
      //For sim comparison:
      h_rechit_energy_part_weighted_region3->Fill(rechitEnergySum_EE+rechitEnergySum_FH);
      h_rechit_energy_full_weighted_region3->Fill(rechitEnergySum_EE+beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL)));
      //For sim comparison:
      // h_rechit_energy_full_weighted_region3->Fill(rechitEnergySum_EE+beta*(rechitEnergySum_FH));
      h_Nrechit_EE_vs_FH_region3->Fill(Nrechit_EE,Nrechit_FH);

      h_rechit_energy_full_weighted_inGev_region3->Fill(tot_E_gev);
      h_rechit_energy_full_weighted_inGev_region3_withoutAHCAL->Fill(EE_FH_gev);
      h_rechit_energy_EE_FHAHCAL_EH_mips->Fill(rechitEnergySum_EE,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL))));
      h_rechit_energy_EE_FHAHCAL_EH_gev->Fill(rechitEnergySum_EE/MIP_to_GeV,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL)))/MIP_to_GeV);

      h_energy_all_gev->Fill(tot_E_gev);
      //h_shower_start_reg3->Fill(shower_lambda_);
      float rel_weight = 0.0;
      //float start = 4.0;
      float start = 3.0;
      float step = 0.1;
      for(int ii = 0; ii < 50; ii++) {
      	rel_weight = start+(step*ii);
      	h_rechit_energy_EH_rel_weightScan[ii]->Fill(rechitEnergySum_EE+rel_weight*(rechitEnergySum_FH+(alpha*rechitEnergySumAHCAL)));
      }


    }

    //if()

    // cout<<jentry<<"  "<<Nrechit_FH<<" "<<ahc_nHits<<" "<<rechitEnergySum_FH<<"  "<<rechitEnergySumAHCAL<<endl;


    // if(ahc_nHits<40) {
    //   h_Nrechit_vs_energy_AH_debug->Fill(ahc_nHits, rechitEnergySumAHCAL);
    //   if(ahc_nHits == 0) {
    // 	h_energy_AH_debug->Fill(rechitEnergySumAHCAL);
    // 	// cout<<jentry<<"\t"<<rechitEnergySumAHCAL<<"\t"<<ahc_energySum<<endl;
    // 	ahc_zeroHit++;
    //   }
    // }
    
    // if(rechitEnergySumAHCAL < 1.0)
    //   h_Nrechit_AH_debug->Fill(ahc_nHits);


    h_trackX_trackY_EE1->Fill(track_x[0],track_y[0]);
    h_trackX_trackY_FH12->Fill(track_x[39],track_y[39]);


    if(DEBUG) cout<<"DEBUG: End of Event = "<<jentry+1<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;
    if(DEBUG && jentry > 50) return;

    // if(jentry > 5000) break;
  } // loop over entries

  
  cout<<"Got Out "<<jentry<<endl;
  Long64_t total_events = (region_1_classified_events+region_2_classified_events+region_3_classified_events+non_classified_events);
  cout<<"Events with zero hits in AHCAL = "<<ahc_zeroHit<<endl;
  cout<<"MIP like events = "<<((float)region_1_classified_events*100.0)/total_events<<"%"<<endl;
  cout<<"shower start in EE = "<<((float)region_3_classified_events*100.0)/total_events<<"%"<<endl;
  cout<<"shower start in FH = "<<((float)region_2_classified_events*100.0)/total_events<<"%"<<endl;
  cout<<"Non-classified events = "<<((float)non_classified_events*100.0)/total_events<<"%"<<endl;
  //cout<<"Sum = "<<(region_1_classified_events+region_2_classified_events+region_3_classified_events+non_classified_events)<<endl;
  cout<<"Sum = "<<total_events<<endl;

  if(E_beam < 0) {
    cout<<"E_beam negative!!!"<<endl;
    return;
  }


  /////////////////////////////////////////////////////
  //////                 FH Hadrons            ////////
  /////////////////////////////////////////////////////
  
  // d_weightScan->cd();
  // char* histname = new char[100];
  // float rel_weight = 0.02;
  // float step_ = rel_weight;
  // const int n = 50;
  // float x[n];
  // float y[n];

  // for(int ii = 0; ii < n; ii++) {
  //   rel_weight = step_*(ii+1);
  //   float mean = 0.0;
  //   float RMS = 0.0;
  //   float x_min = (mean-1.5*RMS);
  //   float x_max = (mean+1.0*RMS);
  //   mean  = h_rechit_energy_FB_rel_weightScan[ii]->GetMean();
  //   RMS  = h_rechit_energy_FB_rel_weightScan[ii]->GetRMS();
  //   TF1 *gaus = new TF1("gaus","gaus(0)",(mean - 1.5*RMS),(mean + RMS));
  //   // TF1 *gaus = new TF1("gaus","gaus(0)",x_min,x_max);
  //   gaus->SetParameters(1000.0, mean,  RMS);
  //   h_rechit_energy_FB_rel_weightScan[ii]->Fit(gaus,"R","",x_min,x_max);

  //   gaus = h_rechit_energy_FB_rel_weightScan[ii]->GetFunction("gaus");
  //   mean = gaus->GetParameter(1);
  //   RMS = gaus->GetParameter(2);
  //   x_min = (mean-1.5*RMS);
  //   x_max = (mean+1.0*RMS);
  //   h_rechit_energy_FB_rel_weightScan[ii]->Fit(gaus,"R","",x_min,x_max);  

  //   gaus = h_rechit_energy_FB_rel_weightScan[ii]->GetFunction("gaus");
  //   mean = gaus->GetParameter(1);
  //   RMS = gaus->GetParameter(2);
  //   x_min = (mean-1.5*RMS);
  //   x_max = (mean+1.0*RMS);
  //   h_rechit_energy_FB_rel_weightScan[ii]->Fit(gaus,"R","",x_min,x_max);  
    
  //   float res = gaus->GetParameter(2)/gaus->GetParameter(1);
  //   //cout<<"Fittig Gaus for "<<rel_weight<<" with range = ["<<(mean - RMS)<<","<<(mean + 2*RMS)<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;
  //   cout<<"Fittig Gaus for "<<rel_weight<<" with range = ["<<x_min<<","<<x_max<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;

  //   sprintf(histname,"rel weight = %f, resolution = %f",rel_weight, res);
  //   h_rechit_energy_FB_rel_weightScan[ii]->SetTitle(histname);
  //   x[ii] = rel_weight;
  //   y[ii] = res;
  //   // gaus->Delete();
  // }
  
  // h_reso_weights = new TGraph(n,x,y);
  // h_reso_weights->SetTitle("Resolution vs weights");
  // h_reso_weights->GetXaxis()->SetLabelSize(0.06);
  // h_reso_weights->GetXaxis()->SetLabelSize(0.06);
  // h_reso_weights->SetName("pion_weight_scan");
  // h_reso_weights->SetMarkerColor( 4 );
  // h_reso_weights->SetMarkerSize( 2 );
  // h_reso_weights->SetMarkerStyle( 21 );
  // h_reso_weights->Write();

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////



  /////////////////////////////////////////////////////
  //////                 EH Hadrons            ////////
  /////////////////////////////////////////////////////
  
  // d_weightScan_EH->cd();
  // char* histname_EH = new char[100];
  // float rel_weight_EH = 0.0;
  // float start = 3.0;
  // float step = 0.1;
  // bool rebin = true;
  // const int n_EH = 50;
  // float x_EH[n_EH];
  // float y_EH[n_EH];

  // for(int ii = 0; ii < 50; ii++) {
  //   cout<<"****N E W   W E I G H T***"<<endl;
  //   //rel_weight_EH = step+(0.038*ii);
  //   rel_weight_EH = start+(step*ii);
  //   float mean = 0.0;
  //   float RMS = 0.0;
  //   if(rebin)
  //     h_rechit_energy_EH_rel_weightScan[ii]->Rebin(2);
  //   mean  = h_rechit_energy_EH_rel_weightScan[ii]->GetMean();
  //   RMS  = h_rechit_energy_EH_rel_weightScan[ii]->GetRMS();
  //   float x_min = (mean-1.5*RMS);
  //   float x_max = (mean+0.8*RMS);

  //   cout<<"Weight =  "<<rel_weight_EH<<" with range = ["<<x_min<<","<<x_max<<"]"<<endl;


  //   //TF1 *gaus = new TF1("gaus","gaus(0)",(mean - 1.5*RMS),(mean + RMS));
  //   TF1 *gaus = new TF1("gaus","gaus(0)",(mean - 1.5*RMS),(mean + 0.8*RMS));
  //   // TF1 *gaus = new TF1("gaus","gaus(0)",x_min,x_max);
  //   gaus->SetParameters(1000.0, mean,  RMS);
  //   h_rechit_energy_EH_rel_weightScan[ii]->Fit(gaus,"QR","",x_min,x_max);

  //   gaus = h_rechit_energy_EH_rel_weightScan[ii]->GetFunction("gaus");
  //   gaus->SetParameters(1000.0, mean,  RMS);
  //   h_rechit_energy_EH_rel_weightScan[ii]->Fit(gaus,"QR","",x_min,x_max);

  //   gaus = h_rechit_energy_EH_rel_weightScan[ii]->GetFunction("gaus");
  //   gaus->SetParameters(1000.0, mean,  RMS);
  //   h_rechit_energy_EH_rel_weightScan[ii]->Fit(gaus,"QR","",x_min,x_max);

  //   float res = gaus->GetParameter(2)/gaus->GetParameter(1);
  //   //cout<<"Fittig Gaus for "<<rel_weight<<" with range = ["<<(mean - RMS)<<","<<(mean + 2*RMS)<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;
  //   cout<<"Fittig Gaus for "<<rel_weight_EH<<" with range = ["<<x_min<<","<<x_max<<"], Obtained mean = "<<gaus->GetParameter(1)<<" ,resolution = "<<res<<endl;

  //   sprintf(histname_EH,"rel weight = %f, resolution = %f",rel_weight_EH, res);
  //   h_rechit_energy_EH_rel_weightScan[ii]->SetTitle(histname_EH);
  //   x_EH[ii] = rel_weight_EH;
  //   y_EH[ii] = res;
  //   // gaus->Delete();
  // }
  
  // h_reso_weights_EH = new TGraph(n_EH,x_EH,y_EH);
  // h_reso_weights_EH->SetTitle("Resolution vs weights");
  // h_reso_weights_EH->GetXaxis()->SetLabelSize(0.06);
  // h_reso_weights_EH->GetXaxis()->SetLabelSize(0.06);
  // h_reso_weights_EH->SetName("pion_weight_scan_EH");
  // h_reso_weights_EH->SetMarkerColor( 4 );
  // h_reso_weights_EH->SetMarkerSize( 2 );
  // h_reso_weights_EH->SetMarkerStyle( 21 );
  // h_reso_weights_EH->Write();
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  cout<<endl<<endl;

}

