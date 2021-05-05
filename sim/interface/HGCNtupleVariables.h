//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 16 18:22:34 2018 by ROOT version 6.06/01
// from TTree hits/HGC rechits
// found on file: muon_v10.root
//////////////////////////////////////////////////////////

#ifndef HGCNtupleVariables_h
#define HGCNtupleVariables_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TProfile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

using namespace std;

class HGCNtupleVariables {
public :

   HGCNtupleVariables(TTree * /*tree*/ =0) : fChain(0) { }
   ~HGCNtupleVariables() { }
   /* void    Init(TTree *tree); */
   void    Init(TTree *tree, TTree *tree2);
   Bool_t  Notify();
   Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   Int_t   GetEntry2(Long64_t entry, Int_t getall = 0) { return fChain2 ? fChain2->GetTree()->GetEntry(entry, getall) : 0; }
  std::vector<int> getModuleLocation(int moduleID);
  int getBIN(unsigned int skiroc,unsigned int channel);
  float deltaR(float x1, float y1, float x2, float y2);

  std::vector<float> getLayerPosition(int layer_);
  std::map<int, std::vector<float>> layer_positions;

  std::vector<float> getWeights(int beamEnergy_);
  std::map<int, std::vector<float>> rel_weights;

  std::vector<float> getChi2Weights_EH(int beamEnergy_);
  std::map<int, std::vector<float>> chi2_weights_EH;

  std::vector<float> getChi2Weights_H(int beamEnergy_);
  std::map<int, std::vector<float>> chi2_weights_H;

  //double shower_comparisons(TProfile* shower, TH1F* hist);
  //double shower_comparisons(TProfile* shower, TH1F* ref_h);
  // TH1F* shower_comparisons(TProfile* shower, TH1F* hist);
  // float find_my_calib(int layer, int en_chan);
  // float find_official_calib(int layer, int en_chan);
  std::map<int, std::vector<int>> module_map;
  std::map<int, std::pair<float,float> > align_map;
  std::map<std::pair<int,int>, float > noise_map;
  std::map<std::pair<int,int>, float > mip_ratio_map;
  std::pair<float,float> dxy_alignment(int layer);
  float getNoise(std::pair<int,int> mod_chip);
  float getMIPRatio(std::pair<int,int> mod_chip);


  std::map<int, float> scaling_factor_EH;
  std::map<int, float> scaling_factor_H;

  // std::map<std::pair<int, int>, float> offical_calib_map;
  // std::map<std::pair<int, int>, float> my_calib_map;
  
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TTree          *fChain2;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   Int_t           fCurrent2; //!current Tree number in a TChain


    //for tree
  TTree* pion_tree;
  UInt_t          pi_event;
  UInt_t          pi_run;
  Int_t           pi_pdgID;
  Float_t         pi_beamEnergy;
  Float_t         pi_trueBeamEnergy;
  Int_t           pi_NRechits;
  vector<float>   *pi_rechit_energy;
   vector<unsigned int> *pi_rechit_module;
   vector<unsigned int> *pi_rechit_layer;
   vector<unsigned int> *pi_rechit_chip;
   vector<unsigned int> *pi_rechit_channel;
   vector<unsigned int> *pi_rechit_type;
  vector<float>   *pi_rechit_x;
  vector<float>   *pi_rechit_y;
  vector<float>   *pi_rechit_cogX;
  vector<float>   *pi_rechit_cogY;

  vector<float>   *pi_rechit_z;
  vector<short>     *pi_rechit_iu;
  vector<short>     *pi_rechit_iv;
  vector<short>     *pi_rechit_iU;
  vector<short>     *pi_rechit_iV;
  vector<float>   *pi_rechit_amplitudeHigh;
  vector<float>   *pi_rechit_amplitudeLow;
  vector<bool>    *pi_rechit_noise_flag;
  int             pi_rechit_shower_start_layer;
  vector<float>   *pi_rechit_energyPerLayer;
  vector<float>   *pi_rechit_nHitsPerLayer;

  
  Int_t           pi_ntracks;
  Float_t         pi_trackChi2_X;
  Float_t         pi_trackChi2_Y;
  Int_t           pi_dwcReferenceType;
  Double_t        pi_m_x;
  Double_t        pi_m_y;
  Double_t        pi_b_x;
  Double_t        pi_b_y;
  vector<float>   *pi_impactX_layer;
  vector<float>   *pi_impactY_layer;


  Int_t           pi_ahc_nHits;
   vector<float>   *pi_ahc_energyPerLayer;
   vector<int>     *pi_ahc_nHitsPerLayer;
   vector<int>     *pi_ahc_hitI;
   vector<int>     *pi_ahc_hitJ;
   vector<int>     *pi_ahc_hitK;
   vector<float>   *pi_ahc_hitEnergy;
   vector<float>   *pi_ahc_hitX;
   vector<float>   *pi_ahc_hitY;
   vector<float>   *pi_ahc_hitZ;


  bool pi_isHGC_AHC_sync;
  bool pi_isGoodTrack;
  bool pi_isFHNoisy;
  bool pi_MuonVeto;
  bool pi_isInTrackWindow;
  vector<bool> *pi_hgc_channel_mask;
  vector<bool> *pi_ahc_channel_mask;
  vector<bool> *pi_pass_noise_thres;


