/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 * Base class for volume readers. Sublasses need to implement read/write
 *
 */

#include <VolumeReader.h>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

using namespace osp;
namespace fs = boost::filesystem;

VolumeReader::VolumeReader() 
  : gridType_(1), 
    numTimesteps_(0),
    xDim_(0), 
    yDim_(0), 
    zDim_(0),
    numVoxelsPerTimestep_(0),
    min_(1e20f), 
    max_(-1e20f) {
}

VolumeReader::~VolumeReader() {
}

void VolumeReader::SetDimensions(unsigned int _xDim, 
                                 unsigned int _yDim,
                                 unsigned int _zDim) {
  xDim_ = _xDim;
  yDim_ = _yDim;
  zDim_ = _zDim;
  numVoxelsPerTimestep_ = xDim_*yDim_*zDim_;
}

void VolumeReader::SetGridType(unsigned int _gridType) {
  gridType_ = _gridType;
}


bool VolumeReader::WriteHeader(const std::string &_destFolder) {

  // Create folder if it doesn't exist
  fs::path dir(_destFolder);
  if (!fs::exists(dir)) {
    if (!fs::create_directory(dir)) {
      std::cerr << "Failed to create directory" << std::endl;
      return false;
    }
  }

  std::string fullPath = _destFolder + headerFilename_ + headerSuffix_;

  std::fstream out;
  out.open(fullPath.c_str(),
    std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if (!out.is_open()) {
    std::cerr << "Failed to open " << fullPath << std::endl;
    return false;
  }

  std::cout << "Writing data: " << std::endl;
  std::cout << "Grid type: " << gridType_ << std::endl;
  std::cout << "Num timesteps: " << numTimesteps_ << std::endl;
  std::cout << "Dimensions: "<<xDim_<< " " << yDim_<< " " << zDim_<< std::endl;

  out.write(reinterpret_cast<char*>(&gridType_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&numTimesteps_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&xDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&yDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&zDim_), sizeof(unsigned int));

  out.close();

  std::cout << "Header written to " << fullPath << std::endl;

  return true;
}


bool VolumeReader::WriteFinal(const std::string &_destFolder) {

  std::cout << "Assembling final output" << std::endl;

  std::string fullPath = _destFolder + finalFilename;

  std::fstream out;
  out.open(fullPath.c_str(),
    std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if (!out.is_open()) {
    std::cerr << "Failed to open " << fullPath << std::endl;
    return false;
  }

  size_t fileSize;
  char * buffer;

  // Look for header and read it
  std::string headerPath = _destFolder + headerFilename_ + headerSuffix_;
  { // scope fstream 
  std::fstream in;
  in.open(headerPath.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!in.is_open()) {
    std::cerr << "Failed to open " << headerPath << std::endl;
    return false;
  }
  in.seekg(0, std::ios::end);
  fileSize = in.tellg();
  buffer = new char[fileSize];
  in.seekg(0, std::ios::beg);
  in.read(buffer, fileSize);
  in.close();
  }

  out.write(buffer, fileSize);
  delete[] buffer;

  // Read timesteps
  // Process each file in turn
  float * floatBuffer = new float[numVoxelsPerTimestep_];
  for (unsigned int i=0; i<numTimesteps_; ++i) {


    std::string timestepStr = boost::lexical_cast<std::string>(i);
    std::string timestepPath = _destFolder + timestepFilename_ + timestepStr +
      timestepSuffix_;
    std::fstream in;
    in.open(timestepPath.c_str(), std::ios_base::in | std::ios_base::binary);
    in.seekg(0, std::ios::beg);
    if (!in.is_open()) {
      std::cerr << "Failed to open " << timestepPath << std::endl;
      return false;
    }

    in.read(reinterpret_cast<char*>(&floatBuffer[0]), 
            numVoxelsPerTimestep_*sizeof(float));
    in.close();


    // Normalize
    for (unsigned int j=0; j<numVoxelsPerTimestep_; ++j) {
      floatBuffer[j] = (floatBuffer[j] - min_)/(max_ - min_);
    }

    out.write(reinterpret_cast<char*>(&floatBuffer[0]), 
              numVoxelsPerTimestep_*sizeof(float));
  }

  delete[] floatBuffer;

  out.close();

  return true;
}

bool VolumeReader::DeleteTempFiles(const std::string &_destFolder) {

  std::cout << "Deleting temporary files" << std::endl;

  // Iterate over directory, delete if temp file
  for (fs::directory_iterator it(_destFolder);
       it!=fs::directory_iterator(); ++it) {
    std::string filename = it->path().string();
    if (filename.find_last_of(".") != std::string::npos) {
      if (filename.substr(filename.find_last_of(".")) == tempSuffix_) {
        fs::remove_all(filename);
      }
    }
  }

  return true;
}


/*
bool VolumeReader::Write(const std::string &_outFilename) {
  
  // Do some checks
  if (!hasRead_) {
    std::cerr << "Cannot write without reading first" << std::endl;
    return false;
  }

  if (xDim_ == 0 || yDim_ == 0 || zDim_ == 0) {
    std::cerr << "One or more dimensions == 0" << std::endl;
    return false;
  }

  // Write
  std::fstream out;
  out.open(_outFilename.c_str(), 
           std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if (!out.is_open()) {
    std::cerr << "Failed to open " << _outFilename << std::endl;
    return false;
  }

  std::cout << "Writing data: " << std::endl;
  std::cout << "Grid type: " << gridType_ << std::endl;
  std::cout << "Num timesteps: " << numTimesteps_ << std::endl;
  std::cout << "Dimensions: "<<xDim_<< " " << yDim_<< " " << zDim_<< std::endl;

  out.write(reinterpret_cast<char*>(&gridType_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&numTimesteps_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&xDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&yDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&zDim_), sizeof(unsigned int));
  out.write(reinterpret_cast<char*>(&data_[0]), 
    sizeof(float)*numTimesteps_*numVoxelsPerTimestep_);

  out.close();

  std::cout << "Written to " << _outFilename << " succesfully!" << std::endl;

  return true; 
}
*/



