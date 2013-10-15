using namespace std;
/**
 * Saving Histogram Data
 */
void ExportHistogram(TFile* f,const char* histKey,const char* outputfile){
  // Root Setup
  TH1F* h = (TH1F*) f->Get(histKey);
	h->Sumw2();
	h->Scale(1.0/h->GetEntries());
  // File Setup
  ofstream out;
  out.open(outputfile);
  for (int i = 1; i < h->GetNbinsX()-1; i++){
    out<<h->GetBinCenter(i)<<","<<h->GetBinContent(i)<<","<<h->GetBinError(i)<<endl;
  }
  out.close();
}


/**
 * Main
 *  root[#] .L ExportHist.C
 *  root[#] main()
 */
int main(){
	TFile* fN1 = new TFile("neutron_50um.root","r");
	TFile* fN2 = new TFile("neutron_25um.root","r");
	TFile* fN3 = new TFile("neutron_10um.root","r");

	TFile* fG1 = new TFile("Co60_50um.root","r");
	TFile* fG2 = new TFile("Co60_25um.root","r");
	TFile* fG3 = new TFile("Co60_10um.root","r");

	ExportHistogram(fN1,"numPhotons","neutron_50um.csv");
	ExportHistogram(fN2,"numPhotons","neutron_25um.csv");
	ExportHistogram(fN3,"numPhotons","neutron_10um.csv");
	
	ExportHistogram(fG1,"numPhotons","Co60_50um.csv");
	ExportHistogram(fG2,"numPhotons","Co60_25um.csv");
	ExportHistogram(fG3,"numPhotons","Co60_10um.csv");
}
