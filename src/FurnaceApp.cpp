//TODO Get rid of this file (or move it) and build Furnace like a proper lib

#include <Furnace.h>
#include <string>

#include <ccmc/Kameleon.h>

int main()
{
  osp::Furnace *furnace = osp::Furnace::New();
  furnace->SetModelType(osp::Furnace::ENLIL);
  furnace->SetDimensions(128, 128, 128);
	furnace->SetPath("/home/vsand/OpenSpace/CDFData/testthree/");
  furnace->SetOutFilename("/home/vsand/OpenSpace/enlilTestData.vdf");
  furnace->ReadFolder();
  furnace->Write();
  delete furnace;
  return 0;
}
