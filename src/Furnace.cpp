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
  : volumeReader_(NULL), 
    sourceFolder_("NotSet"),
    destFolder_("NotSet") {
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

void Furnace::SetDestFolder(const std::string &_destFolder) {
  destFolder_ = _destFolder;
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

bool Furnace::ProcessFolder() {
  if (!volumeReader_) {
    std::cerr << "Cannot process folder without a reader" << std::endl;
    return false;
  }
  if (!volumeReader_->ProcessFolder(sourceFolder_, destFolder_)) {
    std::cerr << "Failed to process folder " << sourceFolder_ << std::endl;
  }
  return true;
}


bool Furnace::SetModelType(ModelType _modelType) {

  if (volumeReader_) {
    std::cout << "Warning: Model already set" << std::endl;
    delete volumeReader_;
  }

  if (_modelType == ENLIL) {
    volumeReader_ = ENLILReader::New();
  //} else if (_modelType == POINTCLOUD) {
  //  volumeReader_ = PointCloudReader::New();
  } else {
    std::cerr << "Unknown model type" << std::endl;
    return false;
  }

  return true;
}
