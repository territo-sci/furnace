//TODO Get rid of this file (or move it) and build Furnace like a proper lib

#include <Furnace.h>
#include <string>

int main()
{
  osp::Furnace *furnace = osp::Furnace::New();
  furnace->SetModelType(osp::Furnace::ENLIL);
  furnace->SetDimensions(128, 128, 128);
  furnace->SetInFilename("/home/mtornros/Documents/OpenSpace/data/Hong_Xie_120312_SH_1/Hong_Xie_120312_SH_1.enlil.0032.cdf");
  furnace->SetOutFilename("/home/vsand/OpenSpace/enlilTestData.vdf");
  furnace->Read();
  furnace->Write();
  delete furnace;
  return 0;
}
