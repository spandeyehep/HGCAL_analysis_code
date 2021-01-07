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
#include "RecHit.h"
#include "LateralRings.h"
#include "RecHit_AHCAL.h"
#include "LateralSquares.h"
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
  // Bool_t   FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList);
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  //void     EventLoop(const char *,const char *);
  void     EventLoop(const char *, bool, int);
  void     BookHistogram(const char *, const char *, const char* energy);

  void Module_Map_Init(const char *);
  void Alignment_Map_Init();
  void Noise_Map_Init();
  void Layer_Position_Init();
  void Weight_Map_Init();
  void Chi2_Weight_Map_Init();
  void Scaling_Factor_Init();


  TFile *oFile;

  TH1F* h_configuration;
  TH1F* h_particleID;
  TH1F* h_nTracks;
  TH1F* h_beamEnergy;
  TH1F* h_runNumber;
  TH1F* h_moduleID;

  TH1F* h_rechit_energy_raw_all;
  TH1F* h_rechit_energy_TrackQuality_all;
  TH1F* h_rechit_energy_MuonVeto_all;
  TH1F* h_rechit_energy_PreShowerReject_all;
  TH1F* h_rechit_energy_TrackWindow_all;
  TH1F* h_rechit_energy_ChannelMask_all;
  TH1F* h_rechit_energy_NoiseRejection_all;

  TH1F* h_rechit_energy_all_cuts_MIPs_in_EEFH;
  TH1F* h_rechit_energy_all_cuts_showering_in_EE;
  TH1F* h_rechit_energy_all_cuts_MIPs_in_EE;
  
  

  
  TH1F* h_rechit_energy_raw;
  TH1F* h_rechit_energy_raw_EE;
  TH1F* h_rechit_energy_raw_FH;
  TH1F* h_rechit_energy_raw_AH;

  TH1F* h_rechit_energy_raw_all_MipsInEEFH;
  TH1F* h_rechit_energy_raw_all_MipsInEE;
  TH1F* h_rechit_energy_raw_all_ShowerInEE;
  TH1F* h_rechit_energy_raw_all_ShowerInEE_modulo_EE1;

  TH2F* h_rechit_energy_raw_EE_vs_FH_MipsInEEFH;
  TH2F* h_rechit_energy_raw_FH_vs_AH_MipsInEEFH;
  TH2F* h_rechit_energy_raw_EE_vs_FH_MipsInEE;
  TH2F* h_rechit_energy_raw_FH_vs_AH_MipsInEE;
  TH1F* h_rechit_energy_frac_EE_last3_MipsInEE;
  TH1F* h_rechit_energy_frac_FH_last3_MipsInEE;
  TH1F* h_rechit_energy_frac_AH_last3_MipsInEE;
  TH2F* h_rechit_energy_raw_EE_vs_FH_ShowerInEE;
  TH2F* h_rechit_energy_raw_FH_vs_AH_ShowerInEE;


  TH2F* h_rechit_energy_raw_EE_vs_FH_all;
  TH1F* h_rechit_energy_frac_EE_last3_ShowerInEE;
  TH1F* h_rechit_energy_frac_FH_last3_ShowerInEE;
  TH1F* h_rechit_energy_frac_AH_last3_ShowerInEE;


  TH1F* h_rechit_energy_raw_EE_MipsInEE;
  TH1F* h_rechit_energy_raw_EE_MipsInEE_extended;
  TH1F* h_rechit_energy_raw_FH_MipsInEE;
  TH1F* h_rechit_energy_raw_AH_MipsInEE;

  TH1F* h_rechit_energy_raw_EE_ShowerInEE;
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
  TH1F* h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2_withoutAHCAL;
  TH1F* h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2_rescaled;


  TH1F* h_energy_MipsInEE_gev;
  TH1F* h_energy_MipsInEE_gev_chi2;
  TH1F* h_energy_MipsInEE_gev_chi2_withoutAHCAL;
  TH1F* h_energy_MipsInEE_gev_chi2_rescaled;

  TH1F* h_energy_MipsInEE_gev_withoutAHCAL;


  TH1F* h_energy_MipsInEE_mips;
  TH1F* h_rechit_energy_full_withEE_weighted_MipsInEE;
  TH1F* h_rechit_energy_full_withoutEE_weighted_MipsInEE;
  TH1F* h_rechit_energy_inEE_MipsInEE_mips; 
  TH1F* h_energy_all_gev;
  TH1F* h_energy_all_gev_withoutAHCAL;

 
  TH2F* h_trackX_trackY_EE1;
  TH2F* h_trackX_trackY_FH12;


  TH2F* h_Nrechit_EE_vs_FH;
  TH2F* h_Nrechits_EE_FH_FB;

  TH2F* h_rechit_energy_FH_AHCAL_FB_raw;
  TH2F* h_rechit_energy_FH_AHCAL_FB_weighted;
  TH2F* h_rechit_energy_EE_FHAHCAL_EH_mips;
  TH2F* h_rechit_energy_EE_FHAHCAL_EH_gev;




  TH1F* h_Nrechit_EE;
  TH1F* h_Nrechit_FH;
  TH1F* h_Nrechit_AH;

  TH1F* h_Nrechit_low_EE;
  TH1F* h_Nrechit_low_FH;
  TH1F* h_Nrechit_low_AH;

  TH1F* h_Nrechit_low_EE_dR2;
  TH1F* h_Nrechit_low_FH_dR2;

  TH1F* h_Nrechit_low_EE_dR2_inv;
  TH1F* h_Nrechit_low_FH_dR2_inv;

  TH1F* h_Nrechit_low_EE_dR3;
  TH1F* h_Nrechit_low_FH_dR3;

  TH1F* h_Nrechit_low_EE_dR3_inv;
  TH1F* h_Nrechit_low_FH_dR3_inv;

  TH1F* h_Nrechit_low_EE_dR4;
  TH1F* h_Nrechit_low_FH_dR4;

  TH1F* h_Nrechit_low_EE_dR4_inv;
  TH1F* h_Nrechit_low_FH_dR4_inv;

  TH1F* h_Nrechit_low_EE_dR5;
  TH1F* h_Nrechit_low_FH_dR5;

  TH1F* h_Nrechit_low_EE_dR5_inv;
  TH1F* h_Nrechit_low_FH_dR5_inv;

  TH1F* h_Nrechit_low_EE_dR8;
  TH1F* h_Nrechit_low_FH_dR8;

  TH1F* h_Nrechit_low_EE_dR8_inv;
  TH1F* h_Nrechit_low_FH_dR8_inv;

  TH1F* h_Nrechit_low_EE_dR10;
  TH1F* h_Nrechit_low_FH_dR10;

  TH1F* h_Nrechit_low_EE_dR10_inv;
  TH1F* h_Nrechit_low_FH_dR10_inv;

  TH1F* h_shower_start;
  TH1F* h_shower_start_dN_dLambda;
  
  TH1F* h_shower_start_full_collapsed_EE;
  TH1F* h_shower_start_part_collapsed_EE;

  TH1F* h_shower_start_reg1;
  TH1F* h_shower_start_reg2;
  TH1F* h_shower_start_reg3;

  TH1F* h_shower_start_CHECK_FOR_UNCLASSIFIED;
  TH2F* h_Nrechit_EE_vs_FH_ShowerInEE; 
  TH2F* h_Nrechit_EE_vs_FH_CHECK_FOR_UNCLASSIFIED; 
  TH2F* h_rechit_En_NRechits;

  TH2F* h_2D_corr_SS_En;

  int inEnergy_;
  const char *conf_;  
  //spandey
  TH1F* h_run_SS15;
};
#endif

