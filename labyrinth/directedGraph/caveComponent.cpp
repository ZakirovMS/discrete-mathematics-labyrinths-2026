#include "caveComponent.hpp"

char zakirov::CaveComponent::getCell() const noexcept
{
  return cell_;
}

void zakirov::CaveComponent::setCell(char cell) noexcept
{
  cell_ = cell;
}

size_t zakirov::CaveComponent::getDist()
{
  return dist_;
}

void zakirov::CaveComponent::setDist(size_t dist)
{
  dist_ = dist;
}

void zakirov::CaveComponent::visitCell()
{
  visit_ = true;
}

bool zakirov::CaveComponent::isVisited()
{
  return visit_;
}

bool zakirov::CaveComponent::isWall() const noexcept
{
  return cell_ == '#';
}

bool zakirov::CaveComponent::isPath() const noexcept
{
  return cell_ == '.';
}

bool zakirov::CaveComponent::isEntry() const noexcept
{
  return cell_ == '+';
}

bool zakirov::CaveComponent::isExit() const noexcept
{
  return (cell_ == '=' || (cell_ >= 'a' && cell_ <= 'z') || (cell_ >= 'A' && cell_ <= 'Z'));
}

bool zakirov::CaveComponent::isPassable() const noexcept
{
  return isExit() || isPath();
}

bool zakirov::CaveComponent::isElement(char symbol) const noexcept
{
  return symbol == cell_;
}
