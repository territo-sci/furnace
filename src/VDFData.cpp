/*
 * VDFData.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: vsand
 */

#include <VDFData.h>
#include <iostream>

using namespace osp;

VDFData * VDFData::New() {
  return new VDFData();
}

VDFData::VDFData() :
  dataDimensionality_(1),
  numTimesteps_(1),
  xDim_(0),
  yDim_(0),
  zDim_(0),
	numVoxelsPerTimestep_(0) {}

VDFData::~VDFData() { }

void VDFData::SetDataDimensionality(unsigned int _dataDimensionality) {
  dataDimensionality_ = _dataDimensionality;
}

void VDFData::SetNumTimesteps(unsigned int _numTimesteps) {
  numTimesteps_ = _numTimesteps;
	if (xDim_ == 0 || yDim_ == 0 || zDim_ == 0) {
		std::cout << "WARNING: Call SetDimensions() before SetNumTimesteps()\n";
	}
  std::cout << "Allocating storage" << std::endl;
  std::cout << "Num timesteps: " << numTimesteps_ << std::endl;
  std::cout << "Num voxels/timestep: " << numVoxelsPerTimestep_<<std::endl;
  std::cout << "Size: " << numTimesteps_*numVoxelsPerTimestep_ << std::endl;
	data_.resize(numTimesteps_*numVoxelsPerTimestep_);
  std::cout << "Allocated storage OK" << std::endl;
  std::cout << "Actual size: " << data_.size() << std::endl;
}

void VDFData::SetDimensions(unsigned int _xDim,
                            unsigned int _yDim,
                            unsigned int _zDim) {
  xDim_ = _xDim;
  yDim_ = _yDim;
  zDim_ = _zDim;
  numVoxelsPerTimestep_ = _xDim*_yDim*_zDim;
}

void VDFData::SetMin(float _min) {
  if (_min < min_) {
	  min_ = _min;
  }
}

void VDFData::SetMax(float _max) {
  if (_max > max_) {
  	max_ = _max;
  }
}

void VDFData::Normalize() {
  float min = 999.f;
  float max = -999.f;
  for (std::vector<float>::iterator it=data_.begin(); it!=data_.end(); ++it) {
    *it = (*it-min_)/(max_-min_);
    if (*it < min) {
      min = *it;
    } else if (*it > max) {
      max = *it;
    }
  }
  std::cout << "min norm " << min << std::endl;
  std::cout << "max norm " << max << std::endl;
}
