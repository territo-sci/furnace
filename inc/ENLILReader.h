/*
 * Author: Victor Sand (victor.sand@gmail.com)
 * Example implementation of a VolumeReader
 * Reads ENLIL data from CDF files
 *
 */

#ifndef ENLILREADER_H_
#define ENLILREADER_H_

#include <VolumeReader.h>

namespace ccmc {
  class Kameleon;
  class Interpolator;
}

namespace osp {

class ENLILReader : public VolumeReader {
public:
  // Initializes Kameleon and Interpolator objects, returns ENLILReader
  static ENLILReader * New();
  ~ENLILReader();

  // Read a folder with one CDF file per timestep
  virtual bool ReadFolder(const std::string &_sourceFolder);

private:
  ENLILReader();
  ENLILReader(ccmc::Kameleon *_kameleon);
  ENLILReader(const ENLILReader&);

  // Read individual timestep (called by ReadFolder)
  bool ReadFile(const std::string &_filename, unsigned int _timestep);

  // CCMC parts
  ccmc::Kameleon *kameleon_;
  // Interpolator can't be created until file has been opened!
  ccmc::Interpolator *interpolator_;

};

} // namespace osp

#endif


