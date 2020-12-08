#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TProfile.h"

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

void newd2N(){
    const int nbins = 100;
    const double pi = 3.14159265358979;
    const double pmax = 10;

	TProfile *toward = new TProfile("Toward","Toward Region for Multiplicity 80-100",100,0,pmax,"S");
	TProfile *transverse = new TProfile("Transverse","Transverse Region for Multiplicity 80-100",100,0,pmax,"S");
	TProfile *away = new TProfile("Away","Away Region for Multiplicity 80-100",100,0,pmax,"S");
	
	// Copy paste everything from here upto f->Close() and change filename to stack multiple files in same histogram
	TFile *f=new TFile("13TeV_CR0_RHoff.root");
    TTree *tree = (TTree*)f->Get("pytree80100;6");
    int entries = tree->GetEntries();

    const int maxTrack=50000;
    
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

    for(int ii=0; ii<entries; ii++){
    	tree->GetEntry(ii);
    	int ntrks = ntrack;
    	int maxindex = getindexmax(pT,ntrack);
    	double pTlead = pT[maxindex];
    	int ntwrd = 0 ;
    	int naway = 0 ;
    	int ntrns = 0 ;
    	for(int i=0; i<ntrks; i++) {
    		delphi[i] = subtract_angle(phi[i],phi[maxindex]);
    		if((-pi/3 < delphi[i]) && (pi/3 > delphi[i])){	
    			ntwrd ++;
    		}
    		else if((-2*pi/3 > delphi[i]) || (2*pi/3 < delphi[i])){	
    			naway ++;
    		}
    		else{
    			ntrns ++;
    		}
    	}
    	toward->Fill(pTlead,ntwrd);
    	away ->Fill(pTlead,naway);
    	transverse -> Fill(pTlead,ntrns);
    	

    }
    f->Close();
    toward-> Scale(3/(5*pi));
    transverse-> Scale(3/(5*pi));
    away-> Scale(3/(5*pi));
    //toward->Draw();
    transverse->Draw();
    //away->Draw();
    /*TCanvas* c1 = new TCanvas();
    c1->Divide(1,3); // divides the canvas into three rows and three columns
	c1->cd(1);
	toward->Draw();
	c1->cd(2);
	transverse->Draw();
	c1->cd(3);
	away->Draw();*/
};