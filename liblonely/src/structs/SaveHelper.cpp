#include "structs/SaveHelper.h"
#include "util/ByteConversion.h"
#include "util/ByteSizes.h"
#include <iostream>

using namespace Lonely;

namespace Lonely {


SaveHelper::SaveHelper(std::string& data__,
             DataChunkIDs::DataChunkID id,
             int version)
  : data_(data__),
    startPos_(data_.size() + 8) {
  Tbyte buffer[ByteSizes::uint32Size];
  
  // Write type ID
  ByteConversion::toBytes(static_cast<int>(id),
                          buffer,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  data_ += std::string((char*)buffer, ByteSizes::uint16Size);
  
  // Write version
  ByteConversion::toBytes(version,
                          buffer,
                          ByteSizes::uint16Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  data_ += std::string((char*)buffer, ByteSizes::uint16Size);
  
  // Reserve 4 bytes for length
  data_ += "    ";
}

int SaveHelper::finalize() const {
  Tbyte buffer[ByteSizes::uint32Size];
  
  // Calculate chunk size (minus header)
  ByteConversion::toBytes(bytesWritten(),
                          buffer,
                          ByteSizes::uint32Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  
  // Write length
  for (int i = 0; i < ByteSizes::uint32Size; i++) {
    data_[startPos_ - 4 + i] = buffer[i];
  }
  
  return bytesWritten() + headerSize_;
}

void SaveHelper::saveString(
                       std::string& data,
                       const std::string& saveString) {
  Tbyte buffer[ByteSizes::uint32Size];
  
  // Write length
  ByteConversion::toBytes(saveString.size(),
                          buffer,
                          ByteSizes::uint32Size,
                          EndiannessTypes::little,
                          SignednessTypes::nosign);
  data += std::string((char*)buffer, ByteSizes::uint32Size);
  
  // Write string
  data += saveString;
                          
}

int SaveHelper::saveInt(std::string& data,
                    int value,
                    int size,
                    SignednessTypes::SignednessType sign) {
  Tbyte buffer[saveBufferSize_];
  
  ByteConversion::toBytes(value,
                          buffer,
                          size,
                          EndiannessTypes::little,
                          sign);
  data += std::string((char*)(buffer), size);
  
  return size;
}

int SaveHelper::bytesWritten() const {
  return data_.size() - startPos_;
}


};
