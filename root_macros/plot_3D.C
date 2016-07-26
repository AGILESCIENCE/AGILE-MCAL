{	
	gROOT->Reset();
	TStyle *mcalstyle = new TStyle("mcalstyle","mcalview default style ");
	mcalstyle->SetCanvasBorderMode(0);
	mcalstyle->SetPadBorderMode(0);
	mcalstyle->SetPadColor(0);
	mcalstyle->SetCanvasColor(0);
	mcalstyle->SetTitleColor(1); // 0
	mcalstyle->SetStatColor(0);
	mcalstyle->SetPalette(1,0);  // 2D graph colour version
	//mcalstyle->SetPalette(9,0);  // 2D graph B/W version
	mcalstyle->SetFrameBorderMode(0);
	mcalstyle->SetLabelSize(0.05, "XYZ");
	mcalstyle->SetTitleSize(0.05, "XYZ");
// 	mcalstyle->SetLabelSize(0.05, "Y");
// 	mcalstyle->SetLabelSize(0.05, "Z");
	mcalstyle->SetTitleOffset(1.4, "XYZ");
	gROOT->SetStyle("mcalstyle");

	double a = 1.;  // 6378.137
	double a2 = a*a;
	
	TF3 f3("f3", "x*x +y*y + z*z - 1", -1.2*a, 1.2*a, -1.2*a, 1.2*a, -1.2*a, 1.2*a);
 	f3.Draw("SURF");
	
	
	
}

// S. Jadach, arrows.C, truly 3-dim. reference system
gROOT->Reset();
c1 = new TCanvas("c1","Geometry Shapes",0,0,700,700);

brik = new TBRIK("BRIK","BRIK","void",1000,1000,1000);
cone = new TCONE("CONE","CONE","void",  200,   0,200,  0,0);
//                                     +-z   r1,rmax  r1,rmax
tube = new TTUBE("TUBE","TUBE","void",  0,50,500);
cone->SetLineColor(1);
tube->SetLineColor(2);

//delete rot1;
TRotMatrix *rot1 = new TRotMatrix("rot1","rot1"     ,90,90  ,0,0 ,90,0 );
//                                                     y      z    x
TRotMatrix *rot2 = new TRotMatrix("rot2","rot2"      ,0,0  ,90,0 ,90,90);
//                                                     y      z    x

//  Build the geometry hierarchy
node1 = new TNode("NODE1","NODE1","BRIK");
node1->cd();
TNode *node2 = new TNode("NODE2","NODE2","TUBE", 500,0,0,"rot1");
node2->cd();
TNode *node3 = new TNode("NODE3","NODE3","CONE", 0, 0, 700,"");
node3->SetLineColor(6);

node1->cd();
TNode *node4 = new TNode("NODE4","NODE4","TUBE",  0, 0, 500);
node4->cd();
TNode *node5 = new TNode("NODE5","NODE5","CONE", 0, 0, 700);
node3->SetLineColor(4);

node1->cd();
TNode *node6 = new TNode("NODE6","NODE6","TUBE",  0, 500, 0,"rot2");
node6->cd();
TNode *node7 = new TNode("NODE7","NODE7","CONE", 0, 0, 700);
node7->SetLineColor(5);

// Draw this geometry in the current canvas
node1->cd();
node1->Draw();
node1->SetVisibility(0);
c1->Update();

   pline3D = new TPolyLine3D(2,"");
   pline3D->SetLineColor(7);
   pline3D->SetLineWidth(2);
   pline3D->SetPoint(0, 0,0,0);
   pline3D->SetPoint(1, 1200,800,1000);
   pline3D->Draw();



