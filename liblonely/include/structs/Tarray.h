#ifndef TARRAY_H
#define TARRAY_H


#include <cstring>
#include <cstdlib>
#include "structs/Tbyte.h"
#include "structs/Tstring.h"
#include "util/ByteConversion.h"

namespace Lonely {


/**
 * Generic not-a-vector array implementation.
 */
template <class T>
class Tarray {
public:
  Tarray<T>()
    : data_(NULL),
      size_(0) { };
  
  Tarray<T>(unsigned int size__)
    : data_(NULL),
      size_(0) {
    resize(size__);
  }
  
  Tarray<T>(unsigned int size__,
            const T& initValue)
    : data_(NULL),
      size_(0) {
    resize(size__);
    assign(initValue);
  }
  
  Tarray<T>(const Tarray<T>& t)
    : data_(NULL),
      size_(0) {
    copy(t);
  }
  
  Tarray<T>& operator=(const Tarray<T>& t) {
    copy(t);
    return *this;
  }
  
  ~Tarray<T>() {
    destroyData();
  }
    
  unsigned int size() const {
    return size_;
  }
  
  void clear() {
    destroyData();
    size_ = 0;
  }
  
  bool empty() const {
    return (size_ == 0);
  }
  
  void assign(const T& value) {
    for (int i = 0; i < size_; i++) {
      data_[i] = value;
    }
  }
  
  T* data() {
    return data_;
  }
  
  const T* data() const {
    return data_;
  }
  
  T& operator[](unsigned int index) {
    return data_[index];
  }
  
  const T& operator[](unsigned int index) const {
    return data_[index];
  }
  
  void resize(unsigned int size__) {
    destroyData();
    data_ = new T[size__];
    size_ = size__;
  }
  
  // only meant to work for Tarray<Tbyte>
  // you'd think template specialization was for this kind of stuff, but nooo
  int save(Tstring& dst) const {
    int byteCount = 0;
    
    Tbyte buffer[ByteSizes::uint32Size];
    
    ByteConversion::toBytes(size_,
                            buffer,
                            ByteSizes::uint32Size,
                            EndiannessTypes::little,
                            SignednessTypes::nosign);
    dst += Tstring((char*)(buffer), ByteSizes::uint32Size);
    byteCount += ByteSizes::uint32Size;
    
    for (int i = 0; i < size_; i++) {
      dst += (char)(data_[i]);
    }
    byteCount += size_;
    
    return byteCount;
  }
  
  // Tarray<Tbyte> only
  int load(const Tbyte* src) {
    int byteCount = 0;
    
    int dataSize = ByteConversion::fromBytes(src + byteCount,
                            ByteSizes::uint32Size,
                            EndiannessTypes::little,
                            SignednessTypes::nosign);
    byteCount += ByteSizes::uint32Size;
    
    resize(dataSize);
    byteCount += readFromData(src + byteCount);
    
    return byteCount;
  }
  
  // Tarray<Tbyte> only
  int readFromData(const Tbyte* src) const {
    std::memcpy(data_,
                src,
                size_);
    
    return size_;
  }
  
  // Tarray<Tbyte> only
  int writeToData(Tbyte* dst) const {
    std::memcpy(dst,
                data_,
                size_);
    
    return size_;
  }
protected:
  void copy(const Tarray<T>& t) {
    resize(t.size_);
    
    for (int i = 0; i < t.size_; i++) {
      data_[i] = t.data_[i];
    }
  }
  
  void destroyData() {
    delete[] data_;
    data_ = NULL;
  }

  T* data_;
  
  unsigned int size_;
};


};


#endif
