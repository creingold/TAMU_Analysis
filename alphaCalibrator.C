/*
Craig Reingold
9/30/15
TAMU Alpha Calibrator

This file is a ROOT macro that will calibrate the silicon detector in STARLiGHTER.

It will read in the data to a histogram, and then fit the peaks to a Gaussian with a linear background.

ADD MORE HERE AS YOU GO

*/

/*
double linGauss(double *x , double *par) {
		double linearPart = par[0] + par[1] * x[0];
		double gaussPart = par[2]*TMath::Gaus(x[0],par[3],par[4]);
		double result = linearPart + gaussPart;
		return result;
	}

void alphaCalibrator() {
*/

{
	TFile *run00443 = new TFile("run00443.root");
	if ( !run00443 ){
		cout << "Cannot find run00443.root, must exit" << endl;
		exit(1);
	}


	// Generating a fit function that is a linear combination of a linear background and a Gaussian

	TH1F *h0 = new TH1F("h0" , "E1" , 2400 , 100 , 2500 );
	TCanvas *c0 = new TCanvas("c0" , "E1" );

	c0->cd();

	char cString[80];

	double par[5];

	for ( int segment = 34 ; segment < 35 ; segment += 1 ){
		sprintf(cString , "ADC[%d] >> h0" , segment);
		t->Draw(cString);
		h0->Draw();

		//Trying to fit the 4 visible peaks
		TSpectrum *spec = new TSpectrum(5);
		int peaksFound = spec->Search(h0,5,"",0.2);

		// Uncomment below for debugging
		h0->Draw();

		int nPeaks = 0;
		double *xpeaks = spec->GetPositionX();
		for ( int p = 0 ; p < peaksFound ; p++ ){
			double xCoord = xpeaks[p];
			int bin = h0->GetXaxis()->FindBin(xCoord);
			double yCoord = h0->GetBinContent(bin);

		//Setting Fit parameters for the specific peak found
			
			int xMin = xCoord - 10;
			int xMax = xCoord + 10;


			TF1 *fitFunction = new TF1("fitFunction" , "gaus + pol1" , xMin , xMax);


			par[0] = yCoord;
			par[1] = xCoord;
			par[2] = 3;

			//int xMin = xCoord - 10;
			//int xMax = xCoord + 10;

			fitFunction->SetNpx(xMax - xMin);
			fitFunction->SetParameters(par);
		
			h0->Fit("fitFunction");

			h0->Draw();

		}

	}


}
