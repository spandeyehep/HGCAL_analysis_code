#ifndef AnalyzeHGCOctTB_H
#define AnalyzeHGCOctTB_H



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
#include <fstream>
#include <cmath>
#include <vector>
#include "HGCNtupleVariables.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class AnalyzeHGCOctTB : public HGCNtupleVariables{

 public:
  AnalyzeHGCOctTB(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data", const char *config="alpha", const char* energy = "-1");
  ~AnalyzeHGCOctTB();
  //Bool_t   FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList);
  Bool_t   FillChain(TChain *chain, TChain *chain2, const TString &inputFileList);
  /* Bool_t   FillChain(TChain *chain, const TString &inputFileList); */
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *);
  void     BookHistogram(const char *, const char *, const char* energy);

  void Module_Map_Init(const char *);
  void Alignment_Map_Init();
  void Noise_Map_Init();
  void MIP_ratio_Init();
  void Layer_Position_Init();
  void Weight_Map_Init();
  void Chi2_Weight_Map_Init();


  TFile *oFile;
  TDirectory *d_Nrechit_layer;
  TDirectory *d_rechitEn_layer;
  /* TDirectory *d_track_info; */
  //TDirectory *d_rechit_en_layer; 
  /* TDirectory *d_adcHG; */
  TDirectory *d_tracks;
  TDirectory *d_layers[40];
  TDirectory *d_prof_layers[41];
  TDirectory *d_check_forBeamline;
  TDirectory *d_total_energy_BL;
  TDirectory *d_ratio_energy_BL;
  TH1F* h_check_forBL[40];
  TH1F* h_check_forBL_ratio[40];
  TDirectory *d_check_forBeamline_inv;
  TDirectory *d_total_energy_BL_inv;
  TDirectory *d_ratio_energy_BL_inv;
  TH1F* h_check_forBL_inv[40];
  TH1F* h_check_forBL_ratio_inv[40];

  TH1F* h_check_forBL_ratio_mip_window_L2;
  TH1F* h_check_forBL_ratio_inv_mip_window_L2;
  TH1F* h_check_forBL_ratio_NO_mip_window_L2;
  TH1F* h_check_forBL_ratio_inv_NO_mip_window_L2;

  TH1F* h_check_beamEnergy;
  TH1F* h_check_beamEnergy_SS;

  TDirectory *d_trackInfo;
  TH1F* h_trackX[40];
  TH1F* h_trackY[40];
  TH2F* h_trackX_trackY_[40];

  TH1F *h_Rechits_En_SS[41][40];
  TH1F *h_Rechits_En_inclusive[40];
  
  TDirectory *d_check;
  TH1F *h_check[40];
  TH1F *h_Nrechits_check_EE;
  /* TH1F* Nrechits_check_SS_beyond25_EE; */
  /* TH1F* Nrechits_check_SS_beyond25_EE; */
  TH1F* h_Nrechits_check_EE_whenMIP;
  TH1F* h_Nrechits_check_EE_SS_last;
  TH1F* h_EnRatio_check_EE;
  TH1F* h_EnRatio_check_EE_SSlast;
  TH1F* h_EnRatio_check_EE_SSlast_lowerEWindow;

  TH2F* h_trackX_trackY[40];
  TH2F* h_cogX_cogY[40];
  TH2F* h_cogX_trackX[40];
  TH2F* h_cogY_trackY[40];
  TH2F* h_dX_dY[40];
  TH1F* h_dX[40];
  TH1F* h_dY[40];
  TH2F* h_dX_dY_track_vs_rechit[40];
  

  /* TDirectory *d_EE[28]; */
  // TDirectory *d_FH[12][7];
  TDirectory *d_weightScan;
  TDirectory *d_weightScan_EH;
  TDirectory *d_showerProfile;
  TDirectory *d_showerProfile_SummedEE;
  TH1F* h_rechit_energy_FB_rel_weightScan[50];
  TH1F* h_rechit_energy_EH_rel_weightScan[50];
  TGraph* h_reso_weights;
  TGraph* h_reso_weights_EH;




  TH1F* h_frac_event;
  const char *conf_;  
  /* TH1F *h_ADChg[128]; */
  /* TH1F *h_adcHG_EE[28][128]; */
  /* TH1F *h_adcHG_EE_track[28][128]; */
  /* TH1F *h_adcHG_FH[12][7][128]; */
  /* TH1F *h_adcHG_FH_track[12][7][128]; */
  TH1F *h_NRechits_EE[28];
  TH1F *h_NRechits_EE_L1_chip0;
  TH1F *h_NRechits_EE_L1_chip1;
  TH1F *h_NRechits_EE_L1_chip2;
  TH1F *h_NRechits_EE_L1_chip3;

  /* TH1F *h_NRechits_FH[12][7]; */
  TH1F *h_NRechits_FH[12];


  TH1F *h_Rechits_En_EE[28];
  TH1F *h_Rechits_En_FH[12];

  TProfile *h_longi_profile_MipsInEE_withAH;
  TProfile *h_longi_profile_MipsInEE_withAH_energy_fraction;
  
  TProfile *h_longi_profile_MipsInEE;
  TProfile *h_longi_profile_ShowerInEE;
  TProfile *h_longi_profile_inclusive;

  TProfile *h_longi_profile_MipsInEE_noWeight;
  TProfile *h_longi_profile_ShowerInEE_noWeight;
  TProfile *h_longi_profile_inclusive_noWeight;

  TProfile *h_longi_profile_MipsInEE_fractionalE;
  TProfile *h_longi_profile_ShowerInEE_fractionalE;
  TProfile *h_longi_profile_inclusive_fractionalE;

  
  TProfile *h_longi_profile_raw[40];

  
  TProfile *h_longi_profile_ShowerInEE_summed_up_EE;
  TProfile *h_longi_profile_ShowerInEE_summed_up_15EE;
  TProfile *h_longi_profile_ShowerInEE_summed_up_22EE;
  TProfile *h_longi_profile_ShowerInEE_summed_up_[40];
  

  // TH1F* h_nrechits;
  // TH1F* h_nrechit_1evt[5];	// Tracking hits in a single event
  TH1F* h_configuration;
  TH1F* h_particleID;
  TH1F* h_nTracks;
  TH1F* h_beamEnergy;
  TH1F* h_runNumber;
  TH1F* h_moduleID;
  TH1F* h_nRechits_layer[28];

  TH1F* h_rechit_energy_raw_all;
  TH1F* h_rechit_energy_raw_EE;
  TH1F* h_rechit_energy_raw_FH;

  TH1F* h_rechit_energy_raw_low_all;
  TH1F* h_rechit_energy_raw_low_EE;
  TH1F* h_rechit_energy_raw_low_FH;

  TH1F* h_rechit_energy_frac_EE_last3_all;
  TH1F* h_rechit_energy_frac_FH_last3_all;
  TH1F* h_rechit_energy_frac_AH_last3_all;


  TH1F* h_rechit_energy_frac_EE_last3_MipsInEEFH;
  TH1F* h_rechit_energy_frac_FH_last3_MipsInEEFH;
  TH1F* h_rechit_energy_frac_AH_last3_MipsInEEFH;

  TH1F* h_rechit_energy_frac_EE_last3_MipsInEE;
  TH1F* h_rechit_energy_frac_FH_last3_MipsInEE;
  TH1F* h_rechit_energy_frac_AH_last3_MipsInEE;


  TH1F* h_rechit_energy_frac_EE_last3_ShowerInEE;
  TH1F* h_rechit_energy_frac_FH_last3_ShowerInEE;
  TH1F* h_rechit_energy_frac_AH_last3_ShowerInEE;




  TH1F* h_rechit_energy_raw_all_MipsInEEFH;
  TH1F* h_rechit_energy_raw_all_MipsInEE;
  TH1F* h_rechit_energy_raw_all_ShowerInEE;
  TH1F* h_rechit_energy_raw_all_ShowerInEE_modulo_EE1;



  TH1F* h_rechit_energy_raw_EE_MipsInEEFH;
  TH1F* h_rechit_energy_raw_FH_MipsInEEFH;
  TH1F* h_rechit_energy_raw_AH_MipsInEEFH;
  TH2F* h_rechit_energy_raw_EE_vs_FH_MipsInEEFH;
  TH2F* h_rechit_energy_raw_FH_vs_AH_MipsInEEFH;

  TH2F* h_rechit_energy_raw_EE_vs_FH_MipsInEE;
  TH2F* h_rechit_energy_raw_FH_vs_AH_MipsInEE;
  TH2F* h_rechit_energy_raw_EE_vs_FH_ShowerInEE;
  TH2F* h_rechit_energy_raw_FH_vs_AH_ShowerInEE;


  /* TH1F* h_rechit_energy_raw_EE_MipsInEE_extended; */
  TH1F* h_rechit_energy_raw_FH_MipsInEE_extended;
  TH1F* h_rechit_energy_raw_FH_MipsInEE_extended_v1;
  TH1F* h_rechit_energy_raw_AH_MipsInEE_extended;
  TH1F* h_rechit_energy_raw_AH_MipsInEE_extended_v1;

  TH1F* h_rechit_energy_raw_EE_MipsInFH_extended;
  TH1F* h_rechit_energy_raw_FH_MipsInFH_extended;
  TH1F* h_rechit_energy_raw_AH_MipsInFH_extended;
  TH1F* h_rechit_energy_raw_AH_MipsInFH_extended_v1;


  TH1F* h_rechit_energy_raw_EE_MipsInEEFH_extended;
  TH1F* h_rechit_energy_raw_FH_MipsInEEFH_extended;
  TH1F* h_rechit_energy_raw_AH_MipsInEEFH_extended;

  TH1F* h_rechit_energy_raw_EE_MipsInEE;
  TH1F* h_rechit_energy_raw_EE_MipsInEE_extended;
  TH1F* h_rechit_energy_raw_FH_MipsInEE;
  TH1F* h_rechit_energy_raw_AH_MipsInEE;

  TH1F* h_rechit_energy_raw_EE_ShowerInEE;
  TH1F* h_rechit_energy_raw_EE_ShowerInEE_updated;
  TH1F* h_rechit_energy_raw_FH_ShowerInEE;
  TH1F* h_rechit_energy_raw_AH_ShowerInEE;

  TH1F* h_SS_forLowerEwindow_ShowerInEE;
  
  TH1F* h_rechit_energy_raw_mid_all;
  TH1F* h_rechit_energy_raw_mid_EE;
  TH1F* h_rechit_energy_raw_mid_FH;
  
  TH1F* h_rechit_energy_raw_high_all;
  TH1F* h_rechit_energy_raw_high_EE;
  TH1F* h_rechit_energy_raw_high_FH;



  TH1F* h_rechit_energy_weighted;
  TH1F* h_rechit_energy_FB_FH_alone_raw;
  TH1F* h_rechit_energy_FH_FB_weighted;


  TH1F* h_rechit_energy_raw_ShowerInEE;
  TH1F* h_rechit_energy_raw_EE_alone_ShowerInEE;
  TH1F* h_rechit_energy_raw_FH_AH_alone_ShowerInEE;
  TH1F* h_rechit_energy_part_weighted_ShowerInEE;
  TH1F* h_rechit_energy_full_weighted_ShowerInEE;
  TH1F* h_rechit_energy_full_weighted_ShowerInEE_updated;
  TH1F* h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2;

  TH1F* h_rechit_energy_EE_weighted_inGev_ShowerInEE;
  TH1F* h_rechit_energy_FH_weighted_inGev_ShowerInEE;
  TH1F* h_rechit_energy_AH_weighted_inGev_ShowerInEE;

  TH1F* h_sim_energy_inGev_ShowerInEE;
  TH1F* h_sim_energy_inGev_SS_notFound;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL;

  TH1F* h_energy_MipsInEE_gev;
  TH1F* h_energy_MipsInEE_gev_chi2;


  TH1F* h_energy_MipsInEE_mips;
  TH1F* h_rechit_energy_inEE_MipsInEE_mips;
  TH1F* h_sim_energy_inGev_MipsInEE;
  TH1F* h_energy_MipsInEE_gev_withoutAHCAL;
  TH2F* h_energy_FH_vs_AHCAL_MipsInEE_gev;
  TH1F* h_energy_all_gev;
  TH1F* h_energy_all_gev_withoutAHCAL;
  TH1F* h_sim_energy_all_gev;

  TH1F* h_total_sim_energy;
  TH1F* h_total_absorbed_energy;
  TH1F* h_total_absorbed_energy_inc_BL;

  TH2F* dX_dY_layer_2;

  /* TH1F* h_rechit_energy_weighted_EH; */
  /* TH1F* h_rechit_energy_FB_FH_alone_raw_EH; */
  /* TH1F* h_rechit_energy_FH_FB_weighted_EH; */


  TH2F* h_Nrechit_EE_vs_FH_ShowerInEE; 
  TH2F* h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED;
  TH2F* h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R1;
  TH2F* h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R2;
  TH2F* h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R3;
 

  TH2F* h_Nrechit_EE_vs_FH;
  TH2F* h_Nrechits_EE_FH_FB;

  //TH2F* h_energy_FH_AH_FB;

  TH2F* h_rechit_energy_FH_AHCAL_FB_raw;
  TH2F* h_rechit_energy_FH_AHCAL_FB_weighted;
  TH2F* h_rechit_energy_EE_FHAHCAL_EH_mips;
  TH2F* h_rechit_energy_EE_FHAHCAL_EH_gev;
  TH2F* h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG;

  TH1F* h_Nrechit_EE;
  TH1F* h_Nrechit_FH;
  TH1F* h_Nrechit_AH;

  TH1F* h_shower_start;
  TH1F* h_shower_start_dN_dLambda;
  
  TH1F* h_shower_start_full_collapsed_EE;
  TH1F* h_shower_start_part_collapsed_EE;

  TH1F* h_shower_start_reg1;
  TH1F* h_shower_start_reg2;
  TH1F* h_shower_start_reg3;
  TH1F* h_shower_start_CHECK_FOR_UNCLASSIFIED;
  TH1F* h_shower_start_reg3_DEBUG;
  TH2F* h_rechit_En_NRechits;

  float inEnergy_;
};
#endif

