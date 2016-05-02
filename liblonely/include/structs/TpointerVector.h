#ifndef TPOINTERVECTOR_H
#define TPOINTERVECTOR_H


#include "exception/OutOfRangeIndexException.h"
#include <vector>
#include <iostream>

namespace Lonely {


template <class T>
class TpointerVector {
public:
  TpointerVector<T>()
    : data_() { };
  
  TpointerVector<T>(unsigned int size__)
    : data_() {
    resize(size__);
  }
  
  ~TpointerVector<T>() {
//    deleteData();
  }
  
  TpointerVector<T>(const TpointerVector<T>& t) {
    copy(t);
  }
  
  TpointerVector<T>& operator=(const TpointerVector<T>& t) {
    copy(t);
    return *this;
  }
  
  unsigned int size() const {
    return data_.size();
  }
  
  void resize(unsigned int newSize) {
//    deleteData();
    data_.resize(newSize);
    data_.assign(newSize, NULL);
  }
  
  void set(unsigned int index,
           T* val) {
    if (index >= data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerVector::set()",
                                     index);
    }
  
    data_[index] = val;
  }
  
  void erase(int startIndex, int endIndex) {
    for (int i = startIndex; i < endIndex; i++) {
      deleteElement(i);
    }
    
    data_.erase(data_.begin() + startIndex, data_.begin() + endIndex);
  }
  
  T* operator[](unsigned int index) {
    if (index >= data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerVector::operator[](unsigned"
                                     " int)",
                                     index);
    }
  
    return data_[index];
  }
  
  const T* operator[](unsigned int index) const {
    if (index >= data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerVector::operator[](unsigned"
                                     " int) const",
                                     index);
    }
  
    return data_[index];
  }
  
  void insert(int position, T* item) {
    if (position > data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerVector::insert(int, T*)",
                                     position);
    }
    
    data_.insert(data_.begin() + position, item);
  }
  
  void insert(int position, int num, T* item) {
/*  std::cout << "num: " << num << std::endl;
    for (int i = 0; i < num; i++) {
      data_.insert(data_.begin() + position, item);
      std::cout << i << std::endl;
    } */
    if (position > data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerVector::insert(int, int, T*)",
                                     position);
    }
    
    data_.insert(data_.begin() + position, num, item);
  }
  
  void deleteElement(int i) {
    if (i >= data_.size()) {
      throw OutOfRangeIndexException(TALES_SRCANDLINE,
                                     "TpointerVector::deleteElement()",
                                     i);
    }
    
    delete data_[i];
    data_[i] = NULL;
  }
  
  void deleteData() {
    for (unsigned int i = 0; i < data_.size(); i++) {
      delete data_[i];
      data_[i] = NULL;
    }
  }
  
  void push_back(const T* t) {
    data_.push_back(t);
  }
  
  void push_back(T* t) {
    data_.push_back(t);
  }
  
  void clear() {
//    deleteData();
    data_.clear();
  }
  
protected:
  void copy(const TpointerVector<T>& t) {
//    deleteData();
    data_.resize(t.data_.size());
    for (unsigned int i = 0; i < t.data_.size(); i++) {
//      data_[i] = new T(t.data_[i]);
      data_[i] = t.data_[i];
    }
  }
  
  std::vector<T*> data_;
};


};


#endif
