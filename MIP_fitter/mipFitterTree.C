#define mipFitterTree_cxx
#include "mipFitterTree.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TLatex.h>
#include <sstream>
#include <fstream>
#include <TGraphAsymmErrors.h>

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooFFTConvPdf.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "RooCurve.h"
#include "RooFitResult.h"
using namespace RooFit;

ofstream myfile;
TFile* _file0 = TFile::Open("/home/work/spandey/public/MIP_calibration/samples/v10/muon_v10.root");

void Loop(unsigned int layer,unsigned int skiroc,unsigned int channel)
{
  //if (fChain == 0) return;

  //TFile* _file0 = TFile::Open("/home/work/spandey/public/MIP_calibration/samples/v10/132_162/muon_132_162_v10.root");

  TTree* tree = (TTree*)_file0->Get("rechitntupler/hits");


  vector<float> *rechit_amplitudeHigh_ = nullptr;
  vector<unsigned int> *rechit_layer_ = nullptr;
  vector<unsigned int> *rechit_chip_ = nullptr;
  vector<unsigned int> *rechit_channel_ = nullptr;


  TBranch* b_rechit_amplitudeHigh;
  TBranch* b_rechit_layer;
  TBranch* b_rechit_chip;
  TBranch* b_rechit_channel;

  tree->SetMakeClass(1);

  tree->SetBranchAddress("rechit_amplitudeHigh", &rechit_amplitudeHigh_, &b_rechit_amplitudeHigh);
  tree->SetBranchAddress("rechit_layer", &rechit_layer_, &b_rechit_layer);
  tree->SetBranchAddress("rechit_chip", &rechit_chip_, &b_rechit_chip);
  tree->SetBranchAddress("rechit_channel", &rechit_channel_, &b_rechit_channel);


  // for( unsigned entry = 0; entry < std::min((unsigned)50000000,(unsigned)(tree->GetEntriesFast()) ); entry++){
  // }
  Long64_t nentries = (Long64_t)(tree->GetEntriesFast());

   cout<<nentries<<endl;
   // tree->GetEntry(0);
   cout<<"YOLO"<<endl;

   const int max_ = 400.0;
   const int tf1_min = 20.0;
   const int tf1_max = 60.0;
   bool print_draw = true;
   int bin_ = max_/2;
   std::ostringstream os(std::ostringstream::ate);
   os.str("");
   os<<"Board="<<layer<<" Chip="<<skiroc<<" Channel="<<channel;
   //TH1F* h=new TH1F(os.str().c_str(),os.str().c_str(),bin_,0,max_);
   TH1F* h=new TH1F(os.str().c_str(),os.str().c_str(),bin_,0,max_);
   

   //Long64_t nbytes = 0, nb = 0;
   cout<<nentries<<endl;
   // tree->GetEntry(0);
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     //Long64_t ientry = LoadTree(jentry);
     tree->GetEntry(jentry);
     // if (ientry < 0) break;
     // nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     //if( boardID==layer && skirocID==skiroc && channelID==channel && HighGainADC<200 && HighGainADC>20){
     //if( boardID==layer && skirocID==skiroc && channelID==channel && HighGainADC<max_ && HighGainADC>20){
     
     // cout<<"ADC:layer:chip:channel"<<endl
     // 	 <<rechit_amplitudeHigh_->size()<<":"
     // 	 <<rechit_layer_->size()<<":"
     // 	 <<rechit_chip_->size()<<":"
     // 	 <<rechit_channel_->size()<<endl;
     // h->Fill(1);
     for(unsigned int i = 0; i < rechit_layer_->size(); i++) {
       //if( boardID==layer && skirocID==skiroc && channelID==channel)   h->Fill(HighGainADC);
       if( layer==(rechit_layer_->at(i)-1) && skiroc==(rechit_chip_->at(i)) && channel==(rechit_channel_->at(i)))
	 h->Fill(rechit_amplitudeHigh_->at(i));
     }
   }
   
   // h->Draw();
   // getchar();
   
   TCanvas *cc;
   if (print_draw) {
     setTDRStyle();
     cc=new TCanvas();
     cc->SetWindowSize(650,600);
   }


   RooRealVar x("x","ADC count (high gain)",20,max_) ;
   RooArgList l = RooArgList(x);

   RooRealVar mean0("mean","Smearing mean (fixed at zero, no bias)",0.) ;
   RooRealVar sigma0("SigmaG","Width of smearing gaussian",5,0,100) ;
   RooGaussian mipG0("mipG","Smearing gaussian",x,mean0,sigma0) ;  

   RooRealVar ml0("MPV","Landau MPV",45.,10,70) ;
   RooRealVar sl0("SigmaL","Landau sigma",5,0.,30) ;
   RooLandau landau1("l1","Single MIP distribution",x,ml0,sl0) ;
  
  // If X~Landau(µ1, c1) and Y~Landau(µ2, c2), then X+Y~Landau(µ1+µ2, c1+c2)
  // https://en.wikipedia.org/wiki/Landau_distribution#Properties
   RooFormulaVar ml1("MPV1","Laudau MPV for 2-MIP peak","2*@0",ml0) ;
   RooFormulaVar sl1("SigmaL1","Laudau sigma for 2-MIP peak","2*@0",sl0) ;
   RooLandau landau2("l2","Two-MIP distribution",x,ml1,sl1) ;

   RooFFTConvPdf l1xg("l1xg","landau1 (X) gauss",x,landau1,mipG0) ;
   RooFFTConvPdf l2xg("l2xg","landau2 (X) gauss",x,landau2,mipG0) ;

   RooRealVar p1("p1","coeff #0", 0); // zero slope === constant
   //RooRealVar p1("p1","coeff #0",0.01,0.,0.1); // zero slope === constant
   //RooRealVar p2("p2","coeff1",0.1,0.,1.0); // Non zero slope === constant
   //RooRealVar p1("p1","coeff #1", ,); // zero slope === constant
   RooPolynomial grass("plot","bkgd pdf", x, RooArgList(p1));
   //RooPolynomial grass("plot","bkgd pdf", x, RooArgList(p1,p2));
   RooGaussian noise("noise","Noise gaussian",x,mean0,sigma0) ;  

   // RooRealVar n1mip("n1mip","n1mip",1e3,0.,1.e6) ;
   // RooRealVar n2mip("n2mip","n2mip",1e1,0.,1.e6);
   // RooRealVar nbkg("nbkg","nbkg",1e1,0.,1.e6);
   // RooRealVar nnoise("nnoise","nbkg",1e1,0.,1.e6);

   RooRealVar n1mip("n1mip","n1mip",1e1,0.,1.e6) ;
   RooRealVar n2mip("n2mip","n2mip",1e1,0.,1.e6);
   RooRealVar nbkg("nbkg","nbkg",1e1,0.,1.e6);
   RooRealVar nnoise("nnoise","nbkg",1e1,0.,1.e6);

   RooAddPdf model("model","(two landau(X)gaus) + noise + bkg",RooArgList(l1xg,l2xg,grass,noise),RooArgList(n1mip,n2mip,nbkg,nnoise));
   // RooAddPdf model("model","(two landau(X)gaus)+ noise",RooArgList(l1xg,l2xg,noise),RooArgList(n1mip,n2mip,nnoise));

   
   RooDataHist data("data", h->GetTitle(), l, h);
   RooPlot* xframe = x.frame(Title(" ")) ;
   xframe->SetYTitle("Events / ADC count");
   xframe->SetXTitle("Signal amplitude (ADC counts)");
   xframe->GetYaxis()->SetTitleOffset(1.2) ; 
   
   data.plotOn(xframe) ;
   RooFitResult* rfr = model.fitTo(data,PrintLevel(-1),Save());

   model.plotOn(xframe);

   TGraphAsymmErrors* gr = xframe->getHist();

   model.paramOn(xframe);
   //grass.plotOn(xframe,RooFit.Name("grass"));
   //grass.plotOn(xframe);
   
   if (print_draw) {
     rfr->Print("V");
     xframe->Draw() ;
   }

   TF1 *f=model.asTF( l );

   std::cout << "ml.getValV() = " << ml0.getValV() << "\t"
	     << "convMax = " << f->GetMaximumX(tf1_min,tf1_max) << "\t"
	     << "integrate = " << f->Integral(-20,60) << "\t"
	     << "neventatmax = " << f->Eval(ml0.getValV()) << std::endl;
	


   std::cout << "Total number of entries " << h->GetEntries() << std::endl;
   std::cout << "chi2 = " << xframe->chiSquare() << std::endl;
   std::cout << "value of constant, at 200.0 = " << f->Eval(200.0) <<" ,at "<<max_<<":"<< f->Eval(max_)<<std::endl;
   if (print_draw) {
     TLatex *tex=new TLatex();
     tex->SetTextColor(kBlack);
     tex->SetTextSize(0.041);
     tex->DrawLatexNDC(0.15,0.96,os.str().c_str());
     os.str("");
     //os << setprecision(3) << "MIP = " << f->GetMaximumX() << " #pm " << setprecision(1) << ml0.getError() << " ADC counts";
     os << setprecision(3) << "MIP = " << f->GetMaximumX(tf1_min,tf1_max) << " #pm " << setprecision(1) << ml0.getError() << " ADC counts";
     //tex->DrawLatexNDC(0.44,0.85,os.str().c_str());
     tex->DrawLatexNDC(0.44,0.45,os.str().c_str());

     os.str("");
     os << setprecision(3) << "chi2/ndf = " << xframe->chiSquare();
     tex->DrawLatexNDC(0.44,0.40,os.str().c_str());

     cc->Update();     
     char* canvas_name = new char[20];
     sprintf(canvas_name,"%d_%d_%d.gif",layer,skiroc,channel);
     //cc->SaveAs(os.str().c_str());
     cc->SaveAs(canvas_name);
   }

   double const_val = nbkg.getValV()/h->GetEntries();
   double err = (nbkg.getError()/h->GetEntries());
   TF1* g1 = grass.asTF( l );
   cout<<"######  grass at 10 & 100 = "<<g1->Eval(10)<<" "<<g1->Eval(10)<<", Evalulated sep="<<const_val<<" ,error="<<err<<endl;


   cout<<"*************"<<endl;
   cout<<"layer no. = "<< layer << ", channel_ID="<<(skiroc*1000 + channel)<<endl;
   cout <<  "MIP = " << f->GetMaximumX() << " #pm " <<  ml0.getError() << " ADC counts"<<endl;
   cout<<"*************"<<endl;
   float ADC_MIP = f->GetMaximumX(tf1_min,tf1_max);
   float chi2_ndf = xframe->chiSquare();
   int en_chan = (skiroc*1000+channel);
   float mpv_val = ml0.getValV();
   char *line = new char[50];
   sprintf(line,"%d %d %d %d %.2f %.2f %.4f %.4f %.2f\n",layer,skiroc,channel,en_chan,ADC_MIP,chi2_ndf,const_val,err,mpv_val);
   cout<<line<<endl;
   myfile << line;
   tree->Delete();
}


void runAllChannel(unsigned int layer,unsigned int skiroc)
{
  // mipFitterTree m;
  // char *f_name = new char[30];
  // sprintf(f_name,"calib_file_skiroc_%d.txt",skiroc);
  // myfile.open (f_name);

  for(int i=0; i<32; i++){
    Loop(layer,skiroc,2*i);
  }
  //myfile.close();
}


void runAllSkiROC(unsigned int layer)
{
   char *f_name = new char[30];
   sprintf(f_name,"calib_file_layer_%d.txt",layer);
   myfile.open (f_name);
   for(int i=0; i<4; i++){
     runAllChannel(layer,i);
   }
   myfile.close();
}


void runAllLayers(unsigned int layer_start, unsigned int layer_end)
{
  for(unsigned int i=(layer_start); i<=layer_end; i++) {
     runAllSkiROC(i);
   }

}


