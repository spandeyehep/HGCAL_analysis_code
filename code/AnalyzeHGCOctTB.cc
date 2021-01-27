#define AnalyzeHGCOctTB_cxx

#include <iostream>
#include <vector>
#include <cstring>
#include "AnalyzeHGCOctTB.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include <math.h>

using namespace std;



// chip 3022,44,3028




int main(int argc, char* argv[])
{

  if (argc < 6) {
    cerr << "Please give 5 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "configuration" 
	 <<" " << "energy" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *config          = argv[4];
  const char *energy = argv[5];
  AnalyzeHGCOctTB hgcOctTB(inputFileList, outFileName, data, config, energy);
  cout << "dataset " << data << " " << endl;
  cout << "configuration " << config << " " << endl;
  cout << "energy " << energy << " " << endl;

  hgcOctTB.EventLoop(data);
  return 0;
}

void AnalyzeHGCOctTB::EventLoop(const char *data) {


  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nentries3 = fChain3->GetEntriesFast();
  Long64_t hgc_jentry = 0;

  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  Long64_t nbytes3 = 0, nb3 = 0;

  

  int decade = 0;
  int ahc_zeroHit = 0;



  bool DEBUG = false;

  int TOTAL_ACTIVE_LAYER = -1;
  int EE_LAYER = -1;
  int FH_LAYER = -1;
  int AH_LAYER = -1;
  if(!strcmp(conf_,"alpha") || !strcmp(conf_,"config1")) {
    TOTAL_ACTIVE_LAYER = 79;
    EE_LAYER = 28;
    FH_LAYER = 12;
    AH_LAYER = 39;
  }
  else if(!strcmp(conf_,"bravo") || !strcmp(conf_,"config2")){
    TOTAL_ACTIVE_LAYER = 78;
    EE_LAYER = 28;
    FH_LAYER = 11;
    AH_LAYER = 39;
  }
  else if(!strcmp(conf_,"charlie") || !strcmp(conf_,"config3")) {
    TOTAL_ACTIVE_LAYER = 59;
    EE_LAYER = 8;
    FH_LAYER = 12;
    AH_LAYER = 39;
    
  }
  else {
    cout<<"ERROR: Unknown configuration!!!!"<<endl;
    return;
  }





  if(DEBUG) cout<<"DEBUG: Configuration = "<<conf_<<endl;
  if(DEBUG) cout<<"DEBUG: TOTAL_ACTIVE_LAYER = "<<TOTAL_ACTIVE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: EE_LAYER = "<<EE_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: FH_LAYER = "<<FH_LAYER<<endl;
  if(DEBUG) cout<<"DEBUG: AH_LAYER = "<<AH_LAYER<<endl;

  if(DEBUG) cout<<"DEBUG: Entering event Loop"<<endl;

  Long64_t jentry;

  for (jentry=0; jentry<nentries;jentry++,hgc_jentry++) {

    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;
    
    // ===============read this entry == == == == == == == == == == ==

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) { break; cout<<"Breaking"<<endl;}
    nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
    nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    nb3 = fChain3->GetEntry(jentry); nbytes3 += nb3;

    
    // ===============Synch HGCAL & AHCAL with same entry == == == == == == == == == == ==

    while(run != runNumber) {
      hgc_jentry++;
      nb = fChain->GetEntry(hgc_jentry);   nbytes += nb;
      nb2 = fChain2->GetEntry(hgc_jentry); nbytes2 += nb2;
    }


    h_beamenergy->Fill(beamEnergy);
    h_particle->Fill(pdgID);



    ////////////////////////////////////////////
    //            HGCAL Part                  //
    ////////////////////////////////////////////
    
    /// Read HGCAL Tree
    if(DEBUG) cout<<"DEBUG: Start Analylizing HGCAL  RecHits!!"<<endl;

    for(int i = 0 ; i < NRechits; i++){

      //.. Do something with each HGCAL rechit  ..//
      
    }

    //////////////////////////////////////////


    

    ////////////////////////////////////////////
    //            AHCAL Part                  //
    ////////////////////////////////////////////

    /// Read AHCAL Tree
    if(DEBUG) cout<<"DEBUG: Start Analylizing AHCAL  RecHits!!"<<endl;

    for(int i = 0 ; i < ahc_nHits; i++) {
      //.. Do something with each AHCAL rechit  ..//
    }


    if(DEBUG) cout<<"DEBUG: End of Entry = "<<jentry<<endl;
    if(DEBUG) cout<<"DEBUG: ****************** "<<endl;
    
    if(DEBUG && jentry > 50) return;

    
  } // loop over entries


  ///////////////////////////////////////////////////////////
  ///////  E N D     O F     E N T R Y     L O O P     //////
  ///////////////////////////////////////////////////////////
  
  cout<<"Got Out "<<jentry<<endl;


}

