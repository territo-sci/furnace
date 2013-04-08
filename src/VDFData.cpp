/*
 * VDFData.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: vsand
 */

#include <VDFData.h>

using namespace osp;

VDFData * VDFData::New()
{
  return new VDFData();
}

VDFData::VDFData() :
  nrDimensions_(1),
  nrTimesteps_(1),
  xDim_(0),
  yDim_(0),
  zDim_(0),
  size_(0),
  data_() { }

VDFData::~VDFData()
{ }

void VDFData::SetNrDimensions(unsigned int _nrDimensions)
{
  nrDimensions_ = _nrDimensions;
}

void VDFData::SetNrTimesteps(unsigned int _nrTimesteps)
{
  nrTimesteps_ = _nrTimesteps;
}

void VDFData::SetDimensions(unsigned int _xDim,
                            unsigned int _yDim,
                            unsigned int _zDim)
{
  xDim_ = _xDim;
  yDim_ = _yDim;
  zDim_ = _zDim;
  size_ = _xDim*_yDim*_zDim;
}


