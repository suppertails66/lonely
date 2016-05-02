#ifndef LOADHELPER_H
#define LOADHELPER_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/DataChunkID.h"
#include "util/ByteConversion.h"
#include <string>

namespace Lonely {


class LoadHelper {
public:
  /**
   * Constructor.
   * Reads the chunk header.
   * @param data Raw byte array to load from.
   */
  LoadHelper(const Tbyte* data);

  /**
   * Constructor.
   * Reads the chunk header.
   * @param data Raw byte array to load from.
   * @param byteCount Reference to count of bytes read so far. This is
   * automatically increased by the size of the header.
   */
  LoadHelper(const Tbyte* data,
             int& byteCount);
             
  DataChunkIDs::DataChunkID id() const;
  
  int version() const;
  
  int chunkSize() const;
  
  int fullSize() const;
  
  void readHeader(const Tbyte* data);
  
  void readHeader(const Tbyte* data,
                  int& byteCount);
  
  /**
   * Loads a string from data.
   */
  static int loadString(const Tbyte* data,
                        std::string& dst);
  
  /**
   * Loads an int from data.
   */
  static int loadInt(const Tbyte* data,
                     int& byteCount,
                     int size,
                     SignednessTypes::SignednessType sign
                      = SignednessTypes::nosign);
protected:
  const static int headerSize_ = 8;
  
  DataChunkIDs::DataChunkID id_;
  
  int version_;
  
  int size_;
  
};


};


#endif
