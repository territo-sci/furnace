/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 */

#include <ENLILProcessor.h>
#include <ccmc/Kameleon.h>
#include <iostream>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <set>

using namespace osp;
namespace fs = boost::filesystem;

ENLILProcessor::ENLILProcessor(ccmc::Kameleon *_kameleon)
  : VolumeProcessor(), 
    kameleon_(_kameleon), interpolator_(NULL) {
}

ENLILProcessor * ENLILProcessor::New() {
  ccmc::Kameleon *kameleon = new ccmc::Kameleon();
  if (!kameleon) {
    std::cerr << "Failed to create Kameleon instance" << std::endl;
    return NULL;
  }
  return new ENLILProcessor(kameleon);
}

ENLILProcessor::~ENLILProcessor() {
  if (kameleon_) delete kameleon_;
  if (interpolator_) delete interpolator_;
}

bool ENLILProcessor::ProcessFolder(const std::string &_sourceFolder,
                                const std::string &_destFolder) {
  
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

  // Insert the filenames in folder into a set to keep them ordered
  std::set<fs::path> filenames;
  for (fs::directory_iterator it(_sourceFolder);
       it!=fs::directory_iterator(); ++it) {
    filenames.insert(it->path());
  }

  // Check dimensions
  if (xDim_ == 0 || yDim_ == 0 || zDim_ == 0) {
    std::cerr << "One or more dimensions = 0" << std::endl;
    return false;
  }

  // Set number of timesteps (number of files in folder)
  numTimesteps_ = static_cast<unsigned int>(filenames.size());
  std::cout << "Found " << numTimesteps_ << " timesteps" << std::endl;
  // Allocate space for one timestep
  size_t nvpts = static_cast<size_t>(numVoxelsPerTimestep_);
  data_.resize(nvpts);

  if (numTimesteps_ % 2 != 0) {
    std::cerr << "\nWARNING: Number of timesteps is not even!\n" << std::endl;
  }

  // Write header file
  if (!WriteHeader(_destFolder)) {
    std::cerr << "Failed to write header" << std::endl;
    return false;
  }

  // process the individual timesteps
  unsigned int timestep = 0;
  for (auto it=filenames.begin(); it!=filenames.end(); ++it) {
    if (!ProcessFile(it->string(), _destFolder, timestep++)) {
      std::cerr << "Failed to read timestep " << timestep << std::endl;
      return false;
    }
  }

  // Gather temp files, normalize and write to single output
  if (!WriteFinal(_destFolder)) {
    std::cerr << "Failed to write final file" << std::endl;
    return false;
  }

  // Delete temp files
  DeleteTempFiles(_destFolder);

  // Normalize
  //std::cout << "Normalizing" << std::endl;
  //for (auto it=data_.begin(); it!=data_.end(); ++it) {
  //  *it = (*it - min_)/(max_ - min_);
  //  }

  std::cout << "Processing complete" << std::endl;

  return true;
}