#ifdef AnalyzeHGCOctTB_cxx

void AnalyzeHGCOctTB::BookHistogram(const char *outFileName, const char* conf,  const char* energy) {
  
  char* hname = new char[1000];
  char* dir_name = new char[200];
  double y_max = -1.0;
  double x_2D_max = -1.0;
  double xbin = -1.0;
  double y_2D_max = -1.0;
  double mip_x_max = 3000;
  double mip_y_max = 1500;
  double r2_x_max = -1.0;
  double r2_x_bin = -1.0;
  
  double nrechit_xmax_EE = 0.0;
  double nrechit_xmax_FH = 0.0;
  double nrechit_xmax_AH = 0.0;


  if(!strcmp(energy, "20")) {
    y_max = 200.0;
    x_2D_max = 3000;
    xbin = 300;
    y_2D_max = 1500;
    inEnergy_ = 20;
    r2_x_max = inEnergy_*100;
    r2_x_bin = inEnergy_*10;
    nrechit_xmax_EE = 1000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 1000.0;
    
  }
  else if(!strcmp(energy, "50")) {
    y_max = 200.0;
    /* x_2D_max = 5000; */
    /* x_2D_max = 5000; */
    /* xbin = 500; */
    x_2D_max = 10000;
    xbin = 500;
    y_2D_max = 200;
    inEnergy_ = 50;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;
    nrechit_xmax_EE = 1000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 1000.0;


  }
  else if(!strcmp(energy, "80")) {
    y_max = 200.0;
    /* x_2D_max = 8000; */
    /* xbin = 400; */
    x_2D_max = 12000;
    xbin = 300;
    y_2D_max = 3000;
    inEnergy_ = 80;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;

    nrechit_xmax_EE = 1000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 1000.0;

  }
  else if(!strcmp(energy, "100")) {
    y_max = 300.0;
    /* x_2D_max = 10000; */
    /* xbin = 500; */
    x_2D_max = 18000;
    xbin = 360;
    y_2D_max = 3000;
    inEnergy_ = 100;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;

    nrechit_xmax_EE = 1000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 1000.0;

  }
  else if(!strcmp(energy, "120")) {
    y_max = 400.0;
    /* x_2D_max = 12000; */
    /* xbin = 300; */
    x_2D_max = 20000;
    xbin = 400;
    y_2D_max = 3200;
    inEnergy_ = 120;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;

    nrechit_xmax_EE = 1000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 2000.0;

  }
  else if(!strcmp(energy, "200")) {
    y_max = 600.0;
    /* x_2D_max = 18000; */
    /* xbin = 360; */
    x_2D_max = 40000;
    xbin = 800;
    y_2D_max = 4800;
    inEnergy_ = 200;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;

    nrechit_xmax_EE = 2000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 2000.0;


  }
  else if(!strcmp(energy, "250")) {
    y_max = 700.0;
    /* x_2D_max = 25000; */
    /* xbin = 500; */
    x_2D_max = 50000;
    xbin = 1000;
    y_2D_max = 6000;
    inEnergy_ = 250;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;

    nrechit_xmax_EE = 4000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 2000.0;


  }
  else if(!strcmp(energy, "300")) {
    y_max = 800.0;
    x_2D_max = 60000;
    /* x_2D_max = 20000; */
    xbin = 1000;
    y_2D_max = 6000;
    inEnergy_ = 300;
    r2_x_max = 0.5*inEnergy_*100;
    r2_x_bin = 0.5*inEnergy_*10;

    nrechit_xmax_EE = 4000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 2000.0;

  }
  else if(!strcmp(energy, "420")) {
    y_max = 20.0;
    x_2D_max = 200;
    /* x_2D_max = 20000; */
    xbin = 1000;
    y_2D_max = 6000;
    inEnergy_ = 420;
    r2_x_max = 0.5*200*100;
    r2_x_bin = 0.5*200*10;

    nrechit_xmax_EE = 1000.0;
    nrechit_xmax_FH = 1000.0;
    nrechit_xmax_AH = 1000.0;

  }

  else {
    cout<<"invalid energy!!!"<<endl; 
    return;
  }
  conf_ = conf;
  oFile = new TFile(outFileName, "recreate");

  //spandey

  h_nTracks = new TH1F("h_nTracks","Tracks",12,-1,5);
  h_nTracks->GetXaxis()->SetTitle("Tracks");

  h_configuration = new TH1F("h_configuration","Configuration",60,-10,50);
  h_particleID = new TH1F("h_particleID","particleID",500,0,500);
  h_beamEnergy = new TH1F("h_beamEnergy","beamEnergy",500,0,500);
  h_runNumber = new TH1F("h_runNumber","RunNumber",2000,0,2000);

  h_rechit_energy_raw_all = new TH1F("h_rechit_energy_raw_all","h_rechit_energy_raw_all",xbin,0.0,x_2D_max);
  h_rechit_energy_raw_all->GetXaxis()->SetTitle("Energy in units of MIPs");


  h_rechit_energy_TrackQuality_all = new TH1F("h_rechit_energy_TrackQuality_all","h_rechit_energy_TrackQuality_all",xbin,0.0,x_2D_max);
  h_rechit_energy_TrackQuality_all->GetXaxis()->SetTitle("Energy in units of MIPs");


  h_rechit_energy_MuonVeto_all = new TH1F("h_rechit_energy_MuonVeto_all","h_rechit_energy_MuonVeto_all",xbin,0.0,x_2D_max);
  h_rechit_energy_MuonVeto_all->GetXaxis()->SetTitle("Energy in units of MIPs");


  h_rechit_energy_PreShowerReject_all = new TH1F("h_rechit_energy_PreShowerReject_all","h_rechit_energy_PreShowerReject_all",xbin,0.0,x_2D_max);
  h_rechit_energy_PreShowerReject_all->GetXaxis()->SetTitle("Energy in units of MIPs");



  h_rechit_energy_TrackWindow_all = new TH1F("h_rechit_energy_TrackWindow_all","h_rechit_energy_TrackWindow_all",xbin,0.0,x_2D_max);
  h_rechit_energy_TrackWindow_all->GetXaxis()->SetTitle("Energy in units of MIPs");




  h_rechit_energy_ChannelMask_all = new TH1F("h_rechit_energy_ChannelMask_all","h_rechit_energy_ChannelMask_all",xbin,0.0,x_2D_max);
  h_rechit_energy_ChannelMask_all->GetXaxis()->SetTitle("Energy in units of MIPs");




  h_rechit_energy_NoiseRejection_all = new TH1F("h_rechit_energy_NoiseRejection_all","h_rechit_energy_NoiseRejection_all",xbin,0.0,x_2D_max);
  h_rechit_energy_NoiseRejection_all->GetXaxis()->SetTitle("Energy in units of MIPs");


  h_rechit_energy_all_cuts_showering_in_EE = new TH1F("h_rechit_energy_all_cuts_showering_in_EE","h_rechit_energy_all_cuts_showering_in_EE",xbin,0.0,x_2D_max);
  h_rechit_energy_all_cuts_showering_in_EE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_all_cuts_MIPs_in_EE = new TH1F("h_rechit_energy_all_cuts_MIPs_in_EE","h_rechit_energy_all_cuts_MIPs_in_EE",xbin,0.0,x_2D_max);
  h_rechit_energy_all_cuts_MIPs_in_EE->GetXaxis()->SetTitle("Energy in units of MIPs");

  h_rechit_energy_all_cuts_MIPs_in_EEFH = new TH1F("h_rechit_energy_all_cuts_MIPs_in_EEFH","h_rechit_energy_all_cuts_MIPs_in_EEFH",xbin,0.0,x_2D_max);
  h_rechit_energy_all_cuts_MIPs_in_EEFH->GetXaxis()->SetTitle("Energy in units of MIPs");



  
}

