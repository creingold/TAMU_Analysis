{

	TFile *run00443 = new TFile("run00443.root");
	if ( !run00443 ){
		cout << "Error: Cannot find run00443.root, must exit" << endl;
		exit(1);
	}
	
	TH1F *h0 = new TH1F("h0" , "E1" , 1600 , 800 , 2400 );
	TCanvas *c0 = new TCanvas("c0" , "E1");

	c0->cd();
	
	double par[100];
	char cString[80];
	char writeString[80];

	for ( int segment = 34 ; segment < 64 ; segment += 1 ){

		

		sprintf(writeString , "NewalphaCal%d.dat" , segment);
		ofstream datFile(writeString);
		if ( !datFile ){
			cout << "Error: Cannot open write file" << endl;
			exit(1);
		}

	//	datFile << "Amp\t\tError\t\tMean\t\tError\t\tStdDev\t\tError\t\tInt\t\tError\t\tSlope" << endl;

		sprintf(cString , "ADC[%d] >> h0" , segment);
		t->Draw(cString);
		h0->Draw();

		TSpectrum *spec = new TSpectrum(6);
		int peaksFound = spec->Search(h0 , 2 , "" , 0.2);

		float *peakLocations = spec->GetPositionX();
		
		cout << "Peaks detected at: " << endl;
		for ( int i = 0 ; i < peaksFound ; i++){
			cout << peakLocations[i] << endl << endl;
		}

		for ( int j = 0 ; j < peaksFound ; j++){

			double dMean = peakLocations[j];
			int bin = h0->GetXaxis()->FindBin(dMean);
			double dAmp = h0->GetBinContent(bin);
			double stdDev = 10;

			par[0] = dAmp;
			par[1] = dMean;
			par[2] = stdDev;
			par[3] = 10;
			par[4] = 0.5;
						
			double rangeMin = dMean - 3*stdDev;
			double rangeMax = dMean + 3*stdDev;

			TF1 *total = new TF1("fit","gaus(0) + pol 1(3)",rangeMin,rangeMax);
			fit->SetParameters(par);

			h0->Fit(fit,"R+");

			//for ( int k = 0 ; k < 5 ; k++ ){
			for ( int k = 1 ; k < 2 ; k++ ){			
				double toWrite = fit->GetParameter(k);
				//double errorWrite = fit->GetParError(k);
				datFile << toWrite; //<< "\t\t" << errorWrite << "\t\t";
			}
			datFile << endl;
			h0->Draw();
			char writePdf[80], writeTitle[80];
			//sprintf(writePdf,"alphaCal%d.pdf",segment);
			//sprintf(writeTitle,"Alpha calibration fitting on Segment %d",segment);
			//c0->Print(writePdf , writeTitle);

		}

		h0->Draw();
		datFile.close();

	}



}
