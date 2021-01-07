#ifndef  LateralRings_h
#define  LateralRings_h

#include <TROOT.h>
#include "RecHit.h"
#include "vector"
#include "map"
#include <iostream>

using namespace std;

class  LateralRings {
 public:
   LateralRings();
   LateralRings(std::vector<RecHit*>);
  virtual ~ LateralRings();
  int nRecHits_;
  std::vector<RecHit*> rechits_;
  std::map<std::pair<int,int>, float> rechit_map;
  int layer_;
  int seedIndex;
  std::pair<int,int> seed_iuiv;
  //Long64_t event_;
  Double_t seed_energy;
  bool setSeedIndex();
  bool setSeed_iuiv();
  int getSeedIndex();
  bool setSeedEnergy();
  Double_t getSeedEnergy();

  Double_t energy_ring1;
  Double_t energy_ring2;
  bool setEnergyInRing1();
  bool setEnergyInRing2();
  bool createRechitMap();
  

  
  Double_t getEnergyInRing1();
  Double_t getEnergyInRing2();

  bool setNRechits(int nRecHits__);
  int getNRechits();
  bool setLayer(int Layer__);
  int getLayer();
  // bool setEvent(Long64_t event__);
  // Long64_t getEvent();
  float findRechit_getEnergy(int iu, int iv);


  
  bool setRechitCollection(std::vector<RecHit*> rechit_collection);
  bool AddRechitElement(RecHit* rechit_el);
  std::vector<RecHit*> getRechitCollection();
  //bool setOwnInfo(std::vector<RecHit*> full_rechit_collection);
  
  
};

#endif

#ifdef  LateralRings_cxx


LateralRings:: LateralRings() { }
LateralRings:: LateralRings(std::vector<RecHit*> cons___) { 
  rechits_ = cons___;

  if(rechits_.size() > 0) {
    setRechitCollection(rechits_);
    setNRechits((int)rechits_.size());
    createRechitMap();
    setSeedIndex();
    setSeed_iuiv();
    setSeedEnergy();
    setEnergyInRing1();
    setEnergyInRing2();
  }
  else {
    setLayer(-1);
    setSeedEnergy();
    energy_ring1 = 0.0;
    energy_ring2 = 0.0;
    seedIndex = -1.0;
    seed_iuiv = std::make_pair(-999,-999);

  }
}
LateralRings::~ LateralRings() { }

bool LateralRings::createRechitMap(){
  std::pair<int,int> temp_pair;
  std::pair<std::pair<int,int>, float > temp_map;
  for(int i = 0; i < (int)rechits_.size(); i++) {
    int iu = rechits_.at(i)->getCellCoordinate_uv()[0];
    int iv = rechits_.at(i)->getCellCoordinate_uv()[1];
    temp_pair = std::make_pair(iu,iv);
    temp_map = std::make_pair(temp_pair, rechits_.at(i)->getEnergy());
    rechit_map.insert(temp_map);
  }
  return true;
}

float LateralRings::findRechit_getEnergy(int iu, int iv) {
  std::pair<int,int> iuiv = std::make_pair(iu,iv); 
  std::map<std::pair<int,int>,float >::iterator it = rechit_map.find(iuiv);
  if(it != rechit_map.end()){
    return it->second;
  }
  else {
    return -1.0;
  }
}

bool LateralRings::setEnergyInRing1() {
  std::vector<std::pair<int,int> > r1;
  r1.push_back(std::make_pair(0,-1));
  r1.push_back(std::make_pair(1,-1));
  r1.push_back(std::make_pair(1,0));
  r1.push_back(std::make_pair(0,1));
  r1.push_back(std::make_pair(-1,1));
  r1.push_back(std::make_pair(-1,0));
  energy_ring1 = 0.0;
  int seed_iu = seed_iuiv.first;
  int seed_iv = seed_iuiv.second;
  for(int i = 0; i< (int)r1.size(); i++) {
    int iu = seed_iu + r1[i].first;
    int iv = seed_iv + r1[i].second;
    if(abs(iu) > 5 || abs(iv) > 5) continue;
    if(findRechit_getEnergy(iu,iv) < 0.0) continue;
    else energy_ring1 += findRechit_getEnergy(iu,iv);
  }
  return true;
}

