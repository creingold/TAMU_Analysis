{
	TCanvas *c0 = new TCanvas("c0" , "E1");

	c0->cd();
	
	double par[2];
	char cString[80];
	char writeString[80];
	char outFileString[80];
	ofstream outFile("calParam.dat");
	if (!outFile){
		cout << "Error: cannot open outfile";
		exit(1);
	}

	for ( int segment = 34 ; segment < 64 ; segment += 1 ){

		sprintf(writeString , "NewalphaCal%d.dat" , segment);
		ifstream datFile2(writeString);
		if ( !datFile2 ){
			cout << "Error: Cannot open read file" << endl;
			exit(1);
		}

		
		double channel[4]; 
		double channelSort[4];
		int indexArray[4];
		for ( int b = 0 ; b < 4 ; b++ ){
			datFile2 >> channel[b];
		}
		
		double EnergykeV[4];
		EnergykeV[0] = 4780;
		EnergykeV[1] = 5480;
		EnergykeV[2] = 6000;
		EnergykeV[3] = 7680;
		
		int iFour = 4;

		TMath::Sort( iFour , channel, indexArray , kFALSE );

		for ( int d = 0 ; d < iFour ; d++ ){
			channelSort[d] = channel[indexArray[d]];
		}
		
		TGraph *graph = new TGraph( 4, channelSort , EnergykeV );
		

		graph->Draw("A*");

		TF1 *total = new TF1("fit","pol 1", 1200 , 2400);
		fit->SetParameters(par);

		graph->Fit(fit,"R+");

		for ( int k = 0 ; k < 2 ; k++ ){
			double toWrite = fit->GetParameter(k);
			//double errorWrite = fit->GetParError(k);
			
			outFile << toWrite << '\t' ;
		}
		outFile << endl;

		graph->Draw();		

		datFile2.close();
	}

	outFile.close();

}
