#define HGCNtupleVariables_cxx
#include "HGCNtupleVariables.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

//Takes chip number[0-3] and channel number[0-62] as input : Returns h_ADChgp[]'s index [0-127]
int HGCNtupleVariables::getBIN(unsigned int skiroc,unsigned int channel) {
  if (channel%2 != 0 || channel > 62 || skiroc > 3) return -1;
  else return (skiroc*32 + (channel/2));
}

int HGCNtupleVariables::getRunBin(int run_) {
   int runlist[40] ={ 
     132,136,137,138,140,142,143,145,146,147,148,149,150,151,152,153,154,156,157,159,160,161,
     206,207,210,212,213,214,216,217,219,223,224,225,
     387,388,389,390,391,392
   };
   for(int i = 0; i < 40; i++) {
     if(run_ == runlist[i]) return i;
   }
   return -1;
}


float HGCNtupleVariables::deltaR(float x1, float y1, float x2, float y2) {
  float deltaR_ = sqrt(pow((x2-x1),2) + pow((y2-y1),2));
  return deltaR_;
}


Float_t HGCNtupleVariables::min(vector<Float_t> list_) {
  Float_t m = list_.at(0);
  for(unsigned int i = 0; i < list_.size(); i++) {
    if(m > list_.at(i)) m = list_.at(i);
  }
  return m;
}


std::pair<float,float> HGCNtupleVariables::dxy_alignment(int run, int layer) {
  std::pair<int,int> temp_find = std::make_pair(run,layer);
  std::pair<float,float> temp_dXY = std::make_pair(-999.0,-999.0);

  std::map<std::pair<int,int>, std::pair<float,float> >::iterator it = align_map.find(temp_find);
  if(it != align_map.end()) {
    temp_dXY = it->second;
  }
  else {
    std::cout<<"Value NOT found for run = "<< run << " ,layer = "<<layer<<std::endl;
  }
  return temp_dXY;
}
