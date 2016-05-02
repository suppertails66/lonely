#ifndef TPOINTERARRAY_H
#define TPOINTERARRAY_H


#include "structs/Tarray.h"
#include "exception/OutOfRangeIndexException.h"
#include <cstdlib>

namespace Lonely {


template <class T>
class TpointerArray {
public:
  TpointerArray<T>()
    : data_() {
    
  }
  
  TpointerArray<T>(int size__)
    : data_() {
    resize(size__);
  }
  
  ~TpointerArray<T>() {
//    deleteData();
  }
  
  TpointerArray<T>(const TpointerArray<T>& t) {
    copy(t);
  }
  
  TpointerArray<T>& operator=(const TpointerArray<T>& t) {
    copy(t);
  }
  
  unsigned int size() const {
    return data_.size();
  }
  
  void resize(unsigned int newSize) {
//    deleteData();
    data_.resize(newSize);
    data_.assign(newSize, NULL);
  }
  
  T* operator[](unsigned int index) {
    if (index >= data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerArray::operator[](unsigned int)",
                                     index);
    }
  
    return data_[index];
  }
  
  const T* operator[](unsigned int index) const {
    if (index >= data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerArray::operator[](unsigned int)"
                                     " const",
                                     index);
    }
  
    return data_[index];
  }
  
  void deleteData() {
    for (unsigned int i = 0; i < data_.size(); i++) {
      delete data_[i];
    }
  }
  
protected:
  void copy(const TpointerArray<T>& t) {
//    deleteData();
    data_.resize(t.data_.size());
    for (unsigned int i = 0; i < t.data_.size(); i++) {
      data_[i] = t.data_[i];
    }
  }

  Tarray<T*> data_;
};


};


#endif
