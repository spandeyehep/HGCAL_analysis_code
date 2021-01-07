#ifndef RecHit_h
#define RecHit_h

#include "vector"
using namespace std;

class RecHit {
 public:
  RecHit();
  RecHit(int layer__, int chip__, int channel__, int encoded_chan__,
	 bool isPartOfFH__, int daisy_location__, int module_number__, int iu__, int iv__,
	 float x__, float y__, float z__, float energy__);
  virtual ~RecHit() {};
  int layer_;
  int chip_;
  int channel_;
  int encoded_chan_;
  bool isPartOfFH_;
  int daisy_location_;
  int module_number_;
  int iu_;
  int iv_;
  float x_;
  float y_;
  float z_;
  float energy_;
  int getLayer();
  int getEncodedChannel();
  std::vector<int> getCellCoordinate_uv();
  std::vector<float> getCellCoordinate_xyz();
  //std::vector<float> getRechitPosition();
  
  //bool setDelta(float del = 16.0);

  float getEnergy();
  bool isPartOfFH();
  int getDaisyLocation();
  int getModuleNumber();
};

#endif

#ifdef RecHit_cxx

RecHit::RecHit(int layer__, int chip__, int channel__, int encoded_chan__,
	       bool isPartOfFH__, int daisy_location__, int module_number__, int iu__, int iv__,
	       float x__, float y__, float z__, float energy__) {

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
    
}



std::vector<int> RecHit::getCellCoordinate_uv(){
  std::vector<int> temp__;
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




int RecHit::getLayer(){ return layer_;}
int RecHit::getEncodedChannel(){ return encoded_chan_;}
float RecHit::getEnergy(){ return energy_;}
int RecHit::getDaisyLocation(){ return daisy_location_;}
int RecHit::getModuleNumber(){ return module_number_;}
bool RecHit::isPartOfFH(){ return isPartOfFH_;}


#endif
