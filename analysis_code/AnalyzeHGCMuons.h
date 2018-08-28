#ifndef ANALYZEHGCMuons_H
#define ANALYZEHGCMuons_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "HGCNtupleVariables.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class AnalyzeHGCMuons : public HGCNtupleVariables{

 public:
  AnalyzeHGCMuons(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~AnalyzeHGCMuons();
  Bool_t   FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *);
  void     BookHistogram(const char *);
  void     Alignment_Map_Init();
  
  TFile *oFile;
  TDirectory *d_adc;
  TDirectory *d_Layer[28];
  TDirectory *d_correction;
  TDirectory *d_run[40];
  TDirectory *d_run_layer[40][28];
  TH1F *h_ADChg[28][128];
  TH1F *h_nrechits[28];
  TH1F *h_deltaR_layer[28];
  TH1F *h_deltaR_min_layer[28];
  TH1F *h_deltaR_test;
  TH1F *h_deltaR_min;
  TH1F *h_trackX_chi2;
  TH1F *h_trackY_chi2;
  TH1F *h_tablePositionY;
  TH2F *h_min_dR_nrechit_layer1;
  TH2F *h_rechitXY_layer[28];
  TH2F *h_trackXY_layer[28];
  TH2F *h_trackX_rechitX_layer[28];
  TH2F *h_trackY_rechitY_layer[28];
  TH2F *h_dXY_layer[28];
  TH2F *h_dX_run_132_161_layer[28];
  TH2F *h_dY_run_132_161_layer[28];
  TH2F *h_dX_run_206_225_layer[28];
  TH2F *h_dY_run_206_225_layer[28];
  TH2F *h_dX_run_387_392_layer[28];
  TH2F *h_dY_run_387_392_layer[28];
  TH1F *h_trackX_layer[28];
  TH1F *h_trackY_layer[28];

  TH2F *h_dX_corrected_run_132_161_layer[28];
  TH2F *h_dY_corrected_run_132_161_layer[28];
  TH2F *h_dX_corrected_run_206_225_layer[28];
  TH2F *h_dY_corrected_run_206_225_layer[28];
  TH2F *h_dX_corrected_run_387_392_layer[28];
  TH2F *h_dY_corrected_run_387_392_layer[28];


  //int runlist[40];
  TH1F *h_dx_run[40][28];
  TH1F *h_dy_run[40][28];

  TH1F *h_dx_corr_run[40][28];
  TH1F *h_dy_corr_run[40][28];

  
};
#endif

#ifdef ANALYZEHGCMuons_cxx

