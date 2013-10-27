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
 * Runs the polystyene analysis
 *
 * @param fN neutron file pointer
 * @param fG gamma file pointer
 */
void RunPS(TFile* fN,TFile* fG){

	TH1F* hN = (TH1F*) fN->Get("3");
  hN->Sumw2();
  hN->Scale(1.0/hN->GetIntegral());
	TH1F* hG = (TH1F*) fG->Get("3");
  hG->Sumw2();
  hG->Scale(1.0/hG->GetIntegral());
	TCanvas* c = new TCanvas();
  gStyle->SetOptStat(0);
	hN->Draw();
	hN->SetTitle("Optical Photons Detected");
	hN->GetXaxis()->SetTitle("Number of Photons");
  hN->SetLineColor(1);
	hG->Draw("same");
  hG->SetLineColor(2);
  c->Update();
  c->SaveAs("PSSimulatedLightOverlap.eps");
  
  ExportHistogram(fN,"3","PSNeutronOPDist.csv");
  ExportHistogram(fG,"3","PSGammaOPDist.csv");

}
/**
 * Runs the GS20 analysis
 *
 * @param fN neutron file pointer
 * @param fG gamma file pointer
 */
void RunGS20(TFile* fN,TFile* fG){

	TH1F* hN = (TH1F*) fN->Get("3");
  hN->Sumw2();
  hN->Scale(1.0/hN->GetIntegral());
	TH1F* hG = (TH1F*) fG->Get("3");
  hG->Sumw2();
  hG->Scale(1.0/hG->GetIntegral());
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
  
  ExportHistogram(fN,"3","GS20NeutronOPDist.csv");
  ExportHistogram(fG,"3","GS20GammaOPDist.csv");

}
/**
 * Main
 *  root[#] .L Analysis.C
 *  root[#] main()
 */
int main(){
 
	TFile* fN = NULL;
	TFile* fG = NULL;

	/* Running the Polystyrene */
	fN = new TFile("PS_Neutron.root","r");
  fG = new TFile("PS_Gamma.root","r");
	if (fN && fG){
		RunPS(fN,fG);
		fN->Close();
		fG->Close();
	}

	/* Runnign the GS20 */
	fN = new TFile("GS20_Neutron.root","r");
  fG = new TFile("GS20_Gamma.root","r");
	if (fN && fG){
		RunGS20(fN,fG);
		fN->Close();
		fG->Close();
	}
}