bool ENLILProcessor::ProcessFile(const std::string &_filename, 
                              const std::string &_destFolder,
                              unsigned int _timestep) {

  std::string timestepStr = boost::lexical_cast<std::string>(_timestep);
  std::string fullPath = _destFolder + timestepFilename_ + timestepStr +
    timestepSuffix_;

  // If file already exists (eg left from aborted run), skip it
  if (fs::exists(fullPath)) {
    std::cout << fullPath << " already exists" << std::endl;
    return true;
  }


  // Open file using Kameleon instance
  if (kameleon_->open(_filename) != ccmc::FileReader::OK) {
    std::cerr << "Failed to open " << _filename << std::endl;
    return false;
  }

  // Create interpolator (has to be done after file is opened)
  interpolator_ = kameleon_->createNewInterpolator();

  if (!interpolator_) {
    std::cerr << "Failed to create interpolator" << std::endl;
    return false;
  }

  // Get variable limits from model
  float rMin =
    kameleon_->getVariableAttribute("r", "actual_min").getAttributeFloat();
  float rMax =
    kameleon_->getVariableAttribute("r", "actual_max").getAttributeFloat();
  float thetaMin =
    kameleon_->getVariableAttribute("theta", "actual_min").getAttributeFloat();
  float thetaMax =
    kameleon_->getVariableAttribute("theta", "actual_max").getAttributeFloat();
  float phiMin =
    kameleon_->getVariableAttribute("phi", "actual_min").getAttributeFloat();
  float phiMax =
    kameleon_->getVariableAttribute("phi", "actual_max").getAttributeFloat();

  // Loop over volume
  // [x, y, z] -> [r, theta, phi] for spherical model
  // TODO: Parallelize
  for (unsigned int phi=0; phi<zDim_; ++phi) {
    unsigned int progress = (unsigned int)(((float)phi/(float)zDim_)*100.f);
    if (progress % 10 == 0) {
      std::cout << "Processing timestep " << _timestep+1 << "/" 
        << numTimesteps_ << ", " << progress << "%\r" << std::flush;
    }
    for (unsigned int theta=0; theta<yDim_; ++theta) {
      for (unsigned int r=0; r<xDim_; ++r) {
  
        // Calculate array index
        unsigned int index = r + theta*xDim_ + phi*xDim_*yDim_;

        // Put r in the [0..sqrt(3)] range
        float rNorm = sqrt(3.0)*(float)r/(float)(xDim_-1);

        // Put theta in the [0..PI] range
        float thetaNorm = M_PI*(float)theta/(float)(yDim_-1);

        // Put phi in the [0..2PI] range
        float phiNorm = 2.0*M_PI*(float)phi/(float)(zDim_-1);

        // Go to physical coordinates before sampling
        float rPh = rMin + rNorm*(rMax-rMin);
        float thetaPh = thetaNorm;
        //phi range needs to be mapped to the slightly different
        // model range to avoid gaps in the data
        // Subtract a small term to avoid rounding errors when comparing
        // to phiMax.
        float phiPh = phiMin + phiNorm/(2.0*M_PI)*(phiMax-phiMin-0.000001);

        // Hardcoded variables (rho or rho - rho_back)
        // TODO Don't hardcode, make more flexible
        float rho = 0.f, rho_back = 0.f, diff = 0.f;
        // See if sample point is inside domain
        if (rPh < rMin || rPh > rMax || thetaPh < thetaMin ||
            thetaPh > thetaMax || phiPh < phiMin || phiPh > phiMax) {
          if (phiPh > phiMax) {
            std::cout << "Warning: There might be a gap in the data\n";
          }
          // Leave values at zero if outside domain
        } else { // if inside

          // ENLIL CDF specific hacks!
          // Convert from meters to AU for interpolator
          rPh /= ccmc::constants::AU_in_meters;
          // Convert from colatitude [0, pi] rad to latitude [-90, 90] degrees
          thetaPh = -thetaPh*180.f/M_PI+90.f;
          // Convert from [0, 2pi] rad to [0, 360] degrees
          phiPh = phiPh*180.f/M_PI;
          // Sample
          rho = interpolator_->interpolate("rho", rPh, thetaPh, phiPh);
          rho_back = interpolator_->interpolate("rho-back",rPh,thetaPh,phiPh);

          // Calculate difference (or just rho)
          //diff = rho;
          diff = rho - rho_back;

          // Clamp to 0
          if (diff < 0.f) diff = 0.f;
        }

        // Update min/max
        if (diff > max_) { 
          max_ = diff;
        } else if (diff < min_) {
          min_ = diff;
        }

        data_[index] = diff;

      } // r
    } // theta
  } // phi

  std::cout<<"                                              \r" <<std::flush;

  // Write to file

  std::FILE *out = fopen(fullPath.c_str(), "w");
  if (!out) {
    std::cerr << "Failed to open " << fullPath << " for writing" << std::endl;
    return false;
  }

  fwrite(reinterpret_cast<void*>(&data_[0]),
                                 static_cast<size_t>(numVoxelsPerTimestep_)*
                                 sizeof(float),
                                 1,
                                 out);
  fclose(out);

  return true;
}
