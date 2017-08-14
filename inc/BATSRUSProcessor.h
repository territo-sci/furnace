/*
 * Author: Victor Sand (victor.sand@gmail.com)
 * Example implementation of a VolumeReader
 * Reads BATSRUS data from CDF files
 *
 */

#ifndef BATSRUSPROCESSOR_H_
#define BATSRUSPROCESSOR_H_

#include <VolumeProcessor.h>

namespace ccmc {
class Kameleon;
class Interpolator;
}

namespace osp {

class BATSRUSProcessor : public VolumeProcessor {
public:
    // Initializes Kameleon and Interpolator objects, returns BATSRUSProcessor
    static BATSRUSProcessor *New();

    ~BATSRUSProcessor();

    // Process a folder with one CDF file per timestep
    virtual bool ProcessFolder(const std::string &_sourceFolder, const std::string &_destFolder);

private:
    BATSRUSProcessor();

    BATSRUSProcessor(ccmc::Kameleon *_kameleon);

    BATSRUSProcessor(const BATSRUSProcessor &);

    // Process timesteps from CDF files and write result to new file
    bool ProcessFile(const std::string &_filename,
                     const std::string &_destFolder,
                     size_t _timestep);

    // CCMC parts
    ccmc::Kameleon *kameleon_;
    // Interpolator can't be created until file has been opened!
    ccmc::Interpolator *interpolator_;

};

} // namespace osp

#endif


