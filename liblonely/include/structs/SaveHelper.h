#ifndef SAVEHELPER_H
#define SAVEHELPER_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/DataChunkID.h"
#include "util/ByteConversion.h"
#include <string>

namespace Lonely {


class SaveHelper {
public:
  /**
   * Constructor.
   * Writes the chunk header and saves necessary data for finalization.
   * @param data__ String to save to.
   * @param id ID of the chunk being saved.
   * @param version Version identifier for the chunk.
   */
  SaveHelper(std::string& data__,
             DataChunkIDs::DataChunkID id,
             int version);
  
  /**
   * Fills in length field of chunk header.
   * This should be called after writing all data to the string.
   * @return Total number of bytes written to saved chunk, including header.
   */
  int finalize() const;
                         
  /**
   * Returns the amount of data written to the save string
   * since the SaveHelper was instantiated.
   * @return Number of bytes of data written since SaveHelper was instantiated.
   */
  int bytesWritten() const;
  
  /**
   * Saves a string to a string.
   */
  static void saveString(std::string& data,
                         const std::string& saveString);
  
  /**
   * Saves an int to a string.
   */
  static int saveInt(std::string& data,
                      int value,
                      int size,
                      SignednessTypes::SignednessType sign
                        = SignednessTypes::nosign);
  
protected:
  const static int saveBufferSize_ = 16;

  const static int headerSize_ = 8;
  
  std::string& data_;
  
  int startPos_;
  
};


}


#endif