  Float_t         pi_energyLostEE;
   Float_t         pi_energyLostFH;
   Float_t         pi_energyLostBH;
   Float_t         pi_energyLostBeam;

  
  void init_piTree();

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          event;
   UInt_t          run;
   Int_t           pdgID;
   Float_t         beamEnergy;
   Float_t         trueBeamEnergy;
   Float_t         energyLostEE;
   Float_t         energyLostFH;
   Float_t         energyLostBH;
   Float_t         energyLostBeam;
   Float_t         energyLostOutside;
   Float_t         energyLeakTransverseEE;
   Float_t         energyLeakTransverseFH;
   Float_t         energyLeakTransverseAH;
   Float_t         energyLeakLongitudinal;
   Float_t         energyLeakResidual;
   Float_t         sim_energyAH;
   /* vector<float>   *energyAbsorbedEE; */
   /* vector<float>   *energyAbsorbedFH; */

   Int_t           NRechits;
   vector<unsigned int> *rechit_detid;
   vector<unsigned int> *rechit_module;
   vector<unsigned int> *rechit_layer;
   vector<unsigned int> *rechit_chip;
   vector<unsigned int> *rechit_channel;
   vector<unsigned int> *rechit_type;
   vector<float>   *rechit_x;
   vector<float>   *rechit_y;
   vector<float>   *rechit_z;
   vector<short>     *rechit_iu;
   vector<short>     *rechit_iv;
   vector<short>     *rechit_iU;
   vector<short>     *rechit_iV;
   vector<float>   *rechit_energy;
   vector<float>   *rechit_energy_noHG;
   vector<float>   *rechit_amplitudeHigh;
   vector<float>   *rechit_amplitudeLow;
   vector<bool>    *rechit_hg_goodFit;
   vector<bool>    *rechit_lg_goodFit;
   vector<bool>    *rechit_hg_saturated;
   vector<bool>    *rechit_lg_saturated;
  vector<bool>    *rechit_noise_flag;
   vector<bool>    *rechit_fully_calibrated;
   vector<float>   *rechit_TS2High;
   vector<float>   *rechit_TS2Low;
   vector<float>   *rechit_TS3High;
   vector<float>   *rechit_TS3Low;
   vector<unsigned short> *rechit_Tot;
   /* vector<float>   *rechit_time; */
   vector<float>   *rechit_timeMaxHG;
   vector<float>   *rechit_timeMaxLG;
   vector<unsigned short> *rechit_toaRise;
   vector<unsigned short> *rechit_toaFall;


  //AHCAL
  Int_t           ahc_nHits;
  vector<int>     *ahc_hitI;
  vector<int>     *ahc_hitJ;
  vector<int>     *ahc_hitK;
  vector<float>   *ahc_hitEnergy;
  vector<float>   *ahc_hitX;
  vector<float>   *ahc_hitY;
  vector<float>   *ahc_hitZ;
  vector<float>   *ahc_energyPerLayer;
   vector<int>     *ahc_nHitsPerLayer;

  
   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_run;   //!
   TBranch        *b_pdgID;   //!
   TBranch        *b_beamEnergy;   //!
   TBranch        *b_trueBeamEnergy;   //!
   TBranch        *b_energyLostEE;
   TBranch        *b_energyLostFH;
   TBranch        *b_energyLostBH;
   TBranch        *b_energyLostBeam;
   TBranch        *b_energyLostOutside;
   TBranch        *b_energyLeakTransverseEE;
   TBranch        *b_energyLeakTransverseFH;
   TBranch        *b_energyLeakTransverseAH;
   TBranch        *b_energyLeakLongitudinal;
   TBranch        *b_energyLeakResidual;
   TBranch        *b_sim_energyAH;
   /* TBranch        *b_energyAbsorbedEE; */
   /* TBranch        *b_energyAbsorbedFH; */

   TBranch        *b_NRechits;   //!
   TBranch        *b_rechit_detid;   //!
   TBranch        *b_rechit_module;   //!
   TBranch        *b_rechit_layer;   //!
   TBranch        *b_rechit_chip;   //!
   TBranch        *b_rechit_channel;   //!
   TBranch        *b_rechit_type;   //!
   TBranch        *b_rechit_x;   //!
   TBranch        *b_rechit_y;   //!
   TBranch        *b_rechit_z;   //!
   TBranch        *b_rechit_iu;   //!
   TBranch        *b_rechit_iv;   //!
   TBranch        *b_rechit_iU;   //!
   TBranch        *b_rechit_iV;   //!
   TBranch        *b_rechit_energy;   //!
   TBranch        *b_rechit_energy_noHG;   //!
   TBranch        *b_rechit_amplitudeHigh;   //!
   TBranch        *b_rechit_amplitudeLow;   //!
   TBranch        *b_rechit_hg_goodFit;   //!
   TBranch        *b_rechit_lg_goodFit;   //!
   TBranch        *b_rechit_hg_saturated;   //!
  TBranch        *b_rechit_lg_saturated;   //!
  TBranch        *b_rechit_noise_flag;   //!
   TBranch        *b_rechit_fully_calibrated;   //!
   TBranch        *b_rechit_TS2High;   //!
   TBranch        *b_rechit_TS2Low;   //!
   TBranch        *b_rechit_TS3High;   //!
   TBranch        *b_rechit_TS3Low;   //!
   TBranch        *b_rechit_Tot;   //!
   /* TBranch        *b_rechit_time;   //! */
   TBranch        *b_rechit_timeMaxHG;   //!
   TBranch        *b_rechit_timeMaxLG;   //!
   TBranch        *b_rechit_toaRise;   //!
   TBranch        *b_rechit_toaFall;   //!

