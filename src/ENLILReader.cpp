/*
 * ENLILReader.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: vsand
 */

// TODO OpenMP parallelization

#include <ENLILReader.h>
#include <VDFData.h>
#include <ccmc/Kameleon.h>
#include <iostream>

using namespace osp;

ENLILReader * ENLILReader::New() {
  return new ENLILReader();
}

ENLILReader::~ENLILReader() { }

ENLILReader::ENLILReader() : CDFReader() { }

bool ENLILReader::ReadFile(const std::string &_filename, 
                           unsigned int _timestep) {

  // TODO hardcoded variables and difference until proper CDF files arrives
  variableNames_.push_back("rho");
  variableNames_.push_back("rho-back");

	// Open file using Kameleon instance (from CDFReader base class)
  if (kameleon_->open(_filename) != ccmc::FileReader::OK) {
    std::cout << "Failed to open " << _filename << "\n";
    return false;
  }
 
  // Interpolator has to be created after opening file
	if (interpolator_ != NULL) delete interpolator_;
	interpolator_ = kameleon_->createNewInterpolator();

  // Mins and maxes from the model
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

  // TODO loop over the volume in different orders
  // maybe make a "Volume" class with different iterators?

	std::cout << "r:   [" << rMin << ", " << rMax << "]\n"; 
	std::cout << "theta: [" << thetaMin << ", " << thetaMax << "]\n";
	std::cout << "phi:   [" << phiMin << ", " << phiMax << "]\n";

  // Keep track of min and max values for interpolation
  // TODO compare how well actual_min and actual_max works for this
  std::vector<float> *data = dataObject_->Data();
	
	std::cout << "Reading timestep " << _timestep+1 << 
	             "/" << dataObject_->NumTimesteps() << "\n";

  float min = 1e20f;
  float max = -1e20f;
  // Loop over voxels in the grid that is about to be filled
  for (unsigned int z=0; z<dataObject_->ZDim(); z++) {
    unsigned int progress =
        (unsigned int)(((float)z/(float)dataObject_->ZDim())*100.f);
    if (progress % 10 == 0)
      std::cout << "Processing... " << progress << "%\r" << std::flush;
    for (unsigned int y=0; y<dataObject_->ZDim(); y++) {
      for (unsigned int x=0; x<dataObject_->XDim(); x++) {
        int index =
						dataObject_->TimestepOffset(_timestep) +
            x +
            y*dataObject_->XDim() +
            z*dataObject_->XDim()*dataObject_->YDim();

        // Calculate normalized coordinates [-1, 1]
        float xNorm = -1.f + 2.f*(float)x/(float)(dataObject_->XDim()-1);
        float yNorm = -1.f + 2.f*(float)y/(float)(dataObject_->YDim()-1);
        float zNorm = -1.f + 2.f*(float)z/(float)(dataObject_->ZDim()-1);

        // Convert to spherical coordinates
        float r = sqrt(xNorm*xNorm+yNorm*yNorm+zNorm*zNorm);
        float theta, phi;
        if (r == 0.f) {
          theta = phi = 0.f;
        } else {
          theta = acos(zNorm/r);
          phi = atan2(yNorm, xNorm);
        }

        while (phi < 0.f) phi += M_PI*2.f;

        // Go to physical coordinates before sampling
        float rPh = rMin + r*(rMax-rMin);
        float thetaPh = theta;
        float phiPh = phi;

        // TODO hardcoded values
        float rho, rho_back;
        float diff = 0.f;
        // See if sample point is inside domain
        if (rPh < rMin || rPh > rMax || thetaPh < thetaMin ||
            thetaPh > thetaMax || phiPh < phiMin || phiPh > phiMax) {
          rho = rho_back = 0.f;
        } else {
          // ENLIL CDF specific hacks!
          // Convert from meters to AU for interpolator
          rPh /= ccmc::constants::AU_in_meters;
          // Convert from colatitude [0, pi] rad to latitude [-90, 90] degrees
          thetaPh = -thetaPh*180.f/M_PI+90.f;
          // Convert from [0, 2pi] rad to [0, 360] degrees
          phiPh = phiPh*180.f/M_PI;
          // Sample
          rho = interpolator_->interpolate(variableNames_[0],
                                          rPh,
                                          thetaPh,
                                          phiPh);
          rho_back = interpolator_->interpolate(variableNames_[1],
                                               rPh,
                                               thetaPh,
                                               phiPh);
          // Difference with a magic number scalar
          // TODO update when proper CDF comes around
					diff = rho - 1.f*rho_back;
          // Update max/min
          if (diff > max) max = diff;
          else if (diff < min) min = diff;
        }
        // Save value
        data->at(index) = diff;
      }
    }
  }

  dataObject_->SetMin(min);
	dataObject_->SetMax(max);

  std::cout << "Processing... 100%\n";

  hasRead_ = true;

  return true;
}
