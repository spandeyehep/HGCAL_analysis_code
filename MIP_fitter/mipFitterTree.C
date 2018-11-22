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
#include <iomanip>
#include "helper.h"
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
ofstream myfile_all;
// TFile* _file0 = TFile::Open("Muon_200GeV_config3_v1.root");
TFile* _file0 = TFile::Open("Muon_200GeV_config2_tracking_v2.root");
// TFile* _file0 = TFile::Open("/home/work/spandey/public/MIP_calibration/CMSSW_8_0_1/src/v11/alignment_inclusion/track_matching_132_161_v11.root");

//int module_ID[] = {78,90,89,88,77,85,84,32,69,79,76,83,70,73,86,87,82,72,67,65,35,}
//void Loop(const char *part, unsigned int layer,unsigned int skiroc,unsigned int channel,bool track=true)
void Loop(const char *part, unsigned int layer, unsigned int position,unsigned int skiroc,unsigned int channel, unsigned int module)
{
  
  // part = EE/FH
  // const int max_ = 400.0;
  const int max_ = 200.0;
  const int tf1_min = 20.0;
  const int tf1_max = 60.0;
  // const int tf1_min = 20.0;
  // const int tf1_max = 400.0;

  bool print_draw = true;
  int bin_ = max_/2;
  std::ostringstream os(std::ostringstream::ate);
  os.str("");
  // os<<"Board="<<layer<<" Chip="<<skiroc<<" Channel="<<channel;
  os<<"Module="<<module<<" Chip="<<skiroc<<" Channel="<<channel;

  char* hname = new char[200];
  //h_adcHG_EE_L1_P0_chip3_chan18
  if(!strcmp(part,"EE")) {
    // sprintf(hname,"ADC_distributions/%s_%d/h_adcHG_EE_L%d_P0_chip%d_chan%d",part,layer,layer,skiroc,channel);
    sprintf(hname,"ADC_distributions/%s_%d/h_adcHG_EE_L%d_P0_chip%d_chan%d_track",part,layer,layer,skiroc,channel);
  }
  else if(!strcmp(part,"FH")) {
    sprintf(hname,"ADC_distributions/%s_L%d_P%d/h_adcHG_FH_L%d_P%d_chip%d_chan%d",part,layer,position,layer,position,skiroc,channel);
  }
  //sprintf(hname,"ADC_distributions/%s_%d/h_adcHG_EE_L%d_P0_chip%d_chan%d",part,layer,layer,skiroc,channel);

  // if(track)
  //   sprintf(hname,"ADC_distributions/Layer_%d/%d_%d_%d_track",layer,layer,skiroc,channel);
  // else
  //   sprintf(hname,"ADC_distributions/Layer_%d/%d_%d_%d",layer,layer,skiroc,channel);

  //cout<<"Taking histogram -> "<<hname<<endl;
  TH1F* h = (TH1F*)_file0->Get(hname);
  if(!h) {
    cout<<"Error: Could NOT load histogram = "<<hname<<endl;
    return;
  }
  else {
    cout<<"INFO: Taking histogram = "<<hname<<endl;
    cout<<"INFO: histogram loaded!!!"<<endl;
  }
  if(h->GetEntries() < 10) {
    cout<<"Error: "<<hname<<" has entries less than 10 !!!!"<<endl;
    return;
  }

  // return;
   // h->Draw();
   // getchar();
   TCanvas *cc;
   if (print_draw) {
     setTDRStyle();
     cc=new TCanvas();
     cc->SetWindowSize(650,600);
   }

   
   RooRealVar x("x","ADC count (high gain)",20.0,max_) ;
   // RooRealVar x("x","ADC count (high gain)",0.0,max_) ;

   RooArgList l = RooArgList(x);

   RooRealVar mean0("mean","Smearing mean (fixed at zero, no bias)",0.) ;
   // RooRealVar mean0("mean","Smearing mean (fixed at zero, no bias)",20.) ;
   RooRealVar sigma0("SigmaG","Width of smearing gaussian",5,0,100) ;
   RooGaussian mipG0("mipG","Smearing gaussian",x,mean0,sigma0) ;  

   // RooRealVar ml0("MPV","Landau MPV",45.,10,70) ;
   RooRealVar ml0("MPV","Landau MPV",30.,10,70) ;
   RooRealVar sl0("SigmaL","Landau sigma",5,0.,30) ;
   RooLandau landau1("l1","Single MIP distribution",x,ml0,sl0) ;
  
   RooFormulaVar ml1("MPV1","Laudau MPV for 2-MIP peak","2*@0",ml0) ;
   RooLandau landau2("l2","Two-MIP distribution",x,ml1,sl0) ;

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

   RooRealVar n1mip("n1mip","n1mip",1e3,0.,1.e6) ;
   RooRealVar n2mip("n2mip","n2mip",1e1,0.,1.e6);
   RooRealVar nbkg("nbkg","nbkg",1e1,0.,1.e6);
   RooRealVar nnoise("nnoise","nbkg",1e1,0.,1.e6);

   RooAddPdf model("model","(two landau(X)gaus) + noise + bkg",RooArgList(l1xg,l2xg,grass,noise),RooArgList(n1mip,n2mip,nbkg,nnoise));
   // RooAddPdf model("model","(two landau(X)gaus)+ noise",RooArgList(l1xg,l2xg,noise),RooArgList(n1mip,n2mip,nnoise));
   // RooAddPdf model("model","(landau(X)gaus)+ noise + bkg",RooArgList(l1xg,grass,noise),RooArgList(n1mip,nbkg,nnoise));
   // RooAddPdf model("model","(one landau(X)gaus)+ noise",RooArgList(l1xg,noise),RooArgList(n1mip,nnoise));

   
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
   //spandey derivative
   RooAbsReal* deriv = (RooAbsReal*)model.derivative(x,1) ;
   //RooDerivative *deriv = model.derivative(x,1) ;
   Double_t new_max = deriv->findRoot(x,tf1_min,tf1_max,0) ;

   if (print_draw) {
     rfr->Print("V");
     xframe->Draw() ;
   }
   //log log log
   //gPad->SetLogy();
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
     //tex->DrawLatexNDC(0.15,0.96,os.str().c_str());
     tex->DrawLatexNDC(0.15,0.96,os.str().c_str());
     os.str("");
     //os << setprecision(3) << "MIP = " << f->GetMaximumX() << " #pm " << setprecision(1) << ml0.getError() << " ADC counts";
     os << std::setprecision(3) << "MIP = " << f->GetMaximumX(tf1_min,tf1_max) << " #pm " << std::setprecision(1) << ml0.getError() << " ADC counts";
     //tex->DrawLatexNDC(0.44,0.85,os.str().c_str());
     //tex->DrawLatexNDC(0.44,0.45,os.str().c_str());
     tex->SetTextSize(0.031);
     tex->DrawLatexNDC(0.55,0.45,os.str().c_str());

     os.str("");
     os << std::setprecision(3) << "chi2/ndf = " << xframe->chiSquare();
     tex->DrawLatexNDC(0.55,0.40,os.str().c_str());

     cc->Update();     
     char* canvas_name = new char[200];
      
     // if(channel < 10) 
     //   sprintf(canvas_name,"%d_%d_0%d_%d.gif",layer,skiroc,channel,module);
     // else
     //   sprintf(canvas_name,"%d_%d_%d_%d.gif",layer,skiroc,channel,module);
     char *dir_name = new char[100];
     sprintf(dir_name,"gifs/v2/%d",module);
     if(channel < 10) 
       sprintf(canvas_name,"%s/%d_%d_0%d.gif",dir_name,module,skiroc,channel);
     else
       sprintf(canvas_name,"%s/%d_%d_%d.gif",dir_name,module,skiroc,channel);

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
   float adc_err = ml0.getError();
   char *line = new char[1000];
   char *ins = new char[1000];
   int h_entry = h->GetEntries();
   cout<<"\n\n MIP = "<<ADC_MIP<<" \t derivative_root = "<<new_max<<endl;
   sprintf(line,"%d %d %d %d %.2f %.2f %.2f %d %.4f %.4f %.2f\n",module,skiroc,channel,en_chan,ADC_MIP,chi2_ndf,adc_err,h_entry,const_val,err,mpv_val);
   sprintf(ins,"%d\t%d\t%d\t%.2f\t%.2f\t%.2f\t%d\n",module,skiroc,channel,ADC_MIP,chi2_ndf,adc_err,h_entry);

   cout<<line<<endl;
   // cout<<ins<<endl;
   //myfile << line;
   myfile_all << line;

}


