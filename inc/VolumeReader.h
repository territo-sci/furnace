/*
 * Author: Victor Sand (victor.sand@gmail.com);
 *
 * Base class for reading (and later writing) volume data. The function
 * ReadFolder() needs to be implemented in subclasses, and the standard Write()
 * function may be overridden if specializations are needed in the output
 * file format. 
 * 
 * ReadFolder() reads a folder with one or more files of source data.
 *
 * Note that this class also defines the file format!
 *
 */

#ifndef VOLUMEREADER_H_
#define VOLUMEREADER_H_

#include <string>
#include <vector>

namespace osp {

class VolumeReader {
public:

  virtual ~VolumeReader();

  // Read/write functions for subclasses to implement
  // Read contents of a given folder
  virtual bool ReadFolder(const std::string &_sourceFolder) = 0;
  // Write read data to specified output file
  // May be overridden if needed
  virtual bool Write(const std::string &_outFilename);

  void SetDimensions(unsigned int _xDim,
                     unsigned int _yDim,
                     unsigned int _zDim);

  // 0 for cartesian
  // 1 for spherical
  void SetGridType(unsigned int _gridType);

protected:
  VolumeReader();
  VolumeReader(const VolumeReader&);

  // Data to write
  unsigned int gridType_; 
  unsigned int numTimesteps_;
  unsigned int xDim_;
  unsigned int yDim_;
  unsigned int zDim_;
  std::vector<float> data_;

  // Num voxels for one frame
  unsigned int numVoxelsPerTimestep_;

  // For normalization etc
  float min_;
  float max_;
  
  // State
  bool hasRead_;
};

} // namespace

#endif
