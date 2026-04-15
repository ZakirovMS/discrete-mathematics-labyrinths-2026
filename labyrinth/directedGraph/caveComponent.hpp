#ifndef CAVE_COMPONENTS_HPP
#define CAVE_COMPONENTS_HPP
#include <cstddef>
#include <limits>

namespace zakirov
{
  class CaveComponent
  {
  public:
    char getCell() const noexcept;
    void setCell(char cell) noexcept;
    size_t getDist();
    void setDist(size_t dist);
    void visitCell();
    bool isVisited();
    bool isWall() const noexcept;
    bool isPath() const noexcept;
    bool isEntry() const noexcept;
    bool isExit() const noexcept;
    bool isPassable() const noexcept;
    bool isElement(char symbol) const noexcept;
  private:
    char cell_;
    size_t dist_ = std::numeric_limits< size_t >::max();
    bool visit_ = false;
  };
}

#endif
