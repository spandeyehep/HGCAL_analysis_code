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
