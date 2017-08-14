/*
 * Author: Victor Sand (victor.sand@gmail.com)
 * Example implementation of a VolumeReader
 * Reads ENLIL data from CDF files
 *
 */

#ifndef ENLILPROCESSOR_H_
#define ENLILPROCESSOR_H_

#include <VolumeProcessor.h>

namespace ccmc {
class Kameleon;
class Interpolator;
}

namespace osp {

class ENLILProcessor : public VolumeProcessor {
public:
    struct AttributeObject {
        size_t r;
        float rMin;
        float rMax;
        size_t phi;
        float phiMin;
        float phiMax;
        size_t theta;
        float thetaMin;
        float thetaMax;
    };

    // Initializes Kameleon and Interpolator objects, returns ENLILProcessor
    static ENLILProcessor *New();

    ~ENLILProcessor();

    // Process a folder with one CDF file per timestep
    virtual bool ProcessFolder(const std::string &_sourceFolder,
                               const std::string &_destFolder);

private:
    ENLILProcessor();

    ENLILProcessor(ccmc::Kameleon *_kameleon);

    ENLILProcessor(const ENLILProcessor &);

    // Process timesteps from CDF files and write result to new file
    bool ProcessFile(const std::string &_filename,
                     const std::string &_destFolder,
                     size_t _timestep);

    bool fileWorker(AttributeObject &attr, ccmc::Interpolator *interpolator);

    // CCMC parts
    ccmc::Kameleon *kameleon_;
    // Interpolator can't be created until file has been opened!
    ccmc::Interpolator *interpolator_;

};

} // namespace osp

#endif


