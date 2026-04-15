#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <cstdlib>
#include <vector>
#include <iostream>
#include <list>
#include "../directedGraph/node.hpp"
#include "../directedGraph/caveComponent.hpp"
#include "wrapper.hpp"

namespace Labyrinth
{
  class Matrix
  {
  public:
    Matrix() = default;
    void setName(const std::string& name) { name_ = name; }
    std::string getName() const { return name_; }

    void setScopes(std::pair< size_t, size_t > scope);
    std::pair< size_t, size_t > getScopes();
    void setEntry(std::pair< size_t, size_t > entry);
    std::pair< size_t, size_t > getEntry();
    void setConnect(char connect);
    std::list< std::pair < char, size_t > > & getConnect();
    void setField(ArrayWrapper< CaveComponent > & field);
    ArrayWrapper< CaveComponent > & getField();

    void generateKruskalMaze(size_t grid_rows, size_t grid_cols, char entry_sym, std::vector<char> exits_sym);

    void waveAlgo();
    void showMatrix();
    void clearMetadata();
    void showTechInfo();

    void writePath(char to);
  private:
    bool expandWave(std::vector< std::pair< size_t, size_t > > & from, std::vector< std::pair< size_t, size_t > > & to);
    std::pair< size_t, size_t > expandCell(size_t row, size_t col);
    std::pair< size_t, size_t > scopes_;
    std::pair< size_t, size_t > entry_;
    ArrayWrapper< CaveComponent > field_;
    std::list< std::pair < char, size_t > > ties_;
    std::string name_;
  };

  std::istream & operator>>(std::istream & in, Matrix & layer_c);
}

#endif
