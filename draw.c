
// Taken from this example: https://root.cern.ch/doc/master/classTGeoManager.html

void drawNew(const char* filename="emc_module12_2018v1.root"){

	TFile* f = new TFile(filename, "READ");
	TGeoVolumeAssembly* assembly = (TGeoVolumeAssembly*)f->Get("BarrelEMC");
	assembly->Print();

	// Define vacuum medium and material
	TGeoMaterial *vacuumMaterial = new TGeoMaterial("vacuumMaterial", 0, 0, 0);
	TGeoMedium *vacuumMedium = new TGeoMedium("vacuumMedium", 1, vacuumMaterial);

	// Create top world volume
	// TGeoManager *geoManager = new TGeoManager("geoManager", "Simple geometry");
	TGeoVolume *topVolume = gGeoManager->MakeBox("topVolume", vacuumMedium, 0, 0, 0);
	topVolume->AddNode(assembly, 1);
	// topVolume->SetVisibility(kFALSE);

	gGeoManager->SetTopVolume(topVolume);
	// geoManager->SetTopVolume(topVolume);
	gGeoManager->CloseGeometry();

	// Add child nodes
	// std::cout << "Assembly contains " <<  barrelEMC->GetRefCount() << " daughters." << std::endl;
	// for (int i = 0; i < barrelEMC->GetRefCount(); i++){
	// 	TGeoNode* node = barrelEMC->GetNode(i);
	// 	if (node->InheritsFrom(TGeoVolumeAssembly::Class())){
	// 		TGeoVolumeAssembly* assembly = (TGeoVolumeAssembly*)node;
	// 		assembly->Print();
	// 	}
	// }

	gGeoManager->SetVisLevel(4);
	// gStyle->SetCanvasPreferGL();
	topVolume->Draw("ogl");
}
