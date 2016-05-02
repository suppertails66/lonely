#ifndef ADDRESSTABLEREADER_H
#define ADDRESSTABLEREADER_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tarray.h"
#include "util/ByteConversion.h"

namespace Lonely {


class AddressTableReader {
public:
  static void readAddressTable(Tarray<Taddress>& dst,
                               const Tbyte* src,
                               int numEntries,
                               int bytesPerAddress = ByteSizes::uint16Size,
                               EndiannessTypes::EndiannessType
                                 endianness = EndiannessTypes::little);
  
  static void writeAddressTable(Tbyte* dst,
                                const Tarray<Taddress>& src,
                                int bytesPerAddress = ByteSizes::uint16Size,
                                EndiannessTypes::EndiannessType
                                  endianness = EndiannessTypes::little);
protected:
  
};


};


#endif