void AnalyzeHGCMuons::BookHistogram(const char *outFileName) {

//  char hname[200], htit[200];
//  double xlow = 0.0,  xhigh = 2000.0;
//  int nbins = 2000;

  char* hname = new char[200];
  char* htitle = new char[200];
  char* dir_name = new char[200];
  int runlist[40] = { 132,136,137,138,140,142,143,145,146,147,148,149,150,151,152,153,154,156,157,159,160,161,206,207,210,212,213,214,216,217,219,223,224,225,387,388,389,390,391,392};
  oFile = new TFile(outFileName, "recreate");

  h_deltaR_test = new TH1F("h_deltaR_test","deltaR layer1 test",160,-1.0,15);

  //h_min_dR_nrechit_layer1 = new TH2F("h_min_dR_nrechit_layer1","Min dR vs nRechits layer1",50,0,50,160,-1.0,15);
  h_min_dR_nrechit_layer1 = new TH2F("h_min_dR_nrechit_layer1","Min dR vs nRechits layer1",160,-1.0,15,50,0,50);

  h_min_dR_nrechit_layer1->GetXaxis()->SetTitle("minimum dR");
  h_min_dR_nrechit_layer1->GetYaxis()->SetTitle("# of rechits");

  h_trackX_chi2 = new TH1F("h_trackX_chi2","track_X chi2",210,-1.0,20);
  h_trackY_chi2 = new TH1F("h_trackY_chi2","track_Y chi2",210,-1.0,20);
  h_tablePositionY = new TH1F("h_tablePositionY","metaData tablePositionY",500,-30.0,20.0);

  d_adc = oFile->mkdir("ADC_distributions");
  d_adc->cd();
  for(int i_layer = 0; i_layer < 28; i_layer++) {
    int n = 0;
    sprintf(dir_name,"Layer_%d",i_layer);
    //d_Layer[i_layer] = oFile->mkdir(dir_name);
    d_Layer[i_layer] = d_adc->mkdir(dir_name);

    d_Layer[i_layer]-> cd();
    sprintf(hname,"h_nrechits_layer%d",i_layer+1);
    h_nrechits[i_layer] = new TH1F(hname,hname,60,0,60);

    sprintf(hname,"h_deltaR_layer%d",i_layer+1);
    sprintf(htitle,"deltaR layer%d",i_layer+1);
    h_deltaR_layer[i_layer] = new TH1F(hname,htitle,160,-1.0,15);

    sprintf(hname,"h_deltaR_min_layer%d",i_layer+1);
    sprintf(htitle,"min(deltaR) layer%d",i_layer+1);
    h_deltaR_min_layer[i_layer] = new TH1F(hname,htitle,160,-1.0,15);

    sprintf(hname,"h_rechitXY_layer%d",i_layer+1);
    sprintf(htitle,"rechit_x vs rechit_y layer%d",i_layer+1);
    h_rechitXY_layer[i_layer] = new TH2F(hname,htitle,16,-8.0,8.0,16,-8.0,8.0);
    h_rechitXY_layer[i_layer]->GetXaxis()->SetTitle("rechit_x");
    h_rechitXY_layer[i_layer]->GetYaxis()->SetTitle("rechit_y");

    sprintf(hname,"h_trackXY_layer%d",i_layer+1);
    sprintf(htitle,"track_x vs track_y layer%d",i_layer+1);
    h_trackXY_layer[i_layer] = new TH2F(hname,htitle,16,-8.0,8.0,16,-8.0,8.0);
    h_trackXY_layer[i_layer]->GetXaxis()->SetTitle("track_x");
    h_trackXY_layer[i_layer]->GetYaxis()->SetTitle("track_y");



    sprintf(hname,"h_trackX_rechitX_layer%d",i_layer+1);
    sprintf(htitle,"track_x vs rechit_x layer%d",i_layer+1);
    h_trackX_rechitX_layer[i_layer] = new TH2F(hname,htitle,16,-8.0,8.0,16,-8.0,8.0);
    h_trackX_rechitX_layer[i_layer]->GetXaxis()->SetTitle("track_x");
    h_trackX_rechitX_layer[i_layer]->GetYaxis()->SetTitle("rechit_x");


    sprintf(hname,"h_trackY_rechitY_layer%d",i_layer+1);
    sprintf(htitle,"track_y vs rechit_y layer%d",i_layer+1);
    h_trackY_rechitY_layer[i_layer] = new TH2F(hname,htitle,16,-8.0,8.0,16,-8.0,8.0);
    h_trackY_rechitY_layer[i_layer]->GetXaxis()->SetTitle("track_y");
    h_trackY_rechitY_layer[i_layer]->GetYaxis()->SetTitle("rechit_y");



    sprintf(hname,"h_dXY_layer%d",i_layer+1);
    sprintf(htitle,"dX vs dY layer%d",i_layer+1);
    h_dXY_layer[i_layer] = new TH2F(hname,htitle,16,-8.0,8.0,16,-8.0,8.0);
    h_dXY_layer[i_layer]->GetXaxis()->SetTitle("dX");
    h_dXY_layer[i_layer]->GetYaxis()->SetTitle("dY");



    sprintf(hname,"h_dX_run_132_161_layer%d",i_layer+1);
    sprintf(htitle,"dX vs run_132_161 layer%d",i_layer+1);
    h_dX_run_132_161_layer[i_layer] = new TH2F(hname,htitle,33,130,163,16,-8.0,8.0);
    h_dX_run_132_161_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dX_run_132_161_layer[i_layer]->GetYaxis()->SetTitle("dX");

    sprintf(hname,"h_dY_run_132_161_layer%d",i_layer+1);
    sprintf(htitle,"dY vs run_132_161 layer%d",i_layer+1);
    h_dY_run_132_161_layer[i_layer] = new TH2F(hname,htitle,33,130,163,16,-8.0,8.0);
    h_dY_run_132_161_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dY_run_132_161_layer[i_layer]->GetYaxis()->SetTitle("dY");


    //////Corrected 2D histo
    sprintf(hname,"h_dX_corrected_run_132_161_layer%d",i_layer+1);
    sprintf(htitle,"dX vs run_132_161 layer%d",i_layer+1);
    h_dX_corrected_run_132_161_layer[i_layer] = new TH2F(hname,htitle,33,130,163,16,-8.0,8.0);
    h_dX_corrected_run_132_161_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dX_corrected_run_132_161_layer[i_layer]->GetYaxis()->SetTitle("dX");

    sprintf(hname,"h_dY_corrected_run_132_161_layer%d",i_layer+1);
    sprintf(htitle,"dY vs run_132_161 layer%d",i_layer+1);
    h_dY_corrected_run_132_161_layer[i_layer] = new TH2F(hname,htitle,33,130,163,16,-8.0,8.0);
    h_dY_corrected_run_132_161_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dY_corrected_run_132_161_layer[i_layer]->GetYaxis()->SetTitle("dY");
    /////////

    sprintf(hname,"h_dX_run_206_225_layer%d",i_layer+1);
    sprintf(htitle,"dX vs run_206_225 layer%d",i_layer+1);
    h_dX_run_206_225_layer[i_layer] = new TH2F(hname,htitle,22,205,227,32,-8.0,8.0);
    h_dX_run_206_225_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dX_run_206_225_layer[i_layer]->GetYaxis()->SetTitle("dX");

    sprintf(hname,"h_dY_run_206_225_layer%d",i_layer+1);
    sprintf(htitle,"dY vs run_206_225 layer%d",i_layer+1);
    h_dY_run_206_225_layer[i_layer] = new TH2F(hname,htitle,22,205,227,32,-8.0,8.0);
    h_dY_run_206_225_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dY_run_206_225_layer[i_layer]->GetYaxis()->SetTitle("dY");


    //////Corrected 2D histo
    sprintf(hname,"h_dX_corrected_run_206_225_layer%d",i_layer+1);
    sprintf(htitle,"dX vs run_206_225 layer%d",i_layer+1);
    h_dX_corrected_run_206_225_layer[i_layer] = new TH2F(hname,htitle,22,205,227,32,-8.0,8.0);
    h_dX_corrected_run_206_225_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dX_corrected_run_206_225_layer[i_layer]->GetYaxis()->SetTitle("dX");

    sprintf(hname,"h_dY_corrected_run_206_225_layer%d",i_layer+1);
    sprintf(htitle,"dY vs run_206_225 layer%d",i_layer+1);
    h_dY_corrected_run_206_225_layer[i_layer] = new TH2F(hname,htitle,22,205,227,32,-8.0,8.0);
    h_dY_corrected_run_206_225_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dY_corrected_run_206_225_layer[i_layer]->GetYaxis()->SetTitle("dY");
    ///////

    sprintf(hname,"h_dX_run_387_392_layer%d",i_layer+1);
    sprintf(htitle,"dX vs run_387_392 layer%d",i_layer+1);
    h_dX_run_387_392_layer[i_layer] = new TH2F(hname,htitle,8,386,394,32,-8.0,8.0);
    h_dX_run_387_392_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dX_run_387_392_layer[i_layer]->GetYaxis()->SetTitle("dX");

    sprintf(hname,"h_dY_run_387_392_layer%d",i_layer+1);
    sprintf(htitle,"d_y vs run_387_392 layer%d",i_layer+1);
    h_dY_run_387_392_layer[i_layer] = new TH2F(hname,htitle,8,386,394,32,-8.0,8.0);
    h_dY_run_387_392_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dY_run_387_392_layer[i_layer]->GetYaxis()->SetTitle("dY");


    //////Corrected 2D histo
    sprintf(hname,"h_dX_corrected_run_387_392_layer%d",i_layer+1);
    sprintf(htitle,"dX vs run_387_392 layer%d",i_layer+1);
    h_dX_corrected_run_387_392_layer[i_layer] = new TH2F(hname,htitle,8,386,394,32,-8.0,8.0);
    h_dX_corrected_run_387_392_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dX_corrected_run_387_392_layer[i_layer]->GetYaxis()->SetTitle("dX");

    sprintf(hname,"h_dY_corrected_run_387_392_layer%d",i_layer+1);
    sprintf(htitle,"dY vs run_387_392 layer%d",i_layer+1);
    h_dY_corrected_run_387_392_layer[i_layer] = new TH2F(hname,htitle,8,386,394,32,-8.0,8.0);
    h_dY_corrected_run_387_392_layer[i_layer]->GetXaxis()->SetTitle("run");
    h_dY_corrected_run_387_392_layer[i_layer]->GetYaxis()->SetTitle("dY");
    /////////



    sprintf(hname,"h_trackX_layer%d",i_layer+1);
    sprintf(htitle,"track_x layer%d",i_layer+1);
    h_trackX_layer[i_layer] = new TH1F(hname,htitle,160,-8.0,8.0);

    sprintf(hname,"h_trackY_layer%d",i_layer+1);
    sprintf(htitle,"track_y layer%d",i_layer+1);
    h_trackY_layer[i_layer] = new TH1F(hname,htitle,160,-8.0,8.0);


    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 64; j+=2,n++) {
	sprintf(hname,"%d_%d_%d",i_layer,i,j);
	h_ADChg[i_layer][n] = new TH1F(hname,hname,200,0,400);
	//cout<<hname<<endl;
      }
    }
  }

  d_correction = oFile->mkdir("dx_dy_run");
  d_correction->cd();
  for(int i = 0; i < 40; i++) {
    sprintf(dir_name,"run_%d",runlist[i]);
    d_run[i] = d_correction->mkdir(dir_name);
    d_run[i]->cd();
    for(int j = 0; j < 28; j++) {
      sprintf(dir_name,"Layer%d",j);
      d_run_layer[i][j] = d_run[i]->mkdir(dir_name);
      d_run_layer[i][j]->cd();
      sprintf(hname,"h_dx_run_%d_%d",runlist[i],j);
      h_dx_run[i][j] = new TH1F(hname,hname,96,-12.0,12.0);

      sprintf(hname,"h_dy_run_%d_%d",runlist[i],j);
      h_dy_run[i][j] = new TH1F(hname,hname,96,-12.0,12.0);

      //corrected
      sprintf(hname,"h_dx_corr_run_%d_%d",runlist[i],j);
      h_dx_corr_run[i][j] = new TH1F(hname,hname,96,-12.0,12.0);

      sprintf(hname,"h_dy_corr_run_%d_%d",runlist[i],j);
      h_dy_corr_run[i][j] = new TH1F(hname,hname,96,-12.0,12.0);
      ////////

    }
  }
}

