/*
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 */

#ifndef FURNACE_H_
#define FURNACE_H_

#include <string>

namespace osp {

class VolumeReader;

class Furnace {
public:
  static Furnace * New();
  ~Furnace();
  
  // List all supported model types
  enum ModelType {
    NO_MODEL = 0,
    ENLIL
  };

  // Set model type and create a corresponding VolumeReader subclass instance
  bool SetModelType(ModelType _modelType);
  // Setters
  void SetDimensions(unsigned int _xDim,
                     unsigned int _yDim,
                     unsigned int _zDim);
  void SetSourceFolder(const std::string &_sourceFolder);
  void SetOutFilename(const std::string &_outFilename);
  // Read input folder using VolumeReader instance
  bool ReadFolder();
  // Write read variables to output using VolumeReader instance
  bool Write();

private:
  Furnace();
  Furnace(const Furnace&);
  
  // Paths
  std::string sourceFolder_;
  std::string outFilename_;

  // State varibles
  bool hasRead_;

  // Instance of reader (set by SetModelType() function)
  VolumeReader *volumeReader_;

};

} // namespace

#endif