  //AHCAL
   TBranch        *b_ahc_nHits;   //!
   TBranch        *b_ahc_hitI;   //!
   TBranch        *b_ahc_hitJ;   //!
   TBranch        *b_ahc_hitK;   //!
   TBranch        *b_ahc_hitEnergy;   //!
   TBranch        *b_ahc_hitX;   //!
   TBranch        *b_ahc_hitY;   //!
   TBranch        *b_ahc_hitZ;   //!
   TBranch        *b_ahc_energyPerLayer;   //!
   TBranch        *b_ahc_nHitsPerLayer;   //!


  
   // second tree
   Int_t           ntracks;
   Float_t         impactX_HGCal_layer_1;
   Float_t         impactY_HGCal_layer_1;
   Float_t         impactX_HGCal_layer_2;
   Float_t         impactY_HGCal_layer_2;
   Float_t         impactX_HGCal_layer_3;
   Float_t         impactY_HGCal_layer_3;
   Float_t         impactX_HGCal_layer_4;
   Float_t         impactY_HGCal_layer_4;
   Float_t         impactX_HGCal_layer_5;
   Float_t         impactY_HGCal_layer_5;
   Float_t         impactX_HGCal_layer_6;
   Float_t         impactY_HGCal_layer_6;
   Float_t         impactX_HGCal_layer_7;
   Float_t         impactY_HGCal_layer_7;
   Float_t         impactX_HGCal_layer_8;
   Float_t         impactY_HGCal_layer_8;
   Float_t         impactX_HGCal_layer_9;
   Float_t         impactY_HGCal_layer_9;
   Float_t         impactX_HGCal_layer_10;
   Float_t         impactY_HGCal_layer_10;
   Float_t         impactX_HGCal_layer_11;
   Float_t         impactY_HGCal_layer_11;
   Float_t         impactX_HGCal_layer_12;
   Float_t         impactY_HGCal_layer_12;
   Float_t         impactX_HGCal_layer_13;
   Float_t         impactY_HGCal_layer_13;
   Float_t         impactX_HGCal_layer_14;
   Float_t         impactY_HGCal_layer_14;
   Float_t         impactX_HGCal_layer_15;
   Float_t         impactY_HGCal_layer_15;
   Float_t         impactX_HGCal_layer_16;
   Float_t         impactY_HGCal_layer_16;
   Float_t         impactX_HGCal_layer_17;
   Float_t         impactY_HGCal_layer_17;
   Float_t         impactX_HGCal_layer_18;
   Float_t         impactY_HGCal_layer_18;
   Float_t         impactX_HGCal_layer_19;
   Float_t         impactY_HGCal_layer_19;
   Float_t         impactX_HGCal_layer_20;
   Float_t         impactY_HGCal_layer_20;
   Float_t         impactX_HGCal_layer_21;
   Float_t         impactY_HGCal_layer_21;
   Float_t         impactX_HGCal_layer_22;
   Float_t         impactY_HGCal_layer_22;
   Float_t         impactX_HGCal_layer_23;
   Float_t         impactY_HGCal_layer_23;
   Float_t         impactX_HGCal_layer_24;
   Float_t         impactY_HGCal_layer_24;
   Float_t         impactX_HGCal_layer_25;
   Float_t         impactY_HGCal_layer_25;
   Float_t         impactX_HGCal_layer_26;
   Float_t         impactY_HGCal_layer_26;
   Float_t         impactX_HGCal_layer_27;
   Float_t         impactY_HGCal_layer_27;
   Float_t         impactX_HGCal_layer_28;
   Float_t         impactY_HGCal_layer_28;
   Float_t         impactX_HGCal_layer_29;
   Float_t         impactY_HGCal_layer_29;
   Float_t         impactX_HGCal_layer_30;
   Float_t         impactY_HGCal_layer_30;
   Float_t         impactX_HGCal_layer_31;
   Float_t         impactY_HGCal_layer_31;
   Float_t         impactX_HGCal_layer_32;
   Float_t         impactY_HGCal_layer_32;
   Float_t         impactX_HGCal_layer_33;
   Float_t         impactY_HGCal_layer_33;
   Float_t         impactX_HGCal_layer_34;
   Float_t         impactY_HGCal_layer_34;
   Float_t         impactX_HGCal_layer_35;
   Float_t         impactY_HGCal_layer_35;
   Float_t         impactX_HGCal_layer_36;
   Float_t         impactY_HGCal_layer_36;
   Float_t         impactX_HGCal_layer_37;
   Float_t         impactY_HGCal_layer_37;
   Float_t         impactX_HGCal_layer_38;
   Float_t         impactY_HGCal_layer_38;
   Float_t         impactX_HGCal_layer_39;
   Float_t         impactY_HGCal_layer_39;
   Float_t         impactX_HGCal_layer_40;
   Float_t         impactY_HGCal_layer_40;


   Float_t         trackChi2_X;
   Float_t         trackChi2_Y;
   Int_t           dwcReferenceType;
   Double_t        m_x;
   Double_t        m_y;
   Double_t        b_x;
   Double_t        b_y;

