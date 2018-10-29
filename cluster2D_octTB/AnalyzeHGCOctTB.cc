#define AnalyzeHGCOctTB_cxx

#include <iostream>
#include <vector>
#include <cstring>
#include "RecHit.h"
#include "Cluster2D.h"
#include "AnalyzeHGCOctTB.h"
#include <fstream>
using namespace std;



// chip 3022,44,3028




int main(int argc, char* argv[])
{

  if (argc < 3) {
    cerr << "Please give 4 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" " << "configuration" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *config          = argv[4];

  AnalyzeHGCOctTB hgcOctTB(inputFileList, outFileName, data, config);
  cout << "dataset " << data << " " << endl;
  cout << "congiguration " << config << " " << endl;

  hgcOctTB.EventLoop(data);
  return 0;
}

void AnalyzeHGCOctTB::EventLoop(const char *data) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  int decade = 0;

  float rechit_cut[28] = {0.02,0.039,0.019,0.02,0.037,0.076,0.10,
			  0.14,0.18,0.24,0.28,0.33,0.48,0.52,
			  0.56,0.6,0.64,0.70,0.74,0.78,0.82,
			  0.86,0.90,0.92,0.94,0.95,0.98,1.0};
  Long64_t cut_count[28];
  Long64_t nEvents = 0;
  Long64_t MIP_pions = 0;
  for(int i = 0; i< 28; i++){ cut_count[i] = 0;}
  Long64_t jentry;
  //for (Long64_t jentry=0; jentry<nentries;jentry++) {
  for (jentry=0; jentry<nentries;jentry++) {
    // cout<<"Hello"<<endl;
    // if(jentry != 10044 ) continue;
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;
    
    // ===============read this entry == == == == == == == == == == ==

    Long64_t ientry = LoadTree(jentry);
    // cout<<"ientry = "<<ientry<<endl;
    //if (ientry < 0) break;
    if (ientry < 0) { break; cout<<"Breaking"<<endl;}
        // cout<<"****"<<endl;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if(NRechits == 0) continue;


    Double_t rechitEnergySum = 0.0;
    Double_t rechitEnergySum_EE = 0.0;
    Double_t rechitEnergySum_FH = 0.0;
    Double_t un_cali = 0.0;
    Long_t Nrechit_layer[40];
    Long_t NRechits_EE[28];
    Long_t NRechits_FH[12][7];
    int module_part_ = 0;
    int module_layer_ = 0;
    int module_position_ = 0;
    for(int ii=0;ii<40;ii++){
      if(ii<28)
	NRechits_EE[ii]=0;
      Nrechit_layer[ii]=0;
    }
    for(int ii=0;ii<12;ii++){
      for(int jj=0;jj<7;jj++){
      NRechits_FH[ii][jj]=0;
      }
    }
    std::vector<std::vector<float>> layer1;
    std::vector<int> temp_moduleID;
    std::vector<RecHit*> rechits[40];
    for(int i = 0 ; i < 40; i++) {
      rechits[i].clear();
    }

    for(int i = 0 ; i < NRechits; i++){
      temp_moduleID.clear();
      int temp_layer = rechit_layer->at(i);
      int temp_chip = rechit_chip->at(i);
      int temp_channel = rechit_channel->at(i);
      int en_chan = temp_chip*1000+temp_channel;
      if(en_chan == 3022 || en_chan == 3028 || en_chan == 44) continue;
      
      if(temp_layer == 1) {
	std::vector<float> tt1;
	tt1.push_back((float)en_chan);
	tt1.push_back((float)rechit_iu->at(i));
	tt1.push_back((float)rechit_iv->at(i));
	tt1.push_back((float)rechit_energy->at(i));
	layer1.push_back(tt1);
      }
      
      //if(rechit_amplitudeHigh->at(i) < 20) continue;
      Nrechit_layer[temp_layer]++;

      temp_moduleID = getModuleLocation(rechit_module->at(i));
      if(!temp_moduleID.size() || temp_moduleID.size()<3) {
	cout<<"Could NOT locate MODULE location!!!!"<<endl;
	return;
      }
      module_part_ = temp_moduleID.at(0);
      module_layer_ = temp_moduleID.at(1);
      module_position_ = temp_moduleID.at(2);
      int channel_bin = getBIN(temp_chip,temp_channel);
      if (channel_bin < 0) { cout<<"INVALID CHANNEL!!!"<<endl; return;}
      h_moduleID->Fill(module_part_);
      if(module_part_ == 0) {
      	rechitEnergySum_EE+=rechit_energy->at(i);
	NRechits_EE[module_layer_-1]++;
      	h_adcHG_EE[module_layer_-1][channel_bin]->Fill(rechit_amplitudeHigh->at(i));
      }
      if(module_part_ == 1) {
      	rechitEnergySum_FH+=rechit_energy->at(i);
	NRechits_FH[module_layer_-1][module_position_-1]++;
      	h_adcHG_FH[module_layer_-1][module_position_-1][channel_bin]->Fill(rechit_amplitudeHigh->at(i));
      }
      rechitEnergySum+=rechit_energy->at(i);
      RecHit* temp_rechit = new RecHit(temp_layer, temp_chip, temp_channel, en_chan, module_part_, module_position_, rechit_module->at(i), rechit_iu->at(i), rechit_iv->at(i), rechit_x->at(i), rechit_y->at(i), rechit_z->at(i), rechit_energy->at(i), false, 0);
      rechits[temp_layer-1].push_back(temp_rechit);
    }

    cout<<"**********N E W    E V E N T**********"<<endl;
    cout<<"Event = "<<jentry+1<<endl;


    for(int i = 0; i<40; i++) {
      // if(i+1 != 13) continue;
      cout<<endl<<endl;
      cout<<"Size of rechit object for layer "<<i+1<<" = "<<rechits[i].size()<<endl;

      cout<<"INFO: Calculating  density & delta for layer = "<<i+1<<endl;
      std::vector<RecHit*> rechit_collection_1 = makeCluster(rechits[i]);
      cout<<"INFO: Sending Rechit_collection of layer "<<i+1<<" to clustering algorithm..."<<endl;

      //std::vector<Cluster2D*> cluster_collection_1 = ClusterProducer(rechit_collection_1);
      std::vector<Cluster2D*> cluster_collection_1;
      cluster_collection_1 = ClusterProducer(rechit_collection_1);

      // cout<<"Look Here ********"<<endl;
      // cout<<"S I Z E = "<<cluster_collection_1.size()<<endl;
      
      if(!cluster_collection_1.size()) {
	cout<< "INFO: Something Went wrong!!!"<<endl;
	return;
      }
      else {
	cout<<"INFO: clustering successfull!!!"<<endl;
	// cout<<"INFO: number of seeds found = "<<cluster_collection_1.size()<<endl;
	// int total_=0;
	for(int j = 0; j <(int)cluster_collection_1.size(); j++) {
	  auto rank_ = cluster_collection_1.at(j)->getClusterRank();
	  std::vector<RecHit*> rh_ = cluster_collection_1.at(j)->getRechitCollection();
	  // total_+=(int)rh_.size();
	  // cout<<"\n\n\n CLUSTER = "<<j+1<<endl;
	  for(int k = 0; k < (int)rh_.size(); k++) {
	    RecHit* rh_k = rh_[k];
	    auto layer_ = rh_k->getLayer();
	    auto channel_ = rh_k->getEncodedChannel();
	    auto energy_ = rh_k->getEnergy();
	    auto density_ = rh_k->getDensity();
	    auto delta_ = rh_k->getDelta();
	    auto x_ = rh_k->getCellCoordinate_xyz().at(0);
	    auto y_ = rh_k->getCellCoordinate_xyz().at(1);

	    // cout<<k+1<<"\t"<<energy_
	    // 	<<"\t"<<density_
	    // 	<<"\t"<<delta_
	    // 	<<"\t"<<x_
	    // 	<<"\t"<<y_
	    // 	<<endl;


	    char* line = new char[500];
	    // sprintf()
	    sprintf(line,"%lld\t %d\t %d\t %0.3f\t %0.3f\t %0.3f\t %0.3f\t %0.3f\t %d\n",jentry,layer_,channel_,x_,y_,energy_,density_, delta_,rank_);
	    cout<<line<<endl;
	    myfile << line;
	  }
	}

	// std::cout<<"TOTAL Rechit accounted for = "<<total_<<endl;
      }
      rechit_collection_1.clear();
      cluster_collection_1.clear();
    }
    cout<<endl<<endl;
     
    
    cout<<"INFO: end of Event "<<jentry+1<<endl;
    cout<<"*********************"<<endl;
    return;
    //if(jentry == 1) return;
    Long_t Nrechit_EE = 0;
    Long_t Nrechit_FH = 0;
    //cout<<"Config = "<<conf_<<endl;
    for(int iL = 0; iL < 40; iL++){
      if(iL < 28) {
	Nrechit_EE+=NRechits_EE[iL];
    	h_NRechits_EE[iL]->Fill(NRechits_EE[iL]);
      }
      else {
	for(int j = 0; j < 7; j++) {
	  if(!strcmp(conf_,"alpha") && (j+1) !=4 && ((iL-28)+1) > 9 ) continue;
	  Nrechit_FH+=NRechits_FH[iL-28][j];
	  h_NRechits_FH[iL-28][j]->Fill(NRechits_FH[iL-28][j]);
	}
      }
    }

    
    h_particleID->Fill(pdgID);
    h_beamEnergy->Fill(beamEnergy);
    h_runNumber->Fill(run);
    h_rechit_energy_all->Fill(rechitEnergySum);
    h_rechit_energy_EE_only->Fill(rechitEnergySum_EE);
    h_rechit_energy_FH_only->Fill(rechitEnergySum_FH);
    h_rechit_energy_EE_vs_FH->Fill(rechitEnergySum_EE,rechitEnergySum_FH);
    h_rechit_En_NRechits->Fill(NRechits,rechitEnergySum);
    h_Nrechit_EE_vs_FH->Fill(Nrechit_EE,Nrechit_FH);
    h_Nrechit_EE->Fill(Nrechit_EE);
    h_Nrechit_FH->Fill(Nrechit_FH);

  } // loop over entries

  cout<<"Got Out "<<jentry<<endl;
  // cout<<"% Events survived = "<<((float)MIP_pions/nEvents)*100 << endl;
}

