/*
 * Furnace.h
 *
 *  Created on: Apr 5, 2013
 *      Author: vsand
 */

#ifndef FURNACE_H_
#define FURNACE_H_

#include <string>

namespace osp
{

class CDFReader;
class VDFWriter;

class Furnace
{
public:
  // List all the supported model types
  enum ModelType
  {
    NO_MODEL = 0,
    ENLIL
  };
  static Furnace * New();
  ~Furnace();
  // Setters for variables that are not read from file
  void SetModelType(ModelType _modelType);
  void SetInFilename(std::string _inFilename);
  void SetOutFilename(std::string _outFilename);
  void SetDimensions(unsigned int _xDim,
                     unsigned int _yDim,
                     unsigned int _zDim);
  // Methods
  bool Read();
  bool Write();
private:
  ModelType modelType_;
  Furnace();
  VDFWriter *VDFWriter_;
  CDFReader *CDFReader_;
};

}

#endif /* FURNACE_H_ */
