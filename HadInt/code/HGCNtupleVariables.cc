#define HGCNtupleVariables_cxx
#include "HGCNtupleVariables.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
//#include <TProfile.h>

using namespace std;



float HGCNtupleVariables::deltaR(float x1, float y1, float x2, float y2) {
  float deltaR_ = sqrt(pow((x2-x1),2) + pow((y2-y1),2));
  return deltaR_;
}


int HGCNtupleVariables::getBIN(unsigned int skiroc,unsigned int channel) {
  if (channel%2 != 0 || channel > 62 || skiroc > 3) return -1;
  else return (skiroc*32 + (channel/2));
}


int HGCNtupleVariables::getNextLayer(float had_z_) {

  
  //float delta_z_ = 100000.0;
    for(int i = 1; i <= 79; i++){
      float cm_ = layer_positions[i].at(0);
       if(had_z_ < cm_) return i;
      // return i;
      // if(had_z_ > cm_) continue;
      // return i;
    }


    return -1;
  // if(had_z_ < 0.0) return 1;
  // else {
  //   float delta_z_ = 100000.0;
  //   for(int i = 1; i <= 79; i++){
  //     float cm_ = layer_positions[i].at(0);
  //     if(had_z_ > cm_) continue;
  //     return i;
  //   }
  // }
  // return 111;
}


std::vector<int> HGCNtupleVariables::getModuleLocation(int moduleID) {
  std::map<int, std::vector<int>>::iterator it = module_map.find(moduleID);
  if(it != module_map.end()){
    return it->second;
  }
  else {
    return std::vector<int>();
  }
}

std::pair<float,float> HGCNtupleVariables::dxy_alignment(int layer) {
  //int temp_find = layer;
  std::pair<float,float> temp_dXY = std::make_pair(-999.0,-999.0);

  //std::map<std::pair<int,int>, std::pair<float,float> >::iterator it = align_map.find(temp_find);
  std::map<int, std::pair<float,float> >::iterator it = align_map.find(layer);
  if(it != align_map.end()) {
    temp_dXY = it->second;
  }
  else {
    std::cout<<"Value NOT found for layer = "<<layer<<std::endl;
  }
  return temp_dXY;
}

float HGCNtupleVariables::getNoise(std::pair<int,int> mod_chip) {
  //int temp_find = layer;
  //std::pair<float,float> temp_dXY = std::make_pair(-999.0,-999.0);
  float temp_noise = -1.0;
  //std::map<std::pair<int,int>, std::pair<float,float> >::iterator it = align_map.find(temp_find);
  std::map<std::pair<int,int>, float >::iterator it = noise_map.find(mod_chip);
  if(it != noise_map.end()) {
    temp_noise = it->second;
  }
  else {
    std::cout<<"Value NOT found for Module = "<<mod_chip.first<<" & chip = "<<mod_chip.second<<std::endl;
  }
  return temp_noise;
}


float HGCNtupleVariables::getMIPRatio(std::pair<int,int> mod_chip) {
  //int temp_find = layer;
  //std::pair<float,float> temp_dXY = std::make_pair(-999.0,-999.0);
  float temp_ratio = -1.0;
  //std::map<std::pair<int,int>, std::pair<float,float> >::iterator it = align_map.find(temp_find);
  std::map<std::pair<int,int>, float >::iterator it = mip_ratio_map.find(mod_chip);
  if(it != mip_ratio_map.end()) {
    temp_ratio = it->second;
  }
  else {
    //std::cout<<"Value NOT found for Module = "<<mod_chip.first<<" & chip = "<<mod_chip.second<<std::endl;
  }
  return temp_ratio;
}

double HGCNtupleVariables::getLeadingKE(vector<double> list) {
  double max = -1.0;
  for(int i = 0; i < list.size(); i++) {
    if(list.at(i) > max) max = list.at(i);
  }
  return max;
}

//double HGCNtupleVariables::shower_comparisons(TProfile* shower, TH1F* hist) {
// TH1F* HGCNtupleVariables::shower_comparisons(TProfile* shower, TH1F* hist) {
//   // TFile* file0 = TFile::Open("/home/shubham/work/HGCAL/CERNTB/niramay/niramay_github/forked_repo/E_Pi_Comparison/functions/Template_150_GeV.root");
//   // TH1F* ref_hist = (TH1F*)file0->Get("hist_ref_template");
//   //TH1F* hist = new TH1F("hist","hist",30,0,30);
//   for(int i = 1; i <= shower->GetNbinsX(); i++){
//     hist->SetBinContent(i,shower->GetBinContent(i));
//   }
//   return hist;
//   //return (hist->Chi2Test(ref_h,"CHI2/NDF"));
// }


// float HGCNtupleVariables::find_official_calib(int layer, int en_chan){
//   std::pair<int, int> temp_find;
//   temp_find = std::make_pair(layer,en_chan);
//   std::map<std::pair<int,int>, float >::iterator it = offical_calib_map.find(temp_find);
//   if(it != offical_calib_map.end()){
//     return it->second;
//   }
//   else {
//     return -1.0;
//   }
// }


// float HGCNtupleVariables::find_my_calib(int layer, int en_chan){
//   std::pair<int, int> temp_find;
//   temp_find = std::make_pair(layer,en_chan);
//   std::map<std::pair<int,int>, float >::iterator it = my_calib_map.find(temp_find);
//   if(it != my_calib_map.end()){
//     return it->second;
//   }
//   else {
//     return -1.0;
//   }
// }


