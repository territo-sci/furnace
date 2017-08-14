/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 */

#include <Furnace.h>
#include <VolumeProcessor.h>
#include <ENLILProcessor.h>
#include <MLProcessor.h>
#include <iostream>
#include <unordered_map>
#include <BATSRUSProcessor.h>


using namespace osp;

const std::unordered_map<std::string, Furnace::ModelType> Furnace::MODEL_TYPES = {
        {"ENLIL", ModelType::ENLIL},
        {"ML", ModelType::ML},
        {"BATSRUS", ModelType::BATSRUS}
};

Furnace::Furnace()
    : volumeProcessor_(nullptr)
    , sourceFolder_("NotSet")
    , destFolder_("NotSet") { }

Furnace::~Furnace() {
    delete volumeProcessor_;
}

Furnace *Furnace::New() {
    return new Furnace();
}

void Furnace::SetSourceFolder(const std::string &_sourceFolder) {
    sourceFolder_ = _sourceFolder;
}

void Furnace::SetDestFolder(const std::string &_destFolder) {
    destFolder_ = _destFolder;
}

void Furnace::SetGridType(GridType _gridType) {
    volumeProcessor_->SetGridType(static_cast<size_t>(_gridType));
}

void Furnace::SetDimensions(size_t _xDim,
                            size_t _yDim,
                            size_t _zDim) {
    if (!volumeProcessor_) {
        std::cerr << "Trying to set dimensions without a reader" << std::endl;
    } else {
        volumeProcessor_->SetDimensions(_xDim, _yDim, _zDim);
    }
}

bool Furnace::ProcessFolder() {
    if (!volumeProcessor_) {
        std::cerr << "Cannot process folder without a reader" << std::endl;
        return false;
    }
    if (!volumeProcessor_->ProcessFolder(sourceFolder_, destFolder_)) {
        std::cerr << "Failed to process folder " << sourceFolder_ << std::endl;
    }
    return true;
}


bool Furnace::SetModelType(ModelType _modelType) {

    if (volumeProcessor_) {
        std::cout << "Warning: Model already set" << std::endl;
        delete volumeProcessor_;
    }
    switch(_modelType) {
    case ENLIL:
        volumeProcessor_ = ENLILProcessor::New();
        break;
    case ML:
        volumeProcessor_ = MLProcessor::New();
        break;
    case BATSRUS:
        volumeProcessor_ = BATSRUSProcessor::New();
        break;
    default:
        std::cerr << "Unknown model type" << std::endl;
        return false;
    }

    return true;
}
