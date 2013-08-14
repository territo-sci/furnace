//TODO Get rid of this file (or move it) and build Furnace like a proper lib

#include <Furnace.h>
#include <string>

#include <ccmc/Kameleon.h>

int main() {
  osp::Furnace *furnace = osp::Furnace::New();
  furnace->SetModelType(osp::Furnace::ENLIL);
  furnace->SetDimensions(512, 512, 512);
	furnace->SetPath("/home/vsand/OpenSpace/CDFData/test_32/");
  furnace->SetOutFilename("/home/vsand/OpenSpace/enlil_512_32_sph_rho.vdf");
  furnace->ReadFolder();
  furnace->Write();
  delete furnace;
  return 0;
}
