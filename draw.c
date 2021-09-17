
// Taken from this example: https://root.cern.ch/doc/master/classTGeoManager.html

void draw(const char* filename="emc_module12_2018v1.root"){

	gStyle->SetCanvasPreferGL(1);

	TFile* f = new TFile(filename, "READ");
	TGeoVolumeAssembly* assembly = (TGeoVolumeAssembly*)f->Get("BarrelEMC");
	assembly->Print();

	// Define vacuum medium and material
	TGeoMaterial *vacuumMaterial = new TGeoMaterial("vacuumMaterial", 0, 0, 0);
	TGeoMedium *vacuumMedium = new TGeoMedium("vacuumMedium", 1, vacuumMaterial);

	// Create top world volume
	TGeoVolume *topVolume = gGeoManager->MakeBox("topVolume", vacuumMedium, 0, 0, 0);
	topVolume->AddNode(assembly, 1);

	gGeoManager->SetVisLevel(5);
	gGeoManager->SetTopVolume(topVolume);
	gGeoManager->CloseGeometry();

	TObjArray* nodesArray = assembly->GetNodes();

	// Iterate child nodes
	std::cout << "Assembly contains " <<  nodesArray->GetEntries() << " entries." << std::endl;
	for (int i = 0; i < nodesArray->GetEntries(); i++){
		TGeoNode* node = assembly->GetNode(i);
		node->Print();
	}

	// Print certain node
	// for (int i = 0; i < nodesArray->GetEntries(); i++){
	// }

	// Print one node
	// TGeoNode* node = assembly->GetNode(15);
	// if (node->InheritsFrom(TGeoNodeMatrix ::Class())){
	// 	TGeoNodeMatrix* nodeMatrix = (TGeoNodeMatrix*)node;
	// 	TGeoVolume* volume = nodeMatrix->GetVolume();
	// 	if (volume->InheritsFrom(TGeoVolumeAssembly::Class())){
	// 		TGeoVolumeAssembly* childAssembly = (TGeoVolumeAssembly*)volume;
	// 		std::cout << "^^ Is volume assembly" << std::endl;
	// 		// TString volumeName = TString::Format("myVolume%d", i);
	// 		// TGeoVolume *myVolume = gGeoManager->MakeBox(volumeName.Data(), vacuumMedium, 0, 0, 0);
	// 		topVolume->AddNode(childAssembly, 1);
	// 	}
	// }

	// gStyle->SetCanvasPreferGL();
	// topVolume->Draw("");
	topVolume->Draw("ogl");
}
