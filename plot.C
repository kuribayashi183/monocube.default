
void plot1();

void plot()
{
   gStyle->SetOptStat(0);
   gStyle->SetNumberContours(45);
   gStyle->SetPadRightMargin (0.12);
	plot1();
}

void plot1()
{
	int maxfile=30;
//  KEY: TTree cube;1   n, e, x, y, z, nphotons, npx, npy, npz
   TH2F *nAllPhotoMap= new TH2F("nAllPhotoMap","nAllPhotoMap",30,-0.5,0.5,30,-0.5,0.5);
   TH2F *nPhotoXread = new TH2F("nPhotoXread","nPhotoXread",30,-0.5,0.5,30,-0.5,0.5);
   TH2F *nPhotoYread = new TH2F("nPhotoYread","nPhotoYread",30,-0.5,0.5,30,-0.5,0.5);
   TH2F *nPhotoZread = new TH2F("nPhotoZread","nPhotoZread",30,-0.5,0.5,30,-0.5,0.5);
   TH1F *nPhotoXreadAll = new TH1F("nPhotoXreadAll","nPhotoXreadAll",200,0,200);
   TH1F *nPhotoYreadAll = new TH1F("nPhotoYreadAll","nPhotoYreadAll",200,0,200);
   TH1F *nPhotoZreadAll = new TH1F("nPhotoZreadAll","nPhotoZreadAll",200,0,200);

	TFile* tf[30][30];
	TTree* tt[30][30];
	double aveYieldX=0;
	double aveYieldY=0;
	double aveYieldZ=0;
	for (int i=0; i<maxfile; i++) {
		for (int j=0; j<maxfile; j++) {
			stringstream name;
			name << "../output/root_X" << i << "_Y" << j << ".root";
			cerr << name.str().data() << endl;
			tf[i][j]	= new TFile(name.str().data());
			//tf[i][j] ->ls();
   		tt[i][j] = static_cast<TTree*>(tf[i][j]->Get("cube"));

			tt[i][j]->Draw("nPhotons>>a","");
   		TH1F *a = static_cast<TH1F*>(gROOT->FindObject("a"));
			nAllPhotoMap->SetBinContent(i+1,j+1,a->GetMean());

         tt[i][j]->Draw("npx>>x(200,0,200)","");
         TH1F *x = static_cast<TH1F*>(gROOT->FindObject("x"));
         nPhotoXread->SetBinContent(i+1,j+1,x->GetMean());

         tt[i][j]->Draw("npy>>y(200,0,200)","");
         TH1F *y = static_cast<TH1F*>(gROOT->FindObject("y"));
         nPhotoYread->SetBinContent(i+1,j+1,y->GetMean());

         tt[i][j]->Draw("npz>>z(200,0,200)","");
         TH1F *z = static_cast<TH1F*>(gROOT->FindObject("z"));
         nPhotoZread->SetBinContent(i+1,j+1,z->GetMean());

			nPhotoXreadAll->Add(x);
			nPhotoYreadAll->Add(y);
			nPhotoZreadAll->Add(z);
			cerr << x->GetMean() << " " << y->GetMean() << " " << z->GetMean() << endl;

			aveYieldX+=x->GetMean();
			aveYieldY+=y->GetMean();
			aveYieldZ+=z->GetMean();

			tf[i][j] ->Close();
		}
	}
	//return;

	cerr << "ave Yield X = " << aveYieldX/pow(maxfile,2) << endl;
	cerr << "ave Yield Y = " << aveYieldY/pow(maxfile,2) << endl;
	cerr << "ave Yield Z = " << aveYieldZ/pow(maxfile,2) << endl;

	TCanvas *c1 = new TCanvas();
	nAllPhotoMap->Draw("colz");
	//return;

	TCanvas *c2 = new TCanvas("c2","c2",800,800);
	c2->Divide(2,2);
	c2->cd(1); nPhotoXread->Draw("colz"); 
	c2->cd(2); nPhotoYread->Draw("colz"); 
	c2->cd(3); nPhotoZread->Draw("colz"); 

   TCanvas *c3 = new TCanvas("c3","c3",900,300);
   c3->Divide(3,1);
	c3->cd(1); nPhotoXreadAll->Draw(" ");
	c3->cd(2); nPhotoYreadAll->Draw(" ");
	c3->cd(3); nPhotoZreadAll->Draw(" ");

 	nPhotoXread->GetZaxis()->SetRangeUser(10,35);
 	nPhotoYread->GetZaxis()->SetRangeUser(10,35);
 	nPhotoZread->GetZaxis()->SetRangeUser(10,35);
}
