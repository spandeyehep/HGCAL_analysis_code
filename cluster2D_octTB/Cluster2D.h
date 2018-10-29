#ifndef Cluster2D_h
#define Cluster2D_h

#include <TROOT.h>
#include "RecHit.h"
#include "vector"
using namespace std;

class Cluster2D {
 public:
  Cluster2D();
  Cluster2D(std::vector<RecHit*>);
  virtual ~Cluster2D();
  int nRecHits_;
  std::vector<RecHit*> rechits_;
  int layer_;
  //Long64_t event_;
  int clusterRank_;
  Double_t energy_;
  bool setNRechits(int nRecHits__);
  int getNRechits();
  bool setLayer(int Layer__);
  int getLayer();
  // bool setEvent(Long64_t event__);
  // Long64_t getEvent();
  bool setClusterRank(int rank_);
  int getClusterRank();
  bool setClusterEnergy(Double_t energy__);
  Double_t getClusterEnergy();
  bool setRechitCollection(std::vector<RecHit*> rechit_collection);
  bool AddRechitElement(RecHit* rechit_el);
  std::vector<RecHit*> getRechitCollection();
  bool setOwnInfo(std::vector<RecHit*> full_rechit_collection);
  
  
};

#endif

#ifdef Cluster2D_cxx


Cluster2D::Cluster2D() { }
Cluster2D::Cluster2D(std::vector<RecHit*> cons___) { rechits_ = cons___;}
Cluster2D::~Cluster2D() { }

bool Cluster2D::setNRechits(int nRecHits__) {
  nRecHits_ = nRecHits__;
  return true;
}

bool Cluster2D::setLayer(int Layer__) {
  layer_ = Layer__;
  return true;
}

// bool Cluster2D::setEvent(Long64_t event__) {
//   event_ = event__;
//   return true;
// }

bool Cluster2D::setClusterRank(int rank_) {
  clusterRank_ = rank_;
  return true;
}


int Cluster2D::getNRechits() { return nRecHits_; }


int Cluster2D::getLayer() {  return layer_; }

// Long64_t Cluster2D::getEvent() { return event_; }

int Cluster2D::getClusterRank() { return clusterRank_; }


bool Cluster2D::setRechitCollection(std::vector<RecHit*> rechit_collection) {
  rechits_ = rechit_collection;
  return true;
}


bool Cluster2D::AddRechitElement(RecHit* rechit_el) {
  rechits_.push_back(rechit_el);
  return true;
}

std::vector<RecHit*> Cluster2D::getRechitCollection() { return rechits_;}


bool Cluster2D::setOwnInfo(std::vector<RecHit*> full_rechit_collection) {
  nRecHits_ = full_rechit_collection.size();
  energy_ = 0;
  layer_ = full_rechit_collection.at(0)->getLayer();
  for(int i = 0; i < full_rechit_collection.size(); i++) {
    energy_+=full_rechit_collection.at(i)->getEnergy();
    if(layer_ != full_rechit_collection.at(0)->getLayer()) {
      return false;
    }
  }
  return true;
}
// bool Cluster2D::setClusterEnergy(Double_t energy__) {
//   energy_ = energy__;
//   return true;
// }

// Double_t Cluster2D::getClusterEnergy() { return energy_; }






#endif
