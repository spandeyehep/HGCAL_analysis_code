#define AnalyzeHGCOctTB_cxx
//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


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

  bool DoRunOn100k = true;
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  if(DoRunOn100k) {
    float fraction_events = (float)(100000.0/nentries);
    fraction_events = fraction_events*100;
    nentries = 100000;
    std::cout<<std::endl<<BOLDRED<<"####################################################################"<<RESET<<std::endl;
    std::cout<<BOLDRED<<"IMPROTANT:: Running on 100k events ("<<fraction_events<<"% of original stat)"<<RESET<<std::endl;
    std::cout<<BOLDRED<<"IMPROTANT:: Please turn OFF \'DoRunOn100k\' switch"<<RESET<<std::endl;
    std::cout<<BOLDRED<<"####################################################################"<<RESET<<std::endl<<std::endl;
  }


  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  // Long64_t nbytes3 = 0, nb3 = 0;


  Long64_t region_1_classified_events = 0;
  Long64_t region_2_classified_events = 0;
  Long64_t region_3_classified_events = 0;
  Long64_t non_classified_events = 0;
  bool UseShowerStartAlgo = false;
  bool UseCompartmentEnergy = true;

  if(UseShowerStartAlgo && !UseCompartmentEnergy) {
    std::cout<<"Event Categorization based on :"<<BOLDGREEN<<" Shower start Algorithm"<<RESET<<std::endl;
  }

  else if(!UseShowerStartAlgo && UseCompartmentEnergy) {
    std::cout<<"Event Categorization based on :"<<BOLDGREEN<<" EnergySum in CE-E & CE-H"<<RESET<<std::endl;
  }
  else {
    std::cout<<BOLDRED<<"No Switch selected for Event Categorization"<<RESET<<std::endl;
    return;
  }

  
  int decade = 0;
  int ahc_zeroHit = 0;

  float lambda[79];
  for(int i = 0; i < 79; i++) {
    lambda[i] = layer_positions[i+1].at(2);
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
  // float ahc_front = 169.9;
  float ahc_front = 159.4;     //from Shilpi link: https://twiki.cern.ch/twiki/pub/CMS/HgcalSimulation/config1LengthsVals.txt

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

  ROOT::Math::SMatrix<double,3, 3, ROOT::Math::MatRepStd<double,3> > coeffs_EH;
  ROOT::Math::SVector<double, 3> consts_EH;
  ROOT::Math::SVector<double, 3> values_EH;
  bool isInverted_EH = false;

  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      coeffs_EH(i,j) = 0.0;
      // coeffs_H(i,j) = 0.0;
    }
    consts_EH(i) = 0.0;
    values_EH(i) = 0.0;
    // consts_H(i) = 0.0;
    // values_H(i) = 0.0;

  }

  // ROOT::Math::SMatrix<double,3, 3, ROOT::Math::MatRepStd<double,3> > coeffs_H;
  // ROOT::Math::SVector<double, 3> consts_H;
  // ROOT::Math::SVector<double, 3> values_H;
  // bool isInverted_H = false;

  ROOT::Math::SMatrix<double,2, 2, ROOT::Math::MatRepStd<double,2> > coeffs_H;
  ROOT::Math::SVector<double, 2> consts_H;
  ROOT::Math::SVector<double, 2> values_H;
  bool isInverted_H = false;

  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      coeffs_H(i,j) = 0.0;
    }
    consts_H(i) = 0.0;
    values_H(i) = 0.0;

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
    // nb3 = fChain3->GetEntry(jentry); nbytes3 += nb3;

    
    // ===============Synch HGCAL & AHCAL with same entry == == == == == == == == == == ==

    // while(run != runNumber) {
    //   hgc_jentry++;
    //   nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
    //   nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    // }


    if(NRechits == 0) continue;
    // if(trueBeamEnergy < 190.0) continue;
    h_nTracks->Fill(ntracks);
    if(ntracks != 1) continue;

    bool isGoodTrack = (dwcReferenceType >= 13 && trackChi2_X < 10 && trackChi2_Y < 10);
    if(!isGoodTrack) continue;

    E_beam = beamEnergy;
    
    h_particleID->Fill(pdgID);
    h_runNumber->Fill(run);
    h_beamEnergy->Fill(trueBeamEnergy);


    int PS_beamEnergyTres;
    switch((int)E_beam) {
    case 20: PS_beamEnergyTres = 19; break;
    case 50: PS_beamEnergyTres = 48; break;
    case 80: PS_beamEnergyTres = 76; break;
    case 100: PS_beamEnergyTres = 95; break;
    case 120: PS_beamEnergyTres = 115; break;
    case 200: PS_beamEnergyTres = 190; break;
    case 250: PS_beamEnergyTres = 240; break;
    case 300: PS_beamEnergyTres = 280; break;
    default: std::cout<<BOLDRED<<"Incorrect Beam energy"<<RESET<<std::endl; return; 
    }
    

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


    /// Read Tree

    ////////////////////////////////////////////
    //            HGCAL Part                  //
    ////////////////////////////////////////////


    
    if(DEBUG) cout<<"DEBUG: Start Analylizing RecHits!!"<<endl;
    if(DEBUG) cout<<"DEBUG: NRechits = "<<NRechits<<endl;


    bool DoMIPrescaling = false;
    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;
    std::vector<int> temp_moduleID;

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
      
      // noise cut
      
      float noise_chip = getNoise(temp_mod_chip);
      if(DEBUG) cout<<"Module, layer, chip , noise = "<<rechit_module->at(i)<<", "<<rechit_layer->at(i)<<", "<<rechit_chip->at(i)<<", "<<noise_chip<<endl;
      if(rechit_amplitudeHigh->at(i) < 3*noise_chip) {
	//cout<<jentry+1<<" "<<temp_layer<<" "<<i+1<<" Rejected..."<<endl;
	continue;
	
      }

      
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
      


      
      
      /////////////////// Saving information for shower start algo ////////

      
      double dR = deltaR(recx,recy,trackx,tracky);
      if(dR < 2.0) {
	energy_sum_dR2_[temp_layer-1]+=rechit_energy->at(i);
      }
            
      ///////////////////////////////////////////////
      
      
      if(module_part_ == 0) {
	rechitEnergySum_EE += rechit_energy->at(i);
	Nrechit_EE++;
	RechitsEn_EE[module_layer_-1]+=rechit_energy->at(i);
	NRechits_EE[module_layer_-1]++;
      }
      else if(module_part_ == 1) {
	rechitEnergySum_FH += rechit_energy->at(i);
	Nrechit_FH++;
	RechitsEn_FH[module_layer_-1]+=rechit_energy->at(i);
	NRechits_FH[module_layer_-1]++;
	
      }
      else {
	cout<<"ERROR: Unknown Module Part detected!!!!"<<endl;
	return;
	
      }
      rechitEnergySum+=rechit_energy->at(i);
      
    }

    ////////////////////////////////////
    ////////// End of HGCAL nRechit Loop
    ////////////////////////////////////

   

    ////////////////////////////////////////////
    //            AHCAL Part                  //
    ////////////////////////////////////////////
    
    //Double_t rechitEnergySum_AH = sim_energyAH;
    Double_t rechitEnergySum_AH = 0.0;
    for(int i = 0 ; i < ahc_nHits; i++) {
      int temp_layer = ahc_hitK->at(i);
      RechitsEn_AH[temp_layer -1] += ahc_hitEnergy->at(i);
      rechitEnergySum_AH += ahc_hitEnergy->at(i);
      NRechits_AH[temp_layer -1]++;
    }
    double AH_last3 = (RechitsEn_AH[36]+RechitsEn_AH[37]+RechitsEn_AH[38]);
    if(AH_last3/rechitEnergySum_AH > 1.0) {
      cout<<AH_last3<<"::"<<rechitEnergySum_AH<<endl;
    }


    //////////////////////////////////////////////
    ////////// End of AHCAL nRechit Loop  ////////
    //////////////////////////////////////////////


    
    /////////////////////////////////////////////

    if(DEBUG) cout<<"DEBUG: For shower start"<<endl;
    if(DEBUG && false) {
      for (int ll = 0; ll < 40; ll++) {
	cout<<ll+1<<"\t"<<energy_sum_dR2_[ll]<<endl;
      }
    }

   
    ///////////////////////////////////////////////////////////////
    /// S H O W E R    S T A R T    F I  N D I N G     A L G  O ///
    ///////////////////////////////////////////////////////////////


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
	if((energy_sum_dR2_[i] > 20) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-1]) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-2])) {

	  shower_start_index = i;
	  shower_lambda_ = lambda[i];
	  shower_weight_ = lambda[i]-lambda[i-1];
	  MIP = false;
	  break;
	}
      }
    }

    
    //////////////////////////////////////
    ///    E N D     O F     A L G O   ///
    //////////////////////////////////////

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




    ////////////////////////////////////////////////////////////////
    //                Comapartment wise calculation              //
    ///////////////////////////////////////////////////////////////


    
    // Long_t Nrechit_EE = 0;
    // Long_t Nrechit_FH = 0;
    // Double_t rechitEnergySum_EE = 0.0;
    // Double_t rechitEnergySum_FH = 0.0;
    Double_t test = 0.0;
    bool zero_rh[40];
    double EE_last3 = RechitsEn_EE[25]+RechitsEn_EE[26]+RechitsEn_EE[27];
    double FH_last3 = RechitsEn_FH[9]+RechitsEn_FH[10]+RechitsEn_FH[11];

    // bool isGoodTrack = (dwcReferenceType >= 13 && trackChi2_X < 10 && trackChi2_Y < 10);
    
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
      float del_lamb_weight = weights_[iL];
      if(iL < EE_LAYER) {
	// Nrechit_EE+=NRechits_EE[iL];
	// rechitEnergySum_EE+=RechitsEn_EE[iL];
	cogX_[iL] = cogX_[iL]/RechitsEn_EE[iL];
	cogY_[iL] = cogY_[iL]/RechitsEn_EE[iL];
	h_Rechits_En_inclusive[iL]->Fill(RechitsEn_EE[iL]);
	if(trueBeamEnergy < PS_beamEnergyTres) {
	  h_check_forBL[iL]->Fill(RechitsEn_EE[iL]);
	  h_check_forBL_ratio[iL]->Fill(RechitsEn_EE[iL]/rechitEnergySum_EE);
	}
	else {
	  h_check_forBL_inv[iL]->Fill(RechitsEn_EE[iL]);
	  h_check_forBL_ratio_inv[iL]->Fill(RechitsEn_EE[iL]/rechitEnergySum_EE);	  
	}
	
	
	if(!MIP) {
	  h_Rechits_En_SS[shower_start_index][iL]->Fill(RechitsEn_EE[iL]);
	}
	else {
	  h_Rechits_En_SS[40][iL]->Fill(RechitsEn_EE[iL]);
	  // cout<<"NOTICE  ME SENPAI!!!"<<endl;
	}
      }
      else {
	// Nrechit_FH+=NRechits_FH[iL-EE_LAYER];
	// rechitEnergySum_FH+=RechitsEn_FH[iL-EE_LAYER];
	cogX_[iL] = cogX_[iL]/RechitsEn_FH[iL-EE_LAYER];
	cogY_[iL] = cogY_[iL]/RechitsEn_FH[iL-EE_LAYER];
	h_Rechits_En_inclusive[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
	if(trueBeamEnergy < PS_beamEnergyTres) {
	  h_check_forBL[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
	  h_check_forBL_ratio[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]/rechitEnergySum_FH);
	}
	else {
	  h_check_forBL_inv[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
	  h_check_forBL_ratio_inv[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]/rechitEnergySum_FH);
	}
	
	if(!MIP) {
	  h_Rechits_En_SS[shower_start_index][iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
	}
	else {
	  h_Rechits_En_SS[40][iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
	}
      }
      
      h_trackX_trackY[iL]->Fill(track_x[iL],track_y[iL]);
      h_cogX_cogY[iL]->Fill(cogX_[iL],cogY_[iL]);
      h_cogX_trackX[iL]->Fill(cogX_[iL],track_x[iL]);
      h_cogY_trackY[iL]->Fill(cogY_[iL],track_y[iL]);
      h_dX_dY[iL]->Fill(cogX_[iL]-track_x[iL],cogY_[iL]-track_y[iL]);
      h_dX[iL]->Fill(cogX_[iL]-track_x[iL]);
      h_dY[iL]->Fill(cogY_[iL]-track_y[iL]);


      if(isGoodTrack) {
	h_trackX[iL]->Fill(track_x[iL]);
	h_trackY[iL]->Fill(track_y[iL]);
	h_trackX_trackY_[iL]->Fill(track_x[iL],track_y[iL]);
      }

      //cout<<iL+1<<":("<<track_x[iL]<<","<<track_y[iL]<<"):("<<cogX_[iL]<<","<<cogY_[iL]<<")"<<endl;
    }

   
    
    if(DEBUG) cout<<"DEBUG: NRechits in EE = "<<Nrechit_EE<<"\t Rechit_energy in EE = "<<rechitEnergySum_EE<<endl;
    if(DEBUG) cout<<"DEBUG: NRechits in FH = "<<Nrechit_FH<<"\t Rechit_energy in FH = "<<rechitEnergySum_FH<<endl;


    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    



    ///////////////////////////////////////////////////////////
    ////           Longitudinal shower profile             ////
    ///////////////////////////////////////////////////////////

    // cout<<"*** NEW EVENT="<<jentry+1<<",shower_start="<<shower_start_index<<" ***"<<endl;
    // cout<<"layer:track(x,y):cog(x,y)"<<endl;
    /*
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
    // if(jentry < 1)
    //   cout<<"Layer \t lambda \t delta_lambda \t weight "<<endl;


    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
      float del_lamb_weight = weights_[iL];
      // if(jentry < 1) {
      // 	cout<<iL+1<<" \t "<<lambda[iL]<<" \t "<<"xx"<<" \t "<<del_lamb_weight<<endl;
      // }
      float to_fill = 0.0;
      float frac_ = 0.0;
      if(iL < EE_LAYER) {
	h_longi_profile_inclusive->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	h_longi_profile_inclusive_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
	frac_ = RechitsEn_EE[iL]/relative_weighted_energy;

	if(!MIP) {

	  
	  //h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],frac_);
	  if(shower_start_index+1 < 29) {
	    h_longi_profile_ShowerInEE->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	    h_longi_profile_ShowerInEE_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
	    switch(iL+1) {
	    case 4:  for(int jj=0;jj<4;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 10:  for(int jj=4;jj<10;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 16:  for(int jj=10;jj<16;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 22:  for(int jj=17;jj<22;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);
		      
	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    case 28:  for(int jj=23;jj<28;jj++) {to_fill+=RechitsEn_EE[jj];}
	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
	    default:
	      break;
	    }

	    h_longi_profile_ShowerInEE_fractionalE->Fill(lambda[iL],frac_);
	    //if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);

	  }
	  else {
	    h_longi_profile_MipsInEE->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
	    h_longi_profile_MipsInEE_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
	    h_longi_profile_MipsInEE_fractionalE->Fill(lambda[iL],frac_);
	  }

	}
      }
      else {
	frac_ = (beta_*RechitsEn_FH[iL-EE_LAYER])/relative_weighted_energy;
	//h_longi_profile_inclusive->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	h_longi_profile_inclusive->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	//h_longi_profile_inclusive_noWeight->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
	h_longi_profile_inclusive_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
	if(!MIP) {
	  //h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER],del_lamb_weight);
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],frac_);
	  if(shower_start_index+1 < 29) {
	    h_longi_profile_ShowerInEE->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	    h_longi_profile_ShowerInEE_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
	    h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
	    h_longi_profile_ShowerInEE_fractionalE->Fill(lambda[iL],frac_);
	    if(shower_start_index+1 == 15)
	      h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
	    if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);

	    h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);

	  }
	  else {
	    h_longi_profile_MipsInEE->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
	    h_longi_profile_MipsInEE_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
	    h_longi_profile_MipsInEE_fractionalE->Fill(lambda[iL],frac_);
	    h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);


	  }
	  
	}
      }
      
      
    }
    */
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////



    
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




    ///////////////////////////////////////////////////////////
    //// E V E N T     C A T E G O R I Z A T I O N       /////
    ///////////////////////////////////////////////////////////


    if(UseShowerStartAlgo && !UseCompartmentEnergy)  {
      /////// BASED ON SHOWER START
      if(shower_start_index == -1) {  //MIP LIKE
	isRegion1 = true;
	isRegion2 = false;
	isRegion3 = false;
      }
      else if(shower_start_index > 27) { //H hadrons
	isRegion1 = false;
	isRegion2 = true;
	isRegion3 = false;
      }
      else {  //EH hadron
	isRegion1 = false;
	isRegion2 = false;
	isRegion3 = true;
      }

    }
    else if(!UseShowerStartAlgo && UseCompartmentEnergy) {
      /////// BASED ON ENERGY SUM
      if((rechitEnergySum_EE < 100 && rechitEnergySum_FH < 60) || (AH_last3/rechitEnergySum_AH > 0.03 && rechitEnergySum_AH != 0)) {  //MIP LIKE
	isRegion1 = true;
	isRegion2 = false;
	isRegion3 = false;
      }
      else if(rechitEnergySum_EE < 100 && rechitEnergySum_FH >= 60) { //H hadrons
	isRegion1 = false;
	isRegion2 = true;
	isRegion3 = false;
      }
      else if(rechitEnergySum_EE >= 100) {  //EH hadron
	isRegion1 = false;
	isRegion2 = false;
	isRegion3 = true;

	//if(shower_start_index+1 == 27 || shower_start_index+1 == 28) {
	// if(shower_start_index+1 >= 27) {
	//   isRegion2 = true;
	//   isRegion3 = false;
	// }
	  
      }
      else {
	std::cout<<BOLDRED<<"This event does NOT fall into any category"<<RESET<<std::endl;
	std::cout<<"CE-E : CE-H :: "<<rechitEnergySum_EE<<":"<<rechitEnergySum_FH<<std::endl;
	return;
      }

    }
    else {
      std::cout<<BOLDRED<<"No Switch selected for Event Categorization"<<RESET<<std::endl;
      return;
    
    }

    ///////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////

    




    



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
    // h_rechit_energy_FH_AHCAL_FB->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
    // h_rechit_energy_EE_FH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);


    //// MIPs in EE
    // if(shower_start_index+1 < 0 ||  shower_start_index+1 > 28) { 
    //   h_rechit_energy_raw_EE_MipsInEE_extended->Fill(rechitEnergySum_EE);
    //   h_rechit_energy_raw_FH_MipsInEE_extended->Fill(rechitEnergySum_FH);
    //   h_rechit_energy_raw_FH_MipsInEE_extended_v1->Fill(rechitEnergySum_FH);
    //   h_rechit_energy_raw_AH_MipsInEE_extended->Fill(rechitEnergySum_AH);
    //   h_rechit_energy_raw_AH_MipsInEE_extended_v1->Fill(rechitEnergySum_AH);

    // }

    // if(shower_start_index < 0) { 
    //   h_rechit_energy_raw_EE_MipsInFH_extended->Fill(rechitEnergySum_EE);
    //   h_rechit_energy_raw_FH_MipsInFH_extended->Fill(rechitEnergySum_FH);
    //   h_rechit_energy_raw_AH_MipsInFH_extended->Fill(rechitEnergySum_AH);
    //   h_rechit_energy_raw_AH_MipsInFH_extended_v1->Fill(rechitEnergySum_AH);

    // }
    
    float alpha = -1.0;  // FH & AHCAL relative weight
    float beta = -1.0; // FH & AHCAL relative weight
    float gamma_ee = -1.0;
    float gamma_fh = -1.0;
    float beta_noAH = -1.0; // EE & FH relative weight
    float gamma_noAH = -1.0;


    alpha = getWeights(beamEnergy).at(0);
    beta = getWeights(beamEnergy).at(1);
    gamma_ee = getWeights(beamEnergy).at(2);
    gamma_fh = getWeights(beamEnergy).at(3);
    
    h_rechit_energy_frac_EE_last3_all->Fill(EE_last3/rechitEnergySum_EE);
    h_rechit_energy_frac_FH_last3_all->Fill(FH_last3/rechitEnergySum_FH); h_rechit_energy_frac_AH_last3_all->Fill(AH_last3/rechitEnergySum_AH);




    if(isRegion1) {
      h_rechit_energy_raw_EE_MipsInEEFH->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEEFH->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEEFH->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_EE_MipsInEEFH_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEEFH_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEEFH_extended->Fill(rechitEnergySum_AH);

      h_rechit_energy_raw_all_MipsInEEFH->Fill(rechitEnergySum_EE+rechitEnergySum_FH);
      h_rechit_energy_raw_EE_MipsInFH_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInFH_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInFH_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInFH_extended_v1->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_EE_vs_FH_MipsInEEFH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_MipsInEEFH->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      
      h_shower_start_reg1->Fill(shower_lambda_);
      h_sim_energy_inGev_SS_notFound->Fill(energyLostEE+energyLostFH+energyLostBH);

      h_rechit_energy_frac_EE_last3_MipsInEEFH->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_MipsInEEFH->Fill(FH_last3/rechitEnergySum_FH);
      h_rechit_energy_frac_AH_last3_MipsInEEFH->Fill(AH_last3/rechitEnergySum_AH);



      if(trueBeamEnergy < PS_beamEnergyTres) h_check_forBL_ratio_mip_window_L2->Fill(RechitsEn_EE[1]/rechitEnergySum_EE);
      else h_check_forBL_ratio_inv_mip_window_L2->Fill(RechitsEn_EE[1]/rechitEnergySum_EE);
      region_1_classified_events++; 

    }
    else {
      if(trueBeamEnergy < PS_beamEnergyTres) h_check_forBL_ratio_NO_mip_window_L2->Fill(RechitsEn_EE[1]/rechitEnergySum_EE);
      else h_check_forBL_ratio_inv_NO_mip_window_L2->Fill(RechitsEn_EE[1]/rechitEnergySum_EE);

    }
    ///////CHECK
    if(RechitsEn_EE[1] > 20) h_check_beamEnergy->Fill(trueBeamEnergy);
    if(shower_start_index < 1) h_check_beamEnergy_SS->Fill(trueBeamEnergy);




    
    if(isRegion2) {
      h_shower_start_reg2->Fill(shower_lambda_);
      region_2_classified_events++; 


      h_rechit_energy_frac_EE_last3_MipsInEE->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_MipsInEE->Fill(FH_last3/rechitEnergySum_FH); 
      h_rechit_energy_frac_AH_last3_MipsInEE->Fill(AH_last3/rechitEnergySum_AH);


      h_rechit_energy_inEE_MipsInEE_mips->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_EE_MipsInEE->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_EE_MipsInEE_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEE->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEE->Fill(rechitEnergySum_AH);
      
      h_rechit_energy_raw_all_MipsInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH);
      h_rechit_energy_raw_EE_vs_FH_MipsInEE->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_MipsInEE->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      h_rechit_energy_raw_FH_MipsInEE_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_FH_MipsInEE_extended_v1->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEE_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInEE_extended_v1->Fill(rechitEnergySum_AH);


      // double a = -1.0;
      // double b = -1.0;
      // // double a = 0.0611934;
      // // double b = 0.0183451;
      // double alpha = -1.0;  // FH & AHCAL relative weight
      // double beta = -1.0; // FH & AHCAL relative weight
      // double gamma = -1.0;
      // double gamma_noAH = -1.0;
      // //double MIP_to_GeV_conversion_factor = 12.83;
      // alpha = getWeights(beamEnergy).at(0);
      // beta = getWeights(beamEnergy).at(1);
      // gamma_ee = getWeights(beamEnergy).at(2);
      // gamma_fh = getWeights(beamEnergy).at(3);

      // if(beamEnergy == 20) {
      // 	// a = 0.0611934; b = 0.0183451;
      // 	//a = 0.0630539 ;b = 0.0120932;
      // 	a = 0.0604318 ;b = 0.0307894;
      // 	//alpha = 0.45; gamma = 13.205;
      // 	alpha = 0.351; gamma = 12.826; beta = 5.384;
      // 	gamma_noAH = 11.9;
      // }
      // if(beamEnergy == 50) {
      // 	// a = 0.0608286 ;b = 0.0136046;
      // 	//a = 0.076075 ;b = 0.0116685;
      // 	a = 0.0679195 ;b = 0.0324864;
      // 	//alpha = 0.40; gamma = 12.856;
      // 	alpha = 0.386; gamma = 12.811; beta = 5.111;
      // 	gamma_noAH = 11.06;
      // }
      // if(beamEnergy == 80) {
      // 	// a = 0.0622612 ;b = 0.0152219;
      // 	//a = 0.0788491 ;b = 0.013591;
      // 	a = 0.0683456 ;b = 0.0320886;
      // 	//alpha = 0.40; gamma = 13.1375;
      // 	alpha = 0.387; gamma = 13.225; beta = 4.974;
      // 	gamma_noAH = 11.09;
      // }
      // if(beamEnergy == 100) {
      // 	//a = 0.0786438 ;b = 0.0151615;
      // 	a = 0.0677498 ;b = 0.031738;
      // 	//alpha = 0.40; gamma = 13.38;
      // 	alpha = 0.391; gamma = 13.323; beta = 4.988;
      // 	gamma_noAH = 11.14;
      // }
      // if(beamEnergy == 120) {
      // 	// a = 0.0600868 ;b = 0.018315;
      // 	//a = 0.0794755 ;b = 0.0174122;
      // 	a = 0.068558 ;b = 0.0314515;
      // 	//alpha = 0.40; gamma = 13.38;
      // 	alpha = 0.394; gamma = 13.556; beta = 4.923;
      // 	gamma_noAH = 10.98;
      // }
      // if(beamEnergy == 200) {
      // 	//a = 0.0802285 ;b = 0.0178579;
      // 	//a = 0.0681734 ;b = 0.031085;
      // 	a = 0.0678221 ;b = 0.0308716;
      // 	//alpha = 0.40; gamma = 13.635;
      // 	alpha = 0.405; gamma = 13.941; beta = 4.988;
      // 	gamma_noAH = 11.01;
      // }
      // if(beamEnergy == 250) {
      // 	// a = 0.0802285 ;b = 0.0178579;
      // 	// a = 0.0804709 ;b = 0.0182122;
      // 	a = 0.0678221 ;b = 0.0308716;
      // 	//alpha = 0.40; gamma = 13.756;
      // 	alpha = 0.417; gamma = 14.196; beta = 4.839;
      // 	gamma_noAH = 11.02;
      // }
      // if(beamEnergy == 300) {
      // 	//a = 0.0844364 ;b = 0.0148193;
      // 	a = 0.0703497 ;b = 0.0293021;
      // 	//alpha = 0.40; gamma = 13.7266;
      // 	alpha = 0.42; gamma = 14.193; beta = 4.862;
      // 	gamma_noAH = 10.88;
      // }
      
      // double tot_ = (rechitEnergySum+rechitEnergySum_AH);

      //For 50 GeV
      // alpha = 0.40; gamma = 12.856;
      gamma_noAH = 11.06;
      
      double tot_ = beamEnergy;
      h_shower_start_reg2->Fill(shower_lambda_);

      double MIP_to_GeV_conversion_factor = gamma_fh;
      double FH_Gev_Scale = (rechitEnergySum_FH/MIP_to_GeV_conversion_factor);
      double AH_Gev_Scale = (alpha*rechitEnergySum_AH)/MIP_to_GeV_conversion_factor;
      double full_energy = FH_Gev_Scale+AH_Gev_Scale;
      double wo_ahcal_noAH = (rechitEnergySum_FH/gamma_noAH);
      

      h_energy_MipsInEE_gev->Fill(full_energy);
      h_energy_MipsInEE_mips->Fill(rechitEnergySum_FH+alpha*rechitEnergySum_AH);
      
      h_energy_MipsInEE_gev_withoutAHCAL->Fill(wo_ahcal_noAH);
      h_energy_all_gev_withoutAHCAL->Fill(wo_ahcal_noAH);
      
      h_energy_all_gev->Fill(full_energy);
      h_energy_FH_vs_AHCAL_MipsInEE_gev->Fill(FH_Gev_Scale,AH_Gev_Scale);

      h_sim_energy_inGev_MipsInEE->Fill(energyLostEE+energyLostFH+energyLostBH);
      h_sim_energy_all_gev->Fill(energyLostEE+energyLostFH+energyLostBH);



      
      ///////////////////////////////////////////////////////////
      /////     H hadrons ; Chi2 matrix initialzation     /////
      //////////////////////////////////////////////////////////


      float w1 = getChi2Weights_H(beamEnergy).at(0);
      float w2 = getChi2Weights_H(beamEnergy).at(1);
      float w3 = getChi2Weights_H(beamEnergy).at(2);

      float chi2_energy = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH + w3*rechitEnergySum_AH;
      h_energy_MipsInEE_gev_chi2->Fill(chi2_energy);


      // double sigma2_H = 1.0;
      double sigma2_H = (0.068*0.068) + (1.19*1.19)*full_energy;
      coeffs_H(0,0) += (rechitEnergySum_FH*rechitEnergySum_FH)/sigma2_H;
      coeffs_H(0,1) += (rechitEnergySum_AH*rechitEnergySum_FH)/sigma2_H;
      coeffs_H(1,0) += (rechitEnergySum_FH*rechitEnergySum_AH)/sigma2_H;
      coeffs_H(1,1) += (rechitEnergySum_AH*rechitEnergySum_AH)/sigma2_H;
      consts_H(0)   += E_beam*rechitEnergySum_FH/sigma2_H;
      consts_H(1)   += E_beam*rechitEnergySum_AH/sigma2_H;

      
      // coeffs_H(0,0) += (rechitEnergySum_EE*rechitEnergySum_EE)/sigma2_H;
      // coeffs_H(0,1) += (rechitEnergySum_EE*rechitEnergySum_FH)/sigma2_H;
      // coeffs_H(0,2) += (rechitEnergySum_EE*rechitEnergySum_AH)/sigma2_H;
      // coeffs_H(1,0) += (rechitEnergySum_FH*rechitEnergySum_EE)/sigma2_H;
      // coeffs_H(1,1) += (rechitEnergySum_FH*rechitEnergySum_FH)/sigma2_H;
      // coeffs_H(1,2) += (rechitEnergySum_FH*rechitEnergySum_AH)/sigma2_H;
      // coeffs_H(2,0) += (rechitEnergySum_AH*rechitEnergySum_EE)/sigma2_H;
      // coeffs_H(2,1) += (rechitEnergySum_AH*rechitEnergySum_FH)/sigma2_H;
      // coeffs_H(2,2) += (rechitEnergySum_AH*rechitEnergySum_AH)/sigma2_H;

      // consts_H(0)   += E_beam*rechitEnergySum_EE/sigma2_H;
      // consts_H(1)   += E_beam*rechitEnergySum_FH/sigma2_H;
      // consts_H(2)   += E_beam*rechitEnergySum_AH/sigma2_H;

      //////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////

    }
    
    

    if(isRegion3) {
      h_shower_start_reg3->Fill(shower_lambda_);
      region_3_classified_events++; 

      h_rechit_energy_frac_EE_last3_ShowerInEE->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_ShowerInEE->Fill(FH_last3/rechitEnergySum_FH); 
      h_rechit_energy_frac_AH_last3_ShowerInEE->Fill(AH_last3/rechitEnergySum_AH);



      if(rechitEnergySum_EE < 200) {
	if(shower_start_index < 0) h_SS_forLowerEwindow_ShowerInEE->Fill(shower_start_index);
	else h_SS_forLowerEwindow_ShowerInEE->Fill(shower_start_index+1);
	// if(debug_count < 100) {
	// cout<<"rechitEnergySum_ EE : SS :: "<<rechitEnergySum_EE<<" : "<<shower_start_index+1<<endl;
	// debug_count++; 
	// }
      }


      h_rechit_energy_raw_EE_ShowerInEE->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_ShowerInEE->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_ShowerInEE->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_all_ShowerInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH);
      h_rechit_energy_raw_EE_vs_FH_ShowerInEE->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_ShowerInEE->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      if(shower_start_index > 0)
	h_rechit_energy_raw_all_ShowerInEE_modulo_EE1->Fill(rechitEnergySum_EE+rechitEnergySum_FH);


      
      // double alpha = -1.0; // EE & FH + AHCAL relative weight
      // double beta = -1.0; // FH & AHCAL relative weight
      // double gamma = -1.0; // FH & AHCAL relative weight
      // double beta_noAH = -1.0; // EE & FH relative weight
      // double gamma_noAH = -1.0; // for EE & FH relative weight
      // if(beamEnergy == 20) {
      // 	//beta = 4.8; alpha = 0.45; gamma = 54.15;
      // 	beta = 5.384; alpha = 0.351; gamma = 51.223;
      // 	beta_noAH =  4.332; gamma_noAH = 49.852;
      // }
      // if(beamEnergy == 50) {
      // 	//beta = 5.3; alpha = 0.40; gamma = 62.92;
      // 	beta = 5.111; alpha = 0.386; gamma = 59.03;
      // 	beta_noAH = 4.872 ; gamma_noAH = 56.535;
      // }
      // if(beamEnergy == 80) {
      // 	//beta = 5.0; alpha = 0.40; gamma = 63.737;
      // 	beta = 4.974; alpha = 0.387; gamma = 63.354;
      // 	beta_noAH =  5.461; gamma_noAH = 61.969;
      // }
      // if(beamEnergy == 100) {
      // 	//beta = 4.9; alpha = 0.40; gamma = 64.41;
      // 	beta = 4.988; alpha = 0.391; gamma = 65.967;
      // 	beta_noAH = 5.747; gamma_noAH = 65.148;
      // }
      // if(beamEnergy == 120) {
      // 	//beta = 5.0; alpha = 0.40; gamma = 65.812;
      // 	beta = 4.923; alpha = 0.394; gamma = 65.947;
      // 	beta_noAH =  5.739; gamma_noAH = 65.241;
      // }
      // if(beamEnergy == 200) {
      // 	//beta = 4.5; alpha = 0.40; gamma = 63.3;
      // 	beta = 4.988; alpha = 0.405; gamma = 68.321;
      // 	beta_noAH =  6.34; gamma_noAH = 70.695;
      // }
      // if(beamEnergy == 250) {
      // 	//beta = 5.6; alpha = 0.40; gamma = 73.76;
      // 	beta = 4.839; alpha = 0.417; gamma = 71.396;
      // 	beta_noAH =  7.1; gamma_noAH = 76.04;
      // }
      // if(beamEnergy == 300) {
      // 	//beta = 5.5; alpha = 0.40; gamma = 73.567;
      // 	beta = 4.862; alpha = 0.42; gamma = 71.443;
      // 	beta_noAH =  7.2; gamma_noAH = 76.9;
      // }

      //For 50 GeV
      // beta = 5.3; alpha = 0.40; gamma = 62.92;
      beta_noAH = 4.872 ; gamma_noAH = 56.535;
      
      double MIP_to_GeV = gamma_ee;
    
      double EE_FH_MIPs = rechitEnergySum_EE + beta*(rechitEnergySum_FH);
      double EE_FH_gev = EE_FH_MIPs/MIP_to_GeV;
      double EE_FH_gev_noAH = (rechitEnergySum_EE+ beta_noAH*rechitEnergySum_FH)/gamma_noAH;
      double EE_gev = rechitEnergySum_EE/MIP_to_GeV;
      double FH_gev = (beta*rechitEnergySum_FH)/MIP_to_GeV;
      double AH_gev = (alpha*beta*rechitEnergySum_AH)/MIP_to_GeV ;
      double tot_E_gev = EE_gev + FH_gev + AH_gev;
      
      h_rechit_energy_raw_EE_alone_ShowerInEE->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_AH_alone_ShowerInEE->Fill(beta*rechitEnergySum_FH);
      
      h_rechit_energy_part_weighted_ShowerInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+(alpha*rechitEnergySum_AH));
      //For sim-data comparison
      // h_rechit_energy_part_weighted_ShowerInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

      h_rechit_energy_full_weighted_ShowerInEE->Fill(rechitEnergySum_EE+beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH)));
      h_Nrechit_EE_vs_FH_ShowerInEE->Fill(Nrechit_EE,Nrechit_FH);

      
      h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL->Fill(rechitEnergySum_EE+ beta_noAH*rechitEnergySum_FH);

      h_rechit_energy_full_weighted_inGev_ShowerInEE->Fill(tot_E_gev);
      h_rechit_energy_EE_weighted_inGev_ShowerInEE->Fill(EE_gev);
      h_rechit_energy_FH_weighted_inGev_ShowerInEE->Fill(FH_gev);
      h_rechit_energy_AH_weighted_inGev_ShowerInEE->Fill(AH_gev);
      h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL->Fill(EE_FH_gev_noAH);
      h_energy_all_gev_withoutAHCAL->Fill(EE_FH_gev_noAH);
      // h_rechit_energy_EE_FHAHCAL_EH_mips->Fill(rechitEnergySum_EE,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH))));
      //h_rechit_energy_EE_FHAHCAL_EH_gev->Fill(rechitEnergySum_EE/MIP_to_GeV,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH)))/MIP_to_GeV);
      h_rechit_energy_EE_FHAHCAL_EH_gev->Fill(EE_gev,(FH_gev+AH_gev));

      if(tot_E_gev < 100) {
	h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG->Fill(EE_gev,(FH_gev+AH_gev));
	h_shower_start_reg3_DEBUG->Fill(shower_lambda_);
      }

      h_energy_all_gev->Fill(tot_E_gev);
      h_sim_energy_inGev_ShowerInEE->Fill(energyLostEE+energyLostFH+energyLostBH);
      h_sim_energy_all_gev->Fill(energyLostEE+energyLostFH+energyLostBH);


      h_shower_start_reg3->Fill(shower_lambda_);

      if(shower_start_index < 0)
	h_Nrechits_check_EE_whenMIP->Fill(Nrechit_EE);
      if(shower_start_index+1 >= 26 && shower_start_index+1 <= 28) {
	h_Nrechits_check_EE_SS_last->Fill(Nrechit_EE);
      }
      if(rechitEnergySum_EE < 400) {
	h_Nrechits_check_EE->Fill(Nrechit_EE);
	h_EnRatio_check_EE->Fill((RechitsEn_EE[25]+RechitsEn_EE[26]+RechitsEn_EE[27])/rechitEnergySum_EE);
	if(shower_start_index+1 >= 26 && shower_start_index+1 <= 28) {
	  h_EnRatio_check_EE_SSlast->Fill((RechitsEn_EE[25]+RechitsEn_EE[26]+RechitsEn_EE[27])/rechitEnergySum_EE);
	}
	for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
	  if(iL < EE_LAYER) {
	    h_check[iL]->Fill(RechitsEn_EE[iL]);
	  }
	  else { 
	    h_check[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
	  }
	}
      }
      
      if(rechitEnergySum_FH<400) {
	if(((RechitsEn_EE[25]+RechitsEn_EE[26]+RechitsEn_EE[27])/rechitEnergySum_EE) < 0.7) {
	  h_rechit_energy_raw_EE_ShowerInEE_updated->Fill(rechitEnergySum_EE);
	  h_rechit_energy_full_weighted_ShowerInEE_updated->Fill(rechitEnergySum_EE+beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH)));

	}
      }
      else {
	h_rechit_energy_raw_EE_ShowerInEE_updated->Fill(rechitEnergySum_EE);
	h_rechit_energy_full_weighted_ShowerInEE_updated->Fill(rechitEnergySum_EE+beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH)));
	  
      }

      /////////////////////////////////////////////////////
      /////  For weight Scan shower starting in CE-E  /////
      ////////////////////////////////////////////////////

      // float rel_weight = 0.0;
      // //float start = 4.0;
      // float start = 3.0;
      // float step = 0.1;
      // for(int ii = 0; ii < 50; ii++) {
      // 	rel_weight = start+(step*ii);
      // 	h_rechit_energy_EH_rel_weightScan[ii]->Fill(rechitEnergySum_EE+rel_weight*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH)));
	
      // }

      ////////////////////////////////////////////////////
      ////////////////////////////////////////////////////


      ///////////////////////////////////////////////////////////
      /////     EH hadrons ; Chi2 matrix initialzation     /////
      //////////////////////////////////////////////////////////
      // weights obtained from sigma as resolution parametrization
      // float w1 = 0.0164101;
      // float w2 = 0.0797108;
      // float w3 = 0.028327;

      float w1 = getChi2Weights_EH(beamEnergy).at(0);
      float w2 = getChi2Weights_EH(beamEnergy).at(1);
      float w3 = getChi2Weights_EH(beamEnergy).at(2);

      //weights with sigma = 1
      // float w1 = 0.0158052 ;
      // float w2 = 0.0779923 ;
      // float w3 = -0.000586595;
      
      float chi2_energy = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH + w3*rechitEnergySum_AH;
      h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2->Fill(chi2_energy);
      // double sigma2_EH = 1.0;
      double sigma2_EH = (0.089*0.089) + (1.25*1.25)*tot_E_gev;
      coeffs_EH(0,0) += (rechitEnergySum_EE*rechitEnergySum_EE)/sigma2_EH;
      coeffs_EH(0,1) += (rechitEnergySum_EE*rechitEnergySum_FH)/sigma2_EH;
      coeffs_EH(0,2) += (rechitEnergySum_EE*rechitEnergySum_AH)/sigma2_EH;
      coeffs_EH(1,0) += (rechitEnergySum_FH*rechitEnergySum_EE)/sigma2_EH;
      coeffs_EH(1,1) += (rechitEnergySum_FH*rechitEnergySum_FH)/sigma2_EH;
      coeffs_EH(1,2) += (rechitEnergySum_FH*rechitEnergySum_AH)/sigma2_EH;
      coeffs_EH(2,0) += (rechitEnergySum_AH*rechitEnergySum_EE)/sigma2_EH;
      coeffs_EH(2,1) += (rechitEnergySum_AH*rechitEnergySum_FH)/sigma2_EH;
      coeffs_EH(2,2) += (rechitEnergySum_AH*rechitEnergySum_AH)/sigma2_EH;

      consts_EH(0)   += E_beam*rechitEnergySum_EE/sigma2_EH;
      consts_EH(1)   += E_beam*rechitEnergySum_FH/sigma2_EH;
      consts_EH(2)   += E_beam*rechitEnergySum_AH/sigma2_EH;

      //////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////

      
    }

    
    h_total_sim_energy->Fill(energyLostEE+energyLostFH+energyLostBH+energyLostBeam+energyLostOutside);
    h_total_absorbed_energy->Fill(energyLostEE+energyLostFH+energyLostBH);
    h_total_absorbed_energy_inc_BL->Fill(energyLostEE+energyLostFH+energyLostBH+energyLostBeam);
    

    
    if(DEBUG) cout<<"DEBUG: End of Event = "<<jentry+1<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;
    //if(DEBUG && jentry > 10) return;
    if(DEBUG) return;

    // if(jentry > 10) break;
    // if(shower_start_index < 0)
    //   cout<<jentry+1<<endl;
    // if(region_1_classified_events > 20) break;
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
  //////                 EH Hadrons            ////////
  /////////////////////////////////////////////////////
  
  cout<<"Minimizing for EH hadrons... "<<endl;
  isInverted_EH = coeffs_EH.Invert();
  // for(int i = 0; i < 3; i++) {
  //   for(int j = 0; j < 3; j++) {
  //     cout<<coeffs_EH(i, j)<<"\t";
  //   }
  //   cout<<endl;
  // }
  // cout<<" "<<consts_EH(0)<<"\t"<<consts_EH(1)<<"\t"<<consts_EH(2)<<endl;
  cout<<"isInverted_EH? = "<< isInverted_EH <<endl;

  // cout<<" "<<coeffs_EH(0, 0)<<"\t"<<coeffs_EH(0, 1)<<endl;
  // cout<<" "<<coeffs_EH(1, 0)<<"\t"<<coeffs_EH(1, 1)<<endl;
  // cout<<endl;
  

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  
  /////////////////////////////////////////////////////
  //////                  H Hadrons            ////////
  /////////////////////////////////////////////////////
  

  cout<<"Minimizing for H hadrons... "<<endl;
  isInverted_H = coeffs_H.Invert();
  // for(int i = 0; i < 3; i++) {
  //   for(int j = 0; j < 3; j++) {
  //     cout<<coeffs_H(i, j)<<"\t";
  //   }
  //   cout<<endl;
  // }
  // cout<<" "<<consts_H(0)<<"\t"<<consts_H(1)<<"\t"<<consts_H(2)<<endl;
  cout<<"isInverted_H? = "<< isInverted_H <<endl;

  // cout<<" "<<coeffs_H(0, 0)<<"\t"<<coeffs_H(0, 1)<<endl;
  // cout<<" "<<coeffs_H(1, 0)<<"\t"<<coeffs_H(1, 1)<<endl;
  // cout<<endl;
  

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////



  if(isInverted_EH) {
    values_EH = coeffs_EH*consts_EH;
    cout<<"EH Hadrons => For E = "<<E_beam<<"GeV, w1 = "<<values_EH(0)<<" ;w2 = "<<values_EH(1)<<" ;w3 = "<<values_EH(2)<<";"<<endl;
  }
  else {
    cout<<"Error: Could not invert for EH hadrons..."<<endl;
  }
  if(isInverted_H) {
    values_H = coeffs_H*consts_H;
    //cout<<"H Hadrons => For E = "<<E_beam<<"GeV, w1 = "<<values_H(0)<<" ;w2 = "<<values_H(1)<<" ;w3 = "<<values_H(2)<<";"<<endl;
    cout<<"H Hadrons => For E = "<<E_beam<<"GeV, w1 = 0.0 ;w2 = "<<values_H(0)<<" ;w3 = "<<values_H(1)<<";"<<endl;
  }
  else {
    cout<<"Error: Could not invert for H hadrons..."<<endl;
  }



  cout<<endl<<endl;

}
