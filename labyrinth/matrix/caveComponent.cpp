#include "caveComponent.hpp"

char Labyrinth::CaveComponent::getCell() const noexcept
{
  return cell_;
}

void Labyrinth::CaveComponent::setCell(char cell) noexcept
{
  cell_ = cell;
}

size_t Labyrinth::CaveComponent::getDist()
{
  return dist_;
}

void Labyrinth::CaveComponent::setDist(size_t dist)
{
  dist_ = dist;
}

void Labyrinth::CaveComponent::visitCell()
{
  visit_ = true;
}

bool Labyrinth::CaveComponent::isVisited()
{
  return visit_;
}

bool Labyrinth::CaveComponent::isWall() const noexcept
{
  return cell_ == '#';
}

bool Labyrinth::CaveComponent::isPath() const noexcept
{
  return cell_ == '.';
}

bool Labyrinth::CaveComponent::isEntry() const noexcept
{
  return cell_ == '+';
}

bool Labyrinth::CaveComponent::isExit() const noexcept
{
  return (cell_ == '=' || (cell_ >= 'a' && cell_ <= 'z') || (cell_ >= 'A' && cell_ <= 'Z'));
}

bool Labyrinth::CaveComponent::isPassable() const noexcept
{
  return isExit() || isPath() || isEntry();
}

bool Labyrinth::CaveComponent::isElement(char symbol) const noexcept
{
  return symbol == cell_;
}
