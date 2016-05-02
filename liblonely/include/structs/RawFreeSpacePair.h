#ifndef RAWFREESPACEPAIR_H
#define RAWFREESPACEPAIR_H


namespace Lonely {


/**
 * Simple (address, length) pairs used for a FreeSpaceListing constructor.
 */
struct RawFreeSpacePair {
  int address;
  int length;
};


};


#endif 
