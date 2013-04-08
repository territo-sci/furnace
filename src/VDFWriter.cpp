/*
 * VDFWriter.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: Victor Sand
 */

#include <VDFWriter.h>
#include <VDFData.h>
#include <iostream>
#include <fstream>

using namespace osp;

VDFWriter * VDFWriter::New()
{
  return new VDFWriter();
}

VDFWriter::VDFWriter() : outFilename_("NotSet"),
                         dataObject_(NULL) { }

VDFWriter::~VDFWriter()
{
  // Reader takes care of deleting the data object
}

void VDFWriter::SetOutFilename(std::string _outFilename)
{
  outFilename_ = _outFilename;
}

void VDFWriter::SetDataObject(VDFData *_dataObject)
{
  dataObject_ = _dataObject;
}

bool VDFWriter::Write()
{

  if (dataObject_->XDim() == 0 &&
      dataObject_->YDim() == 0 &&
      dataObject_->ZDim() == 0)
  {
    std::cout << "VDFWriter::Write(): All dimensions are zero\n";
  }

  // TODO is there a way to not hardcode the variables?
  std::fstream out;
  out.open(outFilename_.c_str(),
           std::ios_base::binary |
           std::ios_base::in |
           std::ios_base::out |
           std::ios_base::trunc);
  if (out.fail())
  {
    std::cout << "Failed to open " << outFilename_ << " stream\n";
    return false;
  }
  unsigned int nrDimensions = dataObject_->NrDimensions();
  unsigned int nrTimesteps = dataObject_->NrTimesteps();
  unsigned int xDim = dataObject_->XDim();
  unsigned int yDim = dataObject_->YDim();
  unsigned int zDim = dataObject_->ZDim();
  float *data = &(dataObject_->Data()->at(0));
  out.seekp(std::ios_base::beg);
  out.write((char*)&nrDimensions, sizeof(unsigned int));
  out.write((char*)&nrTimesteps, sizeof(unsigned int));
  out.write((char*)&xDim, sizeof(unsigned int));
  out.write((char*)&yDim, sizeof(unsigned int));
  out.write((char*)&zDim, sizeof(unsigned int));
  out.write((char*)data, sizeof(float)*dataObject_->Size());
  out.close();
  std::cout << outFilename_ << " written successfully\n";
  return true;
}
