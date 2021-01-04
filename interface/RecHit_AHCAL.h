#ifndef RecHit_AHCAL_h
#define RecHit_AHCAL_h

#include "vector"
using namespace std;

class RecHit_AHCAL {
 public:
  RecHit_AHCAL();
  RecHit_AHCAL(int layer__, int hitI__, int hitJ__,
	 float x__, float y__, float z__, float energy__);
  virtual ~RecHit_AHCAL() {};
  int hitI_;
  int hitJ_;
  int layer_;
  float x_;
  float y_;
  float z_;
  float energy_;
  int getLayer();
  std::vector<int> getCellCoordinate_IJ();
  std::vector<float> getCellCoordinate_xyz();
  //std::vector<float> getRechitPosition();
  
  //bool setDelta(float del = 16.0);

  float getEnergy();
};

#endif

#ifdef RecHit_AHCAL_cxx

RecHit_AHCAL::RecHit_AHCAL(int layer__, int hitI__, int hitJ__,
	 float x__, float y__, float z__, float energy__) {

  layer_ = layer__;
  hitI_ = hitI__;
  hitJ_ = hitJ__;
  x_ = x__;
  y_ = y__;
  z_ = z__;
  energy_ = energy__;
    
}



std::vector<int> RecHit_AHCAL::getCellCoordinate_IJ(){
  std::vector<int> temp__;
  temp__.push_back(hitI_);
  temp__.push_back(hitJ_);
  return temp__;
}

std::vector<float> RecHit_AHCAL::getCellCoordinate_xyz(){
  std::vector<float> temp__;
  temp__.push_back(x_);
  temp__.push_back(y_);
  temp__.push_back(z_);
  return temp__;
}




int RecHit_AHCAL::getLayer(){ return layer_;}
float RecHit_AHCAL::getEnergy(){ return energy_;}


#endif
