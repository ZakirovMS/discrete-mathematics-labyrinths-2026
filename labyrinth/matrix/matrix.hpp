#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <cstdlib>
#include <vector>
#include <iostream>
#include <list>
#include "node.hpp"
#include "caveComponent.hpp"
#include "wrapper.hpp"

namespace zakirov
{
  class Matrix
  {
  public:
    Matrix() = default;
    void setScopes(std::pair< size_t, size_t > scope);
    std::pair< size_t, size_t > getScopes();
    void setEntry(std::pair< size_t, size_t > entry);
    std::pair< size_t, size_t > getEntry();
    void setConnect(char connect);
    std::list< std::pair < char, size_t > > & getConnect();
    void setField(ArrayWrapper< CaveComponent > & field);
    ArrayWrapper< CaveComponent > & getField();
    void waveAlgo();
    void writePath(char to);
    void showMatrix();
    void clearMetadata();
    void showTechInfo();
  private:
    bool expandWave(std::vector< std::pair< size_t, size_t > > & from, std::vector< std::pair< size_t, size_t > > & to);
    std::pair< size_t, size_t > scopes_;
    std::pair< size_t, size_t > entry_;
    ArrayWrapper< CaveComponent > field_;
    std::list< std::pair < char, size_t > > ties_;
  };

  std::istream & operator>>(std::istream & in, Matrix & layer_c);
}

#endif
