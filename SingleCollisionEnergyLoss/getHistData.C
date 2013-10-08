{
	gROOT->Reset();

	// Getting Files
	TFile* f30 = new TFile("run_0.root","r");
	TFile* f50 = new TFile("run_1.root","r");
	TFile* f100 = new TFile("run_1.root","r");
	TFile* f500 = new TFile("run_1.root","r");
	TFile* f1000 = new TFile("run_1.root","r");

	// Getting Ntuples
    TNtuple* t30 = (TNtuple*) f30->Get("trajTuple");
    TNtuple* t50 = (TNtuple*) f50->Get("trajTuple");
    TNtuple* t100 = (TNtuple*) f100->Get("trajTuple");
    TNtuple* t500 = (TNtuple*) f500->Get("trajTuple");
    TNtuple* t1000 = (TNtuple*) f1000->Get("trajTuple");

	Float_t eLoss30, eLoss50, eLoss100, eLoss500, eLoss1000;

	// Opening Output File
	FILE *fout = fopen("outData.txt","w");
	if( fout == NULL)
		printf("Could not open output file\n");
	else{
		t30->SetBranchAddress("eLossColl",&eLoss30);
		t50->SetBranchAddress("eLossColl",&eLoss50);
		t100->SetBranchAddress("eLossColl",&eLoss100);
		t500->SetBranchAddress("eLossColl",&eLoss500);
		t1000->SetBranchAddress("eLossColl",&eLoss1000);
		
        fprintf(fout,"30eV\t\t\t50eV\t\t\t100eV\t\t\t500eV\t\t\t1000eV\n");
        Int_t nbytes = 0;
		for(Int_t i=0; i<t30->GetEntries(); i++){
			nbytes += t30->GetEvent(i);
			nbytes += t50->GetEvent(i);
			nbytes += t100->GetEvent(i);
			nbytes += t500->GetEvent(i);
			nbytes += t1000->GetEvent(i);
			fprintf(fout,"%e\t%e\t%e\t%e\t%e\n",eLoss30,eLoss50,
                eLoss100,eLoss500,eLoss1000);
		}
		fclose(fout);
	}
	f30->Close();
	f50->Close();
	f100->Close();
	f500->Close();
	f1000->Close();
}
