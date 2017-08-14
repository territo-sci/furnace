/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 */

#ifndef FURNACE_H_
#define FURNACE_H_

#include <string>
#include <unordered_map>

namespace osp {

class VolumeProcessor;

class Furnace {
public:
    // List all supported model types
    enum ModelType {
        ENLIL = 0,
        ML,
        BATSRUS,
        NO_MODEL
    };

    // List all supported grip types
    enum GridType {
        CARTESIAN = 0,
        SPHERICAL = 1,
        NO_GRID
    };

    static const std::unordered_map<std::string, ModelType> MODEL_TYPES;

    static Furnace *New();
    ~Furnace();

    // Set model type and create a corresponding VolumeProcessor subclass instance
    bool SetModelType(ModelType _modelType);

    // Set grid type
    // Only affects output variable, not reading itself unless implemented
    void SetGridType(GridType _gridType);

    // Setters
    void SetDimensions(size_t _xDim, size_t _yDim, size_t _zDim);
    void SetSourceFolder(const std::string &_sourceFolder);
    void SetDestFolder(const std::string &_destFolder);

    // Read input and write to output using VolumeProcessor instance
    bool ProcessFolder();

private:
    Furnace();

    Furnace(const Furnace &);

    std::string sourceFolder_;
    std::string destFolder_;

    // Instance of reader (set by SetModelType() function)
    VolumeProcessor *volumeProcessor_;

};

} // namespace

#endif
