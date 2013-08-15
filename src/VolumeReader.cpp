/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 * Base class for volume readers. Sublasses need to implement read/write
 *
 */

#include <VolumeReader.h>
#include <iostream>
#include <fstream>

using namespace osp;

VolumeReader::VolumeReader() 
  : dataDimensionality_(1), 
    numTimesteps_(0),
    xDim_(0), 
    yDim_(0), 
    zDim_(0),
    numVoxelsPerTimestep_(0),
    min_(1e20f), 
    max_(-1e20f) {
}

VolumeReader::~VolumeReader() {
}

void VolumeReader::SetDimensions(unsigned int _xDim, 
                                 unsigned int _yDim,
                                 unsigned int _zDim) {
  xDim_ = _xDim;
  yDim_ = _yDim;
  zDim_ = _zDim;
  numVoxelsPerTimestep_ = xDim_*yDim_*zDim_;
}

bool VolumeReader::Write(const std::string &_outFilename) {
  
  // Do some checks
  if (!hasRead_) {
    std::cerr << "Cannot write without reading first" << std::endl;
    return false;
  }

  if (xDim_ == 0 || yDim_ == 0 || zDim_ == 0) {
    std::cerr << "One or more dimensions == 0" << std::endl;
    return false;
  }

  // Write
  std::fstream out;
  out.open(_outFilename.c_str(), 
           std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if (!out.is_open()) {
    std::cerr << "Failed to open " << _outFilename << std::endl;
    return false;
  }

  out.write(reinterpret_cast<char*>(&dataDimensionality_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&numTimesteps_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&xDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&yDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&zDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&data_[0]), 
    sizeof(float)*numTimesteps_*numVoxelsPerTimestep_);

  out.close();

  std::cout << "Written to " << _outFilename << " succesfully!" << std::endl;

  return true; 
}




