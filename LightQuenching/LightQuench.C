using namespace std;
/**
 * Saving Histogram Data
 */
void ExportHistogram(TFile* f,const char* histKey,const char* outputfile){
  // Root Setup
  TH1F* h = (TH1F*) f->Get(histKey);

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
 * @param xMin - minumm x value
 * @param xMax - maximum x value
 * @param title - title of the histogram
 * @param xLabel- x axis label
 * @param norm  - normalization
 * @param drawLogX - flag to draw logarathmic in X
 * @param drawLogY - flag to draw logarathmic in Y
 */
void PlotHistogram(TObjArray *hist, TObjArray *names,double xMin, double xMax,const char *title,const char *xLabel, const char* histFileName, double norm,bool drawLogX, bool drawLogY){
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
    if (norm == 0){
      // h->Scale(1.0/h->Integral());
      h->Scale(1.0/h->GetEntries());
    }
    else
      h->Scale(1.0/norm);
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
 *  root[#] .L LightQuench.C
 *  root[#] main()
 */
int main(){
  TFile* fA = new TFile("alphaOP.root","r");
  TFile* fT = new TFile("tritonOP.root","r");
  TFile* fE = new TFile("electronOP.root","r");

  TObjArray *hist = new TObjArray();
  hist->Add(fA->Get("1"));
  hist->Add(fT->Get("1"));
  hist->Add(fE->Get("1"));
  TObjArray *names = new TObjArray();
  names->Add(new TObjString("Alpha"));
  names->Add(new TObjString("Triton"));
  names->Add(new TObjString("Electron"));
  PlotHistogram(hist,names,0,1000,"Energy of Charged Particles at Creation","Energy [keV]","LightQuenchExample.png",0,false,false);
  
  ExportHistogram(fA,"1","AlphaOPDist.csv");
  ExportHistogram(fT,"1","TritonOPDist.csv");
  ExportHistogram(fE,"1","ElectronOPDist.csv");
}
