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
  // cout << "configuration " << config << " " << endl;
  cout << "energy " << energy << " " << endl;
  // cout << "UseSSBasedEventCategorization " << UseSSBasedCat << " " << endl;
  // cout<< "FH_noise "<<noise_FH_<<endl<<endl;

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
  
  if(strcmp(data,"data") != 0) {
     std::cout<<BOLDGREEN<<"Sim has been rescaled already, EE = 1.04, FH = 1.04 and AH = 1.02"<<RESET<<std::endl;
  }
  
  int decade = 0;
  Long64_t ahc_zeroHit = 0;
  Long64_t EE_zeroHit = 0;
  Long64_t FH_zeroHit = 0;
  Long64_t EE_zeroHit_R2 = 0;
  int chi2_method = 3;
  // std::map<int,double> E_thres;
  // E_thres[20] = 12.0; E_thres[50] = 20.0; E_thres[80] = 25.0; E_thres[100] = 30.0; E_thres[120] = 30.0; E_thres[200] = 40.0; E_thres[250] = 40.0; E_thres[300] = 40.0;

  float lambda[79];
  for(int i = 0; i < 79; i++) {
    lambda[i] = layer_positions[i+1].at(3); //pi_lambda
    //std::cout<<i+1<<"\t"<<lambda[i]<<endl;
  }


  bool DEBUG = false;
  int debug_count = 0;
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





  double E_beam = -1.0;


  if(DEBUG) cout<<"DEBUG: Configuration = "<<conf_<<endl;
  if(DEBUG) cout<<"DEBUG: TOTAL_ACTIVE_LAYER = "<<TOTAL_ACTIVE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: EE_LAYER = "<<EE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: FH_LAYER = "<<FH_LAYER<<endl;

  if(DEBUG) cout<<"DEBUG: Entering event Loop"<<endl;
  Long64_t jentry;


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

    double energySum_all = 0.0;
    double energySum_TrackQuality = 0.0;
    double energySum_MuonVeto = 0.0;
    double energySum_PS_reject = 0.0;
    double energySum_TrackWindow = 0.0;
    double energySum_ChannelMask = 0.0;
    double energySum_NoiseReject = 0.0;

    
    ////////////////////////////////////////////
    //            HGCAL Part                  //
    ////////////////////////////////////////////

    /// Read Tree
    if(DEBUG) cout<<"DEBUG: Start Analylizing RecHits!!"<<endl;


    for(int i = 0 ; i < NRechits; i++){
      energySum_all += rechit_energy->at(i);
      if(isGoodTrack) {
	energySum_TrackQuality += rechit_energy->at(i);
	if(!MuonVeto) {
	  energySum_MuonVeto += rechit_energy->at(i);
	  if(rechit_shower_start_layer > 2) {
	    energySum_PS_reject += rechit_energy->at(i);
	    if(isInTrackWindow) {
	      energySum_TrackWindow += rechit_energy->at(i);
	      if(hgc_channel_mask->at(i) == 0) {
		energySum_ChannelMask += rechit_energy->at(i);
		if(pass_noise_thres->at(i) == 1) {
		  energySum_NoiseReject += rechit_energy->at(i);
		}
	      }
	    }
	  }
	}
      }
      
    }


    ///////////////////////////////////////////
    ////////// End of HGCAL nRechit Loop /////
    //////////////////////////////////////////


    //////////////////////////////////////
    //// AHCAL part
    /////////////////////////////////

    for(int i = 0 ; i < ahc_nHits; i++) {
      energySum_all += ahc_hitEnergy->at(i);
      if(isGoodTrack) {
	energySum_TrackQuality += ahc_hitEnergy->at(i);
	if(!MuonVeto) {
	  energySum_MuonVeto += ahc_hitEnergy->at(i);
	  if(rechit_shower_start_layer > 2) {
	    energySum_PS_reject += ahc_hitEnergy->at(i);
	    if(isInTrackWindow) {
	      energySum_TrackWindow += ahc_hitEnergy->at(i);
	      if(ahc_channel_mask->at(i) == 0) {
		energySum_ChannelMask += ahc_hitEnergy->at(i);
		energySum_NoiseReject += ahc_hitEnergy->at(i);
	      }
	    }
	  }
	}
      }

      
    }
    h_rechit_energy_raw_all->Fill(energySum_all);
    if(!isGoodTrack) continue;
    h_rechit_energy_TrackQuality_all->Fill(energySum_TrackQuality);
    if(MuonVeto) continue;
    h_rechit_energy_MuonVeto_all->Fill(energySum_MuonVeto);
    if(rechit_shower_start_layer <=2 ) continue;
    h_rechit_energy_PreShowerReject_all->Fill(energySum_PS_reject);
    if(!isInTrackWindow) continue;
    h_rechit_energy_TrackWindow_all->Fill(energySum_TrackWindow);
    
    h_rechit_energy_ChannelMask_all->Fill(energySum_ChannelMask);
    h_rechit_energy_NoiseRejection_all->Fill(energySum_NoiseReject);

    if(rechit_shower_start_layer > 2 && rechit_shower_start_layer <= 28) {
      h_rechit_energy_all_cuts_showering_in_EE->Fill(energySum_NoiseReject);
    }
    if(rechit_shower_start_layer > 28) {
      h_rechit_energy_all_cuts_MIPs_in_EE->Fill(energySum_NoiseReject);
    }
    if(rechit_shower_start_layer == -1) {
      h_rechit_energy_all_cuts_MIPs_in_EEFH->Fill(energySum_NoiseReject);
    }
  }
  //////////////////////////////////////////////
  //    E N D   O F   E N T R Y    L O O P    /        
  /////////////////////////////////////////////
  
  if(E_beam < 0) {
    cout<<"E_beam negative!!!"<<endl;
    return;
  }


  cout<<endl<<endl;

}

