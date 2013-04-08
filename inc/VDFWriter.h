/*
 * VDFWriter.h
 *
 *  Created on: Apr 5, 2013
 *      Author: Victor Sand
 *
 *  Writes a VDF data object to file.
 *
 */

#ifndef VDFWRITER_H_
#define VDFWRITER_H_

#include <string>

namespace osp
{

class VDFData;

class VDFWriter
{
public:
  static VDFWriter * New();
  ~VDFWriter();
  void SetOutFilename(std::string _outFilename);
  void SetDataObject(VDFData * _dataObject);
  bool Write(); // Write to output file
private:
  VDFWriter();
  std::string outFilename_;
  VDFData * dataObject_;
};

}

#endif /* VDFWRITER_H_ */
