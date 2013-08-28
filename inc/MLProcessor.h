/* 
 * Author: Victor Sand (victor.sand@gmail.com)
 * Processor for a procedural Marschner-Lobb test data set
 */

#ifndef MLPROCESSOR_H_
#define MLPROCESSOR_H_

#include <VolumeProcessor.h>

namespace osp {

class MLProcessor : public VolumeProcessor {
public:
  static MLProcessor * New();
  ~MLProcessor();

  // Folder includes a single text file with constants for data set
  virtual bool ProcessFolder(const std::string &_sourceFolder,
                             const std::string &_destFolder);

private:
  MLProcessor();
  MLProcessor(const MLProcessor&);

};

} // namespace osp

#endif
