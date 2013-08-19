/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 */

#include <PointCloudReader.h>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

using namespace osp;
namespace fs = boost::filesystem;

PointCloudReader::PointCloudReader() 
  : numParticlesPerTimestep_(0.f),
    timestepDt_(0.f) {
}

PointCloudReader * PointCloudReader::New() {
  return new PointCloudReader();
}

PointCloudReader::~PointCloudReader() {
}

bool PointCloudReader::ReadFolder(const std::string &_sourceFolder) {
  
  // Check dimensions
  if (xDim_ == 0 || yDim_ == 0 || zDim_ == 0) {
    std::cerr << "One or more dimensions = 0" << std::endl;
    return false;
  }

  // See if path exists
  if (!fs::exists(_sourceFolder)) {
    std::cerr << "The path " << _sourceFolder<<" does not exist"<<std::endl;
    return false;
  }

  // See if path is a directory
  if (!fs::is_directory(_sourceFolder)) {
    std::cerr <<"The path "<< _sourceFolder<< " is not a directory"<<std::endl;
    return false;
  }

  // Get filename and check that there is only one file present
  unsigned int numFiles = 0;
  for (fs::directory_iterator it(_sourceFolder);
       it!=fs::directory_iterator(); ++it) {
    fileName_ = it->path().string();
    numFiles++;
  }

  if (numFiles != 1) {
    std::cerr<<"Error: Found " << numFiles << " files, must be one"<<std::endl;
    return false;
  }

  std::cout << "Found file: " << fileName_ << std::endl;

  // Open and read file meta data
  std::fstream in;
  in.open(fileName_.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!in.is_open()) {
    std::cerr << "Failed to open file: " << fileName_ << std::endl;
    return false;
  }

  // Original number of timesteps
  // Must be modifies for power ow two requirement
  // TODO: Remove power of two requirement
  int numTimestepsOrig = 0;
  int numParticlesPerTimestep = 0;
  in.read(reinterpret_cast<char*>(&numTimestepsOrig), sizeof(int));
  in.read(reinterpret_cast<char*>(&numParticlesPerTimestep), sizeof(int));
  numParticlesPerTimestep_=static_cast<unsigned int>(numParticlesPerTimestep);
  in.read(reinterpret_cast<char*>(&timestepDt_), sizeof(float));

  std::cout << "Num timesteps in file: " << numTimestepsOrig << std::endl;
  std::cout << "Num particles per timestep: " << numParticlesPerTimestep_ <<
    std::endl;
  std::cout << "Timestep dt: " << timestepDt_ << std::endl;

  in.close();

  return true;
}



