/*
 * Makefile is provided
 */
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <THStack.h>

using namespace std;
/**
 * Saving Histogram Data
 */
void ExportHistogram(TFile* f,const char* histKey,const char* outputfile){
  // Root Setup
  TH1F* h = (TH1F*) f->Get(histKey);
  // DO NOT SCALE / Sumw2, already compelted!
  //h->Sumw2();
  //h->Scale(1.0/h->GetEntries());

  // File Setup
  ofstream out;
  out.open(outputfile);
  out<<"Bin,Value,Error"<<endl;
  for (int i = 1; i < h->GetNbinsX()-1; i++){
    out<<h->GetBinCenter(i)<<","<<h->GetBinContent(i)<<","<<h->GetBinError(i)<<endl;
  }
  out.close();
}

/**
 * @brief Plots an array of histogram
 * @param hist  - TObjArray of histograms
 * @param names - TObjArray of histgram names (thickness)
 * @param title - title of the histogram
 * @param xLabel- x axis label
 * @param drawLogX - flag to draw logarathmic in X
 * @param drawLogY - flag to draw logarathmic in Y
 */
void PlotHistogram(TObjArray *hist, TObjArray *names,double xMin, double xMax,const char *title,const char *xLabel, const char* histFileName, bool drawLogX, bool drawLogY){
    TH1F* h = NULL;
    TObjString *s = NULL;
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas();
    TLegend* leg = new TLegend(0.8,0.8,0.95,0.95);
    leg->SetFillColor(0);
    double yMax = 0.0;
    double yMin = 0.0;
    THStack *hs = new THStack("hs",title);
    for (int i = 0; i < hist->GetEntriesFast(); i ++){
        h = (TH1F*) hist->At(i);
        if (!h)
          std::cerr<<"Histogram pointer is NULL"<<std::endl;
        s = (TObjString*) names->At(i);
        leg->AddEntry(h,s->String().Data(),"l");
            h->Sumw2();
            h->Scale(1.0/100000)
            //h->Scale(1.0/h->Integral());
            //h->Scale(1.0/h->GetEntries());
        h->SetLineColor(i+1);
        hs->Add(h);
    }
    hs->Draw("HIST,E");
    hs->GetHistogram()->GetXaxis()->SetTitle(xLabel);
    hs->GetHistogram()->GetYaxis()->SetTitle("Frequency");
    hs->GetHistogram()->GetXaxis()->SetRange(xMin,xMax);
    // Histogram Prettying
    if (drawLogY)
      gPad->SetLogy();
    if (drawLogX)
      gPad->SetLogx();
    gROOT->SetStyle("Plain");
    leg->Draw();
    c1->Update();
    c1->SaveAs(histFileName);
}


/**
 * Main
 *  root[#] .L ChargedParticleSecondarEnergy.C
 *  root[#] main()
 */
int main(){
    TFile* fG = new TFile("co60Range.root","r");
    TFile* fA = new TFile("alphaRange.root","r");
    TFile* fT = new TFile("tritonRange.root","r");
    
    TObjArray *hist = new TObjArray();
    hist->Add(fG->Get("5"));
    hist->Add(fA->Get("5"));
    hist->Add(fT->Get("5"));
    TObjArray *names = new TObjArray();
    names->Add(new TObjString("{}^{60}Co"));
    names->Add(new TObjString("Alpha"));
    names->Add(new TObjString("Triton"));
    bool logX = false;
    bool logY = true;
    PlotHistogram(hist,names,0,120,"Energy of Charged Particles at Creation","Energy [MeV]","ChargedParticleEnergy.png",logX,logY);
   
}