void AnalyzeHGCOctTB::Alignment_Map_Init() {
  char* f_name = new char[200];
  sprintf(f_name,"../txt_maps/Alignment_Map.txt");
  std::ifstream in(f_name);
  //int layer;
  if(!in) {
    cout<<"ERROR => "<<f_name<<" Not found"<<endl;
    //return;
    exit(0);
  }

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
  sprintf(f_name,"../txt_maps/Noise_Map.txt");
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
void AnalyzeHGCOctTB::Module_Map_Init(const char* config) {
  char *f_name = new char[200];

  if(strcmp(config,"alpha")==0 || strcmp(config,"config1")==0) {
    sprintf(f_name,"../txt_maps/config_maps/moduleMAP_config1.txt");
    cout<<"\n\nINFO: Mapping module configuration ALPHA (oct10-oct17) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else if(strcmp(config,"bravo")==0 || strcmp(config,"config2")==0) {
    sprintf(f_name,"../txt_maps/config_maps/moduleMAP_config2.txt");
    cout<<"\n\nINFO: Mapping module configuration BRAVO (17oct-22oct) "<<endl;
    cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;

  }
  else if(strcmp(config,"charlie")==0  || strcmp(config,"config3")==0) {
    sprintf(f_name,"../txt_maps/config_maps/moduleMAP_config3.txt");
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

  /* if(!in){ */
  /*   cout<<"Could not find "<<f_name<<endl; */
  /*   return; */
  /* } */
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
  sprintf(f_name,"../txt_maps/config1_lengths.txt");

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
  bool UseSimWeights = false;
  if(UseSimWeights) 
    sprintf(f_name,"../txt_maps/relative_weights_forSim.txt");
  else
    sprintf(f_name,"../txt_maps/relative_weights_forData.txt");

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
  sprintf(f_name_EH,"../txt_maps/chi2_calib_factors_EH_hadrons_DATA.txt");
  sprintf(f_name_H,"../txt_maps/chi2_calib_factors_H_hadrons_DATA.txt");

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


void AnalyzeHGCOctTB::Scaling_Factor_Init() {
  scaling_factor_EH[20] = 1.087;
  scaling_factor_EH[50] = 1.051;
  scaling_factor_EH[80] = 1.032;
  scaling_factor_EH[100] = 1.035;
  scaling_factor_EH[120] = 1.029;
  scaling_factor_EH[200] = 1.026;
  scaling_factor_EH[250] = 1.021;
  scaling_factor_EH[300] = 1.019;

  scaling_factor_H[20] = 1.093;
  scaling_factor_H[50] = 1.047;
  scaling_factor_H[80] = 1.028;
  scaling_factor_H[100] = 1.027;
  scaling_factor_H[120] = 1.016;
  scaling_factor_H[200] = 1.014;
  scaling_factor_H[250] = 1.012;
  scaling_factor_H[300] = 1.009;
  
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

  TChain *tree = new TChain("pion_variables");


  // if( ! FillChain(tree, tree2, tree3, inputFileList) ) {
  if( ! FillChain(tree, inputFileList) ) {
    
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  /* if( ! FillChain(tree, inputFileList) ) { */
  /*   std::cerr << "Cannot get the tree " << std::endl; */
  /* } else { */
  /*   std::cout << "Initiating analysis of dataset " << dataset << std::endl; */
  /* } */

  HGCNtupleVariables::Init(tree);
  // HGCNtupleVariables::Init(tree, tree2, tree3);

  BookHistogram(outFileName, config, energy);
  Module_Map_Init(config);
  Alignment_Map_Init();
  Noise_Map_Init();
  Layer_Position_Init();
  Weight_Map_Init();
  Chi2_Weight_Map_Init();
  Scaling_Factor_Init();
  // offical_calib_init();
  // my_calib_init();
  
}
// Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList) {
Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, const TString &inputFileList) {

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
    // chain2->Add(buffer.c_str());
    // chain3->Add(buffer.c_str());

  }
  std::cout << "No. of Entries in chain  : " << chain->GetEntries() << std::endl;
  // std::cout << "No. of Entries in chain2 : " << chain2->GetEntries() << std::endl;
  // std::cout << "No. of Entries in chain3 : " << chain3->GetEntries() << std::endl;

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

  // if (!fChain2) return -5;
  // Long64_t centry2 = fChain2->LoadTree(entry);
  // if (centry2 < 0) return centry2;
  // if (!fChain2->InheritsFrom(TChain::Class()))  return centry2;
  // TChain *chain2 = (TChain*)fChain2;
  // if (chain2->GetTreeNumber() != fCurrent) {
  //   fCurrent = chain->GetTreeNumber();
  //   //    Notify();
  // }

  // if (!fChain3) return -5;
  // Long64_t centry3 = fChain3->LoadTree(entry);
  // if (centry3 < 0) return centry3;
  // if (!fChain3->InheritsFrom(TChain::Class()))  return centry3;
  // TChain *chain3 = (TChain*)fChain3;
  // if (chain3->GetTreeNumber() != fCurrent) {
  //   fCurrent = chain->GetTreeNumber();
  //   //    Notify();
  // }
  
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
