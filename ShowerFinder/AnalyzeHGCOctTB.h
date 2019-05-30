#ifndef AnalyzeHGCOctTB_H
#define AnalyzeHGCOctTB_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "HGCNtupleVariables.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TProfile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class AnalyzeHGCOctTB : public HGCNtupleVariables{

 public:
  AnalyzeHGCOctTB(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data", const char *config="alpha", const char* energy = "-1");
  ~AnalyzeHGCOctTB();
  Bool_t   FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList);
  /* Bool_t   FillChain(TChain *chain, const TString &inputFileList); */
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *);
  void     BookHistogram(const char *, const char *, const char* energy);

  void moduleMap_init(const char *);
  void Alignment_Map_Init();


  TFile *oFile;
  
  const char *conf_;  

  TH1F* h_particleID;
  TH1F* h_nTracks;
  TH1F* h_beamEnergy;
  TH1F* h_runNumber;
  TH1F* h_moduleID;
  TH1F* h_shower_start;
  TH1F* h_shower_start_reg1;
  TH1F* h_shower_start_reg2;
  TH1F* h_shower_start_reg3;

  int inEnergy_;
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
  
  if(!strcmp(energy, "20")) {
    y_max = 100.0;
    x_2D_max = 3000;
    xbin = 300;
    y_2D_max = 1500;
    inEnergy_ = 20;
  }
  else if(!strcmp(energy, "50")) {
    y_max = 150.0;
    x_2D_max = 5000;
    xbin = 500;
    y_2D_max = 200;
    inEnergy_ = 50;
  }
  else if(!strcmp(energy, "80")) {
    y_max = 200.0;
    x_2D_max = 8000;
    xbin = 400;
    y_2D_max = 3000;
    inEnergy_ = 80;
  }
  else if(!strcmp(energy, "100")) {
    y_max = 300.0;
    x_2D_max = 10000;
    xbin = 500;
    y_2D_max = 3000;
    inEnergy_ = 100;
  }
  else if(!strcmp(energy, "120")) {
    y_max = 400.0;
    x_2D_max = 12000;
    xbin = 300;
    y_2D_max = 3200;
    inEnergy_ = 120;
  }
  else if(!strcmp(energy, "200")) {
    y_max = 600.0;
    x_2D_max = 18000;
    xbin = 360;
    y_2D_max = 4800;
    inEnergy_ = 200;
  }
  else if(!strcmp(energy, "250")) {
    y_max = 700.0;
    x_2D_max = 25000;
    xbin = 500;
    /* x_2D_max = 20000; */
    y_2D_max = 6000;
    inEnergy_ = 250;
  }
  else if(!strcmp(energy, "300")) {
    y_max = 800.0;
    x_2D_max = 25000;
    /* x_2D_max = 20000; */
    xbin = 50;
    y_2D_max = 6000;
    inEnergy_ = 300;
  }
  else {
    cout<<"invalid energy!!!"<<endl; 
    return;
  }
  conf_ = conf;
  oFile = new TFile(outFileName, "recreate");
  h_nTracks = new TH1F("h_nTracks","Tracks",12,-1,5);
  h_nTracks->GetXaxis()->SetTitle("Tracks");

  /* h_configuration = new TH1F("h_configuration","Configuration",60,-10,50); */
  h_particleID = new TH1F("h_particleID","particleID",500,0,500);
  h_beamEnergy = new TH1F("h_beamEnergy","beamEnergy",500,0,500);
  h_runNumber = new TH1F("h_runNumber","RunNumber",2000,0,2000);
  h_moduleID = new TH1F("h_moduleID","moduleID",4,-2,2);




  
  h_shower_start = new TH1F("h_shower_start","h_shower_start",10000,-2.0,5.0);
  h_shower_start->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start->Sumw2();

  
  h_shower_start_reg1 = new TH1F("h_shower_start_reg1","h_shower_start_reg1",10000,-2.0,5.0);
  h_shower_start_reg1->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_reg1->Sumw2();

  
  h_shower_start_reg2 = new TH1F("h_shower_start_reg2","h_shower_start_reg2",10000,-2.0,5.0);
  h_shower_start_reg2->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_reg2->Sumw2();

  
  h_shower_start_reg3 = new TH1F("h_shower_start_reg3","h_shower_start_reg3",10000,-2.0,5.0);
  h_shower_start_reg3->GetXaxis()->SetTitle("#lambda_{0}");
  h_shower_start_reg3->Sumw2();
 
  
  

  
}

void AnalyzeHGCOctTB::Alignment_Map_Init() {
  char* f_name = new char[200];
  sprintf(f_name,"MIPalignment_run718.txt");
  std::ifstream in(f_name);
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


void AnalyzeHGCOctTB::moduleMap_init(const char* config) {
  char *f_name = new char[200];

  if(strcmp(config,"alpha")==0 || strcmp(config,"config1")==0) {
    sprintf(f_name,"../config_maps/moduleMAP_config1.txt");
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
  if(!in){
    cout<<"Could not find "<<f_name<<endl;
    return;
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
  TChain *tree3 = new TChain("bigtree");


  if( ! FillChain(tree, tree2, tree3, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  /* if( ! FillChain(tree, inputFileList) ) { */
  /*   std::cerr << "Cannot get the tree " << std::endl; */
  /* } else { */
  /*   std::cout << "Initiating analysis of dataset " << dataset << std::endl; */
  /* } */

  HGCNtupleVariables::Init(tree, tree2, tree3);

  BookHistogram(outFileName, config, energy);
  moduleMap_init(config);
  Alignment_Map_Init();
  // offical_calib_init();
  // my_calib_init();
  
}
Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList) {
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
    chain3->Add(buffer.c_str());

  }
  std::cout << "No. of Entries in chain  : " << chain->GetEntries() << std::endl;
  std::cout << "No. of Entries in chain2 : " << chain2->GetEntries() << std::endl;
  std::cout << "No. of Entries in chain3 : " << chain3->GetEntries() << std::endl;

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

  if (!fChain3) return -5;
  Long64_t centry3 = fChain3->LoadTree(entry);
  if (centry3 < 0) return centry3;
  if (!fChain3->InheritsFrom(TChain::Class()))  return centry3;
  TChain *chain3 = (TChain*)fChain3;
  if (chain3->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }
  
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
