#include "matrix.hpp"
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <functional>

namespace
{

}

void zakirov::Matrix::clearMetadata()
{
  for (auto i = ties_.begin(); i != ties_.end(); ++i)
  {
    i->second = std::numeric_limits< size_t >::max();
  }

  for (size_t i = 0; i < scopes_.first; ++i)
  {
    for (size_t j = 0; j < scopes_.second; ++j)
    {
      field_(i, j).setDist(std::numeric_limits< size_t >::max());
    }
  }
}

//Get rid of duplication
bool zakirov::Matrix::expandWave(std::vector< std::pair < size_t, size_t > > & from, std::vector< std::pair < size_t, size_t > > & to)
{
  bool flag = true;
  bool returnable = false;
  std::vector< std::pair < size_t, size_t > > stub;

  for (size_t i = 0; i < from.size(); ++i)
  {
    size_t row = from[i].first;
    size_t col = from[i].second;
    if (row != 0)
    {
      if (field_(row - 1, col).isPassable() && !field_(row - 1, col).isVisited())
      {
        stub.push_back(std::make_pair(row - 1, col));
        field_(row - 1, col).visitCell();
        flag = false;
      }
    }
    if (row != scopes_.first - 1)
    {
      if (field_(row + 1, col).isPassable() && !field_(row + 1, col).isVisited())
      {
        stub.push_back(std::make_pair(row + 1, col));
        field_(row + 1, col).visitCell();
        flag = false;
      }
    }
    if (col != 0)
    {
      if (field_(row, col - 1).isPassable() && !field_(row, col - 1).isVisited())
      {
        stub.push_back(std::make_pair(row, col - 1));
        field_(row, col - 1).visitCell();
        flag = false;
      }
    }
    if (col != scopes_.second - 1)
    {
      if (field_(row, col + 1).isPassable() && !field_(row, col + 1).isVisited())
      {
        stub.push_back(std::make_pair(row, col + 1));
        field_(row, col + 1).visitCell();
        flag = false;
      }
    }
    if (flag)
    {
      stub.push_back(std::make_pair(from[i].first, from[i].second));
    }
    else
    {
      flag = true;
      returnable = true;
    }
  }

  //tech remove
  for (size_t i = 0; i < stub.size(); ++i)
  {
    std::cout << '(' << stub[i].first << "; " << stub[i].second << ") ";
  }
  std::cout << '\n';

  std::swap(stub, to);
  return returnable;
}

void zakirov::Matrix::setScopes(std::pair< size_t, size_t > scope)
{
  scopes_ = scope;
}

std::pair< size_t, size_t > zakirov::Matrix::getScopes()
{
  return scopes_;
}

void zakirov::Matrix::setEntry(std::pair< size_t, size_t > entry)
{
  entry_ = entry;
}

std::pair< size_t, size_t > zakirov::Matrix::getEntry()
{
  return entry_;
}

void zakirov::Matrix::setConnect(char connect)
{
  ties_.push_back(std::make_pair(connect, std::numeric_limits< size_t >::max()));
}

std::list< std::pair < char, size_t > > & zakirov::Matrix::getConnect()
{
  return ties_;
}

void zakirov::Matrix::setField(ArrayWrapper< CaveComponent > & field)
{
  field_ = field;
}

zakirov::ArrayWrapper< zakirov::CaveComponent > & zakirov::Matrix::getField()
{
  return field_;
}
    
void zakirov::Matrix::showMatrix()
{
  std::cout << scopes_.first << ' ' << scopes_.second << '\n' << "connects:";
  for (auto i = ties_.begin(); i != ties_.end(); ++i)
  {
    std::cout << ' ' << i->first;
  }

  std::cout << '\n';
  if (scopes_.first == 0 || scopes_.second == 0)
  {
    return;
  }

  for (size_t i = 0; i < scopes_.first - 1; ++i)
  {
    std::cout << field_(i, 0).getCell();
    for (size_t j = 1; j < scopes_.second; ++j)
    {
      std::cout << ' ' << field_(i, j).getCell();
    }
    std::cout << '\n';
  }

  std::cout << field_(scopes_.first - 1, 0).getCell();
  for (size_t j = 1; j < scopes_.second; ++j)
  {
    std::cout << ' ' << field_(scopes_.first - 1, j).getCell();
  }

  std::cout << '\n';
}

std::istream & zakirov::operator>>(std::istream & in, zakirov::Matrix & layer)
{
  std::pair< size_t, size_t > scope;
  in >> scope.first >> scope.second;
  layer.setScopes(scope);
  ArrayWrapper< CaveComponent > field(scope.first, scope.second);
  char el;
  for (size_t i = 0; i < scope.first; ++i)
  {
    for (size_t j = 0; j < scope.second; ++j)
    {
      in >> el;
      if (el == '+')
      {
        layer.setEntry(std::make_pair(i, j));
      }
      else if ((el >= 'A' && el <= 'Z') || (el >= 'a' && el <= 'z') || (el == '='))
      {
        layer.setConnect(el);
      }

      field(i, j).setCell(el);
    }
  }

  layer.setField(field);
  return in;
}

void zakirov::Matrix::waveAlgo()
{
  size_t counter = 0;
  field_(entry_.first, entry_.second).setDist(counter);
  std::vector< std::pair < size_t, size_t > > wave, old_wave;
  old_wave.push_back(entry_);
  while (expandWave(old_wave, wave))
  {
    counter += 1;
    for (size_t i = 0; i < wave.size(); ++i)
    {
      if (field_(wave[i].first, wave[i].second).getDist() == std::numeric_limits< size_t >::max())
      {
        field_(wave[i].first, wave[i].second).setDist(counter);
      }

      if (field_(wave[i].first, wave[i].second).isExit())
      {
        auto it = std::find(ties_.begin(), ties_.end(), std::make_pair(field_(wave[i].first, wave[i].second).getCell(), std::numeric_limits< size_t >::max()));
        if (it != ties_.end())
        {
          it->second = counter;
        }
      }
    }

    std::swap(wave, old_wave);
    wave.clear();
  }
}
/*
void zakirov::Matrix::writePath(char to)
{
  if (field_(entry_.first, entry_.second).getDist() == std::numeric_limits< size_t >::max())
  {
    throw std::logic_error("No pathfinding was performed");
  }

  if (std::find(field_.begin(), field_.end(), std::make_pair(to, std::numeric_limits< size_t >::max())) != field_.end())
  {
    throw std::logic_error("No way to target");
  }

  auto curr_pos = std::find_if(field_.begin(), field_.end(), std::bind(std::equal_to< char >(), std::bind(&CaveComponent::getCell, std::placeholders::_1), to));

}
*/

void zakirov::Matrix::showTechInfo()
{
  for (size_t i = 0; i < scopes_.first; ++i)
  {
    for (size_t j = 0; j < scopes_.second; ++j)
    {
      if (field_(i, j).getDist() == std::numeric_limits< size_t >::max())
      {
        std::cout << "# ";
      }
      else if (field_(i, j).getDist() < 10)
      {
        std::cout << field_(i, j).getDist() << ' ';
      }
      else
      {
        std::cout << field_(i, j).getDist();
      }
      std::cout << ' ';
    }

    std::cout << '\n';
  }
}
