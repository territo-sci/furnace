/*
 * Author: Victor Sand (victor.sand@gmail.com);
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

protected:
  VolumeReader();
  VolumeReader(const VolumeReader&);

  // State
  bool hasRead_;

  // Data to write
  unsigned int dataDimensionality_; // Not currently used (always 1)
  unsigned int numTimesteps_;
  unsigned int xDim_;
  unsigned int yDim_;
  unsigned int zDim_;
  unsigned int numVoxelsPerTimestep_;
  std::vector<float> data_;

  // If the data needs to be normalized
  float min_;
  float max_;
};

} // namespace

#endif
