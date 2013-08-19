/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 */

#include <Furnace.h>
#include <VolumeReader.h>
#include <ENLILReader.h>
#include <PointCloudReader.h>
#include <iostream>

using namespace osp;

Furnace::Furnace() 
  : hasRead_(false), 
    volumeReader_(NULL), 
    sourceFolder_("NotSet"), 
    outFilename_("NotSet") {
}

Furnace::~Furnace() {
  if (volumeReader_) delete volumeReader_;
}

Furnace * Furnace::New() {
  return new Furnace();
}

void Furnace::SetSourceFolder(const std::string &_sourceFolder) {
  sourceFolder_ = _sourceFolder;
}

void Furnace::SetOutFilename(const std::string &_outFilename) {
  outFilename_ = _outFilename;
}

void Furnace::SetGridType(GridType _gridType) {
  volumeReader_->SetGridType(static_cast<unsigned int>(_gridType));
}

void Furnace::SetDimensions(unsigned int _xDim,
                            unsigned int _yDim,
                            unsigned int _zDim) {
  if (!volumeReader_) {
    std::cerr << "Trying to set dimensions without a reader" << std::endl;
  } else {
    volumeReader_->SetDimensions(_xDim, _yDim, _zDim);
  }
}

bool Furnace::ReadFolder() {
  if (!volumeReader_) {
    std::cerr << "Cannot read folder without a reader" << std::endl;
    return false;
  }
  if (!volumeReader_->ReadFolder(sourceFolder_)) {
    std::cerr << "Failed to read folder " << sourceFolder_ << std::endl;
  }
  hasRead_ = true;
  return true;
}

bool Furnace::Write() {
  if (!hasRead_) {
    std::cerr << "Can't write before reading" << std::endl;
    return false;
  }
  return volumeReader_->Write(outFilename_);
}

bool Furnace::SetModelType(ModelType _modelType) {

  if (volumeReader_) {
    std::cout << "Warning: Model already set" << std::endl;
    delete volumeReader_;
  }

  if (_modelType == ENLIL) {
    volumeReader_ = ENLILReader::New();
  } else if (_modelType == POINTCLOUD) {
    volumeReader_ = PointCloudReader::New();
  } else {
    std::cerr << "Unknown model type" << std::endl;
    return false;
  }

  return true;
}
