#define HGCNtupleVariables_cxx
#include "HGCNtupleVariables.h"
#include "RecHit.h"
#include "Cluster2D.h"
#include "KDTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TMath.h"
#include <iostream>

//#include <TProfile.h>
using namespace std;



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

float HGCNtupleVariables::R_ij(float ui_, float vi_, float uj_, float vj_) {
  //float x = TMath::Sqrt(pow((abs(ui-uj),2)) + pow((abs(vi-vj),2)));
  return TMath::Sqrt(pow(abs(ui_-uj_),2) + pow(abs(vi_-vj_),2));
}

int getMaxIndex(std::vector<float> list_) {
  float max_ = -1.0;
  int max_i = -1.0;
  for(int i = 0; i < (int)list_.size(); i++){
    if (list_.at(i) > max_) {
      max_ = list_.at(i);
      max_i = i;
    }
  }
  return max_i;
}

float getMin(std::vector<float> list_) {
  float min_ = 1.0e10;
  int min_i = -1.0;
  for(int i = 0; i < (int)list_.size(); i++){
    if (list_.at(i) < min_) {
      min_ = list_.at(i);
      min_i = i;
    }
  }
  return min_;

}
std::vector<RecHit*> HGCNtupleVariables::sort_rechits(std::vector<RecHit*> rechits_) {
  // Bubble SORT algorithm. Might want to improve later on (i.e. by using QuickSort)
  for(int i = 0; i < (int)rechits_.size()-1; i++) {
    for(int j = 0; j < (int)rechits_.size()-i-1; j++) {
      if(rechits_.at(j)->getEnergy() < rechits_.at(j+1)->getEnergy()) {
	RecHit *temp = rechits_.at(j);
	rechits_.at(j) = rechits_.at(j+1);
	rechits_.at(j+1) =  temp;
      }
    }
  }
  return rechits_;
}



std::vector<Cluster2D*> HGCNtupleVariables::ClusterProducer(std::vector<RecHit*> rechits_coll_) {
  //sort rechits according to cell_energy()
  rechits_coll_ = sort_rechits(rechits_coll_);


  int nSeeds_ = 0;
  pointVec points;
  point_t pt;
  std::vector<Cluster2D*> cluster_coll_;

  /////////////////////////////
  // S E E D       F I N D E R
  /////////////////////////////

  for(int i = 0; i < (int)rechits_coll_.size(); i++) {
    if((rechits_coll_.at(i)->getDensity() > 1.5) && (rechits_coll_.at(i)->getDelta() > 2.0)) {
      nSeeds_++;
      double x = -100.0, y = -100.0;
      x = rechits_coll_.at(i)->getCellCoordinate_xyz().at(0);
      y = rechits_coll_.at(i)->getCellCoordinate_xyz().at(1);
      pt = {x,y};
      points.push_back(pt);

      std::vector<RecHit*> temp_rechit;
      temp_rechit.clear();
      Cluster2D* temp_cluster = new Cluster2D(temp_rechit);
      temp_cluster->setClusterRank(nSeeds_);
      temp_cluster->AddRechitElement(rechits_coll_.at(i));
      cluster_coll_.push_back(temp_cluster);

      
    }
  }

  // std::cout<<"DEBUG: Number of Unique cluster = "<<cluster_coll_.size()<<endl;;
  // std::cout<<"points size = "<<points.size()<<endl;
  KDTree tree(points);
  // std::cout<<"INFO: Iterating over all rechits to assign clusters!!"<<endl;
  // cout<<"#\t x\t y\t Nearest index"<<endl;

  ////////////////////////////////////////
  // C L U S T E R     A S S I G N M E N T
  ///////////////////////////////////////

  for(int i = 0; i < (int)rechits_coll_.size(); i++) {
    // std::vector<RecHit*> temp_rechit;
    // temp_rechit.clear();
    // Cluster2D* temp_cluster = new Cluster2D(temp_rechit);
    // temp_cluster->setClusterRank(nSeeds_);
    // temp_rechit.push_back(rechits_coll_.at(i));
    // temp_cluster->setRechitCollection(temp_rechit);
    // cluster_coll_.push_back(temp_cluster);
    
    double x = -100.0, y = -100.0;
    x = rechits_coll_.at(i)->getCellCoordinate_xyz().at(0);
    y = rechits_coll_.at(i)->getCellCoordinate_xyz().at(1);
    pt = {x,y};
    auto NN_seed_index = tree.nearest_index(pt);
    auto res = tree.nearest_point(pt);
    auto distance = R_ij(x,y,res.at(0),res.at(1));
    if(!distance) continue;
    cluster_coll_[NN_seed_index]->AddRechitElement(rechits_coll_.at(i));

    ////////////////////////////////////////////////
    // S E T  C L U S T E R     I N F O M A T I O N 
    ///////////////////////////////////////////////

    for(int i = 0; i < nSeeds_; i++) {
      if(!cluster_coll_.at(i)->setOwnInfo(cluster_coll_.at(i)->getRechitCollection())) {
	std::cout<<"ERROR: Couldn't initilize cluster collection"<<endl;
      }
    }
    
    //cluster_coll_.push_back(temp_cluster);
      
    //cout<<i+1<<"\t "<<x<<"\t "<<y<<"\t "<<;
    // cout<<i+1<<"\t "<<x<<"\t "
    // 	<<y<<"\t "<<res.at(0)<<"\t "<<res.at(1)<<"\t "
    // 	<<R_ij(x,y,res.at(0),res.at(1))<<endl;
    // for (double b : res) {
    //   std::cout << b << " ";
    // }
    // cout<<endl;
    // cout<<i+1<<"\t "<<x<<"\t "<<y<<"\t "
    // 	<<NN_seed_index+1<<"\t "<<distance<<endl;
    //cout<<"Nearest index = "<<NN_seed_index+1<<std::endl;

  }
  
  return cluster_coll_;
  
}


