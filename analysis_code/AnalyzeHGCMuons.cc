#define ANALYZEHGCMuons_cxx

#include <iostream>
#include <vector>
#include <cstring>
#include "AnalyzeHGCMuons.h"

using namespace std;


// int getBIN(unsigned int skiroc,unsigned int channel) {
//   if (channel%2 != 0 || channel > 62 || skiroc > 3) return -1;
//   else return (skiroc*32 + (channel/2));
// }



int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please give 3 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  AnalyzeHGCMuons hgcmuons(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  hgcmuons.EventLoop(data);

  return 0;
}

void AnalyzeHGCMuons::EventLoop(const char *data) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nbytes2 = 0, nb2 = 0;
  Long64_t nbytes3 = 0, nb3 = 0;
  int decade = 0;

  bool print_=false;


  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;
    
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    nb2 = fChain2->GetEntry(jentry);   nbytes2 += nb2;
    nb3 = fChain3->GetEntry(jentry);   nbytes3 += nb3;

    //if(jentry!=1640) continue;

    // if(tablePositionY>-999) {
    //   // cout<<"*** N E W   E V E N T ***"<<endl;
    //   // cout<<"TablePositionY = "<<tablePositionY<<endl;
    //   h_tablePositionY->Fill(tablePositionY);
    // }
    // cout<<"configuration = "<<configuration<<endl;
    // if(configuration !=17 ) continue;
    //if(tablePositionY > 16.5 || tablePositionY < 16.0) continue;
    //if(tablePositionY==-999.0) continue;
    if(!ntracks) continue;
    h_tablePositionY->Fill(tablePositionY);
    Float_t track_x[28];
    Float_t track_y[28];
    vector<Float_t> dr_[28];
    float dr_min[28] = {1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,1.e10,};
    int count_rechit[28] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    track_x[0] = impactX_HGCal_layer_1;
    track_y[0] = impactY_HGCal_layer_1;
    track_x[1] = impactX_HGCal_layer_2;
    track_y[1] = impactY_HGCal_layer_2;
    track_x[2] = impactX_HGCal_layer_3;
    track_y[2] = impactY_HGCal_layer_3;
    track_x[3] = impactX_HGCal_layer_4;
    track_y[3] = impactY_HGCal_layer_4;
    track_x[4] = impactX_HGCal_layer_5;
    track_y[4] = impactY_HGCal_layer_5;
    track_x[5] = impactX_HGCal_layer_6;
    track_y[5] = impactY_HGCal_layer_6;
    track_x[6] = impactX_HGCal_layer_7;
    track_y[6] = impactY_HGCal_layer_7;
    track_x[7] = impactX_HGCal_layer_8;
    track_y[7] = impactY_HGCal_layer_8;
    track_x[8] = impactX_HGCal_layer_9;
    track_y[8] = impactY_HGCal_layer_9;
    track_x[9] = impactX_HGCal_layer_10;
    track_y[9] = impactY_HGCal_layer_10;
    track_x[10] = impactX_HGCal_layer_11;
    track_y[10] = impactY_HGCal_layer_11;
    track_x[11] = impactX_HGCal_layer_12;
    track_y[11] = impactY_HGCal_layer_12;
    track_x[12] = impactX_HGCal_layer_13;
    track_y[12] = impactY_HGCal_layer_13;
    track_x[13] = impactX_HGCal_layer_14;
    track_y[13] = impactY_HGCal_layer_14;
    track_x[14] = impactX_HGCal_layer_15;
    track_y[14] = impactY_HGCal_layer_15;
    track_x[15] = impactX_HGCal_layer_16;
    track_y[15] = impactY_HGCal_layer_16;
    track_x[16] = impactX_HGCal_layer_17;
    track_y[16] = impactY_HGCal_layer_17;
    track_x[17] = impactX_HGCal_layer_18;
    track_y[17] = impactY_HGCal_layer_18;
    track_x[18] = impactX_HGCal_layer_19;
    track_y[18] = impactY_HGCal_layer_19;
    track_x[19] = impactX_HGCal_layer_20;
    track_y[19] = impactY_HGCal_layer_20;
    track_x[20] = impactX_HGCal_layer_21;
    track_y[20] = impactY_HGCal_layer_21;
    track_x[21] = impactX_HGCal_layer_22;
    track_y[21] = impactY_HGCal_layer_22;
    track_x[22] = impactX_HGCal_layer_23;
    track_y[22] = impactY_HGCal_layer_23;
    track_x[23] = impactX_HGCal_layer_24;
    track_y[23] = impactY_HGCal_layer_24;
    track_x[24] = impactX_HGCal_layer_25;
    track_y[24] = impactY_HGCal_layer_25;
    track_x[25] = impactX_HGCal_layer_26;
    track_y[25] = impactY_HGCal_layer_26;
    track_x[26] = impactX_HGCal_layer_27;
    track_y[26] = impactY_HGCal_layer_27;
    track_x[27] = impactX_HGCal_layer_28;
    track_y[27] = impactY_HGCal_layer_28;

    
    int dr_min_i = 0.0;
    bool l1 = true;
    for(int i = 0; i<28; i++) dr_[i].clear();


    if(print_) cout<<"*** N E W   E V E N T ***"<<endl;
    if(print_) cout<<"track_x = "<< track_x <<" ,track_y = "<< track_y 
		   <<" ,track_chi2_X= "<<trackChi2_X<<" ,track_chi2_Y= "<<trackChi2_Y<<endl;
    if(print_) cout<<"NRechits =  "<<NRechits<<endl;

    for(Int_t i = 0; i < NRechits; i++) {
      
      int temp_channel = -1, temp_chip = -1, temp_layer = -1;
      temp_channel = rechit_channel->at(i);
      temp_chip = rechit_chip->at(i);
      temp_layer = (rechit_layer->at(i)-1);
      if(temp_layer<0 || temp_chip <0 || temp_channel<0) {cout<<"NEGATIVE VALUE??, skipping rechit loop..."<<endl; continue; }
      if((temp_chip*1000+temp_channel)==3022||(temp_chip*1000+temp_channel)==44) { /*cout<<"GOT IT"<<endl;*/ continue; }
      
      if(jentry==24 && false) {
      	cout<<"** "<<jentry<<", layer="<<temp_layer<<" ,(rechit_layer->at(i)-1)="<<(rechit_layer->at(i)-1)<<endl;
      }
      // if(count_rechit[temp_layer]==2) continue;
      count_rechit[temp_layer]++;



      //cout<<"*** N E W   E V E N T ***"<<endl;

      if (temp_layer == 0 || true) {
	float dR = deltaR(track_x[temp_layer],track_y[temp_layer],rechit_x->at(i),rechit_y->at(i));
	if (dR < dr_min[temp_layer]) {dr_min[temp_layer] = dR;dr_min_i = i;}

	dr_[temp_layer].push_back(dR);
	h_deltaR_layer[temp_layer]->Fill(dR);

	l1 = false;

	//if((print_ || (dr_min[temp_layer] > 2.5 && dr_min[temp_layer] < 3.5) && false) || jentry==92718) {
	if(jentry==24 && false) {
	  cout<<"(rechit_x,rechit_y)= ("<<rechit_x->at(i)<<","<<rechit_y->at(i)<<")"<<endl;
	  cout<<"(track_x,track_y)= ("<<track_x[temp_layer]<<","<<track_y[temp_layer]<<")"<<endl;
	  cout<< "Channel_ID="<< ((1000*rechit_chip->at(i))+rechit_channel->at(i))<< ", dR = "<<dR<< endl;
	  cout<<"ADC count = "<<rechit_amplitudeHigh->at(i)<<endl;

	}
      }


      // if (temp_layer == 0) {
      // 	float dR = deltaR(track_x[temp_layer],track_y[temp_layer],rechit_x->at(i),rechit_y->at(i));
      // 	if (dR < dr_min[temp_layer]) {dr_min[temp_layer] = dR;dr_min_i = i;}

      // 	dr_[temp_layer].push_back(dR);
      // 	h_deltaR_layer[temp_layer]->Fill(dR);

      // 	if(print_ || (dr_min[temp_layer] > 2.5 && dr_min[temp_layer] < 3.5) && false) {
      // 	  cout<<"(rechit_x,rechit_y)= ("<<rechit_x->at(i)<<","<<rechit_y->at(i)<<")"<<endl;
      // 	  cout<<"(track_x,track_y)= ("<<track_x[0]<<","<<track_y[0]<<")"<<endl;
      // 	  cout<< "Channel_ID="<< ((1000*rechit_chip->at(i))+rechit_channel->at(i))<< ", dR = "<<dR<< endl;
      // 	  cout<<"ADC count = "<<rechit_amplitudeHigh->at(i)<<endl;

      // 	}
      // }
      float dx = (rechit_x->at(i)-track_x[temp_layer]);
      float dy = (rechit_y->at(i)-track_y[temp_layer]);
      h_ADChg[temp_layer][getBIN(temp_chip,temp_channel)]->Fill(rechit_amplitudeHigh->at(i));
      h_rechitXY_layer[temp_layer]->Fill(rechit_x->at(i),rechit_y->at(i));
      h_trackX_rechitX_layer[temp_layer]->Fill(track_x[temp_layer],rechit_x->at(i));
      h_trackY_rechitY_layer[temp_layer]->Fill(track_y[temp_layer],rechit_y->at(i));
      h_dXY_layer[temp_layer]->Fill(dx,dy);
      if(run < 162) {
	h_dX_run_132_161_layer[temp_layer]->Fill(run,dx);
	h_dY_run_132_161_layer[temp_layer]->Fill(run,dy);
      }
      else if(run > 205 && run < 226) {
	h_dX_run_206_225_layer[temp_layer]->Fill(run,dx);
	h_dY_run_206_225_layer[temp_layer]->Fill(run,dy);
      }
      else {
	h_dX_run_387_392_layer[temp_layer]->Fill(run,dx);
	h_dY_run_387_392_layer[temp_layer]->Fill(run,dy);
      }

    }

    bool zero_RH_flag = false;

    //cout<<"*** N E W   E V E N T ***"<<endl;
    for(Int_t i = 0; i < 28; i++) {
      h_nrechits[i]->Fill(count_rechit[i]);
      //cout<<"RECHITS = "<<count_rechit[i]<<endl;
      if(count_rechit[i]==0) {/*cout << "FOUND ZERO RH,will be skipping\n";*/ zero_RH_flag=true;}
    }

    if(zero_RH_flag) continue;
    //if(l1) continue;
    // if(count_rechit[0] != 1) continue;
    for(Int_t i = 0; i < 28; i++) {
      if(dr_min[i] > 100) { 
	cout <<"layer:min(dR) = "<<i<<":"<< dr_min[i]<< " \nMIN(dR)>100 FOUND at entry="<<jentry<<"!!! \n Exiting program....."<<endl; 
	cout<<"dr_[i].at(0)=>"<<dr_[i].at(0)<<endl;
	cout<<"RECHITS = "<<count_rechit[i]<<endl;
	return;

      }
    }
    //cout<<jentry<<" => MIN:"<<dr_min[0] << ",dr_.size() =>"<<dr_[0].size()<<" ,"<<zero_RH_flag<<endl;
    //cout<<jentry<<" => laye1= MIN(dR):"<<dr_min[0] << ",count_rechit[0] =>"<<count_rechit[0]<<" ,"<<zero_RH_flag<<endl;
    for(Int_t i = 0; i < 28; i++) {
      h_trackXY_layer[i]->Fill(track_x[i],track_y[i]);
      h_trackX_layer[i]->Fill(track_x[i]);
      h_trackY_layer[i]->Fill(track_y[i]);
      h_deltaR_min_layer[i]->Fill(dr_min[i]);

    }
   

    h_min_dR_nrechit_layer1->Fill(dr_min[0],count_rechit[0]);
    h_trackX_chi2->Fill(trackChi2_X);
    h_trackY_chi2->Fill(trackChi2_Y);

  } // loop over entries
  
  // h_deltaR->Draw();
  // getchar();
}

