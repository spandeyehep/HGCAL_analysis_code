#ifndef ANALYZEHGCMuons_H
#define ANALYZEHGCMuons_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "HGCNtupleVariables.h"
#include "TH1F.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class AnalyzeHGCMuons : public HGCNtupleVariables{

 public:
  AnalyzeHGCMuons(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~AnalyzeHGCMuons();
  Bool_t   FillChain(TChain *chain, TChain *chain2, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *);
  void     BookHistogram(const char *);
  
  TFile *oFile;
  TDirectory *d_Layer1;
  TH1F *h_ADChg[128];
};
#endif

#ifdef ANALYZEHGCMuons_cxx

void AnalyzeHGCMuons::BookHistogram(const char *outFileName) {

//  char hname[200], htit[200];
//  double xlow = 0.0,  xhigh = 2000.0;
//  int nbins = 2000;

  oFile = new TFile(outFileName, "recreate");
  d_Layer1 = oFile->mkdir("Layer1");
  char hname[200];
  d_Layer1-> cd();
  for(int ii=0; ii<128; ii++) {
    sprintf(hname, "ADC_HG_%d", ii+1);
    h_ADChg[ii] = new TH1F(hname, hname, 100, 0, 400);
  }
}


AnalyzeHGCMuons::AnalyzeHGCMuons(const TString &inputFileList, const char *outFileName, const char* dataset) {

  TChain *tree = new TChain("rechitntupler/hits");
  TChain *tree2 = new TChain("trackimpactntupler/impactPoints");

  if( ! FillChain(tree, tree2, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  HGCNtupleVariables::Init(tree, tree2);

  BookHistogram(outFileName);
  
}

Bool_t AnalyzeHGCMuons::FillChain(TChain *chain, TChain *chain2, const TString &inputFileList) {

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    //std::cout << "Adding tree from " << buffer.c_str() << std::endl;                                                              
    chain->Add(buffer.c_str());
    chain2->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in chain  : " << chain->GetEntries() << std::endl;
  std::cout << "No. of Entries in chain2 : " << chain2->GetEntries() << std::endl;
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

  if (centry==centry2)
    return centry;
  else return -1;
}

AnalyzeHGCMuons::~AnalyzeHGCMuons() { 

  if (!fChain || !fChain2) return;
  delete fChain->GetCurrentFile();
  delete fChain2->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif
