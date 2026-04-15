#ifndef WRAPPER_HPP
#define WRAPPER_HPP
#include <cstddef>
#include <vector>

namespace zakirov
{
  template< class T >
  struct ArrayWrapper
  {
    ArrayWrapper() = default;
    ArrayWrapper(size_t rows, size_t cols);
    T & operator()(size_t row, size_t col);

    typename std::vector< T >::iterator begin();
    typename std::vector< T >::iterator end();

    std::vector< T > data_;
    size_t rows_;
    size_t cols_;
  };

  template < class T >
  ArrayWrapper< T >::ArrayWrapper(size_t rows, size_t cols):
    data_(rows * cols),
    rows_(rows),
    cols_(cols)
  {}

  template < class T >
  T & ArrayWrapper< T >::operator()(size_t row, size_t col)
  {
    return data_[row * cols_ + col];
  }

  template < class T >
  typename std::vector< T >::iterator ArrayWrapper< T >::begin()
  {
    return data_.begin();
  }

  template < class T >
  typename std::vector< T >::iterator ArrayWrapper< T >::end()
  {
    return data_.end();
  }
}

#endif
