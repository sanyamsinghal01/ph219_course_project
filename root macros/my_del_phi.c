#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THn.h"
#include "THnSparse.h"
#include "TStopwatch.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TTree.h"
#include "TSystem.h"

int getindexmax(double * array, int length){
	double max = array[0];
	int indmax = 0;
	for(int i = 1; i < length ; i++){
		if(array[i] > max){
			max = array[i];
			indmax = i;
		}
	}
	return indmax;
}

double subtract_angle(double phi1, double phi2){
	const double pi = 3.14159265358979;
	double dphi = phi1 - phi2;
	if(dphi > pi){
		return (dphi - 2*pi);
	}
	if (dphi < -pi)
	{
		return (dphi + 2*pi);
	}
	else{
		return dphi;
	}
}

void my_del_phi(){
	TFile *f=new TFile("13TeV_CR0_RHoff.root");
    TTree *tree = (TTree*)f->Get("pytree80100;6");
    int entries = tree->GetEntries();

    const int maxTrack=10000;
   
    int ntrack = 0;
    double pT[maxTrack];
    double eta[maxTrack];
    double rap[maxTrack];
    double phi[maxTrack];
    double delphi[maxTrack];


    tree->SetBranchAddress("ntrack",&ntrack);
    tree->SetBranchAddress("phi",&phi);
    tree->SetBranchAddress("pT",&pT);
    tree->SetBranchAddress("eta",&eta);

    TH1D* hdelphi = new TH1D("Variation of Delta Phi","Delta Phi for Multiplicity 80-100 at 13 TeV",200,0.,3.14159265358979);

    for(int ii=0; ii<entries; ii++){
    	tree->GetEntry(ii);
    	int ntrks = ntrack;
    	int maxindex = getindexmax(pT,ntrack);
    	for(int i=0; i<ntrks; i++) {
    		delphi[i] = TMath::Abs(subtract_angle(phi[i],phi[maxindex]));
    		if(i != maxindex){	
    			hdelphi -> Fill(delphi[i]);
    		}
    	
    	}

    }
    Double_t factor = 1.;
    hdelphi->Scale(factor/hdelphi->GetEntries());
    hdelphi -> Draw();
};