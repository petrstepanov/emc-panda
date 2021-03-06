#include <TGFileDialog.h>
#include <TFile.h>
#include <TGeoVolume.h>
#include <TGeoShape.h>
#include <TGeoBBox.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <iostream>

// Refer to this example: https://root.cern.ch/doc/master/classTGeoManager.html
int main(int argc, char **argv){

	TFile* f = new TFile("./resources/emc_module12_2018v1.root", "READ");
	while (f->IsZombie()) {
		const char *filetypes[] = { "All files",     "*",
		                            "ROOT files",    "*.root",
		                            0,               0 };
		TGFileInfo fi;
		fi.fFileTypes = filetypes;
		new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fi);

		if (fi.fMultipleSelection){
		    std::cout << "ERROR: multiple files not supported yet." << std::endl;
		}
		if (fi.fFilename){
			f = new TFile(fi.fFilename, "READ");
		}
		if (f->IsZombie()){
		    std::cout << "ERROR: corrupted file." << std::endl;
		}
	};

	TGeoVolumeAssembly* assembly = (TGeoVolumeAssembly*)f->Get("BarrelEMC");
	if (assembly == NULL){
	    std::cout << "ERROR: cannot find \"BarrelEMC\" object." << std::endl;
		return -1;
	}
	assembly->Dump();
	assembly->Print();

	// Get Assembly Boundsing Box size
	TGeoShape* shape = assembly->GetShape();
	Double_t boxSize[3] = {0};
	if (!shape->InheritsFrom(TGeoBBox::Class())){
		std::cout << "ERROR: Cannot determine assembly box size." << std::endl;
		return -1;
	}
	TGeoBBox* box = (TGeoBBox*)shape;
	boxSize[0] = box->GetDX();
	boxSize[1] = box->GetDY();
	boxSize[2] = box->GetDZ();
	std::cout << "Assembly half-size: " << boxSize[0] << "x" << boxSize[1] << "x" << boxSize[2] << std::endl;

	// Define vacuum medium and material
	TGeoMaterial *vacuumMaterial = new TGeoMaterial("vacuumMaterial", 0, 0, 0);
	TGeoMedium *vacuumMedium = new TGeoMedium("vacuumMedium", 1, vacuumMaterial);

	// Create top world volume
	// TGeoVolume *topVolume = gGeoManager->MakeBox("topVolume", vacuumMedium, 0, 0, 0);
	// TGeoVolume *topVolume = gGeoManager->MakeBox("topVolume", vacuumMedium, boxSize[0]*2, boxSize[1]*2, boxSize[2]*2);
	// TGeoVolume *topVolume = gGeoManager->MakeBox("topVolume", vacuumMedium, 500, 500, 500);
	// topVolume->AddNode(assembly, 1);

	// Petr Stepanov: child GeoVolumes have NULL fGeoManager

	gGeoManager = assembly->GetGeoManager();

	// gGeoManager->SetVisLevel(5);
	// gGeoManager->SetTopVolume(topVolume);
	// gGeoManager->SetTopVolume(assembly);
	gGeoManager->CloseGeometry();

	// Print child nodes of the PANDA assembly
	TObjArray* nodesArray = assembly->GetNodes();
	// Iterate child nodes
	std::cout << "Assembly contains " <<  nodesArray->GetEntries() << " entries." << std::endl;
	for (int i = 0; i < nodesArray->GetEntries(); i++){
		TGeoNode* node = assembly->GetNode(i);
		node->Print();
	}

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

	// Draw with regular ROOT 3D
	// gStyle->SetCanvasPreferGL();
	// topVolume->Draw("");

	// Draw with OpenGL
	// topVolume->Draw(drawOption);

	// Export to CAD
	// gSystem->Load("libGeoCad.so");
	// TGeoToStep *myStep = new TGeoToStep (gGeoManager);
	// myStep->CreateGeometry();

	// Export to GDML
	gGeoManager->Export("emc-panda.gdml", "topVolume");

	return 0;
}

void setGeoManagerRecursively(TGeoVolume* volume){

}