Double_t LateralRings::getEnergyInRing1() { return energy_ring1; }


bool LateralRings::setEnergyInRing2() {
  std::vector<std::pair<int,int> > r2;
  r2.push_back(std::make_pair(0,2));
  r2.push_back(std::make_pair(1,-2));
  r2.push_back(std::make_pair(2,-2));
  r2.push_back(std::make_pair(2,-1));
  r2.push_back(std::make_pair(2,0));
  r2.push_back(std::make_pair(1,1));
  r2.push_back(std::make_pair(0,2));
  r2.push_back(std::make_pair(-1,2));
  r2.push_back(std::make_pair(-2,2));
  r2.push_back(std::make_pair(-2,1));
  r2.push_back(std::make_pair(-2,0));
  r2.push_back(std::make_pair(-1,-2));

  energy_ring2 = 0.0;
  int seed_iu = seed_iuiv.first;
  int seed_iv = seed_iuiv.second;

 for(int i = 0; i< (int)r2.size(); i++) {
    int iu = seed_iu + r2[i].first;
    int iv = seed_iv + r2[i].second;
    if(abs(iu) > 5 || abs(iv) > 5) continue;
    if(findRechit_getEnergy(iu,iv) < 0.0) continue;
    else energy_ring2 += findRechit_getEnergy(iu,iv);
  }

  return true;
}

Double_t LateralRings::getEnergyInRing2() { return energy_ring2; }



bool  LateralRings::setNRechits(int nRecHits__) {
  nRecHits_ = nRecHits__;
  return true;
}

bool  LateralRings::setLayer(int Layer__) {
  layer_ = Layer__;
  return true;
}

// bool  LateralRings::setEvent(Long64_t event__) {
//   event_ = event__;
//   return true;
// }


bool LateralRings::setSeedIndex(){
  double max_ = -1.0;
  int max_index_ = -1;
  for(int i = 0; i < (int)rechits_.size(); i++) {
    if(rechits_.at(i)->getEnergy() > max_) {
      max_ = rechits_.at(i)->getEnergy();
      max_index_ = i;
    }
  }
  if(max_index_ < 0) {
    std::cout<<"Could not find Seed"<<std::endl;
    seedIndex = -1;
    return false;
  }
  else  {
    seedIndex = max_index_;
    return true;
  }
}
bool LateralRings::setSeed_iuiv() {
  seed_iuiv = std::make_pair((rechits_.at(seedIndex)->getCellCoordinate_uv())[0], (rechits_.at(seedIndex)->getCellCoordinate_uv())[1]);
  // seed_iuiv.push_back(rechits_.at(seedIndex)->getCellCoordinate_uv[0]);
  // seed_iuiv.push_back(rechits_.at(seedIndex)->getCellCoordinate_uv[1]);
  return true;
}
int LateralRings::getSeedIndex() { return seedIndex; }
bool LateralRings::setSeedEnergy() { 
  //if(seedIndex < 0)
  if(rechits_.size() > 0 ) seed_energy = rechits_.at(seedIndex)->getEnergy(); 
  else seed_energy = 0.0;
}

int  LateralRings::getNRechits() { return nRecHits_; }
int  LateralRings::getLayer() {  return layer_; }
Double_t LateralRings::getSeedEnergy() { return seed_energy; }



bool  LateralRings::setRechitCollection(std::vector<RecHit*> rechit_collection) {
  rechits_ = rechit_collection;
  return true;
}


bool  LateralRings::AddRechitElement(RecHit* rechit_el) {
  rechits_.push_back(rechit_el);
  return true;
}

std::vector<RecHit*>  LateralRings::getRechitCollection() { return rechits_;}


// bool  LateralRings::setOwnInfo(std::vector<RecHit*> full_rechit_collection) {
//   nRecHits_ = full_rechit_collection.size();
//   energy_ = 0;
//   layer_ = full_rechit_collection.at(0)->getLayer();
//   for(int i = 0; i < full_rechit_collection.size(); i++) {
//     energy_+=full_rechit_collection.at(i)->getEnergy();
//     if(layer_ != full_rechit_collection.at(0)->getLayer()) {
//       return false;
//     }
//   }
//   return true;
// }

#endif
