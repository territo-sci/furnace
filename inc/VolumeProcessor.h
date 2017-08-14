/*
 * Author: Victor Sand (victor.sand@gmail.com);
 *
 * Base class for reading and writing volume data. The function
 * ProcessFolder() needs to be implemented in subclasses, and the standard 
 * Write() function may be overridden if specializations are needed.
 * 
 * ProcessFolder() reads a folder with one or more files of source data.
 *
 * Note that this class also defines the file format!
 *
 */

#ifndef VOLUMEPROCESSOR_H_
#define VOLUMEPROCESSOR_H_

#include <string>
#include <vector>

#define _FILE_OFFSET_BITS 64
#define off off64_t

namespace boost {
namespace filesystem {
class path;
} //namespace filesystem
} //namespace boost

namespace osp {

class VolumeProcessor {
public:
    virtual ~VolumeProcessor();
    virtual bool ProcessFolder(const std::string &_sourceFolder, const std::string &_destFolder) = 0;
    void SetDimensions(size_t _xDim, size_t _yDim, size_t _zDim);

    // 0 for cartesian
    // 1 for spherical
    void SetGridType(size_t _gridType);

protected:
    VolumeProcessor();
    VolumeProcessor(const VolumeProcessor &);

    // Write (temporary) header
    bool WriteHeader(const std::string &_destFolder);

    // Gather values from temporary files, normalize and write
    // to single output file
    bool WriteFinal(const std::string &_destFolder);

    // Delete temporary files
    bool DeleteTempFiles(const std::string &_destFolder);
    bool testSuffix(boost::filesystem::path path, const char *suffix);

    // Names for (temporary) header and timestep files
    const std::string tempSuffix_ = ".tmp";
    const std::string headerFilename_ = "header";
    const std::string headerSuffix_ = ".tmp";
    const std::string timestepFilename_ = "timestep_";
    const std::string timestepSuffix_ = ".tmp";
    const char *const cdfSuffix = ".cdf";

    // Name for final file
    const std::string finalFilename = "volume.vdf";

    // Data to write
    size_t gridType_;
    size_t numTimesteps_;
    size_t xDim_;
    size_t yDim_;
    size_t zDim_;

    // Stores data in one time step before writing to disk
    std::vector<float> data_;

    // Num voxels for one frame
    size_t numVoxelsPerTimestep_;

    // For normalization step
    float min_;
    float max_;

    // State
    bool hasRead_;
};
} // namespace

#endif