   // List of branches
   TBranch        *b_ntracks;   //!
   TBranch        *b_impactX_HGCal_layer_1;   //!
   TBranch        *b_impactY_HGCal_layer_1;   //!
   TBranch        *b_impactX_HGCal_layer_2;   //!
   TBranch        *b_impactY_HGCal_layer_2;   //!
   TBranch        *b_impactX_HGCal_layer_3;   //!
   TBranch        *b_impactY_HGCal_layer_3;   //!
   TBranch        *b_impactX_HGCal_layer_4;   //!
   TBranch        *b_impactY_HGCal_layer_4;   //!
   TBranch        *b_impactX_HGCal_layer_5;   //!
   TBranch        *b_impactY_HGCal_layer_5;   //!
   TBranch        *b_impactX_HGCal_layer_6;   //!
   TBranch        *b_impactY_HGCal_layer_6;   //!
   TBranch        *b_impactX_HGCal_layer_7;   //!
   TBranch        *b_impactY_HGCal_layer_7;   //!
   TBranch        *b_impactX_HGCal_layer_8;   //!
   TBranch        *b_impactY_HGCal_layer_8;   //!
   TBranch        *b_impactX_HGCal_layer_9;   //!
   TBranch        *b_impactY_HGCal_layer_9;   //!
   TBranch        *b_impactX_HGCal_layer_10;   //!
   TBranch        *b_impactY_HGCal_layer_10;   //!
   TBranch        *b_impactX_HGCal_layer_11;   //!
   TBranch        *b_impactY_HGCal_layer_11;   //!
   TBranch        *b_impactX_HGCal_layer_12;   //!
   TBranch        *b_impactY_HGCal_layer_12;   //!
   TBranch        *b_impactX_HGCal_layer_13;   //!
   TBranch        *b_impactY_HGCal_layer_13;   //!
   TBranch        *b_impactX_HGCal_layer_14;   //!
   TBranch        *b_impactY_HGCal_layer_14;   //!
   TBranch        *b_impactX_HGCal_layer_15;   //!
   TBranch        *b_impactY_HGCal_layer_15;   //!
   TBranch        *b_impactX_HGCal_layer_16;   //!
   TBranch        *b_impactY_HGCal_layer_16;   //!
   TBranch        *b_impactX_HGCal_layer_17;   //!
   TBranch        *b_impactY_HGCal_layer_17;   //!
   TBranch        *b_impactX_HGCal_layer_18;   //!
   TBranch        *b_impactY_HGCal_layer_18;   //!
   TBranch        *b_impactX_HGCal_layer_19;   //!
   TBranch        *b_impactY_HGCal_layer_19;   //!
   TBranch        *b_impactX_HGCal_layer_20;   //!
   TBranch        *b_impactY_HGCal_layer_20;   //!
   TBranch        *b_impactX_HGCal_layer_21;   //!
   TBranch        *b_impactY_HGCal_layer_21;   //!
   TBranch        *b_impactX_HGCal_layer_22;   //!
   TBranch        *b_impactY_HGCal_layer_22;   //!
   TBranch        *b_impactX_HGCal_layer_23;   //!
   TBranch        *b_impactY_HGCal_layer_23;   //!
   TBranch        *b_impactX_HGCal_layer_24;   //!
   TBranch        *b_impactY_HGCal_layer_24;   //!
   TBranch        *b_impactX_HGCal_layer_25;   //!
   TBranch        *b_impactY_HGCal_layer_25;   //!
   TBranch        *b_impactX_HGCal_layer_26;   //!
   TBranch        *b_impactY_HGCal_layer_26;   //!
   TBranch        *b_impactX_HGCal_layer_27;   //!
   TBranch        *b_impactY_HGCal_layer_27;   //!
   TBranch        *b_impactX_HGCal_layer_28;   //!
   TBranch        *b_impactY_HGCal_layer_28;   //!
   TBranch         *b_impactX_HGCal_layer_29;
   TBranch         *b_impactY_HGCal_layer_29;
   TBranch         *b_impactX_HGCal_layer_30;
   TBranch         *b_impactY_HGCal_layer_30;
   TBranch         *b_impactX_HGCal_layer_31;
   TBranch         *b_impactY_HGCal_layer_31;
   TBranch         *b_impactX_HGCal_layer_32;
   TBranch         *b_impactY_HGCal_layer_32;
   TBranch         *b_impactX_HGCal_layer_33;
   TBranch         *b_impactY_HGCal_layer_33;
   TBranch         *b_impactX_HGCal_layer_34;
   TBranch         *b_impactY_HGCal_layer_34;
   TBranch         *b_impactX_HGCal_layer_35;
   TBranch         *b_impactY_HGCal_layer_35;
   TBranch         *b_impactX_HGCal_layer_36;
   TBranch         *b_impactY_HGCal_layer_36;
   TBranch         *b_impactX_HGCal_layer_37;
   TBranch         *b_impactY_HGCal_layer_37;
   TBranch         *b_impactX_HGCal_layer_38;
   TBranch         *b_impactY_HGCal_layer_38;
   TBranch         *b_impactX_HGCal_layer_39;
   TBranch         *b_impactY_HGCal_layer_39;
   TBranch         *b_impactX_HGCal_layer_40;
   TBranch         *b_impactY_HGCal_layer_40;

   TBranch        *b_trackChi2_X;   //!
   TBranch        *b_trackChi2_Y;   //!
   TBranch        *b_dwcReferenceType;   //!
   TBranch        *b_m_x;   //!
   TBranch        *b_m_y;   //!
   TBranch        *b_b_x;   //!
   TBranch        *b_b_y;   //!



};

#endif

#ifdef HGCNtupleVariables_cxx

