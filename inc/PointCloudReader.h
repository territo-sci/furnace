/* 
 * Author: Victor Sand (victor.sand@gmail.com)
 *
 */
 
#ifndef POINTCLOUDREADER_H_
#define POINTCLOUDREADER_H_

#include <VolumeReader.h>

namespace osp {
  
class PointCloudReader : public VolumeReader {
public:
  static PointCloudReader * New();
  ~PointCloudReader();
  
  // Reads a folder (which will only include a single file)
  virtual bool ReadFolder(const std::string &_sourceFolder);
  
private:
  PointCloudReader();
  PointCloudReader(const PointCloudReader&);

  // Path to the single file that should be present in the folder
  std::string fileName_;

  unsigned int numParticlesPerTimestep_;
  float timestepDt_;

  
};

}
 
 
#endif 