#ifdef AnalyzeHGCOctTB_cxx

void AnalyzeHGCOctTB::BookHistogram(const char *outFileName, const char* conf,  const char* energy) {
  
  char* hname = new char[200];
  char* dir_name = new char[200];
  double y_max = -1.0;
  double x_2D_max = -1.0;
  double xbin = -1.0;
  double y_2D_max = -1.0;
  double mip_x_max = 3000;
  double mip_y_max = 1500;
  double per_layer_xbin =  -1.0;
  double per_layer_xmax =  -1.0;
  double r2_x_max = -1.0;
  double r2_x_bin = -1.0;
  double sim_x_max = -1.0;
  double sim_x_bin = -1.0;

  
  if(!strcmp(energy, "20")) {
    y_max = 100.0;
    x_2D_max = 3000;
    xbin = 300;
    y_2D_max = 1500;
    inEnergy_ = 20;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = inEnergy_*100;
    r2_x_bin = inEnergy_*10;
    sim_x_max = 30.0;
    sim_x_bin = 300.0;
  }
  else if(!strcmp(energy, "50")) {
    y_max = 150.0;
    /* x_2D_max = 5000; */
    /* x_2D_max = 5000; */
    /* xbin = 500; */
    x_2D_max = 10000;
    xbin = 500;
    y_2D_max = 200;
    inEnergy_ = 50;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    sim_x_max = 75.0;
    sim_x_bin = 750.0;


  }
  else if(!strcmp(energy, "80")) {
    y_max = 200.0;
    /* x_2D_max = 8000; */
    /* xbin = 400; */
    x_2D_max = 12000;
    xbin = 300;
    y_2D_max = 3000;
    inEnergy_ = 80;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    sim_x_max = 120.0;
    sim_x_bin = 960.0;


  }
  else if(!strcmp(energy, "100")) {
    y_max = 300.0;
    /* x_2D_max = 10000; */
    /* xbin = 500; */
    x_2D_max = 18000;
    xbin = 360;
    y_2D_max = 3000;
    inEnergy_ = 100;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    sim_x_max = 150.0;
    sim_x_bin = 750.0;

  }
  else if(!strcmp(energy, "120")) {
    y_max = 400.0;
    /* x_2D_max = 12000; */
    /* xbin = 300; */
    x_2D_max = 20000;
    xbin = 400;
    y_2D_max = 3200;
    inEnergy_ = 120;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    sim_x_max = 180.0;
    sim_x_bin = 450.0;

  }
  else if(!strcmp(energy, "200")) {
    y_max = 600.0;
    /* x_2D_max = 18000; */
    /* xbin = 360; */
    x_2D_max = 40000;
    xbin = 800;
    y_2D_max = 4800;
    inEnergy_ = 200;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    sim_x_max = 300.0;
    sim_x_bin = 600.0;


  }
  else if(!strcmp(energy, "250")) {
    y_max = 700.0;
    /* x_2D_max = 25000; */
    /* xbin = 500; */
    x_2D_max = 50000;
    xbin = 1000;
    y_2D_max = 6000;
    inEnergy_ = 250;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    sim_x_max = 300.0;
    sim_x_bin = 500.0;



  }
  else if(!strcmp(energy, "300")) {
    y_max = 800.0;
    x_2D_max = 60000;
    /* x_2D_max = 20000; */
    xbin = 1000;
    y_2D_max = 6000;
    inEnergy_ = 300;
    per_layer_xbin = inEnergy_*2;
    per_layer_xmax = inEnergy_*30;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    sim_x_max = 450.0;
    sim_x_bin = 450.0;



  }
  else if(!strcmp(energy, "420")) {
    y_max = 200.0;
    x_2D_max = 200;
    /* x_2D_max = 20000; */
    xbin = 1000;
    y_2D_max = 6000;
    per_layer_xbin = 400;
    per_layer_xmax = 40;

    inEnergy_ = 420;
    r2_x_max = 0.5*200*100;
    r2_x_bin = 0.5*200*10;
    sim_x_max = 300.0;
    sim_x_bin = 600.0;


  }

  else {
    cout<<"invalid energy!!!"<<endl; 
    return;
  }
  conf_ = conf;
  oFile = new TFile(outFileName, "recreate");
  h_nTracks = new TH1F("h_nTracks","Tracks",12,-1,5);
  h_nTracks->GetXaxis()->SetTitle("Tracks");

  h_configuration = new TH1F("h_configuration","Configuration",60,-10,50);
  h_particleID = new TH1F("h_particleID","particleID",500,0,500);
  h_beamEnergy = new TH1F("h_beamEnergy","TrueBeamEnergy",sim_x_bin,0.0,sim_x_max);
  
  h_runNumber = new TH1F("h_runNumber","RunNumber",2000,0,2000);
  h_moduleID = new TH1F("h_moduleID","moduleID",4,-2,2);
  h_Nrechit_EE = new TH1F("h_Nrechit_EE","NRechits in EE",500,0.0,1000.0);
  h_Nrechit_FH = new TH1F("h_Nrechit_FH","NRechits in FH",500,0.0,1000.0);
  h_Nrechit_AH = new TH1F("h_Nrechit_AH","NRechits in AH",500,0.0,1000.0);


  h_rechit_energy_frac_EE_last3_all = new TH1F("h_rechit_energy_frac_EE_last3_all","h_rechit_energy_frac_EE_last3_all",200.0,0.0,1.0);
  h_rechit_energy_frac_FH_last3_all = new TH1F("h_rechit_energy_frac_FH_last3_all","h_rechit_energy_frac_FH_last3_all",200.0,0.0,1.0);
  h_rechit_energy_frac_AH_last3_all = new TH1F("h_rechit_energy_frac_AH_last3_all","h_rechit_energy_frac_AH_last3_all",200.0,0.0,1.0);

  h_rechit_energy_frac_EE_last3_MipsInEEFH = new TH1F("h_rechit_energy_frac_EE_last3_MipsInEEFH","h_rechit_energy_frac_EE_last3_MipsInEEFH",200.0,0.0,1.0);
  h_rechit_energy_frac_FH_last3_MipsInEEFH = new TH1F("h_rechit_energy_frac_FH_last3_MipsInEEFH","h_rechit_energy_frac_FH_last3_MipsInEEFH",200.0,0.0,1.0);
  h_rechit_energy_frac_AH_last3_MipsInEEFH = new TH1F("h_rechit_energy_frac_AH_last3_MipsInEEFH","h_rechit_energy_frac_AH_last3_MipsInEEFH",200.0,0.0,1.0);

  h_rechit_energy_frac_EE_last3_MipsInEE = new TH1F("h_rechit_energy_frac_EE_last3_MipsInEE","h_rechit_energy_frac_EE_last3_MipsInEE",200.0,0.0,1.0);
  h_rechit_energy_frac_FH_last3_MipsInEE = new TH1F("h_rechit_energy_frac_FH_last3_MipsInEE","h_rechit_energy_frac_FH_last3_MipsInEE",200.0,0.0,1.0);
  h_rechit_energy_frac_AH_last3_MipsInEE = new TH1F("h_rechit_energy_frac_AH_last3_MipsInEE","h_rechit_energy_frac_AH_last3_MipsInEE",200.0,0.0,1.0);

  h_rechit_energy_frac_EE_last3_ShowerInEE = new TH1F("h_rechit_energy_frac_EE_last3_ShowerInEE","h_rechit_energy_frac_EE_last3_ShowerInEE",200.0,0.0,1.0);
  h_rechit_energy_frac_FH_last3_ShowerInEE = new TH1F("h_rechit_energy_frac_FH_last3_ShowerInEE","h_rechit_energy_frac_FH_last3_ShowerInEE",200.0,0.0,1.0);
  h_rechit_energy_frac_AH_last3_ShowerInEE = new TH1F("h_rechit_energy_frac_AH_last3_ShowerInEE","h_rechit_energy_frac_AH_last3_ShowerInEE",200.0,0.0,1.0);




  /* h_rechit_energy_raw_EE_MipsInEE_extended = new TH1F("h_rechit_energy_raw_EE_MipsInEE_extended","h_rechit_energy_raw_EE_MipsInEE_extended",500.0,0,1000.0); */
  h_rechit_energy_raw_FH_MipsInEE_extended = new TH1F("h_rechit_energy_raw_FH_MipsInEE_extended","h_rechit_energy_raw_FH_MipsInEE_extended",500.0,0,1000.0);
  h_rechit_energy_raw_FH_MipsInEE_extended_v1 = new TH1F("h_rechit_energy_raw_FH_MipsInEE_extended_v1","h_rechit_energy_raw_FH_MipsInEE_extended_v1",r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_AH_MipsInEE_extended = new TH1F("h_rechit_energy_raw_AH_MipsInEE_extended","h_rechit_energy_raw_AH_MipsInEE_extended",500.0,0,1000.0);
  h_rechit_energy_raw_AH_MipsInEE_extended_v1 = new TH1F("h_rechit_energy_raw_AH_MipsInEE_extended_v1","h_rechit_energy_raw_AH_MipsInEE_extended_v1",r2_x_bin,0.0,r2_x_max);


  h_rechit_energy_raw_EE_MipsInFH_extended = new TH1F("h_rechit_energy_raw_EE_MipsInFH_extended","h_rechit_energy_raw_EE_MipsInFH_extended",500.0,0,1000.0);
  h_rechit_energy_raw_FH_MipsInFH_extended = new TH1F("h_rechit_energy_raw_FH_MipsInFH_extended","h_rechit_energy_raw_FH_MipsInFH_extended",500.0,0,1000.0);
  h_rechit_energy_raw_AH_MipsInFH_extended = new TH1F("h_rechit_energy_raw_AH_MipsInFH_extended","h_rechit_energy_raw_AH_MipsInFH_extended",500.0,0,1000.0);
  h_rechit_energy_raw_AH_MipsInFH_extended_v1 = new TH1F("h_rechit_energy_raw_AH_MipsInFH_extended_v1","h_rechit_energy_raw_AH_MipsInFH_extended_v1",r2_x_bin,0.0,r2_x_max);



  h_rechit_energy_raw_EE_MipsInEEFH_extended = new TH1F("h_rechit_energy_raw_EE_MipsInEEFH_extended","h_rechit_energy_raw_EE_MipsInEEFH_extended",500.0,0,1000.0);
  h_rechit_energy_raw_EE_MipsInEEFH_extended->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_FH_MipsInEEFH_extended = new TH1F("h_rechit_energy_raw_FH_MipsInEEFH_extended","h_rechit_energy_raw_FH_MipsInEEFH_extended",500.0,0,1000.0);
  h_rechit_energy_raw_FH_MipsInEEFH_extended->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_AH_MipsInEEFH_extended = new TH1F("h_rechit_energy_raw_AH_MipsInEEFH_extended","h_rechit_energy_raw_AH_MipsInEEFH_extended",500.0,0,1000.0);
  h_rechit_energy_raw_AH_MipsInEEFH_extended->GetXaxis()->SetTitle("Energy in units of MIPs");





  h_rechit_energy_raw_EE_MipsInEEFH = new TH1F("h_rechit_energy_raw_EE_MipsInEEFH","h_rechit_energy_raw_EE_MipsInEEFH",250.0,0,500.0);
  h_rechit_energy_raw_EE_MipsInEEFH->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_FH_MipsInEEFH = new TH1F("h_rechit_energy_raw_FH_MipsInEEFH","h_rechit_energy_raw_FH_MipsInEEFH",250.0,0,500.0);
  h_rechit_energy_raw_FH_MipsInEEFH->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_AH_MipsInEEFH = new TH1F("h_rechit_energy_raw_AH_MipsInEEFH","h_rechit_energy_raw_AH_MipsInEEFH",250.0,0,500.0);
  h_rechit_energy_raw_AH_MipsInEEFH->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_EE_vs_FH_MipsInEEFH = new TH2F("h_rechit_energy_raw_EE_vs_FH_MipsInEEFH","h_rechit_energy_raw_EE_vs_FH_MipsInEEFH",250.0,0,500.0,250.0,0,500.0);
  h_rechit_energy_raw_EE_vs_FH_MipsInEEFH->GetXaxis()->SetTitle("Energy in EE (mips)");
  h_rechit_energy_raw_EE_vs_FH_MipsInEEFH->GetYaxis()->SetTitle("Energy in FH (mips)");
  h_rechit_energy_raw_EE_vs_FH_MipsInEEFH->GetYaxis()->SetTitleOffset(1.0);

  h_rechit_energy_raw_FH_vs_AH_MipsInEEFH = new TH2F("h_rechit_energy_raw_FH_vs_AH_MipsInEEFH","h_rechit_energy_raw_FH_vs_AH_MipsInEEFH",250.0,0,500.0,250.0,0,500.0);
  h_rechit_energy_raw_FH_vs_AH_MipsInEEFH->GetXaxis()->SetTitle("Energy in FH (mips)");
  h_rechit_energy_raw_FH_vs_AH_MipsInEEFH->GetYaxis()->SetTitle("Energy in AH (mips)");
  h_rechit_energy_raw_FH_vs_AH_MipsInEEFH->GetYaxis()->SetTitleOffset(1.0);


  h_rechit_energy_raw_EE_MipsInEE = new TH1F("h_rechit_energy_raw_EE_MipsInEE","h_rechit_energy_raw_EE_MipsInEE",250.0,0,500.0);
  h_rechit_energy_raw_EE_MipsInEE_extended = new TH1F("h_rechit_energy_raw_EE_MipsInEE_extended","h_rechit_energy_raw_EE_MipsInEE_extended",500.0,0,1000.0);

  h_rechit_energy_raw_EE_MipsInEE->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_FH_MipsInEE = new TH1F("h_rechit_energy_raw_FH_MipsInEE","h_rechit_energy_raw_FH_MipsInEE",r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_FH_MipsInEE->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_AH_MipsInEE = new TH1F("h_rechit_energy_raw_AH_MipsInEE","h_rechit_energy_raw_AH_MipsInEE",r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_AH_MipsInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_EE_vs_FH_MipsInEE = new TH2F("h_rechit_energy_raw_EE_vs_FH_MipsInEE","h_rechit_energy_raw_EE_vs_FH_MipsInEE",250.0,0,500.0,r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_EE_vs_FH_MipsInEE->GetXaxis()->SetTitle("Energy in EE units of MIPs");
  h_rechit_energy_raw_EE_vs_FH_MipsInEE->GetYaxis()->SetTitle("Energy in FH units of MIPs");
  h_rechit_energy_raw_EE_vs_FH_MipsInEE->GetYaxis()->SetTitleOffset(1.0);

  h_rechit_energy_raw_FH_vs_AH_MipsInEE = new TH2F("h_rechit_energy_raw_FH_vs_AH_MipsInEE","h_rechit_energy_raw_FH_vs_AH_MipsInEE",r2_x_bin,0.0,r2_x_max,r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_FH_vs_AH_MipsInEE->GetXaxis()->SetTitle("Energy in FH units of MIPs");
  h_rechit_energy_raw_FH_vs_AH_MipsInEE->GetYaxis()->SetTitle("Energy in AH units of MIPs");
  h_rechit_energy_raw_FH_vs_AH_MipsInEE->GetYaxis()->SetTitleOffset(1.0);



  h_rechit_energy_raw_EE_ShowerInEE = new TH1F("h_rechit_energy_raw_EE_ShowerInEE","h_rechit_energy_raw_EE_ShowerInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_EE_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_EE_ShowerInEE_updated = new TH1F("h_rechit_energy_raw_EE_ShowerInEE_updated","h_rechit_energy_raw_EE_ShowerInEE_updated",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_EE_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_FH_ShowerInEE = new TH1F("h_rechit_energy_raw_FH_ShowerInEE","h_rechit_energy_raw_FH_ShowerInEE",r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_FH_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");
  h_rechit_energy_raw_AH_ShowerInEE = new TH1F("h_rechit_energy_raw_AH_ShowerInEE","h_rechit_energy_raw_AH_ShowerInEE",r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_AH_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_EE_vs_FH_ShowerInEE = new TH2F("h_rechit_energy_raw_EE_vs_FH_ShowerInEE","h_rechit_energy_raw_EE_vs_FH_ShowerInEE",xbin,0.0,x_2D_max,r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_EE_vs_FH_ShowerInEE->GetXaxis()->SetTitle("Energy in EE units of MIPs");
  h_rechit_energy_raw_EE_vs_FH_ShowerInEE->GetYaxis()->SetTitle("Energy in FH units of MIPs");
  h_rechit_energy_raw_EE_vs_FH_ShowerInEE->GetYaxis()->SetTitleOffset(1.0);
  h_rechit_energy_raw_FH_vs_AH_ShowerInEE = new TH2F("h_rechit_energy_raw_FH_vs_AH_ShowerInEE","h_rechit_energy_raw_FH_vs_AH_ShowerInEE",xbin,0.0,x_2D_max,r2_x_bin,0.0,r2_x_max);
  h_rechit_energy_raw_FH_vs_AH_ShowerInEE->GetXaxis()->SetTitle("Energy in FH units of MIPs");
  h_rechit_energy_raw_FH_vs_AH_ShowerInEE->GetYaxis()->SetTitle("Energy in AH units of MIPs");
  h_rechit_energy_raw_FH_vs_AH_ShowerInEE->GetYaxis()->SetTitleOffset(1.0);


  h_SS_forLowerEwindow_ShowerInEE = new TH1F("h_SS_forLowerEwindow_ShowerInEE","SS_forLowerEwindow_ShowerInEE, 100<EE<200 mips",88,-2,42);
  
  h_SS_forLowerEwindow_ShowerInEE->GetXaxis()->SetTitle("layer no");


  
  dX_dY_layer_2 = new TH2F("dX_dY_layer_2","dX_dY_layer_2",160,-8.0,8.0,160,-8.0,8.0);


  h_rechit_energy_raw_EE = new TH1F("h_rechit_energy_raw_EE","h_rechit_energy_raw_EE",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_EE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_FH = new TH1F("h_rechit_energy_raw_FH","h_rechit_energy_raw_FH",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_FH->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_all = new TH1F("h_rechit_energy_raw_all","h_rechit_energy_raw_all",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_all->GetXaxis()->SetTitle("Energy in units of MIPs");



  h_rechit_energy_raw_all_MipsInEEFH = new TH1F("h_rechit_energy_raw_all_MipsInEEFH","h_rechit_energy_raw_all_MipsInEEFH",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_all_MipsInEEFH->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_all_MipsInEE = new TH1F("h_rechit_energy_raw_all_MipsInEE","h_rechit_energy_raw_all_MipsInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_all_MipsInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_all_ShowerInEE = new TH1F("h_rechit_energy_raw_all_ShowerInEE","h_rechit_energy_raw_all_ShowerInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_all_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_all_ShowerInEE_modulo_EE1 = new TH1F("h_rechit_energy_raw_all_ShowerInEE_modulo_EE1","h_rechit_energy_raw_all_ShowerInEE_modulo_EE1",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_all_ShowerInEE_modulo_EE1->GetXaxis()->SetTitle("Energy in units of MIPs");


  // float x_low_range_min = 0.0;
  // float x_low_range_max = x_2D_max*0.1;
  // float x_low_bin = (x_low_range_max - x_low_range_min);
  
  h_rechit_energy_raw_low_EE = new TH1F("h_rechit_energy_raw_low_EE","h_rechit_energy_raw_low_EE", 250.0,0.0,250.0);
  h_rechit_energy_raw_low_EE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_low_FH = new TH1F("h_rechit_energy_raw_low_FH","h_rechit_energy_raw_low_FH", 300.0,0.0,300.0);
  h_rechit_energy_raw_low_FH->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_low_all = new TH1F("h_rechit_energy_raw_low_all","h_rechit_energy_raw_low_all", 500.0,0.0,500.0);
  h_rechit_energy_raw_low_all->GetXaxis()->SetTitle("Energy in units of MIPs");


  
  float x_mid_range_min = x_2D_max*0.1;
  float x_mid_range_max = x_2D_max*0.66;
  float x_mid_bin = (x_mid_range_max - x_mid_range_min)/2;
  
  h_rechit_energy_raw_mid_EE = new TH1F("h_rechit_energy_raw_mid_EE","h_rechit_energy_raw_mid_EE", x_mid_bin, x_mid_range_min, x_mid_range_max);
  h_rechit_energy_raw_mid_EE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_mid_FH = new TH1F("h_rechit_energy_raw_mid_FH","h_rechit_energy_raw_mid_FH", x_mid_bin, x_mid_range_min, x_mid_range_max);
  h_rechit_energy_raw_mid_FH->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_mid_all = new TH1F("h_rechit_energy_raw_mid_all","h_rechit_energy_raw_mid_all", x_mid_bin, x_mid_range_min, x_mid_range_max);
  h_rechit_energy_raw_mid_all->GetXaxis()->SetTitle("Energy in units of MIPs");


  float x_high_range_min = x_2D_max*0.25;
  float x_high_range_max = x_2D_max*0.70;
  float x_high_bin = (x_high_range_max - x_high_range_min)/4;
  
  h_rechit_energy_raw_high_EE = new TH1F("h_rechit_energy_raw_high_EE","h_rechit_energy_raw_high_EE", x_high_bin, x_high_range_min, x_high_range_max);
  h_rechit_energy_raw_high_EE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_high_FH = new TH1F("h_rechit_energy_raw_high_FH","h_rechit_energy_raw_high_FH", x_high_bin, x_high_range_min, x_high_range_max);
  h_rechit_energy_raw_high_FH->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_high_all = new TH1F("h_rechit_energy_raw_high_all","h_rechit_energy_raw_high_all", x_high_bin, x_high_range_min, x_high_range_max);
  h_rechit_energy_raw_high_all->GetXaxis()->SetTitle("Energy in units of MIPs");



  /* h_rechit_energy_weighted = new TH1F("h_rechit_energy_weighted","h_rechit_energy_weighted",xbin,0.0,x_2D_max); */
  /* h_rechit_energy_weighted->GetXaxis()->SetTitle("Energy in units of MIPs"); */

  h_rechit_energy_raw_ShowerInEE = new TH1F("h_rechit_energy_raw_ShowerInEE","h_rechit_energy_raw_ShowerInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_part_weighted_ShowerInEE = new TH1F("h_rechit_energy_part_weighted_ShowerInEE","h_rechit_energy_part_weighted_ShowerInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_part_weighted_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_full_weighted_ShowerInEE = new TH1F("h_rechit_energy_full_weighted_ShowerInEE","h_rechit_energy_full_weighted_ShowerInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_full_weighted_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_full_weighted_ShowerInEE_updated = new TH1F("h_rechit_energy_full_weighted_ShowerInEE_updated","h_rechit_energy_full_weighted_ShowerInEE_updated",xbin,0.0,x_2D_max);
  h_rechit_energy_full_weighted_ShowerInEE_updated->GetXaxis()->SetTitle("Energy in units of MIPs");

    h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL = new TH1F("h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL","h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL",xbin,0.0,x_2D_max);
  h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL->GetXaxis()->SetTitle("Energy in units of MIPs");

  
  h_rechit_energy_full_weighted_inGev_ShowerInEE = new TH1F("h_rechit_energy_full_weighted_inGev_ShowerInEE","h_rechit_energy_full_weighted_inGev_ShowerInEE",y_max,0.0,y_max);
  h_rechit_energy_full_weighted_inGev_ShowerInEE->GetXaxis()->SetTitle("Energy(GeV)");

  h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2 = new TH1F("h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2","h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2",y_max,0.0,y_max);
  h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2->GetXaxis()->SetTitle("Energy(GeV)");


  h_rechit_energy_EE_weighted_inGev_ShowerInEE = new TH1F("h_rechit_energy_EE_weighted_inGev_ShowerInEE","h_rechit_energy_EE_weighted_inGev_ShowerInEE",y_max,0.0,y_max);
  h_rechit_energy_EE_weighted_inGev_ShowerInEE->GetXaxis()->SetTitle("Energy(GeV)");

  h_rechit_energy_FH_weighted_inGev_ShowerInEE = new TH1F("h_rechit_energy_FH_weighted_inGev_ShowerInEE","h_rechit_energy_FH_weighted_inGev_ShowerInEE",y_max,0.0,y_max);
  h_rechit_energy_FH_weighted_inGev_ShowerInEE->GetXaxis()->SetTitle("Energy(GeV)");

  h_rechit_energy_AH_weighted_inGev_ShowerInEE = new TH1F("h_rechit_energy_AH_weighted_inGev_ShowerInEE","h_rechit_energy_AH_weighted_inGev_ShowerInEE",y_max,0.0,y_max);
  h_rechit_energy_AH_weighted_inGev_ShowerInEE->GetXaxis()->SetTitle("Energy(GeV)");




  h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL = new TH1F("h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL","h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL",y_max,0.0,y_max);
  h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL->GetXaxis()->SetTitle("Energy(GeV)");

  h_sim_energy_inGev_ShowerInEE = new TH1F("h_sim_energy_inGev_ShowerInEE","h_sim_energy_inGev_ShowerInEE",sim_x_bin,0.0,sim_x_max);
  h_sim_energy_inGev_ShowerInEE->GetXaxis()->SetTitle("Sim Energy(GeV)");


  h_energy_MipsInEE_mips = new TH1F("h_energy_MipsInEE_mips","h_energy_MipsInEE_mips",xbin,0.0,x_2D_max);
  h_energy_MipsInEE_mips->GetXaxis()->SetTitle("Energy(Mips)");

  h_energy_MipsInEE_gev = new TH1F("h_energy_MipsInEE_gev","h_energy_MipsInEE_gev",y_max,0.0,y_max);
  h_energy_MipsInEE_gev->GetXaxis()->SetTitle("Energy(GeV)");

  h_energy_MipsInEE_gev_chi2 = new TH1F("h_energy_MipsInEE_gev_chi2","h_energy_MipsInEE_gev_chi2",y_max,0.0,y_max);
  h_energy_MipsInEE_gev_chi2->GetXaxis()->SetTitle("Energy(GeV)");


  h_rechit_energy_inEE_MipsInEE_mips = new TH1F("h_rechit_energy_inEE_MipsInEE_mips","h_rechit_energy_inEE_MipsInEE_mips",xbin,0.0,x_2D_max);
  h_rechit_energy_inEE_MipsInEE_mips->GetXaxis()->SetTitle("Energy in units of MIPs");


  h_energy_MipsInEE_gev_withoutAHCAL = new TH1F("h_energy_MipsInEE_gev_withoutAHCAL","h_energy_MipsInEE_gev_withoutAHCAL",y_max,0.0,y_max);
  h_energy_MipsInEE_gev_withoutAHCAL->GetXaxis()->SetTitle("Energy(GeV)");


 
  h_sim_energy_inGev_MipsInEE = new TH1F("h_sim_energy_inGev_MipsInEE","h_sim_energy_inGev_MipsInEE",sim_x_bin,0.0,sim_x_max);
  h_sim_energy_inGev_MipsInEE->GetXaxis()->SetTitle("Energy(GeV)");

  h_sim_energy_inGev_SS_notFound = new TH1F("h_sim_energy_inGev_SS_notFound","h_sim_energy_inGev_SS_notFound",sim_x_bin,0.0,sim_x_max);
  h_sim_energy_inGev_SS_notFound->GetXaxis()->SetTitle("Energy(GeV)");


  h_energy_FH_vs_AHCAL_MipsInEE_gev = new TH2F("h_energy_FH_vs_AHCAL_MipsInEE_gev","h_energy_FH_vs_AHCAL_MipsInEE_gev",y_max,0.0,y_max,y_max,0.0,y_max);
  h_energy_FH_vs_AHCAL_MipsInEE_gev->GetXaxis()->SetTitle("FH Energy in GeV");
  h_energy_FH_vs_AHCAL_MipsInEE_gev->GetYaxis()->SetTitle("AHCAL sim Energy in GeV");
  h_energy_FH_vs_AHCAL_MipsInEE_gev->GetYaxis()->SetTitleOffset(1.0);


  h_energy_all_gev = new TH1F("h_energy_all_gev","h_energy_all_gev",y_max,0.0,y_max);
  h_energy_all_gev->GetXaxis()->SetTitle("Energy(GeV)");


    h_energy_all_gev_withoutAHCAL = new TH1F("h_energy_all_gev_withoutAHCAL","h_energy_all_gev_withoutAHCAL",y_max,0.0,y_max);
  h_energy_all_gev_withoutAHCAL->GetXaxis()->SetTitle("Energy(GeV)");

  
  h_sim_energy_all_gev = new TH1F("h_sim_energy_all_gev","h_sim_energy_all_gev",y_max,0.0,y_max);
  h_sim_energy_all_gev->GetXaxis()->SetTitle("Energy(GeV)");



  h_total_sim_energy = new TH1F("h_total_sim_energy","h_total_sim_energy",sim_x_bin,0.0,sim_x_max);
  h_total_sim_energy->GetXaxis()->SetTitle("Energy(GeV)");

  h_total_absorbed_energy = new TH1F("h_total_absorbed_energy","h_total_absorbed_energy",sim_x_bin,0.0,sim_x_max);
  h_total_absorbed_energy->GetXaxis()->SetTitle("Energy(GeV)");

  h_total_absorbed_energy_inc_BL = new TH1F("h_total_absorbed_energy_inc_BL","h_total_absorbed_energy_inc_BL",sim_x_bin,0.0,sim_x_max);
  h_total_absorbed_energy_inc_BL->GetXaxis()->SetTitle("Energy(GeV)");



  h_rechit_energy_EE_FHAHCAL_EH_mips = new TH2F("h_rechit_energy_EE_FHAHCAL_EH_mips","h_rechit_energy_EE_FHAHCAL_EH_mips",xbin,0.0,x_2D_max,xbin,0.0,x_2D_max);
  h_rechit_energy_EE_FHAHCAL_EH_mips->GetXaxis()->SetTitle("EE (MIPs)");
  h_rechit_energy_EE_FHAHCAL_EH_mips->GetYaxis()->SetTitle("#beta *(FH+#alpha *AH) (MIPs)");

  h_rechit_energy_EE_FHAHCAL_EH_gev = new TH2F("h_rechit_energy_EE_FHAHCAL_EH_gev","h_rechit_energy_EE_FHAHCAL_EH_gev",y_max,0.0,y_max,y_max,0.0,y_max);
  h_rechit_energy_EE_FHAHCAL_EH_gev->GetXaxis()->SetTitle("EE (GeV)");
  h_rechit_energy_EE_FHAHCAL_EH_gev->GetYaxis()->SetTitle("#beta *(FH+#alpha *AH)  (GeV)");

  h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG = new TH2F("h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG","h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG",y_max,0.0,y_max,y_max,0.0,y_max);
  h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG->GetXaxis()->SetTitle("EE (GeV)");
  h_rechit_energy_EE_FHAHCAL_EH_gev_DEBUG->GetYaxis()->SetTitle("#beta *(FH+#alpha *AH)  (GeV)");



  h_rechit_energy_raw_EE_alone_ShowerInEE = new TH1F("h_rechit_energy_raw_EE_alone_ShowerInEE","h_rechit_energy_raw_EE_alone_ShowerInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_EE_alone_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_raw_FH_AH_alone_ShowerInEE = new TH1F("h_rechit_energy_raw_FH_AH_alone_ShowerInEE","h_rechit_energy_raw_FH_AH_alone_ShowerInEE",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_FH_AH_alone_ShowerInEE->GetXaxis()->SetTitle("Energy in units of MIPs");





  /* h_rechit_energy_FH_FB_weighted = new TH1F("h_rechit_energy_FH_FB_weighted","h_rechit_energy_FH_FB_weighted",y_max,0.0,y_max); */
  /* h_rechit_energy_FH_FB_weighted->GetXaxis()->SetTitle("Energy in GeV"); */

  /* h_rechit_energy_FB_FH_alone_raw = new TH1F("h_rechit_energy_FB_FH_alone_raw","h_rechit_energy_FB_FH_alone_raw",xbin,0.0,x_2D_max); */
  /* h_rechit_energy_FB_FH_alone_raw->GetXaxis()->SetTitle("Energy in units of MIPs"); */





  h_Nrechit_EE_vs_FH = new TH2F("h_Nrechit_EE_vs_FH","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0);
  h_Nrechit_EE_vs_FH->GetXaxis()->SetTitle("NRechits in EE");
  h_Nrechit_EE_vs_FH->GetYaxis()->SetTitle("NRechits in FH");
  h_Nrechit_EE_vs_FH->GetYaxis()->SetTitleOffset(1.0);

  /* h_Nrechits_EE_FH_FB = new TH2F("h_Nrechits_EE_FH_FB","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0); */
  /* h_Nrechits_EE_FH_FB->GetXaxis()->SetTitle("NRechits in EE"); */
  /* h_Nrechits_EE_FH_FB->GetYaxis()->SetTitle("NRechits in FH"); */
  /* h_Nrechits_EE_FH_FB->GetYaxis()->SetTitleOffset(1.0); */


  /* h_rechit_energy_FH_AHCAL_FB_raw = new TH2F("h_rechit_energy_FH_AHCAL_FB_raw","h_rechit_energy_FH_AHCAL_FB_raw",xbin,0.0,x_2D_max,xbin,0.0,x_2D_max); */
  /* h_rechit_energy_FH_AHCAL_FB_raw->GetXaxis()->SetTitle("FH Energy in units of MIPs"); */
  /* h_rechit_energy_FH_AHCAL_FB_raw->GetYaxis()->SetTitle("AHCAL Energy in units of MIPs"); */
  /* h_rechit_energy_FH_AHCAL_FB_raw->GetYaxis()->SetTitleOffset(1.0); */


  /* h_rechit_energy_FH_AHCAL_FB_weighted = new TH2F("h_rechit_energy_FH_AHCAL_FB_weighted","h_rechit_energy_FH_AHCAL_FB_weighted",y_max,0.0,y_max,y_max,0.0,y_max); */
  /* h_rechit_energy_FH_AHCAL_FB_weighted->GetXaxis()->SetTitle("FH Energy in GeV"); */
  /* h_rechit_energy_FH_AHCAL_FB_weighted->GetYaxis()->SetTitle("AHCAL Energy in GeV"); */
  /* h_rechit_energy_FH_AHCAL_FB_weighted->GetYaxis()->SetTitleOffset(1.0); */
  
  h_shower_start = new TH1F("h_shower_start","h_shower_start",10000,-2.0,5.0);
  h_shower_start->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start->Sumw2();

  h_shower_start_dN_dLambda = new TH1F("h_shower_start_dN_dLambda","h_shower_start_dN_dLambda",10000,-2.0,5.0);
  h_shower_start_dN_dLambda->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_dN_dLambda->Sumw2();

  h_shower_start_full_collapsed_EE = new TH1F("h_shower_start_full_collapsed_EE","h_shower_start_full_collapsed_EE",10000,-2.0,5.0);
  h_shower_start_full_collapsed_EE->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_full_collapsed_EE->Sumw2();

  h_shower_start_part_collapsed_EE = new TH1F("h_shower_start_part_collapsed_EE","h_shower_start_part_collapsed_EE",10000,-2.0,5.0);
  h_shower_start_part_collapsed_EE->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_part_collapsed_EE->Sumw2();

  
  h_shower_start_reg1 = new TH1F("h_shower_start_reg1","h_shower_start_reg1",10000,-2.0,5.0);
  h_shower_start_reg1->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_reg1->Sumw2();

  
  h_shower_start_reg2 = new TH1F("h_shower_start_reg2","h_shower_start_reg2",10000,-2.0,5.0);
  h_shower_start_reg2->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_reg2->Sumw2();

  
  h_shower_start_reg3 = new TH1F("h_shower_start_reg3","h_shower_start_reg3",10000,-2.0,5.0);
  h_shower_start_reg3->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_reg3->Sumw2();

  h_shower_start_CHECK_FOR_UNCLASSIFIED = new TH1F("h_shower_start_CHECK_FOR_UNCLASSIFIED","h_shower_start_CHECK_FOR_UNCLASSIFIED",10000,-2.0,5.0);
  h_shower_start_CHECK_FOR_UNCLASSIFIED->GetXaxis()->SetTitle("#lambda_{0}");

  h_Nrechit_EE_vs_FH_ShowerInEE = new TH2F("h_Nrechit_EE_vs_FH_ShowerInEE","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0);
  h_Nrechit_EE_vs_FH_ShowerInEE->GetXaxis()->SetTitle("NRechits in EE");
  h_Nrechit_EE_vs_FH_ShowerInEE->GetYaxis()->SetTitle("NRechits in FH");
  h_Nrechit_EE_vs_FH_ShowerInEE->GetYaxis()->SetTitleOffset(1.0);

  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED = new TH2F("h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0);
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED->GetXaxis()->SetTitle("NRechits in EE");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED->GetYaxis()->SetTitle("NRechits in FH");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED->GetYaxis()->SetTitleOffset(1.0);


  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R1 = new TH2F("h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R1","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0);
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R1->GetXaxis()->SetTitle("NRechits in EE");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R1->GetYaxis()->SetTitle("NRechits in FH");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R1->GetYaxis()->SetTitleOffset(1.0);

  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R2 = new TH2F("h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R2","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0);
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R2->GetXaxis()->SetTitle("NRechits in EE");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R2->GetYaxis()->SetTitle("NRechits in FH");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R2->GetYaxis()->SetTitleOffset(1.0);

  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R3 = new TH2F("h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R3","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0);
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R3->GetXaxis()->SetTitle("NRechits in EE");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R3->GetYaxis()->SetTitle("NRechits in FH");
  h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED_R3->GetYaxis()->SetTitleOffset(1.0);


  h_shower_start_reg3_DEBUG = new TH1F("h_shower_start_reg3_DEBUG","h_shower_start_reg3_DEBUG",10000,-2.0,5.0);
  h_shower_start_reg3_DEBUG->GetXaxis()->SetTitle("#lambda_{0}");
  //h_shower_start_reg3->Sumw2();
 


  h_frac_event = new TH1F("h_frac_event","h_frac_event",5.0,-2.0,3);
  h_frac_event->Sumw2();



  h_Nrechits_check_EE = new TH1F("h_Nrechits_check_EE","NRechits in EE when EsumEE < 400",100,0,100);
  h_Nrechits_check_EE_whenMIP = new TH1F("h_Nrechits_check_EE_whenMIP","NRechits in EE when SS = MIP in EE",100,0,100);
  h_Nrechits_check_EE_SS_last = new TH1F("h_Nrechits_check_EE_SS_last","NRechits in EE when SS = last3EElayers",100,0,100);
  h_EnRatio_check_EE = new TH1F("h_EnRatio_check_EE","En ratio of last3 when Esum < 400",100,0,1);
  h_EnRatio_check_EE_SSlast = new TH1F("h_EnRatio_check_EE_SSlast","En ratio of last3 when when SS = last3EElayers",100,0,1);
  h_EnRatio_check_EE_SSlast_lowerEWindow = new TH1F("h_EnRatio_check_EE_SSlast_lowerEWindow","En ratio of last3 when when SS = last3EElayers",100,0,1);
  

  
  char* h_name = new char[200];

  d_showerProfile = oFile->mkdir("shower_profile");
  d_showerProfile->cd();

  sprintf(hname,"h_longi_profile_MipsInEE_withAH");
  h_longi_profile_MipsInEE_withAH = new TProfile(hname,hname,10000,-2.0,10.0);

  sprintf(hname,"h_longi_profile_MipsInEE_withAH_energy_fraction");
  h_longi_profile_MipsInEE_withAH_energy_fraction = new TProfile(hname,hname,10000,-2.0,10.0);

  sprintf(h_name,"h_longi_profile_raw_EE_ShowerInEE");
  h_longi_profile_ShowerInEE = new TProfile(h_name,h_name,700, -1.0, 6.0);

  sprintf(h_name,"h_longi_profile_raw_EE_MipsInEE");
  h_longi_profile_MipsInEE = new TProfile(h_name,h_name,700, -1.0, 6.0);



  sprintf(h_name,"h_longi_profile_raw_inclusive");
  h_longi_profile_inclusive = new TProfile(h_name,h_name,700, -1.0, 6.0);



  sprintf(h_name,"h_longi_profile_raw_EE_ShowerInEE_noWeight");
  h_longi_profile_ShowerInEE_noWeight = new TProfile(h_name,h_name,700, -1.0, 6.0);

  sprintf(h_name,"h_longi_profile_raw_EE_MipsInEE_noWeight");
  h_longi_profile_MipsInEE_noWeight = new TProfile(h_name,h_name,700, -1.0, 6.0);



  sprintf(h_name,"h_longi_profile_raw_inclusive_noWeight");
  h_longi_profile_inclusive_noWeight = new TProfile(h_name,h_name,700, -1.0, 6.0);


  sprintf(h_name,"h_longi_profile_ShowerInEE_summed_up_EE");
  h_longi_profile_ShowerInEE_summed_up_EE = new TProfile(h_name,h_name,700, -1.0, 6.0);

  sprintf(h_name,"h_longi_profile_ShowerInEE_summed_up_15EE");
  h_longi_profile_ShowerInEE_summed_up_15EE = new TProfile(h_name,h_name,700, -1.0, 6.0);

  sprintf(h_name,"h_longi_profile_ShowerInEE_summed_up_22EE");
  h_longi_profile_ShowerInEE_summed_up_22EE = new TProfile(h_name,h_name,700, -1.0, 6.0);


  sprintf(h_name,"h_longi_profile_MipsInEE_fractionalE");
  h_longi_profile_MipsInEE_fractionalE = new TProfile(h_name,h_name,700, -1.0, 6.0);

  sprintf(h_name,"h_longi_profile_ShowerInEE_fractionalE");
  h_longi_profile_ShowerInEE_fractionalE = new TProfile(h_name,h_name,700, -1.0, 6.0);

  sprintf(h_name,"h_longi_profile_inclusive_fractionalE");
  h_longi_profile_inclusive_fractionalE = new TProfile(h_name,h_name,700, -1.0, 6.0);

    

  

  for(int i = 0; i < 40; i++) {
    sprintf(h_name,"h_longi_profile_raw_SS_%02d",i+1);
    h_longi_profile_raw[i] = new TProfile(h_name,h_name,700, -1.0, 6.0);

  }


  d_showerProfile_SummedEE = d_showerProfile->mkdir("Summed_EE");
  d_showerProfile_SummedEE->cd();
  
  for(int i = 0; i < 40; i++) {
    sprintf(h_name,"h_longi_profile_ShowerInEE_summed_up_%02dEE",i+1);
    
    h_longi_profile_ShowerInEE_summed_up_[i] = new TProfile(h_name,h_name,700, -1.0, 6.0);

  }



  d_rechitEn_layer = oFile->mkdir("energy_distribution");
  d_rechitEn_layer->cd();
  for(int i = 0; i < 40; i++) {
    sprintf(h_name,"SS_%02d",i+1);
    d_prof_layers[i] = d_rechitEn_layer->mkdir(h_name);
    d_prof_layers[i]->cd();
    for(int j = 0; j < 40; j++) {
      sprintf(h_name,"h_rechit_energy_SS_%02d_layer_%02d",i+1,j+1);
      //h_Rechits_En_SS[i][j] = new TH1F(h_name,h_name,per_layer_xbin,0.0,per_layer_xmax);
      h_Rechits_En_SS[i][j] = new TH1F(h_name,h_name,250.0,0.0,500.0);
      
    }
  }
  // Energy distribution for SS = -1
  d_prof_layers[40] = d_rechitEn_layer->mkdir("SS_-1");
  d_prof_layers[40]->cd();
  for(int j = 0; j < 40; j++) {
    //sprintf(h_name,"h_rechit_energy_SS_%02d_layer_%02d",i+1,j+1);
    sprintf(h_name,"h_rechit_energy_SS_-1_layer_%02d",j+1);
    //h_Rechits_En_SS[i][j] = new TH1F(h_name,h_name,per_layer_xbin,0.0,per_layer_xmax);
      h_Rechits_En_SS[40][j] = new TH1F(h_name,h_name,250.0,0.0,500.0);
      
  }
  
  d_check = d_rechitEn_layer->mkdir("ShowerInEE_EEenergy_lessThan_400");
  d_check->cd();
  for(int j = 0; j < 40; j++) {
    //sprintf(h_name,"h_rechit_energy_SS_%02d_layer_%02d",i+1,j+1);
    sprintf(h_name,"h_rechit_energy_CHECK_layer_%02d",j+1);
    //h_Rechits_En_SS[i][j] = new TH1F(h_name,h_name,per_layer_xbin,0.0,per_layer_xmax);
    h_check[j] = new TH1F(h_name,h_name,250.0,0.0,500.0);
    
  }

  d_check_forBeamline = d_rechitEn_layer->mkdir("beamlineCheck");
  d_check_forBeamline->cd();
  d_total_energy_BL = d_check_forBeamline->mkdir("energy_plot");
  d_ratio_energy_BL = d_check_forBeamline->mkdir("ratio_plot");
  for(int j = 0; j < 40; j++) {
    d_total_energy_BL->cd();
    sprintf(h_name,"h_rechit_energy_forBL_layer_%02d",j+1);
    h_check_forBL[j] = new TH1F(h_name,h_name,250.0,0.0,500.0);
    d_ratio_energy_BL->cd();
    sprintf(h_name,"h_rechit_ratio_forBL_layer_%02d",j+1);
    h_check_forBL_ratio[j] = new TH1F(h_name,h_name,500.0,0.0,1.0);
    
  }

  d_check_forBeamline_inv = d_rechitEn_layer->mkdir("beamlineCheck_inverted");
  d_check_forBeamline_inv->cd();
  d_total_energy_BL_inv = d_check_forBeamline_inv->mkdir("energy_plot_");
  d_ratio_energy_BL_inv = d_check_forBeamline_inv->mkdir("ratio_plot_");

  for(int j = 0; j < 40; j++) {
    d_total_energy_BL_inv->cd();
    sprintf(h_name,"h_rechit_energy_forBL_inv_layer_%02d",j+1);
    h_check_forBL_inv[j] = new TH1F(h_name,h_name,250.0,0.0,500.0);
    d_ratio_energy_BL_inv->cd();
    sprintf(h_name,"h_rechit_ratio_forBL_inv_layer_%02d",j+1);
    h_check_forBL_ratio_inv[j] = new TH1F(h_name,h_name,500.0,0.0,1.0);
    
  }
  d_rechitEn_layer->cd();
  /* (d_check_forBeamline_inv->GetMother())->cd(); */
  h_check_forBL_ratio_mip_window_L2 = new TH1F("h_check_forBL_ratio_mip_window_L2","For muon like events Eratio@EE-L2 <PS_thres",500.0,0.0,1.0);
  h_check_forBL_ratio_inv_mip_window_L2 = new TH1F("h_check_forBL_ratio_inv_mip_window_L2","For muon like events Eratio@EE-L2 >PS_thres",500.0,0.0,1.0);
  h_check_forBL_ratio_NO_mip_window_L2 = new TH1F("h_check_forBL_ratio_NO_mip_window_L2","After removing muon like events Eratio@EE-L2 <PS_thres",500.0,0.0,1.0);
  h_check_forBL_ratio_inv_NO_mip_window_L2 = new TH1F("h_check_forBL_ratio_inv_NO_mip_window_L2","After removing muon like events Eratio@EE-L2 >PS_thres",500.0,0.0,1.0);
  h_check_beamEnergy = new TH1F("h_check_beamEnergy","beamEnergy for EE_L2>20mips",sim_x_bin,0.0,sim_x_max);
  h_check_beamEnergy_SS = new TH1F("h_check_beamEnergy_SS","beamEnergy for SS<=EE_L1",sim_x_bin,0.0,sim_x_max);

  

  d_tracks = oFile->mkdir("tacks_cog"); 
  d_tracks->cd();
  //char* h_name = new char[200];
  for(int i = 0; i < 40; i++) {
    sprintf(h_name,"layer_%d",i+1);
    d_layers[i] = d_tracks->mkdir(h_name);
    d_layers[i]->cd();

    sprintf(h_name,"h_rechit_energy_inclusive_layer_%02d",i+1);
    h_Rechits_En_inclusive[i] = new TH1F(h_name,h_name,per_layer_xbin,0.0,per_layer_xmax);


    sprintf(h_name,"h_trackX_trackY_%d",i+1);
    h_trackX_trackY[i] = new TH2F(h_name,h_name,32,-8.0,8.0,32,-8.0,8.0);
    sprintf(h_name,"h_cogX_cogY_%d",i+1);
    h_cogX_cogY[i] = new TH2F(h_name,h_name,32,-8.0,8.0,32,-8.0,8.0);

    sprintf(h_name,"h_cogX_trackX_%d",i+1);
    h_cogX_trackX[i] = new TH2F(h_name,h_name,32,-8.0,8.0,32,-8.0,8.0);

    sprintf(h_name,"h_cogY_trackY_%d",i+1);
    h_cogY_trackY[i] = new TH2F(h_name,h_name,32,-8.0,8.0,32,-8.0,8.0);

    sprintf(h_name,"h_dX_dY_%d",i+1);
    h_dX_dY[i] = new TH2F(h_name,h_name,32,-8.0,8.0,32,-8.0,8.0);

    sprintf(h_name,"h_dX_%d",i+1);
    h_dX[i] = new TH1F(h_name,h_name,32,-8.0,8.0);

    sprintf(h_name,"h_dY_%d",i+1);
    h_dY[i] = new TH1F(h_name,h_name,32,-8.0,8.0);

    sprintf(h_name,"h_dX_dY_track_vs_rechit_%d",i+1);
    h_dX_dY_track_vs_rechit[i] = new TH2F(h_name,h_name,32,-8.0,8.0,32,-8.0,8.0);
 
    
  }



  d_trackInfo = oFile->mkdir("track_information");
  d_trackInfo->cd();
  for(int i = 0; i < 40; i++) {

    /* sprintf(hname,"HGCAL_layer_%02d",i+1); */
    /* d_trackLayer[i] = d_trackInfo->mkdir(hname); */

    sprintf(hname,"h_trackX_layer_%02d",i+1);
    h_trackX[i] = new TH1F(hname,hname,400,-10.0,10.0);
    sprintf(hname,"h_trackY_layer_%02d",i+1);
    h_trackY[i] = new TH1F(hname,hname,400,-10.0,10.0);
    sprintf(hname,"h_trackX_trackY_layer_%02d",i+1);
    h_trackX_trackY_[i] = new TH2F(hname,hname,20,-10.0,10.0,20,-10.0,10.0);

  }



  d_weightScan = oFile->mkdir("weight_scan");
  d_weightScan->cd();
  for(int i = 0; i < 50; i++) {
    sprintf(hname,"h_weight_%d",i+1);
    h_rechit_energy_FB_rel_weightScan[i] = new TH1F(hname,hname,xbin,0.0,x_2D_max);
    // /\* h_rechit_energy_FB_rel_weightScan[i] = new TH1F(hname,hname,y_max,0.0,y_max); *\/
    h_rechit_energy_FB_rel_weightScan[i]->GetXaxis()->SetTitle("Energy in units of MIPs");
    h_rechit_energy_FB_rel_weightScan[i]->Sumw2();
  }

  d_weightScan_EH = oFile->mkdir("weight_scan_EH");
  d_weightScan_EH->cd();
  for(int i = 0; i < 50; i++) {
    sprintf(hname,"h_weight_%d_EH",i+1);
    // /\* h_rechit_energy_FB_rel_weightScan[i] = new TH1F(hname,hname,xbin,0.0,x_2D_max); *\/
    h_rechit_energy_EH_rel_weightScan[i] = new TH1F(hname,hname,xbin,0.0,x_2D_max);
    h_rechit_energy_EH_rel_weightScan[i]->GetXaxis()->SetTitle("Energy in units of MIPs");
    h_rechit_energy_EH_rel_weightScan[i]->Sumw2();
  }
  

  
}

void AnalyzeHGCOctTB::Alignment_Map_Init() {
  char* f_name = new char[200];
  sprintf(f_name,"../Alignment_Map.txt");
  std::ifstream in(f_name);
  if(!in) { 
    cout<<"ERROR => "<<f_name<<" Not found"<<endl;
    //return;
    exit(0);
  }

  //int layer;
  std::pair<float,float> dx_dy;
  std::pair<int, std::pair<float,float> > temp;
  int layer;
  float dx,dy;
  while(in>>layer>>dx>>dy) {
    //run_layer = std::make_pair(run,layer);
    dx_dy = std::make_pair(dx,dy);
    temp = std::make_pair(layer,dx_dy);
    align_map.insert(temp);
  }

  std::cout<<"INFO: Alignment MAP initialized successfully!!!"<<endl;
}

void AnalyzeHGCOctTB::Noise_Map_Init() {
  char* f_name = new char[200];
  sprintf(f_name,"../Noise_Map.txt");
  std::ifstream in(f_name);
  //int layer;
  if(!in) { 
    cout<<"ERROR => "<<f_name<<" Not found"<<endl;
    //return;
    exit(0);
  }
  std::pair<int,int> mod_chip;
  std::pair<std::pair<int,int>, float> temp;
  int layer,mod_id,mod_pos,chip;
  float noise;
  while(in>>layer>>mod_id>>mod_pos>>chip>>noise) {
    //run_layer = std::make_pair(run,layer);
    mod_chip = std::make_pair(mod_id,chip);
    temp = std::make_pair(mod_chip,noise);
    noise_map.insert(temp);
  }

  std::cout<<"INFO: Noise MAP initialized successfully!!!"<<endl;
}


void AnalyzeHGCOctTB::MIP_ratio_Init() {
  char* f_name = new char[200];
  sprintf(f_name,"../MIP_ratios_for_sim.txt");
  std::ifstream in(f_name);
  //int layer;
  if(!in) { 
    cout<<"ERROR => "<<f_name<<" Not found"<<endl;
    //return;
    exit(0);
  }
  std::pair<int,int> mod_chip;
  std::pair<std::pair<int,int>, float> temp;
  int layer,mod_id,mod_pos,chip;
  float ratio_;
  while(in>>layer>>mod_id>>mod_pos>>chip>>ratio_) {
    //run_layer = std::make_pair(run,layer);
    mod_chip = std::make_pair(mod_id,chip);
    temp = std::make_pair(mod_chip,ratio_);
    mip_ratio_map.insert(temp);
  }

  std::cout<<"INFO: MIP Ratio MAP initialized successfully!!!"<<endl;
}


void AnalyzeHGCOctTB::Module_Map_Init(const char* config) {
  char *f_name = new char[200];

  if(strcmp(config,"alpha")==0 || strcmp(config,"config1")==0) {
    sprintf(f_name,"..//config_maps/moduleMAP_config1.txt");
    cout<<"\n\nINFO: Mapping module configuration ALPHA (oct10-oct17) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else if(strcmp(config,"bravo")==0 || strcmp(config,"config2")==0) {
    sprintf(f_name,"../config_maps/moduleMAP_config2.txt");
    cout<<"\n\nINFO: Mapping module configuration BRAVO (17oct-22oct) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else if(strcmp(config,"charlie")==0  || strcmp(config,"config3")==0) {
    sprintf(f_name,"../config_maps/moduleMAP_config3.txt");
    cout<<"\n\nINFO: Mapping module configuration CHARLIE (23Oct-4Nov) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else {
    cout<<"\n\nERROR: Incorrect configuration entered "<<endl;
    cout<<" Allowed configuration :\n alpha = Configuration 1 (10Oct-17Nov) \n bravo = Configuration 2 (17Oct-22Oct) \n charlie = Configuration 3 (23Oct-17Nov)"<<endl;
    return;
    
  }

  std::ifstream in(f_name);
  if(!in) { 
    cout<<"ERROR => "<<f_name<<" Not found"<<endl;
    //return;
    exit(0);
  }
  int modID_, part_, layer_, pos_;
  cout<<"File name = "<<f_name<<endl;
  while(in>>modID_>>part_>>layer_>>pos_){
    std::pair<int, std::vector<int>> temp_pair;
    std::vector<int> temp_vector;
    temp_vector.push_back(part_);
    temp_vector.push_back(layer_);
    temp_vector.push_back(pos_);
    temp_pair = std::make_pair(modID_,temp_vector);
    module_map.insert(temp_pair);
  }

  cout<<"INFO: Module Mapping Done!!! "<<endl<<endl;


}

void AnalyzeHGCOctTB::Layer_Position_Init() {
  char *f_name = new char[200];
  sprintf(f_name,"../config1_lengths.txt");

  std::ifstream in(f_name);
  if(!in) { 
    cout<<"ERROR => "<<f_name<<" Not found"<<endl;
    //return;
    exit(0);
  }
  int layer_;
  float  cm_, x0, nuc_int_, pi_int_;
  
  cout<<"File name = "<<f_name<<endl;
  while(in>>layer_>>cm_>>x0>>nuc_int_>>pi_int_){
    std::pair<int, std::vector<float>> temp_pair;
    std::vector<float> temp_vector;
    temp_vector.push_back(cm_/10.0);
    temp_vector.push_back(x0);
    temp_vector.push_back(nuc_int_);
    temp_vector.push_back(pi_int_);
    
    temp_pair = std::make_pair(layer_,temp_vector);
    layer_positions.insert(temp_pair);
  }

  cout<<"INFO: Layer Position Mapping Done!!! "<<endl<<endl;


}

void AnalyzeHGCOctTB::Weight_Map_Init() {
  char *f_name = new char[200];
  bool UseSimWeights = true;
  if(UseSimWeights) 
    sprintf(f_name,"../relative_weights_forSim.txt");
  else
    sprintf(f_name,"../relative_weights_forData.txt");

  std::ifstream in(f_name);
  if(!in) { 
    cout<<"ERROR => "<<f_name<<" Not found"<<endl;
    //return;
    exit(0);
  }
  int beamEnergy;
  float  alpha, beta, gamma_ee, gamma_fh;
  
  cout<<"File name = "<<f_name<<endl;
  while(in>>beamEnergy>>alpha>>beta>>gamma_ee>>gamma_fh){
    std::pair<int, std::vector<float>> temp_pair;
    std::vector<float> temp_vector;
    temp_vector.push_back(alpha);
    temp_vector.push_back(beta);
    temp_vector.push_back(gamma_ee);
    temp_vector.push_back(gamma_fh);
    
    temp_pair = std::make_pair(beamEnergy,temp_vector);
    rel_weights.insert(temp_pair);
  }

  if(UseSimWeights)
    cout<<BOLDRED<<"IMPORTANT: Weight have initialized based on SIM SCAN "<<RESET<<endl<<endl;
  else
    cout<<BOLDRED<<"IMPORTANT: Weight have initialized based on DATA SCAN "<<RESET<<endl<<endl;


}


void AnalyzeHGCOctTB::Chi2_Weight_Map_Init() {
  char *f_name_EH = new char[200];
  char *f_name_H = new char[200];
  bool UseSimWeights = false;
  sprintf(f_name_EH,"../chi2_calib_factors_EH_hadrons_DATA.txt");
  sprintf(f_name_H,"../chi2_calib_factors_H_hadrons_DATA.txt");

  std::ifstream in_EH(f_name_EH);
  std::ifstream in_H(f_name_H);
  if(!in_EH) { 
    cout<<"ERROR => "<<f_name_EH<<" Not found"<<endl;
    //return;
    exit(0);
  }
  if(!in_H) { 
    cout<<"ERROR => "<<f_name_H<<" Not found"<<endl;
    //return;
    exit(0);
  }

  int beamEnergy;
  float  w1, w2, w3;
  
  cout<<"File name = "<<f_name_EH<<endl;
  while(in_EH>>beamEnergy>>w1>>w2>>w3){
    std::pair<int, std::vector<float>> temp_pair;
    std::vector<float> temp_vector;
    temp_vector.push_back(w1);
    temp_vector.push_back(w2);
    temp_vector.push_back(w3);

    
    temp_pair = std::make_pair(beamEnergy,temp_vector);
    chi2_weights_EH.insert(temp_pair);
  }
  cout<<BOLDGREEN<<"INFO: Chi2 calibration Map initialized for EH hadrons!! "<<RESET<<endl;

  beamEnergy = -1.0; w1 = -1.0; w2 = -1.0; w3 = -1.0; 
  

  cout<<"File name = "<<f_name_H<<endl;
  while(in_H>>beamEnergy>>w1>>w2>>w3){
    std::pair<int, std::vector<float>> temp_pair;
    std::vector<float> temp_vector;
    temp_vector.push_back(w1);
    temp_vector.push_back(w2);
    temp_vector.push_back(w3);

    
    temp_pair = std::make_pair(beamEnergy,temp_vector);
    chi2_weights_H.insert(temp_pair);
  }
  cout<<BOLDGREEN<<"INFO: Chi2 calibration Map initialized for H hadrons!! "<<RESET<<endl<<endl;

}


// void AnalyzeHGCOctTB::offical_calib_init() {
//   char *f_name = new char[200];
//   sprintf(f_name,"/home/shubham/work/HGCAL/CERNTB/CERN_5_oct_2018/txt_files/official_calib.txt");
//   std::ifstream in(f_name);
//   if(!in){
//     cout<<"Could not find "<<f_name<<endl;
//     return;
//   }
//   int layer_, module_, chip_, channel_;
//   long en_chan;
//   float adc_;
//   while(in>>layer_>>module_>>chip_>>channel_>>adc_){
//     en_chan = chip_*1000+channel_;
//     std::pair<int, int> temp;
//     temp = std::make_pair(layer_,en_chan);
//     std::pair<std::pair<int,int>, float> temp1;
//     temp1 = std::make_pair(temp, adc_);
//     offical_calib_map.insert(temp1);
//   }
// }

// void AnalyzeHGCOctTB::my_calib_init() {
//   char *f_name = new char[200];
//   sprintf(f_name,"/home/shubham/work/HGCAL/CERNTB/CERN_5_oct_2018/txt_files/ADC_MIP_v11.txt");
//   std::ifstream in(f_name);
//   if(!in){
//     cout<<"Could not find "<<f_name<<endl;
//     return;
//   }
//   int layer_, chip_, channel_,entry;
//   long en_chan;
//   float adc_,chi2,mip_err;
//   while(in>>layer_>>chip_>>channel_>>adc_>>chi2>>mip_err>>entry){
//     en_chan = chip_*1000+channel_;
//     std::pair<int,int> temp;
//     temp = std::make_pair(layer_+1,en_chan);
//     std::pair<std::pair<int,int>, float> temp1;
//     temp1 = std::make_pair(temp, adc_);
//     my_calib_map.insert(temp1);
//   }
// }

AnalyzeHGCOctTB::AnalyzeHGCOctTB(const TString &inputFileList, const char *outFileName, const char* dataset, const char* config, const char* energy) {

  TChain *tree = new TChain("rechitntupler/hits");
  TChain *tree2 = new TChain("trackimpactntupler/impactPoints");
  /* TChain *tree3 = new TChain("bigtree"); */


  if( ! FillChain(tree, tree2, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  /* if( ! FillChain(tree, tree2, tree3, inputFileList) ) { */
  /*   std::cerr << "Cannot get the tree " << std::endl; */
  /* } else { */
  /*   std::cout << "Initiating analysis of dataset " << dataset << std::endl; */
  /* } */

  /* if( ! FillChain(tree, inputFileList) ) { */
  /*   std::cerr << "Cannot get the tree " << std::endl; */
  /* } else { */
  /*   std::cout << "Initiating analysis of dataset " << dataset << std::endl; */
  /* } */

  //HGCNtupleVariables::Init(tree, tree2, tree3);
  HGCNtupleVariables::Init(tree, tree2);

  BookHistogram(outFileName, config, energy);
  Module_Map_Init(config);
  Alignment_Map_Init();
  Noise_Map_Init();
  MIP_ratio_Init();
  Layer_Position_Init();
  Weight_Map_Init();
  Chi2_Weight_Map_Init();
  // offical_calib_init();
  // my_calib_init();
  
}
//Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList) {
Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, TChain *chain2, const TString &inputFileList) {
/* Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, const TString &inputFileList) { */

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    //std::cout << "Adding tree from " << buffer.c_str() << std::endl;                                                              
    chain->Add(buffer.c_str());
    chain2->Add(buffer.c_str());
    /* chain3->Add(buffer.c_str()); */

  }
  std::cout << "No. of Entries in chain  : " << chain->GetEntries() << std::endl;
  std::cout << "No. of Entries in chain2 : " << chain2->GetEntries() << std::endl;
  /* std::cout << "No. of Entries in chain3 : " << chain3->GetEntries() << std::endl; */

  return kTRUE;
}

Long64_t AnalyzeHGCOctTB::LoadTree(Long64_t entry) {
  // Set the environment to read one entry                                                                                          
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }

  if (!fChain2) return -5;
  Long64_t centry2 = fChain2->LoadTree(entry);
  if (centry2 < 0) return centry2;
  if (!fChain2->InheritsFrom(TChain::Class()))  return centry2;
  TChain *chain2 = (TChain*)fChain2;
  if (chain2->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }

  /* if (!fChain3) return -5; */
  /* Long64_t centry3 = fChain3->LoadTree(entry); */
  /* if (centry3 < 0) return centry3; */
  /* if (!fChain3->InheritsFrom(TChain::Class()))  return centry3; */
  /* TChain *chain3 = (TChain*)fChain3; */
  /* if (chain3->GetTreeNumber() != fCurrent) { */
  /*   fCurrent = chain->GetTreeNumber(); */
  /*   //    Notify(); */
  /* } */
  
  //if (centry==centry2)
  return centry;
  // cout<<"centry = "<<centry<<endl;
  // if(centry>0)
  //   return centry;
  // else return -1;
}

AnalyzeHGCOctTB::~AnalyzeHGCOctTB() { 

  // if (!fChain || !fChain2) return;
  // delete fChain->GetCurrentFile();
  // delete fChain2->GetCurrentFile();
  // oFile->cd();
  // oFile->Write();
  // oFile->Close();


  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

/*  LocalWords:  Nrechit EE R1 FH GetXaxis SetTitle Sumw2 TH2F reg3 NRechits
 */
/*  LocalWords:  GetYaxis SetTitleOffset
 */
