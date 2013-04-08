/*
 * CDFReader.h
 *
 *  Created on: Apr 5, 2013
 *      Author: Victor Sand
 *
 *  Base class for various readers. Each specific model needs its own
 *  subclass and implementation of Read(). The idea is to always write
 *  to a uniform VDF format. When adding new features to the VDF format,
 *  make sure to provide default values so that old readers do not break.
 *
 */

#ifndef CDFREADER_H_
#define CDFREADER_H_

#include <string>
#include <vector>

namespace osp
{

class VDFData;

class CDFReader
{
public:
  // Destructor
  virtual ~CDFReader();
  // Member functions
  virtual bool Read() = 0; // This is what sublasses need to implement
  // Setters for members that are not read from file
  void SetInFilename(std::string _inFilename);
  void SetDimensions(unsigned int _xDim,unsigned int _yDim,unsigned int _zDim);
  // Accessors
  VDFData * DataObject() { return dataObject_; }
  bool HasRead() const { return hasRead_; }

protected:
  // Constructor, takes care of VDFDataObject initialization
  CDFReader();
  // Data members
  std::string inFilename_;
  std::vector<std::string> variableNames_; // support combinations of variables
  VDFData * dataObject_;
  bool hasRead_; // Remember to have Read() implementations set this
};

}

#endif
