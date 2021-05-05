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
#include "../interface/AnalyzeHGCOctTB.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include <math.h>
#include <TF1.h>

using namespace std;

int main(int argc, char* argv[]) {

  if (argc < 6) {
    cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "energy" << " " <<"chi2_method"<<endl;
    
    // cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "configuration" 
    // 	 <<" " << "energy" << endl;

    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *energy = argv[4];
  const char *chi2 = argv[5];

  // const char *event_cat = argv[6];
  // const char *FH_noise = argv[7];
  // const char *noise_sigma = argv[6];

  // bool UseSSBasedCat = (bool)(atoi(event_cat));
  // int noise_FH_ = atoi(FH_noise);

  bool UseSSBasedCat = true;
  int noise_FH_ = 4;
  int chi2_method = atoi(chi2);
  // bool UseSSBasedCat = false;
  // int noise_FH_ = 4;

  // AnalyzeHGCOctTB hgcOctTB(inputFileList, outFileName, data, config, energy, chi2_method);
  AnalyzeHGCOctTB hgcOctTB(inputFileList, outFileName, data, "alpha", energy, chi2_method);
  cout << "dataset " << data << " " << endl;
  cout << "energy " << energy << " " << endl;
  cout << "chi2 method " << chi2_method << " " << endl;
  // cout << "noise_sigma " << noise_sigma << " " << endl;
  hgcOctTB.EventLoop(data,UseSSBasedCat,noise_FH_,chi2_method,energy);
  return 0;
}

void AnalyzeHGCOctTB::EventLoop(const char *data, bool UseSSBasedCat, int noise_FH_, int chi2_method, const char *energy) {

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
  Long64_t region_4_classified_events = 0;
  Long64_t non_classified_events = 0;
  bool UseShowerStartAlgo = UseSSBasedCat;
  bool UseCompartmentEnergy = !UseShowerStartAlgo;


  bool ScaleData = false;
  float data_rescaling = 1.08;

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
  
  if(ScaleData) {
    std::cout<<BOLDRED<<"Data rescaling ON, with factor : "<<data_rescaling<<RESET<<std::endl;
  }
  else {
    std::cout<<BOLDRED<<"Data rescaling OFF"<<RESET<<std::endl;
    data_rescaling = 1.0;
  }
  
  int decade = 0;
  Long64_t ahc_zeroHit = 0;
  Long64_t EE_zeroHit = 0;
  Long64_t FH_zeroHit = 0;
  Long64_t EE_zeroHit_R2 = 0;

  std::map<int,double> E_thres;
  E_thres[20] = 12.0; E_thres[50] = 20.0; E_thres[80] = 25.0; E_thres[100] = 30.0; E_thres[120] = 30.0; E_thres[200] = 40.0; E_thres[250] = 40.0; E_thres[300] = 40.0;

  std::map<int,double> en_range_EE_xmin;
  std::map<int,double> en_range_EE_xmax;
  std::map<int,double> en_range_FH_xmin;
  std::map<int,double> en_range_FH_xmax;

  // older incorrct: using chi2 energy to set cuts
  // en_range_EE_xmin[20] = 8.0; en_range_EE_xmax[20] = 30.0; en_range_FH_xmin[20] = 8.0; en_range_FH_xmax[20] = 30.0;
  // en_range_EE_xmin[50] = 28.0; en_range_EE_xmax[50] = 70.0; en_range_FH_xmin[50] = 30.0; en_range_FH_xmax[50] = 70.0;
  // en_range_EE_xmin[80] = 50.0; en_range_EE_xmax[80] = 110.0; en_range_FH_xmin[80] = 50.0; en_range_FH_xmax[80] = 110.0;
  // en_range_EE_xmin[100] = 65.0; en_range_EE_xmax[100] = 130.0; en_range_FH_xmin[100] = 65.0; en_range_FH_xmax[100] = 135.0;
  // en_range_EE_xmin[120] = 80.0; en_range_EE_xmax[120] = 155.0; en_range_FH_xmin[120] = 84.0; en_range_FH_xmax[120] = 155.0;
  // en_range_EE_xmin[200] = 145.0; en_range_EE_xmax[200] = 250.0; en_range_FH_xmin[200] = 148.0; en_range_FH_xmax[200] = 250.0;
  // en_range_EE_xmin[250] = 190.0; en_range_EE_xmax[250] = 310.0; en_range_FH_xmin[250] = 190.0; en_range_FH_xmax[250] = 310.0;
  // en_range_EE_xmin[300] = 230.0; en_range_EE_xmax[300] = 370.0; en_range_FH_xmin[300] = 230.0; en_range_FH_xmax[300] = 370.0;
  

  // newer : using fixed energy to set cuts
  en_range_EE_xmin[20] = 5.72; en_range_EE_xmax[20] = 21.72; en_range_FH_xmin[20] = 7.96; en_range_FH_xmax[20] = 28.82;
  en_range_EE_xmin[50] = 22.7; en_range_EE_xmax[50] = 56.46; en_range_FH_xmin[50] = 29.97; en_range_FH_xmax[50] = 70.41;
  en_range_EE_xmin[80] = 43.23; en_range_EE_xmax[80] = 91.31; en_range_FH_xmin[80] = 56.02; en_range_FH_xmax[80] = 110.61;
  en_range_EE_xmin[100] = 57.48; en_range_EE_xmax[100] = 114.0; en_range_FH_xmin[100] = 71.97; en_range_FH_xmax[100] = 139.10;
  en_range_EE_xmin[120] = 72.16; en_range_EE_xmax[120] = 137.47; en_range_FH_xmin[120] = 90.14; en_range_FH_xmax[120] = 164.29;
  en_range_EE_xmin[200] = 133.11; en_range_EE_xmax[200] = 230.2; en_range_FH_xmin[200] = 162.09; en_range_FH_xmax[200] = 270.27;
  en_range_EE_xmin[250] = 174.02; en_range_EE_xmax[250] = 287.68; en_range_FH_xmin[250] = 207.8; en_range_FH_xmax[250] = 337.71;
  en_range_EE_xmin[300] = 213.74; en_range_EE_xmax[300] = 344.74; en_range_FH_xmin[300] = 253.1; en_range_FH_xmax[300] = 402.0;
  


  //functions corresponding to method (d) : fixed weighted energy inout to chi2, MIP track energy deposit as offset for H hadrons, events selected from the core
  TF1* f_EH_w1 = new TF1("f_EH_w1","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_EH_w1->FixParameter(0,1.189897);
  f_EH_w1->FixParameter(1,5.215187);

  TF1* f_EH_w2 = new TF1("f_EH_w2","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_EH_w2->FixParameter(0,1.004924);
  f_EH_w2->FixParameter(1,3.352913);
  TF1* f_EH_w3 = new TF1("f_EH_w3","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_EH_w3->FixParameter(0,1.025529);
  f_EH_w3->FixParameter(1,3.602263);

  TF1* f_H_w1 = new TF1("f_H_w1","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_H_w1->FixParameter(0,0.0);
  f_H_w1->FixParameter(1,0.0);
  TF1* f_H_w2 = new TF1("f_H_w2","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_H_w2->FixParameter(0,0.878231);
  f_H_w2->FixParameter(1,2.659706);
  TF1* f_H_w3 = new TF1("f_H_w3","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_H_w3->FixParameter(0,0.984368);
  f_H_w3->FixParameter(1,2.765905);

  // cout<<conf_<<endl;
  // cout<<">>> f_H_w3 Eval at 100 : "<<f_H_w3->Eval(100)<<endl;

  
  float lambda[79];
  for(int i = 0; i < 79; i++) {
    lambda[i] = layer_positions[i+1].at(3); //pi_lambda
    //std::cout<<i+1<<"\t"<<lambda[i]<<endl;
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
  //float ahc_front = 169.9;
  float ahc_front = 159.4;     //from Shilpi link: https://twiki.cern.ch/twiki/pub/CMS/HgcalSimulation/config1LengthsVals.txt



  bool DEBUG = false;
  int debug_count = 0;
  Long64_t event_count[10];
  for (int i = 0; i < 10; i++) event_count[i] = 0;
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


  //For Track window selection cut
  std::map<int,vector<double>> track_window;
  vector<double> temp_window;

  temp_window.clear();
  temp_window.push_back(-2.8);temp_window.push_back(1.65);temp_window.push_back(-1.2);temp_window.push_back(3.2);
  track_window[20] = temp_window;

  temp_window.clear();
  temp_window.push_back(-2.8);temp_window.push_back(1.65);temp_window.push_back(-1.25);temp_window.push_back(3.2);
  track_window[50] = temp_window;

  temp_window.clear();
  //temp_window.push_back(-2.8);temp_window.push_back(1.60);temp_window.push_back(-1.15);temp_window.push_back(2.25);
  temp_window.push_back(-2.8);temp_window.push_back(1.60);temp_window.push_back(-1.15);temp_window.push_back(3.2);
  track_window[80] = temp_window;

  temp_window.clear();
  temp_window.push_back(-2.8);temp_window.push_back(1.50);temp_window.push_back(-1.15);temp_window.push_back(3.2);
  track_window[100] = temp_window;

  temp_window.clear();
  //temp_window.push_back(-2.8);temp_window.push_back(1.50);temp_window.push_back(-1.15);temp_window.push_back(3.0);
  temp_window.push_back(-2.8);temp_window.push_back(1.50);temp_window.push_back(-1.15);temp_window.push_back(3.2);
  track_window[120] = temp_window;


  temp_window.clear();
  temp_window.push_back(-3.0);temp_window.push_back(-1.0);temp_window.push_back(0.2);temp_window.push_back(2.2);
  track_window[200] = temp_window;

  temp_window.clear();
  temp_window.push_back(-2.8);temp_window.push_back(-0.8);temp_window.push_back(-0.2);temp_window.push_back(2.0);
  track_window[250] = temp_window;

  temp_window.clear();
  temp_window.push_back(-3.0);temp_window.push_back(-1.0);temp_window.push_back(-0.3);temp_window.push_back(1.8);
  track_window[300] = temp_window;




  if(DEBUG) cout<<"DEBUG: Configuration = "<<conf_<<endl;
  if(DEBUG) cout<<"DEBUG: TOTAL_ACTIVE_LAYER = "<<TOTAL_ACTIVE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: EE_LAYER = "<<EE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: FH_LAYER = "<<FH_LAYER<<endl;

  if(DEBUG) cout<<"DEBUG: Entering event Loop"<<endl;
  Long64_t jentry;


  //for tree
  char* base_dir = new char[1000];
  // sprintf(base_dir,"/home/work/spandey/public/HGCAL/CERN_TB/octTB/CMSSW_9_3_0/src/pion_analysis/create_cobmined_tree_data/condor_jobs/condor_output");
  sprintf(base_dir,".");
  
  char* outFileName = new char[1000];
  sprintf(outFileName,"%s/skimmed_ntuple_data_config22_pion%sGeV_HGCv16_AHCv8.root",base_dir,energy);
  
  TFile* outfile = TFile::Open(outFileName,"recreate");
  pion_tree = new TTree("pion_variables","variables for pion analysis");
  init_piTree();

  // pion_tree->Branch("pi_event_yolo",&pi_event);

  
  for (jentry=0; jentry<nentries;jentry++,hgc_jentry++) {

    // if(jentry < 5) continue;
    // cout<<"Hello 0 "<<endl;

    //for tree
    pi_event = 0;
    pi_run = 0;
    pi_pdgID = 0;
    pi_beamEnergy = -1;
    pi_trueBeamEnergy = -1;
    pi_NRechits = 0;
    pi_rechit_energy->clear();
    pi_rechit_module->clear();
    pi_rechit_layer->clear();
    pi_rechit_chip->clear();
    pi_rechit_channel->clear();
    pi_rechit_type->clear();
    pi_rechit_x->clear();
    pi_rechit_y->clear();
    pi_rechit_z->clear();
    pi_rechit_iu->clear();
    pi_rechit_iv->clear();
    pi_rechit_iU->clear();
    pi_rechit_iV->clear();
    pi_rechit_amplitudeHigh->clear();
    pi_rechit_amplitudeLow->clear();
    pi_rechit_noise_flag->clear();
    pi_rechit_shower_start_layer = -999;
    pi_rechit_energyPerLayer->clear();
    pi_rechit_nHitsPerLayer->clear();
    pi_rechit_cogX->clear();
    pi_rechit_cogY->clear();


    pi_ntracks = -1;
    pi_trackChi2_X = -1;
    pi_trackChi2_Y = -1;
    pi_dwcReferenceType = -1;
    pi_m_x = -999;
    pi_m_y = -999;
    pi_b_x = -999;
    pi_b_y = -999;
    pi_impactX_layer->clear();
    pi_impactY_layer->clear();
    
    pi_ahc_nHits = 0;
    pi_ahc_energyPerLayer->clear();
    pi_ahc_nHitsPerLayer->clear();
    pi_ahc_hitI->clear();
    pi_ahc_hitJ->clear();
    pi_ahc_hitK->clear();
    pi_ahc_hitEnergy->clear();
    pi_ahc_hitX->clear();
    pi_ahc_hitY->clear();
    pi_ahc_hitZ->clear();

    pi_isHGC_AHC_sync = 0;
    pi_isGoodTrack = 0;
    pi_isFHNoisy = 0;
    pi_MuonVeto = 0;
    pi_isInTrackWindow = 0;
    pi_hgc_channel_mask->clear();
    pi_ahc_channel_mask->clear();
    pi_pass_noise_thres->clear();

    pi_energyLostEE = 0;
    pi_energyLostFH = 0;
    pi_energyLostBH = 0;
    pi_energyLostBeam = 0;

    // cout<<"Hello 0.5 "<<endl;
    
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade) {
      cout << 10 * k << " %" << endl;

    }
    decade = k;

    // cout<<"Hello 1 "<<endl;
    // ===============read this entry == == == == == == == == == == ==

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) { break; cout<<"Breaking"<<endl;}
    nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
    nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    nb3 = fChain3->GetEntry(jentry); nbytes3 += nb3;

    // cout<<"Hello 2 "<<endl;
    
    // ===============Synch HGCAL & AHCAL with same entry == == == == == == == == == == ==

    //New sync version
    while(run != runNumber) {
      if(nentries>nentries3){
	
      hgc_jentry++;
      nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
      nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
      cout<<"hgcal extra event"<<endl;
      pi_isHGC_AHC_sync = false;
      pion_tree->Fill();

      }
      else if(nentries<nentries3)
	{
	  jentry++;
	  nb3 = fChain3->GetEntry(jentry); nbytes3 += nb3;
	  cout<<"Ahcal extra event"<<endl;
	  pi_isHGC_AHC_sync = false;
	  pion_tree->Fill();

	}
    }

    // OLD synch version
    // while(run != runNumber) {
    //   cout<<"Synching HGCAL & AHCAL; run number => "<<run<<endl;
    //   hgc_jentry++;
    //   nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
    //   nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    //   //for tree
    //   pi_isHGC_AHC_sync = false;
    //   pion_tree->Fill();
      
    // }
    // cout<<"Hello 3 "<<endl;

    //for tree
    pi_isHGC_AHC_sync = true;
    pi_event = event;
    pi_run = run;
    pi_pdgID = pdgID;
    pi_beamEnergy = beamEnergy;
    pi_trueBeamEnergy = trueBeamEnergy;
    pi_NRechits = NRechits;
    pi_rechit_energy = rechit_energy;
    pi_rechit_x = rechit_x;
    pi_rechit_y = rechit_y;
    pi_rechit_z = rechit_z;
    pi_rechit_iu = rechit_iu;
    pi_rechit_iv = rechit_iv;
    pi_rechit_iU = rechit_iU;
    pi_rechit_iV = rechit_iV;
    pi_rechit_amplitudeHigh = rechit_amplitudeHigh;
    pi_rechit_amplitudeLow = rechit_amplitudeLow;
    pi_rechit_noise_flag = rechit_noise_flag;
    pi_rechit_module = rechit_module;
    pi_rechit_layer = rechit_layer;
    pi_rechit_chip = rechit_chip;
    pi_rechit_channel = rechit_channel;
    pi_rechit_type = rechit_type;


    pi_ntracks = ntracks;
    pi_trackChi2_X = trackChi2_X;
    pi_trackChi2_Y = trackChi2_Y;
    pi_dwcReferenceType = dwcReferenceType;
    pi_m_x = m_x;
    pi_m_y = m_y;
    pi_b_x = b_x;
    pi_b_y = b_y;


    pi_ahc_nHits = ahc_nHits;
    pi_ahc_energyPerLayer = ahc_energyPerLayer;
    pi_ahc_nHitsPerLayer = ahc_nHitsPerLayer;
    pi_ahc_hitI = ahc_hitI;
    pi_ahc_hitJ = ahc_hitJ;
    pi_ahc_hitK = ahc_hitK;
    pi_ahc_hitX = ahc_hitX;
    pi_ahc_hitY = ahc_hitY;
    pi_ahc_hitZ = ahc_hitZ;
    pi_ahc_hitEnergy = ahc_hitEnergy;
    
    
    // pion_tree->Fill();



    
    // if(NRechits == 0) continue;
    h_nTracks->Fill(ntracks);
    event_count[0]++;
    
    // if(ntracks != 1) continue;
    bool isGoodTrack = (dwcReferenceType >= 13 && trackChi2_X < 10 && trackChi2_Y < 10);
    // if(!isGoodTrack) continue;
    event_count[1]++;

    //for tree
    pi_isGoodTrack = isGoodTrack;
    
    E_beam = beamEnergy;
    h_beamEnergy->Fill(E_beam);
    h_particleID->Fill(pdgID);
    h_runNumber->Fill(run);

    Float_t track_x[79];
    Float_t track_y[79];

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


    //track window cut!!!
    double trackX_al_3 = -1.0*track_x[2] + dxy_alignment(3).first;
    double trackY_al_3 = -1.0*track_y[2] + dxy_alignment(3).second;


    vector<double> track_cut = track_window[(int)E_beam];

    //for tree
    if((trackX_al_3 < track_cut.at(0) || trackX_al_3 > track_cut.at(1) || trackY_al_3 < track_cut.at(2) || trackY_al_3 > track_cut.at(3)))  {
      pi_isInTrackWindow = false;
    }
    else {
      pi_isInTrackWindow = true;
    }

    


    for(int j = 0; j < 39; j++) {
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

    //for tree
    for(int j = 0; j < 79; j++)  {
      pi_impactX_layer->push_back(track_x[j]);
      pi_impactY_layer->push_back(track_y[j]);
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

    std::vector<RecHit*> rechit_collection[40];
    std::vector<RecHit_AHCAL*> rechit_AH_collection[39];
    double cogX_[40];
    double cogY_[40];
    Long_t Nrechit_FH_module[12][7];
    Double_t RechitsEn_FH_module[12][7];


    // For shower start
    Double_t En_cog_indR2[40];
    Double_t En_cog_indR10[40];
    int Nrec_cog_indR10[40];
    Double_t R210_avg[40];




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
      cogX_[ii] = 0.0;
      cogY_[ii] = 0.0;
      rechit_collection[ii].clear();
      En_cog_indR2[ii] = 0.0;
      En_cog_indR10[ii] = 0.0;
      Nrec_cog_indR10[ii] = 0;
      R210_avg[ii] = 0.0;
      if(ii < 39) {
      	RechitsEn_AH[ii] = 0.0;
      	NRechits_AH[ii] = 0;
	rechit_AH_collection[ii].clear();
      
      }
      if(ii < 12) {
	for(int j = 0; j < 7; j++) { 
	  Nrechit_FH_module[ii][j] = 0; 
	  RechitsEn_FH_module[ii][j] = 0; 
	}
      }

    }




    ////////////////////////////////////////////
    //            HGCAL Part                  //
    ////////////////////////////////////////////

    /// Read Tree
    if(DEBUG) cout<<"DEBUG: Start Analylizing RecHits!!"<<endl;
    std::vector<int> temp_moduleID;
    std::vector<double> rechitX_EE03;
    std::vector<double> rechitY_EE03;
    std::vector<double> rechitX_EE03_corrected;
    std::vector<double> rechitY_EE03_corrected;
    std::vector<double> rechitEnergy_EE03;

    std::vector<double> rechitX_EE15;
    std::vector<double> rechitY_EE15;
    std::vector<double> rechitX_EE15_corrected;
    std::vector<double> rechitY_EE15_corrected;
    std::vector<double> rechitEnergy_EE15;


    std::vector<double> rechitX_FH10;
    std::vector<double> rechitY_FH10;
    std::vector<double> rechitX_FH10_corrected;
    std::vector<double> rechitY_FH10_corrected;
    std::vector<double> rechitEnergy_FH10;

    rechitX_EE03.clear();
    rechitY_EE03.clear();
    rechitX_EE03_corrected.clear();
    rechitY_EE03_corrected.clear();
    rechitEnergy_EE03.clear();

    rechitX_EE15.clear();
    rechitY_EE15.clear();
    rechitX_EE15_corrected.clear();
    rechitY_EE15_corrected.clear();
    rechitEnergy_EE15.clear();

    rechitX_FH10.clear();
    rechitY_FH10.clear();
    rechitX_FH10_corrected.clear();
    rechitY_FH10_corrected.clear();
    rechitEnergy_FH10.clear();

    double cog_z_EE = 0.0;
    double cog_z_FH = 0.0;
    double cog_z_HGCAL = 0.0;

    double cog_layer_EE = 0.0;
    double cog_layer_FH = 0.0;
    double cog_layer_HGCAL = 0.0;


    double noise_sigma_ = -1.0; 

    for(int i = 0 ; i < NRechits; i++){
      temp_moduleID.clear();
      int temp_layer = rechit_layer->at(i);
      int temp_chip = rechit_chip->at(i);
      int temp_channel = rechit_channel->at(i);
      int en_chan = temp_chip*1000+temp_channel;
      
      double recx = rechit_x->at(i);
      double recy = rechit_y->at(i);
      
      //for tree
      bool channel_mask = (en_chan == 3022 || en_chan == 3028 || en_chan == 44) || (temp_layer == 1 && temp_chip == 0) || (rechit_module->at(i) == 39 && en_chan == 1016);

      pi_hgc_channel_mask->push_back(channel_mask);

      
      if(temp_layer <= 28) noise_sigma_ = 3.0;
      else noise_sigma_ = noise_FH_;

      std::pair<float, float> temp_mod_chip((int)rechit_module->at(i),(int)rechit_chip->at(i));
      float noise_chip = getNoise(temp_mod_chip);  // Get 1Sigma noise value from the noise Map corresponding to moduleID and chip

      bool Isnoisy = rechit_noise_flag->at(i) || (rechit_amplitudeHigh->at(i) < noise_sigma_*noise_chip);

      pi_pass_noise_thres->push_back(!Isnoisy);
      
      //channel masking
      if(en_chan == 3022 || en_chan == 3028 || en_chan == 44) continue;
      if(temp_layer == 1 && temp_chip == 0) continue;
      if(rechit_module->at(i) == 39 && en_chan == 1016) continue;
      if(rechit_noise_flag->at(i)) continue;    //Additional noise flag from ntuple
      
      //////// Noise cut
      // std::pair<float, float> temp_mod_chip((int)rechit_module->at(i),(int)rechit_chip->at(i));
      // float noise_chip = getNoise(temp_mod_chip);  // Get 1Sigma noise value from the noise Map corresponding to moduleID and chip

      // if(temp_layer <= 28) noise_sigma_ = 3.0;
      // else noise_sigma_ = noise_FH_;

      if(rechit_amplitudeHigh->at(i) < noise_sigma_*noise_chip) continue;
      /////////////////////////////////////



      
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
      }
      
      Nrechit_layer[temp_layer-1]++;
      RechitEn_layer[temp_layer-1]+=rechit_energy->at(i);

      cogX_[temp_layer-1] += (rechit_x->at(i)*rechit_energy->at(i));
      cogY_[temp_layer-1] += (rechit_y->at(i)*rechit_energy->at(i));
      
      temp_moduleID = getModuleLocation(rechit_module->at(i));


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
      


      //// Minor debug for traxkImpact vs rechitPosition for EE03
      if(temp_layer == 3) {
	rechitX_EE03.push_back(rechit_x->at(i));
	rechitY_EE03.push_back(rechit_y->at(i));
	rechitX_EE03_corrected.push_back(recx);
	rechitY_EE03_corrected.push_back(recy);
	rechitEnergy_EE03.push_back(rechit_energy->at(i));
      }

      //// Minor debug for traxkImpact vs rechitPosition for EE15
      if(temp_layer == 15) {
	rechitX_EE15.push_back(rechit_x->at(i));
	rechitY_EE15.push_back(rechit_y->at(i));
	rechitX_EE15_corrected.push_back(recx);
	rechitY_EE15_corrected.push_back(recy);
	rechitEnergy_EE15.push_back(rechit_energy->at(i));
      }


      if(temp_layer == 38) {
	rechitX_FH10.push_back(rechit_x->at(i));
	rechitY_FH10.push_back(rechit_y->at(i));
	rechitX_FH10_corrected.push_back(recx);
	rechitY_FH10_corrected.push_back(recy);
	rechitEnergy_FH10.push_back(rechit_energy->at(i));
      }
            
      //////////////////////////////////////////////////////////////////////////
      
      if(rechit_layer->at(i) > 28) {
	// cout<<module_part_<<":"<<module_layer_<<":"<<module_position_<<endl;
	Nrechit_FH_module[rechit_layer->at(i)-29][module_position_-1]++;
	RechitsEn_FH_module[rechit_layer->at(i)-29][module_position_-1] += rechit_energy->at(i);
	if(rechit_layer->at(i) == 36 && module_position_ == 2) {
	  h_FH_debug->Fill(getBIN(rechit_chip->at(i),rechit_channel->at(i)),rechit_energy->at(i));
	}
      }
      if(module_part_ == 0) {
	// rechitEnergySum_EE+=rechit_energy->at(i);
	RechitsEn_EE[module_layer_-1]+=rechit_energy->at(i);
	NRechits_EE[module_layer_-1]++;
	cog_z_EE += rechit_energy->at(i)*rechit_z->at(i);
	cog_layer_EE += rechit_energy->at(i)*rechit_layer->at(i);
	cog_z_HGCAL += rechit_energy->at(i)*rechit_z->at(i);
	cog_layer_HGCAL += rechit_energy->at(i)*rechit_layer->at(i);

      }
      else if(module_part_ == 1) {
	// rechitEnergySum_FH+=rechit_energy->at(i);
	RechitsEn_FH[module_layer_-1]+=rechit_energy->at(i);
	NRechits_FH[module_layer_-1]++;
	cog_z_FH += rechit_energy->at(i)*rechit_z->at(i);
	cog_layer_FH += rechit_energy->at(i)*rechit_layer->at(i);
	cog_z_HGCAL += rechit_energy->at(i)*rechit_z->at(i);
	cog_layer_HGCAL += rechit_energy->at(i)*rechit_layer->at(i);
	
      }
      else {
	cout<<"ERROR: Unknown Module Part detected!!!!"<<endl;
	return;
	
      }
      rechitEnergySum+=rechit_energy->at(i);
      
      //////////////////////////////////////////////////////////////////////////////////
      /////////////////// Saving information Lateral shower variable information ////////
      //////////////////////////////////////////////////////////////////////////////////

      // cout<<temp_layer<<":"<< temp_chip<<":"<< temp_channel<<":"<< en_chan<<":"<< module_part_<<":"<< module_position_<<":"<< rechit_module->at(i)<<":"<< rechit_iu->at(i)<<":"<< rechit_iv->at(i)<<":"<< rechit_x->at(i)<<":"<< rechit_y->at(i)<<":"<< rechit_z->at(i)<<":"<< rechit_energy->at(i)<<endl;

      // cout<<rechit_iu->at(i)<<":"<< rechit_iv->at(i)<<endl;

      RecHit* rechit_temp = new RecHit(temp_layer, temp_chip, temp_channel, en_chan, module_part_, module_position_, rechit_module->at(i), (int)rechit_iu->at(i), (int)rechit_iv->at(i), rechit_x->at(i), rechit_y->at(i), rechit_z->at(i), rechit_energy->at(i));

      rechit_collection[temp_layer-1].push_back(rechit_temp);
      //////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////




    } 
    ////////////////////////////////////
    ////////// End of HGCAL nRechit Loop
    ////////////////////////////////////


    /// REJECTING EVENTS WITH VERY HIGH OCCUPANCY in FH9_P4 and FH9_P5 for config 1

    if(Nrechit_FH_module[8][3] > 80 || Nrechit_FH_module[8][4] > 80) {
      // cout<<jentry<<" : "<<Nrechit_FH_module[8][3]<<" : "<<Nrechit_FH_module[8][4]<<endl;

      //for tree
      pi_isFHNoisy = true;
      // pion_tree->Fill();
      // continue;
    }
    event_count[2]++;


    ////////////////////////////////////////////
    //            AHCAL Part                  //
    ////////////////////////////////////////////
    
    Double_t rechitEnergySum_AH = 0.0;
    Double_t cog_layer_AH = 0.0;
    Long_t Nrechit_AH = ahc_nHits;
    for(int i = 0 ; i < ahc_nHits; i++) {
      int temp_layer = ahc_hitK->at(i);
      //Skip AHCAL layer 38
      //for tree
      pi_ahc_channel_mask->push_back(temp_layer == 38);
      if(temp_layer == 38) continue;
      
      RechitsEn_AH[temp_layer -1] += ahc_hitEnergy->at(i);
      rechitEnergySum_AH += ahc_hitEnergy->at(i);
      NRechits_AH[temp_layer -1]++;
      cog_layer_AH += ahc_hitEnergy->at(i)*temp_layer;

      RecHit_AHCAL* rechit_temp = new RecHit_AHCAL(temp_layer, ahc_hitI->at(i), ahc_hitJ->at(i), ahc_hitX->at(i), ahc_hitY->at(i), ahc_hitZ->at(i), ahc_hitEnergy->at(i));
      rechit_AH_collection[temp_layer-1].push_back(rechit_temp);
    }
    double AH_last3 = (RechitsEn_AH[36]+RechitsEn_AH[37]+RechitsEn_AH[38]);
    if(AH_last3/rechitEnergySum_AH > 1.0) {
      cout<<AH_last3<<"::"<<rechitEnergySum_AH<<endl;
    }

    //////////////////////////////////////////////
    ////////// End of AHCAL nRechit Loop  ////////
    //////////////////////////////////////////////
    


    ///////////////////////////////////////////////////////////////
    /// S H O W E R    S T A R T    F I  N D I N G     A L G  O ///
    ///////////////////////////////////////////////////////////////
    //Deprecated !!!!!!!!!
    // See new shower start algorithm

    bool MIP = true;
    int shower_start_index = -1;
    float shower_lambda_ = -1.0;
    float shower_weight_ = 1.0;

    // if(energy_sum_dR2_[0] > 20) {
    //   // cout<<"Shower started in layer =1"<<endl;
    //   shower_start_index = 0;
    //   shower_lambda_ = lambda[0];
    //   shower_weight_ = lambda[0];
    //   MIP = false;
    // }
    
    // else if(energy_sum_dR2_[1] > 20 && energy_sum_dR2_[1] > 2*energy_sum_dR2_[0]) {
    //   // cout<<"Shower started in layer =2"<<endl;
    //   shower_start_index = 1;
    //   shower_lambda_ = lambda[1];
    //   shower_weight_ = lambda[1]-lambda[0];
    //   MIP = false;
    // }
    
    // else {
      
    //   for(int i = 2; i < 40; i++) {
    // 	if((energy_sum_dR2_[i] > 20) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-1]) && (energy_sum_dR2_[i] > 2*energy_sum_dR2_[i-2])) {
    // 	  shower_start_index = i;
    // 	  shower_lambda_ = lambda[i];
    // 	  shower_weight_ = lambda[i]-lambda[i-1];
    // 	  MIP = false;
    // 	  break;
    // 	}
    //   }
    // }


    //////////////////////////////////////
    ///    E N D     O F     A L G O   ///
    //////////////////////////////////////





    ////////////////////////////////////////////////////////////////
    //                Comapartment wise calculation              //
    ///////////////////////////////////////////////////////////////
    

    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;
    Double_t test = 0.0;
    bool zero_rh[40];
    float seed_energy_incl = 0.0;  
    float r1_energy_incl = 0.0;    
    float r2_energy_incl = 0.0;    
    float E1_inc = 0.0; float E7_inc = 0.0; float E19_inc = 0.0;
    float E1_EE = 0.0; float E7_EE = 0.0; float E19_EE = 0.0;
    float E1_FH = 0.0; float E7_FH = 0.0; float E19_FH = 0.0;
    float S1_AH = 0.0; float S9_AH = 0.0; float S25_AH = 0.0;
    
    int AHCAL_uniqueLayer = 0;

    double EE_last3 = RechitsEn_EE[25]+RechitsEn_EE[26]+RechitsEn_EE[27];
    double FH_last3 = RechitsEn_FH[9]+RechitsEn_FH[10]+RechitsEn_FH[11];



    ////////////////////////////////////////////////////////////////
    /////////////////////// For new Shower start  //////////////////
    ////////////////////////////////////////////////////////////////

    int nrechit_sumEE_dR2_ = 0; int nrechit_sumFH_dR2_ = 0; int nrechit_sumEE_dR2_inv = 0; int nrechit_sumFH_dR2_inv = 0;
    int nrechit_sumEE_dR10_ = 0; int nrechit_sumFH_dR10_ = 0; int nrechit_sumEE_dR10_inv = 0; int nrechit_sumFH_dR10_inv = 0;


    
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){

      if(iL < EE_LAYER) {  //EE layers
	Nrechit_EE+=NRechits_EE[iL];
	rechitEnergySum_EE+=RechitsEn_EE[iL];

	//center of gravity for each EE layer
	if(RechitsEn_EE[iL] > 0.0) {
	  cogX_[iL] = cogX_[iL]/RechitsEn_EE[iL];
	  cogY_[iL] = cogY_[iL]/RechitsEn_EE[iL];
	}
	//layerwise_comparison
	h_ld_energy[iL]->Fill(RechitsEn_EE[iL]);
	h_ld_nrechit[iL]->Fill(NRechits_EE[iL]);

	//for tree
	pi_rechit_energyPerLayer->push_back(RechitsEn_EE[iL]);
	pi_rechit_nHitsPerLayer->push_back(NRechits_EE[iL]);

      }
      else {   //FH layers
	Nrechit_FH+=NRechits_FH[iL-EE_LAYER];
	rechitEnergySum_FH+=RechitsEn_FH[iL-EE_LAYER];
	if(RechitsEn_FH[iL-EE_LAYER] > 0.0) {
	  cogX_[iL] = cogX_[iL]/RechitsEn_FH[iL-EE_LAYER];
	  cogY_[iL] = cogY_[iL]/RechitsEn_FH[iL-EE_LAYER];
	}

	for(int j = 0; j < 7; j++) {
    	  if(iL+1 >= 38 && j+1 != 4) continue;
	  h_nrechit_FH[iL-EE_LAYER][j]->Fill(Nrechit_FH_module[iL-EE_LAYER][j]);
	  h_energy_FH[iL-EE_LAYER][j]->Fill(RechitsEn_FH_module[iL-EE_LAYER][j]);
    	}

	//layerwise_comparison
	h_ld_energy[iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
	h_ld_nrechit[iL]->Fill(NRechits_FH[iL-EE_LAYER]);

	//for tree
	pi_rechit_energyPerLayer->push_back(RechitsEn_FH[iL-EE_LAYER]);
	pi_rechit_nHitsPerLayer->push_back(NRechits_FH[iL-EE_LAYER]);


      }
      h_cogX[iL]->Fill(cogX_[iL]);
      h_cogY[iL]->Fill(cogY_[iL]);
      h_cogX_cogY[iL]->Fill(cogX_[iL],cogY_[iL]);

      //for tree
      pi_rechit_cogX->push_back(cogX_[iL]);
      pi_rechit_cogY->push_back(cogY_[iL]);
      
      // Fill track variables as it is
      // double track_x_p = (-1*track_x[iL]) + dxy_alignment(iL+1).first;
      // double track_y_p = (-1*track_y[iL]) + dxy_alignment(iL+1).second;
      // double track_x_p = track_x[iL];
      // double track_y_p = track_y[iL];
      h_trackX[iL]->Fill(track_x[iL]);
      h_trackY[iL]->Fill(track_y[iL]);
      h_trackX_trackY[iL]->Fill(track_x[iL],track_y[iL]);

      //////////////////////////////////////////////////////
      /////////// Caluculations for new shower start //////////
      //////////////////////////////////////////////////////

      for(int i = 0; i < (int)rechit_collection[iL].size(); i++) {
      	RecHit* temp_rechit = rechit_collection[iL].at(i);
      	float recx = (temp_rechit->getCellCoordinate_xyz())[0];
      	float recy = (temp_rechit->getCellCoordinate_xyz())[1];
      	double dR = deltaR(recx,recy,cogX_[iL],cogY_[iL]);
      	if(dR < 2.0) {
      	  if(iL < 28) nrechit_sumEE_dR2_++;
      	  else nrechit_sumFH_dR2_++;
      	  En_cog_indR2[iL] += temp_rechit->getEnergy();
	  
      	}
      	if(dR < 10.0) {
      	  if(iL < 28) nrechit_sumEE_dR10_++;
      	  else nrechit_sumFH_dR10_++;
      	  En_cog_indR10[iL] += temp_rechit->getEnergy();
      	  Nrec_cog_indR10[iL]++;
      	} 


      }
      

      ///////////////////////////////////////////////////////////////
      //////// For lateral shower variable calculation ////////////
      ///////////////////////////////////////////////////////////////

      // cout<<"size at layer "<<iL+1<<" : "<<rechit_collection[iL].size()<<endl;
      LateralRings* temp_LR = new LateralRings(rechit_collection[iL]);
      if(!rechit_collection[iL].size()) { 
	temp_LR->setLayer(iL+1);
      }

      float seed = temp_LR->getSeedEnergy();
      float r1 = temp_LR->getEnergyInRing1();
      float r2 = temp_LR->getEnergyInRing2();
      float E1 = seed; float E7 = E1+r1; float E19 = E7+r2;

      seed_energy_incl += seed;
      r1_energy_incl += r1;
      r2_energy_incl += r2;
      E1_inc += seed;
      E7_inc += E7;
      E19_inc += E19;
      if(iL < 28) {
	E1_EE += seed;
	E7_EE += E7;
	E19_EE += E19;

      }
      else {
	E1_FH += seed;
	E7_FH += E7;
	E19_FH += E19;
      }
      // cout<<iL+1<<" seed: "<<seed<<" ,r1: "<<r1<<" ,r2: "<<r2<<" :: E1:"<<E1<<" ,E7: "<<E7<<" ,E19: "<<E19<<endl;


      if(iL < 39) {
	LateralSquares* temp_LS = new LateralSquares(rechit_AH_collection[iL]);
	if(!rechit_AH_collection[iL].size()) { 
	  temp_LS->setLayer(iL+1);
	}
	else AHCAL_uniqueLayer++;
	
	float seed_AH = temp_LS->getSeedEnergy(); // ==> 1 cell
	float s1 = temp_LS->getEnergyInSquare1(); // ==> 8 cells
	float s2 = temp_LS->getEnergyInSquare2(); // ==> 16 cells
	float S1 = seed_AH; float S9 = S1+s1; float S25 = S9+s2;
	S1_AH += seed_AH;
	S9_AH += S9;
	S25_AH += S25;
	// cout<<iL+1<<" seed_AH: "<<seed_AH<<" ,s1: "<<s1<<" ,s2: "<<s2<<" :: S1:"<<S1<<" ,S9: "<<S9<<" ,S25: "<<S25<<endl;
	//layerwise_comparison
	h_ld_energy[iL+40]->Fill(RechitsEn_AH[iL]);
	h_ld_nrechit[iL+40]->Fill(NRechits_AH[iL]);

      }
      

      ///////////////////////////////////////////////////////////////
      /////// END of lateral information calculation    /////////
      ///////////////////////////////////////////////////////////////

    }

    ///////////////////////////////////////////////////////////////
    /////// END of compartment wise calculation    /////////
    ///////////////////////////////////////////////////////////////

    
    cog_z_EE = cog_z_EE/rechitEnergySum_EE;
    cog_layer_EE = cog_layer_EE/rechitEnergySum_EE;

    cog_z_FH = cog_z_FH/rechitEnergySum_FH;
    cog_layer_FH = cog_layer_FH/rechitEnergySum_FH;
    cog_layer_FH = cog_layer_FH - 28;

    cog_z_HGCAL = cog_z_HGCAL/rechitEnergySum;
    cog_layer_HGCAL = cog_layer_HGCAL/rechitEnergySum;

    cog_layer_AH = cog_layer_AH/rechitEnergySum_AH;

    // cout<<jentry+1<<" => cog_layer_EE = "<<cog_layer_EE
    // 	<<" cog_layer_FH = "<<(cog_layer_FH)
    // 	<<" cog_layer_HGCAL = "<<cog_layer_HGCAL
    // 	<<" cog_layer_AHCAL = "<<cog_layer_AH<<endl;

    h_cog_layer_EE->Fill(cog_layer_EE);
    h_cog_layer_FH->Fill(cog_layer_FH);
    h_cog_layer_HGCAL->Fill(cog_layer_HGCAL);
    h_cog_layer_AH->Fill(cog_layer_AH);
    // cout<<jentry+1<<" => E1_inc = "<<E1_inc<<" ,E7_incl = "<<E7_inc<<" ,E19_incl = "<<E19_inc<<endl;

    if(E1_EE !=0 ) {
      h_E1_E7_EE->Fill(E1_EE/E7_EE);
      h_E1_E19_EE->Fill(E1_EE/E19_EE);
      h_E7_E19_EE->Fill(E7_EE/E19_EE);
    }
    if(E1_FH != 0) {
      h_E1_E7_FH->Fill(E1_FH/E7_FH);
      h_E1_E19_FH->Fill(E1_FH/E19_FH);
      h_E7_E19_FH->Fill(E7_FH/E19_FH);
    }
    if(E1_inc != 0) {
      h_E1_E7_HGCAL->Fill(E1_inc/E7_inc);
      h_E1_E19_HGCAL->Fill(E1_inc/E19_inc);
      h_E7_E19_HGCAL->Fill(E7_inc/E19_inc);
    }

    // cout<<jentry+1<<" => "<<(E1_AH/E8_AH)<<" : "<<(E1_AH/E16_AH)<<" : "<<(E8_AH/E16_AH)<<endl;
    // cout<<jentry+1<<" => "<<S1_AH<<" : "<<S9_AH<<" : "<<S25_AH<<endl;

    if(S1_AH !=0 && AHCAL_uniqueLayer > 25) {
      h_S1_S9_AH->Fill(S1_AH/S9_AH);
      h_S1_S25_AH->Fill(S1_AH/S25_AH);
      h_S9_S25_AH->Fill(S9_AH/S25_AH);
    }


  
    if(Nrechit_EE == 0) {
      //cout<<"ZeroEEHit : "<<rechitEnergySum_EE<<" : "<<rechitEnergySum_FH<<" : "<<rechitEnergySum_AH<<endl;
      EE_zeroHit++;
    }
    if(Nrechit_FH == 0)  {
      //cout<<jentry<<" : ZeroFHHit : "<<rechitEnergySum_EE<<" : "<<rechitEnergySum_FH<<" : "<<rechitEnergySum_AH<<endl;
      FH_zeroHit++;
    }
    if(Nrechit_AH == 0) {
      // cout<<"ZeroAHHit : "<<rechitEnergySum_EE<<" : "<<rechitEnergySum_FH<<" : "<<rechitEnergySum_AH<<endl;
      ahc_zeroHit++;
    }

    
    ////////////////////////////////////////////
    ///////// NEW SHOWER START ALGORITHM //////
    ////////////////////////////////////////////
    bool foundHadInt = false;
    int new_SS = -1;
    bool show = false;
    if(show) cout<<"***** NEW EVENT = "<<jentry<<endl;
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER;  iL++) {
      if(iL+1 == 39) {
	double R2 = En_cog_indR2[iL] + En_cog_indR2[iL+1];
        double R10 = En_cog_indR10[iL] + En_cog_indR10[iL+1];
	if(R10 > 0.0) R210_avg[iL] = R2/R10;
      }
      else if(iL+1 == 40) {
	double R2 = En_cog_indR2[iL];
        double R10 = En_cog_indR10[iL];
	if(R10 > 0.0) R210_avg[iL] = R2/R10;

      }
      else {
	double R2 = En_cog_indR2[iL] + En_cog_indR2[iL+1] + En_cog_indR2[iL+2];;
        double R10 = En_cog_indR10[iL] + En_cog_indR10[iL+1] + En_cog_indR10[iL+2];;
	if(R10 > 0.0) R210_avg[iL] = R2/R10;
      }
      
      bool cond1 = Nrec_cog_indR10[iL] > 3;
      bool cond2 = En_cog_indR10[iL] > E_thres[(int)E_beam];
      bool cond3 = R210_avg[iL] < 0.96;

      if(cond1 && cond2 && cond3 && !foundHadInt) {
	new_SS = iL+1;
	foundHadInt = true;
	if(!show) break;
	//break;
      }
      char* toShow = new char[2000];
      sprintf(toShow,"%d\t %d\t %0.2f\t %0.2f",iL+1,Nrec_cog_indR10[iL],En_cog_indR10[iL],R210_avg[iL]);
      if(show) cout<<toShow<<endl;
    }


    if(show) cout<<"Old : New :: "<< shower_start_index+1 <<" : "<< new_SS << endl;
    if(jentry > 20 && show) return;

    
    if(new_SS < 0) { shower_start_index = new_SS; shower_lambda_ = -1.0; MIP = true; }
    else { shower_start_index = new_SS - 1; shower_lambda_ = lambda[new_SS - 1]; MIP = false;}

    if(!MIP) {
      if(shower_lambda_ != lambda[shower_start_index]) cout<<">>> YOLO <<<"<<endl;
      
    }
    
    /////////////////////////////////////////////////////////////
    //////////////// END OF ALGORITHM //////////////////////////
    /////////////////////////////////////////////////////////////

    //for tree
    pi_rechit_shower_start_layer = new_SS;
    
    //Few checks
    if(shower_start_index+1 == 15) h_run_SS15->Fill(run);
    h_shower_start->Fill(shower_lambda_);
    h_shower_start_dN_dLambda->Fill(shower_lambda_,1/shower_weight_);


    if(shower_start_index+1 > 0.0 && shower_start_index+1 <= 28) {
      if(E1_EE != 0) {
	h_E1_E7_SS_EE->Fill(E1_EE/E7_EE);
	h_E1_E19_SS_EE->Fill(E1_EE/E19_EE);
	h_E7_E19_SS_EE->Fill(E7_EE/E19_EE);
      }
    }
    else if(shower_start_index+1 > 28) {
      if(E1_EE != 0) {
	h_E1_E7_MIPs_in_EE->Fill(E1_EE/E7_EE);
	h_E1_E19_MIPs_in_EE->Fill(E1_EE/E19_EE);
	h_E7_E19_MIPs_in_EE->Fill(E7_EE/E19_EE);
      }
      if(E1_FH != 0) {
	h_E1_E7_SS_FH->Fill(E1_FH/E7_FH);
	h_E1_E19_SS_FH->Fill(E1_FH/E19_FH);
	h_E7_E19_SS_FH->Fill(E7_FH/E19_FH);
      }
    }
    else {
      if(E1_FH != 0) {
	h_E1_E7_MIPs_in_FH->Fill(E1_FH/E7_FH);
	h_E1_E19_MIPs_in_FH->Fill(E1_FH/E19_FH);
	h_E7_E19_MIPs_in_FH->Fill(E7_FH/E19_FH);
      }
    }


    
    ///////////////////////////////////////////////
    //                Collapsed EE               //
    ///////////////////////////////////////////////
    if(shower_start_index+1 <= 28 && (shower_start_index+1) > 0) {
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


    //// Minor debug for traxkImpact vs rechitPosition for EE03
    if(shower_start_index > 8) {
      // cout<<"Hello => "<<rechitX_EE03.size()<<endl;
      for(int i = 0; i < rechitX_EE03.size(); i++) {

    	double trackX = track_x[2];
    	double trackY = track_y[2];

    	double trackX_corr = -1.0*track_x[2];
    	double trackY_corr = -1.0*track_y[2];

    	double trackX_full_corr = trackX_corr + dxy_alignment(3).first;
    	double trackY_full_corr = trackY_corr + dxy_alignment(3).second;
     
    	double recX = rechitX_EE03.at(i);
    	double recY = rechitY_EE03.at(i);

    	double recX_corr = rechitX_EE03_corrected.at(i);
    	double recY_corr = rechitY_EE03_corrected.at(i);

    	h_trackX_trackY_EE03->Fill(trackX,trackY);
    	h_trackX_trackY_EE03_corrected->Fill(trackX_corr,trackY_corr);
    	h_trackX_trackY_EE03_FULL_corrected->Fill(trackX_full_corr,trackY_full_corr);
      
    	h_trackX_EE03_FULL_corrected->Fill(trackX_full_corr);
    	h_trackY_EE03_FULL_corrected->Fill(trackY_full_corr);

    	h_trackX_EE03_track_corrected->Fill(trackX_corr);
    	h_trackY_EE03_track_corrected->Fill(trackY_corr);

    	h_rechitX_EE03->Fill(recX);
    	h_rechitY_EE03->Fill(recY);

    	h_rechitX_rechitY_EE03->Fill(recX,recY);
   	h_rechitX_rechitY_EE03_En_weighted->Fill(recX,recY,rechitEnergy_EE03.at(i));

    	h_rechitX_rechitY_EE03_corrected->Fill(recX_corr,recY_corr);

    	h_trackX_rechitX_EE03->Fill(trackX,recX);
    	h_trackX_rechitX_EE03_track_corrected->Fill(trackX_corr,recX);
    	h_trackX_rechitX_EE03_track_rechit_corrected->Fill(trackX_corr,recX_corr);

    	h_trackY_rechitY_EE03->Fill(trackY,recY);
    	h_trackY_rechitY_EE03_track_corrected->Fill(trackY_corr,recY);
    	h_trackY_rechitY_EE03_track_rechit_corrected->Fill(trackY_corr,recY_corr);

    	h_dX_dY_EE03->Fill((trackX-recX),(trackY-recY));
    	h_dX_dY_EE03_after_full_correction->Fill((trackX_corr-recX_corr),(trackY_corr-recY_corr));

      }
    }
    ///////////////////////////////////////////////


    //// Minor debug for traxkImpact vs rechitPosition for EE15
    if(shower_start_index > 20) {
      // cout<<"Hello => "<<rechitX_EE15.size()<<endl;
      for(int i = 0; i < rechitX_EE15.size(); i++) {

    	double trackX = track_x[14];
    	double trackY = track_y[14];

    	double trackX_corr = -1.0*track_x[14];
    	double trackY_corr = -1.0*track_y[14];

    	double trackX_full_corr = trackX_corr + dxy_alignment(15).first;
    	double trackY_full_corr = trackY_corr + dxy_alignment(15).second;
     
    	double recX = rechitX_EE15.at(i);
    	double recY = rechitY_EE15.at(i);

    	double recX_corr = rechitX_EE15_corrected.at(i);
    	double recY_corr = rechitY_EE15_corrected.at(i);

    	h_trackX_trackY_EE15->Fill(trackX,trackY);
    	h_trackX_trackY_EE15_corrected->Fill(trackX_corr,trackY_corr);
    	h_trackX_trackY_EE15_FULL_corrected->Fill(trackX_full_corr,trackY_full_corr);
      
    	h_trackX_EE15_FULL_corrected->Fill(trackX_full_corr);
    	h_trackY_EE15_FULL_corrected->Fill(trackY_full_corr);

    	h_trackX_EE15_track_corrected->Fill(trackX_corr);
    	h_trackY_EE15_track_corrected->Fill(trackY_corr);

    	h_rechitX_EE15->Fill(recX);
    	h_rechitY_EE15->Fill(recY);

    	h_rechitX_rechitY_EE15->Fill(recX,recY);
    	h_rechitX_rechitY_EE15_En_weighted->Fill(recX,recY,rechitEnergy_EE15.at(i));

    	h_rechitX_rechitY_EE15_corrected->Fill(recX_corr,recY_corr);

    	h_trackX_rechitX_EE15->Fill(trackX,recX);
    	h_trackX_rechitX_EE15_track_corrected->Fill(trackX_corr,recX);
    	h_trackX_rechitX_EE15_track_rechit_corrected->Fill(trackX_corr,recX_corr);

    	h_trackY_rechitY_EE15->Fill(trackY,recY);
    	h_trackY_rechitY_EE15_track_corrected->Fill(trackY_corr,recY);
    	h_trackY_rechitY_EE15_track_rechit_corrected->Fill(trackY_corr,recY_corr);

    	h_dX_dY_EE15->Fill((trackX-recX),(trackY-recY));
    	h_dX_dY_EE15_after_full_correction->Fill((trackX_corr-recX_corr),(trackY_corr-recY_corr));

      }
    }



    ///////////////////////////////////////////////


    //// Minor debug for traxkImpact vs rechitPosition for FH10
    if(shower_start_index < 0) {
      // cout<<"Hello => "<<rechitX_FH10.size()<<endl;
      for(int i = 0; i < rechitX_FH10.size(); i++) {

    	double trackX = track_x[37];
    	double trackY = track_y[37];

    	double trackX_corr = -1.0*track_x[37];
    	double trackY_corr = -1.0*track_y[37];

    	double trackX_full_corr = trackX_corr + dxy_alignment(38).first;
    	double trackY_full_corr = trackY_corr + dxy_alignment(38).second;
     
    	double recX = rechitX_FH10.at(i);
    	double recY = rechitY_FH10.at(i);

    	double recX_corr = rechitX_FH10_corrected.at(i);
    	double recY_corr = rechitY_FH10_corrected.at(i);

    	h_trackX_trackY_FH10->Fill(trackX,trackY);
    	h_trackX_trackY_FH10_corrected->Fill(trackX_corr,trackY_corr);
    	h_trackX_trackY_FH10_FULL_corrected->Fill(trackX_full_corr,trackY_full_corr);
      
    	h_trackX_FH10_FULL_corrected->Fill(trackX_full_corr);
    	h_trackY_FH10_FULL_corrected->Fill(trackY_full_corr);

    	h_trackX_FH10_track_corrected->Fill(trackX_corr);
    	h_trackY_FH10_track_corrected->Fill(trackY_corr);

    	h_rechitX_FH10->Fill(recX);
    	h_rechitY_FH10->Fill(recY);

    	h_rechitX_rechitY_FH10->Fill(recX,recY);
    	// h_rechitX_rechitY_FH10_En_weighted->Fill(recX,recY,rechitEnergy_FH10.at(i));

    	h_rechitX_rechitY_FH10_corrected->Fill(recX_corr,recY_corr);

    	h_trackX_rechitX_FH10->Fill(trackX,recX);
    	h_trackX_rechitX_FH10_track_corrected->Fill(trackX_corr,recX);
    	h_trackX_rechitX_FH10_track_rechit_corrected->Fill(trackX_corr,recX_corr);

    	h_trackY_rechitY_FH10->Fill(trackY,recY);
    	h_trackY_rechitY_FH10_track_corrected->Fill(trackY_corr,recY);
    	h_trackY_rechitY_FH10_track_rechit_corrected->Fill(trackY_corr,recY_corr);

    	h_dX_dY_FH10->Fill((trackX-recX),(trackY-recY));
    	h_dX_dY_FH10_after_full_correction->Fill((trackX_corr-recX_corr),(trackY_corr-recY_corr));

      }
    }



    ///////////////////////////////////////////////



    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////




    ////////////////////////////////////////////////////////////
    ////           Longitudinal shower profile              ////
    ///////////////////////////////////////////////////////////


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
    case 300: beta_ = 5.253; break;
    }
    if(!DoWeight) beta_ = 1.0;

    
    int summed_up_last_layer[5] = {4, 10, 16, 22, 28};
    float relative_weighted_energy = rechitEnergySum_EE + beta_*rechitEnergySum_FH;
    float absolute_energy = rechitEnergySum_EE + rechitEnergySum_FH;
    // if(jentry < 1)
    //   cout<<"Layer \t lambda \t delta_lambda \t weight "<<endl;

    // MUON VETO
    if(S1_AH/S25_AH > 0.80 && (rechitEnergySum_EE < 100) && (rechitEnergySum_FH < 60)) {
      MIP = true;

      //for tree
      pi_MuonVeto = true;
    }

    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++) {
      if(MIP) continue;
      
      if(shower_start_index+1 <= 28) { // Showering in CE-E
	if(iL < EE_LAYER) {
	  float EE_energy = RechitsEn_EE[iL];
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],EE_energy);
	  h_longi_profile_ShowerInEE->Fill(lambda[iL],EE_energy);
	}
	else {
	  float FH_energy = RechitsEn_FH[iL-EE_LAYER];
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],FH_energy);
	  h_longi_profile_ShowerInEE->Fill(lambda[iL],FH_energy);
	}
	if(iL < 39) {
	  float AH_energy = RechitsEn_AH[iL];
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_ShowerInEE->Fill(lambda[iL+40],AH_energy);
	  
	}
      }
      else { // MIPs in CE-E
	// if(shower_start_index+1 <= 30) continue;
	if(iL < EE_LAYER) {
	  float EE_energy = RechitsEn_EE[iL];
	  // h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],EE_energy);
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL]-shower_lambda_,EE_energy);
	  h_longi_profile_MipsInEE->Fill(lambda[iL],EE_energy);
	  // h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],EE_energy);
	  h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL-shower_start_index],EE_energy);


	}
	else {
	  float FH_energy = RechitsEn_FH[iL-EE_LAYER];
	  //h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],FH_energy);
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL]-shower_lambda_,FH_energy);
	  h_longi_profile_MipsInEE->Fill(lambda[iL],FH_energy);
	  // h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],FH_energy);
	  h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL-shower_start_index],FH_energy);

	}
	if(iL < 39) {
	  float AH_energy = RechitsEn_AH[iL];
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL+40]-shower_lambda_,AH_energy);
	  // h_longi_profile_raw[shower_start_index]->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_MipsInEE->Fill(lambda[iL+40],AH_energy);
	  // h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL+40]-lambda[shower_start_index],AH_energy);
	  h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL+40-shower_start_index],AH_energy);
	  
	}
      }
    }

    
    // for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){
    //   // float del_lamb_weight = weights_[iL];
    //   float del_lamb_weight = 1.0;
    //   // if(jentry < 1) {
    //   // 	cout<<iL+1<<" \t "<<lambda[iL]<<" \t "<<"xx"<<" \t "<<del_lamb_weight<<endl;
    //   // }
    //   float to_fill = 0.0;
    //   float frac_ = 0.0;
    //   float abs_frac_ = 0.0;
    //   if(iL < EE_LAYER) {
    // 	h_longi_profile_inclusive->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
    // 	h_longi_profile_inclusive_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
    // 	frac_ = RechitsEn_EE[iL]/relative_weighted_energy;
    // 	abs_frac_ = RechitsEn_EE[iL]/absolute_energy;
    // 	if(!MIP) {

	  
    // 	  //h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
    // 	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],frac_);
    // 	  if(shower_start_index+1 < 29) {
    // 	    h_longi_profile_ShowerInEE->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
    // 	    h_longi_profile_ShowerInEE_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
    // 	    switch(iL+1) {
    // 	    case 4:  for(int jj=0;jj<4;jj++) {to_fill+=RechitsEn_EE[jj];}
    // 	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
    // 	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
    // 	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

    // 	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
    // 	    case 10:  for(int jj=4;jj<10;jj++) {to_fill+=RechitsEn_EE[jj];}
    // 	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
    // 	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
    // 	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

    // 	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
    // 	    case 16:  for(int jj=10;jj<16;jj++) {to_fill+=RechitsEn_EE[jj];}
    // 	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
    // 	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
    // 	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

    // 	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
    // 	    case 22:  for(int jj=17;jj<22;jj++) {to_fill+=RechitsEn_EE[jj];}
    // 	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
    // 	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
    // 	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);
		      
    // 	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
    // 	    case 28:  for(int jj=23;jj<28;jj++) {to_fill+=RechitsEn_EE[jj];}
    // 	      if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
    // 	      if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],to_fill);
    // 	      h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],to_fill);

    // 	      h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],to_fill); break;
    // 	    default:
    // 	      break;
    // 	    }

    // 	    h_longi_profile_ShowerInEE_fractionalE->Fill(lambda[iL],frac_);
    // 	    h_longi_profile_ShowerInEE_abs_fractionalE->Fill(lambda[iL],abs_frac_);
    // 	    //if(shower_start_index+1 == 15) h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],to_fill);
	    
    // 	  }
    // 	  else {
    // 	    // cout<<jentry<<" == yolo"<<endl;
    // 	    h_longi_profile_MipsInEE->Fill(lambda[iL],RechitsEn_EE[iL]*del_lamb_weight);
    // 	    h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],RechitsEn_EE[iL]*del_lamb_weight);
    // 	    h_longi_2D_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],RechitsEn_EE[iL]*del_lamb_weight);

    // 	    h_longi_profile_MipsInEE_noWeight->Fill(lambda[iL],RechitsEn_EE[iL]);
    // 	    h_longi_profile_MipsInEE_fractionalE->Fill(lambda[iL],frac_);
    // 	    h_longi_profile_MipsInEE_abs_fractionalE->Fill(lambda[iL],abs_frac_);

    // 	  }
    // 	  h_Rechits_nrec_SS[shower_start_index][iL]->Fill(NRechits_EE[iL]);
    // 	  h_Rechits_En_SS[shower_start_index][iL]->Fill(RechitsEn_EE[iL]);
    // 	}
    //   }
    //   else {
    // 	frac_ = (beta_*RechitsEn_FH[iL-EE_LAYER])/relative_weighted_energy;
    // 	abs_frac_ = RechitsEn_FH[iL-EE_LAYER]/absolute_energy;
    // 	//h_longi_profile_inclusive->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
    // 	h_longi_profile_inclusive->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
    // 	//h_longi_profile_inclusive_noWeight->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
    // 	h_longi_profile_inclusive_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
    // 	if(!MIP) {
    // 	  //h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER],del_lamb_weight);
    // 	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],frac_);
    // 	  if(shower_start_index+1 < 29) {
    // 	    h_longi_profile_ShowerInEE->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
    // 	    h_longi_profile_ShowerInEE_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
    // 	    h_longi_profile_ShowerInEE_summed_up_EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
    // 	    h_longi_profile_ShowerInEE_fractionalE->Fill(lambda[iL],frac_);
    // 	    h_longi_profile_ShowerInEE_abs_fractionalE->Fill(lambda[iL],abs_frac_);
    // 	    if(shower_start_index+1 == 15)
    // 	      h_longi_profile_ShowerInEE_summed_up_15EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);
    // 	    if(shower_start_index+1 == 22) h_longi_profile_ShowerInEE_summed_up_22EE->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);

    // 	    h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);

    // 	  }
    // 	  else {
    // 	    h_longi_profile_MipsInEE->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);

    // 	    h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);
    // 	    h_longi_2D_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],RechitsEn_FH[iL-EE_LAYER]*del_lamb_weight);

    // 	    h_longi_profile_MipsInEE_noWeight->Fill(lambda[iL],RechitsEn_FH[iL-EE_LAYER]);
    // 	    h_longi_profile_MipsInEE_fractionalE->Fill(lambda[iL],frac_);
    // 	    h_longi_profile_MipsInEE_abs_fractionalE->Fill(lambda[iL],abs_frac_);
    // 	    h_longi_profile_ShowerInEE_summed_up_[shower_start_index]->Fill(lambda[iL],beta_*RechitsEn_FH[iL-EE_LAYER]);


    // 	  }
    // 	 h_Rechits_nrec_SS[shower_start_index][iL]->Fill(NRechits_FH[iL-EE_LAYER]);
    // 	 h_Rechits_En_SS[shower_start_index][iL]->Fill(RechitsEn_FH[iL-EE_LAYER]);
    // 	}
    //   }
      
    // }

    ////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////

    // Muon veto check
    if(!(S1_AH/S25_AH > 0.80 && (rechitEnergySum_EE < 100) && (rechitEnergySum_FH < 60))) event_count[3]++;

   
    double isMuonLike = false;
    bool isRegion1 = false; //MIP like
    bool isRegion2 = false; //showering in CE-H
    bool isRegion3 = false;  //showering in CE-E
    bool isRegion4 = false; //showering in AHCAL

    ///////////////////////////////////////////////////////////
    //// E V E N T     C A T E G O R I Z A T I O N       /////
    ///////////////////////////////////////////////////////////


    if(UseShowerStartAlgo && !UseCompartmentEnergy)  {
      /////// BASED ON SHOWER START
      //MUON VETO
      if(S1_AH/S25_AH > 0.80 && (rechitEnergySum_EE < 100) && (rechitEnergySum_FH < 60)) {
	isRegion1 = true;
	isRegion2 = false;
	isRegion3 = false;
      }
      if(shower_start_index == -1) {  //MIP LIKE
	//if(shower_start_index < 2) {  //MIP LIKE
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
      //MUON VETO
      if(S1_AH/S25_AH > 0.80 && (rechitEnergySum_EE < 100) && (rechitEnergySum_FH < 60)) {
	isRegion1 = true;
	isRegion2 = false;
	isRegion3 = false;
	isRegion4 = false;

      }
      else if(rechitEnergySum_EE > 100) {
	isRegion1 = false;
	isRegion2 = false;
	isRegion3 = true;
	isRegion4 = false;
	
	if(E1_EE != 0) {
	  h_E1_E7_SS_EE->Fill(E1_EE/E7_EE);
	  h_E1_E19_SS_EE->Fill(E1_EE/E19_EE);
	  h_E7_E19_SS_EE->Fill(E7_EE/E19_EE);
	}
	

      }
      else if(rechitEnergySum_FH > 60) {
	isRegion1 = false;
	isRegion2 = true;
	isRegion3 = false;
	isRegion4 = false;

	if(E1_EE != 0) {
	  h_E1_E7_MIPs_in_EE->Fill(E1_EE/E7_EE);
	  h_E1_E19_MIPs_in_EE->Fill(E1_EE/E19_EE);
	  h_E7_E19_MIPs_in_EE->Fill(E7_EE/E19_EE);
	}
	if(E1_FH != 0) {
	  h_E1_E7_SS_FH->Fill(E1_FH/E7_FH);
	  h_E1_E19_SS_FH->Fill(E1_FH/E19_FH);
	  h_E7_E19_SS_FH->Fill(E7_FH/E19_FH);
	}
	

      }
      else {
	if(rechitEnergySum_AH < 60)  {
	  isRegion1 = true;
	  isRegion2 = false;
	  isRegion3 = false;	 
	  isRegion4 = false;
	}
	else  {
	  isRegion1 = false;
	  isRegion2 = false;
	  isRegion3 = false;	 
	  isRegion4 = true;

	  h_rechit_energy_raw_AH_shower->Fill(rechitEnergySum_AH);
	  if(E1_FH != 0) {
	    h_E1_E7_MIPs_in_FH->Fill(E1_FH/E7_FH);
	    h_E1_E19_MIPs_in_FH->Fill(E1_FH/E19_FH);
	    h_E7_E19_MIPs_in_FH->Fill(E7_FH/E19_FH);
	  }
	  if(S1_AH != 0) {

	    h_S1_S9_AH_debug->Fill(S1_AH/S9_AH);
	    h_S1_S25_AH_debug->Fill(S1_AH/S25_AH);
	    h_S9_S25_AH_debug->Fill(S9_AH/S25_AH);
	  }
	  h_rechit_energy_raw_AH_shower_low_extended->Fill(rechitEnergySum_AH);
	  //if(rechitEnergySum_AH < 200) {
	  // if((S1_AH/S9_AH) > 0.8) {
	  //   cout<<"/EventDisplay/ntuple/showEvent "<<jentry<<endl;
	  //   cout<<rechitEnergySum_EE<<":"<<rechitEnergySum_FH
	  // 	<<":"<<rechitEnergySum_AH<<":"
	  // 	<<AHCAL_uniqueLayer<<":"
	  // 	<<(S1_AH/S9_AH)<<endl;
	  // }

	}
      }


    }
    else {
      std::cout<<BOLDRED<<"No Switch selected for Event Categorization"<<RESET<<std::endl;
      return;
    
    }



    //Fill for correlation plot
    int En_index = -1.0;
    int SS_index = -1.0;
    En_index = (int)(isRegion1 || isRegion4) + (int)isRegion2 + (int)isRegion3;

    if(isRegion1 || isRegion4) {
      En_index = 1;
    }
    else if(isRegion2){
      En_index = 2;
    }
    else if(isRegion3) {
      En_index = 3;
    }
    else {
      cout<<"jentry "<<jentry<<" region not found"<<endl;
      return;
    }

    if(shower_start_index == -1) {
      SS_index = 1;
    }
    else if(shower_start_index+1 >= 1 && shower_start_index+1 <= 28) {
      SS_index = 3;
    }
    else if(shower_start_index+1 >=29 && shower_start_index+1 <= 40) {
      SS_index = 2;
    }
    else {
      cout<<"jentry : shower_start_index :: "<<jentry<<" : "<<shower_start_index<<endl;
      return;
    }
    if(jentry < 20 && false)  { 
      cout<<isRegion1<<" : "<<isRegion2<<" : "<<isRegion3<<" : "<<isRegion4<<endl;
      cout<<jentry<<" : "<<SS_index<<" : "<<En_index<<endl;
    }
    h_2D_corr_SS_En->Fill(SS_index,En_index);


    h_Nrechit_EE->Fill(Nrechit_EE);
    h_Nrechit_FH->Fill(Nrechit_FH);
    h_Nrechit_AH->Fill(ahc_nHits);

    h_Nrechit_low_EE->Fill(Nrechit_EE);
    h_Nrechit_low_FH->Fill(Nrechit_FH);
    h_Nrechit_low_AH->Fill(ahc_nHits);

    
    h_rechit_energy_raw_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_AH->Fill(rechitEnergySum_AH);
    h_rechit_energy_raw_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);


    h_rechit_energy_raw_low_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_low_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_low_AH->Fill(rechitEnergySum_AH);
    h_rechit_energy_raw_low_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

    h_rechit_energy_raw_verylow_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_verylow_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_verylow_AH->Fill(rechitEnergySum_AH);
    h_rechit_energy_raw_verylow_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

    h_rechit_energy_raw_mid_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_mid_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_mid_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

    h_rechit_energy_raw_high_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_high_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_high_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);


    h_Nrechit_EE_vs_FH->Fill(Nrechit_EE,Nrechit_FH);
    


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


    // alpha = getWeights(50).at(0);
    // beta = getWeights(50).at(1);
    // gamma_ee = getWeights(50).at(2);
    // gamma_fh = getWeights(50).at(3);

    h_rechit_energy_frac_EE_last3_all->Fill(EE_last3/rechitEnergySum_EE);
    h_rechit_energy_frac_FH_last3_all->Fill(FH_last3/rechitEnergySum_FH); h_rechit_energy_frac_AH_last3_all->Fill(AH_last3/rechitEnergySum_AH);


    // MIPs in CE-E and MIPs in CE-H
    if(isRegion1) {
      h_shower_start_reg1->Fill(shower_lambda_);
      region_1_classified_events++; 
      
      h_rechit_energy_raw_EE_MipsInEEFH->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEEFH->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_EE_vs_FH_MipsInEEFH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_MipsInEEFH->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInEEFH->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_EE_MipsInEEFH_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEEFH_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEEFH_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInEEFH_extended_v1->Fill(rechitEnergySum_AH);

      h_rechit_energy_raw_all_MipsInEEFH->Fill(rechitEnergySum_EE+rechitEnergySum_FH);
      
      h_rechit_energy_raw_EE_MipsInFH_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInFH_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInFH_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInFH_extended_v1->Fill(rechitEnergySum_AH);
      
      h_rechit_energy_frac_EE_last3_MipsInEEFH->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_MipsInEEFH->Fill(FH_last3/rechitEnergySum_FH);
      h_rechit_energy_frac_AH_last3_MipsInEEFH->Fill(AH_last3/rechitEnergySum_AH);
      
      
    }
    


    // MIPs in CE-E
    if(isRegion2) {
      h_shower_start_reg2->Fill(shower_lambda_);
      region_2_classified_events++; 
      
      h_rechit_energy_frac_EE_last3_MipsInEE->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_MipsInEE->Fill(FH_last3/rechitEnergySum_FH); 
      h_rechit_energy_frac_AH_last3_MipsInEE->Fill(AH_last3/rechitEnergySum_AH);

      if(Nrechit_EE == 0) EE_zeroHit_R2++;
      
    
      h_rechit_energy_raw_FH_MipsInEE_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_FH_MipsInEE_extended_v1->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEE_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInEE_extended_v1->Fill(rechitEnergySum_AH);

      h_Nrechits_EE_FH_FB->Fill(Nrechit_EE,Nrechit_FH);
      h_rechit_energy_inEE_MipsInEE_mips->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_EE_vs_FH_MipsInEE->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_MipsInEE->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      h_rechit_energy_raw_EE_MipsInEE->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_EE_MipsInEE_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEE->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEE->Fill(rechitEnergySum_AH);
      
      h_rechit_energy_raw_all_MipsInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

      //////////////////////////////////////////////////////////////////////////////////////
      ///////////////   Based on Electron/pion scale in CE-E & CE-H-Si //////////////////////
      //////////////////////////////////////////////////////////////////////////////////////

      float FH_AH_relative_scale = 0.4;
      float alpha_ = FH_AH_relative_scale;
      float EE_scale = 94.624; //MIPs per GeV
      // float FH_AH_scale = 12.788; //MIPs per GeV
      float FH_AH_scale = 12.66; //MIPs per GeV


      float total_energy = (rechitEnergySum_FH+ (alpha_ * rechitEnergySum_AH))/FH_AH_scale;
      float total_energy_withoutAH = (rechitEnergySum_FH)/FH_AH_scale;
      

      h_EE_MIPsinEE->Fill(rechitEnergySum_EE);
      h_FH_MIPsinEE->Fill(rechitEnergySum_FH);
      h_AH_MIPsinEE->Fill(rechitEnergySum_AH);
      h_all_MIPsinEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySum_AH);

      h_MIPsinEE_elecpi_scale->Fill(total_energy);
      h_MIPsinEE_elecpi_scale_withoutAH->Fill(total_energy_withoutAH);
      
      

      //////////////////////////////////////////////////////////////////////////////////////

      // gamma_noAH = 11.06;

      // double tot_ = beamEnergy;
      // h_rechit_energy_FB_FH_alone_raw->Fill(rechitEnergySum_FH);

      // double MIP_to_GeV_conversion_factor = gamma_fh;
      // double FH_Gev_Scale = (rechitEnergySum_FH/MIP_to_GeV_conversion_factor);
      // double AH_Gev_Scale = (rechitEnergySum_AH/MIP_to_GeV_conversion_factor);
      // double full_energy = FH_Gev_Scale+(alpha*AH_Gev_Scale);
      // double wo_ahcal = FH_Gev_Scale;
      // double wo_ahcal_noAH = (rechitEnergySum_FH/gamma_noAH);
      // h_rechit_energy_FH_FB_weighted->Fill(FH_Gev_Scale);

      // h_rechit_energy_FH_AHCAL_FB_raw->Fill(rechitEnergySum_FH, rechitEnergySum_AH);
      // h_rechit_energy_FH_AHCAL_FB_weighted->Fill(FH_Gev_Scale,(alpha*AH_Gev_Scale));
      // h_energy_MipsInEE_gev->Fill(full_energy);
      // h_energy_MipsInEE_mips->Fill(rechitEnergySum_FH+alpha*rechitEnergySum_AH);
      // h_energy_all_gev->Fill(full_energy);
      // // h_energy_all_gev_withoutAHCAL->Fill(wo_ahcal);
      // // h_energy_MipsInEE_gev_withoutAHCAL->Fill(wo_ahcal);
      // h_energy_all_gev_withoutAHCAL->Fill(wo_ahcal_noAH);
      // h_energy_MipsInEE_gev_withoutAHCAL->Fill(wo_ahcal_noAH);

      // h_rechit_energy_full_withEE_weighted_MipsInEE->Fill(rechitEnergySum_EE+ 4.9*(rechitEnergySum_FH+0.40*rechitEnergySum_AH));
      // h_rechit_energy_full_withoutEE_weighted_MipsInEE->Fill((rechitEnergySum_FH+0.40*rechitEnergySum_AH));
      // h_rechit_energy_raw->Fill(rechitEnergySum_FH+rechitEnergySum_AH);
      // h_rechit_energy_weighted->Fill(rechitEnergySum_FH+(0.4*rechitEnergySum_AH));


      // if(shower_start_index+1 >= 29 && shower_start_index+1 <= 30) {
      // 	h_compart5->Fill(full_energy);
      // }
      // else if (shower_start_index+1 > 30) {
      // 	h_compart6->Fill(full_energy);
      // }


      // float rel_weight = 0.02;
      // float step_ = rel_weight;
      // for(int ii = 0; ii < 50; ii++) {
      // 	rel_weight = step_*(ii+1);
      // 	//h_rechit_energy_FB_rel_weightScan[ii]->Fill(FH_Gev_Scale + (rel_weight*AH_Gev_Scale));
      // 	h_rechit_energy_FB_rel_weightScan[ii]->Fill(rechitEnergySum_FH + (rel_weight*rechitEnergySum_AH));
      // 	// h_rechit_energy_FB_rel_weightScan[ii]->Fill(rechitEnergySum_FH);
      // }
      



      bool trackwindow = !((trackX_al_3 < track_cut.at(0) || trackX_al_3 > track_cut.at(1) || trackY_al_3 < track_cut.at(2) || trackY_al_3 > track_cut.at(3)));
      bool SS1_reject = (shower_start_index+1 > 1);
      bool SS2_reject = (shower_start_index+1 > 2);



      ///////////////////////////////////////////////////////////
      /////     H hadrons ; Chi2 matrix initialzation     /////
      //////////////////////////////////////////////////////////

      double FH_detscale = rechitEnergySum_FH/FH_AH_scale;
      double AH_detscale = (alpha_ * rechitEnergySum_AH)/FH_AH_scale;

      //double O = 2.5;
      double O = 0.0;

      float scale_ = scaling_factor_H[(int)beamEnergy];

      // float w1 = getChi2Weights_H(beamEnergy).at(0);
      // float w2 = getChi2Weights_H(beamEnergy).at(1);
      // float w3 = getChi2Weights_H(beamEnergy).at(2);

      float w1 = f_H_w1->Eval(beamEnergy);
      float w2 = f_H_w2->Eval(beamEnergy);
      float w3 = f_H_w3->Eval(beamEnergy);

      float recoE_w1 = f_H_w1->Eval(total_energy);
      float recoE_w2 = f_H_w2->Eval(total_energy);
      float recoE_w3 = f_H_w3->Eval(total_energy);

      float chi2_energy_recoE = recoE_w1*1.0 + recoE_w2*FH_detscale + recoE_w3*AH_detscale + 0.4;
      
      // if((int)beamEnergy == 20) {w1= 0.0; w2=1.015 ; w3=1.22588 ; }
      float chi2_energy = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH + w3*rechitEnergySum_AH + O;
      float chi2_energy_withoutAHCAL = w2*rechitEnergySum_FH;

      bool IsIn_en_range = true;

      if(chi2_method > 0)  {
	if(chi2_method > 1) O = 0.4;
	if(chi2_method > 2) IsIn_en_range = (total_energy > en_range_FH_xmin[(int)beamEnergy] && total_energy < en_range_FH_xmax[(int)beamEnergy]);
	chi2_energy = w1*1.0 + w2*FH_detscale + w3*AH_detscale + O;
	chi2_energy_withoutAHCAL = w1*1.0 + w2*FH_detscale + O;
      }

      h_energy_MipsInEE_gev_chi2->Fill(chi2_energy);
      h_energy_MipsInEE_gev_chi2_rescaled->Fill(scale_ * chi2_energy);

      h_energy_MipsInEE_gev_chi2_withoutAHCAL->Fill(chi2_energy_withoutAHCAL);

      

      //double sigma2_H = (0.089*0.089) + (1.16*1.16)*full_energy;
      double sigma2_H = (0.088*0.088) + (1.25*1.25)*total_energy;


      if(trackwindow && SS2_reject && IsIn_en_range) {

	if(chi2_method == 0) {
	  coeffs_H(0,0) += (rechitEnergySum_FH*rechitEnergySum_FH)/sigma2_H;
	  coeffs_H(0,1) += (rechitEnergySum_AH*rechitEnergySum_FH)/sigma2_H;
	  coeffs_H(1,0) += (rechitEnergySum_FH*rechitEnergySum_AH)/sigma2_H;
	  coeffs_H(1,1) += (rechitEnergySum_AH*rechitEnergySum_AH)/sigma2_H;
	
	  consts_H(0)   += (E_beam-O)*rechitEnergySum_FH/sigma2_H;
	  consts_H(1)   += (E_beam-O)*rechitEnergySum_AH/sigma2_H;
	}
	else {
	  coeffs_H(0,0) += (FH_detscale*FH_detscale)/sigma2_H;
	  coeffs_H(0,1) += (AH_detscale*FH_detscale)/sigma2_H;
	  coeffs_H(1,0) += (FH_detscale*AH_detscale)/sigma2_H;
	  coeffs_H(1,1) += (AH_detscale*AH_detscale)/sigma2_H;
	
	  consts_H(0)   += (E_beam-O)*FH_detscale/sigma2_H;
	  consts_H(1)   += (E_beam-O)*AH_detscale/sigma2_H;
	}

      }


      //////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////


      /////////////////////////////////////////////////////
      //////////////// selection cut check  ////////////////
      /////////////////////////////////////////////////////

      
      h_baseline_FH->Fill(total_energy);
      h_baseline_chi2_FH->Fill(chi2_energy);

      if(trackwindow)  {
	h_trackwindow_FH->Fill(total_energy);
	h_trackwindow_chi2_FH->Fill(chi2_energy);
	h_trackwindow_chi2_FH_recoE->Fill(chi2_energy_recoE);
	
      }
      if(SS1_reject) {
	h_SS1_reject_FH->Fill(total_energy);
      }
      if(SS2_reject) {
	h_SS2_reject_FH->Fill(total_energy);
      }
      if(SS1_reject && trackwindow) {
	h_trackwindow_SS1_reject_FH->Fill(total_energy);
      }
      if(SS2_reject && trackwindow) {
	h_trackwindow_SS2_reject_FH->Fill(total_energy);
	
	
      }


      
	
	
      

      /////////////////////////////////////////////////////
      /////////////////////////////////////////////////////




    }
    
    
    //Showering in CE-E
    
    if(isRegion3) {
      h_shower_start_reg3->Fill(shower_lambda_);
      region_3_classified_events++; 
      
      h_rechit_energy_frac_EE_last3_ShowerInEE->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_ShowerInEE->Fill(FH_last3/rechitEnergySum_FH); 
      h_rechit_energy_frac_AH_last3_ShowerInEE->Fill(AH_last3/rechitEnergySum_AH);


      //////////////////////////////////////////////////////////////////////////////////////
      ///////////////   Based on Electron/pion scale in CE-E & CE-H-Si //////////////////////
      //////////////////////////////////////////////////////////////////////////////////////

      float FH_AH_relative_scale = 0.4;
      float alpha_ = FH_AH_relative_scale;
      float EE_scale = 94.624; //MIPs per GeV
      // float FH_AH_scale = 12.788; //MIPs per GeV
      float FH_AH_scale = 12.66; //MIPs per GeV

      float total_energy = (rechitEnergySum_EE/EE_scale) + (rechitEnergySum_FH+ (alpha_ * rechitEnergySum_AH))/FH_AH_scale;
      float total_energy_withoutAH = (rechitEnergySum_EE/EE_scale) + (rechitEnergySum_FH)/FH_AH_scale;
      

      h_EE_showerinEE->Fill(rechitEnergySum_EE);
      h_FH_showerinEE->Fill(rechitEnergySum_FH);
      h_AH_showerinEE->Fill(rechitEnergySum_AH);
      h_all_showerinEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySum_AH);

      h_showerinEE_elecpi_scale->Fill(total_energy);
      h_showerinEE_elecpi_scale_withoutAH->Fill(total_energy_withoutAH);
      
      

      //////////////////////////////////////////////////////////////////////////////////////

     


      h_rechit_energy_raw_EE_ShowerInEE->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_ShowerInEE->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_EE_vs_FH_ShowerInEE->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_ShowerInEE->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      h_rechit_energy_raw_AH_ShowerInEE->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_all_ShowerInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySum_AH);


      // beta_noAH = 4.872 ; gamma_noAH = 56.535;
      // double MIP_to_GeV = gamma_ee;
      // double tot_E_MIPs = rechitEnergySum_EE + beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH));
      // double EE_FH_MIPs = rechitEnergySum_EE + beta*(rechitEnergySum_FH);
      // double tot_E_gev = tot_E_MIPs/MIP_to_GeV;
      // double EE_FH_gev = EE_FH_MIPs/MIP_to_GeV;
      // double EE_FH_gev_noAH = (rechitEnergySum_EE + beta_noAH*(rechitEnergySum_FH))/gamma_noAH;
      // h_rechit_energy_raw_ShowerInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySum_AH);
      // h_rechit_energy_raw_EE_alone_ShowerInEE->Fill(rechitEnergySum_EE);
      // h_rechit_energy_part_weighted_ShowerInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH);
      // h_rechit_energy_full_weighted_ShowerInEE->Fill(rechitEnergySum_EE+beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH)));

      // h_rechit_energy_full_weighted_ShowerInEE_withoutAHCAL->Fill(rechitEnergySum_EE+beta_noAH*rechitEnergySum_FH);

      // h_Nrechit_EE_vs_FH_ShowerInEE->Fill(Nrechit_EE,Nrechit_FH);

      // h_rechit_energy_full_weighted_inGev_ShowerInEE->Fill(tot_E_gev);
      // h_rechit_energy_full_weighted_inGev_ShowerInEE_withoutAHCAL->Fill(EE_FH_gev_noAH);
      // h_energy_all_gev_withoutAHCAL->Fill(EE_FH_gev_noAH);
      // h_rechit_energy_EE_FHAHCAL_EH_mips->Fill(rechitEnergySum_EE,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH))));
      // h_rechit_energy_EE_FHAHCAL_EH_gev->Fill(rechitEnergySum_EE/MIP_to_GeV,(beta*(rechitEnergySum_FH+(alpha*rechitEnergySum_AH)))/MIP_to_GeV);

      // h_energy_all_gev->Fill(tot_E_gev);



      bool trackwindow = !((trackX_al_3 < track_cut.at(0) || trackX_al_3 > track_cut.at(1) || trackY_al_3 < track_cut.at(2) || trackY_al_3 > track_cut.at(3)));
      bool SS1_reject = (shower_start_index+1 > 1);
      bool SS2_reject = (shower_start_index+1 > 2);

      
      ///////////////////////////////////////////////////////////
      /////     EH hadrons ; Chi2 matrix initialzation     /////
      //////////////////////////////////////////////////////////
      double EE_detscale = (rechitEnergySum_EE/EE_scale);
      double FH_detscale = (rechitEnergySum_FH/FH_AH_scale);
      double AH_detscale = (alpha_*rechitEnergySum_AH)/FH_AH_scale;

      float scale_ = scaling_factor_EH[(int)beamEnergy];
      
      // float w1 = getChi2Weights_EH(beamEnergy).at(0);
      // float w2 = getChi2Weights_EH(beamEnergy).at(1);
      // float w3 = getChi2Weights_EH(beamEnergy).at(2);
      
      float w1 = f_EH_w1->Eval(beamEnergy);
      float w2 = f_EH_w2->Eval(beamEnergy);
      float w3 = f_EH_w3->Eval(beamEnergy);

      float recoE_w1 = f_EH_w1->Eval(total_energy);
      float recoE_w2 = f_EH_w2->Eval(total_energy);
      float recoE_w3 = f_EH_w3->Eval(total_energy);

      if(DEBUG) { cout<<"beamE : w1 : w2 : w3 : total_E : Rw1 : Rw2 : Rw3 :: "<<beamEnergy<< " : " <<" : "<<w1 <<" : "<<w2<<" : "<<w3<<" : "<<total_energy<<" : "<<recoE_w1<< " : " << recoE_w2 << " : "<< recoE_w3 <<endl;}
      
      float chi2_energy_recoE = recoE_w1*EE_detscale + recoE_w2*FH_detscale + recoE_w3*AH_detscale;

      //double O = 3.5;
      double O = 0.0;
      float chi2_energy = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH + w3*rechitEnergySum_AH + O;

      float chi2_energy_withoutAHCAL = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH;
      
      bool IsIn_en_range = true;

      if(chi2_method > 0)  {
	if(chi2_method > 1) O = 0.0;
	if(chi2_method > 2) IsIn_en_range = (total_energy > en_range_EE_xmin[(int)beamEnergy] && total_energy < en_range_EE_xmax[(int)beamEnergy]);
	chi2_energy = w1*EE_detscale + w2*FH_detscale + w3*AH_detscale + O;
	chi2_energy_withoutAHCAL = w1*EE_detscale + w2*FH_detscale + O;
      }

      // if((int)beamEnergy == 20) {w1=1.67056; w2=1.04664 ; w3=1.25452 ; }
      // float chi2_energy = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH + w3*rechitEnergySum_AH + O;

      
      h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2->Fill(chi2_energy);
      h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2_rescaled->Fill(scale_ * chi2_energy);


      h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2_withoutAHCAL->Fill(chi2_energy_withoutAHCAL);



      //double sigma2_EH = (0.089*0.089) + (1.25*1.25)*tot_E_gev;
      double sigma2_EH = (0.087*0.087) + (1.48*1.48)*total_energy;
      
      if(trackwindow && SS2_reject && IsIn_en_range) {

	if(chi2_method == 0) {
	  coeffs_EH(0,0) += (rechitEnergySum_EE*rechitEnergySum_EE)/sigma2_EH;
	  coeffs_EH(0,1) += (rechitEnergySum_EE*rechitEnergySum_FH)/sigma2_EH;
	  coeffs_EH(0,2) += (rechitEnergySum_EE*rechitEnergySum_AH)/sigma2_EH;
	  coeffs_EH(1,0) += (rechitEnergySum_FH*rechitEnergySum_EE)/sigma2_EH;
	  coeffs_EH(1,1) += (rechitEnergySum_FH*rechitEnergySum_FH)/sigma2_EH;
	  coeffs_EH(1,2) += (rechitEnergySum_FH*rechitEnergySum_AH)/sigma2_EH;
	  coeffs_EH(2,0) += (rechitEnergySum_AH*rechitEnergySum_EE)/sigma2_EH;
	  coeffs_EH(2,1) += (rechitEnergySum_AH*rechitEnergySum_FH)/sigma2_EH;
	  coeffs_EH(2,2) += (rechitEnergySum_AH*rechitEnergySum_AH)/sigma2_EH;


	  consts_EH(0)   += (E_beam - O)*rechitEnergySum_EE/sigma2_EH;
	  consts_EH(1)   += (E_beam - O)*rechitEnergySum_FH/sigma2_EH;
	  consts_EH(2)   += (E_beam - O)*rechitEnergySum_AH/sigma2_EH;
	}
	else {
	  coeffs_EH(0,0) += (EE_detscale*EE_detscale)/sigma2_EH;
	  coeffs_EH(0,1) += (EE_detscale*FH_detscale)/sigma2_EH;
	  coeffs_EH(0,2) += (EE_detscale*AH_detscale)/sigma2_EH;
	  coeffs_EH(1,0) += (FH_detscale*EE_detscale)/sigma2_EH;
	  coeffs_EH(1,1) += (FH_detscale*FH_detscale)/sigma2_EH;
	  coeffs_EH(1,2) += (FH_detscale*AH_detscale)/sigma2_EH;
	  coeffs_EH(2,0) += (AH_detscale*EE_detscale)/sigma2_EH;
	  coeffs_EH(2,1) += (AH_detscale*FH_detscale)/sigma2_EH;
	  coeffs_EH(2,2) += (AH_detscale*AH_detscale)/sigma2_EH;
	
	
	  consts_EH(0)   += (E_beam - O)*EE_detscale/sigma2_EH;
	  consts_EH(1)   += (E_beam - O)*FH_detscale/sigma2_EH;
	  consts_EH(2)   += (E_beam - O)*AH_detscale/sigma2_EH;
	}

      }
      //////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////

      /////////////////////////////////////////////////////
      //////////////// selection cut check  ////////////////
      /////////////////////////////////////////////////////

      
      h_baseline->Fill(total_energy);
      h_baseline_chi2_EE->Fill(chi2_energy);

      if(trackwindow)  {
	h_trackwindow->Fill(total_energy);
	h_trackwindow_chi2_EE->Fill(chi2_energy);
	
      }
      if(SS1_reject) {
	h_SS1_reject->Fill(total_energy);
	h_SS1_reject_chi2_EE->Fill(chi2_energy);
      }
      if(SS2_reject) {
	h_SS2_reject->Fill(total_energy);
	h_SS2_reject_chi2_EE->Fill(chi2_energy);
      }
      if(SS1_reject && trackwindow) {
	h_trackwindow_SS1_reject->Fill(total_energy);
	h_trackwindow_SS1_reject_chi2_EE->Fill(chi2_energy);
      }
      if(SS2_reject && trackwindow) {
	h_trackwindow_SS2_reject->Fill(total_energy);
	h_trackwindow_SS2_reject_chi2_EE->Fill(chi2_energy);
	h_trackwindow_SS2_reject_chi2_EE_recoE->Fill(chi2_energy_recoE);
      }
      
	
	
      

      /////////////////////////////////////////////////////
      /////////////////////////////////////////////////////

      

    }

    //Showering in AHCAL ????
    if(isRegion4) {
      region_4_classified_events++;
    }

    h_rechit_energy_raw_EE_vs_FH_all->Fill(rechitEnergySum_EE,rechitEnergySum_FH);

    h_trackX_trackY_EE1->Fill(track_x[0],track_y[0]);
    h_trackX_trackY_FH12->Fill(track_x[39],track_y[39]);


    if(Nrechit_EE == 0) {
      cout<<" isRegion1 "<<isRegion1
	  <<" isRegion2 "<<isRegion2
	  <<" isRegion3 "<<isRegion3
	  <<" isRegion4 "<<isRegion4<<endl;
    }

    //for tree
    pion_tree->Fill();
    
    if(DEBUG) cout<<"DEBUG: End of Event = "<<jentry+1<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;
    // if(DEBUG && jentry > 5) return;
    // if(jentry > 5000) break;
  } // loop over entries

  
  //cout<<"Got Out, total events = "<<jentry<<" ; selected events = "<<selected_events<<endl;

  cout<<"***** Event summary ******"<<endl;
  cout<<"Total events = "<<event_count[0]<<endl;
  cout<<"Events with good track = "<<event_count[1]<<endl;
  cout<<"Events with Nrechits in FH[8][3] < 80 and FH[8][4] < 80 = "<<event_count[2]<<endl;
  cout<<" Events passing Muon veto = "<<event_count[3]<<endl;
  cout<<"***************************"<<endl<<endl;
  
  Long64_t total_events_temp = (region_1_classified_events+region_2_classified_events+region_3_classified_events+region_4_classified_events+non_classified_events);
  cout<<"Events with zero hits in EE = "<<EE_zeroHit<<endl;
  cout<<"Events with zero hits in FH = "<<FH_zeroHit<<endl;
  cout<<"Events with zero hits in AHCAL = "<<ahc_zeroHit<<endl;
  cout<<"MIP like events = "<<((float)region_1_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"shower start in EE = "<<((float)region_3_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"shower start in FH = "<<((float)region_2_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"shower start in AH = "<<((float)region_4_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"Non-classified events = "<<((float)non_classified_events*100.0)/total_events_temp<<"%"<<endl;
  //cout<<"Sum = "<<(region_1_classified_events+region_2_classified_events+region_3_classified_events+non_classified_events)<<endl;
  cout<<"Number of events with (Nrechit_EE == 0) = "<<EE_zeroHit_R2<<endl;
  cout<<"Sum = "<<total_events_temp<<endl;


  // cout<<"Incorrect Muon tag: "<<((float)debug_count*100.0)/total_events<<endl;
  
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
    cout<<E_beam<<" "<<values_EH(0)<<" "<<values_EH(1)<<" "<<values_EH(2)<<endl;

  }
  else {
    cout<<"Error: Could not invert for EH hadrons..."<<endl;
  }
  if(isInverted_H) {
    values_H = coeffs_H*consts_H;
    //cout<<"H Hadrons => For E = "<<E_beam<<"GeV, w1 = "<<values_H(0)<<" ;w2 = "<<values_H(1)<<" ;w3 = "<<values_H(2)<<";"<<endl;
    cout<<"H Hadrons => For E = "<<E_beam<<"GeV, w1 = 0.0 ;w2 = "<<values_H(0)<<" ;w3 = "<<values_H(1)<<";"<<endl;
    cout<<E_beam<<" 0.0 "<<values_H(0)<<" "<<values_H(1)<<endl;
  }
  else {
    cout<<"Error: Could not invert for H hadrons..."<<endl;
  }


  cout<<endl<<endl;

  //for tree
  outfile->cd();
  pion_tree->Write();
  outfile->Close();
  cout<<"outFile: "<<outFileName<<" written!!"<<endl;

}



//for tree
void HGCNtupleVariables::init_piTree(){

  
  pion_tree->Branch("event",&pi_event);
  pion_tree->Branch("run",&pi_run);
  pion_tree->Branch("pdgID",&pi_pdgID);
  pion_tree->Branch("beamEnergy",&pi_beamEnergy);
  pion_tree->Branch("trueBeamEnergy",&pi_trueBeamEnergy);


  pion_tree->Branch("isHGC_AHC_sync",&pi_isHGC_AHC_sync);
  pion_tree->Branch("isGoodTrack",&pi_isGoodTrack);
  pion_tree->Branch("isFHNoisy",&pi_isFHNoisy);
  pion_tree->Branch("MuonVeto",&pi_MuonVeto);
  pion_tree->Branch("isInTrackWindow",&pi_isInTrackWindow);
  pion_tree->Branch("hgc_channel_mask",&pi_hgc_channel_mask);
  pion_tree->Branch("ahc_channel_mask",&pi_ahc_channel_mask);
  pion_tree->Branch("pass_noise_thres",&pi_pass_noise_thres);
  pion_tree->Branch("event",&pi_event);

  
  pion_tree->Branch("NRechits",&pi_NRechits);
  pion_tree->Branch("rechit_energy",&pi_rechit_energy);
  pion_tree->Branch("rechit_x",&pi_rechit_x);
  pion_tree->Branch("rechit_y",&pi_rechit_y);
  pion_tree->Branch("rechit_z",&pi_rechit_z);
  pion_tree->Branch("rechit_cogX",&pi_rechit_cogX);
  pion_tree->Branch("rechit_cogY",&pi_rechit_cogY);
  pion_tree->Branch("rechit_iu",&pi_rechit_iu);
  pion_tree->Branch("rechit_iv",&pi_rechit_iv);
  pion_tree->Branch("rechit_iU",&pi_rechit_iU);
  pion_tree->Branch("rechit_iV",&pi_rechit_iV);
  pion_tree->Branch("rechit_amplitudeHigh",&pi_rechit_amplitudeHigh);
  pion_tree->Branch("rechit_amplitudeLow",&pi_rechit_amplitudeLow);
  pion_tree->Branch("rechit_noise_flag",&pi_rechit_noise_flag);
  pion_tree->Branch("rechit_module",&pi_rechit_module);
  pion_tree->Branch("rechit_layer",&pi_rechit_layer);
  pion_tree->Branch("rechit_chip",&pi_rechit_chip);
  pion_tree->Branch("rechit_channel",&pi_rechit_channel);
  pion_tree->Branch("rechit_type",&pi_rechit_type);
  pion_tree->Branch("rechit_shower_start_layer",&pi_rechit_shower_start_layer);
  pion_tree->Branch("rechit_energyPerLayer",&pi_rechit_energyPerLayer);
  pion_tree->Branch("rechit_nHitsPerLayer",&pi_rechit_nHitsPerLayer);


  pion_tree->Branch("ntracks",&pi_ntracks);
  pion_tree->Branch("trackChi2_X",&pi_trackChi2_X);
  pion_tree->Branch("trackChi2_Y",&pi_trackChi2_Y);
  pion_tree->Branch("dwcReferenceType",&pi_dwcReferenceType);
  pion_tree->Branch("m_x",&pi_m_x);
  pion_tree->Branch("m_y",&pi_m_y);
  pion_tree->Branch("b_x",&pi_b_x);
  pion_tree->Branch("b_y",&pi_b_y);
  pion_tree->Branch("TrackImpactX_layer",&pi_impactX_layer);
  pion_tree->Branch("TrackImpactY_layer",&pi_impactY_layer);

  pion_tree->Branch("ahc_nHits",&pi_ahc_nHits);
  pion_tree->Branch("ahc_energyPerLayer",&pi_ahc_energyPerLayer);
  pion_tree->Branch("ahc_nHitsPerLayer",&pi_ahc_nHitsPerLayer);
  pion_tree->Branch("ahc_hitI",&pi_ahc_hitI);
  pion_tree->Branch("ahc_hitJ",&pi_ahc_hitJ);
  pion_tree->Branch("ahc_hitK",&pi_ahc_hitK);
  pion_tree->Branch("ahc_hitEnergy",&pi_ahc_hitEnergy);
  pion_tree->Branch("ahc_hitX",&pi_ahc_hitX);
  pion_tree->Branch("ahc_hitY",&pi_ahc_hitY);
  pion_tree->Branch("ahc_hitZ",&pi_ahc_hitZ);
  pion_tree->Branch("ahc_nHits",&pi_ahc_nHits);
  

  pion_tree->Branch("energyLostEE",&pi_energyLostEE);
  pion_tree->Branch("energyLostFH",&pi_energyLostFH);
  pion_tree->Branch("energyLostBH",&pi_energyLostBH);
  pion_tree->Branch("energyLostBeam",&pi_energyLostBeam);
  

}
