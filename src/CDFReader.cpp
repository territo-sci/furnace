/*
 * CDFReader.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: Victor Sand
 */

#include <CDFReader.h>
#include <VDFData.h>
#include <set>
#include <iostream>
#include <boost/filesystem.hpp>
#include <ccmc/Kameleon.h>

using namespace osp;
namespace fs = boost::filesystem;

CDFReader::CDFReader()
    : hasRead_(false)
    , dataObject_(VDFData::New()) {
    kameleon_ = new ccmc::Kameleon();
    interpolator_ = NULL;
}

CDFReader::~CDFReader() {
    delete dataObject_;
    delete kameleon_;
    if (interpolator_ != NULL) {
        delete interpolator_;
    }
}

void CDFReader::SetPath(const std::string &_path) {
    path_ = _path;
}

bool CDFReader::ReadFolder() {

    // See if path exists
    if (!fs::exists(path_)) {
        std::cout << "The path " << path_ << " does not exist\n";
        return false;
    }

    // See if path is a directory
    if (!fs::is_directory(path_)) {
        std::cout << "The path " << path_ << " is not a directory\n";
        return false;
    }

    // Insert the filenames in folder into a set to keep them ordered
    std::set<fs::path> filenames_;
    for (fs::directory_iterator it(path_);
         it != fs::directory_iterator();
         it++) {
        filenames_.insert(it->path());
    }

    // Set number of timesteps (number of files in folder)
    size_t numTimesteps = (size_t) filenames_.size();
    dataObject_->SetNumTimesteps(numTimesteps);

    // Read the individual timesteps
    size_t timestep = 0;
    for (std::set<fs::path>::iterator it = filenames_.begin();
         it != filenames_.end();
         it++) {
        if (!ReadFile(it->string(), timestep)) return false;
        timestep++;
    }

    // Normalize
    // dataObject_->Normalize();

    return true;

}	



