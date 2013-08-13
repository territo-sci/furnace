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

namespace osp {

class VDFData {
public:
  static VDFData * New();
  ~VDFData();
  void SetDataDimensionality(unsigned int _dataDimensionality);
  void SetNumTimesteps(unsigned int _numTimesteps);
  void SetDimensions(unsigned int _xDim,
                     unsigned int _yDim,
                     unsigned int _zDim);
  unsigned int DataDimensionality() const { return dataDimensionality_; }
  unsigned int NumTimesteps() const { return numTimesteps_; }
  unsigned int XDim() const { return xDim_; }
  unsigned int YDim() const { return yDim_; }
  unsigned int ZDim() const { return zDim_; }
	void SetMin(float _min);
	void SetMax(float _max);
	float Min() const { return min_; }
	float Max() const { return max_; }
  void Normalize();
	unsigned int NumVoxelsPerTimestep() const { return numVoxelsPerTimestep_; }
  // Return first index in data vector for a specific frame
	unsigned int TimestepOffset(unsigned int _timestep) const {
		return numVoxelsPerTimestep_ * _timestep;
	}
	unsigned int NumVoxelsTotal() const {
		return numVoxelsPerTimestep_ * numTimesteps_;
	}
  std::vector<float> * Data() { return &data_; }
private:
  // Constructor - make sure to give all values defaults!
  VDFData();
  // Data members
  unsigned int dataDimensionality_;
  unsigned int numTimesteps_;
  unsigned int xDim_;
  unsigned int yDim_;
  unsigned int zDim_;
  unsigned int numVoxelsPerTimestep_;
	float min_;
	float max_;
  std::vector<float> data_;
};

}

#endif /* VDFDATA_H_ */
