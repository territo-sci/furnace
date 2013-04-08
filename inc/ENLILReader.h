/*
 * ENLILReader.h
 *
 *  Created on: Apr 5, 2013
 *      Author: Victor Sand
 */

#ifndef ENLILREADER_H_
#define ENLILREADER_H_

#include <CDFReader.h>

namespace osp
{

class ENLILReader : public CDFReader
{
public:
  static ENLILReader * New();
  virtual bool Read();
  virtual ~ENLILReader();
private:
  ENLILReader();
};

}

#endif
