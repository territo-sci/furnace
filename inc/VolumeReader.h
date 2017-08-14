/*
 * Author: Victor Sand (victor.sand@gmail.com);
 *
 * Base class for reading (and later writing) volume data. The function
 * ReadFolder() needs to be implemented in subclasses, and the standard Write()
 * function may be overridden if specializations are needed in the output
 * file format. 
 * 
 * ReadFolder() reads a folder with one or more files of source data.
 *
 * Note that this class also defines the file format!
 *
 */

#ifndef VOLUMEREADER_H_
#define VOLUMEREADER_H_

#include <string>
#include <vector>

namespace osp {

class VolumeReader {
public:

    virtual ~VolumeReader();
    virtual bool ProcessFolder(const std::string &_sourceFolder, const std::string &_destFolder) = 0;
    void SetDimensions(size_t _xDim, size_t _yDim, size_t _zDim);

    // 0 for cartesian
    // 1 for spherical
    void SetGridType(size_t _gridType);

protected:
    VolumeReader();
    VolumeReader(const VolumeReader &);

    // Write (temporary) header
    bool WriteHeader(const std::string &_destFolder);

    // Gather values from temporary files, normalize and write
    // to single output file
    bool WriteFinal(const std::string &_destFolder);

    // Delete temporary files
    bool DeleteTempFiles(const std::string &_destFolder);

    // Names for (temporary) header and timestep files
    const std::string tempSuffix_ = ".tmp";
    const std::string headerFilename_ = "header";
    const std::string headerSuffix_ = ".tmp";
    const std::string timestepFilename_ = "timestep_";
    const std::string timestepSuffix_ = ".tmp";

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
} // namespace osp

#endif
