#ifndef TVECTOR_H
#define TVECTOR_H


#include <vector>
#include <memory>

namespace Lonely {


template < class T, class Alloc = std::allocator<T> >
class Tvector : public std::vector<T, Alloc> {
public:
  explicit Tvector(
    const typename std::vector<T, Alloc>::allocator_type& alloc
                      = std::vector<T, Alloc>::allocator_type())
    : std::vector<T, Alloc>(alloc) { };
    
  explicit Tvector(typename std::vector<T, Alloc>::size_type n,
    const typename std::vector<T, Alloc>::value_type& val
      = std::vector<T, Alloc>::value_type(),
    const typename std::vector<T, Alloc>::allocator_type& alloc
                    = std::vector<T, Alloc>::allocator_type())
    : std::vector<T, Alloc>(n, val, alloc) { };
    
  template <class InputIterator>
         Tvector (InputIterator first, InputIterator last,
    const typename std::vector<T, Alloc>::allocator_type& alloc
                      = std::vector<T, Alloc>::allocator_type())
    : std::vector<T, Alloc>(first, last, alloc) { };
    
  Tvector (const Tvector& x)
    : std::vector<T, Alloc>(x) { };
protected:
  
};


};


#endif