// New way (with density)
std::vector<RecHit*> HGCNtupleVariables::makeCluster(std::vector<RecHit*> rechits_1) {
  std::vector<float> density_i;
  for(int i = 0; i < (int)rechits_1.size(); i++) {
    float temp_density = 0.0;
    RecHit* temp_rechit_i = rechits_1.at(i);
    float xi = temp_rechit_i->getCellCoordinate_xyz().at(0);
    float yi = temp_rechit_i->getCellCoordinate_xyz().at(1);
    float k = 1.0;
    for(int j = 0; j < (int)rechits_1.size(); j++) {
      RecHit* temp_rechit_j = rechits_1.at(j);
      float E_j = temp_rechit_j->getEnergy();
      float xj = temp_rechit_j->getCellCoordinate_xyz().at(0);
      float yj = temp_rechit_j->getCellCoordinate_xyz().at(1);
      float r_ij = R_ij(xi,yi,xj,yj);
      if(abs(r_ij) > 2.0) continue;
      temp_density += (E_j)*exp(-r_ij/k);
    }
    //std::cout<<"temp_density= "<<temp_density<<endl;
    density_i.push_back(temp_density);
    rechits_1.at(i)->setDensity(temp_density);
  }
  int max_i = getMaxIndex(density_i);
  // if(max_i < 0) {
  //   std::cout<<"INFO: something went wrong at finding highest density"<<endl;
  // }
  // else {
  //   std::cout<<"INFO: Highest density rechit is rechit #"<<max_i+1<<endl;

  //   std::cout<<"INFO: and delta is: "<<rechits_1.at(max_i)->getDelta()<<endl;
    
  // }
  
  for(int i = 0; i < (int)rechits_1.size(); i++) {
    //cout<<i<<"\t hello1"<<endl;
    if(i == max_i) {
       continue; } 
    //cout<<"hello2"<<endl;
    std::vector<float> temp_delta;
    float xi = rechits_1.at(i)->getCellCoordinate_xyz().at(0);
    float yi = rechits_1.at(i)->getCellCoordinate_xyz().at(1);
    for(int j = 0; j < (int)rechits_1.size(); j++) {
      //if(j == i ) continue;
      if(rechits_1.at(j)->getDensity() > rechits_1.at(i)->getDensity()) {
	float xj = rechits_1.at(j)->getCellCoordinate_xyz().at(0);
	float yj = rechits_1.at(j)->getCellCoordinate_xyz().at(1);
	temp_delta.push_back(R_ij(xi,yi,xj,yj));
      }
    }
    rechits_1.at(i)->setDelta(getMin(temp_delta));
    // if(!getMin(temp_delta))
    //   rechits_1.at(i)->setDelta(16.0);
    // else
    //   rechits_1.at(i)->setDelta(getMin(temp_delta));
    //std::cout<<"Rechit #"<<i+1<<"\t "<<getMin(temp_delta)<<endl;
  }
  return rechits_1;
}


// OLD WAY (like PF seed)
/*
void HGCNtupleVariables::makeCluster(std::vector<std::vector<float>> list_) {
  std::map<int, std::vector<float>> primary_cluster;
  int iu_list[] = {0.0,1.0,0.0,-1.0,1.0,0.0,-1.0};
  int iv_list[] = {0.0,-1.0,-1.0,0.0,0.0,1.0,1.0};
  float seed_energy = -1.0;
  int seed_i = -1;
  std::vector<float> seed_info;
  std::map<>
  if(list_size() == 0) {
    cout<<"INFO: Nothing to cluster!!! Returning..."<<endl;
    return;
  }
  for(int i= 0 ; i< list_size(); i++) {
    std::vector<float> temp_l = list_.at(i);
    if(seed_energy < temp_l.at(3)) {
      seed_energy = temp_l.at(3);
      seed_i = i;
      seed_info = list_.at(i);
    }
  }
  if(seed_energy < 0 || seed_i < 0) {
    cout<<"INFO: Something went wrong!!!"<<endl;
    return;
  }
  else {
    cout<<"INFO: Found Seed at channel = " <<seed_info.at(0)<<endl;
  }
  cout<<"INFO: CREATING local cluster out of "<<list_size()<<" rechits"<<endl;

  for(int i= 1 ; i < 7; i++) {
    
    for(int j = 0 ; j < list_copy.size(); j++) {
    }
  }
}
*/

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