void for_oct_TB(){
  char *f_name = new char[500];
  char *arg = new char[500];
  int modd = 51;
  myfile_all.open("./calibration_all_51.txt");

  sprintf(f_name,"../config_maps/moduleMAP_config1.txt");
  //moduleMap_init("charlie");
  moduleMap_init("bravo");
  std::ifstream in(f_name);
  if(!in){
    cout<<"Could not find "<<f_name<<endl;
    return;
  }
  int modID_, part_, layer_, pos_;
  int count  = 0;
  cout<<"File name = "<<f_name<<endl;
  while(in>>modID_>>part_>>layer_>>pos_){
    if(modID_  != modd) continue;
    cout<<modID_<<" "<<(modID_  == 143 || modID_  == 144 || modID_  == 145 )<<endl;
    std::vector<int> temp_loc = getModuleLocation(modID_);

    if(temp_loc.size() < 3) {
      cout<<"Could Not locate module #"<<modID_<<"!!!"<<endl;
      continue;
    }
    else {
      if(temp_loc.at(0) == 0) {
	//sprintf(arg,"EE_%d_%d",temp_loc.at(1),temp_loc.at(2));
	sprintf(arg,"EE_%d_%d",temp_loc.at(1),temp_loc.at(2));
	// cout<<"Layer = "<<temp_loc.at(1)<<" , Position = "<<temp_loc.at(2)<<endl;
	// continue;
	// Loop("EE",temp_loc.at(1),temp_loc.at(2),2,36,modID_);
	for(int i = 0; i < 4; i++) {
	  for(int j = 0; j < 64; j+=2) {
	    Loop("EE",temp_loc.at(1),temp_loc.at(2),i,j,modID_);
	  }
	}
      }
      else if(temp_loc.at(0) == 1) {
	continue;
	//Loop("FH",temp_loc.at(1),temp_loc.at(2),2,36,modID_);
	for(int i = 0; i < 4; i++) {
	  for(int j = 0; j < 64; j+=2) {
	    //Loop("FH",temp_loc.at(1),temp_loc.at(2),i,j,modID_);
	    sprintf(arg,"FH,%d,%d,%d,%d,%d",temp_loc.at(1),temp_loc.at(2),i,j,modID_);
	    cout<<arg<<endl;
	  }
	}
	

      }
      else{
	sprintf(arg,"WHAT");
      }

    }

    cout<<layer_<<" MODULE "<<modID_<<" LOCATION = "<<arg<<endl;

    count++;
    if(count == 28) break;
  }

  myfile_all.close();
  return;
  //cout<<"INFO: Module Mapping Done!!! "<<endl<<endl;

}



