{
	t.Draw("clover.energy[0] >> h1(3000,100,3101)" , "telescope.pid == 3 && ((leaf.energy[0] > 10 && leaf.energy[1] > 10) || (leaf.energy[2] > 10 && leaf.energy[3] > 10))");


	t.Draw("clover.energy[0] >> h2(3000,100,3101)" , "telescope.pid == 3 && ((leaf.energy[0] > 10 && leaf.energy[3] > 10) || (leaf.energy[1] > 10 && leaf.energy[2] > 10))");

	h1->Draw();
	h2->Draw("same");

	h2->SetLineColor(2);

}
