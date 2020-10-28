#define Ana_cxx
#include "Ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>


void Ana::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Ana.C
//      root> Ana t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch


  // To call the function, Use following:
  //> Ana a(NULL,200)
  
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   //TFile* file = TFile::Open("out_300mev_10k.root","recreate");

   Long64_t shower = 0;

   auto randgen = new TRandom();
   
   char* outname = new char[2000];
   // sprintf(outname,"out_mu%dgev_10k_rescaling_afterSmearing.root",energy);
   sprintf(outname,"out_mu%dgev_10k.root",energy);
   // sprintf(outname,"yolo.root");
   // sprintf(outname,"out_mu%dgev_10k.root",energy);
   
   TFile* file = TFile::Open(outname,"recreate");
   TH1F* hist_2 = new TH1F("hist_2","hist_2",200,0.,400.0);
   hist_2->GetXaxis()->SetTitle("simHits energy (keV)");
   TH1F* hist_14 = new TH1F("hist_14","hist_14",200,0.,400.0);
   hist_14->GetXaxis()->SetTitle("simHits energy (keV)");
   TH1F* hist_14_mip = new TH1F("hist_14_mip","hist_14_mip",1000,0.,5000.0);
   hist_14_mip->GetXaxis()->SetTitle("MIPs");
   TH1F* hist_28 = new TH1F("hist_28","hist_28",200,0.,400.0);
   hist_28->GetXaxis()->SetTitle("simHits energy (keV)");
   
   TH1F* h_accu_300um = new TH1F("h_accu_300um","h_accu_300um",200,0.,400.0);
   h_accu_300um->GetXaxis()->SetTitle("simHits energy (keV)");
   TH1F* h_accu_200um = new TH1F("h_accu_200um","h_accu_200um",200,0.,400.0);
   h_accu_200um->GetXaxis()->SetTitle("simHits energy (keV)");
   

   TH1F* hist_Layer10 = new TH1F("hist_Layer10","hist_Layer10",200,0.,400.0);

   TH1F* hist_keV_Layer[28];
   TH1F* hist_keV_Layer_zoomed[28];
   TH1F* hist_mip_Layer[28];
   TH1F* hist_mip_Layer_zoomed[28];
   TH1F* hist_mip_Layer_logE[28];
   TH1F* hist_mip_Layer_smeared[28];
   TH1F* hist_mip_Layer_zoomed_smeared[28];

   TDirectory* dir;

   double GeVtoMIP_300um = 85.5;
   double GeVtoMIP_200um = 57.0;

   // double GeVtoMIP_smeared_300um = GeVtoMIP_300um*1.04;
   double GeVtoMIP_smeared_300um = GeVtoMIP_300um*1.0;

   dir = file->mkdir("first_28_EE_layers");
   dir->cd();
   char* hist_name = new char[1000];
   for(int i = 0; i < 28; i++) {
     sprintf(hist_name,"h_keV_Layer%02d",i+1);
     hist_keV_Layer[i] = new TH1F(hist_name,hist_name,400,0,400);
     sprintf(hist_name,"h_keV_Layer%02d_zoomed",i+1);
     hist_keV_Layer_zoomed[i] = new TH1F(hist_name,hist_name,120,60,120);
     sprintf(hist_name,"h_mip_Layer%02d",i+1);
     hist_mip_Layer[i] = new TH1F(hist_name,hist_name,500,0,100);
     sprintf(hist_name,"h_mip_Layer%02d_zoomed",i+1);
     hist_mip_Layer_zoomed[i] = new TH1F(hist_name,hist_name,1000,0,10);
     
     sprintf(hist_name,"h_mip_Layer%02d_logE",i+1);
     hist_mip_Layer_logE[i] = new TH1F(hist_name,hist_name,600,-1.0,5);

     sprintf(hist_name,"h_mip_Layer%02d_smeared",i+1);
     hist_mip_Layer_smeared[i] = new TH1F(hist_name,hist_name,500,0,100);
     sprintf(hist_name,"h_mip_Layer%02d_zoomed_smeared",i+1);
     hist_mip_Layer_zoomed_smeared[i] = new TH1F(hist_name,hist_name,1000,0,10);

   }
   

   TH1F* hist_keV_Layer_FH[26];
   TH1F* hist_keV_Layer_zoomed_FH[26];
   TH1F* hist_mip_Layer_FH[26];
   TH1F* hist_mip_Layer_zoomed_FH[26];
   TH1F* hist_mip_Layer_logE_FH[26];
   TH1F* hist_mip_Layer_FH_smeared[26];
   TH1F* hist_mip_Layer_zoomed_FH_smeared[26];

   TDirectory* dir_FH;

   dir_FH = file->mkdir("first_12_FH_layers");
   dir_FH->cd();
   for(int i = 0; i < 12; i++) {
     sprintf(hist_name,"h_keV_Layer%02d_FH",i+1);
     hist_keV_Layer_FH[i] = new TH1F(hist_name,hist_name,400,0,400);
     sprintf(hist_name,"h_keV_Layer%02d_zoomed_FH",i+1);
     hist_keV_Layer_zoomed_FH[i] = new TH1F(hist_name,hist_name,120,60,120);
     sprintf(hist_name,"h_mip_Layer%02d_FH",i+1);
     hist_mip_Layer_FH[i] = new TH1F(hist_name,hist_name,500,0,100);
     sprintf(hist_name,"h_mip_Layer%02d_zoomed_FH",i+1);
     hist_mip_Layer_zoomed_FH[i] = new TH1F(hist_name,hist_name,1000,0,10);
     
     sprintf(hist_name,"h_mip_Layer%02d_logE_FH",i+1);
     hist_mip_Layer_logE_FH[i] = new TH1F(hist_name,hist_name,600,-1.0,5);

     sprintf(hist_name,"h_mip_Layer%02d_FH_smeared",i+1);
     hist_mip_Layer_FH_smeared[i] = new TH1F(hist_name,hist_name,500,0,100);
     sprintf(hist_name,"h_mip_Layer%02d_zoomed_FH_smeared",i+1);
     hist_mip_Layer_zoomed_FH_smeared[i] = new TH1F(hist_name,hist_name,1000,0,10);


   }
   

   int layer_hit[28];
   double layer_energy[28];


   
   int decade = 0;
   cout<<"Total entry = "<<nentries<<endl;
   Long64_t jentry=0;
   for (jentry=0; jentry<nentries;jentry++) {
     // ==============print number of events done == == == == == == == =                                               
     double progress = 10.0 * jentry / (1.0 * nentries);
     int k = int (progress);
     if (k > decade)
       cout << 10 * k << " %" << endl;
     decade = k;

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      //cout<<"Entry = "<<jentry<<endl;

      for(int i = 0; i < 28; i++) { layer_hit[i] = 0; layer_energy[i] = 0.0;}

      // double EEPassive = 0.0;
      // for(int i = 0; i < hgcPassiveEEEnergy->size(); i++ ) {
      // 	EEPassive += hgcPassiveEEEnergy->at(i);
      // }
      // //cout<<jentry<<" : "<<EEPassive<<endl;
      // if(EEPassive > 1.5) { shower++; continue;}
      
      for (unsigned int icell = 0; icell < simHitCellIdEE->size(); icell++) {
	int subdet, z, layer, wafer, celltype, cellno;
	uint32_t id = simHitCellIdEE->at(icell);
	HGCalTestNumbering::unpackHexagonIndex(id, subdet, z, layer, wafer, celltype, cellno);
	
	
	// cout<<xy.first<<"\t"<<xy.second<<endl;                                                                                                
	HGCalTBCellVertices Thecell;
	std::pair<double,double> xy = position_cell(cellno);
	std::pair<int, int> iuiv = Thecell.GetCellIUIVCoordinates(xy.first,xy.second);;

	std::pair<double, double> XY = position_wafer(wafer);
         double X = XY.first;
         double Y =  XY.second;
         std::pair<int, int> iUiV = Thecell.GetSensorIUIVCoordinates(X, Y);

	
	// cout<<iuiv.first<<"\t"<<iuiv.second<<endl;       
	if (layer == 0) continue;		//no layers with index 0 allowed
	double energy = simHitCellEnEE->at(icell) * 1000000.0;
	
	layer_hit[layer-1]++;
	layer_energy[layer-1] += energy;
	// if(jentry < 10)
	// 	std::cout << id << "  layer: " << layer << "   wafer:  " << wafer << "   cellno:  " << cellno << "  (x,y): (" << xy.first << "," <<  xy.second<<")    (iu,iv):  ("<<iuiv.first<< "," <<iuiv.second<<")"<< "   ,energy: " <<energy<< std::endl;      
	if(layer==2)	hist_2->Fill(energy);
	if(layer==10)   hist_Layer10->Fill(energy);
	if(layer==14)	 { hist_14->Fill(energy); hist_14_mip->Fill(energy/GeVtoMIP_300um);}
	if(layer==28)	hist_28->Fill(energy);
	if(layer <= 26) {
	  hist_keV_Layer[layer-1]->Fill(energy);
	  hist_keV_Layer_zoomed[layer-1]->Fill(energy);
	  hist_mip_Layer[layer-1]->Fill(energy/GeVtoMIP_300um);
	  hist_mip_Layer_zoomed[layer-1]->Fill(energy/GeVtoMIP_300um);
	  hist_mip_Layer_logE[layer-1]->Fill(log10(energy/GeVtoMIP_300um));
	  
	  double smeared_en = (energy/GeVtoMIP_smeared_300um) + randgen->Gaus(0.0, 1.0/6);
	  hist_mip_Layer_smeared[layer-1]->Fill(smeared_en);
	  hist_mip_Layer_zoomed_smeared[layer-1]->Fill(smeared_en);
	  
	  
	}
	if(layer == 27 || layer == 28) {
	  hist_keV_Layer[layer-1]->Fill(energy);
	  hist_keV_Layer_zoomed[layer-1]->Fill(energy);
	  hist_mip_Layer[layer-1]->Fill(energy/GeVtoMIP_200um);
	  hist_mip_Layer_zoomed[layer-1]->Fill(energy/GeVtoMIP_200um);
	  hist_mip_Layer_logE[layer-1]->Fill(log10(energy/GeVtoMIP_200um));
	  
	  double smeared_en = (energy/GeVtoMIP_200um) + randgen->Gaus(0.0, 1.0/6);
	  hist_mip_Layer_smeared[layer-1]->Fill(smeared_en);
	  hist_mip_Layer_zoomed_smeared[layer-1]->Fill(smeared_en);

	}
	//cout<<jentry<<" : "<<layer<< " : "<<iUiV.first<<" : "<<iUiV.second<< " : "<<X << " : " <<Y<<endl;
	
	
      }
      
      //cout<<jentry<<" : "<<simHitCellIdEE->size()<<" : "<<simHitCellIdFH->size()<<endl;
      for (unsigned int icell = 0; icell < simHitCellIdFH->size(); icell++) {
	int subdet, z, layer, wafer, celltype, cellno;
	uint32_t id = simHitCellIdFH->at(icell);
	HGCalTestNumbering::unpackHexagonIndex(id, subdet, z, layer, wafer, celltype, cellno);
	
	
	// cout<<xy.first<<"\t"<<xy.second<<endl;                                                                                                
	HGCalTBCellVertices Thecell;
	std::pair<double,double> xy = position_cell(cellno);
	std::pair<int, int> iuiv = Thecell.GetCellIUIVCoordinates(xy.first,xy.second);;
	// cout<<iuiv.first<<"\t"<<iuiv.second<<endl;       
	//if (layer == 0) continue;     //no layers with index 0 allowed
	double energy = simHitCellEnFH->at(icell) * 1000000.0;
	layer += 28;
	// cout<<layer<<endl;
	std::pair<double, double> XY = position_wafer(wafer);
	double X = XY.first;
	double Y =  XY.second;
	std::pair<int, int> iUiV = Thecell.GetSensorIUIVCoordinates(X, Y);
	
	if(layer-28 > 12 || layer-28 < 0) {
	  cout<<"WHAT?? "<<layer-28<<endl;
	  break;
	}
	if((X == 0.0 && Y == 0.0) || true) {
	  hist_keV_Layer_FH[layer-28-1]->Fill(energy);
	  hist_keV_Layer_zoomed_FH[layer-28-1]->Fill(energy);
	  hist_mip_Layer_FH[layer-28-1]->Fill(energy/GeVtoMIP_300um);
	  hist_mip_Layer_zoomed_FH[layer-28-1]->Fill(energy/GeVtoMIP_300um);
	  hist_mip_Layer_logE_FH[layer-28-1]->Fill(log10(energy/GeVtoMIP_300um));
	  
	  double smeared_en = (energy/GeVtoMIP_smeared_300um) + randgen->Gaus(0.0, 1.0/6);
	  // double smeared_en = (energy/GeVtoMIP_300um) + randgen->Gaus(0.0, 1.0/6);
	  // smeared_en = smeared_en/1.04;
	  hist_mip_Layer_FH_smeared[layer-28-1]->Fill(smeared_en);
	  hist_mip_Layer_zoomed_FH_smeared[layer-28-1]->Fill(smeared_en);
	  
	}
	// if(layer >= 38 || true) {
	//   // cout<<"------------"<<endl;
	//   cout<<jentry<<" : "<<layer<< " : "<<iUiV.first<<" : "<<iUiV.second<< " : "<<X << " : " <<Y<<endl;
	//   // cout<<"------------"<<endl;
	// }
	
	
	
	
	// layer_hit[layer-1]++;
	// layer_energy[layer-1] += energy;
	// if(jentry < 10)
	//    std::cout << id << "  layer: " << layer << "   wafer:  " << wafer << "   cellno:  " << cellno << "  (x,y): (" << xy.first << "," <<  xy.second<<")    (iu,iv):  ("<<iuiv.first<< "," <<iuiv.second<<")"<< "   ,energy: " <<energy<< std::endl;      
	// if(layer==2)   hist_2->Fill(energy);
	// if(layer==14)  hist_14->Fill(energy);
	// if(layer==28)  hist_28->Fill(energy);
	
	
	
      }
      
      
      
      // if(jentry > 10) return;
      for(int i = 0; i < 28; i++) {
      	if(i+1 <= 10) {
	  if(layer_hit[i] == 1) {
	    h_accu_300um->Fill(layer_energy[i]);
	  }
      	}
      	if(i+1 == 27 || i+1 == 28) {
	  if(layer_hit[i] == 1) {
	    h_accu_200um->Fill(layer_energy[i]);
	  }
      	}
      }
      
   }
   
   // hist_2->Write();
   // hist_14->Write();
   // hist_28->Write();
   // h_accu_300um->Write();
   // h_accu_200um->Write();
   
   file->cd();
   file->Write();
   
   file->Close();
   cout<<"Got out "<<jentry<<endl;
   cout<<"Showering in EE : "<<((double)shower * 100.0)/nentries<<endl;
}
