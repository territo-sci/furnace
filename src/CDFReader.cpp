/*
 * CDFReader.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: Victor Sand
 */

#include <CDFReader.h>
#include <VDFData.h>

using namespace osp;

CDFReader::CDFReader() :
  inFilename_("NotSet"),
  hasRead_(false),
  dataObject_(VDFData::New()) { }

CDFReader::~CDFReader()
{
  delete dataObject_;
}

void CDFReader::SetInFilename(std::string _inFilename)
{
  inFilename_ = _inFilename;
}


