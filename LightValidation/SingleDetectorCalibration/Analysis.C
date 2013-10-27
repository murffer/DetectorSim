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
 * Main
 *  root[#] .L Analysis.C
 *  root[#] main()
 */
int main(){
  TFile* fN = new TFile("Neutron.root","r");
  TFile* fG = new TFile("Gamma.root","r");
	std::cout<<"Got the file pointers"<<std::endl;

	TH1F* hN = (TH1F*) fN->Get("3");
  hN->Sumw2();
  hN->Scale(1.0/hN->GetIntegral());
	TH1F* hG = (TH1F*) fG->Get("3");
  hG->Sumw2();
  hG->Scale(1.0/hG->GetIntegral());
	std::cout<<"Got the histogram pointers"<<std::endl;
	TCanvas* c = new TCanvas();
  gStyle->SetOptStat(0);
	hN->Draw();
	hN->SetTitle("Optical Photons Detected");
	hN->GetXaxis()->SetTitle("Number of Photons");
  hN->SetLineColor(1);
	hG->Draw("same");
  hG->SetLineColor(2);
  c->Update();
  c->SaveAs("GS20SimulatedLightOverlap.eps");
	std::cout<<"Wrote the histogram"<<std::endl;
  
  ExportHistogram(fN,"3","NeutronOPDist.csv");
  ExportHistogram(fG,"3","GammaOPDist.csv");
}
