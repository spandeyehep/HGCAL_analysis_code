#ifndef RecHit_h
#define RecHit_h

#include "vector"
using namespace std;

class RecHit {
 public:
  RecHit();
  RecHit(int layer__, int chip__, int channel__, int encoded_chan__,
	 bool isPartOfFH__, int daisy_location__, int module_number__, float iu__, float iv__,
	 float x__, float y__, float z__, float energy__, bool isPartOfcluster__, int clusterType__);
  virtual ~RecHit() {};
  int layer_;
  int chip_;
  int channel_;
  int encoded_chan_;
  bool isPartOfFH_;
  int daisy_location_;
  int module_number_;
  float iu_;
  float iv_;
  float x_;
  float y_;
  float z_;
  float energy_;
  bool isPartOfcluster_;
  int clusterType_;
  float density_;
  float delta_;
  int getLayer();
  int getEncodedChannel();
  std::vector<float> getCellCoordinate_uv();
  std::vector<float> getCellCoordinate_xyz();
  //std::vector<float> getRechitPosition();
  
  bool setClusterType(int type_);
  bool setDensity(float den);
  //bool setDelta(float del = 16.0);
  bool setDelta(float del);
  float getDensity();
  float getDelta();

  float getEnergy();
  bool isPartOfFH();
  int getDaisyLocation();
  int getModuleNumber();
  bool isPartOfCluster();

  int getClusterType();
};

#endif

#ifdef RecHit_cxx

RecHit::RecHit(int layer__, int chip__, int channel__, int encoded_chan__,
	       bool isPartOfFH__, int daisy_location__, int module_number__, float iu__, float iv__,
	       float x__, float y__, float z__, float energy__, bool isPartOfcluster__, int clusterType__) {

  layer_ = layer__;
  chip_ = chip__;
  channel_ = channel__;
  encoded_chan_ = encoded_chan__;
  isPartOfFH_ = isPartOfFH__;
  daisy_location_ = daisy_location__;
  module_number_ = module_number__;
  iu_ = iu__;
  iv_ = iv__;
  x_ = x__;
  y_ = y__;
  z_ = z__;
  energy_ = energy__;
  isPartOfcluster_ = isPartOfcluster__;
  clusterType_ = clusterType__;
  delta_ = 16.0;
    
}



bool RecHit::setClusterType(int type_) { clusterType_ = type_; return 1;}
std::vector<float> RecHit::getCellCoordinate_uv(){
  std::vector<float> temp__;
  temp__.push_back(iu_);
  temp__.push_back(iv_);
  return temp__;
}

std::vector<float> RecHit::getCellCoordinate_xyz(){
  std::vector<float> temp__;
  temp__.push_back(x_);
  temp__.push_back(y_);
  temp__.push_back(z_);
  return temp__;
}


bool RecHit::setDensity(float den) {
  density_ = den;
}
bool RecHit::setDelta(float del /*= 16.0*/) {
  delta_ = del;
}
float RecHit::getDensity() { return density_;}
float RecHit::getDelta() { return delta_;}



int RecHit::getLayer(){ return layer_;}
int RecHit::getEncodedChannel(){ return encoded_chan_;}
float RecHit::getEnergy(){ return energy_;}
int RecHit::getDaisyLocation(){ return daisy_location_;}
int RecHit::getModuleNumber(){ return module_number_;}
bool RecHit::isPartOfFH(){ return isPartOfFH_;}
bool RecHit::isPartOfCluster(){ return isPartOfcluster_;}
int RecHit::getClusterType(){return clusterType_;}


#endif
