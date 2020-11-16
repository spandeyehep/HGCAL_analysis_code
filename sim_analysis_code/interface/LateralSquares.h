#ifndef  LateralSquares_h
#define  LateralSquares_h

#include <TROOT.h>
#include "../interface/RecHit_AHCAL.h"
#include "vector"
#include "map"
#include <iostream>

using namespace std;

class  LateralSquares {
 public:
   LateralSquares();
   LateralSquares(std::vector<RecHit_AHCAL*>);
  virtual ~ LateralSquares();
  int nRecHits_;
  std::vector<RecHit_AHCAL*> rechits_;
  std::map<std::pair<int,int>, float> rechit_map;
  int layer_;
  int seedIndex;
  std::pair<int,int> seed_IJ;
  //Long64_t event_;
  Double_t seed_energy;
  bool setSeedIndex();
  bool setSeed_IJ();
  int getSeedIndex();
  bool setSeedEnergy();
  Double_t getSeedEnergy();

  Double_t energy_square1;
  Double_t energy_square2;
  bool setEnergyInSquare1();
  bool setEnergyInSquare2();
  bool createRechitMap();
  

  
  Double_t getEnergyInSquare1();
  Double_t getEnergyInSquare2();

  bool setNRechits(int nRecHits__);
  int getNRechits();
  bool setLayer(int Layer__);
  int getLayer();
  // bool setEvent(Long64_t event__);
  // Long64_t getEvent();
  float findRechit_getEnergy(int I, int J);


  
  bool setRechitCollection(std::vector<RecHit_AHCAL*> rechit_collection);
  /* bool AddRechitElement(RecHit_AHCAL* rechit_el); */
  std::vector<RecHit_AHCAL*> getRechitCollection();
  //bool setOwnInfo(std::vector<RecHit*> full_rechit_collection);
  
  
};

#endif

#ifdef  LateralSquares_cxx


LateralSquares:: LateralSquares() { }
LateralSquares:: LateralSquares(std::vector<RecHit_AHCAL*> cons___) { 
  rechits_ = cons___;

  if(rechits_.size() > 0) {
    setRechitCollection(rechits_);
    setNRechits((int)rechits_.size());
    createRechitMap();
    setSeedIndex();
    setSeed_IJ();
    setSeedEnergy();
    setEnergyInSquare1();
    setEnergyInSquare2();
  }
  else {
    setLayer(-1);
    setSeedEnergy();
    energy_square1 = 0.0;
    energy_square2 = 0.0;
    seedIndex = -1.0;
    seed_IJ = std::make_pair(-999,-999);

  }
}
LateralSquares::~ LateralSquares() { }

bool LateralSquares::createRechitMap(){
  std::pair<int,int> temp_pair;
  std::pair<std::pair<int,int>, float > temp_map;
  for(int i = 0; i < (int)rechits_.size(); i++) {
    int I = rechits_.at(i)->getCellCoordinate_IJ()[0];
    int J = rechits_.at(i)->getCellCoordinate_IJ()[1];
    temp_pair = std::make_pair(I,J);
    temp_map = std::make_pair(temp_pair, rechits_.at(i)->getEnergy());
    rechit_map.insert(temp_map);
  }
  return true;
}

float LateralSquares::findRechit_getEnergy(int I, int J) {
  std::pair<int,int> IJ = std::make_pair(I,J); 
  std::map<std::pair<int,int>,float >::iterator it = rechit_map.find(IJ);
  if(it != rechit_map.end()){
    return it->second;
  }
  else {
    return -1.0;
  }
}

