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
  // maybe make a "Volume" class with different iterators
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
 
  unsigned int rDim = dataObject_->XDim();
	unsigned int thetaDim = dataObject_->YDim();
	unsigned int phiDim = dataObject_->ZDim();
  unsigned int offset = dataObject_->TimestepOffset(_timestep);

  for (unsigned int phi=0; phi<phiDim; phi++) {

		unsigned int progress;
		progress = (unsigned int)(((float)phi/(float)phiDim)*100.f);
		if (progress % 10 == 0 ) {
			std::cout << "Processing... " << progress <<  "%\r" << std::flush;
		}

		for (unsigned int theta=0; theta<thetaDim; theta++) {
			for (unsigned int r=0; r<rDim; r++) {

				// Calculate array index
				unsigned int index = offset + r + theta*rDim + phi*rDim*thetaDim;
				
				// Put r in the [0..sqrt(3)] range
				float rNorm = sqrt(3.0)*(float)r/(float)(rDim-1);

				// Put theta in the [0..PI] range
				float thetaNorm = M_PI*(float)theta/(float)(thetaDim-1);

				// Put phi in the [0..2PI] range
				float phiNorm = 2.0*M_PI*(float)phi/(float)(phiDim-1);

        // Go to physical coordinates before sampling
        float rPh = rMin + rNorm*(rMax-rMin);
        float thetaPh = thetaNorm;
				// phi range needs to be mapped to the slightly different
				// model range to avoid gaps in the data
        float phiPh = phiMin + phiNorm/(2.0*M_PI)*(phiMax-phiMin);

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
					diff = rho - 10.f*rho_back;
        }
				// Update max/min
				if (diff > max) max = diff;
				else if (diff < min) min = diff;
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
