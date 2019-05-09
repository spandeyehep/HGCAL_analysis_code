#define mipFitterTree_cxx
#include "noiseFitter.h"

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
#include <iomanip>
// #include "helper.h"
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

TFile* _file0 = TFile::Open("./muon_rereco_v12_719/muon_v12_rereco719_TS3_withLG.root");

ofstream myfile_all;
void Loop(int layer,int chip,int channel, int pos_)
{
  

   TCanvas *cc;
   setTDRStyle();

   cc=new TCanvas();
   cc->SetWindowSize(650,600);

   bool saveCanvas_ = true;
   // if (print_draw) {
   //   setTDRStyle();
   // }

   char* hname = new char[500];
   char *det = new char[20];
   // sprintf(hanme,"ADC_distributions/EE%d/h_TS3HG_EE_L1_P0_chip3_chan40");
   if(pos_ == 0) {
     sprintf(hname,"ADC_distributions/EE_%d/h_TS3HG_EE_L%d_P%d_chip%d_chan%d",layer,layer,pos_,chip,channel);
     sprintf(det,"EE");
   }
   else {
     sprintf(hname,"ADC_distributions/FH_L%d_P%d/h_TS3HG_FH_L%d_P%d_chip%d_chan%d",layer,pos_,layer,pos_,chip,channel);
     sprintf(det,"FH");
   }
   TH1F* h = (TH1F*)_file0->Get(hname);

   if(!h) {
     cout<<"Could Not Load hist = "<<hname<<endl;
     return;
   }
   // return;
   // h->GetYaxis()->SetRangeUser(10,20);
   h->GetXaxis()->SetRangeUser(-25,50);

   h->SetTitle("yolo");
   
   RooRealVar x("x","ADC count (high gain)",-10.0,7.0) ;

   RooArgList l = RooArgList(x);

   RooRealVar mean0("mean","Smearing mean (fixed at zero, no bias)",0.,-0.5,0.5) ;
   RooRealVar sigma0("SigmaG","Width of smearing gaussian",5.0,0.0,100.0) ;
   RooGaussian mipG0("smearing_gaus","Smearing gaussian",x,mean0,sigma0) ;  

   RooDataHist data("data", h->GetTitle(), l, h);
   RooPlot* xframe = x.frame(Name("xframe"),Title("YOLO ")) ;
   xframe->SetYTitle("Events");
   xframe->SetXTitle("TS3 ADC amplitude");
   xframe->GetYaxis()->SetTitleOffset(1.2) ; 
   xframe->SetTitle("yolo");
   data.plotOn(xframe) ;
   RooFitResult* rfr = mipG0.fitTo(data,PrintLevel(-1),Save());

   
   mipG0.plotOn(xframe);

   mipG0.paramOn(xframe, 
   		 Layout(0.14, 0.55, 0.95), 
   		 Format("NELU", AutoPrecision(1)));

   rfr->Print("V");

   xframe->Draw();
   std::ostringstream os(std::ostringstream::ate);   
   os.str("");
   os<<"Layer="<<layer<<" Chip="<<chip<<" Channel="<<channel;

   TLatex *tex=new TLatex();
   tex->SetTextColor(kBlack);
   tex->SetTextSize(0.041);
   cout<< "os = "<<os.str()<<endl;
   tex->DrawLatexNDC(0.15,0.96,os.str().c_str());
   gPad->SetGridx();
   gPad->SetGridy();
   cc->Update();     


   if(saveCanvas_) {
     char* canvas_name = new char[200];
     if(layer < 10)
       sprintf(canvas_name,"plots/v12LG/%s_layer_0%d_P%d/layer%d_P%d_chip%d_channel%d.gif",det,layer,pos_,layer,pos_,chip,channel);
     else 
       sprintf(canvas_name,"plots/v12LG/%s_layer_%d_P%d/layer%d_P%d_chip%d_channel%d.gif",det,layer,pos_,layer,pos_,chip,channel);
     cc->SaveAs(canvas_name);
   }

   double mean = mean0.getValV();
   int entries = h->GetEntries();
   double sigma = sigma0.getValV();
   double err_sig = sigma0.getError();

   char* line = new char[1000];
   //FOR EE
   //sprintf(line,"%d %d %d %d %.3f %.3f %.3f\n",layer,chip,channel,entries,mean,sigma,err_sig);

   //FOR FH
   sprintf(line,"%d %d %d %d %d %.3f %.3f %.3f\n",layer,pos_,chip,channel,entries,mean,sigma,err_sig);

   cout<<line<<endl;
   myfile_all << line;


}


void for_all(){
  int layer_start = 1;
  int layer_end = 28;
  myfile_all.open("./Pedestal_noise_EE_LowGain.txt");
  //Layer pos_ Chip channel entries mean sigma error_on_sigma
  // char* line_ = new char[1000];
  // sprintf(line_,"layer ")
  for(int ilayer = layer_start; ilayer <= layer_end; ilayer++){
    for(int chip = 0; chip < 4; chip++){
      for(int channel = 0; channel < 64; channel+=2) {
	Loop(ilayer,chip,channel,0);
	//return;
      }
    }
  }
  myfile_all.close();
}


void for_all_FH(){
  int layer_start = 1;
  int layer_end = 12;
  myfile_all.open("./Pedestal_noise_FH_LowGain.txt");
  //Layer pos_ Chip channel entries mean sigma error_on_sigma
  // char* line_ = new char[1000];
  // sprintf(line_,"layer ")
  for(int ilayer = layer_start; ilayer <= layer_end; ilayer++){
    for(int ipos = 1; ipos <= 7 ; ipos++) {
      for(int chip = 0; chip < 4; chip++){
	for(int channel = 0; channel < 64; channel+=2) {
	  if(ilayer > 9 && ipos != 4) continue;
	  Loop(ilayer,chip,channel,ipos);
	  //return;
	}
	// myfile_all.close();
  	// return;
      }
    }
  }
  myfile_all.close();
}

