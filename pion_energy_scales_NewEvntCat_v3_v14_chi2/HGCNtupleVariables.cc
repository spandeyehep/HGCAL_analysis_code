#define HGCNtupleVariables_cxx
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


std::vector<int> HGCNtupleVariables::getModuleLocation(int moduleID) {
  std::map<int, std::vector<int>>::iterator it = module_map.find(moduleID);
  if(it != module_map.end()){
    return it->second;
  }
  else {
    return std::vector<int>();
  }
}

std::vector<float> HGCNtupleVariables::getLayerPosition(int layer_) {
  std::map<int, std::vector<float>>::iterator it = layer_positions.find(layer_);
  if(it != layer_positions.end()){
    return it->second;
  }
  else {
    return std::vector<float>();
  }
}

std::vector<float> HGCNtupleVariables::getWeights(int beamEnergy_) {
  std::map<int, std::vector<float>>::iterator it = rel_weights.find(beamEnergy_);
  if(it != rel_weights.end()){
    return it->second;
  }
  else {
    return std::vector<float>();
  }
}

std::vector<float> HGCNtupleVariables::getChi2Weights_EH(int beamEnergy_) {
  std::map<int, std::vector<float>>::iterator it = chi2_weights_EH.find(beamEnergy_);
  if(it !=chi2_weights_EH.end()){
    return it->second;
  }
  else {
    return std::vector<float>();
  }
}

std::vector<float> HGCNtupleVariables::getChi2Weights_H(int beamEnergy_) {
  std::map<int, std::vector<float>>::iterator it = chi2_weights_H.find(beamEnergy_);
  if(it !=chi2_weights_H.end()){
    return it->second;
  }
  else {
    return std::vector<float>();
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
  float temp_noise = -1.0;
  std::map<std::pair<int,int>, float >::iterator it = noise_map.find(mod_chip);
  if(it != noise_map.end()) {
    temp_noise = it->second;
  }
  else {
    std::cout<<"Value NOT found for Module = "<<mod_chip.first<<" & chip = "<<mod_chip.second<<std::endl;
  }
  return temp_noise;
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


