/*
 * Furnace.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: vsand
 */

#include <Furnace.h>
#include <VDFData.h>
#include <CDFReader.h>
#include <ENLILReader.h>
#include <VDFWriter.h>
#include <iostream>

using namespace osp;

Furnace * Furnace::New()
{
  return new Furnace();
}

Furnace::Furnace() : VDFWriter_(VDFWriter::New()),
                     CDFReader_(NULL),
                     modelType_(Furnace::NO_MODEL) { }

Furnace::~Furnace()
{
  if (VDFWriter_ != NULL) delete VDFWriter_;
  if (CDFReader_ != NULL) delete CDFReader_;
}

void Furnace::SetModelType(ModelType _modelType)
{
  modelType_ = _modelType;
  // Clean up any old readers
  if (CDFReader_ != NULL) delete CDFReader_;
  // Set the new reader
  switch (_modelType)
  {
    case ENLIL:
      CDFReader_ = ENLILReader::New();
      break;
    default:
      std::cout << "SetModelType(): Invalid type\n";
      break;
  }
}

void Furnace::SetInFilename(std::string _inFilename)
{
  if (CDFReader_ != NULL)
  {
    CDFReader_->SetInFilename(_inFilename);
  }
  else
  {
    std::cout << "SetInFilename(): CDFReader not set\n";
  }
}

void Furnace::SetOutFilename(std::string _outFilename)
{
  if (VDFWriter_ == NULL)
  {
    std::cout << "Furnace::Read(): VDFWriter not set\n";
  }
  VDFWriter_->SetOutFilename(_outFilename);
}

void Furnace::SetDimensions(unsigned int _xDim,
                            unsigned int _yDim,
                            unsigned int _zDim)
{
  CDFReader_->DataObject()->SetDimensions(_xDim, _yDim, _zDim);
}

bool Furnace::Read()
{
  if (modelType_== Furnace::NO_MODEL)
  {
    std::cout << "Furnace::Read(): Model type not set\n";
    return false;
  }
  if (CDFReader_ == NULL)
  {
    std::cout << "Furnace::Reader() : CDFReader is NULL\n";
    return false;
  }
  return CDFReader_->Read();
}

bool Furnace::Write()
{
  if (VDFWriter_ == NULL)
  {
    std::cout << "Furnace::Write(): VDFWriter not set\n";
    return false;
  }
  VDFWriter_->SetDataObject(CDFReader_->DataObject());
  return VDFWriter_->Write();
}