void HGCNtupleVariables::Init(TTree *tree, TTree *tree2)
/* void HGCNtupleVariables::Init(TTree *tree) */
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
  
  /* energyAbsorbedEE = 0; */
  /* energyAbsorbedFH = 0; */
   rechit_detid = 0;
   rechit_module = 0;
   rechit_layer = 0;
   rechit_chip = 0;
   rechit_channel = 0;
   rechit_type = 0;
   rechit_x = 0;
   rechit_y = 0;
   rechit_z = 0;
   rechit_iu = 0;
   rechit_iv = 0;
   rechit_iU = 0;
   rechit_iV = 0;
   rechit_energy = 0;
   rechit_energy_noHG = 0;
   rechit_amplitudeHigh = 0;
   rechit_amplitudeLow = 0;
   rechit_hg_goodFit = 0;
   rechit_lg_goodFit = 0;
   rechit_hg_saturated = 0;
   rechit_lg_saturated = 0;
   rechit_noise_flag = 0;
   rechit_fully_calibrated = 0;
   rechit_TS2High = 0;
   rechit_TS2Low = 0;
   rechit_TS3High = 0;
   rechit_TS3Low = 0;
   rechit_Tot = 0;
   /* rechit_time = 0; */
   rechit_timeMaxHG = 0;
   rechit_timeMaxLG = 0;
   rechit_toaRise = 0;
   rechit_toaFall = 0;

   //AHCAL
   ahc_hitI = 0;
   ahc_hitJ = 0;
   ahc_hitK = 0;
   ahc_hitEnergy = 0;
   ahc_hitX = 0;
   ahc_hitY = 0;
   ahc_hitZ = 0;
   ahc_energyPerLayer = 0;
   ahc_nHitsPerLayer = 0;

   
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("pdgID", &pdgID, &b_pdgID);
   fChain->SetBranchAddress("beamEnergy", &beamEnergy, &b_beamEnergy);
   fChain->SetBranchAddress("trueBeamEnergy", &trueBeamEnergy, &b_trueBeamEnergy);
   fChain->SetBranchAddress("energyLostEE", &energyLostEE, &b_energyLostEE);
   fChain->SetBranchAddress("energyLostFH", &energyLostFH, &b_energyLostFH);
   fChain->SetBranchAddress("energyLostBH", &energyLostBH, &b_energyLostBH);
   fChain->SetBranchAddress("energyLostBeam", &energyLostBeam, &b_energyLostBeam);
   fChain->SetBranchAddress("energyLostOutside", &energyLostOutside, &b_energyLostOutside);
   fChain->SetBranchAddress("energyLeakTransverseEE", &energyLeakTransverseEE, &b_energyLeakTransverseEE);
   fChain->SetBranchAddress("energyLeakTransverseFH", &energyLeakTransverseFH, &b_energyLeakTransverseFH);
   fChain->SetBranchAddress("energyLeakTransverseAH", &energyLeakTransverseAH, &b_energyLeakTransverseAH);
   fChain->SetBranchAddress("energyLeakLongitudinal", &energyLeakLongitudinal, &b_energyLeakLongitudinal);
   fChain->SetBranchAddress("energyLeakResidual", &energyLeakResidual, &b_energyLeakResidual);
   fChain->SetBranchAddress("sim_energyAH", &sim_energyAH, &b_sim_energyAH);
   /* fChain->SetBranchAddress("energyAbsorbedEE", &energyAbsorbedEE, &b_energyAbsorbedEE); */
   /* fChain->SetBranchAddress("energyAbsorbedFH", &energyAbsorbedFH, &b_energyAbsorbedFH); */
   fChain->SetBranchAddress("NRechits", &NRechits, &b_NRechits);
   fChain->SetBranchAddress("rechit_detid", &rechit_detid, &b_rechit_detid);
   fChain->SetBranchAddress("rechit_module", &rechit_module, &b_rechit_module);
   fChain->SetBranchAddress("rechit_layer", &rechit_layer, &b_rechit_layer);
   fChain->SetBranchAddress("rechit_chip", &rechit_chip, &b_rechit_chip);
   fChain->SetBranchAddress("rechit_channel", &rechit_channel, &b_rechit_channel);
   fChain->SetBranchAddress("rechit_type", &rechit_type, &b_rechit_type);
   fChain->SetBranchAddress("rechit_x", &rechit_x, &b_rechit_x);
   fChain->SetBranchAddress("rechit_y", &rechit_y, &b_rechit_y);
   fChain->SetBranchAddress("rechit_z", &rechit_z, &b_rechit_z);
   fChain->SetBranchAddress("rechit_iu", &rechit_iu, &b_rechit_iu);
   fChain->SetBranchAddress("rechit_iv", &rechit_iv, &b_rechit_iv);
   fChain->SetBranchAddress("rechit_iU", &rechit_iU, &b_rechit_iU);
   fChain->SetBranchAddress("rechit_iV", &rechit_iV, &b_rechit_iV);
   fChain->SetBranchAddress("rechit_energy", &rechit_energy, &b_rechit_energy);
   fChain->SetBranchAddress("rechit_energy_noHG", &rechit_energy_noHG, &b_rechit_energy_noHG);
   fChain->SetBranchAddress("rechit_amplitudeHigh", &rechit_amplitudeHigh, &b_rechit_amplitudeHigh);
   fChain->SetBranchAddress("rechit_amplitudeLow", &rechit_amplitudeLow, &b_rechit_amplitudeLow);
   fChain->SetBranchAddress("rechit_hg_goodFit", &rechit_hg_goodFit, &b_rechit_hg_goodFit);
   fChain->SetBranchAddress("rechit_lg_goodFit", &rechit_lg_goodFit, &b_rechit_lg_goodFit);
   fChain->SetBranchAddress("rechit_hg_saturated", &rechit_hg_saturated, &b_rechit_hg_saturated);
   fChain->SetBranchAddress("rechit_lg_saturated", &rechit_lg_saturated, &b_rechit_lg_saturated);
   fChain->SetBranchAddress("rechit_noise_flag", &rechit_noise_flag, &b_rechit_noise_flag);
   fChain->SetBranchAddress("rechit_fully_calibrated", &rechit_fully_calibrated, &b_rechit_fully_calibrated);
   fChain->SetBranchAddress("rechit_TS2High", &rechit_TS2High, &b_rechit_TS2High);
   fChain->SetBranchAddress("rechit_TS2Low", &rechit_TS2Low, &b_rechit_TS2Low);
   fChain->SetBranchAddress("rechit_TS3High", &rechit_TS3High, &b_rechit_TS3High);
   fChain->SetBranchAddress("rechit_TS3Low", &rechit_TS3Low, &b_rechit_TS3Low);
   fChain->SetBranchAddress("rechit_Tot", &rechit_Tot, &b_rechit_Tot);
   /* fChain->SetBranchAddress("rechit_time", &rechit_time, &b_rechit_time); */
   fChain->SetBranchAddress("rechit_timeMaxHG", &rechit_timeMaxHG, &b_rechit_timeMaxHG);
   fChain->SetBranchAddress("rechit_timeMaxLG", &rechit_timeMaxLG, &b_rechit_timeMaxLG);
   fChain->SetBranchAddress("rechit_toaRise", &rechit_toaRise, &b_rechit_toaRise);
   fChain->SetBranchAddress("rechit_toaFall", &rechit_toaFall, &b_rechit_toaFall);


   //AHCAL
   fChain->SetBranchAddress("ahc_hitI", &ahc_hitI, &b_ahc_hitI);
   fChain->SetBranchAddress("ahc_hitJ", &ahc_hitJ, &b_ahc_hitJ);
   fChain->SetBranchAddress("ahc_hitK", &ahc_hitK, &b_ahc_hitK);
   fChain->SetBranchAddress("ahc_nHits", &ahc_nHits, &b_ahc_nHits);
   fChain->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy, &b_ahc_hitEnergy);
   fChain->SetBranchAddress("ahc_hitX", &ahc_hitX, &b_ahc_hitX);
   fChain->SetBranchAddress("ahc_hitY", &ahc_hitY, &b_ahc_hitY);
   fChain->SetBranchAddress("ahc_hitZ", &ahc_hitZ, &b_ahc_hitZ);
   fChain->SetBranchAddress("ahc_energyPerLayer", &ahc_energyPerLayer, &b_ahc_energyPerLayer);
   fChain->SetBranchAddress("ahc_nHitsPerLayer", &ahc_nHitsPerLayer, &b_ahc_nHitsPerLayer);

   
   // Set branch addresses and branch pointers
   if (!tree2) return;
   fChain2 = tree2;
   fCurrent2 = -1;
   fChain2->SetMakeClass(1);

   fChain2->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain2->SetBranchAddress("impactX_HGCal_layer_1", &impactX_HGCal_layer_1, &b_impactX_HGCal_layer_1);
   fChain2->SetBranchAddress("impactY_HGCal_layer_1", &impactY_HGCal_layer_1, &b_impactY_HGCal_layer_1);
   fChain2->SetBranchAddress("impactX_HGCal_layer_2", &impactX_HGCal_layer_2, &b_impactX_HGCal_layer_2);
   fChain2->SetBranchAddress("impactY_HGCal_layer_2", &impactY_HGCal_layer_2, &b_impactY_HGCal_layer_2);
   fChain2->SetBranchAddress("impactX_HGCal_layer_3", &impactX_HGCal_layer_3, &b_impactX_HGCal_layer_3);
   fChain2->SetBranchAddress("impactY_HGCal_layer_3", &impactY_HGCal_layer_3, &b_impactY_HGCal_layer_3);
   fChain2->SetBranchAddress("impactX_HGCal_layer_4", &impactX_HGCal_layer_4, &b_impactX_HGCal_layer_4);
   fChain2->SetBranchAddress("impactY_HGCal_layer_4", &impactY_HGCal_layer_4, &b_impactY_HGCal_layer_4);
   fChain2->SetBranchAddress("impactX_HGCal_layer_5", &impactX_HGCal_layer_5, &b_impactX_HGCal_layer_5);
   fChain2->SetBranchAddress("impactY_HGCal_layer_5", &impactY_HGCal_layer_5, &b_impactY_HGCal_layer_5);
   fChain2->SetBranchAddress("impactX_HGCal_layer_6", &impactX_HGCal_layer_6, &b_impactX_HGCal_layer_6);
   fChain2->SetBranchAddress("impactY_HGCal_layer_6", &impactY_HGCal_layer_6, &b_impactY_HGCal_layer_6);
   fChain2->SetBranchAddress("impactX_HGCal_layer_7", &impactX_HGCal_layer_7, &b_impactX_HGCal_layer_7);
   fChain2->SetBranchAddress("impactY_HGCal_layer_7", &impactY_HGCal_layer_7, &b_impactY_HGCal_layer_7);
   fChain2->SetBranchAddress("impactX_HGCal_layer_8", &impactX_HGCal_layer_8, &b_impactX_HGCal_layer_8);
   fChain2->SetBranchAddress("impactY_HGCal_layer_8", &impactY_HGCal_layer_8, &b_impactY_HGCal_layer_8);
   fChain2->SetBranchAddress("impactX_HGCal_layer_9", &impactX_HGCal_layer_9, &b_impactX_HGCal_layer_9);
   fChain2->SetBranchAddress("impactY_HGCal_layer_9", &impactY_HGCal_layer_9, &b_impactY_HGCal_layer_9);
   fChain2->SetBranchAddress("impactX_HGCal_layer_10", &impactX_HGCal_layer_10, &b_impactX_HGCal_layer_10);
   fChain2->SetBranchAddress("impactY_HGCal_layer_10", &impactY_HGCal_layer_10, &b_impactY_HGCal_layer_10);
   fChain2->SetBranchAddress("impactX_HGCal_layer_11", &impactX_HGCal_layer_11, &b_impactX_HGCal_layer_11);
   fChain2->SetBranchAddress("impactY_HGCal_layer_11", &impactY_HGCal_layer_11, &b_impactY_HGCal_layer_11);
   fChain2->SetBranchAddress("impactX_HGCal_layer_12", &impactX_HGCal_layer_12, &b_impactX_HGCal_layer_12);
   fChain2->SetBranchAddress("impactY_HGCal_layer_12", &impactY_HGCal_layer_12, &b_impactY_HGCal_layer_12);
   fChain2->SetBranchAddress("impactX_HGCal_layer_13", &impactX_HGCal_layer_13, &b_impactX_HGCal_layer_13);
   fChain2->SetBranchAddress("impactY_HGCal_layer_13", &impactY_HGCal_layer_13, &b_impactY_HGCal_layer_13);
   fChain2->SetBranchAddress("impactX_HGCal_layer_14", &impactX_HGCal_layer_14, &b_impactX_HGCal_layer_14);
   fChain2->SetBranchAddress("impactY_HGCal_layer_14", &impactY_HGCal_layer_14, &b_impactY_HGCal_layer_14);
   fChain2->SetBranchAddress("impactX_HGCal_layer_15", &impactX_HGCal_layer_15, &b_impactX_HGCal_layer_15);
   fChain2->SetBranchAddress("impactY_HGCal_layer_15", &impactY_HGCal_layer_15, &b_impactY_HGCal_layer_15);
   fChain2->SetBranchAddress("impactX_HGCal_layer_16", &impactX_HGCal_layer_16, &b_impactX_HGCal_layer_16);
   fChain2->SetBranchAddress("impactY_HGCal_layer_16", &impactY_HGCal_layer_16, &b_impactY_HGCal_layer_16);
   fChain2->SetBranchAddress("impactX_HGCal_layer_17", &impactX_HGCal_layer_17, &b_impactX_HGCal_layer_17);
   fChain2->SetBranchAddress("impactY_HGCal_layer_17", &impactY_HGCal_layer_17, &b_impactY_HGCal_layer_17);
   fChain2->SetBranchAddress("impactX_HGCal_layer_18", &impactX_HGCal_layer_18, &b_impactX_HGCal_layer_18);
   fChain2->SetBranchAddress("impactY_HGCal_layer_18", &impactY_HGCal_layer_18, &b_impactY_HGCal_layer_18);
   fChain2->SetBranchAddress("impactX_HGCal_layer_19", &impactX_HGCal_layer_19, &b_impactX_HGCal_layer_19);
   fChain2->SetBranchAddress("impactY_HGCal_layer_19", &impactY_HGCal_layer_19, &b_impactY_HGCal_layer_19);
   fChain2->SetBranchAddress("impactX_HGCal_layer_20", &impactX_HGCal_layer_20, &b_impactX_HGCal_layer_20);
   fChain2->SetBranchAddress("impactY_HGCal_layer_20", &impactY_HGCal_layer_20, &b_impactY_HGCal_layer_20);
   fChain2->SetBranchAddress("impactX_HGCal_layer_21", &impactX_HGCal_layer_21, &b_impactX_HGCal_layer_21);
   fChain2->SetBranchAddress("impactY_HGCal_layer_21", &impactY_HGCal_layer_21, &b_impactY_HGCal_layer_21);
   fChain2->SetBranchAddress("impactX_HGCal_layer_22", &impactX_HGCal_layer_22, &b_impactX_HGCal_layer_22);
   fChain2->SetBranchAddress("impactY_HGCal_layer_22", &impactY_HGCal_layer_22, &b_impactY_HGCal_layer_22);
   fChain2->SetBranchAddress("impactX_HGCal_layer_23", &impactX_HGCal_layer_23, &b_impactX_HGCal_layer_23);
   fChain2->SetBranchAddress("impactY_HGCal_layer_23", &impactY_HGCal_layer_23, &b_impactY_HGCal_layer_23);
   fChain2->SetBranchAddress("impactX_HGCal_layer_24", &impactX_HGCal_layer_24, &b_impactX_HGCal_layer_24);
   fChain2->SetBranchAddress("impactY_HGCal_layer_24", &impactY_HGCal_layer_24, &b_impactY_HGCal_layer_24);
   fChain2->SetBranchAddress("impactX_HGCal_layer_25", &impactX_HGCal_layer_25, &b_impactX_HGCal_layer_25);
   fChain2->SetBranchAddress("impactY_HGCal_layer_25", &impactY_HGCal_layer_25, &b_impactY_HGCal_layer_25);
   fChain2->SetBranchAddress("impactX_HGCal_layer_26", &impactX_HGCal_layer_26, &b_impactX_HGCal_layer_26);
   fChain2->SetBranchAddress("impactY_HGCal_layer_26", &impactY_HGCal_layer_26, &b_impactY_HGCal_layer_26);
   fChain2->SetBranchAddress("impactX_HGCal_layer_27", &impactX_HGCal_layer_27, &b_impactX_HGCal_layer_27);
   fChain2->SetBranchAddress("impactY_HGCal_layer_27", &impactY_HGCal_layer_27, &b_impactY_HGCal_layer_27);
   fChain2->SetBranchAddress("impactX_HGCal_layer_28", &impactX_HGCal_layer_28, &b_impactX_HGCal_layer_28);
   fChain2->SetBranchAddress("impactY_HGCal_layer_28", &impactY_HGCal_layer_28, &b_impactY_HGCal_layer_28);
   fChain2->SetBranchAddress("impactX_HGCal_layer_29", &impactX_HGCal_layer_29, &b_impactX_HGCal_layer_29);
   fChain2->SetBranchAddress("impactY_HGCal_layer_29", &impactY_HGCal_layer_29, &b_impactY_HGCal_layer_29);
   fChain2->SetBranchAddress("impactX_HGCal_layer_30", &impactX_HGCal_layer_30, &b_impactX_HGCal_layer_30);
   fChain2->SetBranchAddress("impactY_HGCal_layer_30", &impactY_HGCal_layer_30, &b_impactY_HGCal_layer_30);
   fChain2->SetBranchAddress("impactX_HGCal_layer_31", &impactX_HGCal_layer_31, &b_impactX_HGCal_layer_31);
   fChain2->SetBranchAddress("impactY_HGCal_layer_31", &impactY_HGCal_layer_31, &b_impactY_HGCal_layer_31);
   fChain2->SetBranchAddress("impactX_HGCal_layer_32", &impactX_HGCal_layer_32, &b_impactX_HGCal_layer_32);
   fChain2->SetBranchAddress("impactY_HGCal_layer_32", &impactY_HGCal_layer_32, &b_impactY_HGCal_layer_32);
   fChain2->SetBranchAddress("impactX_HGCal_layer_33", &impactX_HGCal_layer_33, &b_impactX_HGCal_layer_33);
   fChain2->SetBranchAddress("impactY_HGCal_layer_33", &impactY_HGCal_layer_33, &b_impactY_HGCal_layer_33);
   fChain2->SetBranchAddress("impactX_HGCal_layer_34", &impactX_HGCal_layer_34, &b_impactX_HGCal_layer_34);
   fChain2->SetBranchAddress("impactY_HGCal_layer_34", &impactY_HGCal_layer_34, &b_impactY_HGCal_layer_34);
   fChain2->SetBranchAddress("impactX_HGCal_layer_35", &impactX_HGCal_layer_35, &b_impactX_HGCal_layer_35);
   fChain2->SetBranchAddress("impactY_HGCal_layer_35", &impactY_HGCal_layer_35, &b_impactY_HGCal_layer_35);
   fChain2->SetBranchAddress("impactX_HGCal_layer_36", &impactX_HGCal_layer_36, &b_impactX_HGCal_layer_36);
   fChain2->SetBranchAddress("impactY_HGCal_layer_36", &impactY_HGCal_layer_36, &b_impactY_HGCal_layer_36);
   fChain2->SetBranchAddress("impactX_HGCal_layer_37", &impactX_HGCal_layer_37, &b_impactX_HGCal_layer_37);
   fChain2->SetBranchAddress("impactY_HGCal_layer_37", &impactY_HGCal_layer_37, &b_impactY_HGCal_layer_37);
   fChain2->SetBranchAddress("impactX_HGCal_layer_38", &impactX_HGCal_layer_38, &b_impactX_HGCal_layer_38);
   fChain2->SetBranchAddress("impactY_HGCal_layer_38", &impactY_HGCal_layer_38, &b_impactY_HGCal_layer_38);
   fChain2->SetBranchAddress("impactX_HGCal_layer_39", &impactX_HGCal_layer_39, &b_impactX_HGCal_layer_39);
   fChain2->SetBranchAddress("impactY_HGCal_layer_39", &impactY_HGCal_layer_39, &b_impactY_HGCal_layer_39);
   fChain2->SetBranchAddress("impactX_HGCal_layer_40", &impactX_HGCal_layer_40, &b_impactX_HGCal_layer_40);
   fChain2->SetBranchAddress("impactY_HGCal_layer_40", &impactY_HGCal_layer_40, &b_impactY_HGCal_layer_40);

   fChain2->SetBranchAddress("trackChi2_X", &trackChi2_X, &b_trackChi2_X);
   fChain2->SetBranchAddress("trackChi2_Y", &trackChi2_Y, &b_trackChi2_Y);
   fChain2->SetBranchAddress("dwcReferenceType", &dwcReferenceType, &b_dwcReferenceType);
   fChain2->SetBranchAddress("m_x", &m_x, &b_m_x);
   fChain2->SetBranchAddress("m_y", &m_y, &b_m_y);
   fChain2->SetBranchAddress("b_x", &b_x, &b_b_x);
   fChain2->SetBranchAddress("b_y", &b_y, &b_b_y);

   Notify();
}

Bool_t HGCNtupleVariables::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef HGCNtupleVariables_cxx