bool LateralSquares::setEnergyInSquare1() {
  std::vector<std::pair<int,int> > s1;
  s1.push_back(std::make_pair(1,0));
  s1.push_back(std::make_pair(-1,0));
  s1.push_back(std::make_pair(0,1));
  s1.push_back(std::make_pair(0,-1));
  s1.push_back(std::make_pair(1,1));
  s1.push_back(std::make_pair(1,-1));
  s1.push_back(std::make_pair(-1,1));
  s1.push_back(std::make_pair(-1,-1));
  energy_square1 = 0.0;
  int seed_I = seed_IJ.first;
  int seed_J = seed_IJ.second;
  for(int i = 0; i< (int)s1.size(); i++) {
    int I = seed_I + s1[i].first;
    int J = seed_J + s1[i].second;
    
    if(I > 24 || I < 1 || J > 24 || J < 1) continue;
    if(findRechit_getEnergy(I,J) < 0.0) continue;
    energy_square1 += findRechit_getEnergy(I,J);
  }
  return true;
}

Double_t LateralSquares::getEnergyInSquare1() { return energy_square1; }


bool LateralSquares::setEnergyInSquare2() {
  std::vector<std::pair<int,int> > s2;
  s2.push_back(std::make_pair(0,2));
  s2.push_back(std::make_pair(0,-2));
  s2.push_back(std::make_pair(2,0));
  s2.push_back(std::make_pair(-2,0));

  s2.push_back(std::make_pair(-1,2));
  s2.push_back(std::make_pair(1,2));
  s2.push_back(std::make_pair(-1,-2));
  s2.push_back(std::make_pair(1,-2));

  s2.push_back(std::make_pair(-2,1));
  s2.push_back(std::make_pair(2,1));
  s2.push_back(std::make_pair(-2,-1));
  s2.push_back(std::make_pair(2,-1));

  s2.push_back(std::make_pair(-2,2));
  s2.push_back(std::make_pair(2,2));
  s2.push_back(std::make_pair(-2,-2));
  s2.push_back(std::make_pair(2,-2));


  energy_square2 = 0.0;
  int seed_I = seed_IJ.first;
  int seed_J = seed_IJ.second;

 for(int i = 0; i< (int)s2.size(); i++) {
    int I = seed_I + s2[i].first;
    int J = seed_J + s2[i].second;
    if(I > 24 || I < 1 || J > 24 || J < 1) continue;
    if(findRechit_getEnergy(I,J) < 0.0) continue;
    else energy_square2 += findRechit_getEnergy(I,J);
  }

  return true;
}

Double_t LateralSquares::getEnergyInSquare2() { return energy_square2; }



bool  LateralSquares::setNRechits(int nRecHits__) {
  nRecHits_ = nRecHits__;
  return true;
}

bool  LateralSquares::setLayer(int Layer__) {
  layer_ = Layer__;
  return true;
}

// bool  LateralSquares::setEvent(Long64_t event__) {
//   event_ = event__;
//   return true;
// }


bool LateralSquares::setSeedIndex(){
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
bool LateralSquares::setSeed_IJ() {
  seed_IJ = std::make_pair((rechits_.at(seedIndex)->getCellCoordinate_IJ())[0], (rechits_.at(seedIndex)->getCellCoordinate_IJ())[1]);
  return true;
}
int LateralSquares::getSeedIndex() { return seedIndex; }
bool LateralSquares::setSeedEnergy() { 
  //if(seedIndex < 0)
  if(rechits_.size() > 0 ) seed_energy = rechits_.at(seedIndex)->getEnergy(); 
  else seed_energy = 0.0;
}

int  LateralSquares::getNRechits() { return nRecHits_; }
int  LateralSquares::getLayer() {  return layer_; }
Double_t LateralSquares::getSeedEnergy() { return seed_energy; }



bool  LateralSquares::setRechitCollection(std::vector<RecHit_AHCAL*> rechit_collection) {
  rechits_ = rechit_collection;
  return true;
}


/* bool  LateralSquares::AddRechitElement(RecHit_AHCAL* rechit_el) { */
/*   rechits_.push_back(rechit_el); */
/*   return true; */
/* } */

std::vector<RecHit_AHCAL*>  LateralSquares::getRechitCollection() { return rechits_;}


// bool  LateralSquares::setOwnInfo(std::vector<RecHit*> full_rechit_collection) {
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
