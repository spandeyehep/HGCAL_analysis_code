#define AnalyzeHGCOctTB_cxx
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

#include <iostream>
#include <vector>
#include <cstring>
#include "../interface/AnalyzeHGCOctTB.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include <math.h>
#include "TStopwatch.h"
#include <TF1.h>

using namespace std;

int main(int argc, char* argv[]) {

  if (argc < 5) {
    cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "configuration" 
    	 <<" " << "energy" << " " <<"event_cat"
    	 << " " << "FH_noise"<<endl;
    
    // cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "configuration" 
    // 	 <<" " << "energy" << endl;

    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *energy = argv[4];
  const char *config          = "alpha";
  
  bool UseSSBasedCat = true;
  int noise_FH_ = 4;

  AnalyzeHGCOctTB hgcOctTB(inputFileList, outFileName, data, config, energy);
  cout << "dataset " << data << " " << endl;
  cout << "configuration " << config << " " << endl;
  cout << "energy " << energy << " " << endl;
  cout << "UseSSBasedEventCategorization " << UseSSBasedCat << " " << endl;
  cout<< "FH_noise "<<noise_FH_<<endl<<endl;

  hgcOctTB.EventLoop(data,UseSSBasedCat,noise_FH_);
  return 0;
}

void AnalyzeHGCOctTB::EventLoop(const char *data, bool UseSSBasedCat, int noise_FH_) {


  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  // Long64_t nentries3 = fChain3->GetEntriesFast();
  // Long64_t hgc_jentry = 0;

  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  // Long64_t nbytes2 = 0, nb2 = 0;
  // Long64_t nbytes3 = 0, nb3 = 0;

  Long64_t region_1_classified_events = 0;
  Long64_t region_2_classified_events = 0;
  Long64_t region_3_classified_events = 0;
  Long64_t region_4_classified_events = 0;
  Long64_t non_classified_events = 0;
  bool UseShowerStartAlgo = UseSSBasedCat;
  bool UseCompartmentEnergy = !UseShowerStartAlgo;


  bool ScaleData = false;
  float data_rescaling = 1.08;

  if(UseShowerStartAlgo && !UseCompartmentEnergy) {
    std::cout<<"Event Categorization based on :"<<BOLDGREEN<<" Shower start Algorithm"<<RESET<<std::endl;
  }

  else if(!UseShowerStartAlgo && UseCompartmentEnergy) {
    std::cout<<"Event Categorization based on :"<<BOLDGREEN<<" EnergySum in CE-E & CE-H"<<RESET<<std::endl;
  }
  else {
    std::cout<<BOLDRED<<"No Switch selected for Event Categorization"<<RESET<<std::endl;
    return;
  }
  
  if(ScaleData) {
    std::cout<<BOLDRED<<"Data rescaling ON, with factor : "<<data_rescaling<<RESET<<std::endl;
  }
  else {
    std::cout<<BOLDRED<<"Data rescaling OFF"<<RESET<<std::endl;
    data_rescaling = 1.0;
  }
  
  int decade = 0;
  Long64_t ahc_zeroHit = 0;
  Long64_t EE_zeroHit = 0;
  Long64_t FH_zeroHit = 0;
  Long64_t EE_zeroHit_R2 = 0;
  int chi2_method = 3;
  // std::map<int,double> E_thres;
  // E_thres[20] = 12.0; E_thres[50] = 20.0; E_thres[80] = 25.0; E_thres[100] = 30.0; E_thres[120] = 30.0; E_thres[200] = 40.0; E_thres[250] = 40.0; E_thres[300] = 40.0;

  std::map<int,double> en_range_EE_xmin;
  std::map<int,double> en_range_EE_xmax;
  std::map<int,double> en_range_FH_xmin;
  std::map<int,double> en_range_FH_xmax;
  en_range_EE_xmin[20] = 8.0; en_range_EE_xmax[20] = 30.0; en_range_FH_xmin[20] = 8.0; en_range_FH_xmax[20] = 30.0;
  en_range_EE_xmin[50] = 28.0; en_range_EE_xmax[50] = 70.0; en_range_FH_xmin[50] = 30.0; en_range_FH_xmax[50] = 70.0;
  en_range_EE_xmin[80] = 50.0; en_range_EE_xmax[80] = 110.0; en_range_FH_xmin[80] = 50.0; en_range_FH_xmax[80] = 110.0;
  en_range_EE_xmin[100] = 65.0; en_range_EE_xmax[100] = 130.0; en_range_FH_xmin[100] = 65.0; en_range_FH_xmax[100] = 135.0;
  en_range_EE_xmin[120] = 80.0; en_range_EE_xmax[120] = 155.0; en_range_FH_xmin[120] = 84.0; en_range_FH_xmax[120] = 155.0;
  en_range_EE_xmin[200] = 145.0; en_range_EE_xmax[200] = 250.0; en_range_FH_xmin[200] = 148.0; en_range_FH_xmax[200] = 250.0;
  en_range_EE_xmin[250] = 190.0; en_range_EE_xmax[250] = 310.0; en_range_FH_xmin[250] = 190.0; en_range_FH_xmax[250] = 310.0;
  en_range_EE_xmin[300] = 230.0; en_range_EE_xmax[300] = 370.0; en_range_FH_xmin[300] = 230.0; en_range_FH_xmax[300] = 370.0;
  

  TF1* f_EH_w1 = new TF1("f_EH_w1","[0]*[0]+[1]*[1]/sqrt(x)", 20, 300);
  f_EH_w1->FixParameter(0,1.025153);
  f_EH_w1->FixParameter(1,1.624093);
  TF1* f_EH_w2 = new TF1("f_EH_w2","[0]*[0]+[1]*[1]/sqrt(x)", 20, 300);
  f_EH_w2->FixParameter(0,0.958412);
  f_EH_w2->FixParameter(1,1.226055);
  TF1* f_EH_w3 = new TF1("f_EH_w3","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_EH_w3->FixParameter(0,1.019201);
  f_EH_w3->FixParameter(1,3.796437);

  TF1* f_H_w1 = new TF1("f_H_w1","[0]*x", 20, 300);
  f_H_w1->FixParameter(0,0.0);
  TF1* f_H_w2 = new TF1("f_H_w2","[0]*[0]+[1]*[1]/sqrt(x)", 20, 300);
  f_H_w2->FixParameter(0,0.908137);
  f_H_w2->FixParameter(1,0.995139);
  TF1* f_H_w3 = new TF1("f_H_w3","sqrt([0]*[0]+[1]*[1]/x)", 20, 300);
  f_H_w3->FixParameter(0,0.977450);
  f_H_w3->FixParameter(1,2.996701);

  

  float lambda[79];
  for(int i = 0; i < 79; i++) {
    lambda[i] = layer_positions[i+1].at(3); //pi_lambda
    //std::cout<<i+1<<"\t"<<lambda[i]<<endl;
  }



  //in mm
  float ahc_pos[39] = {27.45, 53.65, 79.85, 106.05, 132.25, 
		       158.45, 184.65,210.85,237.05, 263.25,
		       289.45, 315.65, 341.85, 368.05, 394.25,
		       420.45, 446.65, 472.85, 499.05, 525.25,
		       551.45, 577.65, 603.85, 630.05, 656.25, 
		       682.45, 708.65, 734.85, 761.05, 787.25,
		       813.45, 839.65, 865.85, 892.05, 918.25, 
		       944.45, 970.65, 996.85, 1075.45};
  //in cm
  //float ahc_front = 169.9;
  float ahc_front = 159.4;     //from Shilpi link: https://twiki.cern.ch/twiki/pub/CMS/HgcalSimulation/config1LengthsVals.txt


  bool DEBUG = false;
  int debug_count = 0;
  Long64_t event_count[10];
  for (int i = 0; i < 10; i++) event_count[i] = 0;
  Long64_t nEvents = 0;
  Long64_t MIP_pions = 0;
  int TOTAL_ACTIVE_LAYER = -1;
  int EE_LAYER = -1;
  int FH_LAYER = -1;
  if(!strcmp(conf_,"alpha") || !strcmp(conf_,"config1")) {
    TOTAL_ACTIVE_LAYER = 40;
    EE_LAYER = 28;
    FH_LAYER = 12;
  }
  else if(!strcmp(conf_,"bravo") || !strcmp(conf_,"config2")){
    TOTAL_ACTIVE_LAYER = 39;
    EE_LAYER = 28;
    FH_LAYER = 11;
  }
  else if(!strcmp(conf_,"charlie") || !strcmp(conf_,"config3")) {
    TOTAL_ACTIVE_LAYER = 20;
    EE_LAYER = 8;
    FH_LAYER = 12;
  }
  else {
    cout<<"ERROR: Unknown configuration!!!!"<<endl;
    return;
  }


  ROOT::Math::SMatrix<double,3, 3, ROOT::Math::MatRepStd<double,3> > coeffs_EH;
  ROOT::Math::SVector<double, 3> consts_EH;
  ROOT::Math::SVector<double, 3> values_EH;
  bool isInverted_EH = false;

  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      coeffs_EH(i,j) = 0.0;
      // coeffs_H(i,j) = 0.0;
    }
    consts_EH(i) = 0.0;
    values_EH(i) = 0.0;
    // consts_H(i) = 0.0;
    // values_H(i) = 0.0;

  }

  ROOT::Math::SMatrix<double,2, 2, ROOT::Math::MatRepStd<double,2> > coeffs_H;
  ROOT::Math::SVector<double, 2> consts_H;
  ROOT::Math::SVector<double, 2> values_H;
  bool isInverted_H = false;

  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      coeffs_H(i,j) = 0.0;
    }
    consts_H(i) = 0.0;
    values_H(i) = 0.0;

  }



  double E_beam = -1.0;


  //For Track window selection cut
  // std::map<int,vector<double>> track_window;
  // vector<double> temp_window;

  // temp_window.clear();
  // temp_window.push_back(-2.8);temp_window.push_back(1.65);temp_window.push_back(-1.2);temp_window.push_back(3.2);
  // track_window[20] = temp_window;

  // temp_window.clear();
  // temp_window.push_back(-2.8);temp_window.push_back(1.65);temp_window.push_back(-1.25);temp_window.push_back(3.2);
  // track_window[50] = temp_window;

  // temp_window.clear();
  // temp_window.push_back(-2.8);temp_window.push_back(1.60);temp_window.push_back(-1.15);temp_window.push_back(2.25);
  // track_window[80] = temp_window;

  // temp_window.clear();
  // temp_window.push_back(-2.8);temp_window.push_back(1.50);temp_window.push_back(-1.15);temp_window.push_back(3.2);
  // track_window[100] = temp_window;

  // temp_window.clear();
  // temp_window.push_back(-2.8);temp_window.push_back(1.50);temp_window.push_back(-1.15);temp_window.push_back(3.0);
  // track_window[120] = temp_window;


  // temp_window.clear();
  // temp_window.push_back(-3.0);temp_window.push_back(-1.0);temp_window.push_back(0.2);temp_window.push_back(2.2);
  // track_window[200] = temp_window;

  // temp_window.clear();
  // temp_window.push_back(-2.8);temp_window.push_back(-0.8);temp_window.push_back(-0.2);temp_window.push_back(2.0);
  // track_window[250] = temp_window;

  // temp_window.clear();
  // temp_window.push_back(-3.0);temp_window.push_back(-1.0);temp_window.push_back(-0.3);temp_window.push_back(1.8);
  // track_window[300] = temp_window;




  if(DEBUG) cout<<"DEBUG: Configuration = "<<conf_<<endl;
  if(DEBUG) cout<<"DEBUG: TOTAL_ACTIVE_LAYER = "<<TOTAL_ACTIVE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: EE_LAYER = "<<EE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: FH_LAYER = "<<FH_LAYER<<endl;

  if(DEBUG) cout<<"DEBUG: Entering event Loop"<<endl;
  Long64_t jentry;

  TStopwatch sw;
  sw.Start();

  double time_hgc = 0.0;
  double time_ahc = 0.0;
  double time_CompartCalc = 0.0;
  double time_LongiCalc = 0.0;
  double time_EnMeasureCalc = 0.0;
  
  for (jentry=0; jentry<nentries;jentry++) {
    // TStopwatch sw;
    // sw.Start();

    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;

    // cout<<">>>>>>>>>>>> yolo 1"<<endl;

    // ===============read this entry == == == == == == == == == == ==

    Long64_t ientry = LoadTree(jentry);
    // cout<<" jentry : ientry :: "<<jentry<<" : "<<ientry<<endl; 
    if (ientry < 0) {  cout<<"Breaking"<<endl; break;}
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // if(NRechits == 0) continue;

    
    h_nTracks->Fill(ntracks);
    event_count[0]++;
    
    // if(ntracks != 1) continue;
    //bool isGoodTrack = (dwcReferenceType >= 13 && trackChi2_X < 10 && trackChi2_Y < 10);
    if(!isGoodTrack) continue;
    event_count[1]++;

    E_beam = beamEnergy;
    h_beamEnergy->Fill(E_beam);
    h_particleID->Fill(pdgID);
    h_runNumber->Fill(run);

    
    if(isFHNoisy) continue;
    event_count[2]++;


    ////// Track information , pickup from tree
    double track_x[79];
    double track_y[79];

    std::vector<RecHit*> rechit_collection[40];
    std::vector<RecHit_AHCAL*> rechit_AH_collection[39];
    Long_t Nrechit_FH_module[12][7];
    Double_t RechitsEn_FH_module[12][7];

    if(TrackImpactX_layer->size() != 79 || TrackImpactX_layer->size() != 79) {
      cout<<"Trackvector size is NOT 79 for jentry "<<jentry<<"  Breaking !!!"<<endl;
      break;
      
    }

    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;

    for(int ii = 0; ii < 79; ii++) {
      track_x[ii] = TrackImpactX_layer->at(ii);
      track_y[ii] = TrackImpactY_layer->at(ii);
      
      if(ii < 40) {

	if(ii < 28) {
	  Nrechit_EE += rechit_nHitsPerLayer->at(ii);
	  rechitEnergySum_EE += rechit_energyPerLayer->at(ii);
	}
	else {
	  Nrechit_FH += rechit_nHitsPerLayer->at(ii);
	  rechitEnergySum_FH += rechit_energyPerLayer->at(ii);
	  
	}
	
	rechit_collection[ii].clear();
	if(ii < 39) {
	  if(ii+1 == 38) {
	    ahc_nHitsPerLayer->at(ii) = 0.0;
	    ahc_energyPerLayer->at(ii) = 0.0;
	  }
	  rechit_AH_collection[ii].clear();
	}
	if(ii < 12) {
	  for(int j = 0; j < 7; j++) { 
	    Nrechit_FH_module[ii][j] = 0; 
	    RechitsEn_FH_module[ii][j] = 0; 
	  }
	}
	
      }
    }


    // following statement is used to make sure old & new sim o/p is same
    // Ideally, one should use "isInTrackWindow" flag for both data and simulation
    // Use above flaf after everything looks fine : Dated 18 Nov 2020
    
    if(strcmp(data,"data") != 0 && Nrechit_EE == 0) continue;

    ////////////////////////////////////////////////////////////

    if(DEBUG) { cout << " DEBUG :: SW >>>>>> Starting of HGCAL loop, Took: "; sw.Print("u");  }

    TStopwatch sw_hgc;
    sw_hgc.Start();
    ////////////////////////////////////////////
    //            HGCAL Part                  //
    ////////////////////////////////////////////

    /// Read Tree
    if(DEBUG) cout<<"DEBUG: Start Analylizing RecHits!!"<<endl;
    
    std::vector<int> temp_moduleID;
    std::vector<double> rechitX_EE03;
    std::vector<double> rechitY_EE03;
    std::vector<double> rechitX_EE03_corrected;
    std::vector<double> rechitY_EE03_corrected;
    std::vector<double> rechitEnergy_EE03;

    std::vector<double> rechitX_EE15;
    std::vector<double> rechitY_EE15;
    std::vector<double> rechitX_EE15_corrected;
    std::vector<double> rechitY_EE15_corrected;
    std::vector<double> rechitEnergy_EE15;


    std::vector<double> rechitX_FH10;
    std::vector<double> rechitY_FH10;
    std::vector<double> rechitX_FH10_corrected;
    std::vector<double> rechitY_FH10_corrected;
    std::vector<double> rechitEnergy_FH10;

    rechitX_EE03.clear();
    rechitY_EE03.clear();
    rechitX_EE03_corrected.clear();
    rechitY_EE03_corrected.clear();
    rechitEnergy_EE03.clear();

    rechitX_EE15.clear();
    rechitY_EE15.clear();
    rechitX_EE15_corrected.clear();
    rechitY_EE15_corrected.clear();
    rechitEnergy_EE15.clear();

    rechitX_FH10.clear();
    rechitY_FH10.clear();
    rechitX_FH10_corrected.clear();
    rechitY_FH10_corrected.clear();
    rechitEnergy_FH10.clear();

    double cog_z_EE = 0.0;
    double cog_z_FH = 0.0;
    double cog_z_HGCAL = 0.0;

    double cog_layer_EE = 0.0;
    double cog_layer_FH = 0.0;
    double cog_layer_HGCAL = 0.0;


    Double_t rechitEnergySum = 0.0;
    int module_part_ = -1;
    int module_layer_ = -1;
    int module_position_ = -1;

    double noise_sigma_ = -1.0; 

    for(int i = 0 ; i < NRechits; i++){
      temp_moduleID.clear();
      int temp_layer = rechit_layer->at(i);
      int temp_chip = rechit_chip->at(i);
      int temp_channel = rechit_channel->at(i);
      int en_chan = temp_chip*1000+temp_channel;
      
      double recx = rechit_x->at(i);
      double recy = rechit_y->at(i);


      //channel masking
      if(hgc_channel_mask->at(i)) continue;

      //Noise cut ( pass_noise_thres == 1 => indicates it is above noise cut threshold => not noisy
      if(!pass_noise_thres->at(i)) continue;

      double trackx = track_x[temp_layer-1];
      double tracky = track_y[temp_layer-1];
      
      if(!strcmp(data,"data")) {
	trackx = -1*trackx;
	tracky = -1*tracky;

	std::pair<float, float> dxy_al = dxy_alignment(temp_layer);
	float dx_corr = dxy_al.first;
	float dy_corr = dxy_al.second; 
	recx -= dx_corr;
	recy -= dy_corr;
      }


      temp_moduleID = getModuleLocation(rechit_module->at(i));

      temp_moduleID = getModuleLocation(rechit_module->at(i));


      if(!temp_moduleID.size() || temp_moduleID.size()<3) {
	cout<<"ERROR: Could NOT locate MODULE location for module "<<rechit_module->at(i)<<endl;
	cout<<"\t more info temp_moduleID.size() = "<<temp_moduleID.size()<<endl;
	return;
      }
      module_part_ = temp_moduleID.at(0);
      module_layer_ = temp_moduleID.at(1);
      module_position_ = temp_moduleID.at(2);
      
      h_moduleID->Fill(module_part_);


      //// Minor debug for traxkImpact vs rechitPosition for EE03
      if(temp_layer == 3) {
	rechitX_EE03.push_back(rechit_x->at(i));
	rechitY_EE03.push_back(rechit_y->at(i));
	rechitX_EE03_corrected.push_back(recx);
	rechitY_EE03_corrected.push_back(recy);
	rechitEnergy_EE03.push_back(rechit_energy->at(i));
      }

      //// Minor debug for traxkImpact vs rechitPosition for EE15
      if(temp_layer == 15) {
	rechitX_EE15.push_back(rechit_x->at(i));
	rechitY_EE15.push_back(rechit_y->at(i));
	rechitX_EE15_corrected.push_back(recx);
	rechitY_EE15_corrected.push_back(recy);
	rechitEnergy_EE15.push_back(rechit_energy->at(i));
      }

      
      if(temp_layer == 38) {
	rechitX_FH10.push_back(rechit_x->at(i));
	rechitY_FH10.push_back(rechit_y->at(i));
	rechitX_FH10_corrected.push_back(recx);
	rechitY_FH10_corrected.push_back(recy);
	rechitEnergy_FH10.push_back(rechit_energy->at(i));
      }
            
      //////////////////////////////////////////////////////////////////////////

      if(rechit_layer->at(i) > 28) {
	// cout<<module_part_<<":"<<module_layer_<<":"<<module_position_<<endl;
	Nrechit_FH_module[rechit_layer->at(i)-29][module_position_-1]++;
	RechitsEn_FH_module[rechit_layer->at(i)-29][module_position_-1] += rechit_energy->at(i);
	if(rechit_layer->at(i) == 36 && module_position_ == 2) {
	  h_FH_debug->Fill(getBIN(rechit_chip->at(i),rechit_channel->at(i)),rechit_energy->at(i));
	}
      }
      if(module_part_ == 0) {
	// rechitEnergySum_EE+=rechit_energy->at(i);
	// RechitsEn_EE[module_layer_-1]+=rechit_energy->at(i);
	//NRechits_EE[module_layer_-1]++;
	cog_z_EE += rechit_energy->at(i)*rechit_z->at(i);
	cog_layer_EE += rechit_energy->at(i)*rechit_layer->at(i);
	cog_z_HGCAL += rechit_energy->at(i)*rechit_z->at(i);
	cog_layer_HGCAL += rechit_energy->at(i)*rechit_layer->at(i);

      }
      else if(module_part_ == 1) {
	if(module_position_ == 4 || true) {
	  // rechitEnergySum_FH+=rechit_energy->at(i);
	  // RechitsEn_FH[module_layer_-1]+=rechit_energy->at(i);
	  // NRechits_FH[module_layer_-1]++;
	  cog_z_FH += rechit_energy->at(i)*rechit_z->at(i);
	  cog_layer_FH += rechit_energy->at(i)*rechit_layer->at(i);
	  cog_z_HGCAL += rechit_energy->at(i)*rechit_z->at(i);
	  cog_layer_HGCAL += rechit_energy->at(i)*rechit_layer->at(i);
	}
	
      }
      else {
	cout<<"ERROR: Unknown Module Part detected!!!!"<<endl;
	return;
	
      }

      rechitEnergySum+=rechit_energy->at(i);

      //////////////////////////////////////////////////////////////////////////////////
      /////////////////// Saving information Lateral shower variable information ////////
      //////////////////////////////////////////////////////////////////////////////////

      RecHit* rechit_temp = new RecHit(temp_layer, temp_chip, temp_channel, en_chan, module_part_, module_position_, rechit_module->at(i), (int)rechit_iu->at(i), (int)rechit_iv->at(i), rechit_x->at(i), rechit_y->at(i), rechit_z->at(i), rechit_energy->at(i));

      rechit_collection[temp_layer-1].push_back(rechit_temp);
      
    }


    ///////////////////////////////////////////
    ////////// End of HGCAL nRechit Loop /////
    //////////////////////////////////////////

    
    time_hgc += sw_hgc.RealTime();
    sw_hgc.Stop();

    if(DEBUG) { cout << "SW >>>>>>END of HGCAL loop; Took: "; sw.Print();  }


    TStopwatch sw_ahc;
    sw_ahc.Start();

    ////////////////////////////////////////////
    //            AHCAL Part                  //
    ////////////////////////////////////////////
    
    Double_t rechitEnergySum_AH = 0.0;
    Double_t cog_layer_AH = 0.0;
    Long_t Nrechit_AH = ahc_nHits;
    for(int i = 0 ; i < ahc_nHits; i++) {
      int temp_layer = ahc_hitK->at(i);
      //Skip AHCAL layer 38
      if(temp_layer == 38) continue;
      // if(ahc_channel_mask->at(i)) continue;
      
      rechitEnergySum_AH += ahc_hitEnergy->at(i);
      cog_layer_AH += ahc_hitEnergy->at(i)*temp_layer;

      RecHit_AHCAL* rechit_temp = new RecHit_AHCAL(temp_layer, ahc_hitI->at(i), ahc_hitJ->at(i), ahc_hitX->at(i), ahc_hitY->at(i), ahc_hitZ->at(i), ahc_hitEnergy->at(i));
      rechit_AH_collection[temp_layer-1].push_back(rechit_temp);
    }

    //////////////////////////////////////////////
    ////////// End of AHCAL nRechit Loop  ////////
    //////////////////////////////////////////////

    time_ahc += sw_ahc.RealTime();
    sw_ahc.Stop();


    /////////////////////////////////////////////
    //// Shower start finder information   /////
    ////////////////////////////////////////////

    bool MIP = true;
    int shower_start_index = -1;
    float shower_lambda_ = -1.0;
    
    if(rechit_shower_start_layer < 0 ) {
      shower_start_index = -1;
      shower_lambda_ = -1;
      MIP = true;

    }
    else {
      shower_start_index = rechit_shower_start_layer -1 ;
      shower_lambda_ = lambda[rechit_shower_start_layer - 1];
      MIP = false;
    }



    ///////////////////////////////////////////////////////


    TStopwatch sw_compart;
    sw_compart.Start();
    
    ////////////////////////////////////////////////////////////////
    //                Comapartment wise calculation              //
    ///////////////////////////////////////////////////////////////
    

    Double_t test = 0.0;
    bool zero_rh[40];
    float seed_energy_incl = 0.0;  
    float r1_energy_incl = 0.0;    
    float r2_energy_incl = 0.0;    
    float E1_inc = 0.0; float E7_inc = 0.0; float E19_inc = 0.0;
    float E1_EE = 0.0; float E7_EE = 0.0; float E19_EE = 0.0;
    float E1_FH = 0.0; float E7_FH = 0.0; float E19_FH = 0.0;
    float S1_AH = 0.0; float S9_AH = 0.0; float S25_AH = 0.0;

    double EE_last3 = rechit_energyPerLayer->at(25) + rechit_energyPerLayer->at(26) + rechit_energyPerLayer->at(27);
    double FH_last3 = rechit_energyPerLayer->at(37) + rechit_energyPerLayer->at(38) + rechit_energyPerLayer->at(39);
    double AH_last3 = ahc_energyPerLayer->at(36) + ahc_energyPerLayer->at(37) + ahc_energyPerLayer->at(38);

    double total_energy_EE = rechitEnergySum_EE;
    double total_energy_FH = rechitEnergySum_FH;
    double total_energy_AH = rechitEnergySum_AH;


    vector< std::pair<double,double> > seed_xy;
    seed_xy.clear();
    
    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++){


      if(iL > EE_LAYER -1) { // for FH layers
	for(int j = 0; j < 7; j++) {
    	  if(iL+1 >= 38 && j+1 != 4) continue;
	  h_nrechit_FH[iL-EE_LAYER][j]->Fill(Nrechit_FH_module[iL-EE_LAYER][j]);
	  h_energy_FH[iL-EE_LAYER][j]->Fill(RechitsEn_FH_module[iL-EE_LAYER][j]);
    	}

      }

      //layerwise_comparison
      h_ld_energy[iL]->Fill(rechit_energyPerLayer->at(iL));
      h_ld_nrechit[iL]->Fill(rechit_nHitsPerLayer->at(iL));

      h_cogX[iL]->Fill(rechit_cogX->at(iL));
      h_cogY[iL]->Fill(rechit_cogY->at(iL));
      h_cogX_cogY[iL]->Fill(rechit_cogX->at(iL),rechit_cogY->at(iL));

      h_trackX[iL]->Fill(track_x[iL]);
      h_trackY[iL]->Fill(track_y[iL]);
      h_trackX_trackY[iL]->Fill(track_x[iL],track_y[iL]);

      ///////////////////////////////////////////////////////////////
      //////// For lateral shower variable calculation ////////////
      ///////////////////////////////////////////////////////////////

      // cout<<"size at layer "<<iL+1<<" : "<<rechit_collection[iL].size()<<endl;
      LateralRings* temp_LR = new LateralRings(rechit_collection[iL]);
      if(!rechit_collection[iL].size()) { 
	temp_LR->setLayer(iL+1);
      }
      
      float seed = temp_LR->getSeedEnergy();
      int seed_index = temp_LR->getSeedIndex();
      float r1 = temp_LR->getEnergyInRing1();
      float r2 = temp_LR->getEnergyInRing2();
      float E1 = seed; float E7 = E1+r1; float E19 = E7+r2;

      if( seed_index < 0) seed_xy.push_back(std::make_pair(-999.0,-999.0));
      else {
	vector<float> temp = ((rechit_collection[iL]).at(seed_index))->getCellCoordinate_xyz();
	if((int)temp.size() !=3) { cout<<"halting at point tango"<<endl; return;}
	seed_xy.push_back(std::make_pair(temp[0],temp[1]));
      }
      
      seed_energy_incl += seed;
      r1_energy_incl += r1;
      r2_energy_incl += r2;
      E1_inc += seed;
      E7_inc += E7;
      E19_inc += E19;
      if(iL < 28) {
	E1_EE += seed;
	E7_EE += E7;
	E19_EE += E19;

      }
      else {
	E1_FH += seed;
	E7_FH += E7;
	E19_FH += E19;
      }

      if(iL < 39) {

	LateralSquares* temp_LS = new LateralSquares(rechit_AH_collection[iL]);

	// if(!rechit_AH_collection[iL].size()) { 
	//   temp_LS->setLayer(iL+1);
	// }
	// else AHCAL_uniqueLayer++;
	
	float seed_AH = temp_LS->getSeedEnergy(); // ==> 1 cell
	float s1 = temp_LS->getEnergyInSquare1(); // ==> 8 cells
	float s2 = temp_LS->getEnergyInSquare2(); // ==> 16 cells
	float S1 = seed_AH; float S9 = S1+s1; float S25 = S9+s2;
	S1_AH += seed_AH;
	S9_AH += S9;
	S25_AH += S25;
	// cout<<iL+1<<" seed_AH: "<<seed_AH<<" ,s1: "<<s1<<" ,s2: "<<s2<<" :: S1:"<<S1<<" ,S9: "<<S9<<" ,S25: "<<S25<<endl;
	//layerwise_comparison
	h_ld_energy[iL+40]->Fill(ahc_energyPerLayer->at(iL));
	h_ld_nrechit[iL+40]->Fill(ahc_nHitsPerLayer->at(iL));

      }
      

      ///////////////////////////////////////////////////////////////
      ///////      END of lateral information calculation ///////////
      ///////////////////////////////////////////////////////////////



    }

    
    ///////////////////////////////////////////////////////////////
    ///////     END of compartment wise calculation       /////////
    ///////////////////////////////////////////////////////////////

    time_CompartCalc += sw_compart.RealTime();
    sw_compart.Stop();

    
    cog_z_EE = cog_z_EE/rechitEnergySum_EE;
    cog_layer_EE = cog_layer_EE/rechitEnergySum_EE;

    cog_z_FH = cog_z_FH/rechitEnergySum_FH;
    cog_layer_FH = cog_layer_FH/rechitEnergySum_FH;
    cog_layer_FH = cog_layer_FH - 28;

    cog_z_HGCAL = cog_z_HGCAL/rechitEnergySum;
    cog_layer_HGCAL = cog_layer_HGCAL/rechitEnergySum;

    cog_layer_AH = cog_layer_AH/rechitEnergySum_AH;


    h_cog_layer_EE->Fill(cog_layer_EE);
    h_cog_layer_FH->Fill(cog_layer_FH);
    h_cog_layer_HGCAL->Fill(cog_layer_HGCAL);
    h_cog_layer_AH->Fill(cog_layer_AH);

    
    if(E1_EE !=0 ) {
      h_E1_E7_EE->Fill(E1_EE/E7_EE);
      h_E1_E19_EE->Fill(E1_EE/E19_EE);
      h_E7_E19_EE->Fill(E7_EE/E19_EE);
    }
    if(E1_FH != 0) {
      h_E1_E7_FH->Fill(E1_FH/E7_FH);
      h_E1_E19_FH->Fill(E1_FH/E19_FH);
      h_E7_E19_FH->Fill(E7_FH/E19_FH);
    }
    if(E1_inc != 0) {
      h_E1_E7_HGCAL->Fill(E1_inc/E7_inc);
      h_E1_E19_HGCAL->Fill(E1_inc/E19_inc);
      h_E7_E19_HGCAL->Fill(E7_inc/E19_inc);
    }

    if(!MuonVeto) {
      h_S1_S9_AH->Fill(S1_AH/S9_AH);
      h_S1_S25_AH->Fill(S1_AH/S25_AH);
      h_S9_S25_AH->Fill(S9_AH/S25_AH);
    }


    if(Nrechit_EE == 0) EE_zeroHit++;
    if(Nrechit_FH == 0) FH_zeroHit++;
    if(Nrechit_AH == 0) ahc_zeroHit++;



    if(shower_start_index+1 == 15) h_run_SS15->Fill(run);
    h_shower_start->Fill(shower_lambda_);
    // h_shower_start_dN_dLambda->Fill(shower_lambda_,1/shower_weight_);


    if(shower_start_index+1 > 0.0 && shower_start_index+1 <= 28) {
      if(E1_EE != 0) {
	h_E1_E7_SS_EE->Fill(E1_EE/E7_EE);
	h_E1_E19_SS_EE->Fill(E1_EE/E19_EE);
	h_E7_E19_SS_EE->Fill(E7_EE/E19_EE);
      }
    }
    else if(shower_start_index+1 > 28) {
      if(E1_EE != 0) {
	h_E1_E7_MIPs_in_EE->Fill(E1_EE/E7_EE);
	h_E1_E19_MIPs_in_EE->Fill(E1_EE/E19_EE);
	h_E7_E19_MIPs_in_EE->Fill(E7_EE/E19_EE);
      }
      if(E1_FH != 0) {
	h_E1_E7_SS_FH->Fill(E1_FH/E7_FH);
	h_E1_E19_SS_FH->Fill(E1_FH/E19_FH);
	h_E7_E19_SS_FH->Fill(E7_FH/E19_FH);
      }
    }
    else {
      if(E1_FH != 0) {
	h_E1_E7_MIPs_in_FH->Fill(E1_FH/E7_FH);
	h_E1_E19_MIPs_in_FH->Fill(E1_FH/E19_FH);
	h_E7_E19_MIPs_in_FH->Fill(E7_FH/E19_FH);
      }
    }


    ///////////////////////////////////////////////
    //                Collapsed EE               //
    ///////////////////////////////////////////////
    if(shower_start_index+1 <= 28 && (shower_start_index+1) > 0) {
      h_shower_start_full_collapsed_EE->Fill(lambda[0]);
      if((shower_start_index+1)%2 == 0) {
	h_shower_start_part_collapsed_EE->Fill(lambda[shower_start_index-1]);
	// cout<<shower_start_index<<" "<<(shower_start_index+1)%2<<" "<<shower_start_index-1<<" "<<lambda[(shower_start_index+1)/2]<<endl;
      }
      else {
	h_shower_start_part_collapsed_EE->Fill(lambda[shower_start_index]);
      }
    }
    else {
      h_shower_start_full_collapsed_EE->Fill(shower_lambda_);
      h_shower_start_part_collapsed_EE->Fill(shower_lambda_);
    }


    if(MuonVeto) MIP = true;

    TStopwatch sw_longi;
    sw_longi.Start();
    
    ////////////////////////////////////////////////////////////
    ////           Longitudinal shower profile              ////
    ///////////////////////////////////////////////////////////

    bool DoWeight = false;
    double beta_ = -1.0;
    double ee_mip2gev = 0.0106; // GeV/MIP
    double fh_mip2gev = 0.0789; // GeV/MIP
    double ah_mip2gev = 0.4*fh_mip2gev; // GeV/MIP
    float relative_weighted_energy = 0.0; //rechitEnergySum_EE + beta_*rechitEnergySum_FH;
    float w1 = 0; float w2 = 0; float w3 = 0;

    // if(!MIP) {
    //   if(shower_start_index+1 <= 28) {
    // 	relative_weighted_energy = ee_mip2gev*rechitEnergySum_EE +  fh_mip2gev*rechitEnergySum_FH + ah_mip2gev*rechitEnergySum_AH;
    // 	w1 = f_EH_w1->Eval(relative_weighted_energy);
    // 	w2 = f_EH_w2->Eval(relative_weighted_energy);
    // 	w3 = f_EH_w3->Eval(relative_weighted_energy);
	
    //   }
    //   else {
    // 	relative_weighted_energy = fh_mip2gev*rechitEnergySum_FH + ah_mip2gev*rechitEnergySum_AH;
    // 	w1 = f_H_w1->Eval(relative_weighted_energy);
    // 	w2 = f_H_w2->Eval(relative_weighted_energy);
    // 	w3 = f_H_w3->Eval(relative_weighted_energy);

    //   }
    // }
    w1 = ee_mip2gev; w2 = fh_mip2gev; w3 = ah_mip2gev;



    int summed_up_last_layer[5] = {4, 10, 16, 22, 28};
    float absolute_energy = rechitEnergySum_EE + rechitEnergySum_FH;
    double total_energy = total_energy_EE + total_energy_FH + total_energy_AH;
    double total_energy_gev = w1*total_energy_EE + w2*total_energy_FH + w3*total_energy_AH;


    for(int iL = 0; iL < TOTAL_ACTIVE_LAYER; iL++) {
      
      if(MIP) continue;

      
      if(iL < EE_LAYER) { //EE part
	

	float EE_energy = rechit_energyPerLayer->at(iL);
	if(shower_start_index+1 == 10  && rechitEnergySum_EE > 0) {
	  h_longi_profile_raw_SS10_check->Fill(iL+1,EE_energy/rechitEnergySum_EE);
	  if(total_energy_gev > 0) h_longi_profile_gev_SS10_check->Fill(iL+1,w1*EE_energy/total_energy_gev);
	}
	
	if(shower_start_index+1 <= 28) { // Showering in CE-E
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],EE_energy);
	  h_longi_profile_ShowerInEE->Fill(lambda[iL],EE_energy);
	  h_longi_profile_ShowerInEE_gev->Fill(lambda[iL],EE_energy*ee_mip2gev);
	  h_longi_profile_inclusive->Fill(lambda[iL],EE_energy);
	  h_longi_profile_inclusive_frac->Fill(lambda[iL],EE_energy/total_energy);

	  h_longi_profile_raw_layer[shower_start_index]->Fill(iL+1,EE_energy);
	  h_longi_profile_gev_layer[shower_start_index]->Fill(iL+1,EE_energy*w1);
	  h_longi_profile_raw_ShowerInEE_layer->Fill(iL+1,EE_energy);
	  h_longi_profile_gev_ShowerInEE_layer->Fill(iL+1,EE_energy*ee_mip2gev);
	  h_longi_profile_raw_inclusive_layer->Fill(iL+1,EE_energy);
	  h_longi_profile_raw_inclusive_frac_layer->Fill(iL+1,EE_energy/total_energy);
	  

	}
	else { // MIPs in CE-E
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],EE_energy);
	  h_longi_profile_MipsInEE->Fill(lambda[iL],EE_energy);
	  h_longi_profile_MipsInEE_gev->Fill(lambda[iL],EE_energy*ee_mip2gev);
	  h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],EE_energy);
	  h_longi_profile_inclusive->Fill(lambda[iL],EE_energy);
	  h_longi_profile_inclusive_frac->Fill(lambda[iL],EE_energy/total_energy);

	  h_longi_profile_raw_layer[shower_start_index]->Fill(iL+1,EE_energy);
	  h_longi_profile_gev_layer[shower_start_index]->Fill(iL+1,EE_energy*w1);
	  h_longi_profile_raw_MipsInEE_layer->Fill(iL+1,EE_energy);
	  h_longi_profile_gev_MipsInEE_layer->Fill(iL+1,EE_energy*ee_mip2gev);
	  h_longi_profile_raw_MipsInEE_SS_ref_layer->Fill(iL+1-shower_start_index,EE_energy);
	  h_longi_profile_raw_inclusive_layer->Fill(iL+1,EE_energy);
	  h_longi_profile_raw_inclusive_frac_layer->Fill(iL+1,EE_energy/total_energy);
	  
	  
	}

	h_Rechits_nrec_SS[shower_start_index][iL]->Fill(rechit_nHitsPerLayer->at(iL));
	h_Rechits_En_SS[shower_start_index][iL]->Fill(EE_energy);
      } 
      else { //FH part
	float FH_energy = rechit_energyPerLayer->at(iL);
	if(shower_start_index+1 == 10  && rechitEnergySum_FH > 0) {
	  h_longi_profile_raw_SS10_check->Fill(iL+1,FH_energy/rechitEnergySum_FH);
	  if(total_energy_gev > 0) h_longi_profile_gev_SS10_check->Fill(iL+1,w2*FH_energy/total_energy_gev);

	}
	
	if(shower_start_index+1 <= 28) { // Showering in CE-E
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],FH_energy);
	  h_longi_profile_ShowerInEE->Fill(lambda[iL],FH_energy);
	  h_longi_profile_ShowerInEE_gev->Fill(lambda[iL],FH_energy*fh_mip2gev);
	  h_longi_profile_inclusive->Fill(lambda[iL],FH_energy);
	  h_longi_profile_inclusive_frac->Fill(lambda[iL],FH_energy/total_energy);

	  h_longi_profile_raw_layer[shower_start_index]->Fill(iL+1,FH_energy);
	  h_longi_profile_gev_layer[shower_start_index]->Fill(iL+1,FH_energy*w2);
	  h_longi_profile_raw_ShowerInEE_layer->Fill(iL+1,FH_energy);
	  h_longi_profile_gev_ShowerInEE_layer->Fill(iL+1,FH_energy*fh_mip2gev);
	  h_longi_profile_raw_inclusive_layer->Fill(iL+1,FH_energy);
	  h_longi_profile_raw_inclusive_frac_layer->Fill(iL+1,FH_energy/total_energy);

	  

	}
	else { // MIPs in CE-E
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL],FH_energy);
	  h_longi_profile_MipsInEE->Fill(lambda[iL],FH_energy);
	  h_longi_profile_MipsInEE_gev->Fill(lambda[iL],FH_energy*fh_mip2gev);
	  h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL]-lambda[shower_start_index],FH_energy);
	  h_longi_profile_inclusive->Fill(lambda[iL],FH_energy);
	  h_longi_profile_inclusive_frac->Fill(lambda[iL],FH_energy/total_energy);

	  h_longi_profile_raw_layer[shower_start_index]->Fill(iL+1,FH_energy);
	  h_longi_profile_gev_layer[shower_start_index]->Fill(iL+1,FH_energy*w2);
	  h_longi_profile_raw_MipsInEE_layer->Fill(iL+1,FH_energy);
	  h_longi_profile_gev_MipsInEE_layer->Fill(iL+1,FH_energy*fh_mip2gev);
	  h_longi_profile_raw_MipsInEE_SS_ref_layer->Fill(iL+1-shower_start_index,FH_energy);
	  h_longi_profile_raw_inclusive_layer->Fill(iL+1,FH_energy);
	  h_longi_profile_raw_inclusive_frac_layer->Fill(iL+1,FH_energy/total_energy);


	}
	h_Rechits_nrec_SS[shower_start_index][iL]->Fill(rechit_nHitsPerLayer->at(iL));
	h_Rechits_En_SS[shower_start_index][iL]->Fill(FH_energy);

      } 
      if(iL < 39) { //AH part
	float AH_energy = ahc_energyPerLayer->at(iL);
	if(shower_start_index+1 == 10 && rechitEnergySum_AH > 0)  {
	  h_longi_profile_raw_SS10_check->Fill(iL+1+40,AH_energy/rechitEnergySum_AH);
	  if(total_energy_gev > 0) h_longi_profile_gev_SS10_check->Fill(iL+1+40,w3*AH_energy/total_energy_gev);

	}
	if(shower_start_index+1 <= 28) { // Showering in CE-E
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_ShowerInEE->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_ShowerInEE_gev->Fill(lambda[iL+40],AH_energy*ah_mip2gev);
	  h_longi_profile_inclusive->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_inclusive_frac->Fill(lambda[iL+40],AH_energy/total_energy);


	  h_longi_profile_raw_layer[shower_start_index]->Fill(iL+1+40,AH_energy);
	  h_longi_profile_gev_layer[shower_start_index]->Fill(iL+1+40,AH_energy*w3);
	  h_longi_profile_raw_ShowerInEE_layer->Fill(iL+1+40,AH_energy);
	  h_longi_profile_gev_ShowerInEE_layer->Fill(iL+1+40,AH_energy*ah_mip2gev);
	  h_longi_profile_raw_inclusive_layer->Fill(iL+1+40,AH_energy);
	  h_longi_profile_raw_inclusive_frac_layer->Fill(iL+1+40,AH_energy/total_energy);


	}
	else {  // MIPs in CE-E
	  h_longi_profile_raw[shower_start_index]->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_MipsInEE->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_MipsInEE_gev->Fill(lambda[iL+40],AH_energy*ah_mip2gev);
	  h_longi_profile_MipsInEE_SS_ref->Fill(lambda[iL+40]-lambda[shower_start_index],AH_energy);
	  h_longi_profile_inclusive->Fill(lambda[iL+40],AH_energy);
	  h_longi_profile_inclusive_frac->Fill(lambda[iL+40],AH_energy/total_energy);


	  h_longi_profile_raw_layer[shower_start_index]->Fill(iL+1+40,AH_energy);
	  h_longi_profile_gev_layer[shower_start_index]->Fill(iL+1+40,AH_energy*w3);
	  h_longi_profile_raw_MipsInEE_layer->Fill(iL+1+40,AH_energy);
	  h_longi_profile_gev_MipsInEE_layer->Fill(iL+1+40,AH_energy*ah_mip2gev);
	  h_longi_profile_raw_MipsInEE_SS_ref_layer->Fill(iL+1+40-shower_start_index,AH_energy);
	  h_longi_profile_raw_inclusive_layer->Fill(iL+1+40,AH_energy);
	  h_longi_profile_raw_inclusive_frac_layer->Fill(iL+1+40,AH_energy/total_energy);


	}
	h_Rechits_nrec_SS[shower_start_index][iL+40]->Fill( ahc_nHitsPerLayer->at(iL) );
	h_Rechits_En_SS[shower_start_index][iL+40]->Fill(AH_energy);

      }


      // for transverse shower shape
      float seed_x = seed_xy[iL].first;
      float seed_y = seed_xy[iL].second;
      if(seed_x > -999) {
	double trackx = track_x[iL];
	double tracky = track_y[iL];
      
	if(!strcmp(data,"data")) {
	  trackx = -1*trackx;
	  tracky = -1*tracky;
      
	  std::pair<float, float> dxy_al = dxy_alignment(iL+1);
	  float dx_corr = dxy_al.first;
	  float dy_corr = dxy_al.second; 
	  seed_x -= dx_corr;
	  seed_y -= dy_corr;
	}
	double dX = (seed_x - trackx);
	double dY = (seed_y - tracky);
	// cout<<"layer : seedX : seedY : dX : dY ::  "<<iL+1<<" : "<<seed_xy[iL].first<<" : "<<seed_xy[iL].second<<" : "<<dX<<" : "<<dY<<endl;
	h_track_seed_diff_x[iL]->Fill(dX);
	h_track_seed_diff_y[iL]->Fill(dY);
	h_track_seed_diff_dR[iL]->Fill(deltaR(seed_x,seed_y,trackx,tracky));
	
      }

  
      double sum_dR2 = 0.0;
      double sum_dR5 = 0.0;
      double sum_dR8 = 0.0;
      double sum_dR10 = 0.0;
      for(int i = 0; i < (int)rechit_collection[iL].size(); i++) {
      	RecHit* temp_rechit = rechit_collection[iL].at(i);
      	double	recx = (temp_rechit->getCellCoordinate_xyz())[0];
      	double	recy = (temp_rechit->getCellCoordinate_xyz())[1];
      	double trackx = track_x[iL];
      	double tracky = track_y[iL];

      	if(!strcmp(data,"data")) {
      	  trackx = -1*trackx;
      	  tracky = -1*tracky;
	  
      	  std::pair<float, float> dxy_al = dxy_alignment(iL+1);
      	  float dx_corr = dxy_al.first;
      	  float dy_corr = dxy_al.second; 
      	  recx -= dx_corr;
      	  recy -= dy_corr;
      	}

      	double dR = deltaR(recx,recy,trackx,tracky);
      	if(dR < 2.0) sum_dR2 += temp_rechit->getEnergy();
      	if(dR < 5.0) sum_dR5 += temp_rechit->getEnergy();
      	if(dR < 8.0) sum_dR8 += temp_rechit->getEnergy();
      	if(dR < 10.0) sum_dR10 += temp_rechit->getEnergy();
	
      }

      h_transverse_dR2[shower_start_index][iL]->Fill(sum_dR2);
      h_transverse_dR5[shower_start_index][iL]->Fill(sum_dR5);
      h_transverse_dR8[shower_start_index][iL]->Fill(sum_dR8);
      h_transverse_dR10[shower_start_index][iL]->Fill(sum_dR10);
      
    }


    ////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    time_LongiCalc += sw_longi.RealTime();
    sw_longi.Stop();
    
    double isMuonLike = false;
    bool isRegion1 = false; //MIP like
    bool isRegion2 = false; //showering in CE-H
    bool isRegion3 = false;  //showering in CE-E
    bool isRegion4 = false; //showering in AHCAL

    ///////////////////////////////////////////////////////////
    //// E V E N T     C A T E G O R I Z A T I O N       /////
    ///////////////////////////////////////////////////////////


    if(UseShowerStartAlgo && !UseCompartmentEnergy)  {
      /////// BASED ON SHOWER START
      //MUON VETO
      if(MuonVeto) {
	isRegion1 = true;
	isRegion2 = false;
	isRegion3 = false;
      }
      if(shower_start_index == -1) {  //MIP LIKE
	//if(shower_start_index < 2) {  //MIP LIKE
	isRegion1 = true;
	isRegion2 = false;
	isRegion3 = false;
      }
      else if(shower_start_index > 27) { //H hadrons
	isRegion1 = false;
	isRegion2 = true;
	isRegion3 = false;
      }
      else {  //EH hadron
	isRegion1 = false;
	isRegion2 = false;
	isRegion3 = true;
      }

    }
    else if(!UseShowerStartAlgo && UseCompartmentEnergy) {
      /////// BASED ON ENERGY SUM
      //MUON VETO
      if(MuonVeto) {
	isRegion1 = true;
	isRegion2 = false;
	isRegion3 = false;
	isRegion4 = false;

      }
      else if(rechitEnergySum_EE > 100) {
	isRegion1 = false;
	isRegion2 = false;
	isRegion3 = true;
	isRegion4 = false;
	
	if(E1_EE != 0) {
	  h_E1_E7_SS_EE->Fill(E1_EE/E7_EE);
	  h_E1_E19_SS_EE->Fill(E1_EE/E19_EE);
	  h_E7_E19_SS_EE->Fill(E7_EE/E19_EE);
	}
	

      }
      else if(rechitEnergySum_FH > 60) {
	isRegion1 = false;
	isRegion2 = true;
	isRegion3 = false;
	isRegion4 = false;

	if(E1_EE != 0) {
	  h_E1_E7_MIPs_in_EE->Fill(E1_EE/E7_EE);
	  h_E1_E19_MIPs_in_EE->Fill(E1_EE/E19_EE);
	  h_E7_E19_MIPs_in_EE->Fill(E7_EE/E19_EE);
	}
	if(E1_FH != 0) {
	  h_E1_E7_SS_FH->Fill(E1_FH/E7_FH);
	  h_E1_E19_SS_FH->Fill(E1_FH/E19_FH);
	  h_E7_E19_SS_FH->Fill(E7_FH/E19_FH);
	}
	

      }
      else {
	if(rechitEnergySum_AH < 60)  {
	  isRegion1 = true;
	  isRegion2 = false;
	  isRegion3 = false;	 
	  isRegion4 = false;
	}
	else  {
	  isRegion1 = false;
	  isRegion2 = false;
	  isRegion3 = false;	 
	  isRegion4 = true;

	  h_rechit_energy_raw_AH_shower->Fill(rechitEnergySum_AH);
	  if(E1_FH != 0) {
	    h_E1_E7_MIPs_in_FH->Fill(E1_FH/E7_FH);
	    h_E1_E19_MIPs_in_FH->Fill(E1_FH/E19_FH);
	    h_E7_E19_MIPs_in_FH->Fill(E7_FH/E19_FH);
	  }
	  if(S1_AH != 0) {

	    h_S1_S9_AH_debug->Fill(S1_AH/S9_AH);
	    h_S1_S25_AH_debug->Fill(S1_AH/S25_AH);
	    h_S9_S25_AH_debug->Fill(S9_AH/S25_AH);
	  }
	  h_rechit_energy_raw_AH_shower_low_extended->Fill(rechitEnergySum_AH);

	}
      }


    }
    else {
      std::cout<<BOLDRED<<"No Switch selected for Event Categorization"<<RESET<<std::endl;
      return;
    
    }




    h_Nrechit_EE->Fill(Nrechit_EE);
    h_Nrechit_FH->Fill(Nrechit_FH);
    h_Nrechit_AH->Fill(ahc_nHits);

    h_Nrechit_low_EE->Fill(Nrechit_EE);
    h_Nrechit_low_FH->Fill(Nrechit_FH);
    h_Nrechit_low_AH->Fill(ahc_nHits);

    
    h_rechit_energy_raw_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_AH->Fill(rechitEnergySum_AH);
    h_rechit_energy_raw_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);


    h_rechit_energy_raw_low_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_low_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_low_AH->Fill(rechitEnergySum_AH);
    h_rechit_energy_raw_low_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

    h_rechit_energy_raw_verylow_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_verylow_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_verylow_AH->Fill(rechitEnergySum_AH);
    h_rechit_energy_raw_verylow_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

    h_rechit_energy_raw_mid_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_mid_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_mid_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);

    h_rechit_energy_raw_high_EE->Fill(rechitEnergySum_EE);
    h_rechit_energy_raw_high_FH->Fill(rechitEnergySum_FH);
    h_rechit_energy_raw_high_all->Fill(rechitEnergySum_EE+rechitEnergySum_FH);


    h_Nrechit_EE_vs_FH->Fill(Nrechit_EE,Nrechit_FH);

    float alpha = -1.0;  // FH & AHCAL relative weight
    float beta = -1.0; // FH & AHCAL relative weight
    float gamma_ee = -1.0;
    float gamma_fh = -1.0;
    float beta_noAH = -1.0; // EE & FH relative weight
    float gamma_noAH = -1.0;


    alpha = getWeights(beamEnergy).at(0);
    beta = getWeights(beamEnergy).at(1);
    gamma_ee = getWeights(beamEnergy).at(2);
    gamma_fh = getWeights(beamEnergy).at(3);


    // alpha = getWeights(50).at(0);
    // beta = getWeights(50).at(1);
    // gamma_ee = getWeights(50).at(2);
    // gamma_fh = getWeights(50).at(3);

    h_rechit_energy_frac_EE_last3_all->Fill(EE_last3/rechitEnergySum_EE);
    h_rechit_energy_frac_FH_last3_all->Fill(FH_last3/rechitEnergySum_FH); h_rechit_energy_frac_AH_last3_all->Fill(AH_last3/rechitEnergySum_AH);



    TStopwatch sw_energy;
    sw_energy.Start();

    // MIPs in CE-E and MIPs in CE-H
    if(isRegion1) {
      h_shower_start_reg1->Fill(shower_lambda_);
      region_1_classified_events++; 
      
      h_rechit_energy_raw_EE_MipsInEEFH->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEEFH->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_EE_vs_FH_MipsInEEFH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_MipsInEEFH->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInEEFH->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_EE_MipsInEEFH_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEEFH_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEEFH_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInEEFH_extended_v1->Fill(rechitEnergySum_AH);

      h_rechit_energy_raw_all_MipsInEEFH->Fill(rechitEnergySum_EE+rechitEnergySum_FH);
      
      h_rechit_energy_raw_EE_MipsInFH_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInFH_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInFH_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInFH_extended_v1->Fill(rechitEnergySum_AH);
      
      h_rechit_energy_frac_EE_last3_MipsInEEFH->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_MipsInEEFH->Fill(FH_last3/rechitEnergySum_FH);
      h_rechit_energy_frac_AH_last3_MipsInEEFH->Fill(AH_last3/rechitEnergySum_AH);
      
      
    }
    
    // MIPs in CE-E
    else if(isRegion2) {
      h_shower_start_reg2->Fill(shower_lambda_);
      region_2_classified_events++; 
      
      h_rechit_energy_frac_EE_last3_MipsInEE->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_MipsInEE->Fill(FH_last3/rechitEnergySum_FH); 
      h_rechit_energy_frac_AH_last3_MipsInEE->Fill(AH_last3/rechitEnergySum_AH);

      if(Nrechit_EE == 0) EE_zeroHit_R2++;
      
    
      h_rechit_energy_raw_FH_MipsInEE_extended->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_FH_MipsInEE_extended_v1->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEE_extended->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_AH_MipsInEE_extended_v1->Fill(rechitEnergySum_AH);

      h_Nrechits_EE_FH_FB->Fill(Nrechit_EE,Nrechit_FH);
      h_rechit_energy_inEE_MipsInEE_mips->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_EE_vs_FH_MipsInEE->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_MipsInEE->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      h_rechit_energy_raw_EE_MipsInEE->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_EE_MipsInEE_extended->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_MipsInEE->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_AH_MipsInEE->Fill(rechitEnergySum_AH);
      
      h_rechit_energy_raw_all_MipsInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH);


      //////////////////////////////////////////////////////////////////////////////////////
      ///////////////   Based on Electron/pion scale in CE-E & CE-H-Si ////////////////////
      /////////////////////////////////////////////////////////////////////////////////////

      float FH_AH_relative_scale = 0.4;
      float alpha_ = FH_AH_relative_scale;
      float EE_scale = 94.624; //MIPs per GeV
      // float FH_AH_scale = 12.788; //MIPs per GeV
      float FH_AH_scale = 12.66; //MIPs per GeV


      float total_energy = (rechitEnergySum_FH+ (alpha_ * rechitEnergySum_AH))/FH_AH_scale;
      float total_energy_withoutAH = (rechitEnergySum_FH)/FH_AH_scale;
      

      h_EE_MIPsinEE->Fill(rechitEnergySum_EE);
      h_FH_MIPsinEE->Fill(rechitEnergySum_FH);
      h_AH_MIPsinEE->Fill(rechitEnergySum_AH);
      h_all_MIPsinEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySum_AH);

      h_MIPsinEE_elecpi_scale->Fill(total_energy);
      h_MIPsinEE_elecpi_scale_withoutAH->Fill(total_energy_withoutAH);
      
      bool trackwindow = isInTrackWindow;
      bool SS1_reject = (shower_start_index+1 > 1);
      bool SS2_reject = (shower_start_index+1 > 2);

      /////////////////////////////////////////////////////////
      /////     H hadrons ; Chi2 matrix initialzation     /////
      /////////////////////////////////////////////////////////

      double FH_detscale = rechitEnergySum_FH/FH_AH_scale;
      double AH_detscale = (alpha_ * rechitEnergySum_AH)/FH_AH_scale;

      //double O = 2.5;
      double O = 0.0;

      float scale_ = scaling_factor_H[(int)beamEnergy];

      float w1 = getChi2Weights_H(beamEnergy).at(0);
      float w2 = getChi2Weights_H(beamEnergy).at(1);
      float w3 = getChi2Weights_H(beamEnergy).at(2);

      // float w1 = f_H_w1->Eval(beamEnergy);
      // float w2 = f_H_w2->Eval(beamEnergy);
      // float w3 = f_H_w3->Eval(beamEnergy);

      float recoE_w1 = f_H_w1->Eval(total_energy);
      float recoE_w2 = f_H_w2->Eval(total_energy);
      float recoE_w3 = f_H_w3->Eval(total_energy);

      float chi2_energy_recoE = recoE_w1*1.0 + recoE_w2*FH_detscale + recoE_w3*AH_detscale + 0.4;
      
      // if((int)beamEnergy == 20) {w1= 0.0; w2=1.015 ; w3=1.22588 ; }
      float chi2_energy = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH + w3*rechitEnergySum_AH + O;
      float chi2_energy_withoutAHCAL = w2*rechitEnergySum_FH;

      bool IsIn_en_range = true;

      if(chi2_method > 0)  {
	if(chi2_method > 1) O = 0.4;
	if(chi2_method > 2) IsIn_en_range = (total_energy > en_range_FH_xmin[(int)beamEnergy] && total_energy < en_range_FH_xmax[(int)beamEnergy]);
	chi2_energy = w1*1.0 + w2*FH_detscale + w3*AH_detscale + O;
	chi2_energy_withoutAHCAL = w1*1.0 + w2*FH_detscale + O;
      }

      h_energy_MipsInEE_gev_chi2->Fill(chi2_energy);
      h_energy_MipsInEE_gev_chi2_rescaled->Fill(scale_ * chi2_energy);

      h_energy_MipsInEE_gev_chi2_withoutAHCAL->Fill(chi2_energy_withoutAHCAL);

      

      /////////////////////////////////////////////////////
      //////////////// selection cut check  ////////////////
      /////////////////////////////////////////////////////

      
      h_baseline_FH->Fill(total_energy);
      h_baseline_chi2_FH->Fill(chi2_energy);

      if(trackwindow)  {
	h_trackwindow_FH->Fill(total_energy);
	h_trackwindow_chi2_FH->Fill(chi2_energy);
	h_trackwindow_chi2_FH_recoE->Fill(chi2_energy_recoE);
	
      }
      if(SS1_reject) {
	h_SS1_reject_FH->Fill(total_energy);
      }
      if(SS2_reject) {
	h_SS2_reject_FH->Fill(total_energy);
      }
      if(SS1_reject && trackwindow) {
	h_trackwindow_SS1_reject_FH->Fill(total_energy);
      }
      if(SS2_reject && trackwindow) {
	h_trackwindow_SS2_reject_FH->Fill(total_energy);
	
	
      }

      /////////////////////////////////////////////////////
      /////////////////////////////////////////////////////


    }

    else if(isRegion3) {  //Showering in CE-E
      h_shower_start_reg3->Fill(shower_lambda_);
      region_3_classified_events++; 
      
      h_rechit_energy_frac_EE_last3_ShowerInEE->Fill(EE_last3/rechitEnergySum_EE);
      h_rechit_energy_frac_FH_last3_ShowerInEE->Fill(FH_last3/rechitEnergySum_FH); 
      h_rechit_energy_frac_AH_last3_ShowerInEE->Fill(AH_last3/rechitEnergySum_AH);

      //////////////////////////////////////////////////////////////////////////////////////
      ///////////////   Based on Electron/pion scale in CE-E & CE-H-Si //////////////////////
      //////////////////////////////////////////////////////////////////////////////////////

      float FH_AH_relative_scale = 0.4;
      float alpha_ = FH_AH_relative_scale;
      float EE_scale = 94.624; //MIPs per GeV
      // float FH_AH_scale = 12.788; //MIPs per GeV
      float FH_AH_scale = 12.66; //MIPs per GeV

      float total_energy = (rechitEnergySum_EE/EE_scale) + (rechitEnergySum_FH+ (alpha_ * rechitEnergySum_AH))/FH_AH_scale;
      float total_energy_withoutAH = (rechitEnergySum_EE/EE_scale) + (rechitEnergySum_FH)/FH_AH_scale;
      

      h_EE_showerinEE->Fill(rechitEnergySum_EE);
      h_FH_showerinEE->Fill(rechitEnergySum_FH);
      h_AH_showerinEE->Fill(rechitEnergySum_AH);
      h_all_showerinEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySum_AH);

      h_showerinEE_elecpi_scale->Fill(total_energy);
      h_showerinEE_elecpi_scale_withoutAH->Fill(total_energy_withoutAH);

      //////////////////////////////////////////////////////////////////////////////////////

      h_rechit_energy_raw_EE_ShowerInEE->Fill(rechitEnergySum_EE);
      h_rechit_energy_raw_FH_ShowerInEE->Fill(rechitEnergySum_FH);
      h_rechit_energy_raw_EE_vs_FH_ShowerInEE->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
      h_rechit_energy_raw_FH_vs_AH_ShowerInEE->Fill(rechitEnergySum_FH,rechitEnergySum_AH);
      h_rechit_energy_raw_AH_ShowerInEE->Fill(rechitEnergySum_AH);
      h_rechit_energy_raw_all_ShowerInEE->Fill(rechitEnergySum_EE+rechitEnergySum_FH+rechitEnergySum_AH);

      bool trackwindow = isInTrackWindow;
      bool SS1_reject = (shower_start_index+1 > 1);
      bool SS2_reject = (shower_start_index+1 > 2);

      ///////////////////////////////////////////////////////////
      /////     EH hadrons ; Chi2 matrix initialzation     /////
      //////////////////////////////////////////////////////////
      
      double EE_detscale = (rechitEnergySum_EE/EE_scale);
      double FH_detscale = (rechitEnergySum_FH/FH_AH_scale);
      double AH_detscale = (alpha_*rechitEnergySum_AH)/FH_AH_scale;

      float scale_ = scaling_factor_EH[(int)beamEnergy];
      
      float w1 = getChi2Weights_EH(beamEnergy).at(0);
      float w2 = getChi2Weights_EH(beamEnergy).at(1);
      float w3 = getChi2Weights_EH(beamEnergy).at(2);
      
      // float w1 = f_EH_w1->Eval(beamEnergy);
      // float w2 = f_EH_w2->Eval(beamEnergy);
      // float w3 = f_EH_w3->Eval(beamEnergy);

      float recoE_w1 = f_EH_w1->Eval(total_energy);
      float recoE_w2 = f_EH_w2->Eval(total_energy);
      float recoE_w3 = f_EH_w3->Eval(total_energy);

      if(DEBUG) { cout<<"DEBUG :: beamE : w1 : w2 : w3 : total_E : Rw1 : Rw2 : Rw3 :: "<<beamEnergy<< " : " <<" : "<<w1 <<" : "<<w2<<" : "<<w3<<" : "<<total_energy<<" : "<<recoE_w1<< " : " << recoE_w2 << " : "<< recoE_w3 <<endl;}
      
      float chi2_energy_recoE = recoE_w1*EE_detscale + recoE_w2*FH_detscale + recoE_w3*AH_detscale;

      double O = 0.0;
      float chi2_energy = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH + w3*rechitEnergySum_AH + O;
      float chi2_energy_withoutAHCAL = w1*rechitEnergySum_EE + w2*rechitEnergySum_FH;
     
      bool IsIn_en_range = true;

      if(chi2_method > 0)  {
	if(chi2_method > 1) O = 0.0;
	if(chi2_method > 2) IsIn_en_range = (total_energy > en_range_EE_xmin[(int)beamEnergy] && total_energy < en_range_EE_xmax[(int)beamEnergy]);
	chi2_energy = w1*EE_detscale + w2*FH_detscale + w3*AH_detscale + O;
	chi2_energy_withoutAHCAL = w1*EE_detscale + w2*FH_detscale + O;
      }

      h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2->Fill(chi2_energy);
      h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2_rescaled->Fill(scale_ * chi2_energy);


      h_rechit_energy_full_weighted_inGev_ShowerInEE_chi2_withoutAHCAL->Fill(chi2_energy_withoutAHCAL);




      /////////////////////////////////////////////////////
      //////////////// selection cut check  ////////////////
      /////////////////////////////////////////////////////

      
      h_baseline->Fill(total_energy);
      h_baseline_chi2_EE->Fill(chi2_energy);

      if(trackwindow)  {
	h_trackwindow->Fill(total_energy);
	h_trackwindow_chi2_EE->Fill(chi2_energy);
	
      }
      if(SS1_reject) {
	h_SS1_reject->Fill(total_energy);
	h_SS1_reject_chi2_EE->Fill(chi2_energy);
      }
      if(SS2_reject) {
	h_SS2_reject->Fill(total_energy);
	h_SS2_reject_chi2_EE->Fill(chi2_energy);
      }
      if(SS1_reject && trackwindow) {
	h_trackwindow_SS1_reject->Fill(total_energy);
	h_trackwindow_SS1_reject_chi2_EE->Fill(chi2_energy);
      }
      if(SS2_reject && trackwindow) {
	h_trackwindow_SS2_reject->Fill(total_energy);
	h_trackwindow_SS2_reject_chi2_EE->Fill(chi2_energy);
	h_trackwindow_SS2_reject_chi2_EE_recoE->Fill(chi2_energy_recoE);
      }

      /////////////////////////////////////////////////////
      /////////////////////////////////////////////////////


    }

    //Showering in AHCAL ????
    if(isRegion4) {
      region_4_classified_events++;
    }

    time_EnMeasureCalc += sw_energy.RealTime();
    sw_energy.Stop();
    
    if(Nrechit_EE == 0) {
      cout<<" **** Nrechit_EE is zero for jentry = "<<jentry<<endl;
      cout<<" isRegion1 "<<isRegion1
	  <<" isRegion2 "<<isRegion2
	  <<" isRegion3 "<<isRegion3
	  <<" isRegion4 "<<isRegion4<<endl;
      cout<<" isGoodTrack : isInTrackWindow :: "<<isGoodTrack<<" : "<<isInTrackWindow<<endl;
      cout<<"*************"<<endl;
    }
    
    // if(MuonVeto) continue;
    // event_count[3]++;
    

    if(DEBUG) { cout << "SW >>>>>>END of event, Took: "; sw.Print(); }
    if(DEBUG) cout<<"DEBUG: End of Event = "<<jentry+1<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;

    if(DEBUG && jentry > 5) return;
    
    // if(jentry > 10) break;
    
  } // loop over entries

  if(E_beam < 0) {
    cout<<"E_beam negative!!!"<<endl;
    return;
  }


  cout<<"***** Timing budget summmary ******"<<endl;
  cout << " Central clocktime; Took: "; sw.Print("m"); 
  sw.Stop();

  
  cout<<"Total calulated time(sec) : "<<(time_hgc+time_ahc+time_CompartCalc+time_LongiCalc+time_EnMeasureCalc)<<endl;
  cout<<"HGC took = "<< time_hgc <<endl;
  cout<<"AHC took = "<< time_ahc <<endl;
  cout<<"Compart Calc took = "<< time_CompartCalc <<endl;
  cout<<"Longi Calc took = "<< time_LongiCalc <<endl;
  cout<<"Energy measured took = "<< time_EnMeasureCalc <<endl;
  cout<<"***************************"<<endl<<endl;
  //cout<<"Got Out, total events = "<<jentry<<" ; selected events = "<<selected_events<<endl;

  cout<<"***** Event summary ******"<<endl;
  cout<<"Total events = "<<event_count[0]<<endl;
  cout<<"Events with good track = "<<event_count[1]<<endl;
  cout<<"Events with Nrechits in FH[8][3] < 80 and FH[8][4] < 80 = "<<event_count[2]<<endl;
  cout<<" Events passing Muon veto = "<<event_count[3]<<endl;
  cout<<"***************************"<<endl<<endl;
  

  Long64_t total_events_temp = (region_1_classified_events+region_2_classified_events+region_3_classified_events+region_4_classified_events+non_classified_events);
  cout<<"Events with zero hits in EE = "<<EE_zeroHit<<endl;
  cout<<"Events with zero hits in FH = "<<FH_zeroHit<<endl;
  cout<<"Events with zero hits in AHCAL = "<<ahc_zeroHit<<endl;
  cout<<"MIP like events = "<<((float)region_1_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"shower start in EE = "<<((float)region_3_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"shower start in FH = "<<((float)region_2_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"shower start in AH = "<<((float)region_4_classified_events*100.0)/total_events_temp<<"%"<<endl;
  cout<<"Non-classified events = "<<((float)non_classified_events*100.0)/total_events_temp<<"%"<<endl;
  
  cout<<"Number of events with (Nrechit_EE == 0) = "<<EE_zeroHit_R2<<endl;
  cout<<"Sum = "<<total_events_temp<<endl;

  cout<<endl<<endl;

}

