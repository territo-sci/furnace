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
	data_.resize(numTimesteps_*numVoxelsPerTimestep_);
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
	min_ = _min;
}

void VDFData::SetMax(float _max) {
	max_ = _max;
}