void AnalyzeHGCMuons::Alignment_Map_Init() {
  char* f_name = new char[200];
  sprintf(f_name,"alignment_27_aug.txt");
  std::ifstream in(f_name);
  std::pair<int,int> run_layer;
  std::pair<float,float> dx_dy;
  std::pair<std::pair<int,int>, std::pair<float,float> > temp;
  int run,layer;
  float dx,dy;
  while(in>>run>>layer>>dx>>dy) {
    run_layer = std::make_pair(run,layer);
    dx_dy = std::make_pair(dx,dy);
    temp = std::make_pair(run_layer,dx_dy);
    align_map.insert(temp);
  }
}


AnalyzeHGCMuons::AnalyzeHGCMuons(const TString &inputFileList, const char *outFileName, const char* dataset) {

  TChain *tree = new TChain("rechitntupler/hits");
  TChain *tree2 = new TChain("trackimpactntupler/impactPoints");
  TChain *tree3 = new TChain("metadatantupler/meta");

  if( ! FillChain(tree, tree2, tree3, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  HGCNtupleVariables::Init(tree, tree2, tree3);

  BookHistogram(outFileName);

  Alignment_Map_Init();
  
}

Bool_t AnalyzeHGCMuons::FillChain(TChain *chain, TChain *chain2, TChain *chain3, const TString &inputFileList) {

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;// = (std::string)inputFileList;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  /* chain->Add(buffer.c_str()); */
  /* chain2->Add(buffer.c_str()); */
  /* chain3->Add(buffer.c_str()); */

  if(!chain || !chain2 || !chain3) {
    cout<<"Could NOT add chain for "<<buffer.c_str()<<"\n Exiting now!!"<<endl;
    return kFALSE;
  }
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    std::cout << "Adding tree from " << buffer.c_str() << std::endl;
    chain->Add(buffer.c_str());
    chain2->Add(buffer.c_str());
    chain3->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in rechitNtuple  : " << chain->GetEntries() << std::endl;
  std::cout << "No. of Entries in trackNtuple : " << chain2->GetEntries() << std::endl;
  std::cout << "No. of Entries in metadataNtuple : " << chain3->GetEntries() << std::endl;
  return kTRUE;
}

Long64_t AnalyzeHGCMuons::LoadTree(Long64_t entry) {
  // Set the environment to read one entry                                                                                          
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }

  if (!fChain2) return -5;
  Long64_t centry2 = fChain2->LoadTree(entry);
  if (centry2 < 0) return centry2;
  if (!fChain2->InheritsFrom(TChain::Class()))  return centry2;
  TChain *chain2 = (TChain*)fChain2;
  if (chain2->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }

  if (!fChain3) return -5;
  Long64_t centry3 = fChain3->LoadTree(entry);
  if (centry3 < 0) return centry3;
  if (!fChain3->InheritsFrom(TChain::Class()))  return centry3;
  TChain *chain3 = (TChain*)fChain3;
  if (chain3->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }

  if (centry==centry2 && centry==centry3)
    return centry;
  else return -1;
}

AnalyzeHGCMuons::~AnalyzeHGCMuons() { 

  if (!fChain || !fChain2 || !fChain3) return;
  delete fChain->GetCurrentFile();
  delete fChain2->GetCurrentFile();
  delete fChain3->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif
