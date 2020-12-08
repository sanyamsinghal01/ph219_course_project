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

void newd2psum(){
    const int nbins = 100;
    const double pi = 3.14159265358979;
    const double pmax = 10;

	TProfile *toward = new TProfile("Toward","Toward Region for Multiplicity 20-40",100,0,pmax,"S"); //make changes here as you change the multiplicity class
	TProfile *transverse = new TProfile("Transverse","Transverse Region for Multiplicity 20-40",100,0,pmax,"S"); //here too
	TProfile *away = new TProfile("Away","Away Region for Multiplicity 20-40",100,0,pmax,"S"); //here too
	
	// Copy paste everything from here upto f->Close() and change filename to stack multiple files in same histogram
	TFile *f=new TFile("13TeV_CR0_RHoff.root");
    TTree *tree = (TTree*)f->Get("pytree2040;27"); //this represents multiplicity class 20-40
    /* 
    for multiplicity 0-20, use pytree020;23
    for multiplicity 0-20, use pytree2040;27
    for multiplicity 0-20, use pytree4060;23
    for multiplicity 0-20, use pytree6080;15
    for multiplicity 0-20, use pytree80100;6
    */
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
    	double ptwrd = 0 ;
    	double paway = 0 ;
    	double ptrns = 0 ;
    	for(int i=0; i<ntrks; i++) {
    		delphi[i] = subtract_angle(phi[i],phi[maxindex]);
    		if((-pi/3 < delphi[i]) && (pi/3 > delphi[i])){	
    			ptwrd += pT[i];
    		}
    		else if((-2*pi/3 > delphi[i]) || (2*pi/3 < delphi[i])){	
    			paway += pT[i];
    		}
    		else{
    			ptrns += pT[i];
    		}
    	}
    	toward->Fill(pTlead,ptwrd);
    	away ->Fill(pTlead,paway);
    	transverse -> Fill(pTlead,ptrns);
    	

    }
    f->Close();
    toward-> Scale(3/(5*pi));
    transverse-> Scale(3/(5*pi));
    away-> Scale(3/(5*pi));
    toward->Draw();
    //transverse->Draw();
    //away->Draw();
    //this will draw only the toward part, uncomment the away part and transverse part
    // one by one to obtain them also
    // so you have to make 3 plots per multiplicity class


    //dont use this:
    /*TCanvas* c1 = new TCanvas();
    c1->Divide(1,3); // divides the canvas into three rows and three columns
	c1->cd(1);
	toward->Draw();
	c1->cd(2);
	transverse->Draw();
	c1->cd(3);
	away->Draw();*/
};