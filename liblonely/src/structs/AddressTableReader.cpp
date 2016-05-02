#include "structs/AddressTableReader.h"

namespace Lonely {


void AddressTableReader::readAddressTable(Tarray<Taddress>& dst,
                               const Tbyte* src,
                               int numEntries,
                               int bytesPerAddress,
                               EndiannessTypes::EndiannessType
                                 endianness) {
  if (dst.size() != numEntries) {
    dst.resize(numEntries);
  }
  
  for (int i = 0; i < numEntries; i++) {
    dst[i] = ByteConversion::fromBytes(
                    src,
                    bytesPerAddress,
                    endianness,
                    SignednessTypes::nosign);
    
    src += bytesPerAddress;
  }
}
  
void AddressTableReader::writeAddressTable(Tbyte* dst,
                              const Tarray<Taddress>& src,
                              int bytesPerAddress,
                              EndiannessTypes::EndiannessType
                                endianness) {
  for (int i = 0; i < src.size(); i++) {
    ByteConversion::toBytes(src[i],
                            dst,
                            bytesPerAddress,
                            endianness,
                            SignednessTypes::nosign);
    
    dst += bytesPerAddress;
  }
}


};
