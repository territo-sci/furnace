/*
 * VDFData.h
 *
 *  Created on: Apr 8, 2013
 *      Author: vsand
 *
 *  Data container (and specification) for the VDF data
 *
 */



#ifndef VDFDATA_H_
#define VDFDATA_H_

#include <vector>

namespace osp
{

class VDFData
{
public:
  static VDFData * New();
  ~VDFData();
  // Setters
  void SetNrDimensions(unsigned int _nrDimensions);
  void SetNrTimesteps(unsigned int _nrTimesteps);
  void SetDimensions(unsigned int _xDim,
                     unsigned int _yDim,
                     unsigned int _zDim);
  // Accessors
  unsigned int NrDimensions() const { return nrDimensions_; }
  unsigned int NrTimesteps() const { return nrTimesteps_; }
  unsigned int XDim() const { return xDim_; }
  unsigned int YDim() const { return yDim_; }
  unsigned int ZDim() const { return zDim_; }
  unsigned int Size() const { return size_; }
  std::vector<float> * Data() { return &data_; }
private:
  // Constructor - make sure to give all values defaults!
  VDFData();
  // Data members
  unsigned int nrDimensions_;
  unsigned int nrTimesteps_;
  unsigned int xDim_;
  unsigned int yDim_;
  unsigned int zDim_;
  unsigned int size_; // Calculated from x, y and z dimensions
  std::vector<float> data_;
};

}

#endif /* VDFDATA_H_ */
