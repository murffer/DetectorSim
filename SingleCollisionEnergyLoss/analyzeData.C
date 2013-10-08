{
    // ROOT Macro for analyzing histogram data

	// Getting the NTuple / Data
    TFile* fGamma = new TFile("gamma1MeV.root","r");
    TFile* fNeutron = new TFile("neutronThermal.root","r");
    
    TNtuple* trajGamma = (TNtuple*) fGamma->Get("trajTuple");
    TNtuple* trajNeutron = (TNtuple*) fNeutron->Get("trajTuple");
    TNtuple* hitGamma = (TNtuple*) fGamma->Get("hitTuple");
    TNtuple* hitNeutron = (TNtuple*) fNeutron->Get("hitTuple");
    
}
